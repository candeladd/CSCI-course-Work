/*
 * File: util.c
 * Author: Andrew Candelaresi
 * Project: CSCI/3753 Programming Assignment 3
 * Create Date: 10/10/2015
 * Description:
 * 	This file contains a multi-threaded solution to
 *      Programming Assignment 3
 *  
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "queue.h"
#include "util.h"
#include "multi-lookup.h"

int threads_status;
int start_resolver_flag = 0;
pthread_mutex_t queue_mutex, out_mutex, requester_mutex;




int main(int argc, char* argv[]) {
	
	/* Local Vars */
	long numofcpus = sysconf(_SC_NPROCESSORS_ONLN);
	int resolver_thread_count = numofcpus;
	if (resolver_thread_count > MAX_RESOLVER_THREADS) {
		resolver_thread_count = MAX_RESOLVER_THREADS;
	} else if (resolver_thread_count < MIN_RESOLVER_THREADS) {
		resolver_thread_count = MIN_RESOLVER_THREADS;
	}
	int num_input_files = argc-2;
	if (num_input_files > MAX_INPUT_FILES) {
		num_input_files = MAX_INPUT_FILES;
	}
	int requester_thread_count = num_input_files;
	pthread_t resolver_threads[resolver_thread_count];
	pthread_t requester_threads[requester_thread_count];
	requester_thread_attr req_attr[requester_thread_count];
	resolver_thread_attr res_attr[resolver_thread_count];
	
	
	pthread_mutex_init(&queue_mutex, NULL);
	pthread_mutex_init(&requester_mutex, NULL);
	pthread_mutex_init(&out_mutex, NULL);

	long t;
	int successful_req[requester_thread_count];
	int rc1, rc2;
	FILE* inputfp = NULL;
    FILE* outputfp = NULL;
    char errorMessage[1024];

	/* Check Arguments */
    if(argc < MINARGS){
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
    }

    /* Initialize Queue */
    queue q;
    int qSize = 30;
    if(queue_init(&q, qSize) == QUEUE_FAILURE){
		fprintf(stderr, "error: queue_init failed!\n");
    }

    /* Open Output File */
    outputfp = fopen(argv[(argc-1)], "w");
    if(!outputfp){
		perror("Error Opening Output File");
		return EXIT_FAILURE;
    }

    /* Go through Input Files and create Requester Threads */
    for (t = 0; t < requester_thread_count; t++) {

    	inputfp = fopen(argv[(1+t)], "r");
    	if(!inputfp){
    		sprintf(errorMessage, "Error Opening Input File %s\n", argv[1+t]);
    		perror(errorMessage);
    		successful_req[t] = 0;
    	} else {
    		/* Display that requester threads will exist */
    		threads_status++;
    		successful_req[t] = 1;
	    	req_attr[t].finput = inputfp;
	    	
	    	req_attr[t].q = &q;
	    	

	    	rc1 = pthread_create(&requester_threads[t], NULL, Requester, &(req_attr[t]));
	    	if (rc1){
			    printf("ERROR; return code from pthread_create() is %d\n", rc1);
			    exit(EXIT_FAILURE);
			}
		}
	}

	if (!threads_status) {
		fclose(outputfp);
		queue_cleanup(&q);
		pthread_mutex_destroy(&queue_mutex);
		pthread_mutex_destroy(&out_mutex);
		return 0;
	}
	/* Create resolver threads */
	for (t = 0; t < resolver_thread_count; t++) {

		res_attr[t].foutput = outputfp;
		
		
		res_attr[t].q = &q;

		rc2 = pthread_create(&resolver_threads[t], NULL, Resolver, &(res_attr[t]));
		if (rc2){
			printf("ERROR; return code from pthread_create() is %d\n", rc2);
		    exit(EXIT_FAILURE);
		}
	}

	for (t = 0; t < requester_thread_count; t++) {
		if (successful_req[t] == 1)
			pthread_join(requester_threads[t], NULL);
	} 

	for (t = 0; t < resolver_thread_count; t++) {
	    pthread_join(resolver_threads[t], NULL);
	} 
	printf("all threads joined!\n");
	
	fclose(outputfp);
	queue_cleanup(&q);
	pthread_mutex_destroy(&queue_mutex);
	pthread_mutex_destroy(&out_mutex);
	return 0;
}
/* Function for our Requester Threads */
void* Requester(void* thread_data) {
	//block out resolver threads till requester threads have run//
	start_resolver_flag = 1;
	requester_thread_attr* attr = thread_data;
	char hostname[MAX_NAME_LENGTH];
	int names_count = 0;

	while(fscanf(attr->finput, INPUTFS, hostname) > 0) {

		pthread_mutex_lock(&requester_mutex);
		while (queue_is_full(attr->q)) {
			usleep((rand()%100));
		}

		char* payload = strdup(hostname);
		pthread_mutex_lock(&queue_mutex);
		if (queue_push(attr->q, payload) == QUEUE_FAILURE) {
			fprintf(stderr, "error: queue_push failed! - %s\n", payload);
		}
		pthread_mutex_unlock(&queue_mutex);
		names_count++;
		pthread_mutex_unlock(&requester_mutex);
	}

	fclose(attr->finput);
	threads_status--;
	printf("Requester thread added %d hostnames to queue.\n", names_count);
	return NULL;
}

/* Function for our Resolver Threads */
void* Resolver(void* thread_data) {
	/* Make sure our requester has time to begin before our resolver
	   so as to avoid our our resolver quitting too soon. */
	 
	while (!start_resolver_flag) {
		usleep((rand()%20));
	}

	resolver_thread_attr* attr = thread_data;
	char hostname[MAX_NAME_LENGTH];
	char firstipstr[ADDRESS_COUNT][MAX_IP_LENGTH];
	char output[MAX_NAME_LENGTH];
	int run_flag = 1;
	int process_count, k = 0;

	while(run_flag) {

		if (!queue_is_empty(attr->q)) {

		    pthread_mutex_lock(&queue_mutex);
		    char* payload = queue_pop(attr->q);
		    strcpy(hostname, payload);
		    free(payload);
		    pthread_mutex_unlock(&queue_mutex);

		    int count = dnslookup(hostname, firstipstr, sizeof(firstipstr[0]));
		    if (count == UTIL_FAILURE){
				fprintf(stderr, "dnslookup error: %s\n", hostname);
				strncpy(firstipstr[0], "", sizeof(firstipstr[0]));
				count = 1;
		    }
		    
		    strcpy(output, hostname);
		    for (k = 0; k < count; k+=3) {
		    	strcat(output, ",");
		    	strcat(output, firstipstr[k]);
		    }

		    pthread_mutex_lock(&out_mutex);
		    fprintf(attr->foutput, "%s\n", output);
		    pthread_mutex_unlock(&out_mutex);
		    process_count++;

		} else {

			if (threads_status == THREADS_FINISHED) {
				run_flag = 0;
			} else {
				/* Wait a certain amount of time for our requester
				   threads to add to the queue */
				usleep((rand()%100)+ 10);
			}
		}
	}
	printf("Resolver thread processed %d hostnames from queue.\n", process_count);
	return NULL;
}
