# Quick Start

This guide provides a rapid introduction to the **GL** module. In just a few minutes, you will define a graph, populate it with vertices and weighted edges, and run Dijkstra's shortest path algorithm.

## Basic Usage Example

The following C++ program demonstrates the standard workflow of the CPP-GL library:

1. **Defining Traits:** Selecting the memory layout, directionality, and payloads.
2. **Building Topology:** Adding vertices and edges.
3. **Execution:** Running a decoupled algorithm.
4. **Reconstruction:** Extracting and formatting the results.

```cpp
#include <gl/graph.hpp> // (1)!
#include <gl/algorithm.hpp>
#include <gl/io.hpp>

#include <iostream>

int main() {
    using traits_t = gl::graph_traits< // (2)!
        gl::directed_t,
        gl::empty_properties,
        gl::weight_property<int>,
        gl::repr::list_t,
        gl::api::strict_t,
        std::uint8_t>;

    gl::graph<traits_t> graph(5); // (3)!

    graph.add_edge(0, 1)->weight = 10; // (4)!
    graph.add_edge(0, 2)->weight = 1;
    graph.add_edge(2, 3)->weight = 2;
    graph.add_edge(3, 1)->weight = 4;
    graph.add_edge(1, 4)->weight = 2;

    const auto source_id = 0u;
    auto paths = gl::algorithm::dijkstra_shortest_paths(graph, source_id); // (5)!

    const auto target_id = 4u;
    if (not gl::algorithm::is_reachable(paths.predecessors, target_id)) { // (6)!
        std::cerr << "Vertex " << target_id << " is unreachable.\n";
        return 1;
    }

    auto path_to_target
        = gl::algorithm::reconstruct_path(paths.predecessors, target_id); // (7)!
    std::cout << "Shortest path distance to vertex " << target_id << ": "
              << paths.distances[target_id] << "\nPath: "
              << gl::io::range_formatter(path_to_target, " -> ", "", "") // (8)!
              << '\n';

    return 0;
}
```

1. Include the necessary core graph, algorithm, and I/O headers.
2. Define the graph's memory layout (adjacency list), directionality, element payloads, ID type, and API policy via a single traits structure. <br/> **NOTE:** Alternatively, you can use one of the provided [generic graph type aliases](../cpp-gl/group__GL-Core.md#public-types).
3. Instantiate the generic `graph` class, pre-allocating it with 5 vertices (IDs 0 through 4).
4. The `add_edge` method returns an `edge_descriptor`. Use the overloaded `->` operator to directly access and assign the attached `weight` payload in a type-safe manner.
5. Execute the external algorithm on the instantiated graph.
6. Use built-in utilities to verify if the target vertex was successfully reached during the pathfinding execution.
7. Walk backward through the resulting predecessor map to reconstruct the exact sequence of vertices forming the shortest path.
8. Stream the reconstructed path seamlessly using the library's built-in formatting utilities, customizing the delimiter to `" -> "`.

**Output:**
```text
Shortest path distance to vertex 4: 9
Path: 0 -> 2 -> 3 -> 1 -> 4
```

### Understanding the Code

- **Traits (`gl::list_graph_traits`):** CPP-GL relies heavily on template abstraction. Instead of passing multiple arguments to the `gl::graph` constructor, you pass a single *Traits* struct as its template parameter. This strictly dictates whether the graph uses an adjacency list or matrix, if it is directed, what custom properties (like `gl::weight_property`) exist on its elements and what ID type is used for its elements, and its API validation policy (defaulting to safe, strict bounds-checking).

- **Property Access (`->weight`):** Adding an edge returns a descriptor handle. You access the specific payload fields associated with that edge by using the overloaded `->` operator, ensuring highly performant and type-safe data manipulation.

- **Algorithm Separation:** Search engines and algorithms (like `dijkstra_shortest_paths`) exist entirely outside the graph class. They accept the graph as a `const` reference, mathematically guaranteeing that algorithms will never accidentally mutate your topology.

- **Formatting (`gl::io::range_formatter`):** The library includes lightweight utility formatters so you can easily stream sequences, paths, and standard ranges directly to the console with complete control over delimiters and brackets.
