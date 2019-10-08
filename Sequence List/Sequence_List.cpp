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

class Student
{
public:
	char no[8];    //8 位学号
	char name[20]; // 姓名
	double score;  // 成绩
	friend istream& operator>>(istream& in, Student& S);
	friend ostream& operator<<(ostream& out, const Student& S);
	friend bool operator==(const Student& S1, const Student& S2);
};

istream& operator>>(istream& in, Student& S)
{
	in >> S.no >> S.name >> S.score;
	return in;
}

ostream& operator<<(ostream& out, const Student& S)
{
	out << S.no << " " << S.name << " " << S.score;
	return out;
}

bool operator==(const Student& S1, const Student& S2)
{
	if (fabs(S1.score - S2.score) > EPS)
		return false;
	if (strcmp(S1.no, S2.no) != 0)
		return false;
	if (strcmp(S1.name, S2.name) != 0)
		return false;
	return true;
}

// 线性表
template <class ElemType>
class Link_List
{
protected:
	ElemType* Elem;
	int Length;
	int Size;

public:
	Link_List();
	~Link_List();
	Status Clear_List();
	Status List_Empty();
	int List_Length();
	Status Get_Elem(int p, ElemType* e);
	int Locate_Elem(const ElemType& e);
	int Prior_Elem(const ElemType& cur_e, ElemType* pri_e);
	int Next_Elem(const ElemType& cur_e, ElemType* next_e);
	Status List_Insert(int p, const ElemType& e);
	Status List_Delete(int p, ElemType* e);
	Status List_Reverse();
	Status List_Delete_Elem(const ElemType& e);
	Status List_Remove_Dup();
	Status List_Travel(Status(*visit)(ElemType e));
	//这个函数是为了T1专门写的
	Status Get_Stu_Info_From_Name(Student& S);
};

// 有序表
template <class ElemType>
class Order_List : public Link_List<ElemType>
{
public:
	Status Order_Read_Init(int n);
	Status Merge_List(Order_List L1, Order_List L2);
	Status Inc_Insert(ElemType e);
	Status Dec_Insert(ElemType e);
};

// ================Link_List====================

// 读入按有序表顺序n个数组
template <class ElemType>
Status Order_List<ElemType>::Order_Read_Init(int n)
{
	for (int i = 0; i < n; i++)
		cin >> this->Elem[i];
	this->Length = n;
	return OK;
}

// 合并两个顺序表
template <class ElemType>
Status Order_List<ElemType>::Merge_List(Order_List L1, Order_List L2)
{
	this->Length = L1.Length + L2.Length;
	ElemType* p1 = L1.Elem, * p2 = L2.Elem, * l = this->Elem;
	while (p1 - L1.Elem < L1.Length && p2 - L2.Elem < L2.Length)
	{
		if (*p1 <= *p2)
			* l++ = *p1++;
		else
			*l++ = *p2++;
	}
	while (p1 - L1.Elem < L1.Length)
		* l++ = *p1++;
	while (p2 - L2.Elem < L2.Length)
		* l++ = *p2++;
	return OK;
}

// 在有序表中插入元素 讲道理可以二分，但我忘了，也懒得写。
template <class ElemType>
Status Order_List<ElemType>::Inc_Insert(ElemType e)
{
	ElemType* p = this->Elem;
	while (p - this->Elem < this->Length && *p < e)
		p++;
	for (ElemType* i = &this->Elem[this->Length - 1]; p < i; i--)
		* (i + 1) = *i;
	*p = e;
	this->Length++;
	return OK;
}

// 在递减的有序表中插入元素
template <class ElemType>
Status Order_List<ElemType>::Dec_Insert(ElemType e)
{
	ElemType* p = this->Elem;
	while (p - this->Elem<this->Length && *p> e)
		p++;
	for (ElemType* i = &this->Elem[this->Length - 1]; p < i; i--)
		* (i + 1) = *i;
	*p = e;
	this->Length++;
	return OK;
}

// ================Link_List====================

