﻿#include <iostream>
using namespace std;

typedef int Rank;						// 秩
#define ListNodePosi(T) ListNode<T> *	// 列表节点位置

template <typename T>
struct ListNode
{
	T data;
	ListNodePosi(T) pred;
	ListNodePosi(T) succ;
	ListNode() 
	{
		
		pred = succ = NULL;
	}
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL) :data(e), pred(p), succ(s) {}
	ListNodePosi(T)	insertAsPred(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);
};

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, pred, this);
	pred->succ = x;
	this->pred = x;
	return x;
}

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
	ListNodePosi(T) x = new ListNode(e, this, succ);
	succ->pred = x;
	this->succ = x;
	return x;
}


template<typename T>
class List
{
private:
	int _size;
	ListNodePosi(T) header;
	ListNodePosi(T) trailer;
protected:
	void init();
	int clear();
	void copyNodes(ListNodePosi(T), int);
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);
	// void mergeSort(ListNodePosi(T)&, int);
	// void selectionSort(ListNodePosi(T), int);
	// void insertionSort(ListNodePosi(T), int);
public:
	//构造函数
	List() { init(); }
	List(List<T> const& L);
	List(List<T> const& L, Rank r, int n);
	List(ListNodePosi(T) p, int n);
	//析构函数
	~List();
	//只读访问接口
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[] (Rank r) const;
	ListNodePosi(T) first() const { return header->succ; }
	ListNodePosi(T) last() const { return trailer->pred; }
	bool valid(ListNodePosi(T) p) { return p && (trailer != p) && (header != p); }
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;
	ListNodePosi(T) search(T const& e) const { return search(e, _size, trailer); }
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); }
	//可写读访问接口
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);
	void merge(List<T>& L) { merge(first(), size, L, L.first, L._size); }
	// void sort(ListNodePosi(T) p, int n);
	void sort() { sort(first(), _size); }
	int deduplicate();
	int uniquify();
	void reverse();
	//遍历
	void traverse(void(*) (T&));
	template <typename VST>
	void traverse(VST&);
};

template <typename T>
void List<T>::init()
{
	header = new ListNode <T>;
	trailer = new ListNode <T>;
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	_size = 0;
}


template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{
	init();
	while (n--)
	{
		insertAsLast(p->data);
		p = p->succ;
	}
}


template<typename T>
List<T>::List(ListNodePosi(T) p, int n) 
{
	copyNodes(p, n); 
}

template<typename T>
List<T>::List(List<T> const& L) 
{
	copyNodes(L.first(), L._sizee); 
}

template<typename T>
List<T>::List(List<T> const& L, int r, int n) 
{ 
	copyNodes(L[r], n); 
}

template<typename T>
T List<T>::remove(ListNodePosi(T) p) {
	T e = p->data;
	p->pred->succ = p->succ; p->succ->pred = p->pred;
	delete p; _size--;
	return e;
}

template <typename T> 
List<T>::~List()
{
	clear();
	delete header; delete trailer;
}

template<typename T> int 
List<T>::clear()
{
	int oldSize = _size;
	while (0 < _size) remove(header->succ);
	return oldSize;
}



template <typename T> 
int List<T>::deduplicate() 
{
	if (_size < 2)return 0;
	int oldSize = _size;
	ListNodePosi(T) p = header; Rank r = 0;
	while (trailer != (p = p->succ))
	{
		ListNodePosi(T) q = find(p->data, r, p);
		q ? remove(q) : r++;
	}
	return oldSize - _size;
}


template <typename T> template<typename VST>
void List<T>::traverse(VST& visit)
{
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template <typename T>
int List<T>::uniquify() 
{
	if (_size < 2) return 0;
	int oldSize = _size;
	ListNodePosi(T) p = first();
	ListNodePosi(T) q;
	while (trailer != (q = p->succ))
		if (p->data != q->data)
			p = q;
		else 
			remove(q);
	return oldSize - _size;
}

template<typename T>
void List<T>::reverse()
{
	ListNodePosi(T) p = header;
	ListNodePosi(T) q = trailer;
	for (int i = 1; i < _size; i += 2)
		swap((p = p->succ)->data, (q = q->pred)->data);
}

template<typename T>
void List<T>::traverse(void(*visit)(T&))
{
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const 
{
	while (0 <= n--)
		if (((p = p->pred)->data) <= e) 
			break;
	return p;
}


template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) 
{
	ListNodePosi(T) max = p;
	for (ListNodePosi(T) cur = p; 1 < n; n--)
		if (!lt((cur = cur->succ)->data, max->data))
			max = cur;
	return max;
}

template<typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
	_size++;
	return trailer->insertAsPred(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e)
{
	_size++;
	return p->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p, T const& e)
{
	_size++;
	return p->insertAsPred(e);
}


template<typename T>
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m)
{
	ListNodePosi(T) pp = p->pred;
	while (0 < m)
		if ((0 < n) && (p->data <= q->data))
		{
			if (q == (p = p->succ))
				break; 
			n--;
		}
		else
		{
			insertB(p, L.remove((q = q->succ)->pred));
			m--;
		}
	p = pp->succ;
}

template<typename T>
void myvisit(T& val)
{
	cout << val << " ";
}

int main()
{
	List<int> L;
	int n = 10;
	cout << n << endl;
	for (int i = 1; i <= n; i++)
		L.insertAsLast(i);
	L.traverse(myvisit);
	return 0;
}
