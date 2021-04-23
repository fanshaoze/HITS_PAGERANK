#pragma once
#ifndef pgrk_h
#define pgrk_h

using namespace std;

class pgrk_5857 {
private:
	void Result(int index, vector<double> pr);
	void Resfinal(int index, vector<double> pr);
	bool Err(vector<double> vec, vector<double> vec_t1, double errorate);
	vector<double> Iterone(vector<int> Lnum, vector< vector<int> > m, vector<double> v, int N, double d);

public:
	vector<int> Readfile(string filename, string pattern);
	void CreAdjList(vector<int>& Lnum, vector< vector<int> >& LT, vector<int> nums);
	int IniPara(vector<double>& pr, double& errorate, int& iterations, int& initval, int N);
	int Iter(vector<int> Lnum, vector< vector<int> > LT, vector<double> pr, double errorate, int iterations, int N, double d);
};

#endif