# Quick Start

This guide provides a rapid introduction to the **HGL** (Hypergraph) module. In just a few minutes, you will define a hypergraph, populate it with vertices and weighted hyperedges, and run a Breadth-First B-Search (B-BFS) algorithm to evaluate reachability across its generalized graph topology.

## Basic Usage Example

The following C++ program demonstrates the standard workflow of the CPP-GL hypergraph library:

1. **Defining Traits:** Selecting the directionality model, memory layout, and payload types.
2. **Building Topology:** Adding vertices and defining hyperedges that map sets of tail vertices to sets of head vertices.
3. **Execution:** Running a decoupled B-reachability traversal algorithm.
4. **Reconstruction & Output:** Querying the resulting search tree and formatting the hypergraph natively to the standard output.

```cpp
#include <hgl/hypergraph.hpp> // (1)!
#include <hgl/algorithm/traversal/backward_search.hpp>
#include <hgl/io/core.hpp>

#include <iostream>
#include <vector>

int main() {
    using traits_t = hgl::hypergraph_traits< // (2)!
        hgl::bf_directed_t,
        gl::name_property,
        gl::weight_property<double>,
        hgl::repr::list_t<hgl::repr::bidirectional_t>>;

    hgl::hypergraph<traits_t> hg(4); // (3)!

    auto v0 = hg.vertex(0); v0->name = "Source"; // (4)!
    auto v1 = hg.vertex(1); v1->name = "Router A";
    auto v2 = hg.vertex(2); v2->name = "Router B";
    auto v3 = hg.vertex(3); v3->name = "Target";

    hg.add_hyperedge({0}, {1, 2})->weight = 10.5; // (5)!
    hg.add_hyperedge({v1, v2}, {v3})->weight = 5.0;

    std::vector<hgl::default_id_type> roots = {0u};
    auto search_tree = hgl::algorithm::backward_bfs(hg, roots); // (6)!

    if (search_tree.is_reachable(3u)) // (7)!
        std::cout << "Target is B-reachable from the Source.\n\n";
    else
        std::cout << "Target is NOT B-reachable from the Source.\n\n";

    std::cout << hgl::io::verbose << hgl::io::with_properties; // (8)!
    std::cout << "Hypergraph Topology:\n" << hg << '\n';

    return 0;
}
```

1. Include the necessary core hypergraph, algorithm, and I/O headers.
2. Define the hypergraph's memory layout (bidirectional incidence list), directionality, and element payloads via a single traits structure. <br/> **NOTE:** Alternatively, you can use one of the provided [generic hypergraph type aliases](../cpp-gl/group__HGL-Core.md#public-types).
3. Instantiate the generic `hypergraph` class, pre-allocating it with 4 vertices (IDs 0 through 3), using the defined traits.
4. Retrieve stable vertex descriptors using `.vertex(id)` to assign property payloads safely.
5. BF-directed hyperedges connect a set of *tail* vertices to a set of *head* vertices. An edge can be added using vertex IDs or descriptors. <br/> **NOTE:** You can alternatively instantiate the hypergraph with a given number of hyperedges and simply *bind* vertices to them using the dedicated binding methods. <br/> **IMPORTANT:** Property Access Safety <br/> Assigning properties inline via the returned descriptor is safe here because the temporary descriptor is immediately discarded, meaning no dangling references are kept. The same bahaviour can be achieved using the `add_hyperedge_with` methods.
6. Execute a Breadth-First Backward Search (B-BFS) to compute B-reachability semantics from the source vertex.
7. Query the resulting search tree to validate if the target vertex was successfully reached.
8. Standard GL stream manipulators inject persistent formatting state to output the hypergraph's structure in a verbose format, including the element properties.

**Output:**

```text
Target is B-reachable from the Source.

Hypergraph Topology:
type: BF-directed, |V| = 4, |E| = 2
vertices:
  - [id: 0 | "Source"]
  - [id: 1 | "Router A"]
  - [id: 2 | "Router B"]
  - [id: 3 | "Target"]
hyperedges:
  - [id: 0 | tail: {0}, head: {1, 2} | 10.5]
  - [id: 1 | tail: {1, 2}, head: {3} | 5]
```

## Understanding the Code

- **Generic Traits:** Similar to the standard GL module, the HGL module relies entirely on template abstraction. By passing a [**hgl::hypergraph_traits**](../cpp-gl/structhgl_1_1hypergraph__traits.md) struct to the [**hgl::hypergraph**](../cpp-gl/classhgl_1_1hypergraph.md) constructor, you dictate whether the hypergraph is undirected or BF-directed ([**hgl::bf_directed_t**](../cpp-gl/structhgl_1_1bf__directed__t.md)), what custom properties exist on vertices and hyperedges, and which underlying memory architecture it utilizes (e.g., [**hgl::repr::list_t**](../cpp-gl/structhgl_1_1repr_1_1list__t.md)).

- **Generalized Incidence:** Unlike standard graphs where an edge connects exactly two vertices, hyperedges connect sets of vertices of arbitrary sizes. In a BF-directed hypergraph, `add_hyperedge` accepts an iterable range of *tail* (source) vertices and *head* (destination) vertices and represents a single connection from all tail vertices to all head vertices simultaneously.

- **Property Access:** Because the internal memory can shift during insertion, it is best practice to pre-allocate memory for vertices and hyperedges in the hypergraph's constructor and then fetch them using accessors like `.vertex(id)`. However, you access specific payload fields directly, e.g. through the overloaded `->` operator on the returned descriptor.

- **Algorithm Separation:** Algorithms exist entirely outside the hypergraph class. In this example, [**hgl::algorithm::backward_bfs**](../cpp-gl/group__HGL-Algorithm.md#function-backward_bfs) applies strict B-reachability rules: a hyperedge is only fully traversed (and its head vertices discovered) if all of its tail vertices have already been visited. The [**hgl::algorithm::is_reachable**](../cpp-gl/group__HGL-Algorithm.md#function-is_reachable) utility then checks the resulting search tree to evaluate whether the Target vertex is B-reachable in the defined hypergraph in $O(1)$ time.

- **Formatting:** The HGL module provides custom stream manipulators (e.g. [**hgl::io::verbose**](../cpp-gl/group__HGL-IO.md#variable-verbose), [**hgl::io::with_properties**](../cpp-gl/group__HGL-IO.md#variable-with_properties)) allowing you to effortlessly format or serialize the complex topology and payloads straight to `std::cout` or other I/O streams.
