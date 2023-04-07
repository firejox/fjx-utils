#include "fjx-utils/avl_tree.h"

#define avl_relink_child(node, link, child_link) \
    fjx_bst_node_relink_child(node, link, child_link)

enum {
    BF_M = (uintptr_t)0,
    BF_L = (uintptr_t)1,
    BF_R = (uintptr_t)2,
    BF_MASK = (uintptr_t)3,
    BF_INV_MASK = ~(uintptr_t)3
};

static inline void avl_left_heavify(fjx_avl_node *node) {
    node->__parent |= BF_L;
}

static inline void avl_balancify(fjx_avl_node *node) {
    node->__parent &= BF_INV_MASK;
}

static inline void avl_right_heavify(fjx_avl_node *node) {
    node->__parent |= BF_R;
}

static inline fjx_avl_node *
avl_double_rotate(
        fjx_avl_node *new_top,
        fjx_avl_node *new_left,
        fjx_avl_node *new_right) {

    avl_relink_child(new_left, &new_left->right, &new_top->left);
    avl_relink_child(new_right, &new_right->left, &new_top->right);

    switch (fjx_avl_bf(new_top)) {
        case BF_M:
            fjx_bst_set_parent(new_left, new_top, BF_M);
            fjx_bst_set_parent(new_right, new_top, BF_M);
            break;

        case BF_L:
            fjx_bst_set_parent(new_left, new_top, BF_M);
            fjx_bst_set_parent(new_right, new_top, BF_R);
            avl_balancify(new_top);
            break;

        case BF_R:
            fjx_bst_set_parent(new_left, new_top, BF_L);
            fjx_bst_set_parent(new_right, new_top, BF_M);
            avl_balancify(new_top);
            break;

        default:
            unreachable();
    }

    return new_top;
}

static inline fjx_avl_node *
avl_rotate_left(
        fjx_avl_node *parent,
        fjx_avl_node *node) {

    avl_relink_child(parent, &parent->right, &node->left);

    if (fjx_avl_bf(node) == BF_M) {
        fjx_bst_set_parent(parent, node, BF_R);
        avl_left_heavify(node);
    } else {
        fjx_bst_set_parent(parent, node, BF_M);
        avl_balancify(node);
    }

    return node;
}

static inline fjx_avl_node *
avl_rotate_right(
        fjx_avl_node *parent,
        fjx_avl_node *node) {

    avl_relink_child(parent, &parent->left, &node->right);

    if (fjx_avl_bf(node) == BF_M) {
        fjx_bst_set_parent(parent, node, BF_L);
        avl_right_heavify(node);
    } else {
        fjx_bst_set_parent(parent, node, BF_M);
        avl_balancify(node);
    }

    return node;
}