// 构造函数 线性表生成
template <class ElemType>
Link_List<ElemType>::Link_List()
{
	try
	{
		Elem = new ElemType[LIST_INIT_SIZE];
	}
	catch (bad_alloc)
	{
		exit(OVER_FLOW);
	}
	Length = 0;
	Size = LIST_INIT_SIZE;
}

// 析构函数 线性表销毁
template <class ElemType>
Link_List<ElemType>::~Link_List()
{
	//若未执行InitList，执行本程序会出错，因为指针未初始化
	if (Elem == nullptr)
		delete[] Elem;
	Length = 0;
	Size = 0;
}

//清除线性表（已初始化，不释放空间，只清除内容）
template <class ElemType>
Status Link_List<ElemType>::Clear_List()
{
	Length = 0;
	return OK;
}

// 判断是否为空表
template <class ElemType>
Status Link_List<ElemType>::List_Empty()
{
	return Length == 0 ? TRUE : FALSE;
}

// 求表长度
template <class ElemType>
int Link_List<ElemType>::List_Length()
{
	return Length;
}

// 取表中指定位置的元素，存入e中
template <class ElemType>
Status Link_List<ElemType>::Get_Elem(int p, ElemType* e)
{
	//位置不正确
	if (p < 1 || p > Length)
		return ERROR;
	//实际上存在p-1里面
	*e = Elem[p - 1];
	return OK;
}

// 查找元素e的位置
template <class ElemType>
int Link_List<ElemType>::Locate_Elem(const ElemType& e)
{
	ElemType* p = Elem;
	while ((p - Elem) < Length && (*p != e))
		p++;
	return (p - Elem) < Length ? (p - Elem) + 1 : 0;
}

// 查找前驱元素
template <class ElemType>
Status Link_List<ElemType>::Prior_Elem(const ElemType& cur_e, ElemType* pre_e)
{
	ElemType* p = Elem;
	while (*p != cur_e)
		p++;
	if ((p - Elem) == 0 || (p - Elem) >= Length)
		return ERROR;
	*pre_e = *--p;
	return OK;
}

// 查找后驱元素
template <class ElemType>
Status Link_List<ElemType>::Next_Elem(const ElemType& cur_e, ElemType* next_e)
{
	ElemType* p = Elem;
	while (*p != cur_e)
		p++;
	if ((p - Elem) >= (int)Length - 1)
		return ERROR;
	*next_e = *++p;
	return OK;
}

// 在指定位置插入元素
template <class ElemType>
Status Link_List<ElemType>::List_Insert(int p, const ElemType& e)
{

	//位置不正确
	if (p < 1 || p > Length + 1)
		return ERROR;
	if (Length >= Size)
	{
		//由于C++没有realloc所以动态顺序表比较难写
		cerr << "顺序表已达到最大长度" << endl;
	}
	ElemType* q = &(Elem[p - 1]);
	for (ElemType* i = &Elem[Length - 1]; i >= q; i--)
		* (i + 1) = *i;
	*q = e;
	Length++;
	return OK;
}
// 删除指定位置元素
template <class ElemType>
Status Link_List<ElemType>::List_Delete(int p, ElemType* e)
{
	if (p < 1 || p > Length)
		return ERROR;

	*e = Elem[p - 1];
	ElemType* q = &(Elem[Length - 1]);
	for (ElemType* i = &Elem[p]; i <= q; i++)
		* (i - 1) = *i;
	Length--;
	return OK;
}

// 表的遍历 传入用户自定义的显示函数
template <class ElemType>
Status Link_List<ElemType>::List_Travel(Status(*visit)(ElemType e))
{
	ElemType* p = Elem;
	while ((p - Elem) < Length && (*visit)(*p) == TRUE)
		p++;
	//cout << "Travel END" << endl;
	cout << endl;
	return OK;
}

// 翻转表内元素
template <class ElemType>
Status Link_List<ElemType>::List_Reverse()
{
	ElemType* p = Elem, * q = &Elem[Length - 1], r;
	for (; p < q; p++, q--)
	{
		r = *p;
		*p = *q;
		*q = r;
	}
	return OK;
}


