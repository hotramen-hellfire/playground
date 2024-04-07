#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

 
int main(){
	   /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
	int n; // number of processes
	
 
	cin >> n;
	
	vector< int > memory;  
	
	int a;
	for(int i=0; i<n; i++){
		for(int j=0; j<i; j++)
			cin >> a;
		for(int j=i; j<n; j++){
			cin >> a;
			memory.push_back(a);
		}
	}
	
	
	
  	cout << 0 << endl;
	
	

	return 0;
}
