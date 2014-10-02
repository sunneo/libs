#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "bpt.h"
#define THREADSNUM 4
#define EVEN 0

#if (DEBUG==1)
#define __debug(X) X
#else
#define __debug(X)
#endif

/* pData record pthread need information */
struct pData{
   int id; // Pthread ID
   int *key_arr; // <B-tree.h> BTreeNode struct in key address
   int key; // Search key
   int *writeReturnPath;
   int iseq;
   volatile int* sharedIsFind;
   int size; // Node exist number 
};
typedef struct pData pData;


static 
void MiddlePathTwoV(pData *pD){
     register int i;
     int block = pD->size / THREADSNUM;
	 register int startLoop = (pD->id)*block;
     register int endLoop = startLoop + block;
	 if(endLoop > pD->size){
		endLoop = pD->size;
	 }
     if (pD->id != THREADSNUM-1){
        for (i=startLoop ; i< endLoop; ++i){
            if(*(pD->sharedIsFind)){
               return;
            }
			if(pD->iseq){
               if (pD->key == pD->key_arr[i]){
 	              *(pD->writeReturnPath) = i;
                  *(pD->sharedIsFind) = 1;
                  break;
               }

			}
			else{
               if (pD->key_arr[i] < pD->key && pD->key_arr[i+1] > pD->key){
        	       *(pD->writeReturnPath) = i+1;
                   *(pD->sharedIsFind) = 1;
                  break;
               }
			}
            if (pD->key == pD->key_arr[i]){
 	           *(pD->writeReturnPath) = i;
               *(pD->sharedIsFind) = 1;
               break;
            }
         }
    }
    else{
 	for (i=(THREADSNUM-1)*block ; i<pD->size-1 ; ++i){
            if(*(pD->sharedIsFind)){
               return;
            }
			if(pD->iseq){
                if (pD->key == pD->key_arr[i]){
 	               *(pD->writeReturnPath) = i;
                   break;
                }

			}
			else{
               if (pD->key_arr[i] < pD->key && pD->key_arr[i+1] > pD->key){
 	             *(pD->writeReturnPath) = i+1;
                 break;
               }
			}
            if (pD->key == pD->key_arr[i]){
 	       *(pD->writeReturnPath) = i;
               break;
            }
        }
    }
}


/*
     First node add a work load.
     pD : Need pthread infomation.
*/
__inline  static void firstPath(pData *pD){
       /* Determine path */
       if (pD->key_arr[0]>=pD->key){
	  *(pD->writeReturnPath) = 0;
          *(pD->sharedIsFind) = 1;
       }
}
/*
    Last node add a work load.
    pD : Need pthread infomation.
*/
__inline static void lastPath(pData *pD){
       /* Determine path */
       if ( pD->key > pD->key_arr[pD->size-1] ){
	  *(pD->writeReturnPath) = pD->size;
          *(pD->sharedIsFind) = 1;
          return;
       }
       /* key equal key_arr[i] must need special handle */
       else if (pD->key == pD->key_arr[pD->size-1]){
	  *(pD->writeReturnPath) = pD->size-1;
          *(pD->sharedIsFind) = 1;
          return;
       }
}

/*
     Final version pthread special function.
     data : Need pthread infomation.
*/
static void *Final_thread_function(void *data){
       /* Sunneo Ans */
       pData *pD = (pData*)data;
       /* First a thread */
       //printf("thread ID : [%d]\n",pD->id);
       if (pD->id == 0){
	  firstPath(pD);	
       }
       if(*(pD->sharedIsFind)){
          return 0;
       }

       /* Each a thread must execute */
       MiddlePathTwoV(pD);
       if(*(pD->sharedIsFind)){
          return 0;
       }

       /* Last a thread */
       if (pD->id == (THREADSNUM-1)){
	  lastPath(pD);
       }
}

