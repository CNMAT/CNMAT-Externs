/*
 * Rumati AVL
 * Copyright (c) 2010 Jesse Long <jpl@unknown.za.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "avl.h"

#include <stdlib.h>     /* for malloc(), free() */
#include <stdint.h>     /* for int8_t */
#include <stdbool.h>    /* for bool */

/*
 * The maximum height for this tree. This is the maximum height a tree can be
 * BEFORE balancing. Make this 1 more than the worst case height for the amount
 * of nodes you want. For perfectly balanced trees, the tree will hold
 * (2^(RUMATI_AVL_MAX_HEIGHT - 1)) - 1 nodes. Add one for AVL imbalance
 * tolerance. RUMATI_AVL_MAX_HEIGHT 40 will hold all the nodes 1 terbyte of ram
 * can handle.
 *
 * Also, for each rumati_avl_put() and rumati_avl_delete() operation, a stack
 * allocation of about RUMATI_AVL_MAX_HEIGHT * 8 will be made. For small values
 * this is fine (you can afford ~300 bytes, trust me), but if you make
 * RUMATI_AVL_MAX_HEIGHT something stupid like 102400, dont be surprised if
 * you waste some memory. Think about it: each node takes about 12-16 bytes
 * of ram before your data, say 20 bytes conservatively. Does your computer
 * really have ((2^RUMATI_AVL_MAX_HEIGHT)-1)*20 bytes of RAM? Didn't think so.
 */
#define RUMATI_AVL_MAX_HEIGHT   40

/*
 * Tree type
 */
struct rumati_avl_tree {
    /*
     * comparator, function to compare values
     */
    RUMATI_AVL_COMPARATOR comparator;
    /*
     * User provided pointer
     */
    void *udata;
    /*
     * Root node in tree, NULL initially
     */
    struct rumati_avl_node *root;
};

/*
 * Tree node structure. Each value stored in the tree has an associated node.
 */
struct rumati_avl_node {
    /*
     * Link to left child (smaller value), or NULL is there is no left child.
     */
    struct rumati_avl_node *left;
    /*
     * Link to right child (greater value), or NULL is there is no right child.
     */
    struct rumati_avl_node *right;
    /*
     * Difference in height of sub trees. If left subtree is 1 layer higher
     * than the right subtree, then balance is -1. Balance is +1 if the right
     * subtree is one layer higher then the left subtree. AVL rules specify
     * that one subtree may not be more than one layer higher than the other
     * subtree. However, during double rotations, it is possible for the first
     * node to have a balance >= +2 or <= -2.
     */
    int8_t balance;
    /*
     * The data held by this node.
     */
    void *data;
};

/*
 * A structure representing an update needing to happen on the balance of a
 * node beneath which a change (insert or delete) has been made.
 */
struct rumati_avl_update {
    /*
     * A pointer to the pointer to the node. We need pointer to pointer
     * because the pointer to the node needs to be updated when we rotate
     * the node to rebalance it.
     */
    struct rumati_avl_node **node_ptr;
    /*
     * A boolean indicating if the inserted/deleted node is in the left subtree
     * of the node, or not. If false, the inserted/deleted node is in the right
     * subtree.
     */
    bool left;
};

/*
 * A list of updates, representing a path taken down the tree.
 */
struct rumati_avl_update_list {
    /* the number of updates in the list */
    unsigned int number_of_updates;
    /* an array holding the updates */
    struct rumati_avl_update update[RUMATI_AVL_MAX_HEIGHT];
};

/*
 * rumati_avl_new() - creates a new AVL tree.
 *
 * Parameters:
 *      tree -  a pointer to a pointer to an AVL tree. This will be populated
 *              with a pointer to the new tree if create successfully.
 *      comparator -    a function that compares node values, for sorting.
 *      udata - a user defined pointer to be passed to the comparator and
 *              other user callback functions.
 *
 * Returns:
 *      RUMATI_AVL_OK       On success
 *      RUMATI_AVL_EINVAL   If comparator is NULL or tree is NULL
 *      RUMATI_AVL_ENOMEM   If there was a memory allocation error.
 */
