#include<iostream>
#include<omp.h>

using namespace std;

int x;

#pragma omp threadprivate(x)
int main()
{
	int tid, a;

	omp_set_num_threads(4);
	tid = omp_get_thread_num();
	a = -1;
	x = -2;

	cout << "******1st serial region" << endl;
	cout << "a=" << a << " x=" << x << " tid=" << tid << endl;
	cout << endl;

	cout << "******1st parallel region" << endl;
	cout << endl;
#pragma omp parallel firstprivate(a) private(tid) copyin(x)
	{
		tid = omp_get_thread_num();
		cout << "a=" << a << " x=" << x << " tid=" << tid << endl;
		a = a + tid + 100;
		x = x + tid + 100;
		cout << "a=" << a << " x=" << x << " a and x changed, tid=" << tid << endl;
		
	}
	cout << endl;
	tid = omp_get_thread_num();
	cout << "******2nd serial region" << endl;
	cout << "a=" << a << " x=" << x << " tid=" << tid << endl;
	a = a + tid + 100;
	x = x + tid + 100;
	cout << "a=" << a << " x=" << x << " a and x changed, tid=" << tid << endl;
	cout << endl;
	
	cout << "******2nd parallel region" << endl;
	cout << endl;

#pragma omp parallel firstprivate(a) private(tid)
	{
		tid = omp_get_thread_num();
		cout << "---2nd parallel region before single:a=" << a << " x=" << x << " tid=" << tid << endl;
		cout << endl;

#pragma omp single copyprivate(x,a)
		{
			tid = omp_get_thread_num();
			cout << "---2nd parallel region single block: a=" << a << " x=" << x << " tid=" << tid << endl;
			a = a + tid + 100;
			x = x + tid + 100;
			cout << "---2nd parallel region single block: a=" << a << " x=" << x << " a and x changed, tid=" << tid << endl;
			cout << endl;
		}
		tid = omp_get_thread_num();
		cout << "---2nd parallel region after single: a=" << a << " x=" << x << " tid=" << tid << endl;
	}

	cout << "******3rd serial region" << endl;
	tid = omp_get_thread_num();
	cout << "a=" << a << " x=" << x << " tid=" << tid << endl;

	return 0;
}