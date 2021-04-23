#pragma once
#ifndef hits_h
#define hits_h

using namespace std;

class hits_5857 {
private:
	void Result(int index, vector<double> auth, vector<double> hub);
	void Resfinal(int index, vector<double> auth, vector<double> hub);
	bool Err(vector<double> vec, vector<double> vec_t1, double errorate);
	vector<double> Iterone(vector< vector<int> > m, vector<double> v, int N);

public:
	vector<int> Readfile(string filename, string pattern);
	void CreAdjList(vector< vector<int> >& L, vector< vector<int> >& LT, vector<int> nums);
	int IniVal(vector<double>& auth, vector<double>& hub, double& errorate, int& iterations, int& initval, int N);
	int Iter(vector< vector<int> > L, vector< vector<int> > LT, vector<double> auth, vector<double> hub, double errorate, int iterations, int N);
};

#endif 
