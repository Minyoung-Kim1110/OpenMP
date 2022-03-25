/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>
int main(){ 
    omp_set_num_threads(10);
    int n=-1;
    #pragma omp parallel firstprivate(n)
    {
        // master thread 만 동작하게 함 
        #pragma omp master 
        {
            n = omp_get_thread_num();
            printf("%d\n", n);
        }
        // 모든 thread 가 여기 도착할때까지 기다리기
        // 즉, master thread 가 동작하는 것을 기다리고 다음 구문 실행 
        #pragma omp barrier

        // 모든 thread 에서 다음 구문을 순차적으로 실행하는데, 
        // master thread 는 제외해야 하므로 if 문 걸어주고 실행 
        #pragma omp critical (printing)
        if (n == -1){
            printf("%d\n", omp_get_thread_num());
        }

        

    }
}

