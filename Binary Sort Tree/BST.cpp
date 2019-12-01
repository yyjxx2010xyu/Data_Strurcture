#include <iostream>
#include <queue>
#include <stack>
#include <iomanip>
namespace Binary_Tree
{
#define FromParentTo(x) \
		(x->Is_Root() ? _root : ( x->Is_LC() ? (x->_pa->_lc) : (x->_pa->_rc )) ) 

	template <typename T>
	class BinNode;

	template <typename T>
	using pBinNode = BinNode<T>*;

	template <typename T>
	class BinTree;

	template <typename T>
	using pBinTree = BinTree<T>*;



	template <typename T>
	class BinNode
	{
	protected:
		T _data;
		pBinNode<T>  _pa, _lc, _rc;
		int _height;
	public:
		BinNode() : _pa(NULL), _lc(NULL), _rc(NULL), _height(0) {};
		BinNode(T data, pBinNode<T> pa = NULL, pBinNode<T> lc = NULL, pBinNode<T> rc = NULL, int height = 0, int size = 0) :
			_data(data), _pa(pa), _lc(lc), _rc(rc), _height(height) {};
		pBinNode<T> Insert_LC(const T& data)
		{
			return _lc = new BinNode(data, this);
		}
		pBinNode<T> Insert_RC(const T& data)
		{
			return _rc = new BinNode(data, this);
		}
		pBinNode<T>& LC() { return _lc; }
		pBinNode<T>& RC() { return _rc; }
		pBinNode<T>& PA() { return _pa; }
		int Height() { return _height; }
		T& Data() { return _data; }
		void Travel_Level(void(*visit)(T&));
		void Travel_Pre(void(*visit)(T&));		// preorder
		void Travel_In(void(*visit)(T&));		// inorder
		void Travel_Post(void(*visit)(T&));		// postorder
		void Travel_Print(pBinNode<T> Node, int depth, void(*visit)(T&));
		void Travel_In_Basic(void(*visit)(T&));
		bool Is_Root() { return !this->_pa; }
		bool Is_LC() { return !Is_Root() && (this == this->_pa->_lc); }
		bool Is_RC() { return !Is_Root() && (this == this->_pa->_rc); }
		bool Has_Pa() { return this->_pa; }
		bool Has_LC() { return this->_lc; }
		bool Has_RC() { return this->_rc; }
		bool Has_Child() { return Has_LC() || Has_RC(); }
		bool Has_Both_Child() { return Has_LC() && Has_RC(); }
		bool Is_Leaf() { return !Has_Child(); }

		friend BinTree<T>;
		template <typename VST>
		friend int removeAt(pBinNode<VST> Node);
		template <typename VST>
		friend void Release(pBinNode<VST> Node);
		template <typename VST>
		friend void Release(pBinTree<VST> Tree);
	};

	template <typename T>
	void BinNode<T>::Travel_Level(void(*visit)(T&))
	{
		std::queue<pBinNode<T>> Q;
		Q.push(this);
		while (!Q.empty())
		{
			pBinNode<T> u = Q.front();
			Q.pop();
			visit(u->_data);
			if (u->Has_LC())
				Q.push(u->_lc);
			if (u->Has_RC())
				Q.push(u->_rc);
		}
	}

	template <typename T>
	void BinNode<T>::Travel_Pre(void(*visit)(T&))
	{
		visit(this->_data);
		if (this->_lc)
			this->_lc->Travel_Pre(visit);
		if (this->_rc)
			this->_rc->Travel_Pre(visit);
	}

	template <typename T>
	void BinNode<T>::Travel_In(void(*visit)(T&))
	{
		if (this->_lc)
			this->_lc->Travel_In(visit);
		visit(this->_data);
		if (this->_rc)
			this->_rc->Travel_In(visit);
	}

	template <typename T>
	void BinNode<T>::Travel_Post(void(*visit)(T&))
	{
		if (this->_lc)
			this->_lc->Travel_Post(visit);
		if (this->_rc)
			this->_rc->Travel_Post(visit);
		visit(this->_data);
	}

