#include <tuple>
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template <typename T>
struct DirectedMST {
    typedef tuple<int,int,T> Edge;  // (u, v, w)   u -- (w) --> v
    static int& _U(Edge& e) {
        return get<0>(e);
    }
    static int& _V(Edge& e) {
        return get<1>(e);
    }
    static T& _W(Edge& e) {
        return get<2>(e);
    }

    int N;
    vector<Edge> edges;

    DirectedMST() : N(0) {
    }

    explicit DirectedMST(int n) {
        init(n);
    }

    ~DirectedMST() {
    }

    void init(int n) {
        N = n;
        edges.clear();
    }

    // add an edge of u -> v 
    // [CAUTION]
    // - if this edge is selected in the MST result, u is the parent of v
    // - if v -> u direction is used finally, w should be the weight of v -> u
    void addEdge(int u, int v, T w) {
        edges.emplace_back(u, v, w);
    }

    // ---

    // - return (weight-sum, { parent[vertex 0], parent[vertex 1], ..., parent[vertex N-1] })
    //   --> the directions of result edges are reversed.
    //   --> If an u -> v edge is selected, u is the parent of v
    // - if no MST exists, return (-1, {})
    // O(E*logV)
    pair<T, vector<int>> mst(int root) {
        HeapNodes nodes;
        vector<Node*> heap(N);
        for (auto& e : edges)
            heap[_V(e)] = Node::merge(heap[_V(e)], nodes.createNode(e));

        RollbackUF uf(N);

        T res = 0;
        vector<int> seen(N, -1), path(N), parent(N);
        seen[root] = root;
        vector<Edge> Q(N), in(N, { -1, -1, 0 }), comp;
        deque<tuple<int, int, vector<Edge>>> cycs;
        for (int s = 0; s < N; s++) {
            int u = s, qi = 0, w;
            while (seen[u] < 0) {
                if (!heap[u])
                    return { -1, {} };
                Edge e = heap[u]->top();
                heap[u]->delta -= _W(e);
                heap[u] = heap[u]->pop();

                Q[qi] = e;
                path[qi++] = u;
                seen[u] = s;
                res += _W(e);
                u = uf.find(_U(e));
                if (seen[u] == s) { // found cycle, contract
                    Node* cyc = 0;
                    int end = qi, time = uf.time();
                    do {
                        cyc = Node::merge(cyc, heap[w = path[--qi]]);
                    } while (uf.merge(u, w));
                    u = uf.find(u);
                    heap[u] = cyc;
                    seen[u] = -1;
                    cycs.push_front({ u, time, { &Q[qi], &Q[end] } });
                }
            }
            for (int i = 0; i < qi; i++)
                in[uf.find(_V(Q[i]))] = Q[i];
        }

        // restore to select edges
        for (int i = 0; i < int(cycs.size()); i++) {
            auto u = get<0>(cycs[i]);
            auto t = get<1>(cycs[i]);
            uf.rollback(t);
            Edge inEdge = in[u];
            for (auto& e : get<2>(cycs[i]))
                in[uf.find(_V(e))] = e;
            in[uf.find(_V(inEdge))] = inEdge;
        }

        for (int i = 0; i < N; i++)
            parent[i] = _U(in[i]);

        return { res, parent };
    }

private:
    struct RollbackUF {
        vector<int> e;
        vector<pair<int, int>> st;

        RollbackUF(int n) : e(n, -1) {
        }

        int size(int x) {
            return -e[find(x)];
        }

        int find(int x) {
            return e[x] < 0 ? x : find(e[x]);
        }

        int time() {
            return int(st.size());
        }

        void rollback(int t) {
            for (int i = time(); i-- > t; )
                e[st[i].first] = st[i].second;
            st.resize(t);
        }

        bool merge(int a, int b) {
            a = find(a);
            b = find(b);
            if (a == b)
                return false;

            if (e[a] > e[b])
                swap(a, b);
            st.push_back({ a, e[a] });
            st.push_back({ b, e[b] });
            e[a] += e[b];
            e[b] = a;
            return true;
        }
    };

    // heap node
    struct Node {
        Edge key;
        Node *l, *r;
        T    delta;

        void init(const Edge& e) {
            key = e;
            l = r = nullptr;
            delta = 0;
        }

        void pushDown() {
            _W(key) += delta;
            if (l)
                l->delta += delta;
            if (r)
                r->delta += delta;
            delta = 0;
        }

        Edge top() {
            pushDown();
            return key;
        }

        Node* pop() {
            pushDown();
            return merge(l, r);
        }

        static Node* merge(Node* a, Node* b) {
            if (!a || !b)
                return a ? a : b;

            a->pushDown();
            b->pushDown();
            if (_W(a->key) > _W(b->key))
                swap(a, b);

            a->r = merge(b, a->r);
            swap(a->l, a->r);

            return a;
        }
    };

    struct HeapNodes {
        vector<Node*> nodes;

        ~HeapNodes() {
            for (auto* p : nodes)
                delete p;
            nodes.clear();
        }

        Node* createNode(const Edge& e) {
            nodes.push_back(new Node{ e, nullptr, nullptr, 0 });
            return nodes.back();
        }
    };
};

int main(void) {
    int N, M, S;
    cin >> N >> M >> S;

    DirectedMST<long long> dmst(N);
    int u, v, w;
    for (int i = 0; i < M; i++) {
        cin >> u >> v >> w;
        dmst.addEdge(u, v, w);
    }

    auto ans = dmst.mst(S);
    ans.second[S] = S;

    cout << ans.first << endl;
    cout << ans.second[0];
    for (int i = 1; i < N; i++)
        cout << ' ' << ans.second[i];
    cout << endl;

    return 0;
}