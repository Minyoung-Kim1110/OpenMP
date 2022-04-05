/*
Minyoung Kim (Apr. 04, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>
#include <math.h>
// Numerical integration 
// int from 0 to 1 (0,1)
// dx/2 ( f(0) + 2f(1 dx) + ... + 2f((N-1) dx)+f(N dx))

double f(double x){
    double ret = 1/sqrt(1+x*x);
    return ret;
}
int main(){
    int N = 1000;
    double a, b; 
    a= 0, b = 1 ;
    double dx = (b-a) / (double) N ; 
    double s =0; 

    //s = f(0)/2
    s += f(a)/ 2.0 ; 

    int i;
    double x;  
    #pragma omp parallel private(i, x) 
    {
        // s = f(0)/2 + f(1 dx) + ... + f((N-1) dx) 
        #pragma omp for reduction(+:s)
        for(i=1; i<N; i++){
            x = a+(double) i * dx; 
            s+= f(x);
        }
    }
    // s = f(0)/2 + f(1 dx) + ... + f((N-1) dx) + f(N dx)/2
    s += f(b) / 2.0 ; 
    double answer = s * dx ; 
    printf("answer is %f, it should be pi", answer);

}