#include "extractor.hpp"

#include <fstream>
#include <iostream> 

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file>" << std::endl;
        return 0;
    }

    std::string vfs_filename = argv[1];

    LP1CExtractor extractor(vfs_filename);

    extractor.extract_all();
}