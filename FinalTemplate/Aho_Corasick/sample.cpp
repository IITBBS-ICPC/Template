#include<bits/stdc++.h>
using namespace std;

#include "template.h"

// Based on CF problem , submission link is in README.md

const int INF = 1e9 + 5;
const int K = 10; // For digits 0-9
 
struct Vertex {
    int next[K];
    int leaf = 0; // Number of words ending at this node.
    int p = -1;
    char pch;
    int link = -1;
    int go[K];      
 
    Vertex(int p=-1, char ch='$') : p(p), pch(ch) {
        fill(begin(next), end(next), -1);
        fill(begin(go), end(go), -1);
    }
};
vector<Vertex> t;   // Automation is stored in form of vector. 
 
// Add String s to the automaton.
void add_s(string const& s) {
    int v = 0;
    for (char ch : s) {
        int c = ch - '0';
        if (t[v].next[c] == -1) {
            t[v].next[c] = t.size();
            t.emplace_back(v, ch);
        }
        v = t[v].next[c];
    }
    t[v].leaf += 1;
}
 
// Forward declaration of functions
int go(int v, char ch);
 
// gets the link from vertex v.
int get_link(int v) {
    if (t[v].link == -1) {
        if (v == 0 || t[v].p == 0)
            t[v].link = 0;
        else
            t[v].link = go(get_link(t[v].p), t[v].pch);
    }
    return t[v].link;
}
 
int go(int v, char ch) {
    int c = ch - '0';
    if (t[v].go[c] == -1) {
        if (t[v].next[c] != -1)
            t[v].go[c] = t[v].next[c];
        else
            t[v].go[c] = v == 0 ? 0 : go(get_link(v), ch);
    }
    return t[v].go[c];
} 
 
// To calculate links and leafs (exit link) for all nodes.
void bfs() {
    queue<int> order;
    order.push(0);
    while(!order.empty()) {
        int cur = order.front(); order.pop();
        t[cur].link = get_link(cur);
        t[cur].leaf += t[t[cur].link].leaf;
        for(int i=0; i<K; ++i) {
            if(t[cur].next[i] != -1) {
                order.push(t[cur].next[i]);
            }
        }
    }
}
 
int N, X;
string S;
vector<string> prime;
 
bool check(string str) {
    int n = int(str.size());
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            sum += str[j] - '0';
            if (sum < X && X % sum == 0)
                return false;
        }
    }
    return true;
}
 
void generate(string str, int sum) {
    if (sum > X)
        return;
    if (sum == X) {
        if (check(str))
            prime.push_back(str);
        return;
    }
    for (int d = 1; d <= 9 && sum + d <= X; d++)
        generate(str + char(d + '0'), sum + d);
}
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
 
    cin >> S >> X;
    N = int(S.size());
 
    generate("", 0); // Generate all valid prime strings.
    t.emplace_back(); // Start with the root vertex.
    for (const string &word : prime)
        add_s(word);  // Add all generated primes to the automaton.
 
    bfs();  // Build suffix links and update leaf counts.
 
    int ST = int(t.size());
    vector<int> dp(ST, INF);
    dp[0] = 0;
 
    for (auto &ch : S) {
        vector<int> next_dp(ST, INF);
        for (int state = 0; state < ST; state++) {
            if (dp[state] < INF) {
                next_dp[state] = min(next_dp[state], dp[state] + 1);
                int transition = go(state, ch);
                if (t[transition].leaf == 0) // Only transition if not ending in a word.
                    next_dp[transition] = min(next_dp[transition], dp[state]);
            }
        }
        swap(dp, next_dp);
    }
 
    cout << *min_element(dp.begin(), dp.end()) << '\n';
}