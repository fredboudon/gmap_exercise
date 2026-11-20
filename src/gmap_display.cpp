#include "gmap.hpp"
#include "gmap_viewer.hpp"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <limits>
#include <locale>
#include <stdexcept>

namespace {
struct Mesh {
    std::vector<vec3_t> vertices;
    std::vector<std::vector<std::size_t>> faces;
};

// Convert the 0-cells and the ordered boundaries of the 2-cells to an indexed mesh.
Mesh extract_mesh(const GMap3D& gmap)
{
    Mesh mesh;
    std::unordered_map<GMap::id_t, std::size_t> vertex_indices;

    for (auto dart : gmap.elements(0)) {
        const auto vertex = gmap.get_embedding_dart(dart);
        const auto position = gmap.get_position(vertex);
        if (!std::isfinite(position[0]) || !std::isfinite(position[1]) ||
            !std::isfinite(position[2])) {
            throw std::runtime_error("Coordonnées de sommet non finies.");
        }
        vertex_indices.emplace(vertex, mesh.vertices.size());
        mesh.vertices.push_back(position);
    }

    for (auto dart : gmap.elements(2)) {
        const auto boundary = gmap.orderedorbit({0, 1}, dart);
        if (boundary.size() < 6 || boundary.size() % 2 != 0) {
            throw std::runtime_error("Le contour d'une face doit contenir au moins trois paires de brins.");
        }
        mesh.faces.emplace_back();
        auto& face = mesh.faces.back();
        for (std::size_t i = 0; i < boundary.size(); i += 2) {
            face.push_back(vertex_indices.at(gmap.get_embedding_dart(boundary[i])));
        }
    }

    if (mesh.faces.empty()) {
        throw std::runtime_error("La G-carte ne contient aucune face à afficher.");
    }
    return mesh;
}

template<class Range, class WriteItem>
void write_array(std::ostream& output, const Range& items, WriteItem write_item)
{
    output << '[';
    const char* separator = "";
    for (const auto& item : items) {
        output << separator;
        write_item(item);
        separator = ",";
    }
    output << ']';
}
}

int display(const GMap3D& gmap)
{
    try {
        const auto mesh = extract_mesh(gmap);
        std::ofstream output("gmap-viewer.html");
        output.exceptions(std::ios::failbit | std::ios::badbit);
        output.imbue(std::locale::classic());
        output << std::setprecision(std::numeric_limits<float>::max_digits10);
        output << viewer_html << "<script>showMesh(";
        write_array(output, mesh.vertices, [&](const vec3_t& vertex) {
            output << '[' << vertex[0] << ',' << vertex[1] << ',' << vertex[2] << ']';
        });
        output << ',';
        write_array(output, mesh.faces, [&](const std::vector<std::size_t>& face) {
            write_array(output, face, [&](std::size_t index) { output << index; });
        });
        output << ");</script>\n";
        output.close();
    } catch (GMap::id_t dart) {
        std::fprintf(stderr, "Plongement manquant pour le brin %lu.\n", dart);
        return 1;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "Visualisation impossible : %s\n", error.what());
        return 1;
    }

    std::puts("Visualisation écrite dans gmap-viewer.html.");
    // The filename is fixed. No mesh data or user input enters the shell command.
#if defined(_WIN32)
    const int status = std::system("start \"\" \".\\gmap-viewer.html\"");
#elif defined(__APPLE__)
    const int status = std::system("open ./gmap-viewer.html");
#elif defined(__linux__)
    const int status = std::system("xdg-open ./gmap-viewer.html");
#else
    const int status = -1;
#endif
    if (status != 0) {
        std::fprintf(stderr, "Ouvre gmap-viewer.html manuellement dans ton navigateur.\n");
        return 1;
    }
    return 0;
}
