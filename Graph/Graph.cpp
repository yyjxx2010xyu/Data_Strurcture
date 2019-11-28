#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <iomanip>
#include <stack>
#include <queue>
#include <algorithm>
#include <cassert>

namespace Graph
{
	//	最大权重
	const int MAX_RANK = 0x3f3f3f3f;
	const int MAX_DIS = 0x3f3f3f3f;
	//	点状态
	enum class VStatus { UNDISCOVERED, DISCOVERED, VISITED };
	//	边状态
	enum class EStatus { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD };

	//	模板类声明
	template <typename VType>	//	图基类
	class Graph;
	template <typename VType>	//	基于邻接矩阵的图
	class Graph_Matrix;
	template <typename VType>	//	基于邻接链表的图
	class Graph_List;
	template <typename VType>	//	点
	class Vertex;
	class Edge;					//	边

	template <typename VType>
	class Vertex
	{
	private:
		VType _name;					//	点名字
		int _indegree, _outdegree;		//	出入度
		VStatus _status;				//	点状态
		int _beg_stamp, _end_stamp;		//	出入时间戳
		int _father;					//	父节点
		int _rank;						//	优先级
	public:
		Vertex(VType const& name = (VType)0) :
			_name(name), _indegree(0), _outdegree(0), _status(VStatus::UNDISCOVERED),
			_beg_stamp(-1), _end_stamp(-1), _father(-1), _rank(MAX_RANK) {};
		//使用模板时，不能在嵌套作用域中用相同的名称声明模板参数
		/*
		Error
		template<class T>
		class linklist
		{
			template<class T>   //错误
			class node
			{
			}
		}

		//Correct
		template<class T>
		class linklist
		{
			template<class U>
			class node
			{
			}
		}
		*/
		template <typename VST>
		friend class Graph;
		template <typename VST>
		friend class Graph_Matrix;
		template <typename VST>
		friend class Graph_List;
	};

	class Edge
	{
	private:
		int _weight;					//	边权
		EStatus _status;				//	边状态
		int _v;							//	指向节点，为Id而不是VType的名字
	public:
		Edge(int weight, int v = 0) :
			_weight(weight), _status(EStatus::UNDETERMINED), _v(v) {}


		template <typename VST>
		friend class Graph;
		template <typename VST>
		friend class Graph_Matrix;
		template <typename VST>
		friend class Graph_List;
		template <typename VST>
		friend bool Critical_Path(Graph_List<VST>& List);
		template <typename VST>
		friend bool Topological_Order(Graph_List<VST>& List, std::stack<int>& S, std::vector<int>& Route);
		friend bool Edge_Cmp(Edge* A, Edge* B);
	};

	template <typename VType>
	class Graph
	{
	private:
		void _Clear()											// 重置 
		{
			for (int i = 0; i < n; i++)
			{
				Status(i) = VStatus::UNDISCOVERED;
				Beg_Stamp(i) = End_Stamp(i) = -1;
				Father(i) = -1;
				Rank(i) = MAX_RANK;
				for (int j = 0; j < n; j++)
					if (Exist(i, j))
						Status(i, j) = EStatus::UNDETERMINED;
			}
		}
		virtual void _Bfs(const int u, std::queue<int>& S) = 0;	//	Bfs
		virtual void _Dfs(const int u, std::queue<int>& S) = 0;	//	Dfs
	public:
		Graph()
		{
			this->n = this->e = 0;
		}
		int n;				//	点个数
		int e;				//	边个数
		//	纯虚函数定义	
		//	点操作
		virtual void Insert(VType const&) = 0;					//	插入点
		virtual VType& Name(int) = 0;							//	点名字
		virtual int& InDegree(int) = 0;							//	入度
		virtual int& OutDegree(int) = 0;						//	初度
		virtual VStatus& Status(int) = 0;						//	点状态
		virtual int& Beg_Stamp(int) = 0;						//	开始访问时间戳
		virtual int& End_Stamp(int) = 0;						//	结束访问时间戳
		virtual int& Father(int) = 0;							//	遍历时的父亲节点
		virtual int& Rank(int) = 0;								//	优先级
		//	边操作
		virtual bool Exist(int, int) = 0;						//	<u,v> 存在
		virtual void Insert(int, int, int) = 0;					//	<u,v> 之间插入权w
		virtual EStatus& Status(int, int) = 0;					//	边状态
		virtual int& Weight(int, int) = 0;						//	边权
	};


