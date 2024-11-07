#include<iostream>
#include<glad/glad.h>
int main(int argc, char* argv[])
{
	if (!gladLoadGLLoader((GLADloadproc)nullptr)) {
		std::cout << "66" << std::endl;
	}
	return 0;
}