RUMATI_AVL_API
RUMATI_AVL_ERROR rumati_avl_new(
        RUMATI_AVL_TREE **tree,
        RUMATI_AVL_COMPARATOR comparator,
        void *udata)
{
    RUMATI_AVL_TREE *retv;

    /*
     * comparator cannot be NULL, otherwise we would be unable to compare
     * node values.
     * tree cannot be NULL, other we are guarenteed to have a memory leak.
     */
    if (comparator == NULL || tree == NULL){
        return RUMATI_AVL_EINVAL;
    }

    retv = malloc(sizeof(*retv));
    if (retv == NULL){
        return RUMATI_AVL_ENOMEM;
    }

    retv->comparator = comparator;
    retv->udata = udata;
    retv->root = NULL;

    *tree = retv;
    return RUMATI_AVL_OK;
}

/*
 * rumati_avl_destroy_node() - destroys a single node by invoking a destructor
 * on the node's data, and free()ing the node.
 *
 * Parameters:
 *      n -     the node to destroy
 *      destructor -    the destrctor to use to destroy the nodes data
 *      udata - the user supplied pointer associated with the tree
 */
static void rumati_avl_destroy_node(
        struct rumati_avl_node *n,
        RUMATI_AVL_NODE_DESTRUCTOR destructor,
        void *udata)
{
	if(destructor){
    		destructor(udata, n->data);
	}
    free(n);
}

/*
 * rumati_avl_destroy_node_recursive() - recursively destroys a node, and all
 * its children using rumati_avl_node_destroy().
 *
 * Parameters:
 *      n -     the node to destroy, along with all its children
 *      destructor -    the destrctor to use to destroy the nodes data
 *      udata - the user supplied pointer associated with the tree
 */
static void rumati_avl_destroy_node_recursive (
        struct rumati_avl_node *n,
        RUMATI_AVL_NODE_DESTRUCTOR destructor,
        void *udata)
{
    if (n->left != NULL){
        rumati_avl_destroy_node_recursive(n->left, destructor, udata);
    }
    if (n->right != NULL){
        rumati_avl_destroy_node_recursive(n->right, destructor, udata);
    }
    rumati_avl_destroy_node(n, destructor, udata);
} 

/*
 * rumati_avl_clear() - removes all nodes from the tree, using the destructor
 * provided.
 *
 * Parameters:
 *      tree -  the tree to clear
 *      destructor -    the destructor to use when destroying each node's data
 */
RUMATI_AVL_API
void rumati_avl_clear(
        RUMATI_AVL_TREE *tree,
        RUMATI_AVL_NODE_DESTRUCTOR destructor)
{
    if (tree->root != NULL){
        rumati_avl_destroy_node_recursive(tree->root, destructor, tree->udata);
    }
    tree->root = NULL;
}

/*
 * rumati_avl_destroy() - destroys a tree, freeing all memory used by the tree,
 * and cleanly destrying node data using a destructor.
 * provided.
 *
 * Parameters:
 *      tree -  the tree to destroy
 *      destructor -    the destructor to use when destroying each node's data
 */
RUMATI_AVL_API
void rumati_avl_destroy(
        RUMATI_AVL_TREE *tree,
        RUMATI_AVL_NODE_DESTRUCTOR destructor)
{
    rumati_avl_clear(tree, destructor);
    free(tree);
}

/*
 * rumati_avl_rotate_right() - rotates a subtree to the right/clock wise
 *
 * Parameters:
 *      node_ptr -  A pointer to the pointer to the root of this subtree
 *                  this will be updated to point to the new root, after
 *                  rotation.
 */
