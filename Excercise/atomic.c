/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include<stdio.h>
#include<omp.h>

// atomic 동작은 critical 과 비슷
// binary operation, scalar variable 에 대해서만 가능 
int main(){
    int sum=0; 

    //0부터 1000-1까지의 합을 구함 
    #pragma omp parallel for
    for(int i=0; i<1000; i++){
        #pragma omp atomic 
        sum+=i;
    }
    printf("atomic sum: %d \n", sum);
        
    sum = 0;
    // 같은 구문이지만 공유 변수에 동시에 접근해서 합이 제대로 저장되지 않음 
    #pragma omp parallel for
    for(int i = 0 ; i < 1000; i++)
        sum += i;
    printf("not atomic sum : %d\n", sum);
}