/*
 * 表达式计算
 * 注意，乘方是右结合的，而本模板不是。
 */

# include <iostream>
# include <cstdio>
# include <cstring>
# include <cmath>
# include <stack>
# include <cstdlib>

using namespace std;

int priv[300];
double value[300];
const double eps = 1e-10;
bool valid = true;

double calc(double a,double b,char op) {
	if(op == '+') return a+b;
	if(op == '-') return a-b;
	if(op == '*') return a*b;
	if(op == '/') {
		if(fabs(b)<eps) {
            valid = false; 
            return 0;
		}
		return a/b;
	}
	if(op == '^') return pow(a,b);
    return 0;
}

double calculate(string str) {
	stack<double> num;
	stack<char> oper;
	priv['+']=priv['-']=3;
	priv['*']=priv['/']=2;
	priv['^']=1; priv['(']=10;
	double x,y;
	size_t i; char last = 0;
	for(i=0;i<str.length();i++) {
		if(isdigit(str[i])||str[i]=='.') {
			num.push(atof(str.c_str()+i));
			for(;i+1<str.length()&&isdigit(str[i+1]);i++);
			if(i+1<str.length()&&str[i+1]=='.')
				for(i++;i+1<str.length()&&isdigit(str[i+1]);i++);
		}else if(str[i]=='(') {
			oper.push(str[i]);
		}else if(str[i]==')') {
			while(oper.top()!='('){
				y=num.top(); num.pop();
				x=num.top(); num.pop();
				char op=oper.top();
				oper.pop();
				num.push(calc(x,y,op));
			}
			oper.pop();
		}else if(str[i]=='-'&&(last==0||last=='(')) {
			num.push(0.0); oper.push('-');
		}else if(str[i]=='+'&&(last==0||last=='(')) {
			num.push(0.0); oper.push('+');
		} else if(priv[str[i]]>0) {
			while(oper.size()>0&&priv[str[i]]>=priv[oper.top()]){
				y=num.top();num.pop();
				x=num.top();num.pop();
				char op=oper.top();
				oper.pop();
				num.push(calc(x,y,op));
			}
			oper.push(str[i]);
		}else continue;
		last = str[i];
	}
	while(oper.size()>0) {
		y=num.top(); num.pop();
		x=num.top(); num.pop();
		char op=oper.top();
		oper.pop();
		num.push(calc(x,y,op));
	}
	return num.top();
}

char a[12000];
int main() {
	while(~scanf("%s",a)) {
		valid = true;
		double ans = calculate(string(a));
		if(!valid) puts("The expression is invalid!");
		else printf("%.8lf\n", ans);
	}
}
