#ifndef fjx_utils_list_h
#define fjx_utils_list_h

#include "utils.h"

struct __fjx_list {
    struct __fjx_list *prev;
    struct __fjx_list *next;
};

typedef struct __fjx_list fjx_list;

static inline void fjx_list_init(fjx_list *link) {
    link->prev = link->next = link;
}

static inline void fjx_list_link(fjx_list *prev, fjx_list *next) {
    prev->next = next;
    next->prev = prev;
}

static inline void fjx_list_link_between(fjx_list *prev, fjx_list *next, fjx_list *target) {
    prev->next = target;
    target->prev = prev;
    target->next = next;
    next->prev = target;
}

static inline void fjx_list_add(fjx_list *head, fjx_list *node) {
    fjx_list_link_between(head, head->next, node);
}

static inline void fjx_list_add_list(fjx_list *head, fjx_list *other) {
    fjx_list_link(other->prev, head->next);
    fjx_list_link(head, other->next);
}

static inline void fjx_list_add_tail(fjx_list *head, fjx_list *node) {
    fjx_list_link_between(head->prev, head, node);
}

static inline void fjx_list_add_list_tail(fjx_list *head, fjx_list *other) {
    fjx_list_link(head->prev, other->next);
    fjx_list_link(other->prev, head);
}

static inline void fjx_list_replace(fjx_list *victim, fjx_list *node) {
    fjx_list_link_between(victim->prev, victim->next, node);
}

static inline bool fjx_list_empty(fjx_list *head) {
    return head->prev == head && head->next == head;
}

static inline void fjx_list_unlink(fjx_list *node) {
    fjx_list_link(node->prev, node->next);
}

static inline fjx_list *fjx_list_pop_head(fjx_list *head) {
    fjx_list *it = head->next;
    fjx_list_unlink(it);
    return it;
}

static inline fjx_list *fjx_list_pop_tail(fjx_list *head) {
    fjx_list *it = head->prev;
    fjx_list_unlink(it);
    return it;
}

#define fjx_list_foreach(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define fjx_list_foreach_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); pos = n, n = n->next)

#define fjx_list_foreach_reverse(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define fjx_list_forach_reverse_safe(pos, p, head) \
    for (pos = (head)->prev, p = pos->prev; pos != (head); pos = p, p = p->prev)

#endif
