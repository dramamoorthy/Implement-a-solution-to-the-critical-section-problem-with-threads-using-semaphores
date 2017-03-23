/* Dinesh Ramamoorthy */
/* U51867354 */


/* the program with  mutex lock with 2 threads runs faster than this code with semaphores and two threads */
/* this is due to the fact that the semaphores suffer from overhead caused by the wait and signal functions, thus mutex locks execute faster than the semaphores */

#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>

/*Tested on linux 1/23/11 */
/* compile with gcc -o -pthread filename filename.c  */

struct shared_dat
   {
   int value;     /* shared variable to store result*/
   };

struct rusage mytiming;
struct timeval mytimeval;
struct shared_dat *counter;
int temp;
sem_t sem1;
int value;

/****************************************************************
 * * This function increases the value of shared variable "counter"
 *   by one 2000000 times
 *   ****************************************************************/
void * thread1(void *arg)
{
	int line = 0;
 	int ind_count = 0;
	sem_getvalue(&sem1, &value);
	
	while(value==1)
	{
		
		while (line < 2000000)
		{
	 	    line++;
                //    if((counter->value%100)==0)
		//	counter->value = counter->value+100;
		
        

		    counter->value = counter->value + 1;
		    counter->value = counter->value * 2;
		    counter->value = counter->value / 2;
	         }
          /* Semaphore is implemented here at this point */		
		sem_post(&sem1);
		sem_getvalue(&sem1, &value);
	}
	
	
	 printf("from process1 counter  =  %d\n", counter->value); 
	// printf("from thread1 %d 100 counts were performed.\n", ind_count);
 
	 
 	 return(NULL);
}


/****************************************************************
 * This function increases the value of shared variable "counter"
 * by one 2000000 times
 * ****************************************************************/
void * thread2(void *arg)
{
	int line = 0;	
	sem_getvalue(&sem1, &value);	
	
	while(value<2);

	while(value==2){
	
	while (line < 2000000)
	   {
            line++;

	

/* Critical Section */
	    counter->value = counter->value + 1;
	    counter->value = counter->value * 2;
	    counter->value = counter->value / 2;
   	}	
	    sem_post(&sem1);
	    sem_getvalue(&sem1, &value);
	
	}
	 printf("from process2 counter = %d\n", counter->value); 
	
return(NULL);
}

/* the additional thread, 3 is added here */

void * thread3(void *arg)
{
        int line = 0;
	sem_getvalue(&sem1, &value);
	

	while(value<3);
		

	while(value==3){
        while (line < 2000000)
           {
            line++;

/* Critical Section */
            counter->value = counter->value + 1;
            counter->value = counter->value * 2;
            counter->value = counter->value / 2;
	
 
      }
	  sem_post(&sem1);
	  sem_getvalue(&sem1, &value);
	
		
           
	}
        printf("from process3 counter = %d\n", counter->value);

return(NULL);
}


/****************************************************************
 * *                  Main Body                                    *
 * ****************************************************************/
main()
{

	sem_init(&sem1, 0, 1);	

	getrusage(RUSAGE_SELF, &mytiming);
	mytimeval = mytiming.ru_stime;	        

	int             r=0;
	int 		i;
	int 		shmid;	  /* shared memory ID */
        pthread_t	tid1[1];     /* process id for thread 1 */
        pthread_t	tid2[1];     /* process id for thread 2 */
        pthread_t       tid3[1];
        pthread_attr_t	attr[1];     /* attribute pointer array */

         counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));

	/* initialize shared memory to 0 */
	counter->value = 0 ;
           printf("1 - I am here %d in pid %d\n",r,getpid());
   	   
	fflush(stdout);
 /* Required to schedule thread independently.
 *  Otherwise use NULL in place of attr. */
        pthread_attr_init(&attr[0]);
        pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);  /* system-wide contention */ 

	/* end to schedule thread independently */

/* Create the threads */
        pthread_create(&tid1[0], &attr[0], thread1, NULL);
        pthread_create(&tid2[0], &attr[0], thread2, NULL);
        pthread_create(&tid3[0], &attr[0], thread3, NULL);


/* Wait for the threads to finish */
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
    pthread_join(tid3[0], NULL); 
   
	        printf("from parent counter  =  %d\n", counter->value);
		printf("---------------------------------------------------------------------------\n");
		printf("\t\t	End of simulation\n");
/* getsurage is implemented here */

getrusage(RUSAGE_SELF, &mytiming);
mytimeval = mytiming.ru_stime;

printf("Time used is sec: %d, usec %d\n",mytiming.ru_utime.tv_sec,
mytiming.ru_utime.tv_usec);
printf("System Time used is sec: %d, usec %d\n",mytiming.ru_stime.tv_sec,
mytiming.ru_stime.tv_usec);

		exit(0);
		
}

