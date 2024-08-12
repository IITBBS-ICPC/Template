// CSES Problem "Finding Patterns" : https://cses.fi/problemset/task/2102
// Submission Link : https://cses.fi/paste/89c9bd1a052eb3659af826/

#include <iostream>
#include <map>
#include <string>
using namespace std;
 
struct state {
    int len, link;
    map<char, int> next;
};
 
const int MAXLEN = 100000;
state st[MAXLEN * 2];
int sz, last;
 
void sa_init() {
    st[0].len = 0;
    st[0].link = -1;
    sz = 1;  // Initialize sz to 1 since we start from 0
    last = 0;
}
 
void sa_extend(char c) {
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    int p = last;
    while (p != -1 && !st[p].next.count(c)) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            st[clone].next = st[q].next;
            st[clone].link = st[q].link;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last = cur;
}
 
bool pattern_exists(const string &pattern) {
    int current_state = 0;
    for (char c : pattern) {
        if (!st[current_state].next.count(c)) {
            return false;
        }
        current_state = st[current_state].next[c];
    }
    return true;
}
 
int main() {
    string s;
    cin >> s;
 
    sa_init();
    for (char c : s) {
        sa_extend(c);
    }
 
    int k;
    cin >> k;
    while (k--) {
        string pattern;
        cin >> pattern;
        if (pattern_exists(pattern)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
 
    return 0;
}