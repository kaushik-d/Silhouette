#include <iostream>
#include <filesystem>

#include "STLReader.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error("Insuccificent arguments. Specify an STL file name.\n Usage: test stl_file_name [num_threads]");
    }

    std::filesystem::path file_name(argv[1]);

    unsigned int number_threads(6);
    if (argc > 2)
    {
        number_threads = std::stod(argv[2]);
    }

    using Clock = std::chrono::high_resolution_clock;

    // 1. Read in triangles. Each triangles has 3 vertex. There will be common vertex among triangles

    double total_time(0);
    auto start = Clock::now();

    auto triangle_list = STL::readBinary(file_name);

    std::chrono::nanoseconds duration = Clock::now() - start;
    std::cout << "Reading file takes " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    STL::check_normals(triangle_list);

    duration = Clock::now() - start;
    std::cout << "Checking normals takes " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    return 0;
}