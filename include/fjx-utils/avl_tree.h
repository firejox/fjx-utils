#ifndef fjx_utils_avl_tree_h
#define fjx_utils_avl_tree_h

#include "bstree.h"

typedef fjx_bst_node fjx_avl_node;
typedef fjx_bst fjx_avl;

#define fjx_avl_node_link(nd, pa, link) fjx_bst_node_link(nd, pa, link)
#define fjx_avl_next(nd) fjx_bst_next(nd)
#define fjx_avl_prev(nd) fjx_bst_prev(nd)
#define fjx_avl_node_init(nd) fjx_bst_node_init(nd)
#define fjx_avl_parent(nd) fjx_bst_parent(nd)
#define fjx_avl_bf(nd) fjx_bst_tag(nd)
#define fjx_avl_node_empty(nd) fjx_bst_node_empty(nd)
#define fjx_avl_empty(tree) fjx_bst_empty(tree)

extern void fjx_avl_insert_bf(fjx_avl_node *, fjx_avl *);
extern void fjx_avl_remove(fjx_avl_node *, fjx_avl *);

#endif