	template<typename T>
	void BinNode<T>::Travel_Print(pBinNode<T> Node, int depth, void(*visit)(T&))
	{
		const int PRINT_GAP = 10;
		if (Node->_rc)
			Travel_Print(Node->_rc, depth + 1, visit);
		std::cout << std::setw((long long)PRINT_GAP * depth) << "";
		visit(Node->_data);
		std::cout << std::endl;
		if (Node->_lc)
			Travel_Print(Node->_lc, depth + 1, visit);
	}

	template<typename T>
	void BinNode<T>::Travel_In_Basic(void(*visit)(T&))
	{
		std::stack<pBinNode<T>> S;
		pBinNode<T> cur = this;
		while (cur != NULL || !S.empty())
		{
			if (cur)
			{
				std::cout << "push " << cur->_data << std::endl;
				S.push(cur);
				cur = cur->_lc;
			}
			else
			{
				std::cout << "pop" << std::endl;
				cur = S.top();
				S.pop();
				visit(cur->_data);
				std::cout << std::endl;
				cur = cur->_rc;
			}
		}
	}



	template <typename T>
	class BinTree
	{
	protected:
		int _size;
		pBinNode<T> _root;
		virtual int Update_Height(pBinNode<T> Node);
		void Update_Height_Above(pBinNode<T> Node);
		int _Create_Tree(const Binary_Tree::pBinNode<T>& parent, Binary_Tree::pBinNode<T>& Node);
		int _Leaf(pBinNode<T> Node);
		void _Rotate_Tree(pBinNode<T> Node);
	public:
		BinTree() :_size(0), _root(NULL) {};
		~BinTree() { if (_size)	Remove(_root); }
		int Size() { return _size; }
		int Height() { return _root->_height; }
		int Leaf();
		void Rotate_Tree() { _Rotate_Tree(_root); };
		bool Empty() { return !_root; }
		pBinNode<T>& Root() { return _root; }
		pBinNode<T> Insert_Root(T const& data);
		pBinNode<T> Insert_LC(pBinNode<T> Node, T const& data);
		pBinNode<T> Insert_RC(pBinNode<T> Node, T const& data);
		pBinNode<T> Attach_LC(pBinNode<T> Node, pBinTree<T> Att_Tree);
		pBinNode<T> Attach_RC(pBinNode<T> Node, pBinTree<T> Att_Tree);
		int Remove(pBinNode<T> Node);
		pBinTree<T> Secede(pBinNode<T> Node);

		void Create_Tree()
		{
			_size += _Create_Tree(NULL, _root);
		}


		//template <typename VST>
		void Travel_Level(void(*visit)(T&))
		{
			if (_root)
				_root->Travel_Level(visit);
		}

		void Travel_Pre(void(*visit)(T&))
		{
			if (_root)
				_root->Travel_Pre(visit);
		}
		void Travel_In(void(*visit)(T&))
		{
			if (_root)
				_root->Travel_In(visit);
		}
		void Travel_Post(void(*visit)(T&))
		{
			if (_root)
				_root->Travel_Post(visit);
		}
		void Travel_Print(void(*visit)(T&))
		{
			if (_root)
				_root->Travel_Print(_root, 0, visit);
		}
		void Travel_In_Basic(void(*visit)(T&))
		{
			if (_root)
				_root->Travel_In_Basic(visit);
		}
	};

	template<typename T>
	int BinTree<T>::Update_Height(pBinNode<T> Node)
	{
		return Node->_height = 1 + std::max(Node->_lc ? Node->_lc->_height : 0, Node->_rc ? Node->_rc->_height : 0);
	}

	template<typename T>
	void BinTree<T>::Update_Height_Above(pBinNode<T> Node)
	{
		while (Node)
		{
			Update_Height(Node);
			Node = Node->_pa;
		}
	}

	template<typename T>
	int BinTree<T>::Leaf()
	{
		return _Leaf(_root);
	}

