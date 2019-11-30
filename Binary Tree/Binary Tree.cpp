#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
#include <queue>
#include <bitset>
#include <iomanip>
#include <map>
#include <string>
#include <algorithm>
#include <list>
#include <cstdio>
#include <cstring>

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
		T Data() { return _data; }
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
		const int PRINT_GAP = 5;
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

namespace Huffman_Tree
{
	using Binary_Tree::BinTree;
	using Binary_Tree::BinNode;
	using Binary_Tree::pBinNode;
	using Binary_Tree::pBinTree;

	const int MAX_DEPTH = 1000;			//	树的最深深度
	const int N_CHAR = 256;				//	字符个数，从0到255
	const char JOINT_C = '^';			//	链接字符

	class Huff_Char
	{
	protected:
		char _ch;			//	字符
		int _weight;		//	权重
	public:

		Huff_Char(char ch, int weight) : _ch(ch), _weight(weight) {}
		int Weight() { return _weight; }
		char Ch() { return _ch; }
	};


	using Huff_Tree = BinTree<Huff_Char>;
	using pHuff_Tree = Huff_Tree*;
	using Huff_Node = BinNode<Huff_Char>;
	using pHuff_Node = Huff_Node*;

	struct Cmp
	{
		bool operator () (pHuff_Tree A, pHuff_Tree B)
		{
			return A->Root()->Data().Weight() > B->Root()->Data().Weight();
		}
	};


	typedef std::priority_queue<pHuff_Tree, std::vector<pHuff_Tree>, Cmp> Huff_Forest;
	typedef std::bitset<MAX_DEPTH> Huff_Code;
	typedef std::map<char, std::string> Huff_Table;

	//	统计字符串中每个字符的出现频率
	void Statistics(std::string Str, int* freq)
	{
		for (size_t i = 0; i < Str.length(); i++)
			freq[(int)Str[i]]++;
	}

	//	初始化Huffman Forest，即将每个节点都变成Huff_Tree放到Huff_Forest里
	void Init_Huff_Forest(Huff_Forest& Forest, int* freq)
	{
		// 清空优先队列
		while (!Forest.empty())
			Forest.pop();
		for (int i = 0; i < N_CHAR; i++)
			if (freq[i])			//	只给存在的字符建树	
			{
				pHuff_Tree tp = new Huff_Tree;
				tp->Insert_Root(Huff_Char(i, freq[i]));
				Forest.push(tp);
			}
	}

	//	通过优先队列生成Huffman树，返回树的根节点
	pHuff_Tree Generate_Tree(Huff_Forest& Forest)
	{
		//	只有一个字符的情况,要特殊处理
		if (Forest.size() == 1)
		{
			pHuff_Tree tp1 = Forest.top();
			Forest.pop();
			pHuff_Tree tp = new Huff_Tree;
			tp->Insert_Root(Huff_Char(JOINT_C, tp1->Root()->Data().Weight()));
			tp->Attach_LC(tp->Root(), tp1);
			Forest.push(tp);
		}
		while (Forest.size() >= 2)
		{
			pHuff_Tree tp1 = Forest.top();
			Forest.pop();
			pHuff_Tree tp2 = Forest.top();
			Forest.pop();
			pHuff_Tree tp = new Huff_Tree;
			tp->Insert_Root(Huff_Char(JOINT_C, tp1->Root()->Data().Weight() + tp2->Root()->Data().Weight()));
			tp->Attach_LC(tp->Root(), tp1);
			tp->Attach_RC(tp->Root(), tp2);
			Forest.push(tp);
		}
		return Forest.top();
	}

	//	将通过Huffman树，生成Huffman编码
	static void _Generate_T(pHuff_Node Node, int depth, Huff_Table& Table, Huff_Code& Code)
	{
		if (Node->Is_Leaf())
		{
			std::string _ = Code.to_string().substr(Code.size() - depth);
			std::reverse(_.begin(), _.end());
			Table.insert({ (char)Node->Data().Ch(), _ });
			return;
		}
		if (Node->Has_LC())
		{
			//	置0
			Code.reset(depth);
			_Generate_T(Node->LC(), depth + 1, Table, Code);
		}
		if (Node->Has_RC())
		{
			//	置1
			Code.set(depth);
			_Generate_T(Node->RC(), depth + 1, Table, Code);
		}

	}
	void Generate_Table(pHuff_Tree Huff_Tree, Huff_Table& Table)
	{
		Huff_Code Code;
		_Generate_T(Huff_Tree->Root(), 0, Table, Code);
	}
	std::string Encode(Huff_Table& Table, std::string Str)
	{
		std::string Ret("");
		for (size_t i = 0; i < Str.length(); i++)
		{
			std::string Cur = Table[Str[i]];
			Ret += Cur;
		}
		return Ret;
	}
	std::string Decode(const pHuff_Tree& Tree, std::string Str)
	{
		std::string Ret("");
		pHuff_Node p = Tree->Root();
		for (size_t i = 0; i < Str.length(); i++)
		{
			if (Str[i] == '0')
				p = p->LC();
			if (Str[i] == '1')
				p = p->RC();
			if (p->Is_Leaf())
			{
				Ret += p->Data().Ch();
				p = Tree->Root();
			}
		}
		return Ret;
	}

}

