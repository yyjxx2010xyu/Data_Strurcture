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
	char no[8];    //8 λѧ��
	char name[20]; // ����
	double score;  // �ɼ�
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

// ���Ա�
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
	//���������Ϊ��T1ר��д��
	Status Get_Stu_Info_From_Name(Student& S);
};

// �����
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

// ���밴�����˳��n������
template <class ElemType>
Status Order_List<ElemType>::Order_Read_Init(int n)
{
	for (int i = 0; i < n; i++)
		cin >> this->Elem[i];
	this->Length = n;
	return OK;
}

// �ϲ�����˳���
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

// ��������в���Ԫ�� ��������Զ��֣��������ˣ�Ҳ����д��
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

// �ڵݼ���������в���Ԫ��
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

// ���캯�� ���Ա�����
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

// �������� ���Ա�����
template <class ElemType>
Link_List<ElemType>::~Link_List()
{
	//��δִ��InitList��ִ�б�����������Ϊָ��δ��ʼ��
	if (Elem == nullptr)
		delete[] Elem;
	Length = 0;
	Size = 0;
}

//������Ա��ѳ�ʼ�������ͷſռ䣬ֻ������ݣ�
template <class ElemType>
Status Link_List<ElemType>::Clear_List()
{
	Length = 0;
	return OK;
}

// �ж��Ƿ�Ϊ�ձ�
template <class ElemType>
Status Link_List<ElemType>::List_Empty()
{
	return Length == 0 ? TRUE : FALSE;
}

// �����
template <class ElemType>
int Link_List<ElemType>::List_Length()
{
	return Length;
}

// ȡ����ָ��λ�õ�Ԫ�أ�����e��
template <class ElemType>
Status Link_List<ElemType>::Get_Elem(int p, ElemType* e)
{
	//λ�ò���ȷ
	if (p < 1 || p > Length)
		return ERROR;
	//ʵ���ϴ���p-1����
	*e = Elem[p - 1];
	return OK;
}

// ����Ԫ��e��λ��
template <class ElemType>
int Link_List<ElemType>::Locate_Elem(const ElemType& e)
{
	ElemType* p = Elem;
	while ((p - Elem) < Length && (*p != e))
		p++;
	return (p - Elem) < Length ? (p - Elem) + 1 : 0;
}

// ����ǰ��Ԫ��
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

// ���Һ���Ԫ��
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

// ��ָ��λ�ò���Ԫ��
template <class ElemType>
Status Link_List<ElemType>::List_Insert(int p, const ElemType& e)
{

	//λ�ò���ȷ
	if (p < 1 || p > Length + 1)
		return ERROR;
	if (Length >= Size)
	{
		//����C++û��realloc���Զ�̬˳���Ƚ���д
		cerr << "˳����Ѵﵽ��󳤶�" << endl;
	}
	ElemType* q = &(Elem[p - 1]);
	for (ElemType* i = &Elem[Length - 1]; i >= q; i--)
		* (i + 1) = *i;
	*q = e;
	Length++;
	return OK;
}
// ɾ��ָ��λ��Ԫ��
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

// ��ı��� �����û��Զ������ʾ����
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

// ��ת����Ԫ��
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


// ɾ������Ԫ��e ȥ������
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

// ɾ�������ظ�Ԫ��
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
		// δ�ҵ��ظ�Ԫ��
		if (!flag_same)
			* p++ = *i;
	}
	Length = p - Elem;
	return OK;
}

