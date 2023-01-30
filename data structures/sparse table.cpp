const int K=20;
int st[K+1][N];
int lg[N+1];
int query(int L,int R){
    int i=lg[R-L+1];
    return min(st[i][L],st[i][R-(1<<i)+1]);
}

void build(){
    lg[1]=0;
    for (int i=2;i<=N;i++)
        lg[i]=lg[i/2]+1;

    for (int i=0;i<n;i++)st[0][i]=a[i];

    for (int i=1;i<=K;i++)
        for (int j=0;j+(1<<i)<=n;j++)
            st[i][j]=min(st[i-1][j],st[i-1][j+(1<<(i-1))]);
}
