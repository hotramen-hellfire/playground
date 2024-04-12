#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;
// do not pass oldopt(sequence) by refrence!!
int evaluatesequence(vector<int> sequence, int ball, int posn, const vector<vector<int>> &memory)
{
    sequence.insert(sequence.begin() + posn, ball);
    int peak_memory = 0;
    int n = sequence.size();
    for (int k = 0; k < n; ++k)
    {
        int mem_k = memory[sequence[k]][sequence[k]]; // m(ik)
        int sum_p = 0;
        int sum_q = 0;
        int sum_pq = 0;
        for (int p = 0; p < k; ++p)
        {
            sum_p += memory[sequence[p]][sequence[k]]; // ∑(1≤p<k) m(ip, ik)
        }
        for (int q = k + 1; q < n; ++q)
        {
            sum_q += memory[sequence[k]][sequence[q]]; // ∑(k<q≤n) m(ik, iq)
        }
        for (int p = 0; p < k; ++p)
        {
            for (int q = k + 1; q < n; ++q)
            {
                sum_pq += memory[sequence[p]][sequence[q]]; // ∑(1≤p<k)∑(k<q≤n) m(ip, iq)
            }
        }
        int current_peak_memory = mem_k + sum_p + sum_q + sum_pq;
        peak_memory = max(peak_memory, current_peak_memory);
    }
    return peak_memory;
}

int maxh(long long num, int n)
{
    int ret=0;
    for (int i=n-1; i>=0; i--)
    {
        if ((((1 << i) & num) == (1 << i)) && ret!=0) {ret=i;};
    }
    return ret;
}
int main()
{
    int a;
    int alpha = MATH_INFINITY;
    size_t n;
    cin >> n;
    vector<vector<int>> memory;
    for (int i = 0; i < n; i++)
    {
        vector<int> t1;
        for (int j = 0; j < n; j++)
        {
            cin >> a;
            t1.push_back(a);
        }
        memory.push_back(t1);
    }
    vector<int> peaks((1 << n), MATH_INFINITY);
    vector<vector<int>> sequence((1 << n), vector<int>{});
    peaks[0] = 0;
    for (long long ss = 0; ss <= (1 << n) - 1; ss++)
    {
        map<int, int> positions;
        if (peaks[ss] == MATH_INFINITY)
            continue;
        for (int i = 0; i < sequence[ss].size(); i++)
            positions[sequence[ss][i]] = i;
        for (int newel = maxh(ss, n); newel < n; newel++)///scope
        {
            // now check can a topo sort be constructed??
            bool isValid = true;
            int minposn = -1;
            if (((1 << newel) & ss) == (1 << newel))
                continue;
            for (int col = 0; col < newel; col++)
            {
                if (memory[col][newel] > 0)
                {
                    if (((1 << col) & ss) == 0)
                    {
                        isValid = false;
                        break;
                    }
                    minposn=max(minposn, positions[col]);
                }
                if (isValid == false)
                    break;
            }
            if (isValid == false)
            {
                assert(peaks[((1 << newel) | ss)] == MATH_INFINITY);
                continue;
            }
            else
            {
                vector<int> prevseq = sequence[ss];
                int bestposn=minposn+1;
                int bestvalue=peaks[((1 << newel) | ss)];
                for (int idx=minposn+1; idx<=prevseq.size(); idx++)
                {
                    int value = evaluatesequence(prevseq, newel, idx, memory);
                    if (value<bestvalue) 
                    {
                        bestvalue=value;
                        bestposn=idx;
                    }
                }
                if (bestvalue<peaks[((1 << newel) | ss)])
                {
                    peaks[((1 << newel) | ss)]=bestvalue;
                    prevseq.insert(prevseq.begin()+bestposn, newel);
                    sequence[((1 << newel) | ss)]=prevseq;
                }
            }
        }
    }
    cout<<peaks[(1 << n) - 1];
    return 0;
}