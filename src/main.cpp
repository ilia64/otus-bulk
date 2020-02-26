#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "program expect bulk size argument!" << std::endl;
        return EXIT_FAILURE;
    }
}
