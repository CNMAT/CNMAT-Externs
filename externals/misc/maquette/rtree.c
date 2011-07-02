#include "rtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

// rtree_search
// rtree_adjust_tree
// rtree_delete
// rtree_find_leaf
// rtree_contense_tree

#define MIN(v1, v2) (v1 < v2 ? (v1) : (v2))
#define MAX(v1, v2) (v1 > v2 ? (v1) : (v2))
#define AREA(r) ((r.xmax - r.xmin) * (r.ymax - r.ymin))
#define MBR(r1, r2) ((t_rtree_rect){MIN(r1.xmin, r2.xmin), MAX(r1.xmax, r2.xmax), MIN(r1.ymin, r2.ymin), MAX(r1.ymax, r2.ymax)})
//#define INSIDE(r1, r2) ((r2.xmin >= r1.xmin) && (r2.xmax <= r1.xmax) && (r2.ymin >= r1.ymin) && (r2.ymax <= r1.ymax))
//#define INSIDE(r1, r2) ((MIN(r1.xmax, r2.xmax) - MAX(r1.xmin, r2.xmin)) * (MIN(r1.ymax, r2.ymax) - MAX(r1.ymin, r2.ymin))) > 0
#define INSIDE(r1, r2) (((MIN(r1.xmax, r2.xmax) - MAX(r1.xmin, r2.xmin)) > 0) && ((MIN(r1.ymax, r2.ymax) - MAX(r1.ymin, r2.ymin)) > 0))

#ifdef RTREE_DEBUG
#define PP(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define PP(fmt, ...)
#endif

int rtree_isinside(t_rtree_rect r1, t_rtree_rect r2);
void rtree_split_node(t_rtree *tree, t_rtree_node *node, t_rtree_node **new1, t_rtree_node **new2);
void rtree_unlink(t_rtree_node *entry);
void rtree_unlink_entry(t_rtree_node *node, t_rtree_node *entry);
void rtree_add_entry(t_rtree_node *node, t_rtree_node *entry);
void rtree_shorten_tree(t_rtree *tree);

void rtree_resize_node(t_rtree_node *node){
	t_rtree_node *entry = node->entries;
	t_rtree_rect extrema = (t_rtree_rect){DBL_MAX, -DBL_MAX, DBL_MAX, -DBL_MAX};
	while(entry){
		if(entry->rect.xmin < extrema.xmin){
			extrema.xmin = entry->rect.xmin;
		}
		if(entry->rect.xmax > extrema.xmax){
			extrema.xmax = entry->rect.xmax;
		}
		if(entry->rect.ymin < extrema.ymin){
			extrema.ymin = entry->rect.ymin;
		}
		if(entry->rect.ymax > extrema.ymax){
			extrema.ymax = entry->rect.ymax;
		}
		entry = entry->next;
	}
	node->rect = extrema;
}

void rtree_adjust_tree(t_rtree *tree, t_rtree_node *node){
	t_rtree_node *n = node;
	while(n){
		t_rtree_node *parent = n->parent;
		t_rtree_rect extrema;
		t_rtree_node *entry = n->entries;
		if(!entry){
			extrema = n->rect;
		}else{
			extrema = entry->rect;
			while(entry){
				extrema = MBR(extrema, entry->rect);
				entry = entry->next;
			}
			n->rect = extrema;
		}
		if(n->nentries > RTREE_M){
			t_rtree_node *n1 = NULL, *n2 = NULL;
			rtree_split_node(tree, n, &n1, &n2);
		}
		n = parent;
	}
}

t_rtree_node *rtree_choose_leaf(t_rtree *tree, t_rtree_rect rect){
	t_rtree_node *n = tree->root;
	while(n->node_type != RTREE_LEAF){
		double overlap = DBL_MAX;
		t_rtree_node *best = n->entries;
		t_rtree_node *entry = n->entries;
		while(entry){
			t_rtree_rect mbr = MBR(entry->rect, rect);
			double o = AREA(mbr) - AREA(entry->rect);
			if(o <= overlap){
				overlap = o;
				best = entry;
			}
			entry = entry->next;
		}
		n = best;
		if(!n){
			break;
		}
	}
	return n;
}

