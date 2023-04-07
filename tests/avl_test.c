#include <check.h>
#include "fjx-utils/avl_tree.h"

struct my_node {
    int value;
    fjx_avl_node link;
};

fjx_avl tree;
struct my_node n1, n2, n3, n4;

extern Suite *test_suite(void);

static void setup(void) {
    n1.value = 1;
    fjx_avl_node_init(&n1.link);

    n2.value = 2;
    fjx_avl_node_init(&n2.link);

    n3.value = 3;
    fjx_avl_node_init(&n3.link);

    n4.value = 4;
    fjx_avl_node_init(&n4.link);
}

static void teardown(void) {}

static void insert(struct my_node *node, fjx_avl *tree) {
    fjx_avl_node **it = &tree->root;
    fjx_avl_node *pa = NULL;

    for (fjx_avl_node *i = *it; i != NULL; i = *it) {
        pa = i;
        struct my_node *cur = fjx_container_of(pa, struct my_node, link);

        if (cur->value < node->value) {
            it = &pa->right;
        } else {
            it = &pa->left;
        }
    }

    fjx_bst_set_parent_fast(&node->link, pa);
    *it = &node->link;

    fjx_avl_insert_bf(&node->link, tree);
}

START_TEST (test_avl_insert)
{
    tree.root = NULL;

    insert(&n1, &tree);
    ck_assert_ptr_eq(tree.root, &n1.link);
    ck_assert_ptr_eq(fjx_avl_parent(&n1.link), NULL);
    ck_assert_ptr_eq(n1.link.left, NULL);
    ck_assert_ptr_eq(n1.link.right, NULL);

    insert(&n2, &tree);
    ck_assert_ptr_eq(tree.root, &n1.link);
    ck_assert_ptr_eq(fjx_avl_parent(&n1.link), NULL);
    ck_assert_ptr_eq(n1.link.left, NULL);
    ck_assert_ptr_eq(n1.link.right, &n2.link);

    ck_assert_ptr_eq(fjx_avl_parent(&n2.link), &n1.link);
    ck_assert_ptr_eq(n2.link.left, NULL);
    ck_assert_ptr_eq(n2.link.right, NULL);

    insert(&n3, &tree);
    ck_assert_ptr_eq(tree.root, &n2.link);
    ck_assert_ptr_eq(fjx_avl_parent(&n2.link), NULL);
    ck_assert_ptr_eq(n2.link.left, &n1.link);
    ck_assert_ptr_eq(n2.link.right, &n3.link);

    ck_assert_ptr_eq(fjx_avl_parent(&n1.link), &n2.link);
    ck_assert_ptr_eq(n1.link.left, NULL);
    ck_assert_ptr_eq(n1.link.right, NULL);

    ck_assert_ptr_eq(fjx_avl_parent(&n3.link), &n2.link);
    ck_assert_ptr_eq(n3.link.left, NULL);
    ck_assert_ptr_eq(n3.link.right, NULL);
}
END_TEST

START_TEST (test_avl_remove)
{
    tree.root = NULL;

    insert(&n1, &tree);
    insert(&n2, &tree);
    insert(&n3, &tree);
    insert(&n4, &tree);
    fjx_avl_remove(&n3.link, &tree);

    ck_assert_ptr_eq(tree.root, &n2.link);
    ck_assert_ptr_eq(fjx_avl_parent(&n2.link), NULL);
    ck_assert_ptr_eq(n2.link.left, &n1.link);
    ck_assert_ptr_eq(n2.link.right, &n4.link);

    ck_assert_ptr_eq(fjx_avl_parent(&n1.link), &n2.link);
    ck_assert_ptr_eq(n1.link.left, NULL);
    ck_assert_ptr_eq(n1.link.right, NULL);

    ck_assert_ptr_eq(fjx_avl_parent(&n4.link), &n2.link);
    ck_assert_ptr_eq(n4.link.left, NULL);
    ck_assert_ptr_eq(n4.link.right, NULL);

    fjx_avl_remove(&n2.link, &tree);
    ck_assert_ptr_eq(tree.root, &n4.link);
    ck_assert_ptr_eq(fjx_avl_parent(&n4.link), NULL);
    ck_assert_ptr_eq(n4.link.left, &n1.link);
    ck_assert_ptr_eq(n4.link.right, NULL);

    ck_assert_ptr_eq(fjx_avl_parent(&n1.link), &n4.link);
    ck_assert_ptr_eq(n1.link.left, NULL);
    ck_assert_ptr_eq(n1.link.right, NULL);
}
END_TEST

Suite *test_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("AVL Tree");

    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_avl_insert);
    tcase_add_test(tc_core, test_avl_remove);
    suite_add_tcase(s, tc_core);

    return s;
}

