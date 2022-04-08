
/*
compile : g++ jacobi.cpp -fopenmp -o test.exe
*/

#include<stdio.h>
#include<omp.h>
#include<string.h>
#include <cmath>
#include <iostream> 
#include <vector>
#include <ctime>
#include <numeric>
#include <random>
#include <fstream>
#include <iostream>


using namespace std;
struct equations;
struct values;
void free_vector(vector<double> vec);
void free_2d_matrix(vector<vector<double>> mat);
void free_equations(equations M);
auto generate_matrix(int N);
double get_error(vector<double> pred, vector<double> answer);
auto jacobi_method(int N, auto equations, double tol,  int N_chunks);
values get_time(int ave_num, int N, auto equations, double tol);
void print_2Dmatrix(vector<vector<double>> mat);
void print_column_vector(vector<double> vec);

//default_random_engine generator;
//normal_distribution<double> distribution(0.0, 1.0);

struct equations{
        vector<double> b ; 
        vector<vector<double>> A ;
        vector<double> X; //Answer
    };
void free_vector(vector<double> vec){
    vec.clear();
    vector<double> ().swap(vec);
}
void free_2d_matrix(vector<vector<double>> mat){
    mat.clear();
    vector<vector<double>>().swap(mat);
}
void free_equations(equations M){
    free_vector(M.b);
    free_vector(M.X);
    free_2d_matrix(M.A);
    return;
}
auto generate_matrix(int N)
{
    //N is dimension
    //set random seed
    srand((unsigned) time(0));
    //make A, X, b and initialize with 0
    vector<double> b(N, 0) ; 
    vector<vector<double>> A (N, vector<double>(N, 0));
    vector<double> X(N, 0);

    int i;
    double row_sum=0.0;
    #pragma omp parallel shared(X, A, b) firstprivate(row_sum)
    {
        int i;
        #pragma omp for schedule(auto) private(i)
        // generate X 
        for(i=0; i<N; i++){
            X[i]=((double) (rand() % 20)-10.0) / 10.0;
        } //implicit barrier here
        #pragma omp for schedule(auto) private(i)
        for(i=0; i<N; i++){
            for (int j=0; j<N; j++){
                double num =((double) (rand() % 200)-100.0)/ 100.0; 
                A[i][j]=num;
                row_sum +=abs(num);
            }row_sum -=A[i][i];
             // To make matrix A diagonally dominant, add sum of row and 1 to diagonal component 
            A[i][i]=1+row_sum;
            for (int j=0; j<N; j++){
                b[i]+=A[i][j]*X[j];
            }
        }
    }
    return equations {b, A, X};
}
double get_error(vector<double> pred, vector<double> answer){
    //calculate error
    double a=0.0;
    int N = pred.size();
    #pragma omp parallel for reduction(+:a)
    for(int i=0; i<N; i++){
        a += (pred[i]-answer[i])*(pred[i]-answer[i])/(double) N;
    }
    return a;
}
double jacobi_method(int N, equations M, double tol,int N_chunks=16){
    //cout<<"Jacobi_method starts\n";
    double start, end, err;
    //initialize x with zeros. <- fisrt guess
    vector<double> x(N, 0);
     //Solve
    start = omp_get_wtime();
    err = 100.0;
    // Execute until err>tol <- until converge
    while(err > tol){
        // Use tmp to update x     
        double tmp[N], s;
        int j, k;
        err = 0 ;
        #pragma omp parallel shared(M, x, tmp) if(N>1023)
        {   
            //#pragma omp for schedule(static, N_chunks) private(s, k, j)
            //#pragma omp for schedule(dynamic, N_chunks) private(s, k, j)
            #pragma omp for schedule(guided, N_chunks) private(s, k, j)
           // #pragma omp for schedule(auto) private(s, k, j)
            for (k=0; k<N; k++){
                //sum _{j !=i} A_{i, j} * x^k_j=sum _{j} A_{i, j} * x^k_j + A_{i, i}*x_i 
                s=0;
                for(j=0; j<N; j++){
                    s += (M.A[k][j]*x[j]);
                }s-=M.A[k][k]*x[k];
                // x_i^{k+1} = 1/A_{i,i} * (b_i - sum _{j !=i} A_{i, j} * x^k_j)
                tmp[k] =(M.b[k]-s)/M.A[k][k];
                // Use MSE error
                err += (x[k]-tmp[k])*(x[k]-tmp[k])/(double) N;    
            }
            //Update x for next iteration
            //#pragma omp for schedule(static, N_chunks) private(s, k, j)
            //#pragma omp for schedule(dynamic, N_chunks) private(s, k, j)
            #pragma omp for schedule(guided, N_chunks) private(s, k, j)
            //#pragma omp for schedule(auto) private(s, k, j)
            for( k=0; k<N; k++){
                x[k]=tmp[k];
            }
        }
    }
    end = omp_get_wtime();
    return end - start;
}
struct values{
    double mean; 
    double std;
};
values get_time(int ave_num, int N, double tol, int N_chunks=16){
    vector<double> times(ave_num, 0);
    for(int count = 0; count<ave_num; count++){
        equations M = generate_matrix(N);
        times[count] = jacobi_method(N, M, tol, N_chunks);
        free_equations(M);
    }
    double mean = accumulate(times.begin(), times.end(), 0.0) / (double)ave_num;
    double std = inner_product(times.begin(), times.end(), times.begin(), 0.0)/ (double)ave_num;
    std = sqrt(std- mean * mean);
    return values{mean, std};
}
void print_2Dmatrix(vector<vector<double>> mat){
    int N = mat.size();
    int M = mat[0].size();
    for (int i = 0; i < N; i++){
       for (int j = 0; j < M; j++)      
          cout << mat[i][j] << " \n"[j == N-1];
    }
    return;
}
void print_column_vector(vector<double> vec){
    int N = vec.size();
    for(int i=0; i<N; i++){
        cout<< vec[i]<<" \n";
    }
    return;
}

 