static void rumati_avl_rotate_right(struct rumati_avl_node **node_ptr)
{
    /*
     * In this function, we will rotate a tree clockwise. See the following
     * diagram:
     *
     *      D           B
     *     / \         / \
     *    B   E  =>   A   D
     *   / \             / \
     *  A   C           C   E
     *
     * The tree rooted at D is rotated clockwise so that the new root is B.
     * D takes the right child of B (ie. C) as its left child, while B takes
     * D as its right child.
     */
    int8_t orb, nrb;

    /* keep reference to old root (D) */
    struct rumati_avl_node *old_root = *node_ptr;

    /* make root pointer point to new root, old root (D)'s left child (B) */
    *node_ptr = old_root->left;

    /* 
     * old root (D) inherits new root (B)'s right child (C) as it's left child.
     * remember, its previous left child (B) is now it's (D's) parent
     */
    old_root->left = (*node_ptr)->right;

    /* new root (B)'s right child is the old root (D) */
    (*node_ptr)->right = old_root;

    /* old root balance pre rotate (balance of D pre-rotate) */
    orb = old_root->balance;
    /* new root balance pre rotate (balance of B pre-rotate) */
    nrb = (*node_ptr)->balance;

    /*
     * Now, we increment the balance of the old root (D), ie. Make it heavier
     * by one on the right side. This is not because the right side became
     * heavier, but because the left side became lighter.
     *
     * Proof:
     *      Situation 1, trees rooted at A and C are of equal height (balance
     *      of B is 0), or tree C is heavier than tree A (balance of B is > 0):
     *          The left tree of D has lost a layer by D adopting its granchild
     *          (C) in place of B. So, A's left tree is one layer lighter.
     *      Situation 2, tree rooted at A is heavier than tree rooted at C
     *      (balance of B is < 0):
     *          The rule for Situation 1 still applies, because D has still
     *          adopted its granchild C, thereby losing a layer. Additionally
     *          D is lighter on the left side by the amount that tree A is
     *          heavier than tree C, since D is no longer an ancestor of tree
     *          A. We know the difference in height between tree A and tree C,
     *          it is the balance of node B.
     */
    old_root->balance++;
    if (nrb < 0){
        /*
         * For situation 2 above, tree D is additionally lighter on the left
         * by the amount by which tree A is heavier than tree C (ie, balance
         * of node B). (-= negative number...)
         */
        old_root->balance -= nrb;
    }

    /*
     * Similarly, node B is now heavier on the right side, since it has gained
     * a layer. Previously, Node C was the direct child of node B, but it is
     * now the the left child of D, which is the child of B - C is now the
     * granchild of B. We make B heavier by one level on the right side.
     */
    (*node_ptr)->balance++;
    if (old_root->balance > 0){
        /*
         * B was never an ancestor of the tree rooted at E. If the tree rooted
         * E is heavier than the tree rooted at C, then B is additionally
         * heavier on the right side by the amount by which tree E is heavier
         * than tree C (new balance of node D).
         */
        (*node_ptr)->balance += old_root->balance;
    }
}

/*
 * rumati_avl_rotate_left() - rotates a subtree to the left/anti-clock wise
 *
 * Parameters:
 *      node_ptr -  A pointer to the pointer to the root of this subtree
 *                  this will be updated to point to the new root, after
 *                  rotation.
 */
static void rumati_avl_rotate_left(struct rumati_avl_node **node_ptr)
{
    /*
     * The implementation of this function is identical to the implementation
     * of rumati_avl_rotate_right(), please see comments there.
     */
    int8_t orb, nrb;
    struct rumati_avl_node *old_root = *node_ptr;

    *node_ptr = old_root->right;
    old_root->right = (*node_ptr)->left;
    (*node_ptr)->left = old_root;

    orb = old_root->balance;
    nrb = (*node_ptr)->balance;

    old_root->balance--;
    if (nrb > 0){
        old_root->balance -= nrb;
    }

    (*node_ptr)->balance--;
    if (old_root->balance < 0){
        (*node_ptr)->balance += old_root->balance;
    }
}

/*
 * rumati_avl_add_update() - adds an update to a list of updates, checking for
 * buffer overflow.
 *
 * Parameters:
 *      updates -   the list of updates
 *      node_ptr -  a pointer to the parent nodes link to the node to be
 *                  updated.
 *      left -      was the update on the left subtree or not (not == right
 *                  subtree).
 *
 * Returns:
 *      true    On success.
 *      false   If the update list is longer than RUMATI_AVL_MAX_HEIGHT.
 */
static bool rumati_avl_add_update(
        struct rumati_avl_update_list *updates,
        struct rumati_avl_node **node_ptr,
        bool left)
{
    if (updates->number_of_updates == RUMATI_AVL_MAX_HEIGHT - 1){
        return false;
    }
    updates->update[updates->number_of_updates].node_ptr = node_ptr;
    updates->update[updates->number_of_updates].left = left;
    updates->number_of_updates++;
    return true;
}

