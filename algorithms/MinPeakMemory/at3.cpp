#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;

// begin###printers
//  int max(int a, int b)
//  {
//  	if (a>b) return a;
//  	else return b;
//  }

void show(vector<vector<int>> const m)
{
    for (auto it : m)
    {
        cout << endl;
        for (auto jt : it)
        {
            cout << jt << " ";
        }
    }
    cout << endl;
    cout << endl;
}

void seq(vector<int> sequence)
{
    for (auto it : sequence)
    {
        cout << it << " ";
    }
    cout << endl
         << endl;
}

void beg(unordered_set<int> sequence)
{
    cout << endl;
    for (auto it : sequence)
    {
        cout << it << " ";
    }
    cout << endl;
}
// end###printers

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

// given optimal solution of size n-1 find the optimal solution of size n
int nextopt(vector<vector<int>> &oldopts, const vector<vector<int>> &memory)
{
    vector<vector<int>> newopts;
    int global_min=MATH_INFINITY;
    vector<pair<vector<int>, vector<pair<int, vector<int>>>>> finale;
    for (auto opt : oldopts)
    {
        map<int, int> positions;
        int subglobal_min = MATH_INFINITY;
        vector<pair<int, vector<int>>> ball_posn = {pair(MATH_INFINITY, vector<int>{MATH_INFINITY})};
        for (int i = 0; i < opt.size(); i++)
            positions[opt[i]] = i;
        unordered_set<int> bag;
        for (int i=0; i<memory.size(); i++)  if (positions.count(i)==0) bag.insert(i);
        for (auto ball : bag)
        {
            int minposn = -1;
            bool fit = true;
            for (int i = 0; i < ball; i++)
            {
                if (memory[i][ball] > 0)
                {
                    if (positions.count(i) > 0)
                        minposn = max(positions[i], minposn);
                    else
                    {
                        fit = false;
                        break;
                    }
                }
            }
            if (fit == false)
                continue;
            // insert ball at all positions>=minposn and get the weight
            vector<int> bestposns = {minposn + 1};
            int bestvalue = MATH_INFINITY;
            for (int i = minposn + 1; i <= opt.size(); i++)
            {
                // calculate the sequence weight and store
                int value = evaluatesequence(opt, ball, i, memory);
                if (value < bestvalue)
                {
                    bestvalue = value;
                    bestposns.clear();
                    bestposns.push_back(i);
                }
                else if (value == bestvalue)
                {
                    bestposns.push_back(i);
                }
            }
            if (bestvalue < subglobal_min)
            {
                subglobal_min = bestvalue;
                ball_posn.clear();
                ball_posn.push_back(pair(ball, bestposns));
            } else if (bestvalue==subglobal_min)
            {
                ball_posn.push_back(pair(ball, bestposns));
            }
        }
        if (subglobal_min<global_min)
        {
            global_min=subglobal_min;
            finale.clear();
            finale.push_back(pair(opt, ball_posn));
        }
        else if (subglobal_min==global_min)
        {
            finale.push_back(pair(opt, ball_posn));
        }
    }

    //now we use the finale
    for (auto it: finale)
    {
        for (auto jt: it.second)
        {
            for (auto kt: jt.second)
            {
                vector<int> seq=it.first;
                seq.insert(seq.begin()+kt,jt.first);
                newopts.push_back(seq);
            }
        }
    }
    oldopts=newopts;
    return global_min;
}

int main()
{
    int a;
    int n;
    int alpha = MATH_INFINITY;
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
    show(memory);
    // start with min size
    vector<vector<int>> prevopts={{}};
    int iters=0;
    while (iters!=n)
    {
        alpha = nextopt(prevopts, memory);
        iters++;
    }
    cout << alpha << endl;
    return 0;
}