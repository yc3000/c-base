#include "../hashtable.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct val_obj {
	int val;
	struct hlist_node hentry;
};

int main()
{
	DECLARE_HASHTABLE(busy_hash, 6);
	hash_init(busy_hash);

	struct val_obj *check;

	for (int i = 0; i < 10; ++i) {
		struct val_obj *a = malloc(sizeof(struct val_obj));
		a->val = i;
		hlist_node_init(&a->hentry);

		if (i == 5)
			check = a;

		hash_add(busy_hash, &a->hentry, a);
	}

	/* 找回 */
	struct hlist_head *head =
		&busy_hash[hash_min(check, hash_bits(busy_hash))];

	for (struct hlist_node *p = head->first; p; p = p->next) {
		struct val_obj *inside = hlist_entry(p, struct val_obj, hentry);
		if (inside == check) {
			printf("find: %d\n", inside->val);
			return 0;
		}
	}

	assert(0);
}
