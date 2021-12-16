#include <iostream>
#include <omp.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <time.h>

using namespace std;

double find_max_parallel(int n_threads, double* arr, int n, double true_max)
{
    double max_element = arr[0];
	double begin = omp_get_wtime();
    #pragma omp parallel for num_threads(n_threads) reduction(max:max_element)
	    for(int i = 0; i < n; i++)
	        max_element = max(max_element, arr[i]);

	double exec_time = (double)(omp_get_wtime() - begin);
	if (max_element == true_max)
	    return exec_time;
	else
	    return -1;

}

int main(int argc, char** argv)
{
    srand(time(NULL));
    int n_threads = 10;
    long int N = atoi(argv[1]);
	double* arr = new double[N];
	double true_max = 0;
	// fill an array and find true maximum element
	for(int i = 0; i < N; i++)
	{
        arr[i] = (double(rand()) / RAND_MAX);
        true_max = max(true_max, arr[i]);
    }

	for(int i = 1; i <= 10; i++)
	{
	    double total_time = 0;
	    for(int j = 0; j < 10; j++)
	    {
	        double exec_time = find_max_parallel(i, arr, N, true_max);
	        if(exec_time == -1)
	            cout << "Wrong answer for " << i << " threads" << endl;
	        total_time += exec_time;
	    }
	    cout << "Execution time with " << i << " threads is: " << total_time / 10 << endl;
	}
	return 0;
}