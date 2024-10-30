#include<bits/stdc++.h>
using namespace std;
#include "../../../codes/NumberTheory/inequality_solver.h"

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    for(int n=1;n<=3;++n){
        // x>=y
        inEq i1(-1,1,0);
        //x+y>=0
        inEq i2(-1,-1,0);
        //x+y<=2n
        inEq i3(1,1,n<<1);
        // x-y<=2n
        inEq i4(1,-1,n<<1);
        vector<inEq> q = {i1,i2,i3,i4};
        cout<<integerInequalitySolver(q)<<"\n";
    }

}