template <typename T>
void myvisit(T& val)
{
	std::cout << val;
}


int main()
{
	// T1
	if (0)
	{
		Binary_Tree::BinTree<char> Tree;
		Tree.Create_Tree();
		Tree.Travel_Pre(myvisit);
		std::cout << std::endl;
		Tree.Travel_In(myvisit);
		std::cout << std::endl;
		Tree.Travel_Post(myvisit);
		std::cout << std::endl;
		Tree.Travel_Level(myvisit);
		std::cout << std::endl;
		Tree.Travel_Print(myvisit);
	}

	// T2
	if (0)
	{
		Binary_Tree::BinTree<char> Tree;
		Tree.Create_Tree();
		std::cout << Tree.Leaf() << std::endl;
		std::cout << Tree.Size() << std::endl;
		std::cout << Tree.Height() << std::endl;
		Tree.Travel_Print(myvisit);
		std::cout << std::endl;
		Tree.Rotate_Tree();
		Tree.Travel_Print(myvisit);
		std::cout << std::endl;
	}

	// T3
	if (0)
	{
		Binary_Tree::BinTree<char> Tree;
		Tree.Create_Tree();
		Tree.Travel_In_Basic(myvisit);
	}

	// Huffman Tree
	if (1)
	{
		//freopen("c.in", "r", stdin);
		std::cout << "请输入串" << std::endl;
		std::string Str;
		std::cin >> Str;
		int* freq = new int[Huffman_Tree::N_CHAR];
		memset(freq, 0, sizeof(int) * Huffman_Tree::N_CHAR);
		Huffman_Tree::Statistics(Str, freq);
		Huffman_Tree::Huff_Forest Forest;
		Huffman_Tree::Huff_Table Table;
		Huffman_Tree::Init_Huff_Forest(Forest, freq);
		Huffman_Tree::pHuff_Tree pTree = Huffman_Tree::Generate_Tree(Forest);
		Huffman_Tree::Generate_Table(pTree, Table);
		std::string Encrypted = Huffman_Tree::Encode(Table, Str);
		std::cout << "编码结果" << Encrypted << std::endl;

		std::cout << "每个字符的哈夫曼编码" << std::endl;
		for (int i = 0; i < Huffman_Tree::N_CHAR; i++)
			if (freq[i])
				std::cout << (char)i << " " << Table[i] << std::endl;
		std::cout << std::endl;

		std::string Decrypted = Huffman_Tree::Decode(pTree, Encrypted);
		std::cout << "解码结果" << Decrypted << std::endl;
		delete[] freq;
		//fclose(stdin);
	}

	// POJ 1521
	if (0)
	{
		while (true)
		{
			std::string Str;
			std::cin >> Str;
			if (Str == "END")
				break;
			int* freq = new int[Huffman_Tree::N_CHAR];
			memset(freq, 0, sizeof(int) * Huffman_Tree::N_CHAR);
			Huffman_Tree::Statistics(Str, freq);
			Huffman_Tree::Huff_Forest Forest;
			Huffman_Tree::Huff_Table Table;
			Huffman_Tree::Init_Huff_Forest(Forest, freq);
			Huffman_Tree::pHuff_Tree pTree = Huffman_Tree::Generate_Tree(Forest);
			Huffman_Tree::Generate_Table(pTree, Table);
			std::string Encrypted = Huffman_Tree::Encode(Table, Str);

			std::cout << Str.length() * 8 << " " << Encrypted.length() << " " << std::fixed << std::setprecision(1) << (double)((double)Str.length() * 8.0) / (double)Encrypted.length() << std::endl;

			delete[] freq;
		}
	}
	return 0;
}