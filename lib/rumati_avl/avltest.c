#include "avl.c"

static int int_comparator(void *udata, void *ip1, void *ip2)
{
    int i1 = *(int*)ip1;
    int i2 = *(int*)ip2;

    (void)udata;

    if (i1 < i2){
        return -1;
    }else if (i1 > i2){
        return 1;
    }

    return 0;
}

static void destructor(void *udata, void *node)
{
    (void)udata;
    (void)node;
}

#include <stdio.h>

#define MAX_TEST_NUMBER 10000

static int verify_node_height(struct rumati_avl_node *n)
{
    int left_height = 0;
    int right_height = 0;

    if (n->left != NULL){
        left_height = verify_node_height(n->left);
    }

    if (n->right != NULL){
        right_height = verify_node_height(n->right);
    }

    if (left_height < 0 || right_height < 0){
        return -1;
    }

    if (right_height - left_height != n->balance){
        printf("Error, node %d has balance of %d, but left height is %d and right height is %d\n",
                *(int*)n->data, n->balance, left_height, right_height);
        return -1;
    }

    if (left_height > right_height){
        return left_height + 1;
    }else{
        return right_height + 1;
    }
}

static bool verify_tree(RUMATI_AVL_TREE *tree, bool in_tree[])
{
    int i;
    int *ip;

    for (i = 0; i < MAX_TEST_NUMBER; i++){
        ip = (int*)(rumati_avl_get(tree, &i));
        if (in_tree[i] == true){
            if (ip == NULL){
                printf("Number %d not found in tree, but was expected\n", i);
                return false;
            }
            if (*ip != i){
                printf("Incorrect return value for rumati_avl_get(%d): %d\n", i, *ip);
                return false;
            }
        }else{
            if (ip != NULL){
                printf("Number %d found in tree, but was not expected\n", i);
                return false;
            }
        }
    }

    if (tree->root != NULL && verify_node_height(tree->root) < 0){
        return false;
    }

    return true;
}

int main (int argc, char *argv[])
{
    RUMATI_AVL_TREE *tree;
    RUMATI_AVL_ERROR err;
    bool in_tree[MAX_TEST_NUMBER];
    int num[MAX_TEST_NUMBER];
    int i, n, retv;

    if ((err = rumati_avl_new(&tree, int_comparator, NULL)) != RUMATI_AVL_OK){
        printf("Error creating avl tree: %d\n", err);
        return 1;
    }

    retv = 0;

    for (i = 0; i < MAX_TEST_NUMBER; i++){
        in_tree[i] = false;
        num[i] = i;
    }

    if (verify_tree(tree, in_tree) == false){
        retv = 1;
        goto out1;
    }

    for (i = 0; i < 8000; i++){
        n = random() % MAX_TEST_NUMBER;
        printf("INSERT: %d\n", n);
        in_tree[n] = true;
        if ((err = rumati_avl_put(tree, &num[n], NULL)) != RUMATI_AVL_OK){
            retv = 1;
            printf("Error adding %d to tree: %d\n", n, err);
            break;
        }
        if (verify_tree(tree, in_tree) == false){
            retv = 1;
            goto out1;
        }
    }

    for (i = 0; i < 5000; i++){
        n = random() % MAX_TEST_NUMBER;
        printf("DELETE: %d\n", n);
        err = rumati_avl_delete(tree, &num[n], NULL);
        if (in_tree[n] == true){
            if (err != RUMATI_AVL_OK){
                retv = 1;
                printf("Error removing number %d tree, when it did exist in the tree: %d\n", n, err);
                goto out1;
            }
        }else{
            if (err == RUMATI_AVL_OK){
                retv = 1;
                printf("Error, number %d successfully deleted from tree, when it wasn't in the tree\n", n);
                goto out1;
            }else if (err != RUMATI_AVL_ENOENT){
                retv = 1;
                printf("Error removing number %d from tree when it did not exist in tree: %d\n", n, err);
                goto out1;
            }
        }
        in_tree[n] = false;
        if (verify_tree(tree, in_tree) == false){
            retv = 1;
            goto out1;
        }
    }
    
    printf("OK! Tests passed successfully!\n");

out1:
    rumati_avl_destroy(tree, destructor);
    return retv;
}
