# I/O Utility

The CPP-GL library provides a robust, customizable, and type-safe I/O system for formatting, printing, serializing, and deserializing graphs and their underlying elements (vertices and edges). The system relies on custom stream manipulators that seamlessly integrate with C++ standard streams (e.g. `std::cout`).

This guide covers basic console output, formatting manipulators, the Graph Specification Format (GSF) for serialization, and general range formatting utilities.

---

## Basic I/O Operations

At its core, the library overloads the standard `operator<<` for the [**graph**](../cpp-gl/classgl_1_1graph.md), [**vertex_descriptor**](../cpp-gl/classgl_1_1vertex__descriptor.md), and [**edge_descriptor**](../cpp-gl/classgl_1_1edge__descriptor.md) classes. By default, printing any of these objects yields a compact/concise representation.

```cpp
#include <gl/graph.hpp>
#include <iostream>

int main() {
    gl::directed_graph<> g;
    auto v0 = g.add_vertex();
    auto v1 = g.add_vertex();
    auto e = g.add_edge(v0, v1);

    std::cout << "Vertex: " << v0 << '\n'; // (1)!
    std::cout << "Edge: " << e << '\n'; // (2)!
    std::cout << "Graph:\n" << g << '\n'; // (3)!
}
```

1. Prints the vertex ID.
2. Prints the edge endpoints.
3. Prints the adjacency list representation of the graph in a compact format.

**Output:**

```text
Vertex: 0
Edge: (0, 1)
Graph:
0 : [1]
1 : []
```

---

## Stream Options Manipulators

To customize how graphs and elements are formatted, CPP-GL uses [Stream Options Manipulators](../cpp-gl/classgl_1_1io_1_1options__manip.md) (defined in <gl/io/options.hpp>). These manipulators inject custom formatting states directly into standard I/O streams using a bitmask stored at a uniquely allocated (using `std::ios_base::xalloc`) index within the stream.

### Behavior Notes

