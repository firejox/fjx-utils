#include <check.h>
#include "fjx-utils/list.h"

extern Suite *test_suite(void);

struct my_node {
    int value;
    fjx_list link;
};

struct my_list {
    fjx_list head;
};

static struct my_list list, olist;
static struct my_node n1, n2, n3, n4;

static void setup(void) {
    n1.value = 1;
    n2.value = 2;
    n3.value = 3;
    n4.value = 4;
}

static void teardown(void) {

}

START_TEST (test_list_init)
{
    fjx_list_init(&list.head);
    ck_assert_ptr_eq(&list.head, list.head.next);
    ck_assert_ptr_eq(&list.head, list.head.prev);
}
END_TEST

START_TEST (test_list_add)
{
    fjx_list_init(&list.head);
    fjx_list_add(&list.head, &n1.link);
    fjx_list_add(&list.head, &n2.link);

    ck_assert_ptr_eq(list.head.next, &n2.link);
    ck_assert_ptr_eq(n2.link.next, &n1.link);
    ck_assert_ptr_eq(n1.link.next, &list.head);

    ck_assert_ptr_eq(list.head.prev, &n1.link);
    ck_assert_ptr_eq(n1.link.prev, &n2.link);
    ck_assert_ptr_eq(n2.link.prev, &list.head);
}
END_TEST

START_TEST (test_list_add_tail)
{
    fjx_list_init(&list.head);
    fjx_list_add_tail(&list.head, &n1.link);
    fjx_list_add_tail(&list.head, &n2.link);

    ck_assert_ptr_eq(list.head.next, &n1.link);
    ck_assert_ptr_eq(n1.link.next, &n2.link);
    ck_assert_ptr_eq(n2.link.next, &list.head);

    ck_assert_ptr_eq(list.head.prev, &n2.link);
    ck_assert_ptr_eq(n2.link.prev, &n1.link);
    ck_assert_ptr_eq(n1.link.prev, &list.head);
}
END_TEST

START_TEST (test_list_add_list)
{
    fjx_list_init(&list.head);
    fjx_list_init(&olist.head);

    fjx_list_add(&list.head, &n1.link);
    fjx_list_add(&list.head, &n2.link);

    fjx_list_add(&olist.head, &n3.link);
    fjx_list_add(&olist.head, &n4.link);

    fjx_list_add_list(&list.head, &olist.head);

    ck_assert_ptr_eq(list.head.next, &n4.link);
    ck_assert_ptr_eq(n4.link.next, &n3.link);
    ck_assert_ptr_eq(n3.link.next, &n2.link);
    ck_assert_ptr_eq(n2.link.next, &n1.link);
    ck_assert_ptr_eq(n1.link.next, &list.head);

    ck_assert_ptr_eq(list.head.prev, &n1.link);
    ck_assert_ptr_eq(n1.link.prev, &n2.link);
    ck_assert_ptr_eq(n2.link.prev, &n3.link);
    ck_assert_ptr_eq(n3.link.prev, &n4.link);
    ck_assert_ptr_eq(n4.link.prev, &list.head);
}
END_TEST

START_TEST (test_list_add_list_tail)
{
    fjx_list_init(&list.head);
    fjx_list_init(&olist.head);

    fjx_list_add(&list.head, &n1.link);
    fjx_list_add(&list.head, &n2.link);

    fjx_list_add(&olist.head, &n3.link);
    fjx_list_add(&olist.head, &n4.link);

    fjx_list_add_list_tail(&list.head, &olist.head);

    ck_assert_ptr_eq(list.head.next, &n2.link);
    ck_assert_ptr_eq(n2.link.next, &n1.link);
    ck_assert_ptr_eq(n1.link.next, &n4.link);
    ck_assert_ptr_eq(n4.link.next, &n3.link);
    ck_assert_ptr_eq(n3.link.next, &list.head);

    ck_assert_ptr_eq(list.head.prev, &n3.link);
    ck_assert_ptr_eq(n3.link.prev, &n4.link);
    ck_assert_ptr_eq(n4.link.prev, &n1.link);
    ck_assert_ptr_eq(n1.link.prev, &n2.link);
    ck_assert_ptr_eq(n2.link.prev, &list.head);
}
END_TEST

START_TEST (test_list_replace)
{
    fjx_list_init(&list.head);
    fjx_list_init(&olist.head);

    fjx_list_add(&list.head, &n1.link);
    fjx_list_add(&list.head, &n2.link);
    fjx_list_add(&list.head, &n3.link);
    fjx_list_add(&list.head, &n4.link);

    fjx_list_replace(&list.head, &olist.head);

    ck_assert_ptr_eq(olist.head.next, &n4.link);
    ck_assert_ptr_eq(n4.link.next, &n3.link);
    ck_assert_ptr_eq(n3.link.next, &n2.link);
    ck_assert_ptr_eq(n2.link.next, &n1.link);
    ck_assert_ptr_eq(n1.link.next, &olist.head);

    ck_assert_ptr_eq(olist.head.prev, &n1.link);
    ck_assert_ptr_eq(n1.link.prev, &n2.link);
    ck_assert_ptr_eq(n2.link.prev, &n3.link);
    ck_assert_ptr_eq(n3.link.prev, &n4.link);
    ck_assert_ptr_eq(n4.link.prev, &olist.head);
}
END_TEST

START_TEST (test_list_unlink)
{
    fjx_list_init(&list.head);

    fjx_list_add(&list.head, &n1.link);
    fjx_list_add(&list.head, &n2.link);
    fjx_list_add(&list.head, &n3.link);

    fjx_list_unlink(&n2.link);

    ck_assert_ptr_eq(list.head.next, &n3.link);
    ck_assert_ptr_eq(n3.link.next, &n1.link);
    ck_assert_ptr_eq(n1.link.next, &list.head);

    ck_assert_ptr_eq(list.head.prev, &n1.link);
    ck_assert_ptr_eq(n1.link.prev, &n3.link);
    ck_assert_ptr_eq(n3.link.prev, &list.head);
}
END_TEST

Suite *test_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("List");

    tc_core = tcase_create("Core");

    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_list_init);
    tcase_add_test(tc_core, test_list_add);
    tcase_add_test(tc_core, test_list_add_tail);
    tcase_add_test(tc_core, test_list_add_list);
    tcase_add_test(tc_core, test_list_add_list_tail);
    tcase_add_test(tc_core, test_list_replace);
    tcase_add_test(tc_core, test_list_unlink);
    suite_add_tcase(s, tc_core);

    return s;
}
