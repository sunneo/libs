#ifndef QUEUE_H_
#  define QUEUE_H_


#  ifdef DLLLIB
#    ifdef BUILDING_DLL
#      define DLLIMPORT __declspec(dllexport)
#    else 
#      define DLLIMPORT __declspec(dllimport)
#    endif
#  else
#    define DLLIMPORT
#  endif


#  ifndef QueueCall
#    define QueueCall __attribute__((__fastcall__))
#  endif

typedef struct DLLIMPORT Queue{
   void* a;
   unsigned long front,back,size,elesize;
   void* vtbl;
}Queue;

DLLIMPORT Queue* QueueCall queue_create(unsigned long elesize);
DLLIMPORT unsigned long  QueueCall queue_size(const Queue* queue);
DLLIMPORT int    QueueCall queue_empty(const Queue* queue);
DLLIMPORT void*  QueueCall queue_front(const Queue* queue);
DLLIMPORT void   QueueCall queue_push(Queue* queue,const void* data);
DLLIMPORT void   QueueCall queue_pop(Queue* queue);
DLLIMPORT void   QueueCall queue_clear(Queue* queue);
DLLIMPORT void   QueueCall queue_delete(Queue* queue);


#endif
