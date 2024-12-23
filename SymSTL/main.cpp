#include <iostream>
#include "Vector.h"
#include<vector>
#include<algorithm>
using namespace sym::stl;

int main() {

	Vector<int> v;

	v.push_back(3);
	v.push_back(2);
	v.push_back(1);

	v.show();
	v.reserve(5);
	v.show();

	v.resize(6);
	v.show();
	std::cout << v.at(2) << std::endl;
	std::cout << v[2] << std::endl;
	v.show();
	
	Vector<int> v2 = {4, 5, 6};
	v.show();
	v2[1] = 100;
	v2.show();
	std::cout << "########################################################" << std::endl;
	for (const auto& a : v) {
		std::cout << a << std::endl;
	}
	std::cout << "########################################################" << std::endl;
	for (Vector<int>::Iterator it = v.begin(); it != v.end(); it++) {
		std::cout << *it << std::endl;
	}
	std::cout << "########################################################" << std::endl;

	// 自定义的容器支持标准库的sort()
	std::sort(v.begin(), v.end());
	
	for (const auto& a : v) {
		std::cout << a << std::endl;
	}
	std::cout << "########################################################" << std::endl;
	for (Vector<int>::Iterator it = v.end() - 1; it != v.begin() - 1; it--) {
		std::cout << *it << std::endl;
	}
	std::cout << "########################################################" << std::endl;
	v.show();
	v2.show();
	std::cout << "########################################################" << std::endl;
	std::cout << "test insert():" << std::endl;
	v.insert(v.begin() + 1, 2, 99);
	v.show();
	v.erase(v.begin() + 1);
	v.show();
	v.erase(v.find(1), v.find(3));
	v.show();

	return 0;
}