#include <iostream>
#include <filesystem>

#include "STLReader.h"
#include "FindVisible.h"
#include "PolygonToVTK.h"
#include "CmdLine.h"

int main(int argc, char *argv[])
{
    std::filesystem::path file_name(argv[1]);
    Eigen::Vector3d shadow_plane_normal(0, 0, 1);
    Eigen::Vector3d shadow_plane_origin(0, 0, 0);

    try {
		CmdLine& cmd = CmdLine::Instance();
		if (!cmd.Initialize(argc, argv)) {
			exit(EXIT_FAILURE);
		}

		file_name = cmd.getString("s");
		file_name.make_preferred();

        shadow_plane_normal = Eigen::Vector3d(cmd.getDouble("nx"),cmd.getDouble("ny"),cmd.getDouble("nz"));
        shadow_plane_origin = Eigen::Vector3d(cmd.getDouble("ox"),cmd.getDouble("oy"),cmd.getDouble("oz"));

        shadow_plane_normal.normalize();
		
	}
	catch (...) {
		std::cout << "Error during command line parsing." << std::endl ;
	}

    if (argc < 2)
    {
        throw std::runtime_error("Insuccificent arguments. Specify an STL file name.\n Usage: test stl_file_name [num_threads]");
    }

    unsigned int number_threads(6);


    using Clock = std::chrono::high_resolution_clock;

    // 1. Read in triangles. Each triangles has 3 vertex. There will be common vertex among triangles

    double total_time(0);
    auto start = Clock::now();

    auto triangle_list = STL::readBinary(file_name);

    std::chrono::nanoseconds duration = Clock::now() - start;
    std::cout << "Reading file takes " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    start = Clock::now();
    STL::check_normals(triangle_list);

    duration = Clock::now() - start;
    std::cout << "Checking normals takes " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    start = Clock::now();

    auto visible_triangles = FindVisible::get_visible_list(triangle_list, shadow_plane_normal);

    std::cout << "Among " << triangle_list.size() << " triangles " << visible_triangles.size() << " are visible." << std::endl;

    STL::writeBinary("./visiable.stl", visible_triangles);

    duration = Clock::now() - start;
    std::cout << "Getting visible list takes " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    start = Clock::now();

    FindVisible::project_tringles(visible_triangles, shadow_plane_normal, shadow_plane_origin);

    STL::writeBinary("./projected.stl", visible_triangles);

    duration = Clock::now() - start;
    std::cout << "Projecting triangles takes " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    start = Clock::now();

    auto shadow_polygon = FindVisible::get_shadow(visible_triangles, shadow_plane_normal, shadow_plane_origin);

    std::cout << "Shadow has an " << shadow_polygon.size() << " polygons. " << std::endl;

    duration = Clock::now() - start;
    std::cout << "Getting shadow " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    start = Clock::now();

    vtk_utils::export_polygon(shadow_polygon, shadow_plane_normal, shadow_plane_origin);

    duration = Clock::now() - start;
    std::cout << "Exporting shadow " << duration.count() / 1e6 << " milliseconds." << std::endl;
    total_time += duration.count() / 1e6;

    return 0;
}