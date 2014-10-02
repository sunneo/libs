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


#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
typedef struct TimeMeasureMent{
   const char* name;
   int count;
   double start;
   double end;
   double len;
   struct TimeMeasureMent* next;
}TimeMeasureMent;

TimeMeasureMent root = {"root",0,0,0,0, NULL };
TimeMeasureMent* tail = &root;

static double getDoubleTime(){
   struct timeval tv;
   gettimeofday(&tv,0);
   return ((double)tv.tv_sec) + 1e-6*tv.tv_usec;
}
static void __timemeasure_start(const char* name,TimeMeasureMent* w){
   w->name = name;
   w->start = getDoubleTime();
   if(!w->count){
	  tail->next = w;
	  tail = w;
   }
   w->count += 1;

}
static void __timemeasure_end(TimeMeasureMent* w){
   w->end = getDoubleTime();
   w->len += w->end - w->start;
}
void print_measure_ment(){
   TimeMeasureMent* p = &root;
   printf("name,count,time,avg\n");
   while(p){
	  if(p->count > 0){
         printf("%-20s,%-10d,%-10.6f,%-10.6f\n",
	       	p->name,p->count,p->len,p->len/p->count*1000);
	  }
	  p = p->next;
   }
}
#define __Measure_I(FN) \
   static struct TimeMeasureMent FN##_measure; 