// Compare speedup 
// int main(){
//     string filePath = "time.csv"; 
//     vector<int> matrix_dims = {2,4,8,16,32,64,128,256,512, 1024,2048, 4096, 8192};
//     //,1024,2048,4096};
//     //8192,16384}; //14 numbers 
//     vector<int> num_threads = {1, 2, 4, 8, 12, 16, 24, 32};  
//     double tol = 0.00001;
//     int ave_num = 100;
//     ofstream writeFile(filePath.data());
//     if(writeFile.is_open()){
//         for(int i=0; i<matrix_dims.size(); i++){
//             int N = matrix_dims[i];
//             for(int num_thread: num_threads){
//                 omp_set_num_threads(num_thread);
//                 auto val = get_time(ave_num, N, tol);
//                 cout<<N<<", "<<num_thread<<", "<<val.mean<<", "<<val.std<<endl;
//                 writeFile<<N<<","<<num_thread<<","<<val.mean<<","<<val.std<<"\n";
//             }
//         }
//         writeFile.close();
//     } 
    
//     matrix_dims.clear();
//     vector<int> ().swap(matrix_dims);
//     num_threads.clear();
//     vector<int> ().swap(num_threads);
// }


//Compare schedule type
// int main(){
    
//     int matrix_dim = 4096;
//     int num_threads = 32;
//     int ave_num = 100;
//     double tol = 0.00001;
//     vector<int> N_chunks = {2,4,6,8,10};
//     omp_set_num_threads(num_threads);

//     //string filePath = "static.csv";
//     //string filePath = "dynamic.csv";
//     string filePath = "guided.csv";
//     string filePath = "auto.csv";
    
//     ofstream writeFile(filePath.data());
//     if(writeFile.is_open()){
//         for(int n: N_chunks){
//             auto val = get_time(ave_num, matrix_dim, tol, n);
//             writeFile<<n<<","<<val.mean<<","<<val.std<<"\n";
//         }
//         writeFile.close();
//     }

//     auto val = get_time(ave_num, matrix_dim, tol);
//     cout<<val.mean<<","<<val.std<<"\n";

// }

int main(){
    int matrix_dim = 4096;
    int num_threads = 32;
    int ave_num = 100;
    double tol = 0.00001;
    omp_set_num_threads(num_threads);
    auto val = get_time(ave_num, matrix_dim, tol);
    cout<<val.mean<<","<<val.std<<"\n";
}