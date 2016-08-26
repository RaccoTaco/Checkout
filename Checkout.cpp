#include <iostream>
#include <fstream>
#include <iomanip>
#include "math.h"
#include "Checkout.h"

#define PI 3.14159265

using namespace std;

int tempclock = 0;

// Not working correctly? + Spewing out very large numbers.
double Checkout::normal(double mean, double sigma)
{
	double returnNormal = 0, saveNormal = 0;
	int numNormals = 0;
	if (numNormals == 0)
	{
		double r1 = (double)rand();
		double r2 = (double)rand();
		returnNormal = sqrt(2 * log(r1)) * cos(2 * PI * r2);
		saveNormal = sqrt(2 * log(r1)) * sin(2 * PI * r2);
	}
	else
	{
		numNormals = 0;
		returnNormal = saveNormal;
	}
	
	return returnNormal * sigma + mean;
}

int Checkout::clockChange(int arr, int dep)
{
	if (dep == 0)
		return arr;
	else if (arr == 0 || arr > dep)
		return dep;
	else
		return arr;
}

int Checkout::mqChange(stack <int> times, int mq)
{
	if (times.size() > mq)
		mq = times.size();
	return mq;
}

int Checkout::lsChange(int ls, int arr, int dep, stack <int> times)
{
	if ((dep == 0 || dep > arr) && times.empty())
		ls = 0;
	else
		ls = 1;
	return ls;
}

void Checkout::changeTemp(int ttempclock)
{
	tempclock = 0;
}

int Checkout::bChange(stack <int> times, int bclock, int clock, int ls, int b)
{
	if (tempclock != 0)
	{
		bclock = b + (clock - tempclock);
		tempclock = clock;
	}
	else
		bclock = clock;

	if (ls == 0 && times.empty())
	{
		b = b;
		tempclock = clock;
	}
	else
		b = bclock;

	return b;
}

stack<int> Checkout::pushOrPop(stack <int> times, int arr, int dep)
{
	
	if (arr == 0 || (!times.empty() && arr > dep))
		times.pop();
	else if (dep == 0 || arr > dep)
		return times;
	else
		times.push(arr);

	if (arr == dep)
		times.pop();

	return times;
}

int Checkout::listSum(list <int> lis)
{
	int sum = 0, p1 = 0, p2 = 0;

	for (list<int>::iterator list_iter = lis.begin(); list_iter != lis.end(); list_iter++)
	{
		if (*list_iter == lis.front())
			p2 = *list_iter;
		else
		{
			p1 = p2;
			p2 = *list_iter;
		}
		
		if (p1 != 0)
			sum = sum + (p2 - p1);
	}

	return sum;
}

int Checkout::timeDiff(list <int> arr, list <int> dep)
{
	int diff = 0, count = 0, size = arr.size();

	list<int>::iterator list_iter = arr.begin();
	list<int>::iterator list_iter2 = dep.begin();
	
	if (dep.size() < arr.size())
	{
		for (list<int>::iterator list_iter2 = dep.begin(); list_iter2 != dep.end(); list_iter2++)
		{
			diff = *list_iter2 - *list_iter;

			if (diff > 4)
				count++;

			list_iter++;
		}
	}
	else
	{
		for (list<int>::iterator list_iter = arr.begin(); list_iter != arr.end(); list_iter++)
		{
			diff = *list_iter - *list_iter2;

			if (diff > 4)
				count++;

			list_iter2++;
		}
	}

	return count;
}

double Checkout::stdDev(list <int> arr, list <int> dep, int finalb)
{
	double diff = 0, sum = 0, part = 0;
	list<int>::iterator list_iter = arr.begin();
	list<int>::iterator list_iter2 = dep.begin();

	if (dep.size() < arr.size())
	{
		for (list<int>::iterator list_iter2 = dep.begin(); list_iter2 != dep.end(); list_iter2++)
		{
			diff = *list_iter2 - *list_iter;
			part = (diff - (double(finalb) / dep.size())) * (diff - (double(finalb) / dep.size()));
			sum = sum + part;

			list_iter++;
		}
	}
	else
	{
		for (list<int>::iterator list_iter = arr.begin(); list_iter != arr.end(); list_iter++)
		{
			diff = *list_iter - *list_iter2;
			part = (diff - (double(finalb) / dep.size())) * (diff - (double(finalb) / dep.size()));
			sum = sum + part;

			list_iter2++;
		}
	}

	return sqrt((1.0 / dep.size()) * sum);
}

void Checkout::printEvent(ofstream &out, int arr, int dep, int clock, int lq, int ls, int evnt, int b, int mq)
{
	out << setw(3) << right << clock << setw(8) << lq << setw(8) << ls;
	string a;

	if (dep == 0)
	{
		a = "(A, " + to_string(arr) + ") (E, " + to_string(evnt) + ")";
		out << "       " << setw(26) << left << a;
	}
	else if (arr == 0)
	{
		a = "(D, " + to_string(dep) + ") (E, " + to_string(evnt) + ")";
		out << "       " << setw(26) << left << a;
	}
	else if (arr > dep)
	{
		a = "(D, " + to_string(dep) + ") (A, " + to_string(arr) + ") (E, " + to_string(evnt) + ")";
		out << "       " << setw(26) << left << a;
	}
	else
	{
		a = "(A, " + to_string(arr) + ") (D, " + to_string(dep) + ") (E, " + to_string(evnt) + ")";
		out << "       " << setw(26) << left << a;
	}

	out << setw(3) << right << b << setw(4) << mq << endl;
}

void Checkout::printHeader(ofstream &out)
{
	out << "CLOCK   LQ(t)   LS(t)      Future Event List          B   MQ" << endl;
	out << "-------------------------------------------------------------" << endl;
}

void Checkout::printCounter(ofstream &out, list <int> a_arr, list <int> a_dep, int clock, int mq, int b, int arr_sum, int count, double dev)
{
	out << "SINGLE SERVER QUEUE SIMULATION - GROCERY STORE CHECKOUT COUNTER" << endl;
	out << setw(8) << " " << "MEAN INTERARRIVAL TIME                       " << double(arr_sum) / a_dep.size() << endl;
	out << setw(8) << " " << "MEAN SERVICE TIME                            " << double(b) / a_dep.size() << endl;
	out << setw(8) << " " << "STANDARD DEVIATION OF SERVICE TIMES          " << dev << endl;
	out << setw(8) << " " << "NUMBER OF CUSTOMERS SERVED                   " << a_dep.size() << endl << endl;
	out << setw(8) << " " << "SERVER UTILIZATION                           " << double(b) / clock << endl;
	out << setw(8) << " " << "MAXIMUM LINE LENGTH                          " << mq << endl;
	out << setw(8) << " " << "AVERAGE RESPONSE TIME                        " << double(clock) / a_dep.size() << " MINUTES" << endl;
	out << setw(8) << " " << "> 4 MINUTES IN SYSTEM                        " << count << endl;
	out << setw(8) << " " << "SIMULATION RUNLENGTH                         " << clock << " MINUTES" << endl;
	out << setw(8) << " " << "NUMBER OF DEPARTURES                         " << a_dep.size() << endl;
}