void rtree_reinsert_orphans_r(t_rtree *tree, t_rtree_node *n){
	if(!n){
		return;
	}
	if(n->node_type == RTREE_DATA){
		//t_rtree_node *next = n->next;
		t_rtree_node *leaf = rtree_choose_leaf(tree, n->rect);
		n->next = NULL;
		n->prev = NULL;
		rtree_add_entry(leaf, n);
		if(leaf->nentries > RTREE_M){
			t_rtree_node *n1 = NULL, *n2 = NULL;
			rtree_split_node(tree, leaf, &n1, &n2);
			rtree_adjust_tree(tree, n1);
			rtree_adjust_tree(tree, n2);
		}else{
			rtree_adjust_tree(tree, leaf);
		}
		//rtree_reinsert_orphans_r(tree, next);
	}else{
		t_rtree_node *next = n->next;
		t_rtree_node *e = n->entries;
		while(e){
			t_rtree_node *e_next = e->next;
			e->next = NULL;
			e->prev = NULL;
			rtree_reinsert_orphans_r(tree, e);
			e = e_next;
		}
		//printf("calling free %p\n", n);
		rtree_node_free(n);
		rtree_reinsert_orphans_r(tree, next);
	}
}

void rtree_condense_tree_r(t_rtree *tree, t_rtree_node *n, t_rtree_node **orphans){
	if(!n){
		return;
	}
	//rtree_resize_node(n);
	t_rtree_node *parent = n->parent;
	if(!parent){// || tree->root == parent){
		return;
	}
	if(n->nentries < RTREE_MOVER2){
		rtree_unlink_entry(parent, n);
		n->next = *orphans;
		*orphans = n;
	}
	rtree_resize_node(parent);
	if(parent != tree->root){
		rtree_condense_tree_r(tree, parent, orphans);
	}
}

void rtree_condense_tree(t_rtree *tree, t_rtree_node *n){
	t_rtree_node *orphans = NULL;
	rtree_condense_tree_r(tree, n, &orphans);
	rtree_reinsert_orphans_r(tree, orphans);
	rtree_shorten_tree(tree);
}

void rtree_shorten_tree(t_rtree *tree){
	if((tree->root->nentries == 1) && (tree->root->entries->node_type != RTREE_DATA)){
		t_rtree_node *r = tree->root;
		tree->root = r->entries;
		tree->root->parent = NULL;
		rtree_node_free(r);
	}
}

void rtree_unlink(t_rtree_node *entry){
	if(entry->prev){
		entry->prev->next = entry->next;
	}
	if(entry->next){
		entry->next->prev = entry->prev;
	}
	entry->next = NULL, entry->prev = NULL;
}

// unlink <entry> from the <entries> list contained in <node>
void rtree_unlink_entry(t_rtree_node *node, t_rtree_node *entry){
	if(!node || !entry){
		return;
	}
	if(node->entries == entry){
		node->entries = entry->next;
	}
	rtree_unlink(entry);
	node->nentries--;
}

void rtree_add_entry(t_rtree_node *node, t_rtree_node *entry){
	/*
if(!node || !entry){
return;
}
	 */
	if(node->entries){
		node->entries->prev = entry;
	}
	entry->parent = node;
	entry->next = node->entries;
	node->entries = entry;
	node->nentries++;
	rtree_resize_node(node);
}