	const bool PRINT_W = true;
	const bool NOT_PRINT_W = false;
	template <typename VType>
	class Graph_Matrix :public Graph<VType>
	{
	private:
		std::vector< Vertex<VType> > _V;				//	记录点
		std::vector<std::vector< Edge* > > _E;			//	邻接矩阵
		std::map<VType, int> _Id;						//	点的编号
		virtual void _Bfs(const int u, std::queue<int>& S) override
		{
			std::queue<int> Q;
			Q.push(u);
			S.push(u);
			Status(u) = VStatus::VISITED;
			while (!Q.empty())
			{
				int v = Q.front();

				Q.pop();
				for (int i = 0; i < this->n; i++)
					if (_E[v][i] && Status(i) == VStatus::UNDISCOVERED)
					{
						Q.push(i);
						Status(i) = VStatus::VISITED;
						S.push(i);
					}
			}
		}
		virtual void _Dfs(const int u, std::queue<int>& S) override
		{
			S.push(u);
			Status(u) = VStatus::VISITED;
			for (int v = 0; v < this->n; v++)
				if (_E[u][v] && Status(v) == VStatus::UNDISCOVERED)
					_Dfs(v, S);
		}
	public:
		//	构造函数
		Graph_Matrix()
		{
			_V.clear();
			_E.clear();
			_Id.clear();
		}
		//	析构函数
		~Graph_Matrix()
		{
			for (int i = 0; i < this->n; i++)
				for (int j = 0; j < this->n; j++)
					delete _E[i][j];
		}
		int Id(VType u)												//	节点Id
		{
			return _Id[u];
		}

		void Print_Ver()											//	输出节点名字
		{
			for (int i = 0; i < this->n; i++)
				std::cout << _V[i]._name << " ";
		}
		void Print_Matrix()											//	输出邻接矩阵
		{
			for (int i = 0; i < this->n; i++)
			{
				for (int j = 0; j < this->n; j++)
					if (_E[i][j] == NULL)
						std::cout << std::setw(4) << "0";
					else
						std::cout << std::setw(4) << _E[i][j]->_weight;
				std::cout << std::endl;
			}
		}
		//	派生类虚函数定义	
		//	点操作
		virtual void Insert(VType const& vertex) override			//	插入点
		{
			_V.push_back(Vertex<VType>(vertex));
			_Id[vertex] = this->n;
			this->n++;
			_E.resize(this->n);
			for (int i = 0; i < this->n; i++)
				_E[i].resize(this->n);
		}

		virtual VType& Name(int u) override							//	点名字
		{
			return _V[u]._name;
		}
		virtual int& InDegree(int u) override						//	入度
		{
			return _V[u]._indegree;
		}
		virtual int& OutDegree(int u) override						//	初度
		{
			return _V[u]._outdegree;
		}
		virtual VStatus& Status(int u) override						//	点状态
		{
			return _V[u]._status;
		}
		virtual int& Beg_Stamp(int u) override						//	开始访问时间戳
		{
			return _V[u]._beg_stamp;
		}
		virtual int& End_Stamp(int u) override						//	结束访问时间戳
		{
			return _V[u]._end_stamp;
		}
		virtual int& Father(int u) override							//	遍历时的父亲节点
		{
			return _V[u]._father;
		}
		virtual int& Rank(int u) override							//	优先级
		{
			return _V[u]._rank;
		}
		//	边操作
		virtual bool Exist(int u, int v) override					//	<u,v> 存在
		{
			return  u >= 0 && u < this->n && v >= 0 && v < this->n && _E[u][v] != NULL;
		}
		virtual void Insert(int u, int v, int w = 0) override		//	<u,v> 之间插入权w
		{
			//	无重边
			if (Exist(u, v))
				return;
			if (!(u >= 0 && u < this->n && v >= 0 && v < this->n))
				return;
			_E[u][v] = new Edge(w);
			this->e++;
			_V[u]._outdegree++;
			_V[v]._indegree++;
		}
		virtual EStatus& Status(int u, int v) override				//	边状态
		{
			return _E[u][v]->_status;
		}
		virtual int& Weight(int u, int v) override					//	边权重
		{
			return _E[u][v]->_weight;
		}

