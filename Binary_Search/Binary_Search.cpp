#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>

namespace Search
{
	template<class ForwardIt, class T, class Compare>
	ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp)
	{
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first, last);

		while (count > 0) 
		{
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (comp(*it, value)) 
			{
				first = ++it;
				count -= step + 1;
			}
			else
				count = step;
		}
		return first;
	}
};

int main()
{
	int n, e, key;
	std::vector<int> data;
	data.clear();
	std::cin >> n;
	for (auto i = 0; i < n; i++)
	{
		std::cin >> e;
		data.push_back(e);
	}
	int KASE;
	std::cin >> KASE;
	bool Greater = false;
	if (data.size() >= 2 && data[0] > data[data.size() - 1])
		Greater = true;

	for (auto Kase = 1; Kase <= KASE; Kase++)
	{
		std::cin >> key;
		std::vector<int>::iterator lower;
		if (Greater)
			lower = Search::lower_bound(data.begin(), data.end(), key, std::greater<int>());
		else
			lower = Search::lower_bound(data.begin(), data.end(), key, std::less<int>());

		if (lower != data.end())
			std::cout << lower - data.begin() << std::endl;
		else
			std::cout << "-1" << std::endl;
	}
	return 0;
}