/*
 * rumati_avl_put() - inserts an entry into the tree, replacing an existing
 * entry if one exists.
 *
 * Parameters:
 *      tree -      The tree to which to add the entry.
 *      entry -     The entry to add to the tree.
 *      old_value - A pointer to a pointer which will be populated with the 
 *                  the previous value for the entry if one exists, or NULL
 *                  if there was previously no matching entry. If NULL is
 *                  passed as old_value, then the previous value will be
 *                  overwritten without being destroyed, which may cause a
 *                  memory leak.
 * 
 * Returns:
 *      RUMATI_AVL_OK       On success
 *      RUMATI_AVL_ETOOBIG  If the tree is to big.
 *      RUMATI_AVL_ENOMEM   If there was an error allocating memory.
 */
RUMATI_AVL_API
RUMATI_AVL_ERROR rumati_avl_put(
        RUMATI_AVL_TREE *tree,
        void *object,
        void **old_value)
{
    struct rumati_avl_node *n = NULL;
    struct rumati_avl_node **parent_link = &tree->root;
    struct rumati_avl_update_list updates;

    /* init updates */
    updates.number_of_updates = 0;

    /* do binary search looking for an existing node with matching data */
    while (*parent_link != NULL){
        int cmp = tree->comparator(tree->udata, object, (*parent_link)->data);
        if (cmp == 0){
            /*
             * This node matches the new node. Populate old_value and replace
             * data. No need for rebalancing.
             */
            if (old_value != NULL){
                *old_value = (*parent_link)->data;
            }
            (*parent_link)->data = object;
            return RUMATI_AVL_OK;
        }else if (cmp > 0){
            /*
             * Destination node is to the right of this node, add this node to
             * the list of nodes to be updates and check right child.
             */
            if (rumati_avl_add_update(&updates, parent_link, false) == false){
                return RUMATI_AVL_ETOOBIG;
            }
            parent_link = &(*parent_link)->right;
        }else if (cmp < 0){
            /*
             * Destination node is to the left of this node, add this node to
             * the list of nodes to be updates and check left child.
             */
            if (rumati_avl_add_update(&updates, parent_link, true) == false){
                return RUMATI_AVL_ETOOBIG;
            }
            parent_link = &(*parent_link)->left;
        }
    }
    /*
     * No matching node found, so, this will be a new node, insert as leaf
     * where our binary search ended.
     */

    n = malloc(sizeof(*n));
    if (n == NULL){
        return RUMATI_AVL_ENOMEM;
    }
    n->left = NULL;
    n->right = NULL;
    n->balance = 0;
    n->data = object;

    *parent_link = n;

    if (old_value != NULL){
        *old_value = NULL;
    }

    /*
     * Do updates
     */
    while (updates.number_of_updates > 0){
        struct rumati_avl_update *update;
        updates.number_of_updates--;
        update = &updates.update[updates.number_of_updates];
        if (update->left){
            /*
             * Node added to the left, so tree must be heavier to the left.
             * In other words, decrease balance.
             */
            (*update->node_ptr)->balance--;
            if ((*update->node_ptr)->balance == 0){
                /*
                 * If the addition of a node in this nodes left subtree left
                 * the node balanced, then no further updates are required to
                 * be performed on this nodes parents.
                 */
                break;
            }else if ((*update->node_ptr)->balance < -1){
                /*
                 * Tree is unbalanced. We now rotate the tree to balance this
                 * node, then break because, for each new node added to a
                 * tree, we only ever need to rebalance one node.
                 *
                 * We may need to do a double rotate, because of the situation
                 * where the right child of our left child is heavier. This
                 * would cause a simple, single rotation to leave the tree as
                 * unbalanced as it was before the rotate. An example of this
                 * behaviour below:
                 *
                 *  Figure 1  |  Figure 2   |   Figure 3    |   Figure 4
                 * -----------+-------------+---------------+---------------
                 *      F     |     B       |         F     |       D
                 *     / \    |    / \      |        / \    |      / \
                 *    /   \   |   /   \     |       /   \   |     /   \
                 *   B     G  |  A     F    |      D     G  |    B     F
                 *  / \       |       / \   |     / \       |   / \   / \
                 * A   D      |      D   G  |    B   E      |  A   C E   G
                 *    / \     |     / \     |   / \         |
                 *   C   E    |    C   E    |  A   C        |
                 *
                 * If the tree in Figure 1 is simply rotated clockwise, the
                 * result is the tree in Figure 2, which is equally unbalanced,
                 * because the previous root (F) inherits its heaviest
                 * granchild (D).
                 *
                 * The solution is to first perform an anti-clokwise rotation
                 * on B, resulting in the tree shown in Figure 3, then rotating
                 * F clockwise, resulting in a balanced tree shown in Figure 4.
                 *
                 * It is also interesting to note that, for any tree rooted at
                 * F (see Figure 1), where the tree is unbalanced towards the
                 * left subtree rooted at B, it is not possible for node B to
                 * have an even balance. If F is unbalanced, then B must be at
                 * least 1 level heavier on either side.
                 */
                if ((*update->node_ptr)->left->balance > 0){
                    rumati_avl_rotate_left(&(*update->node_ptr)->left);
                }
                rumati_avl_rotate_right(update->node_ptr);
                break;
            }
        }else{
            /*
             * Please see discussion above
             */
            (*update->node_ptr)->balance++;
            if ((*update->node_ptr)->balance == 0){
                break;
            }else if ((*update->node_ptr)->balance > 1){
                if ((*update->node_ptr)->right->balance < 0){
                    rumati_avl_rotate_right(&(*update->node_ptr)->right);
                }
                rumati_avl_rotate_left(update->node_ptr);
                break;
            }
        }
    }

    return RUMATI_AVL_OK;
}

