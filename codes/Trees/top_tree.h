// should have identity transform
struct Transform{
    long long val;
    Transform():val(0ll){}
    Transform(long long _val):val(_val){}
    Transform& operator +=(Transform &other){
        val+=other.val;
        return *this;
    }
    bool isLazy()const{return val;}
};
// transforming values and summing = summing and transforming
// sum is commutative and associative
// transforming identity = identity
struct Val{
    int n;
    long long val;
    Val(int _n, long long _val):n(_n),val(_val){}
    Val(long long _val):n(1),val(_val){}
    Val():n(0),val(0){}
    Val operator +(Val &other)const{
        return Val(n+other.n, val+other.val);};
    Val& operator +=(Transform &T){val+=(n*T.val);return *this;}
    bool isIdentity()const{return n==0;}
};

template<typename val, typename transform>
class TopTree{
public:
    struct Splay{
        struct node{
            int l,r,ar,p;
            bool flip;
            val self, path, sub, all;
            transform lazyPath, lazySub;
            node():
                l(0),r(0),ar(0),p(0),flip(false){}
            node(int _val):
                l(0),r(0),ar(0),p(0),flip(false),
                self(_val), path(_val), all(_val){}
        };
        int stx;
        vector<node> nodes;
        Splay(int n,int q){
            nodes.assign(n+q+1,node(0));
            nodes[0] = node();
            for(int i=n+1;i<nodes.size();++i)
                nodes[i] = nodes[0];
            stx = n;
        }
        inline void lazyApplyPath(int u, transform &T){
            if(!nodes[u].path.isIdentity()){
                nodes[u].self+=T,nodes[u].path+=T,
                nodes[u].lazyPath+=T;
                nodes[u].all = nodes[u].path+nodes[u].sub;
            }
        }
        inline void lazyApplySub(int u, transform &T){
            if(!nodes[u].sub.isIdentity()){
                nodes[u].sub+=T,nodes[u].lazySub+=T;
                nodes[u].all = nodes[u].path+nodes[u].sub;
            }
        }
        inline void flip(int u){
            swap(nodes[u].l,nodes[u].r);
            nodes[u].flip^=1;
        }
        inline void push(int u){
            if(nodes[u].lazyPath.isLazy()){
                lazyApplyPath(nodes[u].l, nodes[u].lazyPath),
                lazyApplyPath(nodes[u].r, nodes[u].lazyPath);
                nodes[u].lazyPath = transform();
            }
            if(nodes[u].lazySub.isLazy()){
                lazyApplySub(nodes[u].l, nodes[u].lazySub),
                lazyApplySub(nodes[u].r, nodes[u].lazySub),
                lazyApplySub(nodes[u].ar, nodes[u].lazySub),
                lazyApplyPath(nodes[u].ar, nodes[u].lazySub);
                nodes[u].lazySub = transform();
            }
            if(nodes[u].flip){
                nodes[u].flip = false;
                flip(nodes[u].l);
                flip(nodes[u].r);
            }
        }
        inline void pull(int u){
            if(!u)return;
            int lc = nodes[u].l, rc = nodes[u].r, 
                    ar = nodes[u].ar;
            nodes[u].path = nodes[lc].path
                            +nodes[u].self
                            +nodes[rc].path;
            nodes[u].sub = nodes[lc].sub
                            +nodes[rc].sub
                            +nodes[ar].all;
            nodes[u].all = nodes[u].path+nodes[u].sub;
        }
        inline void rotate(int u){
            int p = nodes[u].p;
            if(nodes[p].r==u){
                nodes[p].r = nodes[u].l;
                if(nodes[u].l)
                    nodes[nodes[u].l].p = p;
                nodes[u].l = p;
            }
            else{
                nodes[p].l = nodes[u].r;
                if(nodes[u].r)
                    nodes[nodes[u].r].p = p;
                nodes[u].r = p;
            }
            nodes[u].p = nodes[p].p;
            nodes[p].p = u;
            if(nodes[nodes[u].p].l == p)
                nodes[nodes[u].p].l = u;
            else if(nodes[nodes[u].p].r == p)
                nodes[nodes[u].p].r = u;
            else if(nodes[nodes[u].p].ar == p)
                nodes[nodes[u].p].ar = u;
        }
        inline void splay(int x){
            while((nodes[nodes[x].p].l==x)||
                    (nodes[nodes[x].p].r==x)){
                int y = nodes[x].p;
                int z = nodes[y].p;
                if((nodes[z].l==y)||(nodes[z].r==y)){
                    push(z);push(y);push(x);
                    if(((nodes[z].l==y)&&(
                        nodes[y].l==x))||
                        ((nodes[z].r==y)&&(
                            nodes[y].r==x)))
                        rotate(y);
                    else
                        rotate(x);
                    rotate(x);
                    pull(z);pull(y);pull(x);
                }
                else{
                    push(y);push(x);
                    rotate(x);
                    pull(y);pull(x);
                }
            }
            push(x);
        }
        inline void detach(int u){
            push(u);
            if(nodes[u].r){
                if(nodes[nodes[u].ar].ar
                        ||(!nodes[u].ar)){
                    nodes[++stx].r = nodes[u].ar;
                    nodes[stx].p = u;
                    if(nodes[stx].r)
                        nodes[nodes[stx].r].p = stx;
                    nodes[u].ar = stx;
                }
                else
                    push(nodes[u].ar);
                nodes[nodes[u].ar].ar = nodes[u].r;
                nodes[nodes[u].r].p = nodes[u].ar;
                nodes[u].r = 0;
                pull(nodes[u].ar);
                pull(u);
            }
        }
        inline int access(int u){
            int x = u;
            int v = u;
            while(x){
                splay(x);
                if(u!=x){
                    push(nodes[x].ar);
                    swap(nodes[x].r,
                        nodes[nodes[x].ar].ar);
                    if(nodes[x].r)
                        nodes[nodes[x].r].p = x;
                    if(nodes[nodes[x].ar].ar)
                        nodes[nodes[nodes[x].ar].ar].p = nodes[x].ar;
                    pull(nodes[x].ar);
                    pull(x);
                }
                else 
                    detach(x);
                v = x;
                x = nodes[x].p;
                if(x){
                    splay(x);
                    x = nodes[x].p;
                }
            }
            splay(u);
            return v;
        }
        void root(int x){
            access(x);flip(x);push(x);
        }
    };
    Splay S;
    int root;
    TopTree(int _n, int _q,int _root):S(_n,_q*2),root(_root){}
    void updateSub(int x,transform T){
        S.root(root);S.access(x);
        int y = S.nodes[x].l;
        S.nodes[x].l = 0;
        S.pull(x);
        S.lazyApplyPath(x, T),S.lazyApplySub(x, T);
        S.push(x);S.nodes[x].l = y;S.pull(x);
    }
    void updatePath(int x,int y,transform T){
        S.root(x);S.access(y);S.lazyApplyPath(y, T);
    }
    void reroot(int r){root = r;}
    val getPath(int x,int y){
        S.root(x);S.access(y);
        return S.nodes[y].path;
    }
    val getSub(int x){
        S.root(root);S.access(x);
        return S.nodes[x].self
            +S.nodes[S.nodes[x].r].path
            +S.nodes[S.nodes[x].ar].all;
    }
    void link(int x,int y){
        S.root(x);S.access(y);
        S.nodes[y].r = x,S.nodes[x].p = y;
        S.pull(y);
    }
    int lca(int x,int y){
        S.root(root);S.access(y);
        return S.access(x);
    }
    void changePar(int x,int y){
        if(lca(x,y)!=x){
            S.nodes[S.nodes[x].l].p = 0;
            S.nodes[x].l = 0;S.pull(x);
            link(x,y);
        }
    }
    void cut(int u){
        S.root(root);S.access(u);
        S.nodes[S.nodes[u].l].p = 0;
        S.nodes[u].l = 0;S.pull(u);
    }
};