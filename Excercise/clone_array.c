/*
Minyoung Kim (Apr. 04, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>

int main(){
    
    int N =  10 ; 
    double a[N], b[N], c[N];
    #pragma omp parallel for 
    for (int i=0; i<N; i++){
        a[i]=0;
        b[i]=0;
        c[i]=i;
    }      
    int i;
    double tmp; 
    //Serial version 
    for(i=0; i< N-1; i++){
        tmp =( b[i]+c[i] )/2.0;
        a[i] = a[i+1] + tmp ; 
    }   


    //Parallel version 
    #pragma omp parallel for 
    for (int i=0; i<N; i++){
        a[i]=0;
        b[i]=0;
        c[i]=i;
    }  
    //배열을 따로 복사해서 병렬화를 용이하게 바꾸기 
    double ta[N];
    #pragma omp for private(i)
    for (i=0; i<N-1; i++)
        ta[i]=a[i+1];
    #pragma omp for private(i, tmp)
    for (i=0; i<N-1; i++){
        tmp =( b[i]+c[i] )/2.0;
        a[i] =ta[i] + tmp ;
    }

    for(int i=0; i< N; i++){
        printf("a[%d]=%f\n", i, a[i]);
    }

}