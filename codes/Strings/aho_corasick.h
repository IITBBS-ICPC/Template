const static int K = 26;
struct Vertex {
  int next[K];
  int leaf = 0;
  int p = -1;
  char pch;
  int link = -1;

  Vertex(int p = -1, char ch = '$') : p(p), pch(ch) {
    fill(begin(next), end(next), -1);
  }
};
vector<Vertex> t(1);
void add_s(string const &s) {
  int v = 0;
  for(char ch : s) {
    int c = ch - 'a';
    if(t[v].next[c] == -1) {
      t[v].next[c] = t.size();
      t.emplace_back(v, ch);
    }
    v = t[v].next[c];
  }
  t[v].leaf += 1;
}
int go(int v, char ch);
int get_link(int v) {
  if(t[v].link == -1) {
    if(v == 0 || t[v].p == 0) t[v].link = 0;
    else
      t[v].link = go(get_link(t[v].p), t[v].pch);
  }
  return t[v].link;
}
int go(int v, char ch) {
  int c = ch - 'a';
  if(t[v].next[c] == -1)
    t[v].next[c] = v == 0 ? 0 : go(get_link(v), ch);
  return t[v].next[c];
}
void bfs() {
  queue<int> order;
  order.push(0);
  while(!order.empty()) {
    int cur = order.front();
    order.pop();
    t[cur].link = get_link(cur);
    t[cur].leaf += t[t[cur].link].leaf;
    for(int i = 0; i < K; ++i) {
      if(t[cur].next[i] != -1) {
        order.push(t[cur].next[i]);
      }
    }
  }
}