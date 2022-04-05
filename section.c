/*
Minyoung Kim (Mar. 25, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>


    
int main(){ 
    int a;
    omp_set_num_threads(3);
    // lastprivate: 마지막 섹션의 변수값이 메인 영역에 있는 변수로 전달됨. 
    #pragma omp parallel sections lastprivate(a)
    {
        // first section 
        #pragma omp section
        {
            printf("This is section 1 with thread %d\n", omp_get_thread_num());
            printf("Initial a is %d\n", a);
            a = 1;
            printf("a is turned to %d\n", a);
        }
        //second section
        #pragma omp section
        {
            printf("This is section 2 with thread %d\n", omp_get_thread_num());
            printf("Initial a is %d\n", a);
            a = 2;
            printf("a is turned to %d\n", a);
        }
        //third section
        #pragma omp section    
        {
            printf("This is section 3 with thread %d\n", omp_get_thread_num());
            printf("Initial a is %d\n", a);
            a = 3;
            printf("a is turned to %d\n", a);
        }
        //fourth section
        #pragma omp section    
        {
            printf("This is section 4 with thread %d\n", omp_get_thread_num());
            printf("Initial a is %d\n", a);
            a = 4;
            printf("a is turned to %d\n", a);
        }
    }
    printf("Final a is %d\n", a);

}
