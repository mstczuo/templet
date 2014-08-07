# include <iostream>
# include <cstdio>

using namespace std;

typedef long long ll;

ll mod_mul(ll x, ll y, ll n){
    ll T = floor(sqrt(n) + 0.5);
    ll t = T * T - n;
    ll a = x / T, b = x % T;
    ll c = y / T, d = y % T;
    ll e = a * c / T, f = a * c % T;
    ll v = ((a * d + b * c) % n + e * t) % n;
    ll g = v / T, h = v % T;
    ll ret = (((f + g) * t % n + b * d) % n + h * T) % n;
    return ret = (ret % n + n) % n;
}

ll exgcd(ll a,ll b,ll &x,ll &y) {
    if(b == 0) {x = 1; y = 0; return a;}
    ll d = exgcd(b, a%b, y, x);
    y -= a / b * x; return d;
}

ll CRT(ll m[], ll b[], int n) {
    ll x, y, d, result, a1, m1, a2, m2;
    m1 = m[0], a1 = b[0];
    for(int i = 1; i < n; i++){
        m2 = m[i], a2 = b[i];
        d = exgcd(m1, m2, x, y);
        if ((a2 - a1) % d != 0) return -1;
        result = (mod_mul(x, (a2 - a1) / d, m2) % m2 + m2) % m2;
        ll tmp = m1;
        m1 = m1 / d * m2;
        a1 = (a1 + mod_mul(tmp, result, m1)) % m1;
        a1 = (a1 % m1 + m1) % m1;
    }
    return a1;
}

ll gcd(ll a,ll b) {
    for(ll t;b;t=a,a=b,b=t%a);
    return a;
}


