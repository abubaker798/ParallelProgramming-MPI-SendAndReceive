#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <stdlib.h>

int main(int argc , char * argv[])
{
    int lenth;
    int *arr;
    int index = 0;
    int start_index = 0;
    int rem ;
    int max;
    int rank;		/* rank of process	*/
	int np;			    /* number of process	*/
	int epp;            /* element per process */
	int source;		   /* rank of sender	*/
	int dest;		   /* rank of reciever	*/
	int tag = 0;		/* tag for messages	*/
	char message[100];	/* storage for message	*/
	MPI_Status status;	/* return status for 	*//* recieve		*/

	/* Start up MPI */
	MPI_Init( &argc , &argv );

	/* Find out process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Find out number of process */
	MPI_Comm_size(MPI_COMM_WORLD, &np);


	if( rank == 0)
	{
	    printf("Hello from master process.\nNumber of slave processes is %d", np);

	    printf("\n\nPlease enter size of array...\n");

        scanf("%d", &lenth);

        arr = (int*)malloc(lenth * sizeof(int));
        int i = 1;

        for(i ; i < np ; i++)
          MPI_Send(&lenth, 1, MPI_INT, i , tag , MPI_COMM_WORLD);
	}

	else{
        MPI_Recv(&lenth , 1 , MPI_INT, 0 ,tag, MPI_COMM_WORLD, &status);
        arr = (int*)malloc(lenth * sizeof(int));
	}

	if(rank == 0){

       epp = lenth / np;
       rem = lenth % np;

       int i = 0;
       printf("\n\nPlease enter array elements ...\n");
  	   for(i ; i < lenth ; i++){
             scanf("%d" , &arr[i]);
        }

	    i = 0;
	    max = arr[0];
	    for(i ; i < epp ; i++){
            if(max < arr[i]){
                  max = arr[i];
                  index = i;
            }
	    }

	    i = 1;
	    start_index+=epp;
        // for remender
        int epp_rem = epp + rem;

           for(i; i < np ; i++, start_index+=epp){
            MPI_Send(&lenth, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(arr, lenth, MPI_INT, i, tag, MPI_COMM_WORLD);
            MPI_Send(&start_index, 1, MPI_INT, i, tag, MPI_COMM_WORLD);

            if(i == np - 1)
               MPI_Send(&epp_rem, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
            else
                MPI_Send(&epp, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
	    }
    }

	else
	{
       MPI_Recv(&lenth, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status );
       MPI_Recv(arr, lenth, MPI_INT, 0, tag, MPI_COMM_WORLD, &status );
       MPI_Recv(&start_index, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status );
       MPI_Recv(&epp, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status );

       // Take Frist number is max
       max = arr[start_index];

       int i = start_index;

       // number of loop
       epp+=start_index;
       int count = 0;
       // get max number and its index
       for(i; i < epp ; i++, count++){
          if(max < arr[i]){
            max = arr[i];
            index = count;
          }
       }

       /* create message */
           sprintf(message, "Hello from slave#%d Max number in my partition is %d and index is %d.", (rank+1) , max, index);

           // Send mesaage to master
           MPI_Send( message, strlen(message)+1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

           MPI_Send(&max, 1 , MPI_INT, 0, tag, MPI_COMM_WORLD);

	}

	if(rank == 0){
         int temp = max;
         printf("\nHello from slave#%d Max number in my partition is %d and index is %d.\n", (rank+1) , max, index);
         for( source = 1; source < np ; source++)
		 {

            MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status );
            MPI_Recv(&max, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status );
            printf("%s\n" , message);

           if(max > temp)
               temp = max;
        }

       index = 0;
       for(index ; index < lenth ; index++){
           if(temp == arr[index])
               break;
       }

        printf("\nMaster process announce the final max which is %d and its index is %d.\n", temp , index);
        printf("\nThanks for using our program\n\n");
	}

    /* shutdown MPI */
	MPI_Finalize();
	return 0;
}
