#include <bits/stdc++.h>
#define MATH_INFINITY 2147483647
using namespace std;
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
    vector<int> f_ohs((1 << n), MATH_INFINITY);
    peaks[0] = 0;
    f_ohs[0] = 0;
    for (long long ss = 0; ss <= (1 << n) - 1; ss++)
    {
        if (peaks[ss] == MATH_INFINITY)
            continue;
        for (long long newel = 0; newel < n; newel++)
        {
            // now check can a topo sort be constructed??
            bool isValid = true;
            int inaccu=0;
            if (((1 << newel) & ss) == (1 << newel))
                continue;//if it is already present continue
            for (long long col = 0; col < newel; col++)
            {
                if (memory[col][newel] > 0)
                {   
                    if (((1 << col) & ss) == 0)
                    {
                        isValid = false;
                        break;
                    }
                    inaccu+=memory[col][newel];
                }
                if (isValid == false)
                    break;
            }   
            if (isValid == false)
            {
                continue;
            }
            else
            {
                //we place this element at the very end of out sequence, given that we had the f_ohs we can compute the minpeak for this new_set
                //let f_ohs_n be the new forward, and minis be the minpeak for this newset
                int f_ohs_n=f_ohs[ss];
                f_ohs_n-=inaccu;
                for (int j=newel+1; j<n; j++)
                {
                    f_ohs_n+=memory[newel][j];
                }
                f_ohs[((1 << newel) | ss)]=f_ohs_n;
                peaks[((1 << newel) | ss)]=min(max(f_ohs_n+inaccu+memory[newel][newel], peaks[ss]), peaks[((1 << newel) | ss)]);
            }
        }
    }
    cout<<peaks[(1 << n) - 1];
    return 0;
}