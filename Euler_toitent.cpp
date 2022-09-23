// phi(n) = n*(1-1/p1)*(1-1/p2)*......(1-1/pm); where p1,p2,.....pm are distinct prime factors of n 

vector<int> phi;
void phi1_to_n(int n) 
{
    phi.resize(n+1);
    for(int i = 0; i <= n; ++i ) phi[i] = i;
    
    for (int p=2; p<=n; ++p) 
    { 
        if (phi[p]==p) 
        {
            for (int i=p; i<=n; i += p) 
                phi[i] -= phi[i]/p; 
        } 
    } 
}
