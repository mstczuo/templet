/* POJ 3580 */
# include <iostream>
# include <cstdio>
# include <cstring> 
# define N 200010
using namespace std;
const int inf=0x7fffffff;

int l[N],r[N],s[N],m[N],v[N],p[N];
int * cur, root; 
bool rev[N]; 

inline void update(const int x){
    if ( !x ) return;
    s[x]=s[l[x]]+s[r[x]]+1;
    m[x]=min(v[x],min(m[l[x]],m[r[x]]));
}

inline void UPD(){
    update(r[root]);
    update(root);
}

inline void left(int &i)
{
    int j=r[i];r[i]=l[j];l[j]=i;
    update(i); /*update(j);*/ i=j;
}

inline void right(int &i)
{
    int j=l[i];l[i]=r[j];r[j]=i;
    update(i); /*update(j);*/ i=j;
}

inline void change(int x,int d,bool t)
{
    if ( !x ) return;
    if ( d ) { m[x]+=d; v[x]+=d; p[x]+=d;}
    if ( t ) {
        swap(l[x],r[x]);
        rev[x]=!rev[x]; 
    }
}

inline void put_tag(const int x)
{
    change(l[x],p[x],rev[x]);
    change(r[x],p[x],rev[x]);
    p[x]=0; rev[x]=false; 
}

void new_node(int &x,const int V)
{
    static int cnt=0;
    x = ++cnt;
    v[x] = m[x] = V;
    s[x] = 1;
}

void Tree_init(int &x,int L,int R)
{
    if ( L>R ){ x=0; return; }
    int Mid = (L+R)>>1;
    new_node(x,0);
    Tree_init(l[x],L,Mid-1);
    scanf("%d",&v[x]);
    Tree_init(r[x],Mid+1,R);
    update(x);
}

inline void Tree_init(int n)
{
    v[0]=m[0]=inf;
    new_node(root,inf);
    new_node(r[root],inf);
    Tree_init(l[r[root]],1,n);
    UPD();
}

inline void rank(int &x,int k)
{
    put_tag(x); 
    if ( k==s[l[x]]+1 ) return;
    if ( k<=s[l[x]] ) { rank(l[x],k); right(x); }
    else { rank(r[x],k-s[l[x]]-1); left(x); } 
}

void _rank(int &x,int k)
{
    rank(x,k);
    update(x);
}

inline void Splay(const int x,int len)
{
    _rank(root,x);
    _rank(r[root],len+1);
    cur = & l[r[root]];     // cur 表示Splay之后当前的区间。 
}

int x,y,val;

void get_min(){
    scanf("%d%d",&x,&y);
    Splay(x,y-x+1);
    printf("%d\n",m[*cur]);
}

void _insert(){
    scanf("%d%d",&x,&val);
    Splay(x+1,0);
    new_node(*cur,val);
    UPD();
}

void _delete()
{
    scanf("%d",&x);
    Splay(x,1);
    *cur = 0;
    UPD();
}

void _add()
{
    scanf("%d%d%d",&x,&y,&val);
    Splay(x,y-x+1);
    change(*cur,val,false);
    UPD();
}

void _reverse()
{
    scanf("%d%d",&x,&y);
    Splay(x,y-x+1);
    change(*cur,0,true);
    UPD();
}

void _revolve()
{
    static int L1,L2,S1,S2;
    scanf("%d%d%d",&x,&y,&val);
    val %= (y-x+1);
    if ( val==0 ) return;
    L1=(y-x+1)-val, L2=val;
    Splay(x,L1); S1=*cur; *cur=0; UPD();
    Splay(x,L2); S2=*cur; *cur=0; UPD();
    Splay(x,0); *cur=S1; UPD();
    Splay(x,0); *cur=S2; UPD();
}

void _check(const int x,const int d,const bool f)
{
    if ( !x ) return;
    int s1=l[x], s2=r[x];
    if ( f ) swap(s1,s2);
    _check(s1,d+p[x],f^rev[x]);
    if ( x>2 )
        printf("%d ",v[x]+d);
    _check(s2,d+p[x],f^rev[x]);
}

void DEBUG()
{
    _check(root,0,false);
    cout << endl;
}

void per_run()
{
    static char st[16];
    scanf("%s",st);
    if ( !strcmp(st,"ADD") ) _add(); 
    else if ( !strcmp(st,"REVERSE") ) _reverse(); 
    else if ( !strcmp(st,"REVOLVE") ) _revolve(); 
    else if ( !strcmp(st,"INSERT") ) _insert(); 
    else if ( !strcmp(st,"DELETE") ) _delete(); 
    else if ( !strcmp(st,"MIN") ) get_min(); 
    else if ( !strcmp(st,"DEBUG") ) DEBUG();
}

int n;
int main()
{
    scanf("%d",&n);
    Tree_init(n);
    scanf("%d",&n);
    while(n--)per_run();
    return 0;
}
