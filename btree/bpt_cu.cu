#include <stdio.h>
#include <cuda.h>
#include <sys/time.h>
#include "bpt.h"
#define BLOCKBASIC 64
#if (DEBUG==1)
#define __debug(X) X
#else
#define __debug(X)
#endif

/*texture<int ,1 , cudaReadModeElementType> texturememory;
static __global__ void gpuSearchPosTextureMemory(int key,int *gpu_pos,int size){
    int globalTx = blockIdx.x * blockDim.x + threadIdx.x;

    if ( globalTx<size-1 ){
       if (tex1Dfetch(texturememory, globalTx) == key){
          *gpu_pos = globalTx;
       }
       if (tex1Dfetch(texturememory, globalTx) < key && tex1Dfetch(texturememory, globalTx+1) > key){
          *gpu_pos = globalTx+1;
       }
    }
}*/


static __global__ void gpuSearchPosShmem1(int key,int * gpu_key_arr,int *gpu_pos,int size) {
    int globalTx = blockIdx.x*blockDim.x+threadIdx.x;   //globalTx=row
    if(globalTx<size){
	   if( key >= gpu_key_arr[globalTx] && key < gpu_key_arr[globalTx+1]){
             *gpu_pos = globalTx;
	   }
	}
     
	
	
}

/*
static __global__ void gpuSearchPosShmem1(int key,int* devKey,int* devPos,int size) {
   int i;
   for(i=0; i< size; ++i){
	  int thiskey = devKey[i];
	  if(key < thiskey){
		 devPos[0] = i;
		 return;
	  }
   }

}
*/
static __global__ void gpuSearchPosShmem1EQ(int key,int* devKey,int* devPos,int size) {
    int globalTx = blockIdx.x*blockDim.x+threadIdx.x;   //globalTx=row
    if(globalTx<size){
       if (devKey[globalTx] == key){
		  devPos[0] = globalTx;
       }
	}
	
}


static int* deviceKey;
static int* devicePos;
const static int negative1 = -2;
extern double getDoubleTime();
#define MEASURETIME(X) 
static int searchPosCudaVersion(int key,int *key_arr,int size,int isequal){
    __debug(printf("[%s] [%s] [%d] key:[%d] key_arr:[%p] size:[%d]\n",__FILE__,__FUNCTION__,__LINE__,key,key_arr,size));
	/*if(!isequal){
       if (key_arr[0]>=key){
          return 0;
       }
       if (key_arr[size-1]<key){
          return size;
       }
       else if (key_arr[size-1]==key){
          return size-1;
       }
	}*/

    int totalSize = sizeof(int)*size;
    int pos = 0;
	if(!deviceKey){
       cudaMalloc((void**)&deviceKey,(BPT_ORDER)*sizeof(int));

	}
	if(!devicePos){
	   cudaMalloc((void**)&devicePos,sizeof(int));
	}
	if(!isequal){
	   if(key < key_arr[0]){
		  return -1;
	   }
	   if(key >= key_arr[size-1]){
		  return size-1;
	   }
	}
    if(cudaMemcpy(devicePos,&negative1,sizeof(int),cudaMemcpyHostToDevice) != cudaSuccess){
	   printf("%d cudaMemcpy Failed(%s)\n",__LINE__,cudaGetErrorString(cudaGetLastError()));
	   exit(0); 
	}
    if(cudaSuccess != cudaMemcpy(deviceKey, key_arr, totalSize, cudaMemcpyHostToDevice)){
	   printf("%d cudaMemcpy Failed(%s)\n",__LINE__,cudaGetErrorString(cudaGetLastError()));
	   exit(0);
	}
    dim3 dimBlock(BLOCKBASIC,1,1);
    dim3 dimGrid(size/BLOCKBASIC+((size % BLOCKBASIC)>0),1);
	if(isequal){
       gpuSearchPosShmem1EQ<<<dimGrid,dimBlock>>>(key,deviceKey,devicePos,size);
	}
	else{
	   gpuSearchPosShmem1<<<dimGrid,dimBlock>>>(key,deviceKey,devicePos,size-1);
	   if(cudaGetLastError() != cudaSuccess){
		  printf("kernel launch failed\n");
	   }
	}
	if(cudaSuccess !=   cudaMemcpy(&pos,devicePos,sizeof(int),cudaMemcpyDeviceToHost) ){
	   printf("%d cudaMemcpy Failed(%s)\n",__LINE__,cudaGetErrorString(cudaGetLastError()));
	   exit(0);
	}
	//cudaFree(devicePos);
	cudaFree(deviceKey);
	//devicePos = 0;
	deviceKey = 0;
	if(isequal){
       return pos;
	}
	else{
	   return pos;
	}
}
extern "C" int bpt_node_find_pos_cuda(bpt_node* n,void* key){
   return searchPosCudaVersion((int)key,(int*)n->keys,n->nkeys,0);
}
extern "C" int bpt_node_find_pos_cuda_eq(bpt_node* n,void* key){
   return searchPosCudaVersion((int)key,(int*)n->keys,n->nkeys,1);
}

