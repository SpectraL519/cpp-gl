# Graph topology generators

The `CPP-GL` library provides a set of graph topology generator functions, which are defined in the [gl/topologies.hpp](/include/gl/topologies.hpp) header file or in the specific header files in the [gl/topology/](/include/gl/topology/) directory.

<br />

## Table of content

- [The defined generators](#the-defined-generators)
- [Example](#example)
- [Related pages](#related-pages)

<br />
<br />

## The defined generators

> [!NOTE]
> All topology generator functions are defined in the `gl::topology` namespace.

- `clique(n_vertices)`
  - *Description*: Generates a [complete graph](https://en.wikipedia.org/wiki/Complete_graph) of the specified type
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
  - *Parameters*:
    - `n_vertices: types::size_type` - the number of vertices the graph will be initialized with
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/clique.hpp](/include/gl/topology/clique.hpp)

- `biclique(n_vertices_a, n_vertices_b)`
  - *Description*: Generates a [complete bipartite graph](https://en.wikipedia.org/wiki/Complete_bipartite_graph) of the specified type where the vertex sets are of sizes `n_vertices_a` and `n_vertices_b` respectively
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
  - *Parameters*:
    - `n_vertices_a: types::size_type` - the number of vertices of the first vertex set of the graph
    - `n_vertices_b: types::size_type` - the number of vertices of the second vertex set of the graph
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/bipartite.hpp](/include/gl/topology/bipartite.hpp)

- `path(n_vertices)`
  - *Description*: Generates a [path graph](https://en.wikipedia.org/wiki/Path_graph) of the specified type
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
  - *Parameters*:
    - `n_vertices: types::size_type` - the number of vertices the graph will be initialized with
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/path.hpp](/include/gl/topology/path.hpp)

- `bidirectional_path(n_vertices)`
  - *Description*: Generates a bidirectional [path graph](https://en.wikipedia.org/wiki/Path_graph) of the specified type:
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
    - For directed graphs: for each edge $(v_i, v_j)$ of a *normal* path graph, an additional edge is added - $(v_j, v_i)$
    - For undirected edges: equivalent to `path(n_vertices)`
  - *Parameters*:
    - `n_vertices: types::size_type` - the number of vertices the graph will be initialized with
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/path.hpp](/include/gl/topology/path.hpp)

- `cycle(n_vertices)`
  - *Description*: Generates a [cycle graph](https://en.wikipedia.org/wiki/Cycle_graph) of the specified type
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
  - *Parameters*:
    - `n_vertices: types::size_type` - the number of vertices the graph will be initialized with
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/path.hpp](/include/gl/topology/path.hpp)

- `bidirectional_cycle(n_vertices)`
  - *Description*: Generates a bidirectional [cycle graph](https://en.wikipedia.org/wiki/Cycle_graph) of the specified type:
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
    - For directed graphs: for each edge $(v_i, v_j)$ of a *normal* cycle graph, an additional edge is added - $(v_j, v_i)$
    - For undirected edges: equivalent to `cycle(n_vertices)`
  - *Parameters*:
    - `n_vertices: types::size_type` - the number of vertices the graph will be initialized with
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/cycle.hpp](/include/gl/topology/cycle.hpp)

- `regular_binary_tree(depth)`
  - *Description*: Generates a [regular binary tree](#regular_dary_tree) of the specified type
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
  - *Parameters*:
    - `depth: types::size_type` - the leaf depth of the generated binary tree graph
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/binary_tree.hpp](/include/gl/topology/binary_tree.hpp)

- `bidirectional_regular_binary_tree(depth)`
  - *Description*: Generates a bidirectional [regular binary tree](#regular_dary_tree) of the specified type:
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the generated graph
    - For directed graphs: for each edge $(v_i, v_j)$ of a *normal* regular binary tree, an additional edge is added - $(v_j, v_i)$
    - For undirected edges: equivalent to `regular_binary_tree(n_vertices)`
  - *Parameters*:
    - `depth: types::size_type` - the leaf depth of the generated binary tree graph
  - *Return type*: `GraphType`
  - *Defined in*: [gl/topology/binary_tree.hpp](/include/gl/topology/binary_tree.hpp)

> [!NOTE]
> <a id="regular_dary_tree"></a>
> A **regular d-ary tree** is a d-ary tree such that all nodes have exaclty 0 or d children and all leaves of such tree are at the same depth/level.

<br />
<br />

## Example

The program below demostrates the graphs returned by the topology generator functions for a *directed* graph type:

```cpp
#include <gl/graph.hpp>
#include <gl/topologies.hpp>

template <gl::type_traits::c_graph GraphType>
void print_graph(const GraphType& graph, const std::string& name) {
    std::cout << "> " << name << ":\n" << graph << std::endl;
}

int main() {
    // define the graph type
    using graph_traits = gl::graph_traits<gl::directed_t, gl::types::name_property, gl::types::name_property>;
    using graph_type = gl::graph<graph_traits>;

    // clique of size 3
    const auto clique = gl::topology::clique<graph_type>(3);
    print_graph(clique, "clique");

    // biclique: |A| = 2, |B| = 3
    const auto biclique = gl::topology::biclique<graph_type>(2, 3);
    print_graph(biclique, "biclique");

    // path graphs of size 5
    const auto path = gl::topology::path<graph_type>(5);
    print_graph(path, "path");

    const auto bidir_path = gl::topology::bidirectional_path<graph_type>(5);
    print_graph(bidir_path, "bidirectional_path");

    // cycle graphs of size 5
    const auto cycle = gl::topology::cycle<graph_type>(5);
    print_graph(cycle, "cycle");

    const auto bidir_cycle = gl::topology::bidirectional_cycle<graph_type>(5);
    print_graph(bidir_cycle, "bidirectional_cycle");

    // regular binary tree graphs of depth 3
    const auto bin_tree = gl::topology::regular_binary_tree<graph_type>(3);
    print_graph(bin_tree, "regular_binary_tree");

    const auto bidir_bin_tree = gl::topology::bidirectional_regular_binary_tree<graph_type>(3);
    print_graph(bidir_bin_tree, "bidirectional_regular_binary_tree");
}
```

The output of this program is:

```txt
> clique:
directed 3 6
- 0 : [0, 1] [0, 2]
- 1 : [1, 0] [1, 2]
- 2 : [2, 0] [2, 1]

> biclique:
directed 5 12
- 0 : [0, 2] [0, 3] [0, 4]
- 1 : [1, 2] [1, 3] [1, 4]
- 2 : [2, 0] [2, 1]
- 3 : [3, 0] [3, 1]
- 4 : [4, 0] [4, 1]

> path:
directed 5 4
- 0 : [0, 1]
- 1 : [1, 2]
- 2 : [2, 3]
- 3 : [3, 4]
- 4 :

> bidirectional_path:
directed 5 8
- 0 : [0, 1]
- 1 : [1, 0] [1, 2]
- 2 : [2, 1] [2, 3]
- 3 : [3, 2] [3, 4]
- 4 : [4, 3]

> cycle:
directed 5 5
- 0 : [0, 1]
- 1 : [1, 2]
- 2 : [2, 3]
- 3 : [3, 4]
- 4 : [4, 0]

> bidirectional_cycle:
directed 5 10
- 0 : [0, 1] [0, 4]
- 1 : [1, 0] [1, 2]
- 2 : [2, 1] [2, 3]
- 3 : [3, 2] [3, 4]
- 4 : [4, 3] [4, 0]

> regular_binary_tree:
directed 7 6
- 0 : [0, 1] [0, 2]
- 1 : [1, 3] [1, 4]
- 2 : [2, 5] [2, 6]
- 3 :
- 4 :
- 5 :
- 6 :

> bidirectional_regular_binary_tree:
directed 7 12
- 0 : [0, 1] [0, 2]
- 1 : [1, 0] [1, 3] [1, 4]
- 2 : [2, 0] [2, 5] [2, 6]
- 3 : [3, 1]
- 4 : [4, 1]
- 5 : [5, 2]
- 6 : [6, 2]
```

<br />
<br />

## Related pages

- [The graph class - library's core](/docs/graph.md)
- [I/O operations](/docs/io.md)
- [Core utility types](/docs/core_util_types.md)
