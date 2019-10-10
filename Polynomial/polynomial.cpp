#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
using namespace std;

const int TRUE = 1;
const int FALSE = 0;
const int OK = 1;
const int ERROR = 0;
const int INFEASIBLE = -1;
const int OVER_FLOW = -2;
const double EPS = 1e-5;
typedef int Status;

class Polynomail;


class Item
{
protected:
	double Coef;
	int Exp;
public:
	Item(double _Coef = 0.0, int _Exp = 0) :Coef(_Coef), Exp(_Exp) {}
	friend bool operator < (const Item& I1, const Item& I2) { return I1.Exp < I2.Exp; }
	friend bool operator > (const Item& I1, const Item& I2) { return I1.Exp > I2.Exp; }
	friend bool operator == (const Item& I1, const Item& I2) { return I1.Exp == I2.Exp; }


	friend class Polynomail;
	friend Polynomail operator + (const Polynomail& P1, const Polynomail& P2);
	friend Polynomail operator * (const Polynomail& P1, const Polynomail& P2);
};


class LNode
{
protected:
	Item data;
	LNode* next;
public:
	LNode(Item _data = Item(), LNode* _next = NULL) :data(_data), next(_next) {}

	friend class Polynomail;
	friend Polynomail operator + (const Polynomail& P1, const Polynomail& P2);
	friend Polynomail operator * (const Polynomail& P1, const Polynomail& P2);
};



class Polynomail
{
protected:
	LNode* head;
public:
	Polynomail();
	~Polynomail();
	Polynomail(const Polynomail& P);
	Polynomail& operator=(const Polynomail& P);
	Status Insert_Item(Item);
	friend Polynomail operator + (const Polynomail& P1, const Polynomail& P2);
	friend Polynomail operator * (const Polynomail& P1, const Polynomail& P2);
	void Travel_Item();
	double Get_Value(double x);
};

Polynomail::Polynomail()
{
	head = new(nothrow) LNode;
	if (head == NULL)
		exit(OVERFLOW);
	head->next = NULL;
}

Polynomail::~Polynomail()
{
	LNode* q, * p = head;
	while (p)
	{
		q = p->next;
		delete p;
		p = q;
	}
	head = NULL;
}

Polynomail::Polynomail(const Polynomail& P)
{
	head = new LNode;
	LNode* l= head, * p = P.head;
	while (p != NULL)
	{
		if (p->next)
			l->next = new LNode;
		else
			break;
		l = l->next;
		p = p->next;
		l->data = p->data;		
	}
}

Status Polynomail::Insert_Item(Item e)
{
	LNode* p = this->head;
	while (p->next != NULL && p->next->data < e)
		p = p->next;
	
	if (p->next!=NULL && p->next->data.Exp == e.Exp)
	{
		p->next->data.Coef += e.Coef;
		return OK;
	}
	
	LNode* t = new LNode;
	if (t == NULL)
		return ERROR;
	t->next = NULL;

	t->data = e;
	t->next = p->next;
	p->next = t;
	return OK;
}

void Polynomail::Travel_Item()
{
	LNode* l = head->next;
	while (l != NULL)
	{
		if (fabs(l->data.Coef)>EPS)
			cout << l->data.Coef << " " << l->data.Exp << endl;
		l = l->next;
	}
}

double Polynomail::Get_Value(double x)
{
	double Ret = 0.0;
	LNode* l = head->next;
	while (l != NULL)
	{
		Ret += pow(x, l->data.Exp) * l->data.Coef;
		l = l->next;
	}
	return Ret;
}

Polynomail operator+(const Polynomail& P1, const Polynomail& P2)
{
	LNode* p1 = P1.head->next, * p2 = P2.head->next;
	Polynomail P;

	while (p1 != NULL && p2 != NULL)
	{
		if (p1->data < p2->data)
		{
			P.Insert_Item(p1->data);
			p1 = p1->next;
			continue;
		}
		if (p1->data > p2->data)
		{
			P.Insert_Item(p2->data);
			p2 = p2->next;
			continue;
		}
		if (p1->data == p2->data)
		{
			P.Insert_Item(Item(p1->data.Coef + p2->data.Coef, p1->data.Exp));
			p1 = p1->next;
			p2 = p2->next;
			continue;
		}
	}
	while (p1 != NULL)
	{
		P.Insert_Item(p1->data);
		p1 = p1->next;

	}
	while (p2 != NULL)
	{
		P.Insert_Item(p2->data);
		p2 = p2->next;
	}
	return P;
}

Polynomail operator*(const Polynomail& P1, const Polynomail& P2)
{
	LNode* p1 = P1.head->next, * p2 = P2.head->next;
	Polynomail P;

	while (p1 != NULL)
	{
		p2 = P2.head->next;
		while (p2 != NULL)
		{
			Item I = Item(p1->data.Coef * p2->data.Coef, p1->data.Exp + p2->data.Exp);
			P.Insert_Item(I);
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	return P;
}

int main()
{
	// 多项式相加 用于T1
	if (0)
	{
		//freopen("1.txt", "r", stdin);
		int m, n;
		double Coef;
		int Exp;

		Polynomail P1, P2;
		cin >> m;
		for (int i = 1; i <= m; i++)
		{
			cin >> Coef >> Exp;
			P1.Insert_Item(Item(Coef, Exp));
		}
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> Coef >> Exp;
			P2.Insert_Item(Item(Coef, Exp));
		}
		Polynomail P(P1 + P2);
		P.Travel_Item();
		//fclose(stdin);
	}

	// 多项式相乘 用于T2
	if (0)
	{
		//freopen("1.txt", "r", stdin);
		int m, n;
		double Coef;
		int Exp;

		Polynomail P1, P2;
		cin >> m;
		for (int i = 1; i <= m; i++)
		{
			cin >> Coef >> Exp;
			P1.Insert_Item(Item(Coef, Exp));
		}
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> Coef >> Exp;
			P2.Insert_Item(Item(Coef, Exp));
		}
		Polynomail P(P1 * P2);
		P.Travel_Item();
		//fclose(stdin);
	}

	// 一元多项式相乘
	if (1)
	{
		int n;
		double Coef;
		int Exp;
		Polynomail P;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> Coef >> Exp;
			P.Insert_Item(Item(Coef, Exp));
		}
		double x;
		cin >> x;
		cout << fixed << setprecision(1) << P.Get_Value(x) << endl;

	}
	return 0;
}