#define __Measure_S(FN) \
   __timemeasure_start(#FN ,&(FN##_measure));
#define __Measure_E(FN) \
   __timemeasure_end(&(FN##_measure));

#define __INS_Measure_S(FN) \
   __timemeasure_start("insert."#FN ,&(FN##_measure));
#define __INS_Measure_E(FN) \
   __timemeasure_end(&(FN##_measure));

#define __DEL_Measure_S(FN) \
   __timemeasure_start("delete."#FN ,&(FN##_measure));
#define __DEL_Measure_E(FN) \
   __timemeasure_end(&(FN##_measure));

#define Measure_I(FN) __Measure_I(FN)
#define Measure_S(FN) __Measure_S(FN)
#define Measure_E(FN) __Measure_E(FN)


#define INS_Measure_S(FN) __INS_Measure_S(FN)
#define INS_Measure_E(FN) __INS_Measure_E(FN)
#define DEL_Measure_S(FN) __DEL_Measure_S(FN)
#define DEL_Measure_E(FN) __DEL_Measure_E(FN)


#include "bpt.h"


#define FND_VERSION FIND_VERSION
static
int bpt_node_full(bpt_node* n)
{
	if (n->is_leaf)
		return n->nkeys == BPT_ORDER;
	else
		return n->nkeys == (BPT_ORDER - 1);
}

extern int bpt_node_find_pos_pth(bpt_node* n,void* key);
extern int bpt_node_find_pos_cuda(bpt_node* n,void* key);
extern int bpt_node_find_pos_cuda_eq(bpt_node* n,void* key);


static int bpt_node_find_pos_plain(bpt_node* n,void *key){
   long i;
   for(i=0; i< n->nkeys ; ++i){
	  long thiskey = ((long)n->keys[ i ]);
	  if(((long)key) < thiskey){
		 return i;
	  }
   }
   return i;
}

static int bpt_node_find_pos_plain_eq(bpt_node* n,void *key){
   long i;
   for(i=0; i< n->nkeys ; ++i){
	  if(((long)n->keys[ i ]) == ((long)key) ){
		 return i;
	  }
   }
   return -1;
}



static
int bpt_node_find_pos(bpt_node* n,void* key,int eq){
   int ret = 0;
   if(n == NULL){
	  return 0;
   }
   Measure_I(bpt_node_find_pos)
   Measure_I(bpt_node_find_pos_pth)
   Measure_I(bpt_node_find_pos_cuda)
   Measure_S(bpt_node_find_pos)	 
#if FIND_VERSION < 2
   Measure_S(bpt_node_find_pos_pth)
   if(!eq){
	  if(n->nkeys >= 128){
		 ret = bpt_node_find_pos_pth(n,key)+1;
	  }
	  else{
		 ret = bpt_node_find_pos_plain(n,key);
	  }
      //ret = bpt_node_find_pos_plain(n,key);

   }
   else{
      if(n->nkeys >= 128){
     	  ret = bpt_node_find_pos_pth_eq(n,key);
   	  }
	  else{
		  ret = bpt_node_find_pos_plain_eq(n,key);
	  }
   }
   Measure_E(bpt_node_find_pos_pth)
#elif FIND_VERSION == 2
   if(!eq){
      ret = bpt_node_find_pos_plain(n,key);
   }
   else{
      ret =  bpt_node_find_pos_plain_eq(n,key);
   }
#elif FIND_VERSION > 2
   Measure_S(bpt_node_find_pos_cuda)
   if(!eq){
	  ret =  bpt_node_find_pos_cuda(n,key)+1;
      //ret = bpt_node_find_pos_plain(n,key);

   }
   else{
	  if(n->nkeys >= 128){
    	  ret =  bpt_node_find_pos_cuda_eq(n,key);
	  }
	  else{
		  ret =  bpt_node_find_pos_plain_eq(n,key);
	  }
   }
   Measure_E(bpt_node_find_pos_cuda)
#endif
   Measure_E(bpt_node_find_pos)

   return ret;
}

bpt_node* bpt_find_leaf(bpt* b, void* key)
{
	long i;
	bpt_node* n;
	long (*cmp)(void*, void*);
	cmp = BPT_CMP(b);

	for (n = b->root; !n->is_leaf; n = n->ptrs[i]){
		//for (i = 0; i < n->nkeys && cmp(key, n->keys[i]) >= 0; i++);
	   i = bpt_node_find_pos(n,key,0);
	}
	return n;
}


void** bpt_find(bpt* b, void* key)
{
	long i;
	bpt_node* n;
	long (*cmp)(void*, void*);
	cmp = BPT_CMP(b);

	n = bpt_find_leaf(b, key);
	i = bpt_node_find_pos(n,key,1);
	if(i!=-1){
	   return n->ptrs+i;
	}
/*	for (i = 0; i < n->nkeys; i++)
		if (!cmp(key, n->keys[i]))
			return n->ptrs + i;
*/	
	return NULL;
}


bpt_node* bpt_first_leaf(bpt* b)
{
	bpt_node* n;

	for (n = b->root; !n->is_leaf; n = n->ptrs[0]);
	return n;
}

bpt_node* bpt_last_leaf(bpt* b)
{
	bpt_node* n;

	for (n = b->root; !n->is_leaf; n = n->ptrs[n->nkeys]);
	return n;
}

void* bpt_first_key(bpt* b)
{
	bpt_node* n;

	n = bpt_first_leaf(b);
	if (n->nkeys)
		return n->keys[0];
	return NULL;
}

void* bpt_first_ptr(bpt* b)
{
	bpt_node* n;

	n = bpt_first_leaf(b);
	if (n->nkeys)
		return n->ptrs[0];
	return NULL;
}


void* bpt_last_key(bpt* b)
{
	bpt_node* n;

	n = bpt_last_leaf(b);
	if (n->nkeys)
		return n->keys[n->nkeys - 1];
	return NULL;
}

void* bpt_last_ptr(bpt* b)
{
	bpt_node* n;

	n = bpt_last_leaf(b);
	if (n->nkeys)
		return n->ptrs[n->nkeys - 1];
	return NULL;
}

void** bpt_export_keys(bpt* b)
{
	long i, j, k;
	void** keys;
	bpt_node* n;

	if (!b->num_entries)
		return NULL;
	keys = malloc(b->num_entries * sizeof(*keys));
	if (!keys)
		return NULL;
	for (i = 0, n = bpt_first_leaf(b); n; n = n->next)
		for (j = 0, k = n->nkeys; j < k; i++, j++)
			keys[i] = n->keys[j];
	return keys;
}

void** bpt_export_ptrs(bpt* b)
{
	long i, j, k;
	void** ptrs;
	bpt_node* n;

	if (!b->num_entries)
		return NULL;
	ptrs = malloc(b->num_entries * sizeof(*ptrs));
	if (!ptrs)
		return NULL;
	for (i = 0, n = bpt_first_leaf(b); n; n = n->next)
		for (j = 0, k = n->nkeys; j < k; i++, j++)
			ptrs[i] = n->ptrs[j];
	return ptrs;
}

void bpt_map(bpt* b, void (*fn)(void*, void*, void*), void* extra)
{
	long i, j;
	bpt_node* n;

	for (n = bpt_first_leaf(b); n; n = n->next)
		for (i = 0, j = n->nkeys; i < j; i++)
			fn(n->keys[i], n->ptrs[i], extra);
}


static
void** simple_insertion(bpt* b, bpt_node* n, void* k, void* d)
{
	long i, j;
	long (*cmp)(void*, void*);
	Measure_I(simple_insertion)
	INS_Measure_S(simple_insertion)
	cmp = BPT_CMP(b);
	j = n->nkeys;
	for (i = 0; i < j && cmp(n->keys[i], k) < 0; i++);
	for (j = BPT_ORDER - 1; j > i; j--) {
		n->keys[j] = n->keys[j - 1];
		n->ptrs[j] = n->ptrs[j - 1];
	}
	n->keys[i] = k;
	n->ptrs[i] = d;
	n->nkeys++;
	INS_Measure_E(simple_insertion)
	return n->ptrs + i;
}


static
void create_new_root(bpt_node* old)
{
	bpt_node* new;

	new = malloc(sizeof(*new));
	new->is_leaf = 0;
	new->is_root = 1;
	new->nkeys = 0;
	new->parent.bpt = old->parent.bpt;

	new->next = NULL;
	new->prev = NULL;

	new->ptrs[0] = old;
	new->parent.bpt->root = new;
	old->parent.node = new;
	old->is_root = 0;
}

static
bpt_node* create_new_neighbor(bpt_node* n)
{
	bpt_node* m;

	m = malloc(sizeof(*m));
	m->is_leaf = n->is_leaf;
	m->is_root = 0;
	m->nkeys = 0;
	m->parent = n->parent;

	if (n->next)
		n->next->prev = m;
	m->next = n->next;
	n->next = m;
	m->prev = n;

	return m;
}

static
void** split_insertion(bpt* b, bpt_node** n_ptr, void** k_ptr, void** d_ptr)
{
	long i, j;
	void *k, *d;
	Measure_I(split_insertion)
	INS_Measure_S(split_insertion)

	bpt_node *n, *m, *new_root;
	void** ret;
	long (*cmp)(void*, void*);

	
	n = *n_ptr;
	k = *k_ptr;
	d = *d_ptr;
	cmp = BPT_CMP(b);

	if (n->is_root)
		create_new_root(n);
	
	m = create_new_neighbor(n);
	
	for (i = 0, j = (BPT_ORDER / 2); i + j < BPT_ORDER; i++) {
		m->keys[i] = n->keys[i + j];
		m->ptrs[i] = n->ptrs[i + j];
	}
	
	n->nkeys = BPT_ORDER / 2;
	m->nkeys = (BPT_ORDER + 1) / 2;
	
	if (n->is_leaf) {
		*k_ptr = m->keys[0];
	} else {
		n->nkeys--;
		m->nkeys--;
		*k_ptr = n->keys[n->nkeys];
	}
	
	if (cmp(*k_ptr, k) < 0)
		ret = simple_insertion(b, m, k, d);
	else
		ret = simple_insertion(b, n, k, d);
	
	if (!n->is_leaf)
		for (i = 0, j = m->nkeys; i <= j; i++)
			((bpt_node*) m->ptrs[i])->parent.node = m;
	for (i = 0; n->parent.node->ptrs[i] != n; i++);
		n->parent.node->ptrs[i] = m;
	*d_ptr = n;
	*n_ptr = n->parent.node;
	INS_Measure_E(split_insertion)
	return ret;
}

void** bpt_insert(bpt* b, void* key, void* data)
{
	void* k;
	void* d;
	bpt_node* n;
	void** ret;
	long i, j;
	Measure_I(bpt_insert);
	INS_Measure_S(bpt_insert);
	b->num_entries++;
	k = key;
	d = data;
	n = bpt_find_leaf(b, key);
	
	if (bpt_node_full(n)){
		ret = split_insertion(b, &n, &k, &d);
	}
	else {
 	   ret = simple_insertion(b, n, k, d);
	   INS_Measure_E(bpt_insert)
	   return ret;

	}


	for (; bpt_node_full(n);)
		split_insertion(b, &n, &k, &d);

	simple_insertion(b, n, k, d);
	INS_Measure_E(bpt_insert);

	return ret;
}



static
long bpt_default_key_cmp(void* a, void* b)
{
	return ((long) a) - ((long) b);
}


bpt* bpt_new(long (*cmp)(void*, void*))
{
	bpt* b;
	bpt_node* n;

	b = malloc(sizeof(*b));
	if (!b)
		return NULL;
	b->num_entries = 0;
	b->cmp = cmp ? cmp : &bpt_default_key_cmp;
	n = malloc(sizeof(*n));
	if (!n) {
		free(b);
		return NULL;
	}
	n->is_leaf = 1;
	n->is_root = 1;
	n->nkeys = 0;
	n->next = NULL;
	n->prev = NULL;
	n->parent.bpt = b;
	
	b->root = n;
	return b;
}



static
void bpt_destroy_node(bpt_node* n)
{
	long i;
	
	if (!n->is_leaf)
		for (i = 0; i <= n->nkeys; i++)
			bpt_destroy_node(n->ptrs[i]);
	free(n);
}

void bpt_destroy(bpt* b)
{
	bpt_destroy_node(b->root);
	free(b);
}


static
void even_extra_left(bpt_node* left, bpt_node* right, void** divider)
{
	long i, j, num;
	
	num = left->nkeys - ((left->nkeys + right->nkeys) / 2);
	for (i = BPT_ORDER - 1; i - num >= 0; i--) {
		right->keys[i] = right->keys[i - num];
		right->ptrs[i] = right->ptrs[i - num];
	}
	
	j = left->nkeys - num + (left->is_leaf ? 0 : 1);
	for (i = 0; i < num; i++) {
		right->keys[i] = left->keys[i + j];
		right->ptrs[i] = left->ptrs[i + j];
	}
	
	left->nkeys -= num;
	right->nkeys += num;
	
	if (left->is_leaf) {
		*divider = right->keys[0];
	} else {
		right->keys[num - 1] = *divider;
		*divider = left->keys[left->nkeys];
		for (i = 0; i < num; i++)
			((bpt_node*) right->ptrs[i])->parent.node = right;
	}
}

static
void even_extra_right(bpt_node* left, bpt_node* right, void** divider)
{
	long i, j, num;
	
	num = right->nkeys - ((left->nkeys + right->nkeys) / 2);
	if (left->is_leaf) {
		*divider = right->keys[num];
	} else {
		left->keys[left->nkeys] = *divider;
		*divider = right->keys[num - 1];
		for (i = 0; i < num; i++)
			((bpt_node*) right->ptrs[i])->parent.node = left;
	}

	j = left->nkeys + (left->is_leaf ? 0 : 1);
	for (i = 0; i < num; i++) {
		left->keys[i + j] = right->keys[i];
		left->ptrs[i + j] = right->ptrs[i];
	}

	left->nkeys += num;
	right->nkeys -= num;

	for (i = 0; i + num < BPT_ORDER; i++) {
		right->keys[i] = right->keys[i + num];
		right->ptrs[i] = right->ptrs[i + num];
	}
}



static
void merge_nodes(bpt_node* left, bpt_node* right, void** divider)
{
	long i, j;
    Measure_I(merge_nodes)
    DEL_Measure_S(merge_nodes)	   
	if (!left->is_leaf) {
		for (i = 0; i <= right->nkeys; i++)
			((bpt_node*) right->ptrs[i])->parent.node = left;
		left->keys[left->nkeys] = *divider;
		left->nkeys++;
		left->ptrs[left->nkeys + right->nkeys] =
		    right->ptrs[right->nkeys];
	}
	
	for (i = 0, j = left->nkeys; i < right->nkeys; i++) {
		left->keys[i + j] = right->keys[i];
		left->ptrs[i + j] = right->ptrs[i];
	}
	
	left->nkeys += right->nkeys;
	
	if (right->next)
		right->next->prev = left;
	left->next = right->next;
	
	free(right);
	DEL_Measure_E(merge_nodes)
}



static
void adjust_root(bpt_node* root)
{
	bpt_node* n;
    Measure_I(adjust_root)
	DEL_Measure_S(adjust_root)
	if (root->nkeys || root->is_leaf){
	    DEL_Measure_E(adjust_root)
		return;
	}

	n = (bpt_node*) root->ptrs[0];
	n->parent = root->parent;
	n->is_root = 1;
	n->parent.bpt->root = n;
	free(root);
	DEL_Measure_E(adjust_root)
}

static
void delete_entry(bpt_node* n, long index)
{
	long i, left_index;
	void** divider;
	bpt_node *pnode, *neighbor, *left, *right;
    Measure_I(delete_entry)
	DEL_Measure_S(delete_entry)
	for (i = index + 1; i < BPT_ORDER; i++) {
		n->keys[i - 1] = n->keys[i];
		n->ptrs[i - 1] = n->ptrs[i];
	}
	n->nkeys--;

	if (n->nkeys >= BPT_MIN_KEYS){
	    DEL_Measure_E(delete_entry)
		return;
	}

	if (n->is_root){
	    DEL_Measure_E(delete_entry)
		return adjust_root(n);
	}

	pnode = n->parent.node;
	for (i = 0; pnode->ptrs[i] != n; i++);

	if (i) {
		neighbor = pnode->ptrs[i - 1];
		left_index = i - 1;
		left = neighbor;
		right = n;
	} else {
		neighbor = pnode->ptrs[1];
		left_index = i;
		left = n;
		right = neighbor;
	}

	divider = pnode->keys + left_index;
	
	if (neighbor->nkeys <= BPT_MIN_KEYS) {
		merge_nodes(left, right, divider);
		pnode->ptrs[left_index + 1] = left;
		delete_entry(pnode, left_index);
	} else if (i) {
		even_extra_left(left, right, divider);
	} else {
		even_extra_right(left, right, divider);
	}
    DEL_Measure_E(delete_entry)

}


void* bpt_delete(bpt* b, void* key)
{
	long i;
	bpt_node* n;
	void* data;	
	long (*cmp)(void*, void*);
	Measure_I(bpt_delete)
	DEL_Measure_S(bpt_delete)
	n = bpt_find_leaf(b, key);
	i = bpt_node_find_pos(n,key,1);
    if(i!=-1){
	   data = n->ptrs[i];
	   b->num_entries--;
	   delete_entry(n, i);
	   DEL_Measure_E(bpt_delete)
	   return data;
	}	   
	/*cmp = BPT_CMP(b);
	for (i = 0; i < n->nkeys; i++) {
		if (!cmp(key, n->keys[i])) {
			data = n->ptrs[i];
			b->num_entries--;
			delete_entry(n, i);
			return data;
		}
	}*/
    DEL_Measure_E(bpt_delete)
	return NULL;
}
