#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<omp.h>
#include <math.h>

int main(){
    
    int num_threads =5;
    //= omp_get_max_threads();
    int order = 6;
    //int order = 14;
    double seconds_m[order-1][num_threads], seconds_std[order-1][num_threads];
    double start, end, mean[20], M,std;
    double tol = 0.00001, err, s;
    
    for (int n = 1; n<order; n++){
        int N = pow(2, n);
        int A[N][N], b[N];
        double x[N];
        #pragma omp parallel for 
        for(int i=0; i<N; i++){
            srand(time(NULL)+i);
            b[i]=0;
            x[i]=0;
            for (int j=0; j<N; j++){
                int num = rand() % 10 ; 
                b[i]+=num;
                A[i][j]=num;
            }
            A[i][i]+=b[i];
            b[i]+=b[i];
        }
        for(int m = 0; m<num_threads; m++){
            omp_set_num_threads(m);
            M=0;
            for(int count =0; count<30; count++){
                start = omp_get_wtime();
                
                //Jacobi method
                err = 1.0;
                int k;
                // Execute until err>tol <- until converge
                while(err > tol){
                    // Use tmp to update x     
                    double tmp[N];
                    err = 0 ;
                    #pragma omp parallel for 
                    for (k=0; k<N; k++){
                        s=0;
                        for(int j=0; j<N; j++){
                            s += ((double)A[k][j]*x[j]);
                        }
                        tmp[k] = (1/(double) A[k][k]) *((double)b[k]-s + (double)A[k][k]*x[k]);
                        err += (x[k]-tmp[k])*(x[k]-tmp[k]);
                        x[k]=tmp[k];
                    }
                    err = err/(double) N;
                }
                double end = omp_get_wtime();
                mean[count]=end - start;
                M +=end - start;
            }M=M/20.0;
            #pragma omp parallel for reduction(+:std)
            for(int count=0; count<20; count++){
                std+=pow(mean[count]-M, 2);
            }std = sqrt(std/20.0);
            seconds_m[n-1][m]= M;
            seconds_std[n-1][m]=std;
        }   
    }
    for(int n=0; n<order-1; n++){
        for(int m=0; m<num_threads; m++){
            printf("%lf ", seconds_m[n][m]);
        }printf("\n");
    }printf("\n");
    for(int n=0; n<order-1; n++){
        for(int m=0; m<num_threads; m++){
            printf("%lf ", seconds_std[n][m]);
        }printf("\n");
    }


}