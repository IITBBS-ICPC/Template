#include<bits/stdc++.h>
using namespace std;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

#define ordered_set tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>
// strict less than is recommonded to avoid problems
// with equality

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