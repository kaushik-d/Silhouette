#include <iostream>
#include "Declustering.h"
#include "graph.h"

std::vector<STL::Triangle> Declustering::get_outter_shell(const Triangles &triangles)
{
    Graph graph_clustered(triangles.num_nodes());

    auto &connectivity = triangles.connectivity();
    for (auto &triangle : connectivity)
    {
        graph_clustered.addEdge(triangle[2], triangle[0], triangle[1]);
        graph_clustered.addEdge(triangle[0], triangle[1], triangle[2]);
        graph_clustered.addEdge(triangle[1], triangle[2], triangle[0]);
    }

    std::vector<std::vector<int>> graph_components = graph_clustered.getConnectedComponents();
    auto &node_coords = triangles.vertex_coordinates();
    int num_component(0);

    if (graph_components.size() == 1)
    {
        std::cout << "There is only one shell in this solid." << std::endl;
    }
    else
    {
        std::cout << "There are " << graph_components.size() << " shells in this solid." << std::endl;
    }

    // convert each components, each componets are an edge. Convert these component matrix to connectivity matrix
    // convert each connectivity matrix to list of triangles suitable to writing to STL files.
    // Libraries like OpenMesh can do that too.

    std::vector<std::vector<STL::Triangle>> shells;
    std::vector<STL::Triangle> outter_shells;

    for (auto &graph_component : graph_components)
    {
        std::set<int> component_set(graph_component.begin(), graph_component.end());
        std::vector<STL::Triangle> triangle_list;
        num_component++;
        for (auto &triangle_connectivity : connectivity)
        {

            bool triangle_in_set(false);
            for (auto &node : triangle_connectivity)
            {
                if (component_set.find(node) != component_set.end())
                {
                    triangle_in_set = true;
                    break;
                }
            }

            if (triangle_in_set)
            {
                STL::Triangle stl_triangle;
                stl_triangle.normal = Eigen::Vector3d{0, 0, 0};

                for (int i = 0; i < 3; i++)
                {
                    stl_triangle.vertex_list[i] = node_coords[triangle_connectivity[i]];
                }
                triangle_list.push_back(stl_triangle);
            }
        }

        shells.push_back(triangle_list);
    }

    int iShellCount(0);
    for (auto &shell : shells)
    {
        iShellCount++;

        double shell_vol(0);
        for (auto &triangle : shell)
        {
            auto &v1 = triangle.vertex_list[0];
            auto &v2 = triangle.vertex_list[1];
            auto &v3 = triangle.vertex_list[2];

            shell_vol += (1.0 / 6.0) * (-v3[0] * v2[1] * v1[2] + v2[0] * v3[1] * v1[2] + v3[0] * v1[1] * v2[2] - v1[0] * v3[1] * v2[2] - v2[0] * v1[1] * v3[2] + v1[0] * v2[1] * v3[2]);
        }

        std::cout << "Volumn of shell #" << iShellCount << " is " << shell_vol << std::endl;

        if(shell_vol > 0)
        {
            outter_shells.insert(outter_shells.end(), shell.begin(), shell.end());
        }
        shell.clear();
    }

    std::cout << "There are " << outter_shells.size() << " triangles on outter shells " << std::endl;

    return outter_shells;
}