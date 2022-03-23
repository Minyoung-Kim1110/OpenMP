/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>

int main(){

    int a=10, b=20;
    int c, d; 

    omp_set_num_threads(4);
    printf("Before fork a = %d, b = %d, c=%d, d=%d\n\n", a,b,c,d);

    // firstprivate 은 각 thread 마다 초기화해서 전달해주고, private 은 각 thread 마다 공간만 제공 
    #pragma omp parallel firstprivate(a,c) private(b,d)
    {   
        printf("Current thread number %d\n", omp_get_thread_num());
        // 초기화 안 했기에 쓰레기 값이 반복해서 출력됨
        printf("without initialization, firstprivate c=%d\n", c);
        // 초기화 안하고, private 으로 설정해서 각기 다 다른 쓰레기 값이 출력됨 
        printf("without initialization, private d=%d\n", d);
        // 초기화 하고, firstprivate 으로 기존 값을 출력
        printf("with initialization, firstprivate a=%d\n", a);
        //초기화 했지만 private 으로 공간만 할당했으므로 다 다른 쓰레기값이 출력됨 
        printf("with initialization, private b=%d\n\n", b);
        a, b, c, d = 7,7,7,7;
    }
    // 병렬 작업이 끝난 뒤 thread 별로 저장된 내용은 사라짐 <- a,b,c,d 는 초기값과 동일 
    printf("After join a = %d, b = %d, c=%d, d=%d\n\n", a,b,c,d);

    int n=1000;
    #pragma omp parallel shared (n)
    // n 출력 후 n=n+1 이 다른 thread 에 공유됨 
    // 자원이 동시에 접근 가능해서 race condition 발생할수도..! 
        printf("thread num = %d, shared n = %d\n", omp_get_thread_num(), n++);
    //병렬 작업이 끝난 후 바뀐 n 이 그대로 출력됨 
    printf("After parallelized region n = %d", n);
}