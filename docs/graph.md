# The graph class - library's core

The `graph` class is the entry point and key element of the `CPP-GL` library. It stores the [graph's elements](/docs/graph_elements.md) and defines methods allowing you to perform various operations on the graph.

<br />

## Table of contents

* [The basics](#the-basics)
* [Graph representation](#graph-representation)
* [Graph operations](#graph-operations)
* [Related pages](#related-pages)

<br />
<br />

## The basics

> [!IMPORTANT]
> All elements of the `CPP-GL` library are defined in the `gl` namespace.

The `graph` class is defined in the `gl/graph.hpp` header file, which includes most of the elements required to work with the graph structure.

```cpp
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

```cpp
using custom_traits = graph_traits<...>;
graph<custom_traits> graph;
```

The table below demostrates what parameters of the graph can be modified:

| **Trait** | **Purpose** | **Constraints** | **Default value** |
| :- | :- | :- | :- |
| EdgeDirectionalTag | Specifies whether the graph should store directed or undirected edges | Either `directed_t` or `undirected_t`<br/>**Concept:** `type_traits::c_edge_directional_tag` | `directed_t` |
| VertexProperties | The properties type associated with each vertex in the graph | Must be default, copy and move constructible and define copy and move assignment operators<br/>**Concept:** `type_traits::c_properties` | `types::empty_properties` |
| EdgeProperties | The properties type associated with each edge in the graph | Must be default, copy and move constructible and define copy and move assignment operators<br/>**Concept:** `type_traits::c_properties` | `types::empty_properties` |
| ImplTag  | Specifies the underlying graph representation structure (adjacency list or matrix) | Either `impl::list_t` or `impl::matrix_t`<br/>**Concept:** `type_traits::c_graph_impl_tag` | `impl::list_t` |

An example on how to define an undirected graph with a *weight* edge properties type and represented as an adjacency matrix:

```cpp
#include <gl/graph.hpp>

using traits = gl::graph_traits<
    gl::undirected_t,                // EdgeDirectionalTag
    gl::types::empty_properties,     // VertexProperties
    gl::types::weight_property<int>, // EdgeProperties
    gl::impl::matrix_t>;             // ImplTag

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

Based on the specified traits, the `graph` class defines the following types:

| **Type** | **Description** |
| :- | :- |
| `traits_type` | The `graph_traits` specialization used as the graph's template parameter |
| `implementation_tag` | The `ImplTag` parameter of the `graph_traits` structure |
| `implementation_type` | The underlying data structure used to store the graph's edges and the adjacency information |
| `vertex_type` | The type of the vertex element (an instantiation of `vertex_descriptor`) |
| `vertex_properties_type` | The type of the properties element associated with each vertex |
| `vertex_iterator_type` | The iterator type used for vertex traversal in the graph |
| `edge_type` | The type of the edge element (an instantiation of `edge_descriptor`) |
| `edge_directional_tag` | The `EdgeDirectionalTag` parameter of the `graph_traits` structure |
| `edge_properties_type` | The type of the properties element associated with each edge |
| `edge_iterator_type` | The iterator type used for edge traversal in the graph |

<br />
<br />

## Graph operations

<!-- TODO: add a note with a link to a types doc page -->

### General Operations

* **`graph()`**:
  * Default constructor. Creates an empty graph with no vertices or edges.

* **`graph(n_vertices)`**:
  * Constructs a graph with the specified number of vertices. Each vertex is initialized with default properties and no adjacent edges.

<!-- TODO: info about copy, move, destruct -->

> [!IMPORTANT]
> Constructing the graph with the number of vertices parameter is more efficient, as it avoids multiple vector reallocations which could happen when creating an empty graph and adding vertices one by one.

* **`graph.n_vertices() const`**:
  * Returns the total number of vertices in the graph.
  * *Return type*: `types::size_type`

* **`graph.n_unique_edges() const`**:
  * Returns the number of unique edges in the graph.
  * *Return type*: `types::size_type`

<br />

### Vertex Operations

* **`graph.vertices() const`**:
  * Returns an iterator range over all vertices in the graph.
  * *Return type*: `types::iterator_range<vertex_iterator_type>`

* **`graph.vertex_ids() const`**:
  * Returns a range of vertex IDs, starting from the initial vertex ID to the number of vertices in the graph (excluded).
  * *Return type*: `std::ranges::iota_view<types::id_type, types::id_type>`

* **`graph.get_vertex(vertex_id) const`**:
  * Retrieves the vertex object associated with the given vertex ID.
  * *Parameters*:
    * `vertex_id: const types::id_type`
  * *Return type*: `const vertex_type&`

* **`graph.has_vertex(vertex_id) const`**:
  * Checks if a vertex exists for the given vertex ID.
  * *Parameters*:
    * `vertex_id: const types::id_type`
  * *Return type*: `bool`

* **`graph.has_vertex(vertex) const`**:
  * Checks if the given vertex object exists in the graph by verifying its ID as well as its memory address.
  * *Parameters*:
    * `vertex: const vertex_type&`
  * *Return type*: `bool`

* **`graph.add_vertex()`**:
  * Adds a new vertex to the graph with default properties and returns a reference to the newly added vertex.
  * *Return type*: `const vertex_type&`

* **`graph.add_vertex(properties)`**:
  * Adds a new vertex with specified properties and returns a reference to the newly added vertex. This overload is available when the vertex properties type is not the default.
  * *Parameters*:
    * `properties: const vertex_properties_type&` – Properties to assign to the new vertex.
  * *Return type*: `const vertex_type&`
  * *Requires*: non-default `vertex_properties_type`

* **`graph.add_vertices(n)`**:
  * Adds `n` new vertices to the graph, each with default properties.
  * *Parameters*:
    * `n: types::size_type` – The number of vertices to add.
  * *Return type*: `void`

* **`graph.add_vertices_with(properties_range)`**:
  * *Template parameters*:
    * `VertexPropertiesRange: type_traits::c_sized_range_of<vertex_properties_type>` – A range of vertex properties that must satisfy the size and type constraints.
  * Adds multiple vertices to the graph, each with the corresponding properties from the given range. The number of vertices added is determined by the size of `properties_range`.
  * *Parameters*:
    * `properties_range: const VertexPropertiesRange&` – A range of properties to assign to each of the new vertices.
  * *Return type*: `void`
  * *Requires*: non-default `vertex_properties_type`

> [!IMPORTANT]
> Simliarily to the graph constructors using the `add_vertices*` methods is more efficient than calling `add_vertex` multiple times which might cause more vector reallocations.

* **`graph.remove_vertex(vertex_id)`**:
  * Removes the vertex with the given ID from the graph.
  * *Parameters*:
    * `vertex_id: types::size_type` – The ID of the vertex to remove.
  * *Return type*: `void`

* **`graph.remove_vertex(vertex)`**:
  * Removes the specified vertex from the graph.
  * *Parameters*:
    * `vertex: const vertex_type&` – A reference to the vertex to remove.
  * *Return type*: `void`

* **`graph.remove_vertices_from(vertex_id_range)`**:
  * *Template Parameters*:
    * `IdRange: type_traits::c_sized_range_of<types::id_type>` – A range of vertex IDs, which must satisfy the size and type constraints.
  * Removes multiple vertices from the graph based on a range of vertex IDs. The IDs are sorted in descending order and duplicate IDs are removed before deletion.
  * *Parameters*:
    * `vertex_id_range: const IdRange&` – A range of vertex IDs to remove.
  * *Return type*: `void`

* **`graph.remove_vertices_from(vertex_ref_range)`**:
  * *Template parameters*:
    * `VertexRefRange: type_traits::c_sized_range_of<types::const_ref_wrap<vertex_type>>` – A range of vertex references that must satisfy the size and type constraints.
  * Removes multiple vertices from the graph based on a range of vertex references. The references are sorted in descending order and duplicates are removed before deletion.
  * *Parameters*:
    * `vertex_ref_range: const VertexRefRange&` – A range of vertex references to remove.
  * *Return type*: `void`

  * **`graph.in_degree(vertex) const`**:
  * returns the in-degree (number of incoming edges) of the specified vertex.
  * *Parameters*:
    * `vertex: const vertex_type&` – the vertex for which to calculate the in-degree.
  * *Return type*: `types::size_type`

* **`graph.in_degree(vertex_id) const`**:
  * returns the in-degree (number of incoming edges) of the vertex with the specified ID.
  * *Parameters*:
    * `vertex_id: types::id_type` – the ID of the vertex for which to calculate the in-degree.
  * *Return type*: `types::size_type`

* **`graph.out_degree(vertex) const`**:
  * returns the out-degree (number of outgoing edges) of the specified vertex.
  * *Parameters*:
    * `vertex: const vertex_type&` – the vertex for which to calculate the out-degree.
  * *Return type*: `types::size_type`

* **`graph.out_degree(vertex_id) const`**:
  * returns the out-degree (number of outgoing edges) of the vertex with the specified ID.
  * *Parameters*:
    * `vertex_id: types::id_type` – the ID of the vertex for which to calculate the out-degree.
  * *Return type*: `types::size_type`

* **`graph.degree(vertex) const`**:
  * returns the degree (number of incoming and outgoing edges) of the specified vertex.
  * *Parameters*:
    * `vertex: const vertex_type&` – the vertex for which to calculate the degree.
  * *Return type*: `types::size_type`

* **`graph.degree(vertex_id) const`**:
  * returns the degree (number of incoming and outgoing edges) of the vertex with the specified ID.
  * *Parameters*:
    * `vertex_id: types::id_type` – the ID of the vertex for which to calculate the degree.
  * *Return type*: `types::size_type`

<br />

### Edge Operations

* **`graph.add_edge(first_id, second_id)`**:
  * Adds a new edge between the vertices with the specified IDs and returns a reference to the newly added edge.
  * *Parameters*:
    * `first_id: types::id_type` – the ID of the first vertex.
    * `second_id: types::id_type` – the ID of the second vertex.
  * *Return type*: `const edge_type&`

* **`graph.add_edge(first_id, second_id, properties)`**:
  * Adds a new edge between the vertices with the specified IDs and returns a reference to the newly added edge. This overload is available when the edge properties type is not the default.
  * *Parameters*:
    * `first_id: types::id_type` – the ID of the first vertex.
    * `second_id: types::id_type` – the ID of the second vertex.
    * `properties: const edge_properties_type&` – properties to assign to the new edge.
  * *Return type*: `const edge_type&`
  * *Requires*: non-default `edge_properties_type`

* **`graph.add_edge(first, second)`**:
  * Adds a new edge between the specified vertices and returns a reference to the newly added edge.
  * *Parameters*:
    * `first: const vertex_type&` – the first vertex.
    * `second: const vertex_type&` – the second vertex.
  * *Return type*: `const edge_type&`

* **`graph.add_edge(first, second, properties)`**:
  * Adds a new edge between the specified vertices and returns a reference to the newly added edge. This overload is available when the edge properties type is not the default.
  * *Parameters*:
    * `first: const vertex_type&` – the first vertex.
    * `second: const vertex_type&` – the second vertex.
    * `properties: const edge_properties_type&` – properties to assign to the new edge.
  * *Return type*: `const edge_type&`
  * *Requires*: non-default `edge_properties_type`

* **`graph.add_edges_from(source_id, destination_id_range)`**:
  * *Template Parameters*:
    * `IdRange: type_traits::c_sized_range_of<types::id_type>` – a range of vertex IDs that must satisfy the size and type constraints.
  * Adds multiple edges from a source vertex (specified by ID) to a range of destination vertices (also specified by IDs).
  * *Parameters*:
    * `source_id: types::id_type` – the ID of the source vertex.
    * `destination_id_range: const IdRange&` – a range of destination vertex IDs to connect to the source vertex.
  * *Return type*: `void`

* **`graph.add_edges_from(source, destination_range)`**:
  * *Template Parameters*:
    * `VertexRefRange: type_traits::c_sized_range_of<types::const_ref_wrap<vertex_type>>` – a range of vertex references that must satisfy the size and type constraints.
  * Adds multiple edges from a specified source vertex to a range of destination vertices (specified by references).
  * *Parameters*:
    * `source: const vertex_type&` – the source vertex.
    * `destination_range: const VertexRefRange&` – a range of destination vertex references to connect to the source vertex.
  * *Return type*: `void`

> [!IMPORTANT]
> Behaviour of adding an edge between `first` and `second`:
>
> * for *directed* graphs: adds a one-directional edge where `first` is the source vertex and `second` is the destination vertex
> * for *undirected* graphs: adds a bidirectional edge where both vertices are source and destination vertices

* **`graph.has_edge(first_id, second_id) const`**:
  * Returns true if there is an edge between the vertices with the specified IDs.
  * *Parameters*:
    * `first_id: types::id_type` – the ID of the first vertex.
    * `second_id: types::id_type` – the ID of the second vertex.
  * *Return type*: `bool`

* **`graph.has_edge(first, second) const`**:
  * Returns true if there is an edge between the specified vertices.
  * *Parameters*:
    * `first: const vertex_type&` – the first vertex.
    * `second: const vertex_type&` – the second vertex.
  * *Return type*: `bool`

* **`graph.has_edge(edge) const`**:
  * Returns true if the specified edge exists in the graph.
  * *Parameters*:
    * `edge: const edge_type&` – the edge to check for existence.
  * *Return type*: `bool`

* **`graph.get_edge(first_id, second_id) const`**:
  * Returns an optional reference to the edge between the vertices with the specified IDs. If no edge exists, `std::nullopt` is returned.
  * *Parameters*:
    * `first_id: types::id_type` – the ID of the first vertex.
    * `second_id: types::id_type` – the ID of the second vertex.
  * *Return type*: `types::optional_cref<edge_type>`

* **`graph.get_edge(first, second) const`**:
  * Returns an optional reference to the edge between the specified vertices. If either vertex does not exist or if no edge exists between them, `std::nullopt` is returned.
  * *Parameters*:
    * `first: const vertex_type&` – the first vertex.
    * `second: const vertex_type&` – the second vertex.
  * *Return type*: `types::optional_cref<edge_type>`

* **`graph.get_edges(first_id, second_id) const`**:
  * Returns a vector of optional references to the edges between the vertices with the specified IDs. If no edges exist, returns an empty vector.
  * *Parameters*:
    * `first_id: types::id_type` – the ID of the first vertex.
    * `second_id: types::id_type` – the ID of the second vertex.
  * *Return type*: `std::vector<types::const_ref_wrap<edge_type>>`

* **`graph.get_edges(first, second) const`**:
  * Returns a vector of optional references to the edges between the specified vertices. If either vertex does not exist or if no edges exist between them, returns an empty vector.
  * *Parameters*:
    * `first: const vertex_type&` – the first vertex.
    * `second: const vertex_type&` – the second vertex.
  * *Return type*: `std::vector<types::const_ref_wrap<edge_type>>`

* **`graph.remove_edge(edge)`**:
  * Removes the specified edge from the graph.
  * *Parameters*:
    * `edge: const edge_type&` – the edge to be removed.
  * *Return type*: `void`

* **`graph.remove_edges_from(edges)`**:
  * *Template Parameters*:
    * `EdgeRefRange: type_traits::c_range_of<types::const_ref_wrap<edge_type>>` – a range of edge references that must satisfy the type constraints.
  * Removes multiple edges from the graph, as specified by the provided range of edge references.
  * *Parameters*:
    * `edges: const EdgeRefRange&` – a range of edges to remove from the graph.
  * *Return type*: `void`

* **`graph.adjacent_edges(vertex_id) const`**:
  * Returns an iterator range of edges that are adjacent to the vertex with the specified ID.
  * *Parameters*:
    * `vertex_id: types::id_type` – the ID of the vertex for which to find adjacent edges.
  * *Return type*: `types::iterator_range<edge_iterator_type>`

* **`graph.adjacent_edges(vertex) const`**:
  * Returns an iterator range of edges that are adjacent to the specified vertex.
  * *Parameters*:
    * `vertex: const vertex_type&` – the vertex for which to find adjacent edges.
  * *Return type*: `types::iterator_range<edge_iterator_type>`

<br />

### Incidence Operations

* **`graph.are_incident(first_id, second_id) const`**:
  * Returns true if the vertices with the specified IDs are incident to each other or the IDs are the same.
  * *Parameters*:
    * `first_id: types::id_type` – the ID of the first vertex.
    * `second_id: types::id_type` – the ID of the second vertex.
  * *Return type*: `bool`

* **`graph.are_incident(first, second) const`**:
  * Returns true if the specified vertices are incident to each other or if they are the same.
  * *Parameters*:
    * `first: const vertex_type&` – the first vertex.
    * `second: const vertex_type&` – the second vertex.
  * *Return type*: `bool`

* **`graph.are_incident(vertex, edge) const`**:
  * Returns true if the given vertex is incident with the given edge.
  * *Parameters*:
    * `vertex: const vertex_type&` – the vertex to check for incidence.
    * `edge: const edge_type&` – the edge to check against.
  * *Return type*: `bool`

* **`graph.are_incident(edge, vertex) const`**:
  * Returns true if the given vertex is incident with the given edge.
  * *Parameters*:
    * `edge: const edge_type&` – the edge to check for incidence.
    * `vertex: const vertex_type&` – the vertex to check against.
  * *Return type*: `bool`

* **`graph.are_incident(edge_1, edge_2) const`**:
  * Returns true if the given edges are incident.
  * *Parameters*:
    * `edge_1: const edge_type&` – the first edge.
    * `edge_2: const edge_type&` – the second edge.
  * *Return type*: `bool`

> [!CAUTION]
> All graph operations which take vertex ids, vertex references or edge references as parameters throw the `std::invalid_argument` exception if such parameter is invalid.

<br />
<br />

## Related pages

This section provides links to additional resources and documentation that can help you further explore the capabilities of the `CPP-GL` library.

* [The vertex and edge classes - representation of the graph's elements](/docs/graph_elements.md)
* [IO operations](/docs/io.md)
* [Graph topology generators](/docs/topologies.md)
* [Algorithms](/docs/algoithms.md)
<!-- TODO: types page -->