//���������Ϊ��T1ר��д�ģ�����ElemTypeһ��ΪStudent
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
	// ���ò�����ʹ�õ���������
	typedef int TestType;

	// Link_List����
	if (0)
	{
		Link_List<TestType> L;
		TestType e1, e2;

		cout << "������������" << endl;
		int i, pos;

		cout << "�ձ�=" << ((L.List_Empty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "��=" << L.List_Length() << endl;

		// ���ò���Ԫ�ظ���
		const int INSERT_NUM = 10;
		cout << "����" << INSERT_NUM << "��Ԫ�أ�" << endl;
		for (i = INSERT_NUM * 2; i > 0; i -= 2)
			L.List_Insert(1, i);
		L.List_Travel(MyVisit); //�˴�����MyVisit������

		cout << "�ձ�=" << ((L.List_Empty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "��=" << L.List_Length() << endl;

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
				cout << "������ҪȡԪ�ص�λ��[1.." << L.List_Length() << "]��" << endl;
				cin >> pos;
				break;
			}

			if (L.Get_Elem(pos, &e1) == OK)
			{
				cout << "��" << pos << "��Ԫ��=" << e1 << endl;

				/* ֻ��ȡ����ĳ��Ԫ�أ�����ȡǰ���ͺ�� */
				if (L.Prior_Elem(e1, &e2) == OK)
					cout << "��" << pos << "��Ԫ��(" << e1 << ")��ǰ��=" << e2 << endl;
				else
					cout << "�޷�ȡ�õ�" << pos << "��Ԫ��(" << e1 << ")��ǰ��" << endl;

				if (L.Next_Elem(e1, &e2) == OK)
					cout << "��" << pos << "��Ԫ��(" << e1 << ")�ĺ��=" << e2 << endl
					<< endl;
				else
					cout << "�޷�ȡ�õ�" << pos << "��Ԫ��(" << e1 << ")�ĺ��" << endl
					<< endl;
			}
			else
				cout << "�޷�ȡ�õ�" << pos << "��Ԫ��" << endl
				<< endl;
		}

		cout << "������Ҫ���ҵ�Ԫ�أ�" << endl;
		cin >> e1;
		if ((pos = L.Locate_Elem(e1)) > 0)
			cout << "Ԫ��" << e1 << "��λ��=" << pos << endl;
		else
			cout << "�Ҳ���Ԫ��" << e1 << endl;

		cout << endl
			<< "������Ҫ����Ԫ�ص�ֵ��" << endl;
		cin >> e1;
		cout << "������Ҫ����Ԫ�ص�λ��" << endl;
		cin >> pos;
		if (L.List_Insert(pos, e1) == OK)
		{
			cout << "��" << pos << "ǰ����Ԫ��" << e1 << "�ɹ�" << endl;
			cout << "�±�Ϊ��" << endl;
			L.List_Travel(MyVisit);
		}
		else
			cout << "��" << pos << "ǰ����Ԫ��" << e1 << "ʧ��" << endl;

		cout << endl
			<< "������Ҫɾ��Ԫ�ص�λ��" << endl;
		cin >> pos;
		if (L.List_Delete(pos, &e1) == OK)
		{
			cout << "ɾ����" << pos << "Ԫ��=" << e1 << "�ɹ�" << endl;
			cout << "�±�Ϊ��" << endl;
			L.List_Travel(MyVisit);
		}
		else
			cout << "ɾ����" << pos << "Ԫ��=" << e1 << "ʧ��" << endl;

		cout << "��ת������" << endl;
		L.List_Reverse();
		L.List_Travel(MyVisit);

		cout << endl
			<< "��ձ�" << endl;
		L.Clear_List();
		cout << "�ձ�=" << ((L.List_Empty() == TRUE) ? "TRUE" : "FALSE") << endl;
		cout << "��=" << L.List_Length() << endl;
	}

	// Order_List ����
	if (0)
	{
		cout << "��ʼ��������" << endl;
		Order_List<TestType> L1, L2, L3;
		int n1, n2, num;
		cout << "�������һ�������ĳ���L1" << endl;
		cin >> n1;
		cout << "�����������n������" << endl;
		L1.Order_Read_Init(n1);
		cout << "�������һ�������ĳ���L2" << endl;
		cin >> n2;
		cout << "�����������n������" << endl;
		L2.Order_Read_Init(n2);

		L3.Merge_List(L1, L2);
		cout << endl << "L1��L2�ĺϲ����Ϊ" << endl;
		L3.List_Travel(MyVisit);

		cout << endl << "������Ҫ���������" << endl;
		cin >> num;
		L3.Inc_Insert(num);
		cout << endl << "�±�Ϊ";
		L3.List_Travel(MyVisit);
	}

	// List_Remove_Elem List_Delete_Dup ����
	if (1)
	{
		cout << "��ʼ��������" << endl;
		Link_List<TestType> L;
		int n, num;
		cout << "���������Ա�ĳ���L" << endl;
		cin >> n;
		for (int i = 1; i <= n; i++)
			L.List_Insert(1, i);
		for (int i = 1; i <= n; i++)
			L.List_Insert(1, i);
		for (int i = 1; i <= n; i++)
			L.List_Insert(1, i);
		L.List_Travel(MyVisit);

		cout << endl
			<< "������Ҫɾ��������" << endl;
		cin >> num;
		L.List_Delete_Elem(num);
		cout << "�±�Ϊ" << endl;
		L.List_Travel(MyVisit);
		cout << endl;
		L.List_Remove_Dup();
		cout << "ȥ�غ���±�Ϊ" << endl;
		L.List_Travel(MyVisit);
	}

	// Student ����� ����T1
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

	// ȥ�ز������� ����T2
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

	// ȥ�ز������� ����T3
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