void rtree_quadratic_split(t_rtree_node *node, t_rtree_node **new1, t_rtree_node **new2){
	t_rtree_node *n1 = rtree_node_alloc();
	t_rtree_node *n2 = rtree_node_alloc();
	n1->parent = n2->parent = node->parent;
	n1->node_type = n2->node_type = node->node_type;
	t_rtree_node *entries = node->entries;
	int nentries = node->nentries;
	node->entries = NULL;
	node->nentries = 0;
	// pick seeds
	{
		t_rtree_node *e1 = NULL, *e2 = NULL;
		double d = -DBL_MAX;
		t_rtree_node *entry1 = entries;
		while(entry1){
			t_rtree_node *entry2 = entry1->next;
			while(entry2){
				double dd = fabs(AREA(MBR(entry1->rect, entry2->rect)) - AREA(entry1->rect) - AREA(entry2->rect));
				if(dd > d){
					d = dd;
					e1 = entry1;
					e2 = entry2;
				}
				entry2 = entry2->next;
			}
			entry1 = entry1->next;
		}
		if(entries == e1){
			entries = entries->next;
		}
		rtree_unlink(e1);
		rtree_add_entry(n1, e1);
		nentries--;
		if(entries == e2){
			entries = entries->next;
		}
		rtree_unlink(e2);
		rtree_add_entry(n2, e2);
		nentries--;
	}

	// pick next
	{
		t_rtree_node *entry = entries;
		int k = 0;
		while(entry){
			k++;
			t_rtree_node *next = entry->next;
			if(nentries + n1->nentries <= RTREE_MOVER2){
				while(entry){
					next = entry->next;
					rtree_unlink(entry);
					rtree_add_entry(n1, entry);
					entry = next;
				}
				*new1 = n1;
				*new2 = n2;
				return;
			}
			if(nentries + n2->nentries <= RTREE_MOVER2){
				while(entry){
					next = entry->next;
					rtree_unlink(entry);
					rtree_add_entry(n2, entry);
					entry = next;
				}
				*new1 = n1;
				*new2 = n2;
				return;
			}
			double d = DBL_MAX;
			t_rtree_node *e = entry;
			t_rtree_node *entry_to_move = e;
			t_rtree_node *node_to_move_to = n1;
			while(e){
				t_rtree_rect mbr1 = MBR(n1->rect, e->rect);
				t_rtree_rect mbr2 = MBR(n2->rect, e->rect);
				double diff1, diff2;
				diff1 = AREA(mbr1) - AREA(n1->rect);
				diff2 = AREA(mbr2) - AREA(n2->rect);
				if(diff1 < d){
					d = diff1;
					entry_to_move = e;
					node_to_move_to = n1;
				}
				if(diff2 < d){
					d = diff2;
					entry_to_move = e;
					node_to_move_to = n2;
				}
				e = e->next;
			}
			if(entry_to_move == entry){
				entry = entry->next;
			}
			rtree_unlink(entry_to_move);
			rtree_add_entry(node_to_move_to, entry_to_move);
			nentries--;
		}
	}
	*new1 = n1;
	*new2 = n2;
}

void rtree_split_node(t_rtree *tree, t_rtree_node *node, t_rtree_node **new1, t_rtree_node **new2){
	t_rtree_node *n1 = NULL, *n2 = NULL;
	rtree_quadratic_split(node, &n1, &n2);
	t_rtree_node *parent = node->parent;
	if(parent){
		parent->node_type = 0;
		rtree_unlink_entry(parent, node);
		rtree_node_free(node);
		rtree_add_entry(parent, n1);
		rtree_add_entry(parent, n2);
	}else{
		// we're at the root. reuse the node
		node->nentries = 0;
		node->entries = NULL;
		node->parent = NULL;
		node->node_type = 0;
		n1->parent = node;
		n2->parent = node;
		rtree_add_entry(node, n1);
		rtree_add_entry(node, n2);
		tree->root = node;
	}
	*new1 = n1;
	*new2 = n2;
}

void rtree_insert(t_rtree *tree, t_rtree_rect r, void *udata){
	t_rtree_rect rect = (t_rtree_rect){MIN(r.xmin, r.xmax), MAX(r.xmin, r.xmax), MIN(r.ymin, r.ymax), MAX(r.ymin, r.ymax)};
	t_rtree_node *leaf = NULL;
	if(tree->root){
		leaf = rtree_choose_leaf(tree, rect);
	}else{
		leaf = rtree_node_alloc();
		leaf->node_type = RTREE_LEAF;
		tree->root = leaf;
	}
	if(!leaf){
		printf("no leaf!!\n");
		return;
	}

	t_rtree_node *n = rtree_node_alloc();
	n->rect = rect;
	n->udata = udata;
	//printf("alloc: %p\n", udata);
	n->node_type = RTREE_DATA;
	rtree_add_entry(leaf, n);
	if(leaf->nentries > RTREE_M){
		t_rtree_node *n1 = NULL, *n2 = NULL;
		rtree_split_node(tree, leaf, &n1, &n2);
		rtree_adjust_tree(tree, n1);
		rtree_adjust_tree(tree, n2);
	}else{
		rtree_adjust_tree(tree, n);
	}
}

