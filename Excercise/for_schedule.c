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
    int N = 17;
    int n = 7;

    #pragma omp parallel
    {   
        #pragma omp master 
            printf("Test schedule, type = static\n");
        #pragma omp barrier
        //schedule: default type = static, chunk_size = int(17/5)+1 = 4 
        // 4개, 4개, 3개, 3개 3개 로 나뉘어 들어감 
        #pragma omp for 
        for(int i=0; i<N; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        }
    }
    #pragma omp parallel  
    { 
        #pragma omp master 
            printf("Test schedule, type = dynamic\n");
        #pragma omp barrier
        
        //일이 끝나면 바로 다음 for문을 실행하도록 nowait 지정 
        #pragma omp for nowait
        for(int i=0; i<n; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        }
        
        // 각 thread 는 할당된 작업이 완료될때까지 수행하고, 다 끝난 애들부터 작업이 할당됨. 
        // chunk size 만큼씩 할당됨 (default chunk size = 1 )
        // 위 for 문에서 nowait 이 있으므로, 5개 thread 중 먼저 끝난 thread 에 일을 2개씩 배정 
        #pragma omp for schedule(dynamic, 2)
        for(int i=0; i<n; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i+100);
        }
    }
    #pragma omp parallel 
    {
        #pragma omp master 
            printf("Test schedule, type = guided\n");
        #pragma omp barrier
        // 일할 수 있는 thread 에 작업을 할당.
        // 배분하는 일 chunk 크기가 점점 줄어듬.
        // 초기값은 지정한 값(default = 1), 
        //각 thread 에 처음 할당된 작업이 완료되면 크기에서 일정한 비율로 줄어든 크기만큼 재할당 
        #pragma omp for schedule(guided, 3)
        for(int i=0; i<20; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        }
    }    
    #pragma omp parallel 
    {
        #pragma omp master 
            printf("Test schedule, type = auto\n");
        #pragma omp barrier
        // 시스템이 가장 적합하다고 판단하는 스케줄링으로 설정 
        #pragma omp for schedule(auto)
        for(int i=0; i<20; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        }
    }

    /* 이 부분은 어떻게 하는건지 잘 모르겠다 
    setenv('OMP_SCHEDULE', 'static, 2', 1);
    #pragma omp parallel 
    {
        #pragma omp master 
            printf("Test schedule, type = runtime\n");
        #pragma omp barrier
        //환경변수에 의해 조정됨 (runtime 에서 결정) 
        #pragma omp for schedule(runtime)
        for(int i=0; i<20; i++){
            printf("Thread num = %d, i = %d\n", omp_get_thread_num(), i);
        }
    }*/

}
