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
    float a[N], b[N], c[N];
    #pragma omp parallel for 
    for (int i=0; i<N; i++){
        a[i]=i;
        b[i]=0;
        c[i]=i+1;
    }      
    int i; 
    float tot ; 
    //Serial version 
    for (i=1; i<N; i++){
        a[i] = a[i-1] + a[i];
        tot += (b[i]+c[i]) / 2.0;
    }

    #pragma omp parallel for 
    for (int i=0; i<N; i++){
        a[i]=i;
        b[i]=0;
        c[i]=i+1;
    }

    //Parallel version 
    // 병렬화 할 수 없는 부분만 분리 
    for (i=1; i<N; i++){
        a[i] = a[i-1] + a[i];
    }

    #pragma omp parallel for reduction( +:tot) private(i)
    for(i=1; i<N; i++){
        tot += (b[i]+c[i])/2.0;
    }


}
