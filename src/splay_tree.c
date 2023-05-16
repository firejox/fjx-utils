#include "fjx-utils/splay_tree.h"

#define splay_relink_child(node, link, child_link) \
    fjx_bst_node_relink_child(node, link, child_link)

static inline fjx_splay_node *
splay_double_rotate(
        fjx_splay_node *new_top,
        fjx_splay_node *new_left,
        fjx_splay_node *new_right) {
    splay_relink_child(new_left, &new_left->right, &new_top->left);
    splay_relink_child(new_right, &new_right->left, &new_top->right);

    fjx_bst_set_parent_fast(new_left, new_top);
    fjx_bst_set_parent_fast(new_right, new_top);

    return new_top;
}

static inline fjx_splay_node **
splay_parent_link_or(
        fjx_splay_node *node,
        fjx_splay_node *parent,
        fjx_splay_node **default_link) {
    if (parent != NULL) {
        if (parent->left == node) {
            return &parent->left;
        } else {
            return &parent->right;
        }
    } else {
        return default_link;
    }
}

void fjx_splay_top(fjx_splay_node *node, fjx_splay *tree) {
    fjx_splay_node *pa = NULL, *grand_pa = NULL, *ggrand_pa = NULL;
    fjx_splay_node **ggrand_pa_link = NULL;

    for (pa = fjx_splay_parent(node); pa != NULL; ) {
        grand_pa = fjx_splay_parent(pa);

        if (grand_pa != NULL) {
            ggrand_pa = fjx_splay_parent(grand_pa);
            ggrand_pa_link = splay_parent_link_or(grand_pa, ggrand_pa, &tree->root);

            if (grand_pa->left == pa) {
                if (pa->left == node) {
                    splay_relink_child(grand_pa, &grand_pa->left, &pa->right);
                    fjx_bst_set_parent_fast(grand_pa, pa);
                    splay_relink_child(pa, &pa->left, &node->right);
                    fjx_bst_set_parent_fast(pa, node);
                    fjx_splay_node_link(
                            node,
                            ggrand_pa,
                            ggrand_pa_link);
                } else {
                    fjx_splay_node_link(
                        splay_double_rotate(
                            node,
                            pa,
                            grand_pa),
                        ggrand_pa,
                        ggrand_pa_link);
                }
            } else {
                if (pa->left == node) {
                    fjx_splay_node_link(
                        splay_double_rotate(
                            node,
                            grand_pa,
                            pa),
                        ggrand_pa,
                        ggrand_pa_link);
                } else {
                    splay_relink_child(grand_pa, &grand_pa->right, &pa->left);
                    fjx_bst_set_parent_fast(grand_pa, pa);
                    splay_relink_child(pa, &pa->right, &node->left);
                    fjx_bst_set_parent_fast(pa, node);
                    fjx_splay_node_link(
                            node,
                            ggrand_pa,
                            ggrand_pa_link);
                }
            }

            pa = ggrand_pa;
            continue;
        } else if (pa->left == node){
            splay_relink_child(pa, &pa->left, &node->right);
            fjx_bst_set_parent_fast(pa, node);
            fjx_splay_node_link(node, NULL, &tree->root);
        } else {
            splay_relink_child(pa, &pa->right, &node->left);
            fjx_bst_set_parent_fast(pa, node);
            fjx_splay_node_link(node, NULL, &tree->root);
        }
        break;
    }
}

void fjx_splay_remove(fjx_splay_node *node, fjx_splay *tree) {
    fjx_splay_top(node, tree);

    if (node->left == NULL) {
        if (node->right == NULL) {
            tree->root = NULL;
        } else {
            fjx_splay_node_link(node->right, NULL, &tree->root);
        }
    } else if (node->right == NULL) {
        fjx_splay_node_link(node->left, NULL, &tree->root);
    } else {
        fjx_splay_node *tmp = fjx_bst_left_most(node->right);

        if (node->right == tmp) {
            fjx_splay_node_link(node->left, tmp, &tmp->left);
            fjx_splay_node_link(tmp, NULL, &tree->root);
        } else {
            fjx_splay_node *tmp_pa = fjx_splay_parent(tmp);

            if (tmp->right == NULL) {
                tmp_pa->left = NULL;
            } else {
                fjx_splay_node_link(tmp->right, tmp_pa, &tmp_pa->left);
            }

            fjx_splay_node_link(node->left, tmp, &tmp->left);
            fjx_splay_node_link(node->right, tmp, &tmp->right);
            fjx_splay_node_link(tmp, NULL, &tree->root);
        }
    }
}
