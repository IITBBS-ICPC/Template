#include <bits/stdc++.h>
using namespace std;
using ll = long long int;
#define pb push_back
#define F first
#define S second
#define mk make_pair
using i32 = int32_t;
using i64 = int64_t;

namespace factorizer {

template <typename T, typename S> constexpr T power(T base, S exp, T mod) {
    T ans = static_cast<T>(1);
    while (exp) {
        if (exp & static_cast<S>(1)) {
            ans = (ans * ans) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return ans;
}

template <typename T> bool IsPrime(T n, const vector<T> &bases) {
    if (n < 2) {
        return false;
    }
    vector<T> small_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    for (const T &x : small_primes) {
        if (n % x == 0) {
            return n == x;
        }
    }
    if (n < 31 * 31) {
        return true;
    }
    int s = 0;
    T d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    for (const T &a : bases) {
        if (a % n == 0) {
            continue;
        }
        T cur = a % n;
        cur = power(cur, d, n);
        if (cur == static_cast<T>(1)) {
            continue;
        }
        bool witness = true;
        for (int r = 0; r < s; r++) {
            if (cur == n - 1) {
                witness = false;
                break;
            }
            cur *= cur;
        }
        if (witness) {
            return false;
        }
    }
    return true;
}

bool IsPrime(i64 n) {
    return IsPrime(n, {2, 325, 9375, 28178, 450775, 9780504, 1795265022});
}

bool IsPrime(i32 n) { return IsPrime(n, {2, 7, 61}); }

template <typename T>
vector<pair<T, int>> MergeFactors(const vector<pair<T, int>> &a,
                                  const vector<pair<T, int>> &b) {
    vector<pair<T, int>> c;
    int i = 0;
    int j = 0;
    while (i < (int)a.size() || j < (int)b.size()) {
        if (i < (int)a.size() && j < (int)b.size() &&
            a[i].first == b[j].first) {
            c.emplace_back(a[i].first, a[i].second + b[j].second);
            ++i;
            ++j;
            continue;
        }
        if (j == (int)b.size() ||
            (i < (int)a.size() && a[i].first < b[j].first)) {
            c.push_back(a[i++]);
        } else {
            c.push_back(b[j++]);
        }
    }
    return c;
}

template <typename T> vector<pair<T, int>> RhoC(const T &n, const T &c) {
    if (n <= 1) {
        return {};
    }
    if ((n & 1) == 0) {
        return MergeFactors({{2, 1}}, RhoC(n / 2, c));
    }
    if (IsPrime(n)) {
        return {{n, 1}};
    }
    T x = 2;
    T saved = 2;
    T power = 1;
    T lam = 1;
    while (true) {
        x = x * x + c;
        if (x >= n)
            x %= n;
        T g = __gcd((x >= saved ? x - saved : x + n - saved)(), n);
        if (g != 1) {
            return MergeFactors(RhoC(g, c + 1), RhoC(n / g, c + 1));
        }
        if (power == lam) {
            saved = x;
            power <<= 1;
            lam = 0;
        }
        lam++;
    }
    return {};
}

template <typename T> vector<pair<T, int>> Rho(const T &n) {
    return RhoC(n, static_cast<T>(1));
}
} // namespace factorizer
