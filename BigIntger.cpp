#include<iostream>
#include<string>
#include<cstring>
#include<cmath>
#include<cassert>
using namespace std;

class BigIntger
{
private:
	int m_size;
	char * m_num;
	int status;//status表示为0正数或1为负数
public:
	BigIntger():m_size(0),m_num(0),status(0){}
	BigIntger(string s,int sta=0):status(sta)
	{
		if(status==0){
		m_size=s.length();
		m_num=new char[m_size+1];//多一位放'\0'
		strcpy(m_num,s.c_str());
		}
		else if(status==1){
		m_size=s.length()+1;
		m_num=new char[m_size+1];//多2位放'\0'和‘-’
		m_num[0]='-';
		m_num[1]='\0';
		strcat(m_num,s.c_str());	
		}
	}

	~BigIntger()
	{
	delete []m_num;
	}

	BigIntger(const BigIntger& big)
	{
		//delete[] m_num;
		if(big.m_num==NULL)
		{
		m_num=0;
		m_size=0;
		status=0;
		}
		//if(m_num!=0){delete []m_num;}
		else
		{
		m_size=big.m_size;
		m_num=new char[m_size+1];
		strcpy(m_num,big.m_num);
		status=big.status;
		}
	}
	
	BigIntger& operator=(const BigIntger&);
	BigIntger mutiInt(int n);//乘法时调用，用来对左乘数乘以0~9
	BigIntger shiftInt(int n);//对于乘法，乘数十位以上的乘被乘数结果要加0
	friend void makeSameSize(char* ,char* ,const BigIntger&,const BigIntger&);
	friend BigIntger operator+(const BigIntger&,const BigIntger&);//返回对象
	friend BigIntger operator-(const BigIntger&,const BigIntger&);
	friend ostream& operator<<(ostream& out,const BigIntger&);
	friend BigIntger operator*(const BigIntger&,const BigIntger&);
	friend BigIntger operator/(const BigIntger&,const BigIntger&);
};
//获得长度相等的数组
void makeSameSize(char* ltemp ,char* rtemp,const BigIntger& lInt,const BigIntger& rInt)
{
	int length=(lInt.m_size>rInt.m_size)?lInt.m_size:rInt.m_size;
	int delt=abs(lInt.m_size-rInt.m_size);//两个数组长度的差
	if(lInt.m_size>rInt.m_size)
	{
		strcpy(ltemp,lInt.m_num);
		for(int i=0;i<delt;i++)
			rtemp[i]='0';
		rtemp[delt]='\0';
		strcat(rtemp,rInt.m_num);
	}
	else if(lInt.m_size<rInt.m_size)
	{
		strcpy(rtemp,rInt.m_num);
		for(int i=0;i<delt;i++)
			ltemp[i]='0';
		ltemp[delt]='\0';
		strcat(ltemp,lInt.m_num);
	}
	else 
	{
		strcpy(rtemp,rInt.m_num);
		strcpy(ltemp,lInt.m_num);
	}
}

BigIntger BigIntger::mutiInt(int n)
{
	BigIntger a;
	if(n==0) return BigIntger("0");
	else if(n==1) a=*this; 
	else a=mutiInt(n-1)+(*this);
	return a;
}

BigIntger BigIntger::shiftInt(int n)
{
	if(n==0) return *this;
	char* newnum=new char[m_size+n+1];
	strcpy(newnum,m_num);
	for(int i=m_size;i<m_size+n;i++)
		newnum[i]='0';
	newnum[m_size+n]='\0';
	return BigIntger(newnum);
}

ostream& operator<<(ostream& out,const BigIntger& bint)
{
	for(int i=0;i<bint.m_size;i++)
		 out<<bint.m_num[i];
	return out;
}

BigIntger& BigIntger::operator=(const BigIntger& big)
{
	if(this==&big)
	{
	return *this;
	}
	if(m_num!=0)
	{
	delete []m_num;
	}
	if(big.m_num==NULL)
	{
		m_num=0;
		m_size=0;
		status=0;
	}	
	else
		{
		m_size=big.m_size;
		status=big.status;
		m_num=new char[m_size+1];
		strcpy(m_num,big.m_num);
		}	
}


BigIntger operator+(const BigIntger& lInt,const BigIntger& rInt)
{
	int length=(lInt.m_size>rInt.m_size)?lInt.m_size:rInt.m_size;
	char *ltemp=new char[length+1];
	char *rtemp=new char[length+1];
	makeSameSize(ltemp,rtemp,lInt,rInt);
	char *sum=new char[length+1];
	sum[length]='\0';
	char *result=new char[length+2];
	int carry=0;//记录进位
	int res=0;//记录整形结果
	for(int i=length-1;i>=0;i--)
	{
		int lnum=ltemp[i]-'0';
		int rnum=rtemp[i]-'0';
		if(lnum+rnum+carry<10)
		{
		res=lnum+rnum+carry;
		carry=0;
		}
		else if(lnum+rnum+carry>9)
		{
		res=lnum+rnum+carry-10;
		carry=1;
		}
		sum[i]=res+'0';
	}
	if(carry==1)
	{
	result[0]='1';
	result[1]='\0';
	strcat(result,sum);
	result[length+1]='\0';
	}
	else {strcpy(result,sum); result[length]='\0';}
	return BigIntger(result);
}

