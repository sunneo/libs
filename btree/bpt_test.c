/*
 * Copyright (c) 2011, William Henderson-Frost <willhf@gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "bpt.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void bpt_print_node(bpt_node* n)
{
	long i;
	if (n->is_leaf)
		printf("lf:");
	if (n->is_root)
		printf("rt:");
	printf("<");
	for (i = 0; i < n->nkeys; i++)
		printf("%ld,", (long) n->keys[i]);
	printf("> ");

}


void bpt_print_level(bpt_node* n)
{
	for (; n; n = n->next)
		bpt_print_node(n);
}

void bpt_print(bpt* b)
{
	bpt_node* n;
	long i;
	if (!b->root) {
		printf("bpt is empty\n");
		return;
	}
	for (i = 0, n = b->root; n; n = n->is_leaf ? NULL : n->ptrs[0], i++) {
		printf("level %ld --\t", i);
		bpt_print_level(n);
		printf("\n");
	}
}


void shuffle(long* xs, long n)
{
	long i, j, temp;
	 if (n > 1) {
		  for (i = 0; i < n - 1; i++) {
			 j = i + rand() / (RAND_MAX / (n - i) + 1);
			 temp  = xs[j];
			 xs[j] = xs[i];
			 xs[i] = temp;
		  }
	 }
}


void bpt_empty_check(bpt* b)
{
	assert("empty check failed:" && !b->num_entries && !b->root->nkeys);
}

void bpt_size_check(bpt* b, long size)
{
	assert("size check failed:" && b->num_entries == size);
}

void bpt_query_check(bpt* b, long* xs, long n)
{
	long i;
	void** ptr;

	for (i = 0; i < n; i++) {
		ptr = bpt_find(b, (void*) xs[i]);
		assert("lookup failed:" && ptr && xs[i] == (long) * ptr);
	}
}

void bpt_insert_check(bpt* b, long* xs, long n)
{
	long i;
	void** ptr;
	
	for (i = 0; i < n; i++) {
		ptr = bpt_insert(b, (void*) xs[i], (void*) xs[i]);
		assert("insert failed:" && ptr && xs[i] == (long) *ptr);
	}
}


void  bpt_delete_check(bpt* b, long* xs, long n)
{
	long i;
	void* ptr;

	for (i = 0; i < n; i++) {
		ptr = bpt_delete(b, (void*) xs[i]);
		assert("delete failed:" && ptr && xs[i] == (long) ptr);
	}
}


void integrity(long num)
{
	long* x1;
	long* x2;
	long i;
	bpt* b;	

	b = bpt_new(NULL);
	
	assert("creation failed:" && b);

	x1 = malloc(sizeof(long) * num);
	x2 = malloc(sizeof(long) * num);
	
	for (i = 1; i <= num; i++) {
		x1[i - 1] = i * 2;
		x2[i - 1] = (i * 2) + 1;
	}
	
	shuffle(x1, num);
	shuffle(x2, num);
	bpt_insert_check(b, x1, num);
	bpt_insert_check(b, x2, num);
	bpt_size_check(b, num * 2);
	bpt_query_check(b, x1, num);
	bpt_query_check(b, x2, num);
//	printf("insert1 pass\n");

	shuffle(x1, num);
	bpt_delete_check(b, x1, num);
	bpt_size_check(b, num);
	bpt_query_check(b, x2, num);
//	printf("delete1 pass\n");
/*	shuffle(x1, num);
	bpt_insert_check(b, x1, num);
	bpt_size_check(b, num * 2);
	bpt_query_check(b, x1, num);
	bpt_query_check(b, x2, num);
	printf("insert2 pass\n");

	shuffle(x1, num);
	shuffle(x2, num);
	bpt_delete_check(b, x1, num);
	bpt_delete_check(b, x2, num);
	bpt_empty_check(b);
	printf("delete2 pass\n");
*/
	
	bpt_destroy(b);
	free(x1);
	free(x2);

//	printf("integrity check passed\n");
}

extern void print_measure_ment();
int main(int argc, char** argv)
{
	if (argc < 2) {
		printf("please supply num\n");
		exit(EXIT_FAILURE);
	}
	integrity(atol(argv[1]));
	print_measure_ment();
	return 0;
}




