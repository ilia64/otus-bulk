#include <iostream>
#include "Bulk.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "program expect bulk size argument!" << std::endl;
        return EXIT_FAILURE;
    }

    const int size = std::stoi(argv[1]);

    Bulk bulk{static_cast<size_t >(size)};

    LogWriter logWriter;
    FileWriter fileWriter;

    bulk.subscribe(&logWriter);
    bulk.subscribe(&fileWriter);

    bulk.run();
}