// 删除表内元素e 去除所有
template <class ElemType>
Status Link_List<ElemType>::List_Delete_Elem(const ElemType& e)
{
	ElemType* p = Elem;
	bool exist_flag = false;
	for (ElemType* i = Elem; i - Elem < Length; i++)
		if (*i != e)
			* p++ = *i;
		else
			exist_flag = true;
	Length = p - Elem;
	if (exist_flag)
		return TRUE;
	else
		return FALSE;
}

// 删除表内重复元素
template <class ElemType>
Status Link_List<ElemType>::List_Remove_Dup()
{
	ElemType* p = Elem;
	for (ElemType* i = Elem; i - Elem < Length; i++)
	{
		bool flag_same = false;
		for (ElemType* j = Elem; j < p; j++)
			if (*i == *j)
			{
				flag_same = true;
				break;
			}
		// 未找到重复元素
		if (!flag_same)
			* p++ = *i;
	}
	Length = p - Elem;
	return OK;
}

//这个函数是为了T1专门写的，所以ElemType一定为Student
template <class ElemType>
Status Link_List<ElemType>::Get_Stu_Info_From_Name(Student& S)
{
	Student* p = (Student*)Elem;
	while (p - Elem < Length)
	{
		if (strcmp(S.name, p->name) == 0)
		{
			strcpy(S.no, p->no);
			S.score = p->score;
			return TRUE;
		}
		p++;
	}
	return FALSE;
}
//===================================================
template <class ElemType>
Status MyVisit(ElemType e)
{
	cout << setw(3) << e << "->";
	return OK;
}

template <class ElemType>
Status Normal_Visit(ElemType e)
{
	cout << e << " ";
	return OK;
}

