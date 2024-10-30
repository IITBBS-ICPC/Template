#include<bits/stdc++.h>
using namespace std;

#include "template.h"

// Based on Yosupo submission, link in README

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin>>n;

    for(int i=0;i<n;i++)
    {
        long long x;
        cin>>x;

        auto factors = factorizer::Factorize((i64)x);

        vector<long long> divisors;

        for(auto [p, e] : factors)
        {
            for(int i=0;i<e;i++)
            {
                divisors.push_back(p);
            }
        }

        cout<<divisors.size()<<" ";

        for(auto d : divisors)
        {
            cout<<d<<" ";
        }

        cout<<"\n";
    }
}