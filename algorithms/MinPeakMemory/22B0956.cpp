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
    vector<int> pks((1 << n), MATH_INFINITY);//this will contain the peaks of the subsets at any instance, prefix will have this subset
    vector<int> frwd((1 << n), MATH_INFINITY);//this will contain the forwarded memory from this subset to the undecided suffix
    vector<int> outs(n, -1);//this is to avoid calculation of outwards from a single element again and again, this will store the sum going out of any memory  element
    pks[0] = 0;
    frwd[0] = 0;
    for (long long thsset = 0; thsset <= (1 << n) - 1; thsset++)
    {
        //this has the propert similar to dijkstra's if you are processing a subset you have explored all possible paths to come to this subset,
        //here we update the peaks for any sets which can be formed using this set(thsset) as a suffix and a new element at the end($) of this subset.
        if (pks[thsset] == MATH_INFINITY)
            continue;
        //find a newelement that can be used for the $ of this suffix
        for (long long nwelmnt = 0; nwelmnt < n; nwelmnt++)
        {
            //check if a valid toposort sort can be constructed??
            bool istpsortbl = true;
            int inwd_n = 0;//this is the inward for this element, wil be used later on
            if (((1 << nwelmnt) & thsset) == (1 << nwelmnt))
                continue; // if it is already present continue
            for (long long clmn = 0; clmn < nwelmnt; clmn++)
            {
                if (memory[n * clmn + nwelmnt] > 0)
                {
                    if (((1 << clmn) & thsset) == 0)
                    {
                        istpsortbl = false;
                        break;
                    }
                    inwd_n += memory[n * clmn + nwelmnt];
                }
                if (istpsortbl == false)
                    break;
            }
            if (istpsortbl == false)
            {
                continue;//better luck next time
            }
            else
            {
                // we place this element at the very end of out sequence, given that we had the frwd we can compute the minpeak for this new_set
                // let frwd_n be the new forward
                int frwd_n = frwd[thsset];
                frwd_n -= inwd_n;
                if (outs[nwelmnt] < 0)
                {
                    int acc = 0;
                    for (int j = nwelmnt + 1; j < n; j++)
                    {
                        acc += memory[n * nwelmnt + j];
                    }
                    outs[nwelmnt] = acc;
                }
                frwd_n+=outs[nwelmnt];
                frwd[((1 << nwelmnt) | thsset)] = frwd_n;
                pks[((1 << nwelmnt) | thsset)] = min(max(frwd_n + inwd_n + memory[n * nwelmnt + nwelmnt], pks[thsset]), pks[((1 << nwelmnt) | thsset)]);
                //(frwd_n + inwd_n + memory[n * nwelmnt + nwelmnt]) is the peak for $
                //pks[thsset] is the maxpeak in the prefix
            }
        }
    }
    cout << pks[(1 << n) - 1]<<endl;
    return 0;
}