#include<bits/stdc++.h>

const int N=1e6+10;

using namespace std;

int n,a[N];
const int BITS_SIZE=30;
int t[N*BITS_SIZE][2],tsz;
int el[N*BITS_SIZE],tot[N*BITS_SIZE];
struct TrieBits{
    TrieBits(){
        tsz=0; // will overwrite the arrays
               // put arrays inside struct if their size is small
    }
    void add(int x,int id){
      int cur=0;
      tot[cur]++;
      for (int i=BITS_SIZE;i>=0;i--){
        bool b=x&(1<<i);
        if (t[cur][b]==0)t[cur][b]=++tsz;
        cur=t[cur][b];
        tot[cur]++;
      }
      el[cur]=id;
    }
    void del(int x,int id){
      int cur=0;
      tot[cur]--;
      for (int i=BITS_SIZE;i>=0;i--){
        bool b=x&(1<<i);
        cur=t[cur][b];
        tot[cur]--;
      }
      assert(el[cur]==id);
      el[cur]=0;
    }
    int max_xor(int x){
      int cur=0;
      for (int i=BITS_SIZE;i>=0;i--){
        bool b=(x&(1<<i));
        if (t[cur][!b]&&tot[t[cur][!b]]>0) cur=t[cur][!b];
        else cur=t[cur][b];
      }
      assert(el[cur]);
      return el[cur];
    }
};

int main(){
    TrieBits tr;
    cin>>n;
    for (int i=1;i<=n;i++){
        cin>>a[i];
        tr.add(a[i],i);
    }
    int ans=0;
    for (int i=1;i<=n;i++){
        ans=max(ans,(a[i]^a[tr.max_xor(a[i])]));
    }
    cout<<ans<<endl;
    return 0;
}
