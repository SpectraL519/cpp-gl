# I/O operations

The `CPP-GL` library provides functionality for input and output stream operations, allowing users to easily read from and write to various data streams. This feature facilitates the serialization and deserialization of graph data, enabling efficient storage and retrieval of graph structures.

The library supports multiple output formats, controlled through predefined options, making it easy to customize the level of detail included in the output. Input operations ensure that graphs can be recreated from formatted input streams, maintaining data integrity.

Users can save graphs to a file in multiple formats, while the library supports reading from files only in the Graph Specification Format (GSF). This ensures that graphs can be reconstructed accurately from files, preserving the graph's structure and properties.

<br />

## Table of content

- [Basic usage of stream I/O operations](#basic-usage-of-stream-io-operations)
- [I/O options](#io-options)
- [File I/O operations](#file-io-operations)
- [GSF (Grahp Specification Format)](#gsf-graph-specification-format)
- [Related pages](#related-pages)

<br />
<br />

## Basic usage of stream I/O operations

The `CPP-GL` library provides convenient stream insertion and extraction operators for the `graph`, `vertex_descriptor`, and `edge_descriptor` classes, allowing for easy reading and writing of graph data.

> [!NOTE]
> In all examples in this section:
>
> - `graph_type` will refer to any specialization of the `gl::graph`
> - `graph_element_type` will refer to any specialization of one of the `vertex_descriptor` or `edge_descriptor` classes

To output a *graph* or *graph element* object to a stream, use the insertion operator `<<`:

```cpp
graph_type g;
// Populate the graph 'g' with vertices and edges
std::cout << g << std::endl; // Outputs the graph in the format set for the std::cout stream
```

```cpp
const graph_element_type& g_elem = g.<element-getter-method>(params);
std::cout << g_elem << std::endl; // Outputs the single graph element in the format set for the std::cout stream
```

<br />
<br />

## I/O options

The output format can be controlled using the predefined option setters defined in the [gl/graph_io.hpp](/include/gl/graph_io.hpp) header file.

| **Option setter** | **Description** | **Equivalent** |
| :- | :- | :- |
| `verbose` | Outputs detailed information about an element | `set_option(graph_option::verbose)` |
| `concise` | Outputs a concise representation of an element | `unset_option(graph_option::verbose)` |
| `with_vertex_properties` | Outputs an element with vertex properties included | `set_option(graph_option::with_vertex_properties)` |
| `without_vertex_properties` | Outputs an element without vertex properties | `unset_option(graph_option::with_vertex_properties)` |
| `with_edge_properties` | Outputs an element with edge properties included<br/> **NOTE:** For vertices the behaviour is unchanged | `set_option(graph_option::with_edge_properties)` |
| `without_edge_properties` | Outputs an element without edge properties<br/> **NOTE:** For vertices the behaviour is unchanged | `unset_option(graph_option::with_edge_properties)` |
| `with_properties` | Includes both vertex and edge properties in the output | <pre>set_options({<br>&nbsp;graph_option::with_vertex_properties,<br>&nbsp;graph_option::with_edge_properties<br>});</pre> |
| `without_properties` | Excludes both vertex and edge properties from the output | <pre>unset_options({<br>&nbsp;graph_option::with_vertex_properties,<br>&nbsp;graph_option::with_edge_properties<br>});</pre> |
| `enable_gsf` | Enables GSF output<br/>**NOTE:** Changes the stream behaviour only for the `graph` class | `set_option(graph_option::gsf)` |
| `disable_gsf` | Disables GSF output<br/>**NOTE:** Changes the stream behaviour only for the `graph` class | `unset_option(graph_option::gsf)` |

> [!IMPORTANT]
>
> - The `(un)set_option` functions return an instance of `io::stream_options_manipulator` with handles setting the options of the stream using bitmasks based on the options' bit positions.
> - The `(un)set_option` functions as well as the `graph_option` enum and all the predefined option setters are defined in the `gl::io` namespace.
> - The `(un)set_option` functions (as well as the predefined option setters) modify the behaviour only of stream they are inserted to or extracted from and have effect for all graph or graph element instances in such stream.

### Example: option setters

Below you can find an example program showing how to use the I/O option setters:

```cpp
#include <gl/graph.hpp>
#include <gl/topologies.hpp> // necessary for `topology::biclique`

int main() {
    // declare the graph traits
    using graph_traits = gl::graph_traits<gl::directed_t, gl::types::name_property, gl::types::name_property>;
    using graph_type = gl::graph<graph_traits>;

    // initialize a biclique where |A| = 2 and |B| = 3
    const auto graph = gl::topology::biclique<graph_type>(2, 3);

    // set the vertex and edge properties of the graph
    std::size_t v_idx = 0, e_idx = 0;
    for (const auto& vertex : graph.vertices()) {
        vertex.properties = {std::format("vertex_{}", ++v_idx)};
        for (const auto& edge : graph.adjacent_edges(vertex))
            edge.properties = {std::format("edge_{}", ++e_idx)};
    }

    // print the graph in a concise representation and without properties
    std::cout << gl::io::concise << gl::io::without_properties
              << "> concise, without_properties\n" << graph << std::endl;

    // print the graph in a verbose representation and with all properties
    std::cout << gl::io::verbose << gl::io::with_properties
              << "> verbose, with_properties\n" << graph << std::endl;
}
```

This program will output:

```txt
> concise, without_properties
directed 5 12
- 0 : [0, 2] [0, 3] [0, 4]
- 1 : [1, 2] [1, 3] [1, 4]
- 2 : [2, 0] [2, 1]
- 3 : [3, 0] [3, 1]
- 4 : [4, 0] [4, 1]

> verbose, with_properties
type: directed
number of vertices: 5
number of edges: 12
vertices:
- [id: 0 | properties: "vertex_1"]
  adjacent edges:
        - [first: 0, second: 2 | properties: "edge_1"]
        - [first: 0, second: 3 | properties: "edge_2"]
        - [first: 0, second: 4 | properties: "edge_3"]
- [id: 1 | properties: "vertex_2"]
  adjacent edges:
        - [first: 1, second: 2 | properties: "edge_4"]
        - [first: 1, second: 3 | properties: "edge_5"]
        - [first: 1, second: 4 | properties: "edge_6"]
- [id: 2 | properties: "vertex_3"]
  adjacent edges:
        - [first: 2, second: 0 | properties: "edge_7"]
        - [first: 2, second: 1 | properties: "edge_8"]
- [id: 3 | properties: "vertex_4"]
  adjacent edges:
        - [first: 3, second: 0 | properties: "edge_9"]
        - [first: 3, second: 1 | properties: "edge_10"]
- [id: 4 | properties: "vertex_5"]
  adjacent edges:
        - [first: 4, second: 0 | properties: "edge_11"]
        - [first: 4, second: 1 | properties: "edge_12"]
```

<br />
<br />

## File I/O Operations

The library provides functionality to read and write graphs to and from files. This allows you to store graph data and load it when needed. Graphs can be saved to files in various formats, while loading is supported exclusively in the Graph Specification Format (GSF).

### Writing graphs to a file

You can save a graph to a file using the `io::save(graph, path, options)` function:

- **Template parameters**:
  - `GraphType: type_traits::c_graph` – The type of the graph to save.
  - `Mode: io::detail::c_io_save_mode = io::write` - File open mode.

> [!NOTE]
> The `Mode` parameter can be:
>
> - `io::write` : Creates a new file. If the file already exists, an instance of `std::filesystem::error` is thrown.
> - `io::append` : Opens an existing file and appends the graph data to it. If the file does not exist, an instance of `std::filesystem::error` is thrown.

- **Description**: Saves the specified graph to the provided file path. The function also allows for optional stream options to control the output format.
- **Parameters**:
  - `graph: const GraphType&` – The graph to be saved to the file.
  - `path: const std::filesystem::path&` (default: `"graph.gsf"`) – The path to the file where the graph will be saved.
  - `options: const std::initializer_list<stream_options_manipulator>&` (default: `{}`) – Optional stream options to control the output.
- **Return type**: `void`

> [!IMPORTANT]
>
> - The `io::save` function uses a new `std::fstream` object, so all desired saving options must be specified in the `options`. list
> - The `io::save` function enables [GSF output](#gsf-graph-specification-format) bu defaul, so to disable it, you have to add `io::disable_gsf` to the `options` list.

### Loading graphs from a file

You can load a graph from a file using the `io::load(path)` function:

- **Description**: Loads a graph from the specified file path.
- **Template parameters**:
  - `GraphType: type_traits::c_graph` – The type of the graph to be loaded.

> [!NOTE]
>
> - If the specified file path does not exist, an instance of `std::filesystem::error` is thrown.
> - If the specified file does not contain a [GSF representation](#gsf-graph-specification-format), the behaviour is undefined.
> - If the [GSF representation](#gsf-graph-specification-format) does not match the `GraphType`, an instance of `std::ios_base::failure` is thrown.

- **Parameters**:
  - `path: const std::filesystem::path&` (default: `"graph.gsf"`) – The path to the file from which the graph will be loaded.
- **Return type**: `GraphType`

### Example: file I/O

> [!IMPORTANT]
> The file I/O utility is defined in the [gl/graph_file_io.hpp](/include/gl/graph_file_io.hpp) header file.

Below you can find an example showing how to use the `save` and `load` functions:

```cpp
#include <gl/graph.hpp>
#include <gl/topologies.hpp> // necessary for `topology::clique`
#include <gl/graph_file_io.hpp> // necessary to `save` and `load` functions

int main() {
    // declare the graph type
    using graph_traits = gl::graph_traits<gl::directed_t, gl::types::name_property, gl::types::name_property>;
    using graph_type = gl::graph<graph_traits>;

    // initialize a clique of size 3
    const auto clique = gl::topology::clique<graph_type>(3);

    // set the vertex and edge properties of the graph
    std::size_t v_idx = 0, e_idx = 0;
    for (const auto& vertex : clique.vertices()) {
        vertex.properties = {std::format("vertex_{}", ++v_idx)};
        for (const auto& edge : clique.adjacent_edges(vertex))
            edge.properties = {std::format("edge_{}", ++e_idx)};
    }

    // set the std::cout stream options
    std::cout << gl::io::verbose << gl::io::with_properties;

    // print the Original graph
    std::cout << "> original graph:\n" << clique << std::endl;

    // save the graph to a `clique.gsf` file with all properties
    gl::io::save(clique, "clique.gsf", {gl::io::with_properties});

    // load the graph from the same file and print it
    const auto loaded_graph = gl::io::load<graph_type>("clique.gsf");
    std::cout << "> loaded graph:\n" << loaded_graph << std::endl;
}
```

This program will output:

```txt
> original graph:
type: directed
number of vertices: 3
number of edges: 6
vertices:
- [id: 0 | properties: "vertex_1"]
  adjacent edges:
        - [first: 0, second: 1 | properties: "edge_1"]
        - [first: 0, second: 2 | properties: "edge_2"]
- [id: 1 | properties: "vertex_2"]
  adjacent edges:
        - [first: 1, second: 0 | properties: "edge_3"]
        - [first: 1, second: 2 | properties: "edge_4"]
- [id: 2 | properties: "vertex_3"]
  adjacent edges:
        - [first: 2, second: 0 | properties: "edge_5"]
        - [first: 2, second: 1 | properties: "edge_6"]

> loaded graph:
type: directed
number of vertices: 3
number of edges: 6
vertices:
- [id: 0 | properties: "vertex_1"]
  adjacent edges:
        - [first: 0, second: 1 | properties: "edge_1"]
        - [first: 0, second: 2 | properties: "edge_2"]
- [id: 1 | properties: "vertex_2"]
  adjacent edges:
        - [first: 1, second: 0 | properties: "edge_3"]
        - [first: 1, second: 2 | properties: "edge_4"]
- [id: 2 | properties: "vertex_3"]
  adjacent edges:
        - [first: 2, second: 0 | properties: "edge_5"]
        - [first: 2, second: 1 | properties: "edge_6"]
```

<br />
<br />

## GSF (Graph Specification Format)

The Graph Specification Format (GSF) is a simple text-based format used to represent the structure of a graph. It stores information about the graph's vertices and edges in a human-readable format. This format is the only supported format for loading graphs from files.

> [!NOTE]
> Files containing the GSF representation of graphs should have a `.gsf` extension, but it is not mandatory.

### The format structure

```txt
<directional-specifier> <n-vertices> <n-unique-edges> <vertex-properties-marker> <vertex-properties-marker>
<optional-vertex-properties-list>
<edge-list>
```

Where:

| **Element** | **Description** |
| :- | :- |
| `directional-specifier` | `1` if the graph is directed<br/>`0` if the graph is undirected |
| `n-vertices` | The number of vertices in the graph |
| `n-unique-edges` | The number of unique edges in the graph |
| `vertex-properties-marker` | `1` if the vertex properties are present in the graph's specification<br/>`0` otherwise |
| `edge-properties-marker` | `1` if the edge properties are present in the graph's specification<br/>`0` otherwise |
| `optional-vertex-properties-list` | A list of length `n-vertices` where each element represents the<br/>properties of a vertex with the ID being the element's index in the list<br/>**NOTE:** This list is present in the specification if `vertex-properties-marker == 1` |
| `edge-list` | A list of length `n-unique-edges` where each element represents an edge in the graph<br/>**NOTE:** Each edge in the list is represented as:<br/>- A piar `first_id second_id` if `edge-properties-marker == 0`<br/>- A triple `first_id second_id properties` otherwise |

Below you can find an example GSF representation of the graph from the [Exampl: file I/O](#example-file-io) section with all properties:

```txt
1 3 6 1 1
"vertex_1"
"vertex_2"
"vertex_3"
0 1 "edge_1"
0 2 "edge_2"
1 0 "edge_3"
1 2 "edge_4"
2 0 "edge_5"
2 1 "edge_6"
```

<br />
<br />

## Related pages

- [The graph class - library's core](/docs/graph.md)
- [The vertex and edge classes - representation of the graph's elements](/docs/graph_elements.md)
- [Graph topology generators](/docs/topologies.md)
- [Core utility types](/docs/core_util_types.md)
