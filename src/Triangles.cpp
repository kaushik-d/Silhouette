#include <iostream>
#include <array>
#include <functional>
#include <unordered_set>
#include <cmath>
#include <exception>
#include <thread>

#include "Triangles.h"
#include "STLReader.h"

void Triangles::convert_trianles_to_connectivity(std::vector<STL::Triangle> triangles, double tolerance, unsigned int num_threads)
{
    constexpr unsigned int max_num_threads = 6;
    if (num_threads > max_num_threads || num_threads > std::thread::hardware_concurrency())
    {
        num_threads = max_num_threads;
    }

    if (triangles.size() < 100)
    {
        num_threads = 1;
        std::cout << "Number of triangles is " << triangles.size() << ". Only one threads will be used. " << std::endl;
    }
    else
    {
        std::cout << "Number of triangles is " << triangles.size() << ". " << num_threads << " threads will be used. " << std::endl;
    }

    int elements_per_thread = triangles.size() / num_threads;
    int remaining_elements = triangles.size() % num_threads;

    std::unordered_set<Eigen::Vector3d, CoordinateHash, CoordinateEqual>
        unique_nodes(0, CoordinateHash(), CoordinateEqual(tolerance));

    std::vector<std::thread> threads;
    std::mutex g_mutex;

    auto merge_nodes = [&](int start, int end)
    {
        std::unordered_set<Eigen::Vector3d, CoordinateHash, CoordinateEqual>
            unique_nodes_this_thread(0, CoordinateHash(), CoordinateEqual(tolerance));

        for (int i = start; i < end; i++)
        {
            auto &triangle = triangles[i];
            for (auto &vertex : triangle.vertex_list)
            {
                auto it = unique_nodes_this_thread.find(vertex);
                if (it == unique_nodes_this_thread.end())
                {
                    unique_nodes_this_thread.insert(vertex);
                }
            }
        }

        g_mutex.lock();
        unique_nodes.insert(unique_nodes_this_thread.begin(), unique_nodes_this_thread.end());
        g_mutex.unlock();
    };

    for (unsigned int i = 0; i < num_threads; ++i)
    {
        int start = i * elements_per_thread;
        int end = start + elements_per_thread;
        if (i == num_threads - 1)
        {
            end += remaining_elements;
        }
        threads.emplace_back(merge_nodes, start, end);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    threads.clear();

    auto triangle_connectivity = [&](int start, int end)
    {
        std::vector<std::vector<int>> connectivity_this_thread;

        for (int i = start; i < end; i++)
        {
            auto &triangle = triangles[i];
            std::vector<int> connectivity;

            for (auto &vertex : triangle.vertex_list)
            {
                auto it = unique_nodes.find(vertex);
                if (it == unique_nodes.end())
                {
                    throw std::runtime_error("Something went wrong.");
                }
                else
                {
                    connectivity.push_back(std::distance(unique_nodes.begin(), it));
                }
            }

            connectivity_this_thread.push_back(connectivity);
        }

        g_mutex.lock();
        m_connectivity.insert(m_connectivity.end(), connectivity_this_thread.begin(), connectivity_this_thread.end());
        g_mutex.unlock();
    };

    for (unsigned int i = 0; i < num_threads; ++i)
    {
        int start = i * elements_per_thread;
        int end = start + elements_per_thread;
        if (i == num_threads - 1)
        {
            end += remaining_elements;
        }
        threads.emplace_back(triangle_connectivity, start, end);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    threads.clear();

    m_vertex_coordinates = std::vector<Eigen::Vector3d>(unique_nodes.begin(), unique_nodes.end());
}