int main()
{
	// 设置测试所使用的数据类型
	typedef int TestType;

	// Link_List测试
	if (0)
	{
		Link_List<TestType> L;
		TestType e1, e2;

		cout << "线性有序表测试" << endl;
		int i, pos;

		cout << "空表=" << ((L.List_Empty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "表长=" << L.List_Length() << endl;

		// 设置插入元素个数
		const int INSERT_NUM = 10;
		cout << "插入" << INSERT_NUM << "个元素：" << endl;
		for (i = INSERT_NUM * 2; i > 0; i -= 2)
			L.List_Insert(1, i);
		L.List_Travel(MyVisit); //此处传入MyVisit函数名

		cout << "空表=" << ((L.List_Empty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "表长=" << L.List_Length() << endl;

		for (int i = 0; i < 5; i++)
		{
			int pos;
			switch (i)
			{
			case 0:
				pos = 1;
				break;
			case 1:
				pos = L.List_Length();
				break;
			case 2:
				pos = -1;
				break;
			case 3:
				pos = L.List_Length() + 1;
				break;
			case 4:
				cout << "请输入要取元素的位序[1.." << L.List_Length() << "]：" << endl;
				cin >> pos;
				break;
			}

			if (L.Get_Elem(pos, &e1) == OK)
			{
				cout << "第" << pos << "个元素=" << e1 << endl;

				/* 只有取得了某个元素，才能取前驱和后继 */
				if (L.Prior_Elem(e1, &e2) == OK)
					cout << "第" << pos << "个元素(" << e1 << ")的前驱=" << e2 << endl;
				else
					cout << "无法取得第" << pos << "个元素(" << e1 << ")的前驱" << endl;

				if (L.Next_Elem(e1, &e2) == OK)
					cout << "第" << pos << "个元素(" << e1 << ")的后继=" << e2 << endl
					<< endl;
				else
					cout << "无法取得第" << pos << "个元素(" << e1 << ")的后继" << endl
					<< endl;
			}
			else
				cout << "无法取得第" << pos << "个元素" << endl
				<< endl;
		}

		cout << "请输入要查找的元素：" << endl;
		cin >> e1;
		if ((pos = L.Locate_Elem(e1)) > 0)
			cout << "元素" << e1 << "的位序=" << pos << endl;
		else
			cout << "找不到元素" << e1 << endl;

		cout << endl
			<< "请输入要插入元素的值：" << endl;
		cin >> e1;
		cout << "请输入要插入元素的位序：" << endl;
		cin >> pos;
		if (L.List_Insert(pos, e1) == OK)
		{
			cout << "在" << pos << "前插入元素" << e1 << "成功" << endl;
			cout << "新表为：" << endl;
			L.List_Travel(MyVisit);
		}
		else
			cout << "在" << pos << "前插入元素" << e1 << "失败" << endl;

		cout << endl
			<< "请输入要删除元素的位序：" << endl;
		cin >> pos;
		if (L.List_Delete(pos, &e1) == OK)
		{
			cout << "删除第" << pos << "元素=" << e1 << "成功" << endl;
			cout << "新表为：" << endl;
			L.List_Travel(MyVisit);
		}
		else
			cout << "删除第" << pos << "元素=" << e1 << "失败" << endl;

		cout << "翻转整个表" << endl;
		L.List_Reverse();
		L.List_Travel(MyVisit);

		cout << endl
			<< "清空表：" << endl;
		L.Clear_List();
		cout << "空表=" << ((L.List_Empty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "表长=" << L.List_Length() << endl;
	}

	// Order_List 测试
	if (0)
	{
		cout << "开始有序表测试" << endl;
		Order_List<TestType> L1, L2, L3;
		int n1, n2, num;
		cout << "请输入第一个有序表的长度L1" << endl;
		cin >> n1;
		cout << "请输入递增的n个数字" << endl;
		L1.Order_Read_Init(n1);
		cout << "请输入第一个有序表的长度L2" << endl;
		cin >> n2;
		cout << "请输入递增的n个数字" << endl;
		L2.Order_Read_Init(n2);

		L3.Merge_List(L1, L2);
		cout << endl << "L1和L2的合并结果为" << endl;
		L3.List_Travel(MyVisit);

		cout << endl << "请输入要插入的数字" << endl;
		cin >> num;
		L3.Inc_Insert(num);
		cout << endl << "新表为";
		L3.List_Travel(MyVisit);
	}

	// List_Remove_Elem List_Delete_Dup 测试
	if (1)
	{
		cout << "开始有序表测试" << endl;
		Link_List<TestType> L;
		int n, num;
		cout << "请输入线性表的长度L" << endl;
		cin >> n;
		for (int i = 1; i <= n; i++)
			L.List_Insert(1, i);
		for (int i = 1; i <= n; i++)
			L.List_Insert(1, i);
		for (int i = 1; i <= n; i++)
			L.List_Insert(1, i);
		L.List_Travel(MyVisit);

		cout << endl
			<< "请输入要删除的数字" << endl;
		cin >> num;
		L.List_Delete_Elem(num);
		cout << "新表为" << endl;
		L.List_Travel(MyVisit);
		cout << endl;
		L.List_Remove_Dup();
		cout << "去重后的新表为" << endl;
		L.List_Travel(MyVisit);
	}

	// Student 类测试 用于T1
	if (0)
	{
		//freopen("c.in", "r", stdin);
		Link_List<Student> L;
		Student S;
		int n, pos;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> S;
			L.List_Insert(i, S);
		}

		cin >> pos >> S;
		if (L.List_Insert(pos, S) == OK)
			cout << "0" << endl;
		else
			cout << "-1" << endl;

		cin >> pos;
		if (L.List_Delete(pos, &S) == OK)
			cout << "0" << endl;
		else
			cout << "-1" << endl;

		cin >> S.name;
		if (L.Get_Stu_Info_From_Name(S) == OK)
			cout << S.no << " " << S.score << endl;
		else
			cout << "-1" << endl;
		cout << L.List_Length() << endl;
		//fclose(stdin);
	}

	// 去重操作测试 用于T2
	if (0)
	{
		Link_List<int> L;
		int n, e;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> e;
			L.List_Insert(i, e);
		}
		cin >> e;
		if (L.List_Delete_Elem(e) == TRUE)
			L.List_Travel(Normal_Visit);
		else
			cout << "-1" << endl;
	}

	// 去重操作测试 用于T3
	if (0)
	{
		Link_List<int> L;
		int n, e;
		cin >> n;
		for (int i = 1; i <= n; i++)
		{
			cin >> e;
			L.List_Insert(i, e);
		}

		L.List_Remove_Dup();
		L.List_Travel(Normal_Visit);
	}
	//system("pause");
	return 0;
}
