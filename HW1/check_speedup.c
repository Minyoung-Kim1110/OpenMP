#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<omp.h>
#include <math.h>

int main(){
    
    int num_threads =32;
    //= omp_get_max_threads();
    int order = 14;
    //int order = 14;
    double seconds_m[order-1][num_threads], seconds_std[order-1][num_threads];
    double start, end, mean[40], M, std;
    double tol = 0.00001, err, s;

    char *filename = "time.txt";
    FILE *fp = fopen(filename, "w");
    if (fp ==NULL){
        printf("ERROR");
        return -1;
    }
    
    for (int n = 1; n<order; n++){
        int N = pow(2, n);
        int A[N][N], b[N];
        double x[N];
        for(int m = 0; m<num_threads; m++){
            omp_set_num_threads(m+1); // 1 ~ num_threads 
            M=0; // average value 
            // Repeat 30 times to get average 
            for(int count =0; count<40; count++){
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
                end = omp_get_wtime();
                mean[count]=(end - start);
                M =1/((double)count + 1.0 ) *(end - start) + (double) count/((double)count +1.0 ) * M;
            }
            
            std=0;
            #pragma omp parallel for reduction(+:std) shared(M) 
            for(int count=0; count<40; count++){
                std+=pow(mean[count]-M, 2);
            }std = sqrt(std/40.0);
            printf("%d,%d,%.8f,%.8f\n", N, m+1, M, std);  
            fprintf(fp, "%d,%d,%.8f,%.8f\n", N, m+1, M, std);            
            //seconds_m[n-1][m]= M;
            //seconds_std[n-1][m]=std;
        }
        /*for(int m=0; m<num_threads; m++){
            printf("%.5f ", seconds_m[n-1][m]);
        }printf("\n<- mu\n");
        for(int m=0; m<num_threads; m++){
            printf("%.5f ", seconds_std[n-1][m]);
        }printf("\n<- std\n");*/

    }
    fclose(fp);
    /*
    for(int n=0; n<order-1; n++){
        for(int m=0; m<num_threads; m++){
            printf("%lf ", seconds_m[n][m]);
        }printf("\n");
    }printf("\n");
    for(int n=0; n<order-1; n++){
        for(int m=0; m<num_threads; m++){
            printf("%lf ", seconds_std[n][m]);
        }printf("\n");
    }*/


}