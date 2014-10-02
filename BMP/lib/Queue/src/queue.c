#include "queue.h"
#include <stdlib.h>

__inline static void* QueueCall queue_array_at(const Queue* queue,unsigned long idx);
__inline static void  QueueCall queue_realloc(Queue* queue);
__inline static void  QueueCall copy_mem(void* dst,const void* src,unsigned long size);

static void* QueueCall queue_front_data(const Queue* queue);
static void* QueueCall queue_front_ptr(const Queue* queue);
static void  QueueCall queue_push_data(Queue* queue,const void* data);
static void  QueueCall queue_push_ptr(Queue* queue,const void* data);

typedef struct FptrTbl{
   void (QueueCall *fn_push)(Queue*,const void*);
   void* (QueueCall *fn_front)(const Queue*);
}FptrTbl;

FptrTbl ftbl[2] = { 
    {queue_push_data,queue_front_data}, 
    {queue_push_ptr ,queue_front_ptr} 
};

__inline static void QueueCall
copy_mem(void* dst,const void* src,unsigned long size){
   while(size>=4){
      *(unsigned long*)dst = *(ulong*)src;
      dst = (void*)((char*)dst + 4);
      src = (const void*)((char*)src + 4);
      size -= 4;
   }
   while(size){
      *(unsigned char*)dst = *(unsigned char*)src;
      dst = (void*)((char*)dst + 1);
      src = (const void*)((char*)src + 1);
      size -= 1;
   }
}


__inline static void* QueueCall
queue_array_at(const Queue* queue,ulong idx){
   return (void*)((char*)queue->a) + (queue->elesize * idx);
}

__inline static void  QueueCall
queue_realloc(Queue* queue){
   queue->size = queue->size * 2 + 1;
   queue->a = realloc(queue->a,queue->size * queue->elesize);
}

Queue* QueueCall 
queue_create(ulong elesize){
   Queue* queue = (Queue*)malloc(sizeof(Queue));
   queue->size = 0;
   queue->front = 0;
   queue->back = 0;
   queue->elesize = (elesize <= 0 ? sizeof(void*) : elesize);
   queue->vtbl = &ftbl[elesize <= 0];
   return queue;
}

unsigned long QueueCall 
queue_size(const Queue* queue){
   return queue->back - queue->front;
}

int QueueCall 
queue_empty(const Queue* queue){
   return queue->front == queue->back;
}

static void* QueueCall 
queue_front_data(const Queue* queue){
   return queue_array_at(queue,queue->front);
}

static void* QueueCall
queue_front_ptr(const Queue* queue){
   return ((void**)queue->a)[ queue->front ];
}

static void QueueCall 
queue_push_data(Queue* queue,const void* data){
   copy_mem(queue_array_at(queue,queue->back),data,queue->elesize);
   ++queue->back;
}


static void QueueCall 
queue_push_ptr(Queue* queue,const void* data){
   ((void**)queue->a)[ queue->back++ ] = (void*)data;  
}

void* QueueCall
queue_front(const Queue* queue){
   return ((FptrTbl*)queue->vtbl)->fn_front(queue);
}

void QueueCall
queue_push(Queue* queue,const void* data){
   if(queue->back == queue->size)
      queue_realloc(queue);
   ((FptrTbl*)queue->vtbl)->fn_push(queue,data);
}

void QueueCall 
queue_pop(Queue* queue){
  if(queue->front == queue->back) return;
  ++queue->front;
  if(queue->front == queue->back)
     queue_clear(queue);
}

void QueueCall 
queue_clear(Queue* queue){
   if(queue->size == 0) return;
   queue->size = 0;
   queue->front = queue->back = 0;
   free(queue->a);
   queue->a = 0;
}

void QueueCall 
queue_delete(Queue* queue){
  queue_clear(queue);
  free(queue);
}
