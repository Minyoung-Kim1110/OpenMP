/*
Minyoung Kim (Apr. 04, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
int main(){
    
    clock_t start, end;
    double start2, end2;
    double cpu_time_used; 

    int N =  100000; 
    int a[N];

    // Check serial execution time 
    start = clock();
    for (int i=0; i<N; i++){
        a[i]=0;
        for (int j=0; j<N; j++){
            a[i]+=j;
        }
    }      
    end = clock(); 
    cpu_time_used = ((double) end - start)/ CLOCKS_PER_SEC;
    printf("For serial execution, time consumed : %f\n", cpu_time_used);

    // Check parallel execution time 
    start = clock(); 
    start2=omp_get_wtime();
    #pragma omp parallel for schedule(auto)
    for (int i=0; i<N; i++){
        a[i]=0;
        for (int j=0; j<N; j++){
            a[i]+=j;
        }
    }      
    
    end = clock(); 
    end2 = omp_get_wtime();
    cpu_time_used = ((double) end - (double) start)/ CLOCKS_PER_SEC;

    printf("For parallel execution, time consumed : %f\n", cpu_time_used);
    printf("With omp get wtime, time consumed : %f\n", end2 - start2);


}
