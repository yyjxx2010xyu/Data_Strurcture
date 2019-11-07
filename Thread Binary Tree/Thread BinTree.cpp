#include <iostream>
#include <iomanip>
using namespace std;

enum class Tag { Link = 0, Thread };


template<typename T>
class ThreadNode;
template<typename T>
using pThreadNode = ThreadNode<T>*;
template<typename T>
class ThreadTree;
template<typename T>
using pThreadTree = ThreadTree<T>*;


template<typename T>
class ThreadNode
{
public:
	T data;
	enum Tag ltag, rtag;
	pThreadNode<T> lchild, rchild;
	ThreadNode(const T& item) :data(item), lchild(NULL), rchild(NULL), ltag(Tag::Link), rtag(Tag::Link) {}
};

template<typename T>
class ThreadTree
{
public:
	ThreadTree() :root(NULL) {}
	ThreadTree(T value) :RefValue(value), root(NULL) {}
	void CreateTree() { CreateTree(root); }
	void CreateInThread()
	{
		pThreadNode<T> pre = NULL;
		if (root != NULL)
		{
			createInThread(root, pre);
			pre->rchild = NULL;
			pre->rtag = Tag::Thread;
		}
	}

	void CreatePreThread()
	{
		pThreadNode<T> pre = NULL;
		if (root != NULL)
		{
			createPreThread(root, pre);
			pre->rchild = NULL;
			pre->rtag = Tag::Thread;
		}

	}
	void InOrder() { InOrder(root); }
	void PreOrder() { PreOrder(root); }
	void PostOrder() { PostOrder(root); }

	void CreateTree(pThreadNode<T>& subTree)
	{
		T item;
		if (cin >> item)
		{
			if (item != RefValue)
			{
				subTree = new ThreadNode<T>(item);
				if (subTree == NULL)
					exit(1);
				CreateTree(subTree->lchild);
				CreateTree(subTree->rchild);
			}
			else
				subTree == NULL;
		}
	}

	void createInThread(pThreadNode<T> Cur, pThreadNode<T>& Pre)
	{
		if (Cur == NULL)
			return;
		createInThread(Cur->lchild, Pre);
		if (Cur->lchild == NULL)
		{
			Cur->lchild = Pre;
			Cur->ltag = Tag::Thread;
		}
		if (Pre != NULL && Pre->rchild == NULL)
		{
			Pre->rchild = Cur;
			Pre->rtag = Tag::Thread;
		}
		Pre = Cur;
		createInThread(Cur->rchild, Pre);
	}

	void createPreThread(pThreadNode<T> Cur, pThreadNode<T>& Pre)
	{
		if (Cur == NULL)
			return;
		if (Cur->lchild == NULL)
		{
			Cur->lchild = Pre;
			Cur->ltag = Tag::Thread;
		}
		if (Pre != NULL && Pre->rchild == NULL)
		{
			Pre->rchild = Cur;
			Pre->rtag = Tag::Thread;
		}
		Pre = Cur;
		if (Cur->ltag == Tag::Link)
			createPreThread(Cur->lchild, Pre);
		if (Cur->rtag == Tag::Link)
			createPreThread(Cur->rchild, Pre);
	}

	pThreadNode<T> First(pThreadNode<T> Cur)
	{
		if (Cur == NULL)
			return NULL;
		pThreadNode<T> p = Cur;
		while (p->ltag == Tag::Link)
			p = p->lchild;
		return p;
	}

	pThreadNode<T> Next(pThreadNode<T> Cur)
	{
		if (Cur == NULL)
			return NULL;
		pThreadNode<T> p = Cur->rchild;
		if (Cur->rtag == Tag::Link)
			return First(p);
		else
			return p;
	}

	pThreadNode<T> Last(pThreadNode<T> Cur)
	{
		if (Cur == NULL)
			return NULL;
		pThreadNode<T> p = Cur;
		while (p->rtag == Tag::Link)
			p = p->rchild;
		return p;
	}

	pThreadNode<T> Prior(pThreadNode<T> Cur)
	{
		if (Cur == NULL)
			return NULL;
		pThreadNode<T> p = Cur->lchild;
		if (Cur->ltag == Tag::Link)
			return Last(p);
		else
			return p;
	}


