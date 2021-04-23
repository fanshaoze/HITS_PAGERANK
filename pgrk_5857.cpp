#include<iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include<string>

#include <cmath>
#include <math.h>

#include <iomanip>


#include "pgrk_5857.h"

using namespace std;

vector<int> pgrk_5857::Readfile(string filename, string pattern)
{
	string::size_type pos;
	string str;
	vector<int> result;
	ifstream input(filename.c_str(), ios_base::binary);

	if (input)
	{
		while (getline(input, str)) //read from input file
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
	else // file not exist
	{
		cout << "NO SUCH FILE" << endl;
		result.clear();
	}

	return result;
}

void pgrk_5857::CreAdjList(vector<int>& Lnum, vector< vector<int> >& LT, vector<int> nums)
{
	int N = nums[0];
	int i;
	for (i = 0; i < N; i++)
	{
		vector<int> vec;
		LT.push_back(vec);
	}
	vector<int>::iterator iter = nums.begin();
	iter = nums.erase(iter);
	iter = nums.erase(iter);
	for (iter = nums.begin(); iter != nums.end();)
	{
		int n0, n1;
		n0 = *iter;
		iter = iter + 1;
		n1 = *iter;
		iter = iter + 1;
		Lnum[n0] = Lnum[n0] + 1;
		LT[n1].push_back(n0);
	}

}

int pgrk_5857::IniPara(vector<double>& pr, double& errorrate, int& iterations, int& initval, int N)
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
	pr.insert(pr.begin(), N, temp);
	return 1;
}

void pgrk_5857::Result(int index, vector<double> pr)
{
	cout << fixed << setprecision(7);
	if (index == 0)
	{
		cout << "Base    :";
	}
	else {
		cout << "Iter    :";
	}
	cout << setw(3) << setfill(' ') << right << index;
	cout << " :";
	for (int i = 0; i < pr.size(); i++)
	{
		cout << " P[";
		cout << setw(2) << setfill(' ') << right << i;
		cout << "]=";
		cout << pr[i];
	}
	cout << endl;

}

void pgrk_5857::Resfinal(int index, vector<double> pr)
{
	cout << fixed << setprecision(7);
	cout << "Iter    :";
	cout << setw(3) << setfill(' ') << right << index << endl;
	for (int i = 0; i < pr.size(); i++)
	{
		cout << " P[";
		cout << setw(2) << setfill(' ') << right << i;
		cout << "]=";
		cout << pr[i];
		cout << endl;
	}

}

bool pgrk_5857::Err(vector<double> vec, vector<double> vec_t1, double errorrate)
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

vector<double> pgrk_5857::Iterone(vector<int> Lnum, vector< vector<int> > m, vector<double> v, int N, double d)
{
	vector<double> res(N, 0.);
	vector<int>::iterator iter;
	for (int i = 0; i < N; i++)
	{
		for (iter = m[i].begin(); iter != m[i].end(); iter++)
		{
			res[i] += v[*iter] / Lnum[*iter];
		}
		res[i] = res[i] * d + (1 - d) / N;
	}

	return res;
}

int pgrk_5857::Iter(vector<int> Lnum, vector< vector<int> > LT, vector<double> pr, double errorrate, int iterations, int N, double d)
{
	vector<double> pr_pre(pr);
	int index = 0;
	if (N <= 10)
		Result(index, pr);
	if (iterations > 0) {
		for (index = 1; index <= iterations; index++)
		{
			pr_pre = Iterone(Lnum, LT, pr, N, d);
			pr = pr_pre;
			if (N <= 10)
				Result(index, pr);
		}
		if (N > 10)
			Resfinal(index - 1, pr);
	}
	else
	{
		while (true)
		{
			index++;
			pr_pre = Iterone(Lnum, LT, pr, N, d);
			if (N <= 10)
				Result(index, pr_pre);
			if (Err(pr, pr_pre, errorrate))
			{
				break;
			}
			pr = pr_pre;
		}
		if (N > 10)
			Resfinal(index, pr_pre);
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

	pgrk_5857 P;

	vector<int> nums = P.Readfile(filename, " ");
	if (nums.empty())
	{
		return 0;
	}
	int N = nums[0];
	vector<int> Lnum(N, 0);
	vector< vector<int> > LT;
	vector<double> pr;
	double errorrate;

	int flag = P.IniPara(pr, errorrate, iterations, initval, N);
	if (flag == 0)
	{
		return 0;
	}
	P.CreAdjList(Lnum, LT, nums);
	double d = 0.85;
	P.Iter(Lnum, LT, pr, errorrate, iterations, N, d);

	return 1;
}