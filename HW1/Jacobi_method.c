
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<omp.h>
#include <math.h>
/*
Check your program's speedup with a large matrix.

Describe your effort to enhance efficiency.

Compress your program and report into one file, and upload it. 
(Late submission will give minus points.)*/


int main(){
    

    //Set number of threads to use.
    int num_threads = 4; 
    omp_set_num_threads(num_threads);
    //Let A is N x N matrix, b, x is vectors
    int N = 2; 
    int A[N][N], b[N];
    double x[N];
    //Let the answer of Ax=b as X = (1,1,...,1)
    double X[N];
    //Initialize A and b with random function
    #pragma omp parallel for 
    for(int i=0; i<N; i++){
        //Set diffrent random seed for each row 
        srand(time(NULL)+i);
        b[i]=0;
        X[i]=1;
        x[i]=0; // Set initial x as zeros vector
        for (int j=0; j<N; j++){
            int num = rand() % 100 ; 
            b[i]+=num;
            A[i][j]=num;
        }
        // To make matrix A diagonally dominant, add sum of row to diagonal component and update b 
        A[i][i]+=b[i];
        b[i]+=b[i];
    }

    //print A, b
    printf("b \n");
    for(int i=0; i<N; i++){
        printf("%d \n", b[i]);
    }
    printf("A\n");
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
        printf("%d ", A[i][j]);
        }
        printf("\n");
    }

    //Define tolerance for Jacobi method 
    double tol = 0.00001, err = 1.0, s = 0; 
    int count =0;
    int k;

    // Notation from ppt omp2 
    double start = omp_get_wtime();
    // Execute until err>tol <- until converge
    while(err > tol){
        // Use tmp to update x     
        double tmp[N];
        err = 0 ;
        #pragma omp parallel for 
        for (k=0; k<N; k++){
            s=0;
            for(int j=0; j<N; j++){
                // s = sum _{j} A_{i, j} * x^k_j
                s += ((double)A[k][j]*x[j]);
            }
            //sum _{j !=i} A_{i, j} * x^k_j=sum _{j} A_{i, j} * x^k_j + A_{i, i}*x_i 
            // x_i^{k+1} = 1/A_{i,i} * (b_i - sum _{j !=i} A_{i, j} * x^k_j)
            tmp[k] = (1/(double) A[k][k]) *((double)b[k]-s + (double)A[k][k]*x[k]);
            //Use MSE error 
            err += (x[k]-tmp[k])*(x[k]-tmp[k]);
            //Update x 
            x[k]=tmp[k];
        }
        

        err = err/(double) N;
        printf("At %d th err=%f\n", count, err);
        count +=1;

        if (count >100){
            break;
        }

    }
    //Calculated accuracy 
    double a;
    #pragma omp parallel for reduction(+:a)
    for(int i=0; i<N; i++){
        a += (x[i]-X[i])/X[i];
    }
    a = (1-a/(double) N) * 100;
    
    double end = omp_get_wtime();
    printf("%f\n", end - start);
    printf("accuracy: %f %%", a);
}