		void Print_S(std::queue<int> S)
		{
			int nS = S.size();
			std::cout << "{";
			while (!S.empty())
			{
				nS--;
				if (nS)
					std::cout << S.front() << " ";
				else
					std::cout << S.front();
				S.pop();
			}
			std::cout << "}";
		}
		//	遍历
		void Bfs()
		{
			for (int i = 0; i < this->n; i++)
				Status(i) = VStatus::UNDISCOVERED;
			std::queue<int> S;
			for (int i = 0; i < this->n; i++)
				if (Status(i) == VStatus::UNDISCOVERED)
				{
					while (!S.empty())
						S.pop();
					_Bfs(i, S);
					if (!S.empty())
						Print_S(S);
				}
		}
		void Dfs()
		{
			for (int i = 0; i < this->n; i++)
				Status(i) = VStatus::UNDISCOVERED;
			std::queue<int> S;
			for (int i = 0; i < this->n; i++)
				if (Status(i) == VStatus::UNDISCOVERED)
				{
					while (!S.empty())
						S.pop();
					_Dfs(i, S);
					if (!S.empty())
						Print_S(S);
				}
		}
	};
	bool Edge_Cmp(Edge* A, Edge* B)
	{
		return A->_v < B->_v;
	}

	//	要加typename
	//	编译器不知道list<T*>::iterator是代表一个类型,还是代表list<T*>类中的一个成员，叫做iterator。
	//	方便给使用的类型的使用
	using l_iter = typename std::list<Edge* >::iterator;
	using r_iter = typename std::list<Edge* >::reverse_iterator;
	//	而且比较还得是同一个List 否则会list iterators incompatible 原因是myIt是myList的迭代器，而myList被赋新值之后变成了别的list的副本，所以myIt和被赋值后的myList的迭代器自然也是incompatable(无法比较)的。
	//std::list< Edge<EType>* > ____TL;
	//const l_iter NULL_It = ____TL.end();

	template <typename VType>
	class Graph_List :public Graph<VType>
	{

	private:
		std::vector< Vertex<VType> > _V;				//	记录点
		std::vector<std::list< Edge* > > _L;			//	邻接链表
		std::map<VType, int> _Id;						//	点的编号
		virtual void _Bfs(const int u, std::queue<int>& S) override
		{
			std::queue<int> Q;
			Q.push(u);
			S.push(u);
			Status(u) = VStatus::VISITED;
			while (!Q.empty())
			{
				int v = Q.front();
				Q.pop();
				for (r_iter i = _L[v].rbegin(); i != _L[v].rend(); i++)
					if (Status((*i)->_v) == VStatus::UNDISCOVERED)
					{
						Q.push((*i)->_v);
						Status((*i)->_v) = VStatus::VISITED;
						S.push((*i)->_v);
					}
			}
		}
		virtual void _Dfs(const int u, std::queue<int>& S) override
		{
			S.push(u);
			Status(u) = VStatus::VISITED;
			for (r_iter v = _L[u].rbegin(); v != _L[u].rend(); v++)
				if (Status((*v)->_v) == VStatus::UNDISCOVERED)
					_Dfs((*v)->_v, S);
		}
	public:
		Graph_List()											//	构造函数
		{
			_V.clear();
			_L.clear();
			_Id.clear();
		}
		~Graph_List()											//	析构函数
		{
			for (int i = 0; i < this->n; i++)
				for (l_iter j = _L[i].begin(); j != _L[i].end(); j++)
					delete (*j);
		}
		int Id(VType u)											//	节点Id
		{
			return _Id[u];
		}


