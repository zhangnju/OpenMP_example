#include<omp.h>
#include<iostream>

omp_lock_t simple_lock;

int main()
{
	int cores = omp_get_num_procs();
	std::cout << "The num of cores in this device:" << cores << std::endl;

	if (omp_in_parallel() == 0)
		std::cout << "now is in non-parallel region" << std::endl;
#pragma omp parallel 
	{
       #pragma omp master
		{
			if (omp_in_parallel() != 0)
				std::cout << "now is in parallel region" << std::endl;
			std::cout << "The default threads num is " << omp_get_num_threads() << " and the max default thread nums is " << omp_get_max_threads() << std::endl;
			std::cout << "Hello world from " << omp_get_thread_num() << " thread" << std::endl;
		}

	}
	std::cout << "trying to set 2x threads" << std::endl;
	omp_set_num_threads(2 * cores);
#pragma omp parallel  
	{
        #pragma omp critical 
		{
			std::cout << "The threads num is" << omp_get_num_threads() <<" and the max thread nums is"<<omp_get_max_threads()<< std::endl;
			std::cout << "Hello world from " << omp_get_thread_num() << " thread" << std::endl;
		}
		
	}
	omp_set_dynamic(cores > 1);
	if (omp_get_dynamic()!=0)
	{
		std::cout << "thread num changes dynamicly according to system status" << std::endl;
        #pragma omp parallel  
		{
            #pragma omp critical 
			{
				std::cout << "The threads num is" << omp_get_num_threads() << " and the max thread nums is" << omp_get_max_threads() << std::endl;
				std::cout << "Hello world from " << omp_get_thread_num() << " thread" << std::endl;
			}

		}
	}

	omp_init_lock(&simple_lock);
	std::cout << "non-blocking acquire lock" << std::endl;
#pragma omp parallel num_threads(4)
	{
		int tid = omp_get_thread_num();

		while (!omp_test_lock(&simple_lock))
			std::cout<<"Thread "<<tid<<" - trying to acquire simple_lock"<<std::endl;

		std::cout<<"Thread "<<tid<<" - acquired simple_lock"<<std::endl;
		omp_unset_lock(&simple_lock);
		std::cout<<"Thread "<<tid<<" - released simple_lock"<<std::endl;
		
	}

	std::cout << "blocking acquire lock" << std::endl;
#pragma omp parallel num_threads(4)
	{
		int tid = omp_get_thread_num();
		omp_set_lock(&simple_lock);
		std::cout << "Thread " << tid << " - acquired simple_lock" << std::endl;
		omp_unset_lock(&simple_lock);
		std::cout << "Thread " << tid << " - released simple_lock" << std::endl;

	}
	omp_destroy_lock(&simple_lock);
	return 0;
}
