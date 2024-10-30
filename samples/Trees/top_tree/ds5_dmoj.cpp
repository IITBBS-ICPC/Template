// DMOJ Problem Link : https://dmoj.ca/problem/ds5
// Submission Link : Not avaialable , submit and test yourself

#include<bits/stdc++.h>
using namespace std;
// should have identity transform
struct Transform{
    bool set;
    int val;
    Transform():set(false),val(0ll){}
    Transform(int _val):set(false),val(_val){}
    Transform(bool _set, int _val):set(_set),val(_val){}
    Transform& operator +=(Transform &other){
        if(other.set){
            set = true;
            val = other.val;
        }
        else val+=other.val;
        return *this;
    }
    bool isLazy()const{return val||set;}
};
// transforming values and summing = summing and transforming
// sum is commutative and associative
// transforming identity = identity
struct Val{
    int n;
    int sum,cmin,cmax;
    Val(int _val):n(1),sum(_val),cmin(_val),cmax(_val){}
    Val(int _n,int _sum,int _cmin,int _cmax):
        n(_n),sum(_sum),cmax(_cmax),cmin(_cmin){}
    Val():n(0){}
    Val operator +(Val &other)const{
        if(!other.n)return *this;
        if(!n)return other;
        return Val(
            n+other.n,
            sum+other.sum,
            min(cmin,other.cmin),
            max(cmax,other.cmax)
        );
    };
    Val& operator +=(Transform &T){
        if(!n)return *this;
        if(T.set){
            sum=(n*T.val);
            cmax=cmin=T.val;
        }
        else{
            sum+=(n*T.val);
            cmax+=T.val;
            cmin+=T.val;
        }
        return *this;
    }
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

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n,m;
    cin>>n>>m;
    TopTree<Val,Transform> T(n,n+n+n+m,1);
    for(int i=1;i<n;++i){
        int u,v;
        cin>>u>>v;
        T.link(u,v);
    }
    for(int i=1;i<=n;++i){
        int v;
        cin>>v;
        T.updatePath(i,i,Transform(true,v));
    }
    int root;
    cin>>root;
    T.reroot(root);
    for(int i=0;i<m;++i){
        int k;
        cin>>k;
        if(k==0){
            int x,y;
            cin>>x>>y;
            T.updateSub(x,Transform(true,y));
        }
        else if(k==1){
            int x;
            cin>>x;
            T.reroot(x);
        }
        else if(k==2){
            int x,y,z;
            cin>>x>>y>>z;
            T.updatePath(x,y,Transform(true,z));
        }
        else if(k==3){
            int x;
            cin>>x;
            cout<<T.getSub(x).cmin<<"\n";
        }
        else if(k==4){
            int x;
            cin>>x;
            cout<<T.getSub(x).cmax<<"\n";
        }
        else if(k==5){
            int x,y;
            cin>>x>>y;
            T.updateSub(x,Transform(y));
        }
        else if(k==6){
            int x,y,z;
            cin>>x>>y>>z;
            T.updatePath(x,y,Transform(z));
        }
        else if(k==7){
            int x,y;
            cin>>x>>y;
            cout<<T.getPath(x,y).cmin<<"\n";
        }
        else if(k==8){
            int x,y;
            cin>>x>>y;
            cout<<T.getPath(x,y).cmax<<"\n";
        }
        else if(k==9){
            int x,y;
            cin>>x>>y;
            T.changePar(x,y);
        }
        else if(k==10){
            int x,y;
            cin>>x>>y;
            cout<<T.getPath(x,y).sum<<"\n";
        }
        else{
            int x;
            cin>>x;
            cout<<T.getSub(x).sum<<"\n";
        }
    }
}