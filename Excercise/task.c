/*
Minyoung Kim (Apr. 05, 2022)
2022 Spring PSC class 
Subject: OpenMP
compile: gcc {fileBasenameNoExtension}.c -fopenmp -o ${fileDirname}\\${fileBasenameNoExtension}.exe
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
int main(){

    //from a, find target position and store that position in coord
    int op=1; //1=True, 0 = False 
    int i=0;
    int a[50][10000], j, coord[2], target=7;



    // in matrix a, generate target 
    int x= rand() % 50 ; 
    int y = rand() % 10000; 
    a[x][y]=target ; 


    #pragma omp parallel shared(a, coord, op, target)
    {
        // used task directive in master thread 
        #pragma omp master 
        {
            while (op && i<50){
                //distribute task to not working threads
                #pragma omp task firstprivate(i) private(j)
                {
                    for(j=0; j<10000; j++){
                        if(a[i][j]==target){
                            coord[0]=i;
                            coord[1]=j; 
                            op=0;
                        }
                    }
                }
                i++;
            }
        }

    }

}
