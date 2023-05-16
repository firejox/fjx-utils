#include <check.h>
#include "fjx-utils/splay_tree.h"

struct my_node {
    int value;
    fjx_splay_node link;
};

fjx_splay tree;
struct my_node n1, n2, n3, n4, n5;

extern Suite *test_suite(void);

static void setup(void) {
    tree.root = NULL;

    n1.value = 1;
    fjx_splay_node_init(&n1.link);

    n2.value = 2;
    fjx_splay_node_init(&n2.link);

    n3.value = 3;
    fjx_splay_node_init(&n3.link);

    n4.value = 4;
    fjx_splay_node_init(&n4.link);

    n5.value = 5;
    fjx_splay_node_init(&n5.link);
}

static void teardown(void) {}

static void insert(struct my_node *node, fjx_splay *tree) {
    fjx_splay_node **it = &tree->root;
    fjx_splay_node *pa = NULL;

    for (fjx_splay_node *p = *it; p != NULL; p = *it) {
        pa = p;
        struct my_node *cur = fjx_container_of(pa, struct my_node, link);

        if (cur->value < node->value) {
            it = &pa->right;
        } else {
            it = &pa->left;
        }
    }

    fjx_splay_node_link(&node->link, pa, it);
}

#define parent_child_check(parent, dir, child) do { \
    ck_assert_ptr_eq((parent)->dir, child); \
    if (child != NULL) \
        ck_assert_ptr_eq(fjx_splay_parent(child), parent); \
} while (0)

START_TEST (test_splay_zig)
{
    insert(&n3, &tree);
    insert(&n1, &tree);
    insert(&n2, &tree);
    insert(&n5, &tree);
    insert(&n4, &tree);

    fjx_splay_top(&n1.link, &tree);

    ck_assert_ptr_eq(tree.root, &n1.link);
    parent_child_check(&n1.link, left, NULL);
    parent_child_check(&n1.link, right, &n3.link);
    parent_child_check(&n3.link, left, &n2.link);
    parent_child_check(&n3.link, right, &n5.link);
    parent_child_check(&n2.link, left, NULL);
    parent_child_check(&n2.link, right, NULL);
}
END_TEST

START_TEST (test_splay_zag)
{
    insert(&n3, &tree);
    insert(&n1, &tree);
    insert(&n2, &tree);
    insert(&n5, &tree);
    insert(&n4, &tree);

    fjx_splay_top(&n5.link, &tree);

    ck_assert_ptr_eq(tree.root, &n5.link);
    parent_child_check(&n5.link, left, &n3.link);
    parent_child_check(&n5.link, right, NULL);
    parent_child_check(&n3.link, left, &n1.link);
    parent_child_check(&n3.link, right, &n4.link);
    parent_child_check(&n4.link, left, NULL);
    parent_child_check(&n4.link, right, NULL);
}
END_TEST

START_TEST (test_splay_zig_zig)
{
    insert(&n3, &tree);
    insert(&n2, &tree);
    insert(&n1, &tree);
    insert(&n4, &tree);
    insert(&n5, &tree);

    fjx_splay_top(&n1.link, &tree);

    ck_assert_ptr_eq(tree.root, &n1.link);
    parent_child_check(&n1.link, left, NULL);
    parent_child_check(&n1.link, right, &n2.link);
    parent_child_check(&n2.link, left, NULL);
    parent_child_check(&n2.link, right, &n3.link);
    parent_child_check(&n3.link, left, NULL);
    parent_child_check(&n3.link, right, &n4.link);
    parent_child_check(&n4.link, left, NULL);
    parent_child_check(&n4.link, right, &n5.link);
}
END_TEST

START_TEST (test_splay_zag_zag)
{
    insert(&n3, &tree);
    insert(&n2, &tree);
    insert(&n1, &tree);
    insert(&n4, &tree);
    insert(&n5, &tree);

    fjx_splay_top(&n5.link, &tree);

    ck_assert_ptr_eq(tree.root, &n5.link);
    parent_child_check(&n5.link, left, &n4.link);
    parent_child_check(&n5.link, right, NULL);
    parent_child_check(&n4.link, left, &n3.link);
    parent_child_check(&n4.link, right, NULL);
    parent_child_check(&n3.link, left, &n2.link);
    parent_child_check(&n3.link, right, NULL);
    parent_child_check(&n2.link, left, &n1.link);
    parent_child_check(&n2.link, right, NULL);
}
END_TEST

START_TEST (test_splay_zig_zag)
{
    insert(&n5, &tree);
    insert(&n1, &tree);
    insert(&n3, &tree);
    insert(&n2, &tree);
    insert(&n4, &tree);

    fjx_splay_top(&n3.link, &tree);

    ck_assert_ptr_eq(tree.root, &n3.link);
    parent_child_check(&n3.link, left, &n1.link);
    parent_child_check(&n3.link, right, &n5.link);
    parent_child_check(&n1.link, left, NULL);
    parent_child_check(&n1.link, right, &n2.link);
    parent_child_check(&n5.link, left, &n4.link);
    parent_child_check(&n5.link, right, NULL);
    parent_child_check(&n2.link, left, NULL);
    parent_child_check(&n2.link, right, NULL);
    parent_child_check(&n4.link, left, NULL);
    parent_child_check(&n4.link, right, NULL);
}
END_TEST

START_TEST (test_splay_zag_zig)
{
    insert(&n1, &tree);
    insert(&n5, &tree);
    insert(&n3, &tree);
    insert(&n2, &tree);
    insert(&n4, &tree);

    fjx_splay_top(&n3.link, &tree);

    ck_assert_ptr_eq(tree.root, &n3.link);
    parent_child_check(&n3.link, left, &n1.link);
    parent_child_check(&n3.link, right, &n5.link);
    parent_child_check(&n1.link, left, NULL);
    parent_child_check(&n1.link, right, &n2.link);
    parent_child_check(&n5.link, left, &n4.link);
    parent_child_check(&n5.link, right, NULL);
    parent_child_check(&n2.link, left, NULL);
    parent_child_check(&n2.link, right, NULL);
    parent_child_check(&n4.link, left, NULL);
    parent_child_check(&n4.link, right, NULL);
}
END_TEST

START_TEST (test_splay_remove)
{
    insert(&n1, &tree);
    insert(&n5, &tree);
    insert(&n3, &tree);
    insert(&n2, &tree);
    insert(&n4, &tree);

    fjx_splay_remove(&n3.link, &tree);

    ck_assert_ptr_eq(tree.root, &n4.link);

    parent_child_check(&n4.link, left, &n1.link);
    parent_child_check(&n4.link, right, &n5.link);
    parent_child_check(&n1.link, right, &n2.link);
}
END_TEST

Suite *test_suite(void)
{
    Suite *s = suite_create("Splay Tree");
    TCase *tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_splay_zig);
    tcase_add_test(tc_core, test_splay_zag);
    tcase_add_test(tc_core, test_splay_zig_zig);
    tcase_add_test(tc_core, test_splay_zag_zag);
    tcase_add_test(tc_core, test_splay_zig_zag);
    tcase_add_test(tc_core, test_splay_zag_zig);
    tcase_add_test(tc_core, test_splay_remove);
    suite_add_tcase(s, tc_core);

    return s;
}

