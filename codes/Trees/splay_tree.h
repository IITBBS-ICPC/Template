const int N = 1e6 + 10;
const int M = 998244353;

struct node { // 0 based indexing
  int l, r, p, val;

  // Lazy Values
  int flip, sum, cnt;

  node() : node(0) { cnt = 0; }

  node(int v) {
    sum = val = v;
    flip = false;
    l = r = p = 0;
    // Init lazy
  }
  bool hasLazy() { // Define when lazy is present }
};

node nodes[N] = {node()};
int tp = 1;

int createNode(int v) {
  nodes[tp] = node(v);
  return tp++;
}

int createNode() {
  nodes[tp] = node();
  return tp++;
}

struct SplayTree {
  int root;

  void build(vector<int> &arr) {
    root = createNode(0);
    nodes[root].r = createNode(0);
    nodes[nodes[root].r].p = root;

    vector<int> stk = {nodes[root].r};
    int curr;
    for(auto x : arr) {
      curr = nodes[stk.back()].r = createNode(x);
      nodes[curr].p = stk.back();
      stk.push_back(curr);
    }
    curr = nodes[stk.back()].r = createNode(0);
    nodes[curr].p = stk.back();

    while(stk.size()) {
      pull(stk.back());
      stk.pop_back();
    }
  }

  void flip(int v) {
    if(v)
      nodes[v].flip ^= 1;
  }

  void lazyEval(int v) {// Evaluate lazy to actual value}

  void lazyApply(int v, int b, int c) {
    if(!v)
      return;
    // Merge Lazy updates
  }

  void push(int v) {
    if(nodes[v].flip) {
      swap(nodes[v].l, nodes[v].r);

      flip(nodes[v].l);
      flip(nodes[v].r);

      nodes[v].flip = false;
    }
    if(nodes[v].hasLazy()) {
      lazyApply(nodes[v].l, nodes[v].b, nodes[v].c);
      lazyApply(nodes[v].r, nodes[v].b, nodes[v].c);

      lazyEval(v);
    }
  }

  void pull(int v) {
    if(nodes[v].r)
      push(nodes[v].r);
    if(nodes[v].l)
      push(nodes[v].l);

    // Re-evaluate node
  }

  void rotate(int v) {
    int p = nodes[v].p;
    if(!p) {
      return;
    }
    int pp = nodes[p].p;

    if(nodes[p].l == v) {
      nodes[p].l = nodes[v].r;
      nodes[v].r = p;

      nodes[p].p = v;
      nodes[v].p = pp;
      nodes[nodes[p].l].p = p;
    } else {
      nodes[p].r = nodes[v].l;
      nodes[v].l = p;

      nodes[p].p = v;
      nodes[v].p = pp;
      nodes[nodes[p].r].p = p;
    }

    if(nodes[pp].r == p)
      nodes[pp].r = v;
    else
      nodes[pp].l = v;
  }

  int find(int ind, int root) {
    int v = root;
    while(v) {
      push(v);
      if(nodes[nodes[v].l].cnt >= ind) {
        v = nodes[v].l;
      } else {
        ind -= nodes[nodes[v].l].cnt;
        if(ind == 1) {
          break;
        } else {
          ind--;
          v = nodes[v].r;
        }
      }
    }
    return v;
  }

  void splay(int v, int _root) {
    int cnt = 0;
    while(v != _root) {
      int p = nodes[v].p;
      int pp = nodes[p].p;

      if(p == _root) {
        rotate(v);
        pull(p);
        pull(v);
        break;
      } else {
        if((nodes[pp].l == p) == (nodes[p].l == v)) {
          rotate(p);
          rotate(v);
        } else {
          rotate(v);
          rotate(v);
        }
      }
      pull(pp);
      pull(p);
      pull(v);
      if(pp == _root)
        break;
    }
  }

  int findAndSplay(int ind, int root) {
    int v = find(ind, root);
    if(!v)
      return v;
    splay(v, root);
    return v;
  }

  node query(int l, int r) {
    l += 1, r += 1;
    int v = findAndSplay(l, nodes[root].r);
    int u = findAndSplay(r - l + 2, nodes[v].r);
    push(nodes[u].l);
    return nodes[nodes[u].l];
  }

  void reverse(int l, int r) {
    l += 1, r += 1;
    int v = findAndSplay(l, nodes[root].r);
    int u = findAndSplay(r - l + 2, nodes[v].r);
    flip(nodes[u].l);
  }

  void update(int l, int r, int b, int c) {
    l += 1, r += 1;
    int v = findAndSplay(l, nodes[root].r);
    int u = findAndSplay(r - l + 2, nodes[v].r);
    lazyApply(nodes[u].l, b, c);
    pull(u);
    pull(v);
  }

  void insert(int ind, int x) {
    ind++;
    int v = findAndSplay(ind, nodes[root].r);
    int u = findAndSplay(1, nodes[v].r);
    nodes[u].l = createNode(x);
    nodes[nodes[u].l].p = u;
    pull(u);
    pull(v);
  }

  void remove(int ind) {
    ind++;
    int v = findAndSplay(ind, nodes[root].r);
    int u = findAndSplay(2, nodes[v].r);
    nodes[u].l = 0;
    pull(u);
    pull(v);
  }
};
