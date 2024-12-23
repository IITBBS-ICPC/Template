
#define u64 __uint64_t
#define u32 __uint32_t
#define vi vector<int>
#define v64 vector<u64>
const int P1 = 880803841; //(105*2^23)+1
const int P2 = 897581057; //(107*2^23)+1
const int P3 = 998244353; //(119*2^23)+1
template <int mod>
constexpr int powmod(int a, int p = mod - 2) {
  int res = 1;
  while(p) {
    if(p & 1) res = (res * 1ll * a) % mod;
    p >>= 1;
    a = (a * 1ll * a) % mod;
  }
  return res;
}
template <int max_base, int mod, int primitive>
class Ntt {
private:
  constexpr static v64 fill(const int o) {
    v64 res(max_base);
    const int m = max_base >> 1;
    res[m] = (1ll << 32) % mod;
    for(int i = m + 1; i < max_base; ++i)
      res[i] = reduce(res[i - 1] * 1ll * o);
    for(int i = m - 1; i; --i)
      res[i] = res[i << 1];
    res[0] = (1ll << 32) % mod;
    return res;
  }
  constexpr static v64 init_omegas() {
    const int omega
      = (powmod<mod>(primitive, (mod - 1) / max_base)
         * (1ll << 32))
        % mod;
    return fill(omega);
  }
  static const v64 omegas;
  constexpr static v64 init_iomegas() {
    const int omega
      = powmod<mod>(primitive, (mod - 1) / max_base);
    const int iomega
      = (powmod<mod>(omega) * (1ll << 32)) % mod;
    return fill(iomega);
  }
  static const v64 iomegas;
  constexpr static u32 init_N1() {
    u32 inv = N2;
    for(int i = 0; i < 4; ++i)
      inv *= (2 - (N2 * inv));
    return -inv;
  }
  static const u32 N1 = init_N1();
  static const u64 N = mod;
  static const u32 N2 = mod;
  static const u32 reduce(u64 x) {
    u32 t
      = (x + (((static_cast<u32>(x)) * N1) * N)) >> 32;
    if(t >= mod) t -= mod;
    return t;
  }

public:
  static void fft(vi &a) {
    int n = a.size();
    for(int m = n >> 1; m; m >>= 1) {
      auto it_start = omegas.begin() + m;
      auto it_end = it_start + m;
      for(auto l = a.begin(); l != a.end(); l += m) {
        for(auto it = it_start; it != it_end;
            ++it, ++l) {
          int e = *l - l[m];
          if(e < 0) e += mod;
          *l += l[m];
          if(*l >= mod) *l -= mod;
          l[m] = reduce(e * *it);
        }
      }
    }
  }
  static void ifft(vi &a) {
    int n = a.size();
    for(int m = 1; m < n; m <<= 1) {
      auto it_start = iomegas.begin() + m;
      auto it_end = it_start + m;
      for(auto l = a.begin(); l != a.end(); l += m) {
        for(auto it = it_start; it != it_end;
            ++it, ++l) {
          l[m] = reduce(l[m] * *it);
          int e = *l - l[m];
          if(e < 0) e += mod;
          *l += l[m];
          if(*l >= mod) *l -= mod;
          l[m] = e;
        }
      }
    }
  }
  static vi mul(vi a, vi b) {
    int need = a.size() + b.size() - 1;
    int nbase = 1 << (32 - __builtin_clz(need - 1));
    a.resize(nbase);
    b.resize(nbase);
    fft(a);
    fft(b);
    for(int i = 0; i < nbase; ++i)
      a[i] = reduce(a[i] * 1ll * b[i]);
    ifft(a);
    u64 f = ((((1ll << 32) / nbase) % mod) * 1ll
             * omegas[0])
            % mod;
    a.resize(need);
    for(int i = 0; i < need; ++i)
      a[i] = reduce(a[i] * f);
    return a;
  }
  static vi inv(vi &a) {
    int n = a.size();
    vi res(1, powmod<mod>(a[0]));
    int k = 1;
    while(k < n) {
      int l = k << 1;
      int need = l << 1;
      if(l > n) a.resize(l);
      res.resize(need);
      vi temp(a.begin(), a.begin() + l);
      temp.resize(need);
      fft(res);
      fft(temp);
      for(int i = 0; i < need; ++i)
        res[i]
          = reduce(temp[i] * 1ll
                   * reduce(res[i] * 1ll * res[i]));
      ifft(res);
      u64 f = ((((1ll << 32) / need) % mod) * 1ll
               * omegas[0])
              % mod;
      f = (f << 32) % mod;
      res.resize(l);
      for(int i = 0; i < l; ++i)
        res[i] = reduce(f * res[i]);
      for(int i = k; i < l; ++i)
        if(res[i]) res[i] = mod - res[i];
      k = l;
    }
    a.resize(n);
    res.resize(n);
    return res;
  }
};
template <int max_base, int mod, int omega>
const v64 Ntt<max_base, mod, omega>::omegas
  = init_omegas();
template <int max_base, int mod, int omega>
const v64 Ntt<max_base, mod, omega>::iomegas
  = init_iomegas();
const int mod = 998244353;
const int base = 1 << 20;
const int primitive = 3;
vi &operator*=(vi &a, const vi &b) {
  if(a.empty() || b.empty()) a.clear();
  else
    a = Ntt<base, mod, primitive>::mul(a, b);
  return a;
}
vi operator*(const vi &a, const vi &b) {
  vi c = a;
  return c *= b;
}
vi &operator/=(vi &a, const vi &b) {
  if(a.size() < b.size()) a.clear();
  else {
    vi d = b;
    reverse(d.begin(), d.end());
    reverse(a.begin(), a.end());
    int deg = a.size() - b.size();
    a.resize(deg + 1);
    d.resize(deg + 1);
    d = Ntt<base, mod, primitive>::inv(d);
    a *= d;
    a.resize(deg + 1);
    reverse(a.begin(), a.end());
  }
  return a;
}
vi operator/(vi &a, const vi &b) {
  vi c = a;
  return c /= b;
}
vi &operator+=(vi &a, const vi &b) {
  if(a.size() < b.size()) a.resize(b.size());
  for(int i = 0; i < b.size(); ++i) {
    a[i] += b[i];
    if(a[i] >= mod) a[i] -= mod;
  }
  return a;
}
vi operator+(const vi &a, const vi &b) {
  vi c = a;
  return c += b;
}
vi &operator-=(vi &a, const vi &b) {
  if(a.size() < b.size()) a.resize(b.size());
  for(int i = 0; i < b.size(); ++i) {
    a[i] -= b[i];
    if(a[i] < 0) a[i] += mod;
  }
  return a;
}
vi operator-(const vi &a, const vi &b) {
  vi c = a;
  return c -= b;
}
vi &operator%=(vi &a, const vi &b) {
  if(a.size() < b.size()) return a;
  vi c = (a / b) * b;
  a -= c;
  a.resize(b.size() - 1);
  return a;
}
vi operator%(const vi &a, const vi &b) {
  vi c = a;
  return c %= b;
}