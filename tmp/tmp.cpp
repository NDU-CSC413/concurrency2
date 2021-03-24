/*****************************************************************//**
 * @file   concurrency2.6.cpp
 * @brief  condition variables
 * @author Hikmat Farhat
 * @date   February 2021
 *********************************************************************/

int f() {
	return 2;
}
struct Test {
	int operator()(int x) {
		return 2*x;
	}
};
int main() {
	Test t;
	int x=f();
	int y = t(4);
}
