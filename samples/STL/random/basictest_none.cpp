// Not a problem submission
// Just for understanding

#include<bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().
                time_since_epoch().count());

// call rng() for random number

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cout<<rng()%10<<"\n";
    cout<<rng()%1000000<<"\n";
}