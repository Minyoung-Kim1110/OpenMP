/*
Minyoung Kim (Mar. 25, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>
int main(){ 

    omp_set_num_threads(5);
    int N = 10;
    
    // do/for directive 는 자동으로 loop 을 나눠서 배정함 
    #pragma omp parallel 
    // 주의: #pragma omp parallel nowait 은 에러. 나눠야 함 
    {
        #pragma omp for nowait
        //C 에서는 이미 한 블록이므로 괄호 없이 바로 for문 적으면 됨
        // no wait 을 통해 암시적 배리어를 제거해서 일을 끝낸 thread 는 다른 일을 할 수 있음 
        for(int i=0; i<N; i++){
            // i=0~9 가 thread 5개에 나뉘어 들어감 
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        } // <- nowait 없이는 여기가 암시적 barrier
        #pragma omp for
        for(int i=0; i<N; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        }
    }

}
