#include <iostream>
#include <vector>
namespace HEAP
{
	typedef int Rank;
	template <typename T, typename Cmp = std::greater<T> >
	class Heap
	{
	protected:
		std::vector<T> _V;
		int _size;
		Cmp Compare;
		Rank Push_Up(Rank);
		Rank Push_Down(Rank);
		void Heapify();
		Rank Parent(Rank u) { return (u - 1) >> 1; }
		Rank LChild(Rank u) { return (u << 1) + 1; }
		Rank RChild(Rank u) { return (u << 1) + 2; }
		Rank LChildValid(Rank u) { return LChild(u) < _size; }
		Rank RChildValid(Rank u) { return RChild(u) < _size; }
		bool ParentValid(Rank u) { return u > 0; }
		Rank Last_Interal() { return Parent(_size - 1); }
		Rank Proper_Parent(Rank u)
		{
			if (LChildValid(u) && !RChildValid(u) && !Compare(_V[LChild(u)], _V[u]))
				return LChild(u);
			if (RChildValid(u) && !LChildValid(u) && !Compare(_V[RChild(u)], _V[u]))
				return RChild(u);
			if (LChildValid(u) && RChildValid(u) && (!Compare(_V[LChild(u)], _V[u]) || !Compare(_V[RChild(u)], _V[u])))
				return Compare(_V[LChild(u)], _V[RChild(u)]) ? RChild(u) : LChild(u);
			return u;
		}
	public:
		Heap();
		Heap(std::vector<T> V, int size);
		void Insert(T v);
		T Get_Top();
		T Del_Top();
	};
	template<typename T, typename Cmp>
	Rank Heap<T, Cmp>::Push_Up(Rank u)
	{
		while (ParentValid(u))
		{
			Rank v = Parent(u);
			if (Compare(_V[u], _V[v]))
				break;
			std::swap(_V[u], _V[v]);
			u = v;
		}
		return u;
	}
	template<typename T, typename Cmp>
	Rank Heap<T, Cmp>::Push_Down(Rank u)
	{
		Rank v;
		while (u != Proper_Parent(u))
		{
			v = Proper_Parent(u);
			std::swap(_V[u], _V[v]);
			u = v;
		}
		return u;
	}
	template<typename T, typename Cmp>
	void Heap<T, Cmp>::Heapify()
	{
		for (int i = Last_Interal(); i >= 0 && i < _size; i--)
			Push_Down(i);
	}
	template<typename T, typename Cmp>
	Heap<T, Cmp>::Heap()
	{
		_V.clear();
		_size = 0;
	}
	template<typename T, typename Cmp>
	Heap<T, Cmp>::Heap(std::vector<T> V, int size)
	{
		_size = size;
		_V.clear();
		for (int i = 0; i < size; i++)
			_V.push_back(V[i]);
		Heapify();
	}
	template<typename T, typename Cmp>
	void Heap<T, Cmp>::Insert(T v)
	{
		_V.push_back(v);
		_size++;
		Push_Up(_size - 1);
	}
	template<typename T, typename Cmp>
	T Heap<T, Cmp>::Get_Top()
	{
		return _V[0];
	}
	template<typename T, typename Cmp>
	T Heap<T, Cmp>::Del_Top()
	{
		T Ret = _V[0];
		_V[0] = _V.back();
		_V.pop_back();
		_size--;
		Push_Down(0);
		return Ret;
	}
};
int main()
{
	int n, k, e;
	HEAP::Heap<int, std::less<int> > H;
	std::cin >> n >> k;
	for (int i = 1; i <= n; i++)
	{
		std::cin >> e;
		H.Insert(e);
	}
	for (int i = 1; i <= k; i++)
		std::cout << H.Del_Top() << " ";
	std::cout << std::endl;
	return 0;
}