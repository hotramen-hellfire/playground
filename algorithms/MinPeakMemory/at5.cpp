#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;
// do not pass oldopt(sequence) by refrence!!

int evaluatesequence(vector<int> &sequence, vector<int> &oldK,int proc, int posn, const vector<vector<int>> &memory, vector<int> &newseq, vector<int> &newK)
{
    newseq=sequence;
    newK=oldK;
    newseq.insert(newseq.begin() + posn, proc);
    newK.insert(newK.begin()+posn, 0);
    int peak_memory = 0;
    //update the newK accordingly with the k values at for eachk
        //use the previously computed values to compute this, initially i have stored 0 at the posn as done above
        //try to keep the complexity to be o(n^2)
        //proc is the process number
        //posn is the position of the insertion
        //memory is the ususal matrix
    int backaccumulator=0;
    for (int i=0; i<posn; i++)
    {
        backaccumulator+=memory[newseq[i]][proc];
        newK[i]+=backaccumulator;
    }
    int forwardaccumulator=0;
    for (int i=newseq.size()-1; i>posn; i--)
    {
        forwardaccumulator+=memory[proc][newseq[i]];
        newK[i]+=forwardaccumulator;
    }
    // we will do the nsq version update for now
    int ovhd=0;
    for (int p = 0; p < posn; ++p)
    {
        for (int q = posn + 1; q < newseq.size(); ++q)
        {
            ovhd += memory[newseq[p]][newseq[q]];
        }
    }
    newK[posn]=ovhd+forwardaccumulator+backaccumulator+memory[proc][proc];
    for (int i=0; i<newseq.size(); i++) peak_memory=max(peak_memory, newK[i]);
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

class Ks{
    int back=0;
    int forwd=0;
    int ovhd=0;
    int curr=0;
};

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
    vector<vector<pair<vector<int>, vector<int>>>> sequence((1 << n), vector<pair<vector<int>, vector<int>>>{pair(vector<int>{}, vector<int>{})});
    peaks[0] = 0;
    for (long long ss = 0; ss <= (1 << n) - 1; ss++)
    {
        if (peaks[ss] == MATH_INFINITY)
            continue;
        map<int, int> positions;
        for (auto seqss : sequence[ss])
        {
            for (int i = 0; i < seqss.first.size(); i++)
                positions[seqss.first[i]] = i;
            for (int newel = maxh(ss, n); newel < n; newel++) /// scope
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
                        minposn = max(minposn, positions[col]);
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
                    vector<int> prevseq = seqss.first;
                    vector<int> prevks = seqss.second;
                    vector<pair<vector<int>, vector<int>>> newseqsK;
                    vector<int> newsequence;
                    vector<int> newK;
                    int bestvalue = peaks[((1 << newel) | ss)];
                    for (int idx=prevseq.size(); idx>minposn; idx--)
                    {
                        int value = evaluatesequence(prevseq, prevks, newel, idx, memory, newsequence, newK);
                        if (value<bestvalue)
                        {
                            bestvalue=value;
                            newseqsK.clear();
                            newseqsK.push_back(pair(newsequence, newK));
                        }
                        // else if (value==bestvalue)
                        // {
                        //     newseqsK.push_back(pair(newsequence, newK));
                        // }
                    }
                    if (bestvalue< peaks[((1 << newel) | ss)])
                    {
                        peaks[((1 << newel) | ss)] = bestvalue;
                        sequence[((1 << newel) | ss)].clear();
                        for (auto newseqK:newseqsK)
                        {
                            sequence[((1 << newel) | ss)].push_back(newseqK);
                        }
                    }
                    // else if (bestvalue==peaks[((1 << newel) | ss)])
                    // {
                    //     for (auto newseqK:newseqsK)
                    //     {
                    //         sequence[((1 << newel) | ss)].push_back(newseqK);
                    //     }
                    // }
                }
            }
        }
    }
    cout << peaks[(1 << n) - 1];
    return 0;
}