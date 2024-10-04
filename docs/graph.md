# The graph class - library's core

The `graph` class is the entry point and key element of the `CPP-GL` library. It stores the [graph's elements](/docs/graph_elements.md) and defines methods allowing you to perform various operations on the graph.

<br />

## Table of contents

* [The basics](#the-basics)
* [Graph representation](#graph-representation)
* [Graph operations](#graph-operations)
* ...

<br />
<br />

## The basics

> [!IMPORTANT]
> All elements of the `CPP-GL` library are defined in the `gl` namespace.

The `graph` class is defined in the `gl/graph.hpp` header file, which includes most of the elements required to work with the graph structure.

```c++
#include <gl/graph.hpp>
```

By default, the `graph` class represents a directed graph using an adjacency list data structure, with no associated properties for vertices or edges. However, this behavior is fully customizable. By passing a specialized `graph_traits` structure as a template parameter, you can modify key aspects of the graph, such as the underlying data structure, directionality, and the vertex and edge properties types.

Instructions on how to define and pass a custom graph_traits structure are provided in the [next section](#graph-representation).

In addition the `graph` class provides some common operations, including adding and removing vertices or edges, traversing the graph's vertices, querying a vertex's adjacent edges and degree, etc.

Detailed information on the available graph operations and how to use them is provided in the [Graph operations](#graph-operations) section.

<br />
<br />

## Graph representation

Defining a custom `graph_traits` structure can be used to modify the underlying implementation structure and behaviour of the `graph` class.

```c++
using custom_traits = graph_traits<...>;
graph<custom_traits> graph;
```

The table below demostrates what parameters of the graph can be modified:

| Trait | Purpose | Constraints | Default value |
| :- | :- | :- | :- |
| EdgeDirectionalTag | Specifies whether the graph should store directed or undirected edges | Either `directed_t` or `undirected_t`<br/>**Concept:** `type_traits::c_edge_directional_tag` | `directed_t` |
| VertexProperties | The properties type associated with each vertex in the graph | Must be default, copy and move constructible and define copy and move assignment operators<br/>**Concept:** `type_traits::c_properties` | `types::empty_properties` |
| EdgeProperties | The properties type associated with each edge in the graph | Must be default, copy and move constructible and define copy and move assignment operators<br/>**Concept:** `type_traits::c_properties` | `types::empty_properties` |
| ImplTag  | Specifies the underlying graph representation structure (adjacency list or matrix) | Either `impl::list_t` or `impl::matrix_t`<br/>**Concept:** `type_traits::c_graph_impl_tag` | `impl::list_t` |

An example on how to define an undirected graph with a *weight* edge properties type and represented as an adjacency matrix:

```c++
#include <gl/graph.hpp>

using traits = gl::graph_traits<
    gl::undirected_t, // EdgeDirectionalTag
    gl::types::empty_properties, // VertexProperties
    gl::types::weight_property<int>, // EdgeProperties
    gl::impl::matrix_t>; // ImplTag

int main() {
    gl::graph<traits> graph;
    // operations on the defined graph structure
}
```

Additionally the library defines template type aliases which make it easier to define the desired graph traits specializations:

* `directed_graph_traits<VertexProperties, EdgeProperties, ImplTag>`
* `undirected_graph_traits<VertexProperties, EdgeProperties, ImplTag>`
* `list_graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties>`
* `matrix_graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties>`

Where the default values of all parameters are the same as in the table above.

<br />
<br />

## Graph operations
