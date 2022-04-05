/*
Minyoung Kim (Apr. 05, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>
//#include <time.h>
int main(){
    // 1. Number of threads 
    // thread 개수 -> cache 사용과 연관됨 
    // 짝수개 -> L2 cache, # cores per CPU -> L3 cache 
    omp_set_num_threads(16);
    int i, k, N=700; 
    int sum, x[N], y[N], A[N][N]; 
    int a[N], b[N], z[N];
    double start, end;

    //2. Algorithm improvement 
    // rearranging loops, reducing overhead, balancing loads 

    //2-1 loop fusion 
    //before 
    #pragma omp parallel for 
    for(i=0; i<N; i++) a[i]+=b[i];
    #pragma omp parallel for 
    for(k=0; k<N; k++){
        y[k]*=y[k];
        z[k]=a[k]*k;
    }

    //after 
    #pragma omp parallel for 
    for(k=0; k<N; k++){
        y[k]*=y[k];
        if (k<N) a[k]+=b[k];
        // array in cache should be reused if possible!
        z[k]=a[k]*k;
    }

    //2-2 Collapse clause 
    // fusing nested loops, reduced overheads 
    // #pragma omp for collapse(# of loops)

    //2-3 loop fission 
    // Array in cache should be reused if possible 

    //2-4 loop tiling 
    // tiles of data chunks fit for cache <- last way to enhance speed 
    //3. Multi-dimensional array 

    
    
    //Initialize array 
    //#pragma omp parallel for 
    for (i=0; i<N; i++){
        x[i]=1;
        y[i]=0;
        for (k=0; k<N; k++){
            A[i][k]=i+k-1;
        }
    }

    
    //Worse access 
    start = omp_get_wtime();
    //#pragma omp parallel for 
    for(int i=0; i<N; i++){
        sum = 0; 
        for(k=0; k<N; k++) 
            sum+= x[k]*A[k][i];
        y[i]=sum;
    }
    end = omp_get_wtime();

    printf("Worse access time consumed: %f\n", end - start);

    //Better access 
    // 같은 행에 있는 자료들이 cache 내에서 가까이 저장됨 
    start = omp_get_wtime();
    //#pragma omp parallel for 
    for(int i=0; i<N; i++){
        sum = 0; 
        for(k=0; k<N; k++) 
            sum+= A[i][k]*x[k];
        y[i]=sum;
    }
    end = omp_get_wtime();

    printf("Better access time consumed: %f", end - start);

    //  유의미한 차이는 안보임 ㅠ
    

    //4. If clause
    // parallelize if big size 

    
}