	template<typename T>
	pBinNode<T> BinTree<T>::Insert_Root(T const& data)
	{
		_size = 1;
		return _root = new BinNode<T>(data);
	}

	template<typename T>
	pBinNode<T> BinTree<T>::Insert_LC(pBinNode<T> Node, T const& data)
	{
		_size++;
		Node->Insert_LC(data);
		Update_Height_Above(Node);
		return Node->_lc;
	}

	template<typename T>
	pBinNode<T> BinTree<T>::Insert_RC(pBinNode<T> Node, T const& data)
	{
		_size++;
		Node->Insert_RC(data);
		Update_Height_Above(Node);
		return Node->_rc;
	}

	template<typename T>
	pBinNode<T> BinTree<T>::Attach_LC(pBinNode<T> Node, pBinTree<T> Att_Tree)
	{
		if (Node->_lc = Att_Tree->_root)
			Node->_lc->_pa = Node;
		_size += Att_Tree->_size;
		Att_Tree->_size = 0;
		Update_Height_Above(Node);
		Release(Att_Tree);
		return Node;
	}

	template<typename T>
	pBinNode<T> BinTree<T>::Attach_RC(pBinNode<T> Node, pBinTree<T> Att_Tree)
	{
		if (Node->_rc = Att_Tree->_root)
			Node->_rc->_pa = Node;
		_size += Att_Tree->_size;

		Update_Height_Above(Node);
		Att_Tree->_size = 0;
		Release(Att_Tree);
		return Node;
	}

	template<typename T>
	int BinTree<T>::Remove(pBinNode<T> Node)
	{
		FromParentTo(Node) = NULL;
		Update_Height_Above(Node->_pa);
		int n = removeAt(Node);
		_size -= n;
		return n;
	}
	template<typename T>
	pBinTree<T> BinTree<T>::Secede(pBinNode<T> Node)
	{
		FromParentTo(Node) = NULL;
		Update_Height_Above(Node->_pa);
		pBinTree<T> Se_Tree = new BinTree<T>;
		Se_Tree->_root = Node;
		Node->_pa = NULL;
		Se_Tree->_size = Node->Size();
		_size -= Se_Tree->_size;
		return Se_Tree;

	}

	template <typename T>
	void Release(pBinNode<T> Node)
	{
		delete Node;
	}

	template<typename VST>
	void Release(pBinTree<VST> Tree)
	{
		delete Tree;
	}

	template <typename T>
	int removeAt(pBinNode<T> Node)
	{
		if (!Node)
			return 0;
		int n = 1 + removeAt(Node->_lc) + removeAt(Node->_rc);
		//release(Node->_data);
		Release(Node);
		return n;
	}

	template <typename T>
	int BinTree<T>::_Create_Tree(const Binary_Tree::pBinNode<T>& parent, Binary_Tree::pBinNode<T>& Node)
	{
		char ch = getchar();
		if (ch == '#')
			return 0;
		else
		{
			Node = new Binary_Tree::BinNode<T>(ch, parent);
			int res = _Create_Tree(Node, Node->_lc) + _Create_Tree(Node, Node->_rc) + 1;
			Update_Height_Above(Node);
			return res;
		}
	}
	template<typename T>
	int BinTree<T>::_Leaf(pBinNode<T> Node)
	{
		if (Node == NULL)
			return 0;
		if (Node->_lc == NULL && Node->_rc == NULL)
			return 1;
		return _Leaf(Node->_lc) + _Leaf(Node->_rc);
	}
	template<typename T>
	void BinTree<T>::_Rotate_Tree(pBinNode<T> Node)
	{
		std::swap(Node->_lc, Node->_rc);
		if (Node->_lc)
			_Rotate_Tree(Node->_lc);
		if (Node->_rc)
			_Rotate_Tree(Node->_rc);
	}
};

