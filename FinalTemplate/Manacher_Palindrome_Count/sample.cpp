#include<bits/stdc++.h>
using namespace std;

#include "template.h"

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s="aaaaa";
    manacher(s);

    for(int i=0;i<s.size();i++){
        cout<<d[i][0]<<" ";
    }

    cout<<"\n";

    for(int i=0;i<s.size();i++){
        cout<<d[i][1]<<" ";
    }

    cout<<"\n";
}