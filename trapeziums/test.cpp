#include <bits/stdc++.h>
using namespace std;
int main()
{
	set<int> S;
	S.insert(10);
	S.insert(43);
	S.insert(7);
	S.insert(11);
	auto it=S.find(7);
	cout<<(it==S.begin());
}
