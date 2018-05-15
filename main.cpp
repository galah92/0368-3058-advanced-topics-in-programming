#include <iostream>
#include <sstream>
#include <string>
#include <vector>


int main(int argc, char *argv[])
{
	std::cout << "Hello World!" << std::endl;
	if (argc < 2)
	{
		std::cout << "Usage: ./ex2 <auto|file>-vs-<auto|file>" << std::endl;
		return -1;
	}
	std::stringstream ss(argv[1]);
	std::string ptype1, vs, ptype2;
	std::getline(ss, ptype1, '-');
	std::getline(ss, vs, '-');
	std::getline(ss, ptype2, '-');
	return 0;
}