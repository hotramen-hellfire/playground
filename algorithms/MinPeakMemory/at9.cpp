#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;
int main()
{
    int a;
    size_t n;
    cin >> n;
    vector<int> memory;
    for (int i = 0; i < n * n; i++)
    {
        cin >> a;
        memory.push_back(a);
    }
    vector<int> peaks((1 << n), MATH_INFINITY);//this will contain the peaks of the subsets at any instance, prefix will have this subset
    vector<int> f_ohs((1 << n), MATH_INFINITY);//this will contain the forwarded memory from this subset to the undecided suffix
    vector<int> outs(n, -1);//this is to avoid calculation of outwards from a single element again and again, this will store the sum going out of any memory  element
    peaks[0] = 0;
    f_ohs[0] = 0;
    for (long long ss = 0; ss <= (1 << n) - 1; ss++)
    {
        //this has the propert similar to dijkstra's if you are processing a subset you have explored all possible paths to come to this subset,
        //here we update the peaks for any sets which can be formed using this set(ss) as a suffix and a new element at the end($) of this subset.
        if (peaks[ss] == MATH_INFINITY)
            continue;
        //find a newelement that can be used for the $ of this suffix
        for (long long newel = 0; newel < n; newel++)
        {
            //check if a valid topo sort can be constructed??
            bool topo = true;
            int inwd_n = 0;//this is the inward for this element, wil be used later on
            if (((1 << newel) & ss) == (1 << newel))
                continue; // if it is already present continue
            for (long long col = 0; col < newel; col++)
            {
                if (memory[n * col + newel] > 0)
                {
                    if (((1 << col) & ss) == 0)
                    {
                        topo = false;
                        break;
                    }
                    inwd_n += memory[n * col + newel];
                }
                if (topo == false)
                    break;
            }
            if (topo == false)
            {
                continue;//better luck next time
            }
            else
            {
                // we place this element at the very end of out sequence, given that we had the f_ohs we can compute the minpeak for this new_set
                // let frwd_n be the new forward
                int frwd_n = f_ohs[ss];
                frwd_n -= inwd_n;
                if (outs[newel] < 0)
                {
                    int acc = 0;
                    for (int j = newel + 1; j < n; j++)
                    {
                        acc += memory[n * newel + j];
                    }
                    outs[newel] = acc;
                }
                frwd_n+=outs[newel];
                f_ohs[((1 << newel) | ss)] = frwd_n;
                peaks[((1 << newel) | ss)] = min(max(frwd_n + inwd_n + memory[n * newel + newel], peaks[ss]), peaks[((1 << newel) | ss)]);
                //(frwd_n + inwd_n + memory[n * newel + newel]) is the peak for $
                //peaks[ss] is the maxpeak in the prefix
            }
        }
    }
    cout << peaks[(1 << n) - 1];
    return 0;
}