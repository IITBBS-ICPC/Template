#include<bits/stdc++.h>
using namespace std;

#include "template.h"

const static int K = 26;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    t.emplace_back();
    add_s("ab");
    add_s("bc");
    add_s("abc");
    add_s("bca");
    add_s("cab");
    add_s("ac");
    add_s("a");
    add_s("b");

    bfs();

    for(int i=0;i<t.size();++i){
        cout << "Vertex " << i << ":\n";
        cout << "Leaf: " << t[i].leaf << "\n";
        cout << "Link: " << t[i].link << "\n";
        for(int j=0;j<K;++j){
            if(t[i].next[j] != -1){
                cout << "Next[" << char(j+'a') << "]: " << t[i].next[j] << "\n";
            }
        }
        for(int j=0;j<K;++j){
            if(t[i].go[j] != -1){
                cout << "Go[" << char(j+'a') << "]: " << t[i].go[j] << "\n";
            }
        }
        cout << "\n";
    }
}