static void* bpt_node_find_pos_pth_worker(void* data){
     pData* pD = (pData*)data;
     int i;
     int block = pD->size / THREADSNUM;
	 int startLoop = (pD->id)*block;
     int endLoop = startLoop + block;
	 if(endLoop > pD->size){
		endLoop = pD->size;
	 }
     if (pD->id != THREADSNUM-1){
        for (i=startLoop ; i< endLoop; ++i){
            if(*(pD->sharedIsFind)){
               return;
            }
            if (pD->key_arr[i] <= pD->key && pD->key_arr[i+1] > pD->key){
        	    *(pD->writeReturnPath) = i;
                *(pD->sharedIsFind) = 1;
                return;
            }
         }
    }
    else{
        for (i=(THREADSNUM-1)*block ; i<pD->size ; ++i){
            if(*(pD->sharedIsFind)){
               return;
            }
            if (pD->key_arr[i] <= pD->key && pD->key_arr[i+1] > pD->key){
 	          *(pD->writeReturnPath) = i;
              *(pD->sharedIsFind) = 1;
              return;
            }
        }
    }

}

static void* bpt_node_find_pos_pth_eq_worker(void* data){
     pData* pD = (pData*)data;
     int i;
     int block = pD->size / THREADSNUM;
	 int startLoop = (pD->id)*block;
     int endLoop = startLoop + block;
	 if(endLoop > pD->size){
		endLoop = pD->size;
	 }
     if (pD->id != THREADSNUM-1){
        for (i=startLoop ; i< endLoop; ++i){
            if(*(pD->sharedIsFind)){
               return;
            }
            if (pD->key_arr[i] == pD->key){
        	    *(pD->writeReturnPath) = i;
                *(pD->sharedIsFind) = 1;
                return;
            }
         }
    }
    else{
        for (i=(THREADSNUM-1)*block ; i<pD->size ; ++i){
            if(*(pD->sharedIsFind)){
               return;
            }
            if (pD->key_arr[i] == pD->key){
 	           *(pD->writeReturnPath) = i;
               *(pD->sharedIsFind) = 1;
               return;
            }
        }
    }


}

/*
    Search pthread final version.
    key : search key.
    key_arr : BTreeNode exist key.
    size : BTreeNode key size number. 
*/
static int searchPosFinalVersion(int key,int *key_arr,int size,int iseq){
    __debug(printf("[%s] [%s] [%d] key:[%d] key_arr:[%p] size:[%d]\n",__FILE__,__FUNCTION__,__LINE__,key,key_arr,size));
    pthread_t p_number[THREADSNUM];
    pData pD[THREADSNUM];
    register int id;
    int returnValue =-1;
    volatile int sharedIsFind = 0;
	if(!iseq){
	   returnValue = -2;
       if(key < key_arr[0]){
	      return -1;
       }
	   if(key >= key_arr[size-1]){
	      return size-1;
	   }
	}
    /* Allocation work load */
    for (id=0 ; id<THREADSNUM ; ++id){
        /* Package struct */
        pD[id].id = id;
        pD[id].key_arr = key_arr;
        pD[id].key = key;
        pD[id].sharedIsFind =(volatile int*) &sharedIsFind;
		pD[id].iseq = iseq;
     	pD[id].writeReturnPath = &returnValue;
        pD[id].size = size;
        /* Create pthread */
		if(iseq){
           if (pthread_create(&p_number[id],NULL,bpt_node_find_pos_pth_eq_worker,(void*)&pD[id])!=0){
              printf("Pthread create error !!!\n");
           }

		}
		else{
           if (pthread_create(&p_number[id],NULL,bpt_node_find_pos_pth_worker,(void*)&pD[id])!=0){
              printf("Pthread create error !!!\n");
           }

		}
        /*if (pthread_create(&p_number[id],NULL,Final_thread_function,(void*)&pD[id])!=0){
              printf("Pthread create error !!!\n");
        }*/

    }
    for (id=0 ; id<THREADSNUM ;++id){
        /* Join */
        pthread_join(p_number[id],NULL);
    }
    return returnValue; 
}

int bpt_node_find_pos_pth(bpt_node* n,void* key){
   return searchPosFinalVersion((int)key,(int*)n->keys,n->nkeys,0);
}
int bpt_node_find_pos_pth_eq(bpt_node* n,void* key){
   return searchPosFinalVersion((int)key,(int*)n->keys,n->nkeys,1);
}

