#include "cs537.h"
#include "request.h"
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <assert.h>

volatile int countThreads = 0;

volatile int countProduced = 0;
volatile int countConsumed = 0;

int numRuns = 20;

volatile int* buffer = NULL;

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1;

pthread_cond_t  emptyBuff = PTHREAD_COND_INITIALIZER;
pthread_cond_t  fullBuff = PTHREAD_COND_INITIALIZER;



int buffSize;


// holds data to pass to thread.. 
typedef struct __p4Thread_data
{
	int threadNum;
} p4ThreadData;


void getargs(int *numThreads, int *buffSize, int argc, char *argv[])
{
    if (argc != 3) {
	fprintf(stderr, "Usage: %s <numThreads> <buffSize>\n", argv[0]);
	exit(1);
    }
    *numThreads = atoi(argv[1]);
    *buffSize = atoi(argv[2]); 
}


void threadFunction_count(void* ptr)
{
	//p4ThreadData * thisThreadsData = (p4ThreadData *)ptr;
	//printf("Hello World from thread: %d\n", thisThreadsData->threadNum);

	int i;
	for(i=0;i<10000;i++)
		{
			pthread_mutex_lock(&mutex1);
			countThreads++;
			pthread_mutex_unlock(&mutex1);
		}
	
	pthread_exit(0);
}


int findUnprocessedIndex()
	{
		int i=0;
		for(i=0;i<buffSize;i++)
			{
				if(buffer[i]==1)
					return i;
			}
		return -1;
	}

int findProcessedIndex()
{
	int i=0;
		for(i=0;i<buffSize;i++)
			{
				if(buffer[i]==0)
					return i;
			}
		return -1;
}

void threadFunction_processBuffer_checkMutex(void* ptr)
{
	p4ThreadData * thisThreadsData = (p4ThreadData *)ptr;
	//printf("Hello World from thread: %d\n", thisThreadsData->threadNum);

	int index = 0;
	int i=0;
	while(1)
		{
		pthread_mutex_lock(&mutex1);
		index = findUnprocessedIndex();
		if(index<0)
			{
			printf("No remaining empty buffers..\n");
			pthread_mutex_unlock(&mutex1);
			pthread_exit(0);
			}
		buffer[index] --;
		pthread_mutex_unlock(&mutex1);
	//	printf("Index: %d\n", index);
		printf("thread: %d \t index:%d \t val:%d\n",thisThreadsData->threadNum, index, buffer[index]);
		for(i=0;i<buffSize;i++)
			{
			countThreads++;		
			}
		}
		
		pthread_exit(0);
}


void threadFunction_processBuffer(void* ptr)
{
	p4ThreadData * thisThreadsData = (p4ThreadData *)ptr;
	//printf("Hello World from thread: %d\n", thisThreadsData->threadNum);

	int index = -1;
	int i=0;
	while(1)
		{
		pthread_mutex_lock(&mutex1);
		index = findUnprocessedIndex();
		
		while(index<0)
			{
			if(countConsumed >= numRuns)
				{
				pthread_cond_signal(&emptyBuff);
				printf("thread: %d exitting..\n", thisThreadsData->threadNum);
				pthread_mutex_unlock(&mutex1);
				pthread_exit(0);
				}
			printf("Thread: %d No remaining unprocessed buffers..waiting\n", thisThreadsData->threadNum);
			pthread_cond_wait(&emptyBuff, &mutex1);
			index = findUnprocessedIndex();

			}
		buffer[index] --;
		countConsumed++;
		printf("thread: %d \t index:%d \t val:%d \t consumed: %d\n",thisThreadsData->threadNum, index, buffer[index], countConsumed);
		
		pthread_cond_signal(&fullBuff);
		pthread_mutex_unlock(&mutex1);
	
		}
		pthread_exit(0);
}







