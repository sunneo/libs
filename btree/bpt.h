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
 * Credits:
 * 
 * Modification of b plus tree code by Amittai Aviram:
 * www.amittai.com/prose/bplustree.html
 * Released under ISC License with his permission.
 * Also consulted by Jan Jannink's deletion paper:
 * infolab.stanford.edu/~widom/cs346/jannink.pdf
 */

#ifndef BPT_H
#define BPT_H

#define BPT_ORDER 1024
#define BPT_MIN_KEYS  ((BPT_ORDER - 1) / 2)
#define BPT_CMP(B) ((B)->cmp)

struct bpt_node;
struct bpt;

struct bpt_node {
	unsigned char is_leaf;
	unsigned char is_root;
	unsigned long nkeys;
	union {
		struct bpt* bpt;
		struct bpt_node* node;
	} parent;
	struct bpt_node* next;
	struct bpt_node* prev;
	void* keys[BPT_ORDER];
	void* ptrs[BPT_ORDER];
};

struct bpt {
	long num_entries;
	struct bpt_node* root;
	long (*cmp)(void*,void*);
};

typedef struct bpt bpt;
typedef struct bpt_node bpt_node;

/*
 * bpt_new - allocates, initializes, and returns a new bpt
 * @cmp: comparison function which behaves 
 *       cmp(a, b) = 1  => a > b
 *       cmp(a, b) = 0  => a = b
 *       cmp(a, b) = -1 => a < b
 * if cmp is NULL, keys are compared as longs
 */
bpt* bpt_new(long (*cmp)(void*,void*));


/*
 * bpt_find - lookup function which returns a pointer to the location
 * at which the data-pointer associated with 'key' is stored in the bpt
 */
void** bpt_find(bpt* b, void* key);

/*
 * bpt_insert - inserts the key/data pair into the bpt
 * returns a pointer to the location 'data' is stored
 */
void** bpt_insert(bpt* b, void* key, void* data);

/*
 * bpt_delete - removes key/data pair associated with 'key'
 * from the bpt and returns data
 */
void* bpt_delete(bpt* b, void* key);

/*
 * bpt_destroy - frees all nodes, key arrays, ptr arrays, and the bpt
 */
void bpt_destroy(bpt* b);


void**    bpt_export_keys(bpt* b);
void**    bpt_export_ptrs(bpt* b);
void      bpt_map(bpt* b, void (*fn)(void*, void*, void*), void* extra);
bpt_node* bpt_first_leaf(bpt* b);
bpt_node* bpt_last_leaf(bpt* b);
void*     bpt_first_key(bpt* b);
void*     bpt_first_ptr(bpt* b);
void*     bpt_last_key(bpt* b);
void*     bpt_last_ptr(bpt* b);

#endif /* BPT_H */


