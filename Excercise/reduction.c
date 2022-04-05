/*
Minyoung Kim (Mar. 23, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include<stdio.h>
#include<omp.h>

int main(){

    int i; 
    int sum=0, mul=1;
    int num_threads = omp_get_max_threads();
    int a[num_threads];

    #pragma omp parallel for
    for(int j =0; j<num_threads; j++){
        a[j]=j;
    }

    // 사용할 thread 개수 지정 
    omp_set_num_threads(4);

    //reduction clause 안에 b 는 firstprivate 으로 생성되며 
    //addition 일 경우 b=0, multiplication 일 경우 1 로 생성된다. 
    #pragma omp parallel reduction(+:sum) reduction(*:mul) private(i) shared(a)
    {
        // sum=0, mul=1 로 초기화 되어 있음 
        printf("For each thread, sum = %d, mul = %d\n", sum, mul);
        i = omp_get_thread_num();
        sum =a[i]*a[i];
        mul = mul + a[i]; // =i+1 to avoid 0 

        printf("Thread num = %d, sum element = %d, mul element =%d \n\n", i, sum, mul);
    }
    // join 할 때 reduction (operation:b) operation 이 실행된 값이 앞에 b operation 한 뒤 저장된다.
    //예를 들어, addition 일 경우 b(기존)+(병렬작업 후 b)
    //          multiplication 일 경우 b(기존)*(병렬작업 후 b)

    // sum = sum+0+1+4+9=14, mul = mul*1*2*3*4=24
    printf("The result is sum=%d, mul = %d",sum, mul);
}