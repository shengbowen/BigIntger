#include <iostream>
#include <assert.h>
#include <string>
#include <cstring>
using namespace std;

const int maxn = 200 +1;
class Num{
private:
	int a[maxn];
	int length;
	bool neg;

public:
	Num();
	Num operator +(const Num &b) const;
	Num operator -(const Num &b) const;
	Num operator *(const Num &b) const;
	Num operator *(int b) const;
	Num operator /(const Num &b) const;

	Num operator <<(int shft) const;
	int& operator [](int idx);
	int operator [](int idx) const;
	bool operator <(const Num &b) const;

	void init();
	void print();
};

Num::Num(){
	memset(a, 0, sizeof(a));
	length = 0;
	neg = 0;
}

int& Num::operator [](int idx){
	assert(idx>=0 && idx <maxn);
	return a[idx];
}
int Num::operator [](int idx) const{
	assert(idx>=0 && idx <maxn);
	return a[idx];
}

Num Num::operator +(const Num &b) const{
	Num c;
	int t = 0;
	for (int i = 0; i<length || i<b.length || t; i++){
		int s = a[i] + b[i] + t;
		c[i] = s%10;
		t = s/10;
		c.length++;
	}
	return c;
}

bool Num::operator <(const Num &b) const{
	if (length != b.length){
		return length < b.length;
	}
	for (int i = length-1; i>=0; i--){
		if (a[i] != b[i]){
			return a[i] < b[i];
		}
	}
	return false;
}

Num Num::operator -(const Num &b) const{
	if (*this < b){
		Num c = b - *this;
		c.neg = true;
		return c;
	}

	int t = 0;
	Num c;
	for (int i = 0; i<length; i++){
		int s = a[i] - b[i] - t;
		if (s < 0){
			s += 10;
			t = 1;
		}else{
			t = 0;
		}
		c[i] = s;
		c.length++;
	}
	while (c.length > 0 && c[c.length-1] == 0){
		c.length--;
	}
	return c;
}

Num Num::operator *(int b) const{
	Num c;
	int t = 0;
	for (int i = 0; i<length || t; i++){
		int s = b * a[i] + t;
		c[i] = s%10;
		t = s/10;
		c.length++;
	}
	return c;
}

Num Num::operator <<(int shft) const{
	Num c;
	for (int i = 0; i<length; i++){
		c[i+shft] = a[i];
	}
	c.length = length + shft;
	return c;
}

Num Num::operator *(const Num &b) const{
	Num c;
	for (int i = 0; i<b.length; i++){
		c = c + (((*this) * b[i]) << i);
	}
	return c;
}

Num Num::operator /(const Num &b) const{
	assert(b.length > 0);

	Num c, tmpa = *this;
	c.length = length - b.length +1;
	for (int i = length - b.length; i>=0; i--){
		Num d = (b << i);

		/* while (tmpa >= d) */
		while (! (tmpa < d) ){
			tmpa = tmpa - d;
			c[i]++;
		}
	}
	while (c.length > 0 && c[c.length-1] == 0){
		c.length--;
	}
	return c;
}

void Num::init(){
	char tmp[100 + 1];
	cin>>tmp;
	length = strlen(tmp);
	for (int i = 0; i<length; i++){
		a[length - i - 1] = tmp[i] - '0';
	}
}

void Num::print(){
	if (length == 0){
		cout<<0<<endl;
		return ;
	}
	if (neg){
		cout<<"-";
	}
	for (int i = length-1; i>=0; i--){
		cout<<a[i];
	}
	cout<<endl;
}
int main(){
	Num a, b, c;
	char op[2];
	a.init();
	cin>>op;
	b.init();
	if (op[0] == '+'){
		c = a+b;
	}
	if (op[0] == '-'){
		c = a-b;
	}
	if (op[0] == '*'){
		c = a*b;
	}
	if (op[0] == '/'){
		c = a/b;
	}
	c.print();
	return 0;
}


