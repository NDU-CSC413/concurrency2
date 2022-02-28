#include <vector>
#include <list>

int main() {

	std::vector<int> v{ 1,2,3,4 };
	std::list<int> l{ 10,11,12,13 };
	auto vb = v.begin();
	auto itr = vb + 2;
	auto lb = l.begin();
	std::list<int>::iterator  p=lb;
	std::advance(p, 2);

}