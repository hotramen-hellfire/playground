#include <bits/stdc++.h>
#include "bst.h"
using namespace std;
int main()
{
    BinarySearchTree lines;
    lines.insert(43, 0);
    lines.insert(33, 1);
    cout<<"before deleting"<<endl;
    lines.printBST(lines.root);
    lines.erase(43, 0); 
    cout<<"after deleting"<<endl;
    lines.printBST(lines.root);
    return 0;
}