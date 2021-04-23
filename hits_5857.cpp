#include<iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include<string>

#include <cmath>
#include <math.h>

#include <iomanip>

#include "hits_5857.h"

using namespace std;

vector<int> hits_5857::Readfile(string filename, string pattern)
{
	string::size_type pos;
	string str;
	vector<int> result;
	ifstream input(filename.c_str(), ios_base::binary);

	if (input)
	{
		while (getline(input, str)) 
		{
			str += pattern;
			string::size_type size = str.size(); 
			for (string::size_type i = 0; i < size; i++)
			{
				pos = str.find(pattern, i);
				if (pos < size)
				{
					string s = str.substr(i, pos - i);
					stringstream ss;
					ss << s;
					int number;
					ss >> number;
					result.push_back(number);
					i = pos + pattern.size() - 1;
				}
			}
		}
	}
	else 
	{
		cout << "NO SUCH FILE" << endl;
		result.clear();
	}

	return result;
}

void hits_5857::CreAdjList(vector< vector<int> >& L, vector< vector<int> >& LT, vector<int> nums)
{
	int N = nums[0];
	int i;
	for (i = 0; i < N; i++)
	{
		vector<int> vec;
		L.push_back(vec);
		LT.push_back(vec);
	}
	vector<int>::iterator iter = nums.begin();
	iter = nums.erase(iter);
	iter = nums.erase(iter);
	for (iter = nums.begin(); iter != nums.end();)
	{
		int n0, n1;
		n0 = *iter;
		iter=iter+1;
		n1 = *iter;
		iter=iter+1;
		L[n0].push_back(n1);
		LT[n1].push_back(n0);
	}

}

int hits_5857::IniVal(vector<double>& auth, vector<double>& hub, double& errorrate, int& iterations, int& initval, int N)
{
	if (N > 10)
	{
		iterations = 0;
		initval = -1;
	}

	if (iterations <= 0) {

		switch (iterations)
		{
		case 0:
			errorrate = pow(10, -5);
			break;
		case -1:
		case -2:
		case -3:
		case -4:
		case -5:
		case -6:
			errorrate = pow(10, iterations);
			break;
		default:
			cout << "ITERATIONS ERROR" << endl;
			return 0;
		}
	}

	double temp;
	switch (initval)
	{
	case 0:
	case 1:
		temp = initval;
		break;
	case -1:
		temp = 1. / N;
		break;
	case -2:
		temp = 1 / sqrt(N);
		break;
	default:
		cout << "INITIAL VALUE ERROR" << endl;
		return 0;
	}
	auth.insert(auth.begin(), N, temp);
	hub.insert(hub.begin(), N, temp);
	return 1;
}

void hits_5857::Result(int index, vector<double> auth, vector<double> hub)
{
	cout << fixed << setprecision(7);
	if (index == 0)
	{
		cout << "Base   :";
	}
	else {
		cout << "Iter   :";
	}
	cout << setw(3) << setfill(' ') << right << index;
	cout << " :";
	for (int i = 0; i < auth.size(); i++)
	{
		cout << " A/H[";
		cout << setw(2) << setfill(' ') << right << i;
		cout << "]=";
		cout << auth[i];
		cout << "/";
		cout << hub[i];
	}
	cout << endl;

}

void hits_5857::Resfinal(int index, vector<double> auth, vector<double> hub)
{
	cout << fixed << setprecision(7);
	cout << "Iter       :";
	cout << setw(3) << setfill(' ') << right << index << endl;
	for (int i = 0; i < auth.size(); i++)
	{
		cout << " A/H[";
		cout << setw(2) << setfill(' ') << right << i;
		cout << "]=";
		cout << auth[i];
		cout << "/";
		cout << hub[i];
		cout << endl;
	}

}

bool hits_5857::Err(vector<double> vec, vector<double> vec_t1, double errorrate)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (abs(vec[i] - vec_t1[i]) > errorrate)
		{
			return false;
		}
	}
	return true;
}

vector<double> hits_5857::Iterone(vector< vector<int> > m, vector<double> v, int N)
{
	vector<double> res(N, 0.);
	vector<int>::iterator iter;
	for (int i = 0; i < N; i++)
	{
		for (iter = m[i].begin(); iter != m[i].end(); iter++)
		{
			res[i] = res[i] + v[*iter];
		}
	}
	double sum = 0;
	for (int i = 0; i < N; i++)
        {
		sum = sum + res[i] * res[i];
	}
        sum = sqrt(sum);
	if (sum == 0)
        {
		sum = sum + 0.000000001;
	}
        for (int i = 0; i < N; i++)
        {
		res[i] = res[i] / sum;
        }
	return res;
}

int hits_5857::Iter(vector< vector<int> > L, vector< vector<int> > LT, vector<double> auth, vector<double> hub, double errorrate, int iterations, int N)
{
	vector<double> hub_t1(hub);
	vector<double> auth_t1(auth);
	int index = 0;
	if (N <= 10)
        {
		Result(index, auth, hub);
	}
        if (iterations > 0) 
        {
		for (index = 1; index <= iterations; index++)
		{
			auth = Iterone(LT, hub, N);
			hub = Iterone(L, auth, N);
			if (N <= 10)
				Result(index, auth, hub);
		}
		if (N > 10)
			Resfinal(index - 1, auth_t1, hub_t1);
	}
	else
	{
		while (true)
		{
			index++;
			auth_t1 = Iterone(LT, hub, N);
			hub_t1 = Iterone(L, auth_t1, N);
			if (N <= 10)
				Result(index, auth_t1, hub_t1);
			if (Err(hub, hub_t1, errorrate) && Err(auth, auth_t1, errorrate))
			{
				break;
			}
			auth = auth_t1;
			hub = hub_t1;
		}
		if (N > 10)
			Resfinal(index, auth_t1, hub_t1);
	}
	return 1;
}


int main(int argc, char* argv[])
{
	stringstream ss;
	int iterations, initval;
	
        ss << argv[1];
	ss >> iterations;
	ss.clear();
	
        ss << argv[2];
	ss >> initval;

        string filename = argv[3];

	hits_5857 H;

	vector<int> nums = H.Readfile(filename, " ");
	if (nums.empty())
	{
		return 0;
	}
	int N = nums[0];

	vector< vector<int> > L;
	vector< vector<int> > LT;
	vector<double> auth;
	vector<double> hub;
	double errorrate;

	int flag = H.IniVal(auth, hub, errorrate, iterations, initval, N);
	if (flag == 0)
	{
		return 0;
	}
	H.CreAdjList(L, LT, nums);
	H.Iter(L, LT, auth, hub, errorrate, iterations, N);

	return 1;
}
