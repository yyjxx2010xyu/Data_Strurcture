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
const double EPS = 1e-6;
const int LIST_INIT_SIZE = 2000000;

typedef int Status;

typedef int ElemType;

class LinkList;

class LNode {
protected:
	ElemType data;	//������
	LNode* next;	//ָ����
public:
	friend class OrderList;
	friend class LinkList;
};

class LinkList {
protected:
	LNode* head;		//ͷָ��
public:
	LinkList();			//���캯�������InitList
	~LinkList();		//�������������DestroyList
	Status	ClearList();
	Status	ListEmpty();
	int	ListLength();
	Status	GetElem(int i, ElemType& e);
	int	LocateElem(ElemType e, Status(*compare)(ElemType e1, ElemType e2));
	Status	PriorElem(ElemType cur_e, ElemType& pre_e);
	Status	NextElem(ElemType cur_e, ElemType& next_e);
	Status	ListInsert(int i, ElemType e);
	Status	ListDelete(int i, ElemType& e);
	Status	ListTraverse(Status(*visit)(ElemType e));
	Status	List_Reverse();
	Status	List_Remove_Dup();

};

class OrderList : public LinkList
{
public:
	Status Inc_Insert(ElemType e);
	Status Merge_List(const OrderList& L1, const OrderList& L2);
};



LinkList::LinkList()
{
	head = new LNode;
	if (head == NULL)
		exit(OVERFLOW);

	head->next = NULL;
}

/* ����������ɾ�����Ա� */
LinkList::~LinkList()
{
	LNode* q, * p = head;

	/* ��ͷ��㿪ʼ�����ͷţ���ͷ��㣩 */
	while (p) {    //������Ϊ�գ���ѭ����ִ��
		q = p->next; //ץס�������һ�����
		delete p;
		p = q;
	}

	head = NULL;	//ͷָ����NULL
}

/* ������Ա�����ͷ��㣩 */
Status LinkList::ClearList()
{
	LNode* q, * p = head->next;

	/* ����Ԫ��㿪ʼ�����ͷ� */
	while (p) {
		q = p->next;   //ץס�������һ�����
		delete p;
		p = q;
	}

	head->next = NULL; //ͷ����next����NULL
	return OK;
}

