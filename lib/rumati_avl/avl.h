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
#ifndef RUMATI_AVL_H
#define RUMATI_AVL_H 1

/*
 * This macro allows you to define a prefix for all public functions
 * of this library. Useful for defining to static, or Windows DLL stuff.
 */
#ifndef RUMATI_AVL_API
#define RUMATI_AVL_API
#endif

/*
 * The basic type for AVL trees. This is the opaque context passed to all
 * library methods.
 */
typedef struct rumati_avl_tree RUMATI_AVL_TREE;

/*
 * Error codes returned by this library
 */
typedef enum {
    RUMATI_AVL_OK,          /* all ok */
    RUMATI_AVL_ENOMEM,      /* malloc failure */
    RUMATI_AVL_EINVAL,      /* invalid parameter, probably NULL */
    RUMATI_AVL_ENOENT,      /* no such element */
    RUMATI_AVL_ETOOBIG      /* tree too big */
} RUMATI_AVL_ERROR;

/*
 * A function to compare node values in a tree. This function should return
 * integers less than zero if value1 is ordered before value2, zero if the
 * values are equal, or values greater than zero if value1 if sorted after
 * value2.
 */
typedef int(*RUMATI_AVL_COMPARATOR)(
        void *udata,
        void *value1,
        void *value2);

/*
 * A function that destructs values in a tree. Typically used to destroy or
 * clear trees. Each node value is passed to the function to be destroyed.
 */
typedef void(*RUMATI_AVL_NODE_DESTRUCTOR)(
        void *udata,
        void *value);

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
        void *udata);

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
        RUMATI_AVL_NODE_DESTRUCTOR destructor);

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
        RUMATI_AVL_NODE_DESTRUCTOR destructor);

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
        void **old_value);

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
        void *key);

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
        void *key);

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
        void *key);

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
        void *key);

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
        void *key);

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
        void **old_value);

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
void *rumati_avl_get_smallest(RUMATI_AVL_TREE *tree);

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
void *rumati_avl_get_greatest(RUMATI_AVL_TREE *tree);

#endif /* RUMATI_AVL_H */
