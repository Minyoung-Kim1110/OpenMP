/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/
#include <stdio.h>
#include <omp.h>

int main(){

    int size = 10; 
    int i;
    int a[size];

    for(i=0; i<size; i++){
        //0,1,2,3,4,5,6,7,8,9 로 구성된 배열 생성 
        a[i]=i;
    }

    int max = a[0];

    //max 값은 모든 thread 가 공유하고, num_threads =5 로 설정 
    //-> 총 10번 돌아가야 하는 for 문을 5개의 thread 로 나눠서 돌림 
    #pragma omp parallel for shared(max) num_threads(5) 
        for(int i=0; i<size; i++){
            // critical 이 있을 경우 한 thread 에서 코드를 실행 후 다른 thread 에서 실행 
            // 없을 경우 모든 thread 가 동시에 실행 -> 모두 max=a[0]=0 으로 읽음 
            // 같은 이름의 critical block 은 한 thread 에서 실행되므로 이름 꼭 지정하기!
            #pragma omp critical(max_condition)
            {
                printf("max=%d, current thread num %d\n", max, omp_get_thread_num());
                if (a[i]>max)
                    max=a[i];
                printf("current i is %d\n", i);
            }
        }
        

}