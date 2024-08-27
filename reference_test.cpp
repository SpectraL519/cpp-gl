#include <vector>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include <gl/types/iterator_range.hpp>

using id_type = std::uint64_t;
using size_type = std::uint64_t;

class vertex {
public:
    vertex() = delete;
    vertex(const vertex&) = delete;
    vertex& operator=(const vertex&) = delete;

    vertex(id_type id) : _id(id) {}

    vertex(vertex&&) = default;
    vertex& operator=(vertex&&) = default;

    ~vertex() = default;

    id_type id() const {
        return this->_id;
    }

private:
    id_type _id;
};

using vertex_ptr_type = std::unique_ptr<vertex>;

class edge {
public:
    edge() = delete;
    edge(const edge&) = delete;
    edge& operator=(const edge&) = delete;

    edge(const vertex_ptr_type& first, const vertex_ptr_type& second)
    : _vertices(first, second) {}

    edge(edge&&) = default;
    edge& operator=(edge&&) = default;

    ~edge() = default;

    const vertex& first() const {
        return *this->_vertices.first;
    }

    const vertex& second() const {
        return *this->_vertices.second;
    }

private:
    std::pair<const vertex_ptr_type&, const vertex_ptr_type&> _vertices;
};

using edge_ptr_type = std::unique_ptr<edge>;

class graph {
public:
    graph(const graph&) = delete;
    graph& operator=(const graph&) = delete;

    graph() = default;

    graph(const size_type n_vertices) : _adj_list(n_vertices) {
        this->_vertices.reserve(n_vertices);

        id_type vertex_id = 0ull;
        std::ranges::generate_n(std::back_inserter(this->_vertices), n_vertices, [&vertex_id]() {
            return std::make_unique<vertex>(vertex_id++);
        });
    }

    graph(graph&&) = default;
    graph& operator=(graph&&) = default;

    ~graph() = default;

    const vertex& add_vertex() {
        this->_adj_list.push_back({});
        this->_vertices.push_back(std::make_unique<vertex>(this->_vertices.size()));
        return *this->_vertices.back();
    }

    auto vertices() const {
        return gl::make_const_iterator_range(this->_vertices);
    }

    const edge& add_edge(const id_type v1_id, const id_type v2_id) {
        this->_adj_list.at(v1_id).push_back(std::make_unique<edge>(this->_vertices[v1_id], this->_vertices.at(v2_id)));
        return *this->_adj_list[v1_id].back();
    }

    auto adjacent_edges(const vertex& v) const {
        return gl::make_const_iterator_range(this->_adj_list.at(v.id()));
    }

private:
    std::vector<vertex_ptr_type> _vertices{};
    std::vector<std::vector<edge_ptr_type>> _adj_list{};
};

int main() {
    constexpr id_type n_vertices{5ull};
    constexpr id_type v1_id = 0ull;
    constexpr id_type v2_id = 1ull;

    graph graph(n_vertices);

    const auto& edge_1 = graph.add_edge(v1_id, v2_id);
    std::cout << "edge_1& -> " << &edge_1
              << " [vertices = " << edge_1.first().id() << ", "
              << edge_1.second().id() << "]" << std::endl;
    std::cout << "all edges : ";
    for (const auto& edge : graph.adjacent_edges(v1_id))
        std::cout << edge.get() << " ";
    std::cout << "\n\n";

    const auto& edge_2 = graph.add_edge(v1_id, v2_id);
    std::cout << "added edge : " << &edge_2
              << " [vertices = " << edge_2.first().id() << ", " << edge_2.second().id() << "]" << std::endl;
    std::cout << "all edges : ";
    for (const auto& edge : graph.adjacent_edges(v1_id))
        std::cout << edge.get() << " ";
    std::cout << "\n\n";

    const auto& edge_3 = graph.add_edge(v1_id, v2_id);
    std::cout << "added edge : " << &edge_3
              << " [vertices = " << edge_3.first().id() << ", " << edge_3.second().id() << "]" << std::endl;
    std::cout << "all edges : ";
    for (const auto& edge : graph.adjacent_edges(v1_id))
        std::cout << edge.get() << " ";
    std::cout << "\n\n";

    std::cout << "edge_1& -> " << &edge_1
              << " [vertices = " << edge_1.first().id() << ", "
              << edge_1.second().id() << "]" << std::endl;
    std::cout << "added edge : " << &edge_2
              << " [vertices = " << edge_2.first().id() << ", " << edge_2.second().id() << "]" << std::endl;
    std::cout << "added edge : " << &edge_3
              << " [vertices = " << edge_3.first().id() << ", " << edge_3.second().id() << "]" << std::endl;
}
