#include <bits/stdc++.h>
using namespace std;

#define ll long long

void solve()
{
    bitset<5> b1;
    cout << b1 << "\n";

    bitset<5> b2("1100");

    cout << b2 << "\n";

    b2.set(1);
    cout << b2 << "\n";

    //

    bitset<5> b3("01100");

    cout << b3.to_string() << "\n";

    cout << b3.any() << " " << b3.none() << " " << b3.all() << "\n";

    cout << b3.count() << "\n";

    cout << b3.test(1) << "\n";

    cout << b3._Find_first() << "\n";

    cout << b3._Find_next(1) << "\n";

    cout << b3._Find_next(2) << "\n";

    cout << "\n";
}

int32_t main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    ll t = 1;
    // cin>>t;

    while (t--)
    {
        solve();
    }

    return 0;
}