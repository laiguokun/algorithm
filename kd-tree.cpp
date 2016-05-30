#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "temperature.h"
using namespace std;
int n;
struct Point
{
	int x,y;
	long long temp;
};
Point point[50050];
struct Node
{
	long long sum;
	long long sum_temp;
	int x1,y1,x2,y2;
	Node* lc;
	Node* rc;
};
int cmpx(Point x,Point y)
{
	return (x.x<y.x);
}
int cmpy(Point x,Point y)
{
	return (x.y<y.y);
}
void sort(int l,int r,int (*cmp)(Point,Point))
{
	int i=l,j=r;
	Point x=point[rand()%(r-l+1)+l];
	while (i<=j) 
	{
		while ((*cmp)(point[i],x)) i++;
		while ((*cmp)(x,point[j])) j--;
		if (i<=j) 
		{
			Point t=point[i];
			point[i]=point[j];
			point[j]=t;			
			i++;j--;
		}
	}
	if (i<r) sort(i,r,cmp);
	if (l<j) sort(l,j,cmp);
}
void buildtree(Node* x,int l,int r,int mark)
{
	if (l==r)
	{
		x->sum=1;
		x->sum_temp=point[l].temp;
//		cout<<l<<" "<<point[l].temp<<x->sum_temp<<endl;
		x->x1=point[l].x;x->y1=point[l].y;
		x->x2=point[l].x;x->y2=point[l].y;
		return ;
	}
	if (mark) 
	{
		sort(l,r,cmpy);
		x->y1=point[l].y;x->y2=point[r].y;
		sort(l,r,cmpx);
		x->x1=point[l].x;x->x2=point[r].x;
	}
	else 
	{
		sort(l,r,cmpx);
		x->x1=point[l].x;x->x2=point[r].x;
		sort(l,r,cmpy);
		x->y1=point[l].y;x->y2=point[r].y;
	}
	x->lc=new Node;x->rc=new Node;
	int mid=(l+r)/2;
	buildtree(x->lc,l,mid,mark^1);
	buildtree(x->rc,mid+1,r,mark^1);
	x->sum=x->lc->sum+x->rc->sum;
	x->sum_temp=x->lc->sum_temp+x->rc->sum_temp;
//	cout<<l<<" "<<r<<" "<<x->sum<<" "<<x->sum_temp<<" "<<x->x1<<" "<<x->y1<<" "<<x->x2<<" "<<x->y2<<endl;
}
bool contain(Node* x,int x1,int y1,int x2,int y2)
{
	if (x->x1<x1 || x->x1>x2 || x->y1<y1 || x->y1>y2)
		return false;
	if (x->x2<x1 || x->x2>x2 || x->y2<y1 || x->y2>y2)
		return false;
	return true;
}
bool outside(Node* x,int x1,int y1,int x2,int y2)
{
	if (x->x1>x2 || x->x2<x1 || x->y1>y2 || x->y2<y1)
		return true;
	return false;
}
void ask(Node* x,int x1,int y1,int x2,int y2,long long &sum_temp,long long &sum)
{
//	cout<<x->x1<<" "<<x->y1<<" "<<x->x2<<" "<<x->y2<<endl;
	if (contain(x,x1,y1,x2,y2))
	{
//		cout<<x->x1<<" "<<x->y1<<" "<<x->x2<<" "<<x->y2<<" "<<x->sum_temp<<endl;
		sum_temp+=x->sum_temp;
		sum+=x->sum;
		return;
	}
	if (outside(x,x1,y1,x2,y2))
		return;
	ask(x->lc,x1,y1,x2,y2,sum_temp,sum);
	ask(x->rc,x1,y1,x2,y2,sum_temp,sum);
}	
int main()
{
	n=GetNumOfStation();
	for (int i=0;i<n;i++)
	{
		int temp;
		GetStationInfo(i,&(point[i].x),&(point[i].y),&temp);
		point[i].temp=temp;
	}
	Node* root=new Node;
//	cout<<n<<endl;
	buildtree(root,0,n-1,1);
//	cout<<2<<endl;
	int x1,y1,x2,y2;
	while (GetQuery(&x1,&y1,&x2,&y2))
	{
		long long sum_temp=0;
		long long sum=0;
		ask(root,x1,y1,x2,y2,sum_temp,sum);
//		cout<<sum_temp<<endl;
//		cout<<sum<<endl;
		if (sum==0) Response(0);
		else Response(sum_temp/sum);
	}
}
