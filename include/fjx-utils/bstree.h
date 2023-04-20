#ifndef fjx_utils_bstree_h
#define fjx_utils_bstree_h

#include "utils.h"

struct fjx_bst_node__ {
    uintptr_t __parent;
    struct fjx_bst_node__ *left;
    struct fjx_bst_node__ *right;
} __attribute__((aligned(sizeof(unsigned long))));

struct fjx_bst__ {
    struct fjx_bst_node__ *root;
};

typedef struct fjx_bst_node__ fjx_bst_node;
typedef struct fjx_bst__ fjx_bst;

static inline fjx_bst_node *fjx_bst_parent(const fjx_bst_node *nd) {
    return (fjx_bst_node *)((void *)(nd->__parent & ~(uintptr_t)3));
}

static inline uintptr_t fjx_bst_tag(const fjx_bst_node *nd) {
    return nd->__parent & (uintptr_t)3;
}

static inline void
fjx_bst_set_parent(
        fjx_bst_node *nd,
        fjx_bst_node *pa,
        uintptr_t tag) {
    nd->__parent = (uintptr_t)((void *)pa) | (tag & (uintptr_t)3);
}

static inline void
fjx_bst_set_parent_fast(
        fjx_bst_node *nd,
        fjx_bst_node *pa) {
    nd->__parent = (uintptr_t)((void *)pa);
}

static inline void
fjx_bst_replace_parent(
        fjx_bst_node *nd,
        fjx_bst_node *new_pa) {
    nd->__parent = (uintptr_t)((void *)new_pa) | (nd->__parent & (uintptr_t)3);
}

static inline void fjx_bst_node_init(fjx_bst_node *nd) {
    nd->left = nd->right = NULL;
    nd->__parent = (uintptr_t)((void *)nd);
}

static inline bool fjx_bst_node_empty(const fjx_bst_node *nd) {
    return nd->__parent == (uintptr_t)((void *)nd);
}

static inline bool fjx_bst_empty(const fjx_bst* tree) {
    return tree->root == NULL;
}

static inline void
fjx_bst_node_link(
        fjx_bst_node *node,
        fjx_bst_node *parent,
        fjx_bst_node **link) {
    fjx_bst_replace_parent(node, parent);
    *link = node;
}

static inline void
fjx_bst_node_relink_child(
        fjx_bst_node *node,
        fjx_bst_node **link,
        fjx_bst_node **child_link) {
    if ((*link = *child_link) != NULL)
        fjx_bst_replace_parent(*link, node);
    *child_link = node;
}

static inline bool fjx_bst_node_leaf(const fjx_bst_node *node) {
    return node->left == NULL && node->right == NULL;
}

static inline fjx_bst_node *
fjx_bst_left_most(fjx_bst_node *node) {
    while (node->left != NULL) node = node->left;
    return node;
}

static inline fjx_bst_node *
fjx_bst_right_most(fjx_bst_node *node) {
    while (node->right != NULL) node = node->right;
    return node;
}

extern fjx_bst_node *fjx_bst_next(const fjx_bst_node *);
extern fjx_bst_node *fjx_bst_prev(const fjx_bst_node *);

#endif
