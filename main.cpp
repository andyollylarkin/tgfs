#include <iostream>
#include "fs_entry.hpp"
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{

	for (auto p : split_by_delim("hello,world,suka", ','))
	{
		std::cout << p << std::endl;
	};
}