void removeZero(char* dest,char* res,int n)//n表示res的大小
{
	int i=0;
	int j=0;
	int k;
	for(i;res[i]=='0';i++);
	for(j,k=i;k<n;k++,j++)
		dest[j]=res[k];
	for(j;j<n+1;j++)
		dest[j]='\0';
}

BigIntger operator-(const BigIntger& lInt,const BigIntger& rInt)
{
	int length=(lInt.m_size>rInt.m_size)?lInt.m_size:rInt.m_size;
	char *ltemp=new char[length+1];
	char *rtemp=new char[length+1];
	makeSameSize(ltemp,rtemp,lInt,rInt);
	int status=0;//结果的符号，0为正1为负
	char *result=new char[length+1];
	char *subtract=new char[length+1];
	result[length]='\0';
	subtract[length]='\0';
	int res=0;
	int borrow=0;//0表示无借位，1表示有借位
	if(strcmp(ltemp,rtemp)==0)return BigIntger("0");
	else if(strcmp(ltemp,rtemp)>0)//如果左值大于右值结果为正
		{
			status=0;
			for(int i=length-1;i>=0;i--)
			{
				int lnum=ltemp[i]-'0';
				    lnum=lnum-borrow;//减去前一位借去的1
				int rnum=rtemp[i]-'0';
				if(lnum>=rnum)
				{
					res=lnum-rnum;
					borrow=0;
				}
				else if(lnum<rnum)
				{
					res=lnum+10-rnum;
					borrow=1;
				}
				subtract[i]=res+'0';
			}
			if(res==0)
			{
			removeZero(result,subtract,length);
			}
			else strcpy(result,subtract);
		}
	else if(strcmp(ltemp,rtemp)<0)//如果左值小于右值结果为负
		{
			status=1;
			for(int i=length-1;i>=0;i--)
			{
				int rnum=rtemp[i]-'0';
				     rnum=rnum-borrow;  //减去前一位借去的1
				int lnum=ltemp[i]-'0';
				if(rnum>=lnum)
				{
					res=rnum-lnum;
					borrow=0;
				}
				else if(rnum<lnum)
				{
					res=rnum+10-lnum;
					borrow=1;
				}
				subtract[i]=res+'0';
			}
			//防止高位为0，去掉结果的0
			if(res==0)
			{
			removeZero(result,subtract,length);
			}
			else strcpy(result,subtract);
		}
	return BigIntger(result,status);

}

BigIntger operator*(const BigIntger& lInt,const BigIntger& rInt)
{
	BigIntger big1=lInt;
	BigIntger big2=rInt;
	BigIntger result("0");
	BigIntger temp("0");
	if(strcmp(lInt.m_num,"0")==0||strcmp(rInt.m_num,"0")==0)
		return result;
	if(strcmp(lInt.m_num,"1")==0) return big2;
	if(strcmp(rInt.m_num,"1")==0) return big1;
	for(int i=big2.m_size-1;i>=0;i--)
	{
		int bit=big2.m_num[i]-'0';
		temp=big1.mutiInt(bit);
		result=result+temp.shiftInt(big2.m_size-1-i);
	}
    return result;
}

BigIntger operator/(const BigIntger& lInt,const BigIntger& rInt)
{
	assert(strcmp(rInt.m_num,"0")!=0);
	if(lInt.m_size<rInt.m_size||strcmp(lInt.m_num,"0")==0) return BigIntger("0");
	if(&lInt==&rInt) return BigIntger("1");
	if(lInt.m_size==rInt.m_size)
	{
		if(strcmp(lInt.m_num,rInt.m_num)==0)
			return BigIntger("1");
	}
	if(strcmp(rInt.m_num,"1")==0) 
		return BigIntger(lInt);
	int m=lInt.m_size;
	int n=rInt.m_size;
	int bit=m-n+1;//结果的位数
	char* result=new char[bit+1];
	BigIntger divdend=lInt;//被除数
	BigIntger divdor=rInt;//除数
	BigIntger subtracter;//减数
	BigIntger remainder=lInt;//余数
	BigIntger temp;
	for(int i=0;i<bit;i++)
	{	
		int res=0;
		subtracter=divdor.shiftInt(bit-i-1);
		temp=remainder-subtracter;
		while(temp.status==0)
		{
			res++;
			remainder=temp;
			temp=temp-subtracter;
		}
		//remainder=remainder+subtracter;
		//res--;
		result[i]=res+'0';
	}
	result[bit]='\0';
	return BigIntger(result);
}



int main()
{
 string s1,s2;
 char sig;
 cin>>s1;
 cin>>sig;
 cin>>s2;
 
 BigIntger a(s1);
 BigIntger b(s2);
 switch(sig)
 {
 case '+':
	 cout<<a+b;
	 break;
 case '-':
	 cout<<a-b;
	 break;
 case '*':
	 cout<<a*b;
	 break;
 case '/':
	 cout<<a/b;
     break;
 default:
	 break;
 }
}