		void Sort()												//	边排序
		{
			for (int i = 0; i < this->n; i++)
				_L[i].sort(Edge_Cmp);
		}
		void Print_Ver()										//	输出节点名字
		{
			for (int i = 0; i < this->n; i++)
				std::cout << _V[i]._name << " ";
		}

		void Print_List(bool Print_W)							//	输出整个邻接表
		{
			for (int i = 0; i < this->n; i++)
			{
				std::cout << _V[i]._name << "-->";
				for (r_iter j = _L[i].rbegin(); j != _L[i].rend(); j++)
				{

					if (Print_W)
						std::cout << (*j)->_v << "," << (*j)->_weight << " ";
					else
						std::cout << (*j)->_v << " ";
				}
				std::cout << std::endl;
			}
		}

		l_iter Get_Iter(int u, int v)							//	得到u指向v的指针
		{
			l_iter it = _L[u].begin();
			for (; it != _L[u].end() && (*it)->_v != v; it++);
			return it;
		}

		//	派生类虚函数定义	
		//	点操作
		virtual void Insert(VType const& vertex) override		//	插入点
		{
			_V.push_back(Vertex<VType>(vertex));
			_Id[vertex] = this->n;
			this->n++;
			_L.resize(this->n);
		}

		virtual VType& Name(int u) override						//	点名字
		{
			return _V[u]._name;
		}
		virtual int& InDegree(int u) override					//	入度
		{
			return _V[u]._indegree;
		}
		virtual int& OutDegree(int u) override					//	初度
		{
			return _V[u]._outdegree;
		}
		virtual VStatus& Status(int u) override					//	点状态
		{
			return _V[u]._status;
		}
		virtual int& Beg_Stamp(int u) override					//	开始访问时间戳
		{
			return _V[u]._beg_stamp;
		}
		virtual int& End_Stamp(int u) override					//	结束访问时间戳
		{
			return _V[u]._end_stamp;
		}
		virtual int& Father(int u) override						//	遍历时的父亲节点
		{
			return _V[u]._father;
		}
		virtual int& Rank(int u) override						//	优先级
		{
			return _V[u]._rank;
		}
		//	边操作
		virtual bool Exist(int u, int v) override				//	<u,v> 存在
		{
			return  u >= 0 && u < this->n && v >= 0 && v < this->n && (Get_Iter(u, v) != _L[u].end());
		}

		virtual void Insert(int u, int v, int w = 0) override	//	<u,v> 之间插入权w
		{
			//	无重边
			if (Exist(u, v))
				return;
			if (!(u >= 0 && u < this->n && v >= 0 && v < this->n))
				return;
			_L[u].push_back(new Edge(w, v));
			this->e++;
			_V[u]._outdegree++;
			_V[v]._indegree++;
		}

		virtual EStatus& Status(int u, int v) override			//	边状态
		{
			return (*Get_Iter(u, v))->_status;
		}

		virtual int& Weight(int u, int v) override				//	边权重
		{
			return (*Get_Iter(u, v))->_weight;
		}

		void Print_S(std::queue<int> S)
		{
			int nS = S.size();
			std::cout << "{";
			while (!S.empty())
			{
				nS--;
				if (nS)
					std::cout << S.front() << " ";
				else
					std::cout << S.front();
				S.pop();
			}
			std::cout << "}";
		}
		//	遍历
		void Bfs()
		{
			for (int i = 0; i < this->n; i++)
				Status(i) = VStatus::UNDISCOVERED;
			std::queue<int> S;
			for (int i = 0; i < this->n; i++)
				if (Status(i) == VStatus::UNDISCOVERED)
				{
					while (!S.empty())
						S.pop();
					_Bfs(i, S);
					if (!S.empty())
						Print_S(S);
				}
		}
		void Dfs()
		{
			for (int i = 0; i < this->n; i++)
				Status(i) = VStatus::UNDISCOVERED;
			std::queue<int> S;
			for (int i = 0; i < this->n; i++)
				if (Status(i) == VStatus::UNDISCOVERED)
				{
					while (!S.empty())
						S.pop();
					_Dfs(i, S);
					if (!S.empty())
						Print_S(S);
				}
		}

