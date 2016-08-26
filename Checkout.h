#include <iostream>
#include <string>
#include <stack>
#include <random>
#include <list>

using namespace std;

class Checkout 
{
private:

public:
	double normal(double mean, double sigma);
	int clockChange(int arr, int dep);
	int mqChange(stack <int> times, int mq);
	int lsChange(int ls, int arr, int dep, stack <int> times);
	int bChange(stack <int> times, int bclock, int clock, int ls, int b);
	stack<int> pushOrPop(stack <int> times, int arr, int dep);

	int listSum(list <int> lis);
	int timeDiff(list <int> arr, list <int> dep);
	double stdDev(list <int> arr, list <int> dep, int finalb);

	void changeTemp(int ttempclock);
	void printEvent(ofstream &out, int arr, int dep, int clock, int lq, int ls, int evnt, int b, int mq);
	void printHeader(ofstream &out);
	void printCounter(ofstream &out, list <int> a_arr, list <int> a_dep, int clock, int mq, int b, int arr_sum, int count, double dev);
};
