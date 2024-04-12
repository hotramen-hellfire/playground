
#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;

class Ks {
public:
    int back = 0;
    int forwd = 0;
    int ovhd = 0;
    int curr = 0;

    Ks(int a, int b, int c, int d) : back(a), forwd(b), ovhd(c), curr(d) {}

    Ks& operator=(const Ks &obj) {
        if (this != &obj) { // Check for self-assignment
            back = obj.back;
            forwd = obj.forwd;
            ovhd = obj.ovhd;
            curr = obj.curr;
        }
        return *this;
    }
};

// do not pass oldopt(sequence) by refrence!!
int evaluatesequence(vector<int> &sequence, vector<Ks> &oldK,int proc, int posn, const vector<vector<int>> &memory, vector<int> &newseq, vector<Ks> &newK)
{
    //this is O(n)
    newseq=sequence;
    newK=oldK;
    newseq.insert(newseq.begin() + posn, proc);
    newK.insert(newK.begin()+posn, Ks(0,0,0,0));
    newK[posn].curr=memory[proc][proc];
    int peak_memory = 0;
    int ovhd=0;
    int t2=0;
    int oh1=0;
    int t3=0;
    int oh2=0;
    if (posn!=0) {t2=newK[posn-1].forwd; oh1=newK[posn-1].ovhd;};
    if (posn!=newseq.size()-1) {t3=newK[posn+1].forwd; oh2=newK[posn+1].ovhd;};
    ovhd=(oh1+oh2+t2+t3)/2;
    int backaccumulator=0;
    int forwardaccumulator=0;
    for (int i=0; i<posn; i++)
    {
        newK[i].forwd+=memory[newseq[i]][proc];
        newK[i].ovhd+=backaccumulator;
        backaccumulator+=memory[newseq[i]][proc];
    }
    for (int i=newseq.size()-1; i>posn; i--)
    {
        newK[i].back+=memory[proc][newseq[i]];
        newK[i].ovhd+=forwardaccumulator;
        forwardaccumulator+=memory[proc][newseq[i]];
    }
    newK[posn].forwd=forwardaccumulator;
    newK[posn].back=backaccumulator;
    newK[posn].ovhd=ovhd;
    for (int i=0; i<newseq.size(); i++) peak_memory=max(peak_memory, newK[i].forwd+newK[i].ovhd+newK[i].back+newK[i].curr);
    return peak_memory;
}

int maxh(long long num, int n)
{
    int ret=0;
    for (int i=n-1; i>=0; i--)
    {
        if ((((1 << i) & n) == (1 << i)) && ret!=0) {ret=i;};
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
    vector<vector<pair<vector<int>, vector<Ks>>>> sequence((1 << n), vector<pair<vector<int>, vector<Ks>>>{pair(vector<int>{}, vector<Ks>{})});
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
                    vector<Ks> prevks = seqss.second;
                    vector<pair<vector<int>, vector<Ks>>> newseqsK;
                    vector<int> newsequence;
                    vector<Ks> newK;
                    int bestvalue = peaks[((1 << newel) | ss)];
                    for (int idx=minposn+1; idx<=prevseq.size(); idx++)
                    {
                        int value = evaluatesequence(prevseq, prevks, newel, idx, memory, newsequence, newK);
                        if (value<bestvalue)
                        {
                            bestvalue=value;
                            newseqsK.clear();
                            newseqsK.push_back(pair(newsequence, newK));
                        }
                        else if (value==bestvalue)
                        {
                            newseqsK.push_back(pair(newsequence, newK));
                        }
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
                    else if (bestvalue==peaks[((1 << newel) | ss)])
                    {
                        for (auto newseqK:newseqsK)
                        {
                            sequence[((1 << newel) | ss)].push_back(newseqK);
                        }
                    }
                }
            }
        }
    }
    cout << peaks[(1 << n) - 1];
    return 0;
}