t_rtree_node *rtree_remove_r(t_rtree_node *n, t_rtree_rect r, void *udata){
	if(!rtree_isinside(n->rect, r)){
		return NULL;
	}
	t_rtree_node *e = n->entries;
	if(n->node_type == RTREE_LEAF){
		while(e){
			if(e->udata == udata){
				return e;
			}
			e = e->next;
		}
		return NULL;
	}else{
		while(e){
			t_rtree_node *nn = rtree_remove_r(e, r, udata);
			if(nn){
				return nn;
			}
			e = e->next;
		}	
	}
	return NULL;
}

void *rtree_remove(t_rtree *tree, t_rtree_rect r, void *udata){
	r = (t_rtree_rect){MIN(r.xmin, r.xmax), MAX(r.xmin, r.xmax), MIN(r.ymin, r.ymax), MAX(r.ymin, r.ymax)};
	t_rtree_node *n = rtree_remove_r(tree->root, r, udata);
	if(n){
		void *fudata = n->udata;
		t_rtree_node *parent = n->parent;
		rtree_unlink_entry(parent, n);
		rtree_resize_node(parent);
		rtree_condense_tree(tree, parent);
		rtree_node_free(n);
		return fudata;
	}
	return NULL;
}

void rtree_search_r(t_rtree_node *n, t_rtree_rect r, long *argc, void ***argv, long *argv_size){
	if(!n){
		return;
	}
 	//if(!INSIDE(n->rect, r) && !INSIDE(r, n->rect)){
	if((rtree_isinside(n->rect, r) == 0) && (rtree_isinside(r, n->rect) == 0)){
		return;
	}
	if(n->node_type == RTREE_DATA){
		//if(rtree_isinside(n->rect, r)){
			if(*argv_size <= (*argc + 1)){
				void **tmp = (void **)realloc(*argv, sizeof(void *) * (*argv_size + 128));
				if(tmp){
					*argv = tmp;
					*argv_size = *argv_size + 128;
				}else{
					return;
				}
			}
			((*argv)[*argc]) = n->udata;
			*argc = (*argc) + 1;
			//return n;
		//}
	}else{
		t_rtree_node *e = n->entries;
		while(e){
			//t_rtree_node *nn = rtree_search_r(e, r, argc, argv, argv_size);
			rtree_search_r(e, r, argc, argv, argv_size);
			/*
			if(nn){
				return nn;
			}
			*/
			e = e->next;
		}
		//return NULL;
	}
}

void rtree_search(t_rtree *tree, t_rtree_rect r, long *argc, void ***argv){
	long bufsize = 128;
	*argv = (void **)malloc(bufsize * sizeof(void *));
	t_rtree_rect rr = (t_rtree_rect){MIN(r.xmin, r.xmax), MAX(r.xmin, r.xmax), MIN(r.ymin, r.ymax), MAX(r.ymin, r.ymax)};
	rtree_search_r(tree->root, rr, argc, argv, &bufsize);
	//printf("found %d entries that overlap with %f %f %f %f\n", *argc, r.xmin, r.xmax, r.ymin, r.ymax);
}

void rtree_clear_r(t_rtree_node *n, t_rtree_dtor d, void *context){
	if(!n){
		return;
	}
	if(n->node_type == RTREE_DATA){
		if(d){
			d(context, n->udata);
		}
		rtree_unlink_entry(n->parent, n);
		rtree_node_free(n);
		return;
	}
	t_rtree_node *e = n->entries;
	while(e){
		t_rtree_node *next = e->next;
		rtree_clear_r(e, d, context);
		e = next;
 	}
	rtree_unlink_entry(n->parent, n);
	rtree_node_free(n);
}

void rtree_clear(t_rtree *tree, t_rtree_dtor d, void *context){
	if(tree){
		rtree_clear_r(tree->root, d, context);
		tree->root = NULL;
	}
}

t_rtree_node *rtree_node_alloc(){
	t_rtree_node *n = (t_rtree_node *)malloc(sizeof(t_rtree_node));
	memset(n, '\0', sizeof(t_rtree_node));
	n->rect = (t_rtree_rect){-DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX};
	return n;
}