		const l_iter Head(int u)
		{
			return _L[u].begin();
		}
		const l_iter End(int u)
		{
			return _L[u].end();
		}

		const r_iter rHead(int u)
		{
			return _L[u].rbegin();
		}
		const r_iter rEnd(int u)
		{
			return _L[u].rend();
		}

		template <typename VST>
		friend bool Critical_Path(Graph_List<VST>& List);

	};


	template <typename VType>
	bool Topological_Order(Graph_List<VType>& List, std::stack<int>& S, std::vector<int>& Route)	//	拓扑排序
	{
		std::stack<int> Zero;
		for (int i = 0; i < List.n - 1; i++)
			if (List.InDegree(i) == 0)
				Zero.push(i);

		int cnt = 0;
		while (!Zero.empty())
		{
			int u = Zero.top();
			Zero.pop();
			S.push(u);
			cnt++;
			for (r_iter i = List.rHead(u); i != List.rEnd(u); i++)
			{
				int v = (*i)->_v;
				int w = (*i)->_weight;
				if (--List.InDegree(v) == 0)
					Zero.push(v);
				if (Route[u] + w > Route[v])
					Route[v] = Route[u] + w;

			}
		}
		//	有回路
		return cnt == List.n;
	}
	template <typename VType>
	bool Critical_Path(Graph_List<VType>& List)														//	关键路径
	{
		std::vector<int> Dis, Route;
		Dis.resize(List.n);
		Route.resize(List.n);
		std::stack<int> S;
		if (!Topological_Order(List, S, Route))
			return false;

		int Dest = List.n - 1;

		int t = 0;
		for (int i = 0; i < List.n; i++)
			if (t < Route[i])
			{
				t = Route[i];
				Dest = i;
			}
		for (int i = 0; i < List.n; i++)
			Dis[i] = Route[Dest];


		while (!S.empty())
		{
			int u = S.top();
			S.pop();
			for (l_iter i = List.Head(u); i != List.End(u); i++)
			{
				int v = (*i)->_v;
				int w = (*i)->_weight;
				if (Dis[v] - w < Dis[u])
					Dis[u] = Dis[v] - w;

			}
		}
		std::cout << Route[Dest] << std::endl;

		for (int u = 0; u < List.n; u++)
			for (r_iter i = List.rHead(u); i != List.rEnd(u); i++)
			{
				int v = (*i)->_v;
				int w = (*i)->_weight;
				if (Route[u] == Dis[v] - w)
					std::cout << List.Name(u) << "->" << List.Name(v) << std::endl;
			}

		return true;
	}
};


template <typename T>
void Euler(Graph::Graph_Matrix<T>& G, int u, int Depth, std::vector<std::vector<bool> >& visited, std::vector<int>& Path, std::vector<std::vector<int> >& Answer)
{
	//	已经到了最后一个元素了，只需要检查他能不能原点即可 
	if (Depth == G.n)
	{
		//	第一个元素
		if (G.Exist(Path[0], u) && !visited[Path[0]][u])
			Answer.push_back(Path);
		return;
	}
	for (int v = 0; v < G.n; v++)
		if (G.Exist(u, v) && u != v && !visited[u][v])
		{
			visited[u][v] = visited[v][u] = true;
			Path.push_back(v);
			Euler(G, v, Depth + 1, visited, Path, Answer);
			visited[u][v] = visited[v][u] = false;
			Path.pop_back();
		}

}

