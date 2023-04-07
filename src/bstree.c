#include "fjx-utils/bstree.h"

fjx_bst_node *fjx_bst_next(const fjx_bst_node *node) {
    fjx_bst_node *parent, *it;

    if (fjx_bst_node_empty(node))
        return (fjx_bst_node *)node;

    if (node->right) {
        for (it = node->right; it->left; it = it->left);
        return it;
    } else {
        for (it = (fjx_bst_node *)node; (parent = fjx_bst_parent(it)) && it == parent->right; it = parent);
        return parent;
    }
}

fjx_bst_node *fjx_bst_prev(const fjx_bst_node *node) {
    fjx_bst_node *parent, *it;

    if (fjx_bst_node_empty(node))
        return NULL;

    if (node->left) {
        for (it = node->left; it->right; it = it->right);
        return it;
    } else {
        for (it = (fjx_bst_node *)node; (parent = fjx_bst_parent(it)) && it == parent->left; it = parent);
        return parent;
    }
}

