// Not a problem submission
// Just a basic test example

#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> d;  
// d[0] contains half the length of max palindrome of odd length with centre at i 
// d[1] contains half the length of max palindrome of even length with right centre at i

void manacher(string s){
    int n = s.size();
    d.resize(n,vector<int>(2));
    for (int t = 0; t < 2; t++) {
        int l = 0, r = -1, j;
        for (int i = 0; i < n; i++) {
            j = (i > r ) ? 1 : min(d[l+r-i+t][t],r-i+t) + 1;
        	while (i + j - t < n && i - j >= 0 && s[i + j - t] == s[i - j]) j++;
        	j--;
        	d[i][t] = j;
        	if (i + j + t > r) { l = i - j; r = i + j - t; }
        }
    }
}

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