static inline fjx_avl_node**
avl_parent_link_or(
        fjx_avl_node *node,
        fjx_avl_node *parent,
        fjx_avl_node **default_link) {
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

void fjx_avl_insert_bf(fjx_avl_node *node, fjx_avl *tree) {
    fjx_avl_node *it, *pa;
    uintptr_t it_tag;
    fjx_avl_node *grand_pa, **pa_link, *tmp;

    avl_balancify(node);
    for (it = node, pa = fjx_bst_parent(it), it_tag = BF_M; pa != NULL; it = pa, pa = fjx_bst_parent(pa)) {
        if (pa->right == it) {
            switch (fjx_avl_bf(pa)) {
                case BF_L:
                    avl_balancify(pa);
                    return;
                case BF_M:
                    it_tag = BF_R;
                    avl_right_heavify(pa);
                    continue;
                case BF_R:
                    grand_pa = fjx_bst_parent(pa);
                    pa_link = avl_parent_link_or(pa, grand_pa, &tree->root);
                    tmp = (it_tag == BF_L) ?
                        avl_double_rotate(it->left, pa, it):
                        avl_rotate_left(pa, it);

                    fjx_avl_node_link(tmp, grand_pa, pa_link);
                    return;
                default:
                    unreachable();
            }
        } else {
            switch (fjx_avl_bf(pa)) {
                case BF_R:
                    avl_balancify(pa);
                    return;
                case BF_M:
                    it_tag = BF_L;
                    avl_left_heavify(pa);
                    continue;
                case BF_L:
                    grand_pa = fjx_bst_parent(pa);
                    pa_link = avl_parent_link_or(pa, grand_pa, &tree->root);
                    tmp = (it_tag == BF_R) ?
                        avl_double_rotate(it->right, it, pa):
                        avl_rotate_right(pa, it);

                    fjx_avl_node_link(tmp, grand_pa, pa_link);
                    return;
                default:
                    unreachable();
            }
        }
    }
}

static inline void
avl_remove_leaf(
        fjx_avl_node *leaf,
        fjx_avl *tree) {
    fjx_avl_node *it, *pa, *grand_pa;
    fjx_avl_node *sibling;
    uintptr_t sibling_tag;
    fjx_avl_node **pa_link;

    for (it = leaf, pa = fjx_bst_parent(it); pa != NULL; pa = grand_pa) {
        grand_pa = fjx_bst_parent(pa);

        if (pa->right == it) {
            switch (fjx_avl_bf(pa)) {
                case BF_R:
                    it = pa;
                    avl_balancify(pa);
                    continue;
                case BF_M:
                    avl_left_heavify(pa);
                    return;
                case BF_L:
                    sibling = pa->left;
                    pa_link = avl_parent_link_or(pa, grand_pa, &tree->root);
                    sibling_tag = fjx_avl_bf(sibling);
                    it = (sibling_tag == BF_R) ?
                        avl_double_rotate(sibling->right, sibling, pa):
                        avl_rotate_right(pa, sibling);

                    fjx_bst_node_link(it, grand_pa, pa_link);

                    if (sibling_tag == BF_M) return;
                    continue;
                default:
                    unreachable();
            }
        } else {
            switch (fjx_avl_bf(pa)) {
                case BF_L:
                    it = pa;
                    avl_balancify(pa);
                    continue;
                case BF_M:
                    avl_right_heavify(pa);
                    return;
                case BF_R:
                    sibling = pa->right;
                    pa_link = avl_parent_link_or(pa, grand_pa, &tree->root);
                    sibling_tag = fjx_avl_bf(sibling);
                    it = (sibling_tag == BF_L) ?
                        avl_double_rotate(sibling->left, pa, sibling):
                        avl_rotate_left(pa, sibling);

                    fjx_bst_node_link(it, grand_pa, pa_link);

                    if (sibling_tag == BF_M) return;
                    continue;
                default:
                    unreachable();
            }
        }
    }
}

static inline fjx_avl_node *
avl_remove_transform(
        fjx_avl_node *node,
        fjx_avl *tree) {
    if (fjx_bst_node_leaf(node)) {
        return node;
    }

    fjx_avl_node *pa = fjx_avl_parent(node);
    fjx_avl_node **pa_link = avl_parent_link_or(node, pa, &tree->root);
    fjx_avl_node *l = node->left, *r = node->right;
    uintptr_t node_bf = fjx_avl_bf(node);

    if (l == NULL) {
        fjx_bst_set_parent(r, pa, BF_R);
        *pa_link = r;

        r->right = node;
        node->right = NULL;
        fjx_bst_set_parent(node, r, BF_M);
    } else if (r == NULL) {
        fjx_bst_set_parent(l, pa, BF_L);
        *pa_link = l;

        l->left = node;
        node->left = NULL;
        fjx_bst_set_parent(node, l, BF_M);
    } else if (r->left == NULL) {
        fjx_avl_node *rr = r->right;

        if (rr == NULL) {
            fjx_avl_node_link(l, r, &r->left);
            r->right = node;
            fjx_bst_set_parent_fast(node, r);

            fjx_bst_set_parent(r, pa, node_bf);
            *pa_link = r;

            node->left = node->right = NULL;
        } else {
            fjx_avl_node_link(l, r, &r->left);
            rr->left = node;
            fjx_bst_set_parent_fast(node, rr);

            fjx_bst_set_parent(r, pa, node_bf);
            *pa_link = r;

            avl_left_heavify(rr);

            node->left = node->right = NULL;
        }
    } else {
        fjx_avl_node *t = fjx_bst_left_most(r);
        fjx_avl_node *t_pa = fjx_avl_parent(t), *tr = t->right;

        fjx_avl_node_link(l, t, &t->left);
        fjx_avl_node_link(r, t, &t->right);

        fjx_bst_set_parent(t, pa, node_bf);
        *pa_link = t;

        node->left = node->right = NULL;

        if (tr == NULL) {
            t_pa->left = node;
            fjx_bst_set_parent_fast(node, t_pa);
        } else {
            t_pa->left = tr;
            fjx_bst_set_parent(tr, t_pa, BF_L);

            tr->left = node;
            fjx_bst_set_parent_fast(node, tr);
        }
    }

    return node;
}

void fjx_avl_remove(fjx_avl_node *node, fjx_avl *tree) {
    fjx_avl_node *leaf, *pa, **pa_link;

    leaf = avl_remove_transform(node, tree);
    avl_remove_leaf(leaf, tree);

    pa = fjx_avl_parent(leaf);
    pa_link = avl_parent_link_or(leaf, pa, &tree->root);
    *pa_link = NULL;
}
