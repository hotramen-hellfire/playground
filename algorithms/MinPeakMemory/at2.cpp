#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;

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

void seq(vector<int> sequence)
{
	cout<<endl;
	for (auto it: sequence)
	{
		cout<<it<<" "; 
	}
	cout<<endl;
}

void beg(unordered_set<int> sequence)
{
	cout<<endl;
	for (auto it: sequence)
	{
		cout<<it<<" "; 
	}
	cout<<endl;
}
//end###printers

//do not pass oldopt(sequence) by refrence!!
int evaluatesequence(vector<int> sequence, const vector<vector<int>> &memory)
{
    int peak_memory = 0;
    int n = sequence.size();

    for (int k = 0; k < n; ++k) {
        int mem_k = memory[sequence[k]][sequence[k]]; // m(ik)
        int sum_p = 0;
        int sum_q = 0;
        int sum_pq = 0;

        for (int p = 0; p < k; ++p) {
            sum_p += memory[sequence[p]][sequence[k]]; // ∑(1≤p<k) m(ip, ik)
        }

        for (int q = k + 1; q < n; ++q) {
            sum_q += memory[sequence[k]][sequence[q]]; // ∑(k<q≤n) m(ik, iq)
        }

        for (int p = 0; p < k; ++p) {
            for (int q = k + 1; q < n; ++q) {
                sum_pq += memory[sequence[p]][sequence[q]]; // ∑(1≤p<k)∑(k<q≤n) m(ip, iq)
            }
        }

        int current_peak_memory = mem_k + sum_p + sum_q + sum_pq;
        peak_memory = max(peak_memory, current_peak_memory);
    }
    return peak_memory;	
}

int main()
{
	int a;
	int n;
	int alpha=MATH_INFINITY;
	cin >> n;
	vector<vector<int>> memory;
	unordered_set<int> bag;
	for (int i = 0; i < n; i++)
	{
		bag.insert(i);
		vector<int> t1;
		for (int j = 0; j < n; j++)
		{
			cin >> a;
			t1.push_back(a);
		}
		memory.push_back(t1);
	}
	show(memory);
	//start with min size
    vector<int> test={1,2,1,3};
	cout << evaluatesequence(test, memory) << endl;
	return 0;
}
