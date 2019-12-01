#include <iostream>
#include <string>
#include <cstring>
namespace Hash_Table
{
	template<typename K, typename V>
	class Entry
	{
	public:
		K _key;
		V _value;
		Entry(K key, V value) :_key(key), _value(value) {};
		Entry(Entry<K, V> const& e) :_key(e._key), _value(e._value) {};
		bool operator < (const Entry<K, V>& E) { return _key < E._key; }
		bool operator > (const Entry<K, V>& E) { return _key > E._key; }
		bool operator == (const Entry<K, V>& E) { return _key == E._key; }
		bool operator != (const Entry<K, V>& E) { return _key != E._key; }
		bool operator >= (const Entry<K, V>& E) { return _key >= E._key; }
		bool operator <= (const Entry<K, V>& E) { return _key <= E._key; }
	};

	template <typename K, typename V>
	class Dictionary
	{
		virtual int size() = 0;
		virtual bool put(K, V) = 0;
		virtual V* get(K) = 0;
		virtual bool remove(K k) = 0;
	};

	template <typename K, typename V>
	class Hash_Table :public Dictionary<K, V>
	{
	private:
		Entry<K, V>** _arr;
		int _prime;
		int _num;

	public:
		//protected:
		int Search_Hit(const K& k);
		int Count_Hit(const K& k);
		int Search_Free(const K& k);
		void ReHash();
	public:
		Hash_Table(int Col = 5, int Prime = 10007);
		~Hash_Table();
		int size() { return _num; }
		bool put(K, V);
		V* get(K);
		bool remove(K);
		size_t Hash_Code(std::string S);
	};


	template<typename K, typename V>
	int Hash_Table<K, V>::Search_Hit(const K& k)
	{
		int r = Hash_Code(k) % _prime;
		while ((_arr[r] && (k != _arr[r]->_key)))
			r = (r + 1) % _num;
		return r;
	}

	template<typename K, typename V>
	int Hash_Table<K, V>::Count_Hit(const K& k)
	{
		int r = Hash_Code(k) % _prime;
		int cnt = 1;
		while ((_arr[r] && k != _arr[r]->_key))
			r = (r + 1) % _num, cnt++;
		return cnt;
	}

	template<typename K, typename V>
	int Hash_Table<K, V>::Search_Free(const K& k)
	{
		int r = Hash_Code(k) % _prime;
		while (_arr[r])
			r = (r + 1) % _num;
		return r;
	}

	template<typename K, typename V>
	void Hash_Table<K, V>::ReHash()
	{
		Entry<K, V>** old_arr = _arr;
		int old_prime = _prime;
		//	此处需要素数表
		_prime = _prime << 1 | 1;
		_arr = new Entry<K, V> * [_num];
		for (int i = 0; i < old_prime; i++)
			if (old_arr[i])
			{
				put(old_arr[i]._key, old_arr[i]._value);
				delete old_arr[i];
			}
		delete _arr;
	}

	template <typename K, typename V>
	Hash_Table<K, V>::Hash_Table(int Num, int Prime)
	{
		_prime = Prime;
		_num = Num;
		_arr = new Entry<K, V> * [Num];
		memset(_arr, 0, sizeof(Entry<K, V>*) * Num);
	}
	template<typename K, typename V>
	Hash_Table<K, V>::~Hash_Table()
	{
		for (int i = 0; i < _num; i++)
			if (_arr[i])
				delete _arr[i];
		delete _arr;
	}
	template<typename K, typename V>
	bool Hash_Table<K, V>::put(K k, V v)
	{
		//	已经在集合中不用再插入
		if (_arr[Search_Hit(k)])
			return false;
		int r = Search_Free(k);
		_arr[r] = new Entry<K, V>(k, v);
		return true;
		//	题目中不需要
		//if (_num * 2 > _prime)
		//	ReHash();
	}
	template<typename K, typename V>
	V* Hash_Table<K, V>::get(K k)
	{
		int e = Search_Hit(k);
		return _arr[e] ? &(_arr[e]->_value) : NULL;
	}
	template<typename K, typename V>
	bool Hash_Table<K, V>::remove(K k)
	{
		int r = Search_Hit(k);
		if (!_arr[r])
			return false;
		delete _arr[r];
		_arr[r] = NULL;
		return true;
	}
	template<typename K, typename V>
	size_t Hash_Table<K, V>::Hash_Code(std::string S)
	{
		const size_t Base = 37;
		size_t Mul = 1;
		size_t Res = 0;
		for (int i = S.length() - 1; i >= 0; i--)
		{
			if (S[i] >= 'a' && S[i] <= 'z')
				Res = (Res + Mul * (S[i] - 'a' + 1)) % _prime;
			else
				Res = Res + Mul * (S[i] - 'A' + 1) % _prime;
			Mul = (Mul * Base) % _prime;
		}
		return Res;
	}
}

int main()
{
	int n, m, p;
	std::string name[200];

	std::cin >> n >> p >> m;
	Hash_Table::Hash_Table<std::string, int> Hash(m, p);
	for (int i = 1; i <= n; i++)
	{
		std::cin >> name[i];
		Hash.put(name[i], i);
	}
	for (int i = 1; i <= n; i++)
		std::cout << Hash.Search_Hit(name[i]) << " ";
	std::cout << std::endl;
	for (int i = 1; i <= n; i++)
		std::cout << Hash.Count_Hit(name[i]) << " ";
	std::cout << std::endl;
	return 0;
}