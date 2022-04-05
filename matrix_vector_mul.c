/*
Minyoung Kim (Mar. 25, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>


    
int main(){ 
    omp_set_num_threads(3);
    int N=5, M=10;
    int A[N][M];
    int x[M], y[N];
    int i, j, k, sum;




    #pragma omp parallel shared(N, M, A, x, y)
    {
        //initialize A 
        #pragma omp for private(j) 
        for(i=0; i<N; i++){
            for(j=0; j<M; j++){
                A[i][j]=1;
            }
        }
        //initialize x 
        #pragma omp for
        for(j=0; j<M; j++){
            x[j]=j+1;
        }
        
        // calculate y = Ax
        #pragma omp for private(j, sum) schedule(dynamic, 2)
        for(i=0; i<N; i++){
            sum = 0.0;
            for(j=0; j<M; j++){
                sum+=A[i][j]*x[j];
            }
            y[i]=sum;
            printf("Thread num = %d , calculated %d\n", omp_get_thread_num(), i);
        }
    }

    for(i=0; i<N; i++){
        printf("%d\n", y[i]);
    }

}