/*
 * File: lookup.c
 * Author: Andy Sayler
 * Project: CSCI 3753 Programming Assignment 2
 * Create Date: 2012/02/01
 * Modify Date: 2012/02/01
 * Description:
 * 	This file contains the reference non-threaded
 *      solution to this assignment.
 *  
 */
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "util.h"
#include "queue.h"

#define NUM_REQTHREADS 10
#define NUM_RESTHREADS 10
#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define SBUFSIZE 1025
#define INPUTFS "%1024s"
#define ALL_TRHEADS = 20

pthread_lock_t "wrtrblck";

int main(int argc, char* argv[]){

	

    /* Local Vars */
    FILE* inputfp = NULL;
    FILE* outputfp = NULL;
    char hostname[SBUFSIZE];
    char errorstr[SBUFSIZE];
    char firstipstr[INET6_ADDRSTRLEN];
    int i;
    
    /* Check Arguments */
    if(argc < MINARGS){
	fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
	fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
	return EXIT_FAILURE;
    }

    /* Open Output File */
    outputfp = fopen(argv[(argc-1)], "w");
    if(!outputfp){
	perror("Error Opening Output File");
	return EXIT_FAILURE;
    }
    
	queue *stringque;
	int stirnQMax = queue_init(stringque, 1025);
    
    /* Loop Through Input Files */
    
    void *readInput(void *thredid) 
	{
		for(i=1; i<(argc-1); i++)
		{
		
				long* tid = threadid;
				long t;
			
				/* Open Input File */
				inputfp = fopen(argv[i], "r");
				if(!inputfp)
				{
					sprintf(errorstr, "Error Opening Input File: %s", argv[i]);
					perror(errorstr);
					break;
				}	
			/*I need to initialize a queue usising the struct provided by the queue.c file. 
			* I need to create a function that reads from the file and places the names into the buffer
			* I need to create a function that pulls from the buffer and refrence the dns look up then write both
			* to the file */

			
				/* Read File and Process*/
					while(fscanf(inputfp, INPUTFS, hostname) > 0)
					{
					
				/*threads should write input names to a the queue*/
				// i will need to impliment my locks here to check if the que is full 
				
					


						queue_node *payload = hostname;
					
						int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
					
						if (queue_is_full==true){
						
						}
					
						stringque.queue_push(stringque, payload);
					
					// after pushing an item into the que we will broadcast
						int pthread_cond_broadcast(pthread_cond_t *cond);

				
					}
			/* Close Input File */
				fclose(inputfp);
			}
		}
	
	void *getFromBuff(void *threadid)
	{
		
	    /* Lookup hostname and get IP string */
	    if(dnslookup(hostname, firstipstr, sizeof(firstipstr))== UTIL_FAILURE)
	    {
		
		fprintf(stderr, "dnslookup error: %s\n", hostname);
		strncpy(firstipstr, "", sizeof(firstipstr));
	    
	    }
	
	    /* Write to Output File */
	    fprintf(outputfp, "%s,%s\n", hostname, firstipstr);
	
	
    }
			

    /* Close Output File */
    fclose(outputfp);
	
	pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    long cpyt[NUM_THREADS];
    
    /* Spawn Request threads */
    for(t=0;t<NUM_REQTHREADS;t++){
	printf("In main: creating thread %ld\n", t);
	cpyt[t] = t;
	rc = pthread_create(&(threads[t]), NULL, readInput, &(cpyt[t]));
	if (rc){
	    printf("ERROR; return code from pthread_create() is %d\n", rc);
	    exit(EXIT_FAILURE);
	}
    }
    /* spawn resolver threads*/
     for(t=0;t<NUM_RESTHREADS;t++){
	printf("In main: creating thread %ld\n", t);
	cpyt[t] = t;
	rc = pthread_create(&(threads[t]), NULL, Function, &(cpyt[t]));
	if (rc){
	    printf("ERROR; return code from pthread_create() is %d\n", rc);
	    exit(EXIT_FAILURE);
	}
    }
    
    /* Wait for All Theads to Finish */
    for(t=0;t<ALL_THREADS;t++){
	pthread_join(threads[t],NULL);
    }
    printf("All of the threads were completed!\n");
    

    /* Last thing that main() should do */
    /* pthread_exit unnecessary due to previous join */ 
    //pthread_exit(NULL);


	

    return EXIT_SUCCESS;
}