/*
 * rumati_avl_get() - returns the matching entry in the tree, if one exists.
 *
 * Parameters:
 *      tree -  The tree to search for a matching entry.
 *      key -   The key with which to search for a matching entry.
 *
 * Returns:
 *      The matching entry in the tree, or NULL if no matching entry was found.
 */
RUMATI_AVL_API
void *rumati_avl_get(
        RUMATI_AVL_TREE *tree,
        void *key)
{
    struct rumati_avl_node *n = tree->root;

    while (n != NULL){
        int cmp = tree->comparator(tree->udata, key, n->data);
        if (cmp > 0){
            n = n->right;
        }else if (cmp < 0){
            n = n->left;
        }else{
            return n->data;
        }
    }

    return NULL;
}

/*
 * rumati_avl_get_greater_than_or_equal() - returns the lowest key which is
 * either greater than or equal to the given key.
 *
 * Parameters:
 *      tree -  The tree in which to search for a qualifying entry
 *      key -   The key which a qualifying entry must be greater than or
 *              equal to.
 *
 * Returns:
 *      A lowest entry that is greater than or equal to key, or NULL if no
 *      entry was found which is greater than or equal to key.
 */
RUMATI_AVL_API
void *rumati_avl_get_greater_than_or_equal(
        RUMATI_AVL_TREE *tree,
        void *key)
{
    struct rumati_avl_node *n = tree->root;
    /*
     * Each time we go left, keep a reference to the node at which we go left.
     * If we fail to find any nodes greater than or equal to the search key,
     * then this will be the first entry which is greater than the search key.
     */
    struct rumati_avl_node *prev = NULL;

    while (n != NULL){
        int cmp = tree->comparator(tree->udata, key, n->data);
        if (cmp > 0){
            n = n->right;
        }else if (cmp < 0){
            prev = n;
            n = n->left;
        }else{
            return n->data;
        }
    }

    if (prev != NULL){
        return prev->data;
    }

    return NULL;
}

/*
 * rumati_avl_get_less_than_or_equal() - returns the highest key which is
 * either less than or equal to the given key.
 *
 * Parameters:
 *      tree -  The tree in which to search for a qualifying entry
 *      key -   The key which a qualifying entry must be less than or
 *              equal to.
 *
 * Returns:
 *      A highest entry that is less than or equal to key, or NULL if no
 *      entry was found which is less than or equal to key.
 */
