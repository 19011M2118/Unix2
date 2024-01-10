/***************************************************************************
 *
 * Sequential version of Matrix Inverse
 * An adapted version of the code by Håkan Grahn
 ***************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include<pthread.h>
#define MAX_SIZE 4096
typedef double matrix[MAX_SIZE][MAX_SIZE];
struct thread_data{
    int start;
    int stop;
    int p;
};
int	N;		/* matrix size		*/
int	maxnum;		/* max number of element*/
char* Init;		/* matrix init type	*/
int	PRINT;		/* print switch		*/
matrix	A;		/* matrix A		*/
matrix I = {0.0};  /* The A inverse matrix, which will be initialized to the identity matrix */
/* forward declarations *///
void find_inverse(void);
void Init_Matrix(void);
void Print_Matrix(matrix M, char name[]);
void Init_Default(void);
int Read_Options(int, char**);


pthread_barrier_t barrier;
int
main(int argc, char** argv)
{
    printf("Matrix Inverse\n");
    int i, timestart, timeend, iter;

    Init_Default();		/* Init default values	*/
    Read_Options(argc, argv);	/* Read arguments	*/
    Init_Matrix();		/* Init the matrix	*/
    find_inverse();

    if (PRINT == 1)
    {
        //Print_Matrix(A, "End: Input");
        Print_Matrix(I, "Inversed");
    }
}
void *onlyloop(void *td){
    struct thread_data *data1=(struct thread_data *)td;
    double multiplier;
    int row,col;
    for(row=data1->start;row < data1->stop;row++){
            multiplier = A[row][data1->p];
            if (row != data1->p) // Perform elimination on all except the current pivot row 
            {
                for (col = 0; col < N; col++)
                {
                    A[row][col] = A[row][col] - A[data1->p][col] * multiplier; /* Elimination step on A */
                    I[row][col] = I[row][col] - I[data1->p][col] * multiplier; /* Elimination step on I */
                }      
                assert(A[row][data1->p] == 0.0);
            }
        }
        pthread_barrier_wait(&barrier);
}
void find_inverse()
{
    int row, col, p; // 'p' stands for pivot (numbered from 0 to N-1)
    double pivalue; // pivot value
    pthread_t thread1,thread2,thread3,thread4;
    struct thread_data data[4];
        data[0].start=0;
        data[0].stop=N/4;


        data[1].start=N/4;
        data[1].stop=N/2;


        data[2].start=N/2;
        data[2].stop=3*(N/4);


        data[3].start=3*(N/4);
        data[3].stop=N;
    /* Bringing the matrix A to the identity form */
    for (p = 0; p < N; p++) { /* Outer loop */
        pivalue = A[p][p];
        for (col = 0; col < N; col++)
        {
            A[p][col] = A[p][col] / pivalue; /* Division step on A */
            I[p][col] = I[p][col] / pivalue; /* Division step on I */
        }
        assert(A[p][p] == 1.0);
	data[0].p=p;
	data[1].p=p;
	data[2].p=p;
	data[3].p=p;
	pthread_barrier_init(&barrier,NULL,5);
        pthread_create(&thread1,NULL,onlyloop,&data[0]);
        pthread_create(&thread2,NULL,onlyloop,&data[1]);
        pthread_create(&thread3,NULL,onlyloop,&data[2]);
        pthread_create(&thread4,NULL,onlyloop,&data[3]);
        pthread_barrier_wait(&barrier);
        pthread_join(thread1,NULL);
  	pthread_join(thread2,NULL);	
        pthread_join(thread3,NULL);
        pthread_join(thread4,NULL);
        pthread_barrier_destroy(&barrier);

    
    }
}

void
Init_Matrix()
{
    int row, col;

    // Set the diagonal elements of the inverse matrix to 1.0
    // So that you get an identity matrix to begin with
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (row == col)
                I[row][col] = 1.0;
        }
    }

    printf("\nsize      = %dx%d ", N, N);
    printf("\nmaxnum    = %d \n", maxnum);
    printf("Init	  = %s \n", Init);
    printf("Initializing matrix...");

    if (strcmp(Init, "rand") == 0) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = (double)(rand() % maxnum) + 5.0;
                else
                    A[row][col] = (double)(rand() % maxnum) + 1.0;
            }
        }
    }
    if (strcmp(Init, "fast") == 0) {
        for (row = 0; row < N; row++) {
            for (col = 0; col < N; col++) {
                if (row == col) /* diagonal dominance */
                    A[row][col] = 5.0;
                else
                    A[row][col] = 2.0;
            }
        }
    }

    printf("done \n\n");
    if (PRINT == 1)
    {
        //Print_Matrix(A, "Begin: Input");
        //Print_Matrix(I, "Begin: Inverse");
    }
}

void
Print_Matrix(matrix M, char name[])
{
    int row, col;

    printf("%s Matrix:\n", name);
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++)
            printf(" %5.2f", M[row][col]);
        printf("\n");
    }
    printf("\n\n");
}

void
Init_Default()
{
    N = 5;
    Init = "fast";
    maxnum = 15.0;
    PRINT = 1;
}

int
Read_Options(int argc, char** argv)
{
    char* prog;

    prog = *argv;
    while (++argv, --argc > 0)
        if (**argv == '-')
            switch (*++ * argv) {
            case 'n':
                --argc;
                N = atoi(*++argv);
                break;
            case 'h':
                printf("\nHELP: try matinv -u \n\n");
                exit(0);
                break;
            case 'u':
                printf("\nUsage: matinv [-n problemsize]\n");
                printf("           [-D] show default values \n");
                printf("           [-h] help \n");
                printf("           [-I init_type] fast/rand \n");
                printf("           [-m maxnum] max random no \n");
                printf("           [-P print_switch] 0/1 \n");
                exit(0);
                break;
            case 'D':
                printf("\nDefault:  n         = %d ", N);
                printf("\n          Init      = rand");
                printf("\n          maxnum    = 5 ");
                printf("\n          P         = 0 \n\n");
                exit(0);
                break;
            case 'I':
                --argc;
                Init = *++argv;
                break;
            case 'm':
                --argc;
                maxnum = atoi(*++argv);
                break;
            case 'P':
                --argc;
                PRINT = atoi(*++argv);
                break;
            default:
                printf("%s: ignored option: -%s\n", prog, *argv);
                printf("HELP: try %s -u \n\n", prog);
                break;
            }
}
