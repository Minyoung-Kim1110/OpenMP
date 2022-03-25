/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>

int main() {
    // ifdef, endif 를 통해 -fopenmp 옵션이 없을때에는 안에 구문이 동작하지 않도록 설정 
    #ifdef _OPENMP 
    // 사용할 thread 개수 설정 -> N 이라 하자 
    omp_set_num_threads(2);
    #pragma omp parallel
    {
        // 사용하는 thread 개수 전체를 thread개수만큼 print 함. 즉, N을 N번출력
        // 예를 들어서, parallel 인 경우 N을 N번 출력 
        // parallel 이 아닌 경우(순차적인 경우) 1을 1번 출력 
        printf("omp get number of threads %d\n", omp_get_num_threads());

        // 현재 사용중인 thread 의 number 를 출력 0~N-1 숫자들이 랜덤하게 출력됨
        printf("Current using thread number %d\n", omp_get_thread_num());

        // CPU 코어 개수를 N 번 출력 
        printf("Number of process %d\n", omp_get_num_procs());

        //사용가능한 thread 개수를 N번 출력 즉, N 을 N번 출력 
        //set_num_threads 를 안 할 시 코어 개수를 코어 개수 번 출력 
        //사용 가능한 것이기 때문에 순차적이든 parallel 이든 N 을 출력 (순차적일때는 한번만, parallel 일 때는 여러번 출력)
        printf("Number of max threads %d\n", omp_get_max_threads());



    }
    #endif 

    printf("Sequential?");
}
