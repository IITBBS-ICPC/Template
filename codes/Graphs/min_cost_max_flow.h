namespace Flow {
int fir[N],nxt[M],to[M],w[M],cst[M],ect = 1;
inline void addedge(int u1,int v1,
                        int w1,int c1) {
    nxt[++ect] = fir[u1];fir[u1] = ect;
    to[ect] = v1;w[ect] = w1;cst[ect] = c1;
}
inline void ins(int u1,int v1,int w1,int c1) { 
    addedge(u1,v1,w1,c1);addedge(v1,u1,0,-c1);
}
int dis[N],h[N],vst[N],pw[N],pe[N];
int tot;
inline void Clr(int _n) {
    tot = _n;ect = 1;
    for(int i = 1;i <= tot;i++) 
        fir[i] = h[i] = 0;
}
struct node {
    int id,dis;
    node(){}
    node(const int _id,const int _dis):
        id(_id),dis(_dis){}
    bool operator < (const node &rhs) const { 
        return dis > rhs.dis;
    }
};
priority_queue<node> Q;
bool Dijkstra(int S,int T) {
    for(int i = 1;i <= tot;i++) 
        dis[i] = 1e9,vst[i] = 0;
    Q.emplace(S,dis[S] = 0);
    while(!Q.empty()) {
        int x = Q.top().id;Q.pop();
        if(vst[x]) continue;
        vst[x] = true;
        for(int i = fir[x],y;y = to[i],i;i = nxt[i])
            if(w[i] > 0 && dis[y] > 
                dis[x] + cst[i] + h[x] - h[y]){
                    Q.emplace(y,dis[y] = dis[x] + 
                                cst[i] + h[x] - h[y]),
                            pw[y] = x,pe[y] = i;
            }
    }
    return dis[T] < 1e9;
}
int MCMF(int S,int T,int K) {
    int res = 0,D = 0,flow = 0;
    while(Dijkstra(S,T)) {
        for(int i = 1;i <= tot;i++) h[i] += dis[i];
        int f = 1e9;	
        for(int i = T;i != S;i = pw[i]) 
            f = min(f,w[pe[i]]);
        if(h[T] != 0) f = min(f,D + K - flow);
        if(!f) break;
        for(int i = T;i != S;i = pw[i]) 
            w[pe[i]] -= f,w[pe[i] ^ 1] += f;
        flow += f;res += h[T] * f;
        if(h[T] == 0) D = flow;
    }
    // printf("flow:%d\n",flow);
    return res;
}
}
 