int main()
{

	// T1
	if (0)
	{
		//freopen("c.in", "r", stdin);
		//	1为有向图,2为有向网,3为无向图,4为无向网
		enum class GType { Di_Graph = 1, Di_Net, Undi_Graph, Undi_Net };
		Graph::Graph_Matrix<char> G_Matrix;
		Graph::Graph_List<char> G_List;

		GType Graph_Type;
		int  n, m, type;
		char u, v;
		int w;
		std::string VerName;
		std::cin >> type;
		Graph_Type = static_cast<GType>(type);
		std::cin >> n >> m;
		std::cin >> VerName;
		for (int i = 0; i < n; i++)
		{
			G_Matrix.Insert(VerName[i]);
			G_List.Insert(VerName[i]);
		}
		G_Matrix.Print_Ver();
		std::cout << std::endl;
		for (int i = 1; i <= m; i++)
		{
			switch (Graph_Type)
			{
			case GType::Di_Graph:
			{
				std::cin >> u >> v;
				int u_id = G_Matrix.Id(u);
				int v_id = G_Matrix.Id(v);
				G_Matrix.Insert(u_id, v_id, 1);
				G_List.Insert(u_id, v_id, 1);
				break;
			}
			case GType::Di_Net:
			{
				std::cin >> u >> v >> w;
				int u_id = G_Matrix.Id(u);
				int v_id = G_Matrix.Id(v);
				G_Matrix.Insert(u_id, v_id, w);
				G_List.Insert(u_id, v_id, w);
				break;
			}
			case GType::Undi_Graph:
			{
				std::cin >> u >> v;
				int u_id = G_Matrix.Id(u);
				int v_id = G_Matrix.Id(v);
				G_Matrix.Insert(u_id, v_id, 1);
				G_Matrix.Insert(v_id, u_id, 1);
				G_List.Insert(u_id, v_id, 1);
				G_List.Insert(v_id, u_id, 1);
				break;
			}
			case GType::Undi_Net:
			{
				std::cin >> u >> v >> w;
				int u_id = G_Matrix.Id(u);
				int v_id = G_Matrix.Id(v);
				G_Matrix.Insert(u_id, v_id, w);
				G_Matrix.Insert(v_id, u_id, w);
				G_List.Insert(u_id, v_id, w);
				G_List.Insert(v_id, u_id, w);
				break;
			}
			}
		}
		G_Matrix.Print_Matrix();

		switch (Graph_Type)
		{
		case GType::Di_Graph:
			G_List.Print_List(Graph::NOT_PRINT_W);
			break;
		case GType::Di_Net:
			G_List.Print_List(Graph::PRINT_W);
			break;
		case GType::Undi_Graph:
			G_List.Print_List(Graph::NOT_PRINT_W);
			break;
		case GType::Undi_Net:
			G_List.Print_List(Graph::PRINT_W);
			break;
		}
		//fclose(stdin);
	}

	// T2
	if (0)
	{
		int n, m, u, v;
		std::cin >> n >> m;
		Graph::Graph_Matrix<int> G_Matrix;
		for (int i = 0; i < n; i++)
			G_Matrix.Insert(i);
		for (int i = 0; i < m; i++)
		{
			std::cin >> u >> v;
			G_Matrix.Insert(u, v);
			G_Matrix.Insert(v, u);
		}
		G_Matrix.Dfs();
		std::cout << std::endl;
		G_Matrix.Bfs();
		std::cout << std::endl;
	}

	// T3
	if (0)
	{
		int n, m, u, v;
		std::cin >> n >> m;
		Graph::Graph_List<int> G_List;
		for (int i = 0; i < n; i++)
			G_List.Insert(i);
		for (int i = 0; i < m; i++)
		{
			std::cin >> u >> v;
			G_List.Insert(u, v);
			G_List.Insert(v, u);
		}
		G_List.Dfs();
		std::cout << std::endl;
		G_List.Bfs();
		std::cout << std::endl;
	}

	// T4
	if (0)
	{
		int n, m, u, v, w;
		std::cin >> n >> m;
		Graph::Graph_List<int> G_List;
		for (int i = 1; i <= n; i++)
			G_List.Insert(i);
		for (int i = 0; i < m; i++)
		{
			std::cin >> u >> v >> w;
			int u_id = G_List.Id(u);
			int v_id = G_List.Id(v);
			G_List.Insert(u_id, v_id, w);
		}
		if (!Graph::Critical_Path(G_List))
			puts("0");
	}

	//	T5
	if (0)
	{
		int n, m, u, v, w;
		std::cin >> n >> m;
		Graph::Graph_List<int> G_List;
		for (int i = 1; i <= n; i++)
			G_List.Insert(i);
		for (int i = 0; i < m; i++)
		{
			std::cin >> u >> v >> w;
			int u_id = G_List.Id(u);
			int v_id = G_List.Id(v);
			G_List.Insert(u_id, v_id);
		}
		std::vector<int> Dis, Route;
		Dis.resize(G_List.n);
		Route.resize(G_List.n);
		std::stack<int> S;
		if (Graph::Topological_Order(G_List, S, Route))
			puts("1");
		else
			puts("0");
	}

	//	Euler	一笔画问题
	/*
		Graph:
		   4
		   /\
		  /  \
		 /    \
		/      \
		5------3
		|\    /|
		| \	 / |
		|  \/  |
		|  /\  |
		| /  \ |
		|/    \|
		1------2
		input:
		5 8
		1 2
		1 3
		1 5
		2 3
		2 5
		3 4
		3 5
		4 5
		output:
		1-->2-->3-->4-->5-->1
		1-->2-->5-->4-->3-->1
		1-->3-->4-->5-->2-->1
		1-->5-->4-->3-->2-->1
		2-->1-->3-->4-->5-->2
		2-->1-->5-->4-->3-->2
		2-->3-->4-->5-->1-->2
		2-->5-->4-->3-->1-->2
		3-->1-->2-->5-->4-->3
		3-->2-->1-->5-->4-->3
		3-->4-->5-->1-->2-->3
		3-->4-->5-->2-->1-->3
		4-->3-->1-->2-->5-->4
		4-->3-->2-->1-->5-->4
		4-->5-->1-->2-->3-->4
		4-->5-->2-->1-->3-->4
		5-->1-->2-->3-->4-->5
		5-->2-->1-->3-->4-->5
		5-->4-->3-->1-->2-->5
		5-->4-->3-->2-->1-->5
	*/
	if (1)
	{
		int n, m, u, v;
		Graph::Graph_Matrix<int> G;
		std::cout << "请输入点的个数和边的个数" << std::endl;
		std::cin >> n >> m;
		for (int i = 0; i < n; i++)
			G.Insert(i);
		std::cout << "请输入边" << std::endl;
		for (int i = 1; i <= m; i++)
		{
			std::cin >> u >> v;
			//	题目编号从1开始，且要求连接为1
			G.Insert(u - 1, v - 1, 1);
			G.Insert(v - 1, u - 1, 1);
		}
		std::cout << "邻接矩阵" << std::endl;
		G.Print_Matrix();
		std::vector<std::vector<bool> > visited;
		std::vector<std::vector<int> > Answer;
		std::vector<int> Path;

		bool Find = false;
		std::cout << "一笔画完的顺序为:" << std::endl;
		for (int i = 0; i < n; i++)
		{
			visited.clear();
			Answer.clear();
			Path.clear();
			visited.resize(n);
			for (int j = 0; j < n; j++)
				visited[j].resize(n);
			for (int j = 0; j < n; j++)
				for (int k = 0; k < n; k++)
					visited[j][k] = false;

			Path.push_back(i);
			Euler(G, i, 1, visited, Path, Answer);
			Path.pop_back();

			
			for (size_t j = 0; j < Answer.size(); j++)
			{
				assert(Answer[j].size() == n);
				Find = true;
				for (int k = 0; k < n; k++)
					std::cout << Answer[j][k] + 1 << "-->";
				std::cout << Answer[j][0] + 1 << std::endl;
			}
		}
		if (!Find)
			std::cout << "未找到结果" << std::endl;
	}
	return 0;
}

