void muland(vector<int> &a, int inv) {
    int n = a.size();
    for(int len = 1; 2*len <= n; len <<= 1) {
        for(int i=0;i<n;i+=2*len) {
            for(int j=0;j<len;++j) {
                int str = a[i+j];
                if(!inv) {
                    a[i+j] = a[i+j+len];
                    a[i+j+len] += str;
                } else {
                    a[i+j] = -str+a[i+j+len];
                    a[i+j+len] = str;
                }
            }
        }
    }
}


void mulor(vector<int> &a, int inv) {
    int n = a.size();
    for(int len = 2; len <= n; len <<= 1) {
        for(int i=0;i<n;i+=len) {
            for(int j=0;j<len/2;++j) {
                int str = a[i+j];
                if(!inv) {
                    a[i+j] += a[i+j+len/2];
                    a[i+j + len/2] = str;   
                } else {
                    a[i+j] = a[i+j+len/2];
                    a[i+j + len/2] = str - a[i+j+len/2];  
                }
            }
        }
    }
}

void mulxor(vector<int> &a, int inv) {
    int n = a.size();
    for(int len = 2; len <= n; len <<= 1) {
        for(int i=0;i<n;i+=len) {
            for(int j=0;j<len/2;++j) {
                int str = a[i+j];
                a[i+j] = a[i+j] + a[i+j+len/2];
                a[i+j + len/2] = str - a[i+j+len/2]; 
            }
        }
    }
    if(inv) for(int i=0;i<n;++i) a[i]/=n;
}

vector<int> Multiand(vector<int> &a, vector<int> &b) {
    int n = max(b.size(), a.size());
    int i=1;
    while(n > i) {
        i*=2;
    } n = i;
    while(a.size()!=n) a.pb(0);
    while(b.size()!=n) b.pb(0);
    
	muland(a,0); muland(b,0);
	vector<int> ans;
	for(int i=0;i<a.size();++i) 
	   ans.pb(a[i]*b[i]);
	muland(ans,1);
	return ans;
}