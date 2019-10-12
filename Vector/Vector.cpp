#include <iostream>
using namespace std;

typedef int Rank;			//ÖÈ
#define DEFAULT_CAPACITY  3 
template <typename T>
class Vector
{
protected:
	Rank _size;
	int _capacity;
	T* _elem;
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	// ÅÅÐòÆ÷ÔÝÊ±Ã»Ð´
	//bool bubble(Rank lo, Rank hi);
	//void bubbleSort(Rank lo, Rank hi);
	//Rank max(Rank lo, Rank hi);
	//void selectionSort(Rank lo, Rank hi);
	//void merge(Rank lo, Rank mi, Rank hi);
	//void mergeSort(Rank lo, Rank hi);
	//Rank partition(Rank lo, Rank hi);
	//void quickSort(Rank lo, Rank hi);
	//void heapSort(Rank lo, Rank hi);
public:
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
	{
		_elem = new T[_capacity = c]; 
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); }
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); }
	~Vector() { delete[] _elem; }
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const;
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e) const
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi) const;
	T& operator[] (Rank r) const;
	Vector<T>& operator= (Vector<T> const&);
	T remove(Rank r);
	int remove(Rank lo, Rank hi);
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }
	//void sort(Rank lo, Rank hi);
	void sort() { sort(0, _size); }
	//void unsort(Rank lo, Rank hi);
	void unsort() { unsort(0, _size); }
	int deduplicate();
	int uniquify();
	void traverse(void (*visit) (T&));
	template <typename VST> void traverse(VST&);
};

template<typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
	_capacity = (hi - lo) << 1;
	_elem = new T[_capacity];
	_size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

template<typename T>
void Vector<T>::expand()
{
	if (_size < _capacity)
		return;
	if (_capacity < DEFAULT_CAPACITY)
		_capacity = DEFAULT_CAPACITY;
	T* old_Elem = _elem;
	_capacity <<= 1;
	_elem = new T[_capacity];
	for (int i = 0; i < _size; i++)
		_elem[i] = old_Elem[i];
	delete[] old_Elem;
}

template<typename T>
void Vector<T>::shrink()
{
	if (_capacity < DEFAULT_CAPACITY << 1) return;
	if (_size << 1 > _capacity)return;
	T* old_elem = _elem;
	_capacity >>= 1;
	_elem = new T[_capacity];
	for (int i = 0; i < _size; i++)
		_elem[i] = old_elem[i];
	delete[] old_elem;
}



template<typename T>
int Vector<T>::disordered() const
{
	int n = 0;
	for (int i = 1; i < _size; i++)
		n += (_elem[i - 1] > _elem[i]);
	return n;
}

template<typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const
{
	while ((lo < hi) && (e != _elem[hi]))
		hi--;
	return hi;
}

template<typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
	return binSearch(_elem, e, lo, hi);
}

template<typename T>
T& Vector<T>::operator[](Rank r) const
{
	return _elem[r];
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)
{
	if (_elem)
		delete[] _elem;
	copy(V._elem, 0, V.size());
	return *this;
}

template<typename T>
T Vector<T>::remove(Rank r)
{
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}

// É¾³ýÇø¼ä[lo,hi)
template<typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
	if (lo == hi) return 0;
	while (hi < _size) _elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}

template<typename T>
Rank Vector<T>::insert(Rank r, T const& e)
{
	expand();
	for (int i = _size; i >= r; i--)
		_elem[i] = _elem[i - 1];
	_size++;
	_elem[r] = e;
	return r;
}

template<typename T>
int Vector<T>::deduplicate()
{
	int old_size = _size;
	Rank i = 1;
	while (i < _size)
		if (find(_elem[i], 0, i) < 0)
			i++;
		else
			delete(i);
	return old_size - _size;
}

template<typename T>
int Vector<T>::uniquify()
{
	int old_size = _size;
	Rank i = 0, j = 0;
	while (++j < _size)
		if (_elem[i] != _elem[j])
			_elem[++i] = _elem[j];
	_size = ++i;
	shrink();
	return old_size - _size;
}

template<typename T>
void Vector<T>::traverse(void(*visit)(T&))
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

template<typename T>
template<typename VST>
void Vector<T>::traverse(VST& visit)
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}


template<typename T>
static Rank binSearch(T* A, T const& e, int lo, int hi)
{
	while (lo < hi)
	{
		int mi = (lo + hi) >> 1;
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	}
	return --lo;
}

template<typename T>
void myvisit(T& val)
{
	cout << val << " ";
}

/*
int main()
{
	Vector<int> V;
	int n = 10;
	cout << n << endl;
	for (int i = 1; i <= n; i++)
		V.insert(i);	
	V.traverse(myvisit);
	return 0;
}
*/