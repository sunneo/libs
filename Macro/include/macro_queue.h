#ifndef MACRO_QUEUE_H_
#define MACRO_QUEUE_H_
#include "src/src_macro_queue.h"
/*********** (Macro_Queue built by IceCold-Sunneo) *************************************
 *
 *  
 *  There're following manipulations in this library 
 *
 *  QueueIsEmpty(PTR__Queue)   
 *     true if queue is empty 
 *  QueueIsFull(PTR__Queue) 
 *     true if queue is full (under restrict mode)
 *  QueueIsRestrict(PTR__Queue)
 *     true if queue is under restrict mode
 *  QueueSetRestrictSize(PTR__Queue,__RESSIZE) 
 *     restrict a queue under _RESSIZE (set to restricted mode )
 *  QueueSetRestrict(PTR__Queue)
 *     restrict a queue right current
 *  QueueCreate(PTR__Queue)          
 *     allocate space for queue structure
 *  QueueDestroy(PTR__Queue)
 *     deallocate the  queue
 *  QueueInit(PTR__Queue)                      
 *     to initialize a queue ( if not, all the operation will not work )
 *  QueuePushback(PTR__Queue,__DATA)
 *     pushback the data into queue
 *  QueuePush(PTR__Queue,__DATA)
 *     just a alias to pushback              
 *  QueuePop(PTR__Queue)
 *     pop up current data from queue                       
 *  QueuePopTo(PTR__Queue,PTR__CONTAINER)
 *     pop currnet data to the address of container
 *  QueueGetCurrent(PTR__Queue)                
 *     get the current data from queue
 *  QueueGetSize(PTR__Queue)
 *     to know how much elements of the queue
 *  QueueDefine(TYPE)                          
 *     define type of queue
 *  QueueDeclare(TYPE)                         
 *     declare a queue with type spacific
 *       ex:   

----------------demo.c------------------------
#include <stdlib.h>
#include <stdio.h>
#include "macro_queue.h"
#define N 3
typedef char *__lpSTR;
QueueDefine(__lpSTR);
QueueDefine(int);
__lpSTR constName[N] = { "Sunneo","IceCold","Reiko" };
int main()
{
   int i=0;
   QueueDeclare(__lpSTR)* qlpstr = QueueCreate(qlpstr);
   QueueDeclare(int)* queue_i = QueueCreate(queue_i);
   QueueSetRestrictSize(queue_i,4);
   for(i=0; i<N; ++i)  QueuePush(qlpstr,constName[ i ]);
   while(!QueueIsEmpty(qlpstr))
   {
      puts(QueueGetCurrent(qlpstr));
      QueuePop(qlpstr);
   }
   for(i=0; i<1000; ++i)  QueuePush(queue_i,i);
   while(!QueueIsEmpty(queue_i))
   {
      printf("%d ",QueueGetCurrent(queue_i));
      QueuePop(queue_i);
   }
   system("pause");
   return 0;
}
--------------------------------------------------
 *
 **********************************************************************************/



#define QueueIsEmpty(PTR__Queue)                                                                              _MACRO_QueueIsEmpty(PTR__Queue)
#define QueueIsFull(PTR__Queue)                                                                               _MACRO_QueueIsFull(PTR__Queue)
#define QueueIsRestrict(PTR__Queue)                                                                           _MACRO_QueueIsRestrict(PTR__Queue)
#define QueueSetRestrictSize(PTR__Queue,__RESSIZE)                                                            _MACRO_QueueSetRestrictSize(PTR__Queue,__RESSIZE)
#define QueueSetRestrict(PTR__Queue)                                                                          _MACRO_QueueSetRestrict(PTR__Queue)

#define QueueCreate(PTR__Queue)                                                                               _MACRO_QueueCreate(PTR__Queue)
#define QueueDestroy(PTR__Queue)                                                                              _MACRO_QueueDestroy(PTR__Queue)
#define QueueInit(PTR__Queue)                                                                                 _MACRO_QueueInit(PTR__Queue) 

#define QueuePushback(PTR__Queue,__DATA)                                                                      _MACRO_QueuePushback(PTR__Queue,__DATA)
#define QueuePush(PTR__Queue,__DATA)                                                                          QueuePushback(PTR__Queue,__DATA)
#define QueuePop(PTR__Queue)                                                                                  _MACRO_QueuePop(PTR__Queue)
#define QueuePopTo(PTR__Queue,PTR__CONTAINER)                                                                 _MACRO_QueuePopTo(PTR__Queue,PTR__CONTAINER)
#define QueueGetCurrent(PTR__Queue)                                                                           _MACRO_QueueGetCurrent(PTR__Queue)
#define QueueGetSize(PTR__Queue)                                                                              _MACRO_QueueGetSize(PTR__Queue)
#define QueueDefine(TYPE)                                                                                     _MACRO_QueueDefine(TYPE)
#define QueueDeclare(TYPE)                                                                                    _MACRO_QueueDeclare(TYPE)



#endif
