#include "PolygonToVTK.h"

void vtk_utils::export_polygon(const mpolygon_t& multi_polygon, const std::filesystem::path &finame_name)
{
    std::vector<double> points_for_vtu;
    std::vector<double> contour_id; // 1 is part, 0 and negatives are for thermal regions
    std::vector<double> layer_id;
    std::vector<double> loop_area;
    // Vertex indices of all cells
    std::vector<vtu11::VtkIndexType> connectivity;

    // Separate cells in connectivity array
    std::vector<vtu11::VtkIndexType> offsets;

    // Cell types of each cell, see vtk doc. vertex is cell type 1
    std::vector<vtu11::VtkCellType> types;

    // const auto &layers = PCFBoostGeom_layers.get_layers();
    // int elem_num(0);
    int node_count(0);

    for (const auto &polygon : multi_polygon)
    {

        for (const auto pt : polygon.outer())
        {
            points_for_vtu.push_back(pt.get<0>());
            points_for_vtu.push_back(pt.get<1>());
            points_for_vtu.push_back(0);

            connectivity.push_back(node_count);
            node_count++;
        }
        offsets.push_back(node_count);
        types.push_back(4); // VTK_POLY_LINE (=4)
        contour_id.push_back(0);

        for (const auto &polygon_inner : polygon.inners())
        {
            for (const auto pt : polygon_inner)
            {
                points_for_vtu.push_back(pt.get<0>());
                points_for_vtu.push_back(pt.get<1>());
                points_for_vtu.push_back(0);

                connectivity.push_back(node_count);
                node_count++;
            }
            offsets.push_back(node_count);
            types.push_back(4); // VTK_POLY_LINE (=4)
            contour_id.push_back(1);
        }
    }

    vtu11::Vtu11UnstructuredMesh mesh{points_for_vtu, connectivity, offsets, types};

    std::vector<vtu11::DataSetInfo> dataSetInfo{
        {"Polygon_type", vtu11::DataSetType::CellData, 1}};

    auto folder = std::filesystem::current_path();

    auto finame_name_local = folder / finame_name;
    finame_name_local.make_preferred();
    vtu11::writeVtu(finame_name_local.string(), mesh, dataSetInfo, {contour_id}, "rawbinary" /*"Ascii"*/);
}