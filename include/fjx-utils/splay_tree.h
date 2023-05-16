#ifndef fjx_utils_splay_tree_h
#define fjx_utils_splay_tree_h

#include "bstree.h"

typedef fjx_bst_node fjx_splay_node;
typedef fjx_bst fjx_splay;

#define fjx_splay_node_link(nd, pa, link) fjx_bst_node_link(nd, pa, link)
#define fjx_splay_next(nd) fjx_bst_next(nd)
#define fjx_splay_prev(nd) fjx_bst_prev(nd)
#define fjx_splay_node_init(nd) fjx_bst_node_init(nd)
#define fjx_splay_parent(nd) fjx_bst_parent(nd)
#define fjx_splay_bf(nd) fjx_bst_tag(nd)
#define fjx_splay_node_empty(nd) fjx_bst_node_empty(nd)
#define fjx_splay_empty(tree) fjx_bst_empty(tree)

extern void fjx_splay_top(fjx_splay_node *, fjx_splay *);
extern void fjx_splay_remove(fjx_splay_node *, fjx_splay *);

#endif