RUMATI_AVL_API
void *rumati_avl_get_less_than_or_equal(
        RUMATI_AVL_TREE *tree,
        void *key)
{
    struct rumati_avl_node *n = tree->root;
    /*
     * Each time we go right, keep a reference to the node at which we go
     * right. If we fail to find any nodes less than or equal to the search
     * key, then this will be the first entry which is less than the search
     * key.
     */
    struct rumati_avl_node *prev = NULL;

    while (n != NULL){
        int cmp = tree->comparator(tree->udata, key, n->data);
        if (cmp > 0){
            prev = n;
            n = n->right;
        }else if (cmp < 0){
            n = n->left;
        }else{
            return n->data;
        }
    }

    if (prev != NULL){
        return prev->data;
    }

    return NULL;
}

/*
 * rumati_avl_get_greater_than() - retrieves the lowest entry is is scrictly
 * greater than the search key.
 *
 * Parameters:
 *      tree -  The tree to search.
 *      key -   The key which a matching entry must be greater than.
 *
 * Returns:
 *      The lowest entry which is scrictly greater than the search key, or
 *      NULL if there is no entry greater than the search key.
 */
RUMATI_AVL_API
void *rumati_avl_get_greater_than(
        RUMATI_AVL_TREE *tree,
        void *key)
{
    struct rumati_avl_node *n = tree->root;
    struct rumati_avl_node *prev = NULL;

    while (n != NULL){
        int cmp = tree->comparator(tree->udata, key, n->data);
        if (cmp > 0){
            n = n->right;
        }else if (cmp < 0){
            prev = n;
            n = n->left;
        }else{
            if (n->right == NULL){
                break;
            }
            n = n->right;
            while (n->left != NULL){
                n = n->left;
            }
            return n->data;
        }
    }

    if (prev != NULL){
        return prev->data;
    }

    return NULL;
}

/*
 * rumati_avl_get_less_than() - retrieves the lowest entry is is scrictly
 * less than the search key.
 *
 * Parameters:
 *      tree -  The tree to search.
 *      key -   The key which a matching entry must be less than.
 *
 * Returns:
 *      The lowest entry which is scrictly less than the search key, or
 *      NULL if there is no entry less than the search key.
 */
RUMATI_AVL_API
void *rumati_avl_get_less_than(
        RUMATI_AVL_TREE *tree,
        void *key)
{
    struct rumati_avl_node *n = tree->root;
    struct rumati_avl_node *prev = NULL;

    while (n != NULL){
        int cmp = tree->comparator(tree->udata, key, n->data);
        if (cmp > 0){
            prev = n;
            n = n->right;
        }else if (cmp < 0){
            n = n->left;
        }else{
            if (n->left == NULL){
                break;
            }
            n = n->left;
            while (n->right != NULL){
                n = n->right;
            }
            return n->data;
        }
    }

    if (prev != NULL){
        return prev->data;
    }

    return NULL;
}

/*
 * rumati_avl_delete() - removes an entry from a tree.
 *
 * Parameters:
 *      tree -      The tree from which to delete the entry.
 *      key -       The value by which to find the entry to be deleted.
 *      old_value - A pointer which will be populated with a pointer to
 *                  the deleted entry if one is found. You should then release
 *                  the memory held by the deleted entry. You may pass NULL as
 *                  old_value, but then you will have no opportunity to
 *                  release the memory used by the deleted entry, which will
 *                  be a memory leak in most uses.
 *
 * Returns:
 *      RUMATI_AVL_OK       If the entry was deleted successfully.
 *      RUMATI_AVL_ENOENT   If no matching entry was found.
 *      RUMATI_AVL_ETOOBIG  If the tree is too large. This should never happen,
 *                          since rumati_avl_put() should fail with
 *                          RUMATI_AVL_ETOOBIG when creating a tree which is
 *                          too large.
 */
