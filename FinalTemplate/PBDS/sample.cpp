#include<bits/stdc++.h>
using namespace std;

#include "template.h"

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ordered_set s;

    s.insert(1);
    s.insert(3);
    s.insert(4);

    cout<<*s.find_by_order(1)<<"\n";
    cout<<s.order_of_key(4)<<"\n";
}