int main(int argc, char *argv[])
{

	int numThreads=5;
	buffSize = 100;
	int retVal;
	int i= 0;
	int index = 0;
	
	getargs(&numThreads, &buffSize, argc, argv);
	
	printf("\nNumthreads: %d\t buffSize: %d\n", numThreads, buffSize);
	
	pthread_t* threads;
	p4ThreadData* threadDatas;
	int * retVals;
	
	
	threads = malloc(sizeof(pthread_t)*numThreads);
	threadDatas = (p4ThreadData *)malloc(sizeof(p4ThreadData)*numThreads);
	retVals = malloc(sizeof(int)*numThreads);
	buffer = malloc(sizeof(int)*buffSize);
	
	if(threads==NULL || threadDatas == NULL || retVals == NULL || buffer == NULL)
	{
		printf("Malloc failed..\n");
		return 0;
	}

	retVal = pthread_mutex_init(&mutex1, NULL);
	assert(retVal==0);

	for(i=0;i<buffSize;i++)
	{
		buffer[i] = 1;
		countProduced++;
	}
	
	printf("Before create statements\n");
	
	for(i=0;i<numThreads;i++)
		{
			threadDatas[i].threadNum = i;
			int retVal = pthread_create(&threads[i], NULL, (void *)&threadFunction_processBuffer, &threadDatas[i]);
			if(retVal!=0)
				{
					printf("thread %d failed to create..\n", i);
				}
		}

	//produce
	int count = 1;
	while(1)
	{
		pthread_mutex_lock(&mutex1);
		
		index = findProcessedIndex();
		while(index < 0)
			{
				printf("Thread: main No remaining processed buffers.. waiting\n");
				pthread_cond_wait(&fullBuff, &mutex1);
				index = findProcessedIndex();
			}
		buffer[index] ++;
		countProduced++;
		printf("thread: main \t index:%d \t val:%d \t produced:%d\n", index, buffer[index], countProduced);
		
		
		pthread_cond_signal(&emptyBuff);
		pthread_mutex_unlock(&mutex1);
		
		if(countProduced == numRuns)
			{
				printf("thread: main exitting..\n");
				pthread_cond_signal(&emptyBuff);
				break;
			}	
	}


	for(i=0;i<numThreads;i++)
		{
		//pthread_cond_signal(&emptyBuff);
		pthread_join(threads[i], NULL);
		}

	printf("All threads completed, 10,000*nThreads = countUsingThreads: %d\n", countThreads);


return 0;
}





int main_process_mutex(int argc, char *argv[])
{

	int numThreads=5;
	buffSize = 100;
	int retVal;
	int i= 0;
	
	getargs(&numThreads, &buffSize, argc, argv);
	
	printf("\nNumthreads: %d\t buffSize: %d\n", numThreads, buffSize);
	
	pthread_t* threads;
	p4ThreadData* threadDatas;
	int * retVals;
	
	
	threads = malloc(sizeof(pthread_t)*numThreads);
	threadDatas = (p4ThreadData *)malloc(sizeof(p4ThreadData)*numThreads);
	retVals = malloc(sizeof(int)*numThreads);
	buffer = malloc(sizeof(int)*buffSize);
	
	if(threads==NULL || threadDatas == NULL || retVals == NULL || buffer == NULL)
	{
		printf("Malloc failed..\n");
		return 0;
	}

	retVal = pthread_mutex_init(&mutex1, NULL);
	assert(retVal==0);

	for(i=0;i<buffSize;i++)
	{
		buffer[i] = 1;
	}
	

	printf("Before create statements\n");
	
	for(i=0;i<numThreads;i++)
		{
			threadDatas[i].threadNum = i;
			int retVal = pthread_create(&threads[i], NULL, (void *)&threadFunction_processBuffer, &threadDatas[i]);
			if(retVal!=0)
				{
					printf("thread %d failed to create..\n", i);
				}
		}

	printf("Returned from create statements\n");

	for(i=0;i<numThreads;i++)
		{
		pthread_join(threads[i], NULL);
		}

	printf("All threads completed, 10,000*nThreads = countUsingThreads: %d\n", countThreads);
	
	for(i=0;i<buffSize;i++)
	{
	//	printf("%d : %d\n",i, buffer[i]);
	}


return 0;
}

int main_simple(int argc, char *argv[])
{

    // 
    // CS537: Create some threads...
    //

	pthread_t thread1, thread2;
	p4ThreadData threadData1, threadData2;
	int retVal1, retVal2;
	
	pthread_t * myThreadPtr;
	myThreadPtr = (pthread_t*) malloc(sizeof(pthread_t)*1);
	
	threadData1.threadNum = 1;
	threadData2.threadNum = 2;

	printf("Before create statements\n");
	retVal1 = pthread_create(myThreadPtr, NULL, (void *)&threadFunction_count, &threadData1);
	retVal2 = pthread_create(&thread2, NULL, (void *)&threadFunction_count, &threadData2);
	printf("Returned from create statements\n");

	pthread_join(*myThreadPtr, NULL);
	pthread_join(thread2, NULL);

	printf("RetVal1 = %d\tRetVal2 = %d\tcountThreads = %d\n", retVal1, retVal2, countThreads);


return 0;
}


    


 