	pThreadNode<T> Parent(pThreadNode<T> t)
	{
		pThreadNode<T> p;
		if (t == root)
			return NULL;

		for (p = t; p->ltag == Tag::Link; p = p->lchild);
		if (p->lchild != NULL)

			for (p = p->lchild; p != NULL && p->lchild != t && p->rchild != t; p = p->rchild);


		if (p == NULL || p->lchild == NULL)
		{
			for (p = t; p->rtag == Tag::Link; p = p->rchild);
			for (p = p->rchild; p != NULL && p->lchild != t && p->rchild != t; p = p->lchild);
		}
		return p;
	}


	void InOrder(pThreadNode<T> p)
	{
		for (p = First(root); p != NULL; p = Next(p))
			cout << p->data << "";
		cout << endl;
	}

	pThreadNode<T> Find_Node(char val)
	{
		pThreadNode<T> p = root;
		for (p = First(root); p != NULL; p = Next(p))
			if (p->data == val)
				return p;
		return NULL;
	}

	void PreOrder(pThreadNode<T> p)
	{
		cout << p->data;
		while (p->rchild != NULL) {
			if (p->ltag == Tag::Link)
				p = p->lchild;
			else
				p = p->rchild;
			cout << p->data;
		}
	}
	/*void PreOrder(pThreadNode<T> p)
	{
		while (p != NULL)
		{
			cout << p->data << " ";
			if (p->ltag == Tag::Link)
				p = p->lchild;
			else if (p->rtag == Tag::Link)
				p = p->rchild;
			else
			{
				while (p != NULL && p->rtag == Tag::Thread)
					p = p->rchild;
				if (p != NULL)
					p = p->rchild;
			}
		}
		cout << endl;
	}*/

	void PostOrder(pThreadNode<T> p)
	{
		pThreadNode<T> t = p;
		while (t->ltag == Tag::Link || t->rtag == Tag::Link)
		{
			if (t->ltag == Tag::Link)
				t = t->lchild;
			else if (t->rtag == Tag::Link)
				t = t->rchild;
		}
		cout << t->data << " ";
		while ((p = Parent(t)) != NULL)
		{

			if (p->rchild == t || p->rtag == Tag::Thread)
				t = p;
			else
			{
				t = p->rchild;
				while (t->ltag == Tag::Link || t->rtag == Tag::Link)
				{
					if (t->ltag == Tag::Link)
						t = t->lchild;
					else if (t->rtag == Tag::Link)
						t = t->rchild;
				}
			}
			cout << t->data << " ";
		}
	}

	void _Print(pThreadNode<T> Cur, int depth)
	{
		const int PRINT_GAP = 5;
		if (Cur->rtag==Tag::Link && Cur->rchild)
			_Print(Cur->rchild, depth + 1);
		cout << setw((long long)PRINT_GAP * depth) << "";
		cout << (Cur->data) << (int)Cur->ltag << (int)Cur->rtag << endl;

		if (Cur->ltag==Tag::Link && Cur->lchild)
			_Print(Cur->lchild, depth + 1);
	}
	void Print()
	{
		_Print(root, 0);
	}
private:
	pThreadNode<T> root;
	T RefValue;
};

int main()
{
	// T1
	if (0)
	{

		ThreadTree<char> tree('#');
		tree.CreateTree();
		tree.CreatePreThread();
		tree.Print();
		tree.PreOrder();
	}


	// T2
	if (1)
	{
		ThreadTree<char> tree('#');
		tree.CreateTree();
		tree.CreateInThread();
		tree.InOrder();
		string Str;
		cin >> Str;
		pThreadNode<char> p = tree.Find_Node(Str[0]);
		if (p==NULL)
			cout << "Not found" << endl;
		else
		{

			cout << "succ is ";
			if (tree.Next(p) == NULL)
				cout << "NULL" << endl;
			else
				cout << tree.Next(p)->data << (int)tree.Next(p)->rtag << endl;
			cout << "prev is ";
			if (tree.Prior(p) == NULL)
				cout << "NULL" << endl;
			else
				cout << tree.Prior(p)->data << (int)tree.Prior(p)->ltag << endl;
		}
	}
	return 0;

}