#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    int** matrix1;
    int** matrix2;
    int** result;
    int** task;
    int start_row = 0;
    int r1, c1;
    int r2, c2;
    int rank;
    int np;
    int epp;
    int i, j;
    int rem;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (rank == 0) {
        printf("Welcome to vector Matrix multiplication program!\nTo read dimensions and values from file press 1\n");
        printf("To read dimensions and values from console press 2\n");
        int select;
        scanf("%d", &select);

        if (select == 1) {
            FILE *myFile;
            myFile = fopen("matrix.txt", "r");
            //read file into array
            r1, c1, r2, c2;
            fscanf(myFile, "%d", &r1);
            fscanf(myFile, "%d", &c1);
            // allocate matrix

            matrix1 = (int**)malloc(sizeof(int*) * r1);
            i = 0;
            for (i; i < r1; i++)
                matrix1[i] = (int*)malloc(sizeof(int) * c1);

           // read element of first matrix from file
            for (i = 0; i < r1; i++) {
                for (j = 0; j < c1; j++) {
                    fscanf(myFile, "%d", &matrix1[i][j]);
                }
            }


           // for scond matrix
            fscanf(myFile, "%d", &r2);
            fscanf(myFile, "%d", &c2);
            if (c1 != r2) {
                printf("\nThe two matrices cannot be multiplied!");
                printf("\nthe second dimension of the first matrix must be equal to the first dimension of the second matrix\n");
            }

            // allocate matrix

            matrix2 = (int**)malloc(sizeof(int*) * r2);
            for (i = 0; i < r2; i++)
                matrix2[i] = (int*)malloc(sizeof(int) * c2);


            // enter element in matrix1
            for (i = 0; i < r2; i++) {
                for (j = 0; j < c2; j++) {
                    fscanf(myFile, "%d", &matrix2[i][j]);
                }
            }


           // allocat the result matrix
            result = (int**)malloc(sizeof(int*) * r1);
            i = 0;
            for (i; i < r1; i++)
                result[i] = (int*)malloc(sizeof(int) * c2);

            // Distribution the tasks
            epp = r1 / np; // 2
            rem = r1 % np;


            // send all info from master to anthor process
            int epp_rem = epp + rem;
            for (i = 1; i < np; i++) {
                MPI_Send(&r1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&c1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&r2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&c2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                 start_row += epp;
                MPI_Send(&start_row, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

                if(i == np - 1)
                   MPI_Send(&epp_rem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                else
                   MPI_Send(&epp, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

                // send first matrix Row by Row
                for (j = 0; j < r1; j++) {
                    MPI_Send(matrix1[j], c1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }

                // send second matrix Row by Row
                for (j = 0; j < r2; j++) {
                    MPI_Send(matrix2[j], c2 , MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }


            // For First task

            // allocat the task
            task = (int**)malloc(sizeof(int*) * epp); // 2 * 5
            i = 0;
            for (i; i < epp; i++)
                task[i] = (int*)malloc(sizeof(int) * c1);

            // reed elements from first matrix
            for (i = 0; i < epp; i++) {
                for (j = 0; j < c1; j++) {
                    task[i][j] = matrix1[i][j];
                }
            }

           int res = 0;
           int postion = rank;
           int c = 0;
           for (i = 0; i < epp; i++) {
                while (c < c2){
               for (j = 0; j < c1; j++) {
                    res += task[i][j] * matrix2[j][c];
                }result[postion][c++] = res; res = 0;
            } postion++; c = 0;
          }

        }

        else {

            printf("Please enter dimensions of the first matrix: ");

            scanf("%d%d", &r1, &c1);


            // allocate matrix

            matrix1 = (int**)malloc(sizeof(int*) * r1);
            i = 0;
            for (i; i < r1; i++)
                matrix1[i] = (int*)malloc(sizeof(int) * c1);


            // enter element in matrix1

            printf("Please enter its elements:\n");
            i = 0;
            for (i; i < r1; i++) {
                 j = 0;
                for (j; j < c1; j++) {
                    scanf("%d", &matrix1[i][j]);
                }
            }

            // for scond matrix

            printf("Please enter dimensions of the second matrix: ");

            scanf("%d%d", &r2, &c2);

            if (c1 != r2) {
                printf("\nThe two matrices cannot be multiplied!");
                printf("\nthe second dimension of the first matrix must be equal to the first dimension of the second matrix\n");
            }

            // allocate matrix

            matrix2 = (int**)malloc(sizeof(int*) * r2);
            i = 0;
            for (i; i < r2; i++)
                matrix2[i] = (int*)malloc(sizeof(int) * c2);


            // enter element in matrix1

            printf("Please enter its elements:\n");
            i = 0;
            for (i; i < r2; i++) {
                j = 0;
                for (j; j < c2; j++) {
                    scanf("%d", &matrix2[i][j]);
                }
            }


            // allocat the result matrix

            result = (int**)malloc(sizeof(int*) * r1);
            i = 0;
            for (i; i < r1; i++)
                result[i] = (int*)malloc(sizeof(int) * c2);

            // Distribution the tasks
            epp = r1 / np; // 2
            rem = r1 % np;


            // send all info from master to anthor process
            int epp_rem = epp + rem;
            for (i = 1; i < np; i++) {
                MPI_Send(&r1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&c1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&r2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&c2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                 start_row += epp;
                MPI_Send(&start_row, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

                if(i == np - 1)
                   MPI_Send(&epp_rem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                else
                   MPI_Send(&epp, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

                // send first matrix Row by Row
                for (j = 0; j < r1; j++) {
                    MPI_Send(matrix1[j], c1, MPI_INT, i, 0, MPI_COMM_WORLD);
                }

                // send second matrix Row by Row
                for (j = 0; j < r2; j++) {
                    MPI_Send(matrix2[j], c2 , MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            }


            // For First task

            // allocat the task
            task = (int**)malloc(sizeof(int*) * epp); // 2 * 5
            i = 0;
            for (i; i < epp; i++)
                task[i] = (int*)malloc(sizeof(int) * c1);

            // reed elements from first matrix
            for (i = 0; i < epp; i++) {
                for (j = 0; j < c1; j++) {
                    task[i][j] = matrix1[i][j];
                }
            }

           int res = 0;
           int postion = rank;
           int c = 0;
           for (i = 0; i < epp; i++) {
                while (c < c2){
               for (j = 0; j < c1; j++) {
                    res += task[i][j] * matrix2[j][c];
                }result[postion][c++] = res; res = 0;
            } postion++; c = 0;
          }

        }
    }

    else {

        MPI_Recv(&r1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&c1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&r2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&c2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&start_row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&epp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

        // allocate matrix
        matrix1 = (int**)malloc(sizeof(int*) * r1);
        i = 0;
        for (i; i < r1; i++)
            matrix1[i] = (int*)malloc(sizeof(int) * c1);

        // allocate matrix
        matrix2 = (int**)malloc(sizeof(int*) * r2);
        i = 0;
        for (i; i < r2; i++)
            matrix2[i] = (int*)malloc(sizeof(int) * c2);

        // allocat the result matrix
        result = (int**)malloc(sizeof(int*) * r1);
        i = 0;
        for (i; i < r1; i++)
            result[i] = (int*)malloc(sizeof(int) * c2);

        // recive first matrix value by value
       for (i = 0; i < r1; i++) {
           MPI_Recv(matrix1[i], c1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        }

        // recive second matrix value by value
        for (i = 0; i < r2; i++) {
           MPI_Recv(matrix2[i], c2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        }


        // allocat the task
        task = (int**)malloc(sizeof(int*) * epp);
        for (i = 0; i < epp; i++)
            task[i] = (int*)malloc(sizeof(int) * c1);

        // reed elements from first matrix
        int start = start_row;
        for (i = 0; i < epp; i++) {
            for (j = 0; j < c1; j++) {
                task[i][j] = matrix1[start][j];
            } start++;
        }

         int res = 0;
         int postion = rank;
         int c = 0;
         for (i = 0; i < epp; i++) {
             while (c < c2){
                for (j = 0; j < c1; j++) {
                    res += task[i][j] * matrix2[j][c];
                }result[postion][c++] = res; res = 0;
            } postion++; c = 0;
          }


       // send rank to start recive
       MPI_Send(&start_row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
       MPI_Send(&epp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

      // SEND TASKE TO MASTER
       for (i = rank; i < (epp + rank); i++) {
            MPI_Send(result[i], c2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if(rank == 0){

      for(i = 1; i < np; i++){
            MPI_Recv(&start_row, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            MPI_Recv(&epp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

           // RECIV TASKE FROM SLAVE
            for (j = start_row; j < (start_row + epp); j++) {
             MPI_Recv(result[j], c2, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
          }
       }

      printf("\n\nResult Matrix is (%dx%d):\n", r1,c2);
        for (i = 0; i < r1; i++) {
            for (j = 0; j < c2; j++) {
                printf("%d ", result[i][j]);
            }
            printf("\n");
        }
    } printf("\n");

    MPI_Finalize();
    return 0;
}
