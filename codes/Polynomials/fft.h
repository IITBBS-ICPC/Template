#define vi vector<int>
using i64 = int64_t;
typedef complex<double> cd;
constexpr int MAXN = 2e5 + 10;
int rev[MAXN * 3];
const cd I(0, 1);
const double PI = acos(-1);
cd F[MAXN * 3];
 
void fft(int N, int sgn = 1) {
    int bit = __lg(N);
    for (int i = 0; i < N; ++i) {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
        if (i < rev[i])
            swap(F[i], F[rev[i]]);
    }
    for (int l = 1; l < N; l <<= 1) {
        cd step = exp(sgn * PI / l * I);
        for (int i = 0; i < N; i += l * 2) {
            cd cur(1, 0);
            for (int k = i; k < i + l; ++k) {
                cd g = F[k], h = F[k + l] * cur;
                F[k] = g + h, F[k + l] = g - h;
                cur *= step;
            }
        }
    }
}
 
vi convolution(vi A, vi B) {
    int n = A.size() - 1, m = B.size() - 1;
    int N = 1 << __lg(n + m + 1) + 1;
    for (int i = 0; i <= N; ++i)
        F[i] = cd(i <= n ? A[i] : 0, i <= m ? B[i] : 0);
    fft(N);
    for (int i = 0; i <= N; ++i)
        F[i] = F[i] * F[i];
    fft(N, -1);
    vi C(n + m + 1);
 
    for (int i = 0; i <= n + m; ++i) {
        C[i] = int64_t(round(F[i].imag() / (N * 2))) % 1009;
    }
    return C;
}