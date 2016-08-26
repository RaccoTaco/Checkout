//Raquel Trejo

#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include "Checkout.h"

using namespace std;

void PhaseOne();
void PhaseTwo();

int evnt = 100, arr, dep;
int clock = 0, lq = 0, ls = 1, b = 0, mq = 0, bclock = 0, finalclock = 0, finalb = 0;

list <int> a_arr;
list <int> a_dep;

stack <int> times;

ifstream infile;
ofstream out;

Checkout CO;

int main()
{
	infile.open("infile.txt");
	out.open("outfile.txt");

	for (int i = 0; i < 5; i++)
	{
		out << endl << "xxxxxxxxxxxxxxxxxxxxxxxx TEST #" << i + 1 << " DONE xxxxxxxxxxxxxxxxxxxxxxxx" << endl << endl;
		PhaseOne();
		PhaseTwo();
	}

	infile.close();
	out.close();

	system("pause");
	return 0;
}

void PhaseOne()
{
	CO.printHeader(out);

	while (!infile.eof())
	{
		infile >> arr >> dep;

		a_arr.push_back(arr);
		a_dep.push_back(dep);

		CO.printEvent(out, arr, dep, clock, lq, ls, evnt, b, mq);
		times = CO.pushOrPop(times, arr, dep);

		lq = times.size();

		ls = CO.lsChange(ls, arr, dep, times);

		mq = CO.mqChange(times, mq);

		finalclock = clock;
		clock = CO.clockChange(arr, dep);

		finalb = b;
		b = CO.bChange(times, bclock, clock, ls, b);
	}
}

void PhaseTwo()
{
	while (arr <= evnt && dep <= evnt)
	{
		int x = (rand()%3) + 2;
		int y = (rand()%3) + 2;

		if (dep != clock && arr == clock)
			arr = x + arr;
		else if (arr != clock && dep == clock)
			dep = y + dep;
		else if(dep == clock && arr == clock)
		{
			arr = x + arr;
			dep = y + dep;
		}

		a_arr.push_back(arr);
		a_dep.push_back(dep);

		CO.printEvent(out, arr, dep, clock, lq, ls, evnt, b, mq);
		times = CO.pushOrPop(times, arr, dep);

		lq = times.size();

		ls = CO.lsChange(ls, arr, dep, times);

		mq = CO.mqChange(times, mq);

		finalclock = clock;
		clock = CO.clockChange(arr, dep);

		finalb = b;
		b = CO.bChange(times, bclock, clock, ls, b);
	}

	out << endl << endl;

	a_arr.sort();
	a_dep.sort();
	a_arr.unique();
	a_dep.unique();

	int arr_sum = CO.listSum(a_arr);
	int count = CO.timeDiff(a_arr, a_dep);
	double dev = CO.stdDev(a_arr, a_dep, finalb);
	CO.printCounter(out, a_arr, a_dep, finalclock, mq, finalb, arr_sum, count, dev);

	// Restart computing with last arr/dep from PhaseOne
	arr = 0;
	dep = 0;
	b = 0;
	mq = 0;
	ls = 1;
	lq = 0;
	clock = 0;
	a_arr.clear();
	a_dep.clear();
	bclock = 0;
	CO.changeTemp(0);

	while (!times.empty())
		times.pop();

}