t_rtree *rtree_new(void){
	t_rtree *r = (t_rtree *)malloc(sizeof(t_rtree));
	if(r){
		r->root = rtree_node_alloc();
		if(r->root){
			t_rtree_node *root = r->root;
			root->rect = (t_rtree_rect){-DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX};
			root->node_type = RTREE_LEAF;
		}
	}
	return r;
}

void rtree_destroy(t_rtree *tree, t_rtree_dtor d, void *context){
	if(tree){
		rtree_clear(tree, d, context);
		free(tree);
	}
}

void rtree_node_free(t_rtree_node *n){
	//printf("%s: %p\n", __func__, n->udata);
	if(n){
		free(n);
	}
}

#define ISPOINT(rect) (rect.xmin == rect.xmax && rect.ymin == rect.ymax)
#define ISLINE(rect) (rect.xmin == rect.xmax || rect.ymin == rect.ymax)

int rtree_isinside(t_rtree_rect r1, t_rtree_rect r2){
	if(ISPOINT(r1) || ISLINE(r1)){
		return 0;
	}
	if(ISPOINT(r2)){
		if((r2.xmin >= r1.xmin) && (r2.xmin <= r1.xmax) && (r2.ymin >= r1.ymin) && (r2.ymin <= r1.ymax)){
			return 1;
		}else{
			return 0;
		}
	}
	if(ISLINE(r2)){
		if(r2.xmin == r2.xmax){
			if((r2.xmin >= r1.xmin) && (r2.xmin <= r1.xmax) && ((r2.ymin >= r1.ymin) || (r2.ymax <= r1.ymax))){
				return 1;
			}else{
				return 0;
			}
		}else{
			if(((r2.xmin >= r1.xmin) || (r2.xmax <= r1.xmax)) && (r2.ymin >= r1.ymin) && (r2.ymin <= r1.ymax)){
				return 1;
			}else{
				return 0;
			}
		}
	}
	return INSIDE(r1, r2);
}

int rtree_counter = 0;

void rtree_print_node_r(t_rtree_node *n, int (*p)(const char*, ...), int depth){
	if(n->node_type == RTREE_DATA){
		char buf[32];
		sprintf(buf, "%%%ds %%p is a DATA node %%f %%f %%f %%f %%p\n", depth * 5);
		p(buf, "node", n, n->rect.xmin, n->rect.xmax, n->rect.ymin, n->rect.ymax, n->udata);
	}else{
		char buf[32];
		sprintf(buf, "%%%ds %%p is a %%s node and contains %%d entries:\n", depth * 5);
		p(buf, "node", n, n->node_type == 1 ? "LEAF" : "BRANCH", n->nentries);
		t_rtree_node *e = n->entries;
		int c = 0;
		while(e){
			if(e->node_type != RTREE_DATA){
				char buf[32];
				sprintf(buf, "%%%dd: %%p %%f %%f %%f\n", depth * 5);
				p(buf, c++, e, e->rect.xmin, e->rect.xmax, e->rect.ymin, e->rect.ymax);
			}
			rtree_print_node_r(e, p, depth + 1);
			e = e->next;
		}
	}
}

void rtree_print(t_rtree *tree, int (*p)(const char*, ...)){
	p("root node: %p, node_type = %d\n", tree->root, tree->root->node_type);
	rtree_counter = 0;
	t_rtree_node *n = tree->root;
	rtree_print_node_r(n, p, 0);
}

void rtree_write_node_r(t_rtree_node *n, FILE *fp){
	if(n->node_type != RTREE_DATA){
		fprintf(fp, "%f %f %f %f\n", n->rect.xmin, n->rect.xmax, n->rect.ymin, n->rect.ymax);
	}
	t_rtree_node *e = n->entries;
	while(e){
		rtree_write_node_r(e, fp);
		e = e->next;
	}
}

void rtree_write(t_rtree *tree, FILE *fp){
	//rtree_counter = 0;
	//FILE *fp = fopen(path, "w+");
	rtree_write_node_r(tree->root, fp);
	//fclose(fp);
}
