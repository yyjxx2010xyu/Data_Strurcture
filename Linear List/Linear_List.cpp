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
	ElemType data;	//数据域
	LNode* next;	//指针域
public:
	friend class OrderList;
	friend class LinkList;
};

class LinkList {
protected:
	LNode* head;		//头指针
public:
	LinkList();			//构造函数，替代InitList
	~LinkList();		//析构函数，替代DestroyList
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

/* 析构函数（删除线性表） */
LinkList::~LinkList()
{
	LNode* q, * p = head;

	/* 从头结点开始依次释放（含头结点） */
	while (p) {    //若链表为空，则循环不执行
		q = p->next; //抓住链表的下一个结点
		delete p;
		p = q;
	}

	head = NULL;	//头指针置NULL
}

/* 清除线性表（保留头结点） */
Status LinkList::ClearList()
{
	LNode* q, * p = head->next;

	/* 从首元结点开始依次释放 */
	while (p) {
		q = p->next;   //抓住链表的下一个结点
		delete p;
		p = q;
	}

	head->next = NULL; //头结点的next域置NULL
	return OK;
}

/* 判断是否为空表 */
Status LinkList::ListEmpty()
{
	/* 判断头结点的next域即可 */
	if (head->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/* 求表的长度 */
int LinkList::ListLength()
{
	LNode* p = head->next; //指向首元结点
	int    len = 0;

	/* 循环整个链表，进行计数 */
	while (p) {
		p = p->next;
		len++;
	}

	return len;
}

/* 取表中元素 */
Status LinkList::GetElem(int i, ElemType& e)
{
	LNode* p = head->next;	//指向首元结点
	int  pos = 1;		//初始位置为1

	/* 链表不为NULL 且 未到第i个元素 */
	while (p != NULL && pos < i) {
		p = p->next;
		pos++;
	}

	if (!p || pos > i)
		return ERROR;

	e = p->data;
	return OK;
}

/* 查找符合指定条件的元素 */
int LinkList::LocateElem(ElemType e, Status(*compare)(ElemType e1, ElemType e2))
{
	LNode* p = head->next;	//指向首元结点
	int  pos = 1;		//初始位置为1

	/* 循环整个链表  */
	while (p && (*compare)(e, p->data) == FALSE) {
		p = p->next;
		pos++;
	}

	return p ? pos : 0;
}

/* 查找符合指定条件的元素的前驱元素 */
Status LinkList::PriorElem(ElemType cur_e, ElemType& pre_e)
{
#if 1
	LNode* p = head->next;	//指向首元结点

	if (p == NULL)	//空表直接返回
		return ERROR;

	/* 从第2个结点开始循环整个链表(如果比较相等，保证有前驱) */
	while (p->next && p->next->data != cur_e)
		p = p->next;

	if (p->next == NULL) //未找到
		return ERROR;

	pre_e = p->data;
	return OK;
#else
	LNode* p = head; //指向头结点

	/* 循环整个链表并比较值是否相等 */
	while (p->next && p->next->data != cur_e)
		p = p->next;

	if (p->next == NULL || p == head) //未找到或首元结点或空表
		return ERROR;

	pre_e = p->data;
	return OK;
#endif
}

/* 查找符合指定条件的元素的后继元素 */
Status LinkList::NextElem(ElemType cur_e, ElemType& next_e)
{
	LNode* p = head->next;  //首元结点

	if (p == NULL)	//空表直接返回
		return ERROR;

	/* 有后继结点且当前结点值不等时继续 */
	while (p->next && p->data != cur_e)
		p = p->next;

	if (p->next == NULL)
		return ERROR;

	next_e = p->next->data;
	return OK;
}

/* 在指定位置前插入一个新元素 */
Status LinkList::ListInsert(int i, ElemType e)
{
	LNode* s, * p = head;	//p指向头结点
	int   pos = 0;

	/* 寻找第i-1个结点 */
	while (p && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p == NULL || pos > i - 1)  //i值非法则返回
		return ERROR;

	//执行到此表示找到指定位置，p指向第i-1个结点

	s = new LNode;
	if (s == NULL)
		return OVERFLOW;

	s->data = e; 	//新结点数据域赋值
	s->next = p->next;	//新结点的next是第i个
	p->next = s;	//第i-1个的next是新结点

	return OK;
}

/* 删除指定位置的元素，并将被删除元素的值放入e中返回 */
Status LinkList::ListDelete(int i, ElemType& e)
{
	LNode* q, * p = head;	//p指向头结点
	int   pos = 0;

	/* 寻找第i个结点（p->next是第i个结点） */
	while (p->next && pos < i - 1) {
		p = p->next;
		pos++;
	}

	if (p->next == NULL || pos > i - 1)	//i值非法则返回
		return ERROR;

	//执行到此表示找到了第i个结点，此时p指向第i-1个结点

	q = p->next;       //q指向第i个结点
	p->next = q->next; //第i-1个结点的next域指向第i+1个

	e = q->data;      //取第i个结点的值
	delete q;           //释放第i个结点

	return OK;
}

/* 遍历线性表 */
Status LinkList::ListTraverse(Status(*visit)(ElemType e))
{
	LNode* p = head->next;
	while (p && (*visit)(p->data) == TRUE)
		p = p->next;

	if (p)
		return ERROR;

	cout << endl;	//最后打印一个换行，只是为了好看，与算法无关
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




/* 用于比较两个值是否相等的具体函数，与LocateElem中的函数指针定义相同，调用时代入
   int LocateElem(sqlist L, ElemType e, Status (*compare)(ElemType e1, ElemType e2)) */
Status MyCompare(ElemType e1, ElemType e2)
{
	if (e1 == e2)
		return TRUE;
	else
		return FALSE;
}

/* 用于访问某个元素的值的具体函数，与ListTraverse中的函数指针定义相同，调用时代入
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

		cout << "空表=" << ((L.ListEmpty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "表长=" << L.ListLength() << endl;

		cout << "插入" << INSERT_NUM << "个元素：" << endl;
		for (i = INSERT_NUM * 2; i > 0; i -= 2)
			L.ListInsert(1, i);
		L.ListTraverse(MyVisit);//此处传入MyVisit函数名

		cout << "空表=" << ((L.ListEmpty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "表长=" << L.ListLength() << endl;

		/* 分别取第1、最后、以及小于第1、大于最后的4种情况下的的元素值、前驱值、后继值
		   最后再加一个任意输入值 */
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
				cout << "请输入要取元素的位序[1.." << L.ListLength() << "]：" << endl;
				cin >> pos;
				break;
			}

			if (L.GetElem(pos, e1) == OK) {
				cout << "第" << pos << "个元素=" << e1 << endl;

				/* 只有取得了某个元素，才能取前驱和后继 */
				if (L.PriorElem(e1, e2) == OK)
					cout << "第" << pos << "个元素(" << e1 << ")的前驱=" << e2 << endl;
				else
					cout << "无法取得第" << pos << "个元素(" << e1 << ")的前驱" << endl;

				if (L.NextElem(e1, e2) == OK)
					cout << "第" << pos << "个元素(" << e1 << ")的后继=" << e2 << endl << endl;
				else
					cout << "无法取得第" << pos << "个元素(" << e1 << ")的后继" << endl << endl;
			}
			else
				cout << "无法取得第" << pos << "个元素" << endl << endl;
		} // end of for

		cout << "请输入要查找的元素：" << endl;
		cin >> e1;
		if ((pos = L.LocateElem(e1, MyCompare)) > 0)
			cout << "元素" << e1 << "的位序=" << pos << endl;
		else
			cout << "找不到元素" << e1 << endl;

		cout << endl << "请输入要插入元素的值：" << endl;
		cin >> e1;
		cout << "请输入要插入元素的位序：" << endl;
		cin >> pos;
		if (L.ListInsert(pos, e1) == OK) {
			cout << "在" << pos << "前插入元素" << e1 << "成功" << endl;
			cout << "新表为：" << endl;
			L.ListTraverse(MyVisit);
		}
		else
			cout << "在" << pos << "前插入元素" << e1 << "失败" << endl;

		cout << endl << "请输入要删除元素的位序：" << endl;
		cin >> pos;
		if (L.ListDelete(pos, e1) == OK) {
			cout << "删除第" << pos << "元素=" << e1 << "成功" << endl;
			cout << "新表为：" << endl;
			L.ListTraverse(MyVisit);
		}
		else
			cout << "删除第" << pos << "元素=" << e1 << "失败" << endl;

		cout << endl << "清空表：" << endl;
		L.ClearList();
		cout << "空表=" << ((L.ListEmpty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "表长=" << L.ListLength() << endl;
	}

	// Linear List 逆置测试
	if (0)
	{
		cout << "开始链表逆置测试" << endl;
		OrderList L;
		int n1, e;
		cout << "请输入元素个数n1" << endl;
		cin >> n1;
		cout << "请输入的n1个数字" << endl;
		for (int i = 1; i <= n1; i++)
		{
			cin >> e;
			L.ListInsert(i,e);
		}
		cout << "翻转整个表" << endl;
		L.List_Reverse();
		L.ListTraverse(MyVisit);
	}


	// Linear List 去重测试
	if (0)
	{
		cout << "开始链表去重测试" << endl;
		OrderList L;
		int n1, e;
		cout << "请输入元素个数n1" << endl;
		cin >> n1;
		cout << "请输入的n1个数字" << endl;
		for (int i = 1; i <= n1; i++)
		{
			cin >> e;
			L.ListInsert(i, e);
		}
		cout << "去重后" << endl;
		L.List_Remove_Dup();
		L.ListTraverse(MyVisit);
	}
	// Order_List 测试
	if (1)
	{
		cout << "开始有序链表测试" << endl;
		OrderList L1, L2, L3;
		int n1, n2, num,e;
		cout << "请输入第一个有序表的长度L1" << endl;
		cin >> n1;
		cout << "请输入递增的n个数字" << endl;
		for (int i = 1; i <= n1; i++)
		{
			cin >> e;
			L1.Inc_Insert(e);
		}
		cout << "请输入第一个有序表的长度L2" << endl;
		cin >> n2;
		cout << "请输入递增的n个数字" << endl;
		for (int i = 1; i <= n2; i++)
		{
			cin >> e;
			L2.Inc_Insert(e);
		}

		L3.Merge_List(L1, L2);
		cout << endl << "L1和L2的合并结果为" << endl;
		L3.ListTraverse(MyVisit);

		cout << endl << "请输入要插入的数字" << endl;
		cin >> num;
		L3.Inc_Insert(num);
		cout << endl << "新表为";
		L3.ListTraverse(MyVisit);
	}

	// 基本操作 用于T1
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

	// 逆置 用于T2
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

	// 去重 用于T3
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

	//合并 用于T4
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

