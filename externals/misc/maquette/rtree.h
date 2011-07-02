#ifndef __RTREE_H__
#define __RTREE_H__

typedef struct _rtree_rect{
	double xmin, xmax, ymin, ymax;
} t_rtree_rect;

#define RTREE_M 4
#define RTREE_MOVER2 (int)(RTREE_M / 2)

enum{
	RTREE_LEAF = 1,
	RTREE_DATA
};

typedef struct _rtree_node{
	t_rtree_rect rect;
	struct _rtree_node *parent; // link to this node's parent node
	struct _rtree_node *entries; // link to this node's children
	struct _rtree_node *next, *prev; // linked list of child nodes
	int nentries;
	int node_type;
	void *udata;
} t_rtree_node;

typedef void (*t_rtree_dtor)(void *context, void *object);

typedef struct _rtree{
	t_rtree_node *root;
} t_rtree;


void rtree_insert(t_rtree *tree, t_rtree_rect r, void *udata);
void *rtree_remove(t_rtree *tree, t_rtree_rect r, void *udata);
void rtree_search(t_rtree *tree, t_rtree_rect r, long *argc, void ***argv);
void rtree_clear(t_rtree *tree, t_rtree_dtor d, void *context);

t_rtree *rtree_new(void);
void rtree_destroy(t_rtree *tree, t_rtree_dtor d, void *context);
t_rtree_node *rtree_node_alloc();
void rtree_node_free(t_rtree_node *n);
void rtree_print(t_rtree *tree, int (*p)(const char *, ...));


#endif // __RTREE_H__