> [!WARNING] State Persistence
>
> Stream options are **persistent**. Once you apply a manipulator to a stream (like `std::cout`), that formatting state remains active for all subsequent operations on that stream until it is explicitly changed or cleared using another manipulator (e.g., [**gl::io::default_options**](../cpp-gl/group__GL-IO.md#variable-default_options)).

> [!IMPORTANT] Shared State Between Modules
>
> The stream manipulator system is completely shared between the Graph Library (GL) and the Hypergraph Library (HGL) modules. Because HGL utilizes the same underlying stream flags, setting an option like [**gl::io::verbose**](../cpp-gl/group__GL-IO.md#variable-verbose) or [**gl::io::with_properties**](../cpp-gl/group__GL-IO.md#variable-with_properties) on a stream will affect the formatting of both graphs and hypergraphs printed to that stream.
>
> **Note:** While HGL defines a `with_hyperedge_properties` option for readability, it maps to the exact same underlying bitmask as the [**gl::io::with_edge_properties**](../cpp-gl/group__GL-IO.md#variable-with_edge_properties) manipulator.

### Available Manipulators

| Manipulator | Description |
| :---------- | :---------- |
| [**gl::io::concise**](../cpp-gl/group__GL-IO.md#variable-concise) | (Default) Sets the layout to a compact, single-line-per-element format. Clears layout flags. |
| [**gl::io::verbose**](../cpp-gl/group__GL-IO.md#variable-verbose) | Sets the layout to a detailed, multi-line format showing full structural hierarchy. |
| [**gl::io::spec_fmt**](../cpp-gl/group__GL-IO.md#variable-spec_fmt) | Enables the Graph Specification Format (GSF) used for raw serialization/deserialization. |
| [**gl::io::with_vertex_properties**](../cpp-gl/group__GL-IO.md#variable-with_vertex_properties) | Instructs the stream to print/read custom payload properties attached to vertices. |
| [**gl::io::without_vertex_properties**](../cpp-gl/group__GL-IO.md#variable-without_vertex_properties) | Disables vertex property formatting. |
| [**gl::io::with_edge_properties**](../cpp-gl/group__GL-IO.md#variable-with_edge_properties) | Instructs the stream to print/read custom payload properties attached to edges. |
| [**gl::io::without_edge_properties**](../cpp-gl/group__GL-IO.md#variable-without_edge_properties) | Disables edge property formatting. |
| [**gl::io::with_properties**](../cpp-gl/group__GL-IO.md#variable-with_properties) | Enables both vertex and edge properties simultaneously. |
| [**gl::io::without_properties**](../cpp-gl/group__GL-IO.md#variable-without_properties) | Disables both vertex and edge properties simultaneously. |
| [**gl::io::default_options**](../cpp-gl/group__GL-IO.md#variable-default_options) | Resets the stream to its default state (concise, no properties). |

### Formatting Example

The following examples demonstrate how the output changes based on the applied stream manipulators.

```cpp
#include <gl/graph.hpp>
#include <gl/io.hpp>
#include <gl/topology.hpp>
#include <iostream>

int main() {
    using graph_type = gl::directed_graph<gl::name_property, gl::name_property>;
    auto graph = gl::topology::biclique<graph_type>(2uz, 3uz); // (1)!

    for (const auto& vertex : graph.vertices()) { // (2)!
        vertex->name = std::format("vertex_{}", vertex.id() + 1);
        for (const auto& edge : graph.out_edges(vertex))
            if (edge->name.empty())
                edge->name = std::format("edge_{}", edge.id() + 1);
    }

    std::cout << graph << std::endl; // (3)!
    std::cout << gl::io::with_properties << graph << std::endl; // (4)!
    std::cout << gl::io::verbose << graph << std::endl; // (5)!
}
```

1. Construct the graph as a $K_{2,3}$ biclique.
2. Set the properties of the graph's elements.
3. Prints the graph using the default options (concise format, without element properties).
4. Prints the graph with both vertex and edge properties, but still in a concise format (options persistency).
5. Prints the graph in a verbose format, with element properties (options persistency).

**Output (Concise, Without Properties):**

```text
0 : [2, 3, 4]
1 : [2, 3, 4]
2 : [0, 1]
3 : [0, 1]
4 : [0, 1]
```

**Output (Concise, With Properties):**

```text
0["vertex_1"] : [2["edge_1"], 3["edge_2"], 4["edge_3"]]
1["vertex_2"] : [2["edge_4"], 3["edge_5"], 4["edge_6"]]
2["vertex_3"] : [0["edge_7"], 1["edge_8"]]
3["vertex_4"] : [0["edge_9"], 1["edge_10"]]
4["vertex_5"] : [0["edge_11"], 1["edge_12"]]
```

**Output (Verbose, With Properties):**

```text
type: directed, |V| = 5, |E| = 12
- [id: 0 | "vertex_1"]
  outgoing edges:
        - [id: 0 | source: 0, target: 2 | "edge_1"]
        - [id: 2 | source: 0, target: 3 | "edge_2"]
        - [id: 4 | source: 0, target: 4 | "edge_3"]
- [id: 1 | "vertex_2"]
  outgoing edges:
        - [id: 6 | source: 1, target: 2 | "edge_4"]
        - [id: 8 | source: 1, target: 3 | "edge_5"]
        - [id: 10 | source: 1, target: 4 | "edge_6"]
- [id: 2 | "vertex_3"]
  outgoing edges:
        - [id: 1 | source: 2, target: 0 | "edge_7"]
        - [id: 7 | source: 2, target: 1 | "edge_8"]
- [id: 3 | "vertex_4"]
  outgoing edges:
        - [id: 3 | source: 3, target: 0 | "edge_9"]
        - [id: 9 | source: 3, target: 1 | "edge_10"]
- [id: 4 | "vertex_5"]
  outgoing edges:
        - [id: 5 | source: 4, target: 0 | "edge_11"]
        - [id: 11 | source: 4, target: 1 | "edge_12"]
```

---

## Graph Specification Format (GSF)

For storage purposes, CPP-GL defines the **Graph Specification Format (GSF)**. This format is triggered by the [**gl::io::spec_fmt**](../cpp-gl/group__GL-IO.md#variable-spec_fmt) manipulator and is designed to be easily parseable.

### Format Structure

```text
<dir-spec> <n-vertices> <n-edges> <vprop-marker> <eprop-marker>
<vertex-prop-list>
<edge-list>
```

| **Element**        | **Description** |
| :----------------- | :-------------- |
| `dir-spec`         | `1` if the graph is directed<br/>`0` if the graph is undirected |
| `n-vertices`       | The number of vertices in the graph |
| `n-edges`          | The number of unique edges in the graph |
| `vprop-marker`     | `1` if the vertex properties are present in the graph's specification<br/>`0` otherwise |
| `eprop-marker`     | `1` if the edge properties are present in the graph's specification<br/>`0` otherwise |
| <span style="white-space: nowrap;">`vertex-prop-list`</span> | A list of length `n-vertices` where each element represents the properties of a vertex with the ID being the element's index in the list<br/>**NOTE:** This list is present in the specification if `vprop-marker == 1` |
| `edge-list`        | A list of length `n-edges` where each element represents an edge in the graph<br/>**NOTE:** Each edge in the list is represented as:<br/>- A piar `first-id second-id` if `eprop-marker == 0`<br/>- A triple `first-id second-id properties` otherwise |

### Example GSF Output

The following representation is that of the $K_{2,3}$ bipartite graph from the previous [formatting example](#formatting-example) and could be printed using:

```cpp
std::cout << gl::io::spec_fmt << gl::io::with_properties << graph << std::endl;
```

**Output:**

```text
1 5 12 1 1
"vertex_1"
"vertex_2"
"vertex_3"
"vertex_4"
"vertex_5"
0 2 "edge_1"
0 3 "edge_2"
0 4 "edge_3"
1 2 "edge_4"
1 3 "edge_5"
1 4 "edge_6"
2 0 "edge_7"
2 1 "edge_8"
3 0 "edge_9"
3 1 "edge_10"
4 0 "edge_11"
4 1 "edge_12"
```

---

## General Range Formatting

Beyond graph-specific structures, the `gl::io` namespace includes highly flexible tools for formatting standard C++ ranges (`<gl/io/ranges.hpp>`).

### Standard Range Formatter

The [**gl::io::range_formatter**](../cpp-gl/structgl_1_1io_1_1range__formatter.md) type allows you to print any `std::ranges::range` with customizable delimiters and separators.

```cpp
std::vector<int> vec = {1, 2, 3};
std::cout << gl::io::range_formatter{vec} << '\n';
std::cout << gl::io::range_formatter{vec, " | ", "<", ">"} << '\n';
```

**Output:**

```text
[1, 2, 3]
<1 | 2 | 3>
```

### Set Formatters

The library defines convenience `gl::io::range_formatter` factories for printing ranges using the curly brace `{}` notation.

```cpp
std::vector<int> vec = {1, 2, 3};
std::cout << gl::io::set_formatter(vec) << '\n';
std::cout << "data: " << gl::io::multiline_set_formatter(vec) << '\n';
```

**Output:**

```text
{1, 2, 3}
data: {
  1,
  2,
  3
}
```

### Implicit Range Formatter

When dealing with massive, contiguous integer sequences, printing every element is inefficient. [**gl::io::implicit_range**](../cpp-gl/structgl_1_1io_1_1implicit__range__formatter.md) smartly abbreviates the output.

```cpp
std::cout << gl::io::implicit_range(0, 100) << '\n'; // (1)!
std::cout << gl::io::implicit_range(5, 8, true) << '\n'; // (2)!
```

1. Prints an end-exclusive range from 0 to 100 (0 to 99).
2. Prints an inclusive range from 5 to 8.

**Output:**

```text
{0, ..., 99}
{5, ..., 8}
```