/* �ж��Ƿ�Ϊ�ձ� */
Status LinkList::ListEmpty()
{
	/* �ж�ͷ����next�򼴿� */
	if (head->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/* ���ĳ��� */
int LinkList::ListLength()
{
	LNode* p = head->next; //ָ����Ԫ���
	int    len = 0;

	/* ѭ�������������м��� */
	while (p) {
		p = p->next;
		len++;
	}

	return len;
}

/* ȡ����Ԫ�� */
Status LinkList::GetElem(int i, ElemType& e)
{
	LNode* p = head->next;	//ָ����Ԫ���
	int  pos = 1;		//��ʼλ��Ϊ1

	/* ����ΪNULL �� δ����i��Ԫ�� */
	while (p != NULL && pos < i) {
		p = p->next;
		pos++;
	}

	if (!p || pos > i)
		return ERROR;

	e = p->data;
	return OK;
}

/* ���ҷ���ָ��������Ԫ�� */
int LinkList::LocateElem(ElemType e, Status(*compare)(ElemType e1, ElemType e2))
{
	LNode* p = head->next;	//ָ����Ԫ���
	int  pos = 1;		//��ʼλ��Ϊ1

	/* ѭ����������  */
	while (p && (*compare)(e, p->data) == FALSE) {
		p = p->next;
		pos++;
	}

	return p ? pos : 0;
}

/* ���ҷ���ָ��������Ԫ�ص�ǰ��Ԫ�� */
Status LinkList::PriorElem(ElemType cur_e, ElemType& pre_e)
{
#if 1
	LNode* p = head->next;	//ָ����Ԫ���

	if (p == NULL)	//�ձ�ֱ�ӷ���
		return ERROR;

	/* �ӵ�2����㿪ʼѭ����������(����Ƚ���ȣ���֤��ǰ��) */
	while (p->next && p->next->data != cur_e)
		p = p->next;

	if (p->next == NULL) //δ�ҵ�
		return ERROR;

	pre_e = p->data;
	return OK;
#else
	LNode* p = head; //ָ��ͷ���

	/* ѭ�����������Ƚ�ֵ�Ƿ���� */
	while (p->next && p->next->data != cur_e)
		p = p->next;

	if (p->next == NULL || p == head) //δ�ҵ�����Ԫ����ձ�
		return ERROR;

	pre_e = p->data;
	return OK;
#endif
}

/* ���ҷ���ָ��������Ԫ�صĺ��Ԫ�� */
Status LinkList::NextElem(ElemType cur_e, ElemType& next_e)
{
	LNode* p = head->next;  //��Ԫ���

	if (p == NULL)	//�ձ�ֱ�ӷ���
		return ERROR;

	/* �к�̽���ҵ�ǰ���ֵ����ʱ���� */
	while (p->next && p->data != cur_e)
		p = p->next;

	if (p->next == NULL)
		return ERROR;

	next_e = p->next->data;
	return OK;
}

/* ��ָ��λ��ǰ����һ����Ԫ�� */
Status LinkList::ListInsert(int i, ElemType e)
{
	LNode* s, * p = head;	//pָ��ͷ���
	int   pos = 0;

	/* Ѱ�ҵ�i-1����� */
	while (p && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p == NULL || pos > i - 1)  //iֵ�Ƿ��򷵻�
		return ERROR;

	//ִ�е��˱�ʾ�ҵ�ָ��λ�ã�pָ���i-1�����

	s = new LNode;
	if (s == NULL)
		return OVERFLOW;

	s->data = e; 	//�½��������ֵ
	s->next = p->next;	//�½���next�ǵ�i��
	p->next = s;	//��i-1����next���½��

	return OK;
}

/* ɾ��ָ��λ�õ�Ԫ�أ�������ɾ��Ԫ�ص�ֵ����e�з��� */
Status LinkList::ListDelete(int i, ElemType& e)
{
	LNode* q, * p = head;	//pָ��ͷ���
	int   pos = 0;

	/* Ѱ�ҵ�i����㣨p->next�ǵ�i����㣩 */
	while (p->next && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p->next == NULL || pos > i - 1)	//iֵ�Ƿ��򷵻�
		return ERROR;

	//ִ�е��˱�ʾ�ҵ��˵�i����㣬��ʱpָ���i-1�����

	q = p->next;       //qָ���i�����
	p->next = q->next; //��i-1������next��ָ���i+1��

	e = q->data;      //ȡ��i������ֵ
	delete q;           //�ͷŵ�i�����

	return OK;
}

/* �������Ա� */
Status LinkList::ListTraverse(Status(*visit)(ElemType e))
{
	LNode* p = head->next;
	while (p && (*visit)(p->data) == TRUE)
		p = p->next;

	if (p)
		return ERROR;

	cout << endl;	//����ӡһ�����У�ֻ��Ϊ�˺ÿ������㷨�޹�
	return OK;
}

Status LinkList::List_Reverse()
{
	LNode* p, * q, * r;

	p = head->next;
	q = head->next->next;
	head->next->next = NULL;

	while (q != NULL)
	{
		r = q->next;
		q->next = p;
		p = q;
		q = r;
	}
	head->next = p;
	return OK;
}

Status LinkList::List_Remove_Dup()
{
	LNode* r = head;
	LNode* p = head->next;
	while (p)
	{
		bool flag = false;
		for (LNode* q = head->next; q != p; q = q->next)
			if (p->data == q->data)
			{
				flag = true;
				break;
			}
		if (flag)
			r->next = p->next;
		else
			r = r->next;
		p = p->next;
	}
	return OK;
}




/* ���ڱȽ�����ֵ�Ƿ���ȵľ��庯������LocateElem�еĺ���ָ�붨����ͬ������ʱ����
   int LocateElem(sqlist L, ElemType e, Status (*compare)(ElemType e1, ElemType e2)) */
Status MyCompare(ElemType e1, ElemType e2)
{
	if (e1 == e2)
		return TRUE;
	else
		return FALSE;
}

/* ���ڷ���ĳ��Ԫ�ص�ֵ�ľ��庯������ListTraverse�еĺ���ָ�붨����ͬ������ʱ����
   Status ListTraverse(sqlist L, Status (*visit)(ElemType e)) */
Status MyVisit(ElemType e)
{
	cout << setw(3) << e << "->";
	return OK;
}

Status Normal_Visit(ElemType e)
{
	cout << e << " ";
	return OK;
}

Status Three_Base(ElemType e)
{
	if (e % 3 == 0)
		cout << e << " ";
	return OK;
}
int main()
{
	// Test Link List
	if (0)
	{
		LinkList L;
		ElemType e1, e2;
		int      i, pos;

		const int INSERT_NUM = 10;

		cout << "�ձ�=" << ((L.ListEmpty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "��=" << L.ListLength() << endl;

		cout << "����" << INSERT_NUM << "��Ԫ�أ�" << endl;
		for (i = INSERT_NUM * 2; i > 0; i -= 2)
			L.ListInsert(1, i);
		L.ListTraverse(MyVisit);//�˴�����MyVisit������

		cout << "�ձ�=" << ((L.ListEmpty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "��=" << L.ListLength() << endl;

		/* �ֱ�ȡ��1������Լ�С�ڵ�1����������4������µĵ�Ԫ��ֵ��ǰ��ֵ�����ֵ
		   ����ټ�һ����������ֵ */
		for (i = 0; i < 5; i++) {
			int pos;
			switch (i) {
			case 0:
				pos = 1;
				break;
			case 1:
				pos = L.ListLength();
				break;
			case 2:
				pos = -1;
				break;
			case 3:
				pos = L.ListLength() + 1;
				break;
			case 4:
				cout << "������ҪȡԪ�ص�λ��[1.." << L.ListLength() << "]��" << endl;
				cin >> pos;
				break;
			}

			if (L.GetElem(pos, e1) == OK) {
				cout << "��" << pos << "��Ԫ��=" << e1 << endl;

				/* ֻ��ȡ����ĳ��Ԫ�أ�����ȡǰ���ͺ�� */
				if (L.PriorElem(e1, e2) == OK)
					cout << "��" << pos << "��Ԫ��(" << e1 << ")��ǰ��=" << e2 << endl;
				else
					cout << "�޷�ȡ�õ�" << pos << "��Ԫ��(" << e1 << ")��ǰ��" << endl;

				if (L.NextElem(e1, e2) == OK)
					cout << "��" << pos << "��Ԫ��(" << e1 << ")�ĺ��=" << e2 << endl << endl;
				else
					cout << "�޷�ȡ�õ�" << pos << "��Ԫ��(" << e1 << ")�ĺ��" << endl << endl;
			}
			else
				cout << "�޷�ȡ�õ�" << pos << "��Ԫ��" << endl << endl;
		} // end of for

		cout << "������Ҫ���ҵ�Ԫ�أ�" << endl;
		cin >> e1;
		if ((pos = L.LocateElem(e1, MyCompare)) > 0)
			cout << "Ԫ��" << e1 << "��λ��=" << pos << endl;
		else
			cout << "�Ҳ���Ԫ��" << e1 << endl;

		cout << endl << "������Ҫ����Ԫ�ص�ֵ��" << endl;
		cin >> e1;
		cout << "������Ҫ����Ԫ�ص�λ��" << endl;
		cin >> pos;
		if (L.ListInsert(pos, e1) == OK) {
			cout << "��" << pos << "ǰ����Ԫ��" << e1 << "�ɹ�" << endl;
			cout << "�±�Ϊ��" << endl;
			L.ListTraverse(MyVisit);
		}
		else
			cout << "��" << pos << "ǰ����Ԫ��" << e1 << "ʧ��" << endl;

		cout << endl << "������Ҫɾ��Ԫ�ص�λ��" << endl;
		cin >> pos;
		if (L.ListDelete(pos, e1) == OK) {
			cout << "ɾ����" << pos << "Ԫ��=" << e1 << "�ɹ�" << endl;
			cout << "�±�Ϊ��" << endl;
			L.ListTraverse(MyVisit);
		}
		else
			cout << "ɾ����" << pos << "Ԫ��=" << e1 << "ʧ��" << endl;

		cout << endl << "��ձ�" << endl;
		L.ClearList();
		cout << "�ձ�=" << ((L.ListEmpty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "��=" << L.ListLength() << endl;
	}

	// Linear List ���ò���
	if (0)
	{
		cout << "��ʼ�������ò���" << endl;
		OrderList L;
		int n1, e;
		cout << "������Ԫ�ظ���n1" << endl;
		cin >> n1;
		cout << "�������n1������" << endl;
		for (int i = 1; i <= n1; i++)
		{
			cin >> e;
			L.ListInsert(i,e);
		}
		cout << "��ת������" << endl;
		L.List_Reverse();
		L.ListTraverse(MyVisit);
	}


	// Linear List ȥ�ز���
	if (0)
	{
		cout << "��ʼ����ȥ�ز���" << endl;
		OrderList L;
		int n1, e;
		cout << "������Ԫ�ظ���n1" << endl;
		cin >> n1;
		cout << "�������n1������" << endl;
		for (int i = 1; i <= n1; i++)
		{
			cin >> e;
			L.ListInsert(i, e);
		}
		cout << "ȥ�غ�" << endl;
		L.List_Remove_Dup();
		L.ListTraverse(MyVisit);
	}
	// Order_List ����
	if (1)
	{
		cout << "��ʼ�����������" << endl;
		OrderList L1, L2, L3;
		int n1, n2, num,e;
		cout << "�������һ�������ĳ���L1" << endl;
		cin >> n1;
		cout << "�����������n������" << endl;
		for (int i = 1; i <= n1; i++)
		{
			cin >> e;
			L1.Inc_Insert(e);
		}
		cout << "�������һ�������ĳ���L2" << endl;
		cin >> n2;
		cout << "�����������n������" << endl;
		for (int i = 1; i <= n2; i++)
		{
			cin >> e;
			L2.Inc_Insert(e);
		}

		L3.Merge_List(L1, L2);
		cout << endl << "L1��L2�ĺϲ����Ϊ" << endl;
		L3.ListTraverse(MyVisit);

		cout << endl << "������Ҫ���������" << endl;
		cin >> num;
		L3.Inc_Insert(num);
		cout << endl << "�±�Ϊ";
		L3.ListTraverse(MyVisit);
	}

	// �������� ����T1
	if (0)
	{
		//freopen("c.in", "r", stdin);
		LinkList L;
		int n, e, pos;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> e;
			L.ListInsert(1, e);
		}
		L.ListTraverse(Normal_Visit);
		cin >> pos >> e;
		if (L.ListInsert(pos, e) == OK)
			L.ListTraverse(Normal_Visit);
		else
			cout << "-1" << endl;

		cin >> pos;
		if (L.ListDelete(pos, e) == OK)
			L.ListTraverse(Normal_Visit);
		else
			cout << "-1" << endl;

		cin >> e;
		if (L.LocateElem(e, MyCompare))
			cout << L.LocateElem(e, MyCompare) << endl;
		else
			cout << "-1" << endl;
		cout << L.ListLength() << endl;
		//fclose(stdin);
	}

	// ���� ����T2
	if (0)
	{
		LinkList L;
		int n, e;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> e;
			L.ListInsert(1, e);
		}
		L.ListTraverse(Normal_Visit);
		L.List_Reverse();
		L.ListTraverse(Three_Base);
	}

	// ȥ�� ����T3
	if (0)
	{
		LinkList L;
		int n, e;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> e;
			L.ListInsert(i, e);
		}
		L.List_Remove_Dup();
		L.ListTraverse(Normal_Visit);
	}

	//�ϲ� ����T4
	if (0)
	{
		OrderList L1, L2, L;
		int e;
		while (true)
		{
			cin >> e;
			if (e == 0)
				break;
			L1.Inc_Insert(e);
		}

		while (true)
		{
			cin >> e;
			if (e == 0)
				break;
			L2.Inc_Insert(e);
		}
		L.Merge_List(L1, L2);
		L.ListTraverse(Normal_Visit);
	}

	return 0;
}



Status OrderList::Inc_Insert(ElemType e)
{
	LNode* p = this->head;
	while (p->next != NULL && p->next->data < e)
		p = p->next;
	LNode* t = new LNode;
	if (t == NULL)
		return ERROR;
	t->next = NULL;

	t->data = e;
	t->next = p->next;
	p->next = t;
	return OK;
}

Status OrderList::Merge_List(const OrderList& L1, const OrderList& L2)
{

	LNode* p1 = L1.head->next, * p2 = L2.head->next;
	LNode* l = this->head;

	while (p1 != NULL && p2 != NULL)
	{
		if (p1->data <= p2->data)
		{
			l->next = new LNode;
			l->next->next = NULL;
			l->next->data = p1->data;
			l = l->next;
			p1 = p1->next;
		}
		else
		{
			l->next = new LNode;
			l->next->next = NULL;
			l->next->data = p2->data;
			l = l->next;
			p2 = p2->next;
		}
	}
	while (p1 != NULL)
	{
		l->next = new LNode;
		l->next->next = NULL;
		l->next->data = p1->data;
		l = l->next;
		p1 = p1->next;

	}
	while (p2 != NULL)
	{
		l->next = new LNode;
		l->next->next = NULL;
		l->next->data = p2->data;
		l = l->next;
		p2 = p2->next;
	}
	return OK;
}

