
#ifndef MULTI_LOOKUP_H
#define MULTI_LOOKUP_H

#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MIN_RESOLVER_THREADS 1
#define MAX_NAME_LENGTH 1025
#define MAX_IP_LENGTH INET6_ADDRSTRLEN

#define THREADS_FINISHED 0

#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
void* Requester(void* thread_data);
void* Resolver(void* thread_data);

typedef struct requester_thread_attr_s{
    queue* q;
    FILE* finput;
 
} requester_thread_attr;

typedef struct resolver_thread_attr_s{
	queue* q;
	FILE* foutput;
	
} resolver_thread_attr;


#endif
