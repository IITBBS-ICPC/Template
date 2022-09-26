#ifndef MYCODER_BASE_HPP_
#define MYCODER_BASE_HPP_

#include <bits/stdc++.h>
using namespace std;

#define FAST_IO       \
    cin.tie(nullptr); \
    ios_base::sync_with_stdio(false);
#define F first
#define S second
#define pb push_back
#define mk make_pair
#define SZ(c) ((int)((c).size()))
#define all(c) (c).begin(), (c).end()

using ll = long long;
using vi = vector<int>;
using pii = pair<int, int>;

template <typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for (auto &x : a) in >> x;
    return in;
}
template <typename T, typename S>
istream &operator>>(istream &in, pair<T, S> &x) {
    in >> x.F >> x.S;
    return in;
}

template <typename T>
ostream &operator<<(ostream &out, vector<T> &a) {
    bool first = true;
    for (const auto &x : a) {
        if (!first) out << ' ';
        first = false;
        out << x;
    }
    return out;
}
template <typename T, typename S>
ostream &operator<<(ostream &out, pair<T, S> &x) {
    out << x.F << ' ' << x.S;
    return out;
}

#endif  // MYCODER_BASE_HPP_