namespace BST
{
	using namespace Binary_Tree;
	template <typename T>
	class BST : public BinTree<T>
	{
	protected:
		pBinNode<T> _hot;
		pBinNode<T>& _Search(pBinNode<T>& u, const T& e, pBinNode<T>& hot);
		pBinNode<T> _Remove(pBinNode<T>& u, pBinNode<T>& hot);
		int _Get_Depth(pBinNode<T> u, int Depth);
	public:
		BST();
		virtual pBinNode<T>& Search(const T& e);
		virtual pBinNode<T> Insert(const T& e);
		virtual bool Remove(const T& e);
		double Get_Depth();
	};

	template <typename T>
	pBinNode<T>& BST<T>::_Search(pBinNode<T>& u, const T& e, pBinNode<T>& hot)
	{
		if (!u || (u->Data() == e))
			return u;
		hot = u;
		return _Search((e < u->Data()) ? u->LC() : u->RC(), e, hot);
	}
	template<typename T>
	BST<T>::BST()
	{
		_hot = NULL;
	}
	template <typename T>
	pBinNode<T>& BST<T>::Search(const T& e)
	{
		return _Search(this->_root, e, _hot);
	}

	template <typename T>
	pBinNode<T> BST<T>::Insert(const T& e)
	{
		pBinNode<T>& u = Search(e);
		//	目标已存在
		if (u)
			return u;
		u = new BinNode<T>(e, _hot);
		this->_size++;
		this->Update_Height_Above(u);
		return u;
	}
	template <typename T>
	pBinNode<T> BST<T>::_Remove(pBinNode<T>& u, pBinNode<T>& hot)
	{
		pBinNode<T> uPa = u->PA();
		pBinNode<T> uBak = u;
		//	OJ要求是直接使用前驱替换
		pBinNode<T> pre = u;
		//	实际被删除的节点的替代者
		pBinNode<T> rep = NULL;

		//	无左右子树 在这里特判了
		if (!u->Has_LC() && !u->Has_RC())
		{
			free(u);
			u = NULL;
			hot = uPa;
			return u;
		}

		//	只有左子树
		if (u->Has_LC() && !u->Has_RC())
			rep = u = u->LC();
		//	只有右子树
		if (!u->Has_LC() && u->Has_RC())
			rep = u = u->RC();
		//	有左右子树
		if (u->Has_Both_Child())
		{
			pre = u->LC();
			while (pre->RC())
				pre = pre->RC();

			std::swap(u->Data(), pre->Data());

			//	特判parent为father的情况
			if (pre->PA() != u)
				rep = pre->PA()->RC() = pre->LC();
			else
				rep = pre->PA()->LC() = pre->LC();
		}

		hot = pre->PA();
		if (rep)
			rep->PA() = hot;
		delete pre;
		return rep;
	}

	template <typename T>
	bool BST<T>::Remove(const T& e)
	{
		pBinNode<T>& u = Search(e);
		if (!u)
			return false;
		_Remove(u, _hot);
		this->_size--;
		this->Update_Height_Above(_hot);
		return true;
	}

	template <typename T>
	int BST<T>::_Get_Depth(pBinNode<T> u, int Depth)
	{
		if (u == NULL)
			return 0;
		return Depth + _Get_Depth(u->LC(), Depth + 1) + _Get_Depth(u->RC(), Depth + 1);
	}
	template <typename T>
	double BST<T>::Get_Depth()
	{
		return (double)_Get_Depth(this->_root, 1) / (double)this->_size;
	}
};

void myvisit(int& e)
{
	std::cout << e << " ";
}
int main()
{
	BST::BST<int> Tree;
	int n, e;
	std::cin >> n;
	for (int i = 1; i <= n; i++)
	{
		std::cin >> e;
		Tree.Insert(e);
	}
	int ae;
	std::cin >> ae;
	if (Tree.Remove(ae))
		std::cout << "1" << std::endl;
	else
		std::cout << "0" << std::endl;
	if (Tree.Search(ae))
		std::cout << "1" << std::endl;
	else
		std::cout << "0" << std::endl;



	Tree.Insert(ae);
	Tree.Travel_Pre(myvisit);
	std::cout << std::endl;
	std::cout << std::fixed << std::setprecision(2) << Tree.Get_Depth() << std::endl;

	return 0;
}