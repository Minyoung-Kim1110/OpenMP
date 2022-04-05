/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>

int main(){
    
    int id, N ; 
    float fraction ;
    N=7;
    omp_set_num_threads(N);

    printf("How many threads are we using? A. %d\n", omp_get_num_threads());
    
    printf("Fork!\n");
    
    #pragma omp parallel private(id, fraction) shared(N)
    {
        id = omp_get_thread_num();
        fraction = (float) id/ (float) N ;

        #pragma omp critical(printing)
        {
            printf("Hello, I'm thread %d\n", id);
            printf("fraction is %f\n", fraction);
            printf("How many threads are we using? A. %d\n\n", omp_get_num_threads());
        }
        
    }// implicit barrier is here  

    printf("Join!");



}