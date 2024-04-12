#include <cmath>
#include <cstdio>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#define MATH_INFINITY 2147483647
using namespace std;

int max(int a, int b)
{
	if (a>b) return a;
	else return b;
}

int findnext(vector<vector<int>> &memoryCurr, int sf)
{
	int j=sf;
	bool found=0;
	for (j=sf; j<memoryCurr[0].size(); j++)
	{
		bool isminimal=1;
		for (int i=0; i<j; i++)
		{
			if (memoryCurr[i][j]!=0) 
			{	
				isminimal=0;
				break;
			}
		}
		if (isminimal==1 && memoryCurr[j][j]!=-1) {found=1; break;}
	}
	if (found==0) return -1;//no minimal elemnt>=sf
	else return j;
}

void show(vector<vector<int>> const m)
{
	for (auto it: m){
		cout<<endl;
		for (auto jt: it)
		{
			cout<<jt<<" ";
		}
	}
	cout<<endl;
	cout<<endl;
}

int getpeak(vector<vector<int>> const &memoryCurr, vector<vector<int>> const &residual, int const sf, int const curas,  vector<int> const &sequenceCurr)
{
	// show(memoryCurr);
	int prevhead=0;//second term in function
	int nexhead=0;//third term in function
	int backhead=0;//fourth term in the function
	for (int i=0; i<sf; i++)
	{
		prevhead+=residual[i][sf];//must have been updated to residual
	}
	for (int j=sf+1; j<memoryCurr.size(); j++)
	{
		nexhead+=memoryCurr[sf][j];//yet to be updated to residual
	}
	for (int i=0; i<curas; i++)
	{
		int sf0=sequenceCurr[i];
		for (int j=sf0+1; j<memoryCurr.size(); j++)
		{
			if (j==sf) continue;
			if (memoryCurr[j][j]!=-1) backhead+=residual[sf0][j];
			else break;
		}
	}
	return memoryCurr[sf][sf]+prevhead+nexhead+backhead;
}

void seq(vector<int> sequence)
{
	cout<<endl;
	for (auto it: sequence)
	{
		cout<<it<<" "; 
	}
	cout<<endl;
}

void overheads(vector<vector<int>> &memoryCurr, vector<vector<int>> &residual, vector<int> &sequenceCurr, int curas, int &alpha, int maxpeakt_n)
{
	//find the next element to be tested
	//calculate the peak for this selection
	//if peak>alpha backtrack
	int sf=0;
	int currpeak=MATH_INFINITY;
	for (sf=findnext(memoryCurr, 0); sf>=0 ;sf=findnext(memoryCurr, sf+1))
	{
		//if curas>=sequenceCurr.size() this doesnt enter
		//sf is the current selection lets first calculate the new overhead and check feasability without updating the data
		currpeak=getpeak(memoryCurr, residual, sf, curas, sequenceCurr);
		if (currpeak>alpha) continue;// atleast one dfs would have been completed if this happens
		if (curas==memoryCurr.size()-1) {alpha=max(currpeak, maxpeakt_n);}//this is the last assignment
		//make the updates and feed forward
		//push the row values to the residual curves and mark mem[sf][sf] to be -1
		for (int j=sf; j<memoryCurr.size(); j++)
		{
			residual[sf][j]=memoryCurr[sf][j];
			memoryCurr[sf][j]=0;
		}
		memoryCurr[sf][sf]=-1;
		//update the sequence curr and currpeaks
		sequenceCurr[curas]=sf;
		//make the function call 
		overheads(memoryCurr, residual, sequenceCurr, curas+1, alpha, max(maxpeakt_n, currpeak));
		sequenceCurr[curas]=-1;
		for (int j=sf; j<memoryCurr.size(); j++)
		{
			memoryCurr[sf][j]=residual[sf][j];
			residual[sf][j]=0;
		}
	}
	return;
}

int main()
{
	int a;
	int n;
	int alpha=MATH_INFINITY;

	cin >> n;
	vector<vector<int>> memoryOrig;
	vector<vector<int>> memoryCurr;
	vector<vector<int>> residual;
	vector<int> sequenceCurr(n,-1);
	vector<int> curpeaks(n,MATH_INFINITY);
	for (int i = 0; i < n; i++)
	{
		vector<int> t1;
		vector<int> t2;
		vector<int> t3;
		for (int j = 0; j < n; j++)
		{
			cin >> a;
			t1.push_back(a);
			t2.push_back(a);
			t3.push_back(0);
		}
		memoryCurr.push_back(t1);
		memoryOrig.push_back(t2);
		residual.push_back(t3);
	}
	overheads(memoryCurr, residual, sequenceCurr, 0, alpha, -MATH_INFINITY);
	cout << alpha << endl;
	return 0;
}
