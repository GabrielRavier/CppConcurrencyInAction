#include <iostream>
#include <thread>

using std::cout;

void hello()
{
	cout << "Hello, Concurrent World !\n";
}

int main()
{
	std::thread t(hello);
	t.join();
}