RUMATI_AVL_API
RUMATI_AVL_ERROR rumati_avl_delete(
        RUMATI_AVL_TREE *tree,
        void *key,
        void **old_value)
{
    struct rumati_avl_node **parent_link = &tree->root;
    struct rumati_avl_update_list updates;

    /* init updates */
    updates.number_of_updates = 0;

    while (1){
        int cmp;

        if (*parent_link == NULL){
            /*
             * We reached a leaf's NULL pointer child link, without finding
             * a matching entry - none exists.
             */
            return RUMATI_AVL_ENOENT;
        }

        /* normal binary search descend based on key comparison */
        cmp = tree->comparator(tree->udata, key, (*parent_link)->data);
        if (cmp > 0){
            /*
             * Node to be deleted is to the right of this node, descend.
             */
            if (rumati_avl_add_update(&updates, parent_link, false) == false){
                return RUMATI_AVL_ETOOBIG;
            }
            parent_link = &(*parent_link)->right;
        }else if (cmp < 0){
            /*
             * Node to be deleted is to the left of this node, descend.
             */
            if (rumati_avl_add_update(&updates, parent_link, true) == false){
                return RUMATI_AVL_ETOOBIG;
            }
            parent_link = &(*parent_link)->left;
        }else if (cmp == 0){
            struct rumati_avl_node *delnode = NULL;
            void *tmp_data_ptr;
            /*
             * This is the node which must be deleted
             */
            delnode = *parent_link;
            tmp_data_ptr = delnode->data;
            /*
             * First, try delete the node in place if it does not have 2
             * children, by replacing it with it's only node if it have one,
             * or by making it's parent a leaf if it has no children.
             */
            if ((*parent_link)->right == NULL){
                *parent_link = (*parent_link)->left;
            }else if ((*parent_link)->left == NULL){
                *parent_link = (*parent_link)->right;
            }else{
                /*
                 * The node to be deleted has two children. We cannot simply
                 * delete it by replacing it with it's only child. So, we
                 * delete it by overwritting its data with that of its inner-
                 * most child on its heavier subtree, then deleting that inner
                 * most child in the heavier subtree.
                 */
                if ((*parent_link)->balance < 0){
                    /*
                     * Left subtree is greater, replace the node to be deleted
                     * with the right most node in the left subtree.
                     *
                     * Move left, then as far right as possible.
                     */
                    if (rumati_avl_add_update(&updates, parent_link, true) == false){
                        return RUMATI_AVL_ETOOBIG;
                    }
                    parent_link = &(*parent_link)->left;
                    while ((*parent_link)->right != NULL){
                        if (rumati_avl_add_update(&updates, parent_link, false) == false){
                            return RUMATI_AVL_ETOOBIG;
                        }
                        parent_link = &(*parent_link)->right;
                    }
                    /*
                     * Overwrite node to be deleted with replacement node.
                     */
                    delnode->data = (*parent_link)->data;
                    delnode = *parent_link;
                    /*
                     * Consider:
                     *
                     *     E
                     *    / \
                     *   B   F
                     *  / \   \
                     * A   D   G
                     *    /
                     *   C
                     *
                     * E's content is replaced with the content from D, then D
                     * is deleted. B must inherits D's outside hild (C) if any.
                     */
                    *parent_link = delnode->left;
                }else{
                    /*
                     * Same as block above
                     */
                    if (rumati_avl_add_update(&updates, parent_link, false) == false){
                        return RUMATI_AVL_ETOOBIG;
                    }
                    parent_link = &(*parent_link)->right;
                    while ((*parent_link)->left != NULL){
                        if (rumati_avl_add_update(&updates, parent_link, true) == false){
                            return RUMATI_AVL_ETOOBIG;
                        }
                        parent_link = &(*parent_link)->left;
                    }
                    delnode->data = (*parent_link)->data;
                    delnode = *parent_link;
                    *parent_link = delnode->right;
                }
            }
            /*
             * If the user has given an "out" variable for the deleted value,
             * populate it with the deleted value.
             */
            if (old_value != NULL){
                *old_value = tmp_data_ptr;
            }
            free(delnode);
            break;
        }
    }

    /*
     * Do updates
     */
    while (updates.number_of_updates > 0){
        struct rumati_avl_update *update;
        updates.number_of_updates--;
        update = &updates.update[updates.number_of_updates];
        if (update->left){
            /*
             * Node deleted to the left of this node, bump balance towards
             * the right.
             */
            (*update->node_ptr)->balance++;
            /*
             * TODO discuss affect on parent:
             * (balance is after adjustment for deleted descendant)
             *  -   balance < 0:    impossible, would have had to be imbalanced
             *                      before delete
             *  -   balance = 0:    parent loses 1 height, as tree is now one
             *                      layer lighter
             *  -   balance = 1:    tree was balanced, now 1 layer heavier on
             *                      right. No affect on parent, no more updates
             *                      required.
             *  -   balance > 1:    tree was one heavier on right, now 2
             *                      heavier, ie. imbalanced. This situation has
             *                      no affect on parent, because node was
             *                      deleted on lighter subtree. However, a
             *                      rotation is required to rebalance tree.
             *                      This rotation may or may not cause the
             *                      parent to be one layer lighter. If it does,
             *                      we must continue updating the parent. If
             *                      not, we stop updating here.
             */
            if ((*update->node_ptr)->balance > 1){
                /*
                 * Node is now imbalanced. Rebalance according to normal
                 * AVL rules. See rumati_avl_put() for discussion.
                 */
                if ((*update->node_ptr)->right->balance < 0){
                    /*
                     * Double rotation required, eg:
                     *
                     *  A      A         B
                     *   \      \       / \
                     *    C =>   B  => A   C
                     *   /        \
                     *  B          C
                     *
                     *  This will leave the tree lighter, so we continue to
                     *  update parents.
                     */
                    rumati_avl_rotate_right(&(*update->node_ptr)->right);
                    rumati_avl_rotate_left(update->node_ptr);
                }else if ((*update->node_ptr)->right->balance == 0){
                    /*
                     * The tree is in need of rotation, but the rotation will
                     * not change the size of the tree, so stop updating here.
                     * There is no change in parent balance. eg:
                     *
                     * A         C
                     *  \       / \
                     *   C  => A   D
                     *  / \     \
                     * B   D     B
                     */
                    rumati_avl_rotate_left(update->node_ptr);
                    break;
                }else{
                    /*
                     * A simple left rotation which will cause the tree to be
                     * lighter - continue updating. eg:
                     *
                     * A         B
                     *  \       / \
                     *   B  => A   C
                     *    \
                     *     C
                     */
                    rumati_avl_rotate_left(update->node_ptr);
                }
            }else if ((*update->node_ptr)->balance == 1){
                break;
            }
        }else{
            (*update->node_ptr)->balance--;
            if ((*update->node_ptr)->balance < -1){
                if ((*update->node_ptr)->left->balance > 0){
                    rumati_avl_rotate_left(&(*update->node_ptr)->left);
                    rumati_avl_rotate_right(update->node_ptr);
                }else if ((*update->node_ptr)->left->balance == 0){
                    rumati_avl_rotate_right(update->node_ptr);
                    break;
                }else{
                    rumati_avl_rotate_right(update->node_ptr);
                }
            }else if ((*update->node_ptr)->balance == -1){
                break;
            }
        }
    }

    return RUMATI_AVL_OK;
}

/*
 * rumati_avl_get_smallest() - retrieves the smallest entry in the tree.
 *
 * Parameters:
 *      tree -  The tree in which to find the smallest value.
 * 
 * Returns:
 *      A pointer to the smallest value in the tree, or NULL if the tree
 *      is empty.
 */
RUMATI_AVL_API
void *rumati_avl_get_smallest(RUMATI_AVL_TREE *tree)
{
    struct rumati_avl_node *n = tree->root;
    
    if (n == NULL){
        return NULL;
    }

    while (n->left != NULL){
        n = n->left;
    }

    return n->data;
}

/*
 * rumati_avl_get_greatest() - retrieves the greatest entry in the tree.
 *
 * Parameters:
 *      tree -  The tree in which to find the greatest value.
 * 
 * Returns:
 *      A pointer to the greatest value in the tree, or NULL if the tree
 *      is empty.
 */
RUMATI_AVL_API
void *rumati_avl_get_greatest(RUMATI_AVL_TREE *tree)
{
    struct rumati_avl_node *n = tree->root;
    
    if (n == NULL){
        return NULL;
    }

    while (n->right != NULL){
        n = n->right;
    }

    return n->data;
}
