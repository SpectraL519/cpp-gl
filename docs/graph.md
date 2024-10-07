# The graph class - library's core

The `graph` class is the entry point and key element of the `CPP-GL` library. It stores the [graph's elements](/docs/graph_elements.md) and defines methods allowing you to perform various operations on the graph.

<br />

## Table of content

- [The basics](#the-basics)
- [Graph representation](#graph-representation)
- [Graph operations](#graph-operations)
- [Additional utility](#additional-utility)
- [Related pages](#related-pages)

<br />
<br />

## The basics

> [!IMPORTANT]
> All elements of the `CPP-GL` library are defined in the `gl` namespace.

The `graph` class is defined in the [gl/graph.hpp](/include/gl/graph.hpp) header file, which includes most of the elements required to work with the graph structure.

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

- `directed_graph_traits<VertexProperties, EdgeProperties, ImplTag>`
- `undirected_graph_traits<VertexProperties, EdgeProperties, ImplTag>`
- `list_graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties>`
- `matrix_graph_traits<EdgeDirectionalTag, VertexProperties, EdgeProperties>`

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

### General Operations

- **`graph()`**:
  - Default constructor. Creates an empty graph with no vertices or edges.

- **`graph(n_vertices)`**:
  - Constructs a graph with the specified number of vertices. Each vertex is initialized with default properties and no adjacent edges.

> [!IMPORTANT]
> Constructing the graph with the number of vertices parameter is more efficient, as it avoids multiple vector reallocations which could happen when creating an empty graph and adding vertices one by one.

- **Move constructor and assignment operator**: *default*

- **Destructor**: *default*

- **Deleted**:
  - Copy constructor and assignment operator: Copying a graph is explicitly disallowed to prevent unintentional duplication of potentially large graph structures and ensure performance efficiency.

<br />

### Size operations

- **`graph.n_vertices() const`**:
  - *Description*: Returns the total number of vertices in the graph.
  - *Returned value*: $|V|$ where $V$ is the vertex set of the graph
  - *Return type*: `types::size_type`

- **`graph.n_unique_edges() const`**:
  - Returns the number of unique edges in the graph.
  - *Returned value*: $|E|$ where $E$ is the edge set of the graph
  - *Return type*: `types::size_type`

<br />

### Vertex Operations

- **`graph.vertices() const`**:
  - *Description*: Returns an iterator range over all vertices in the graph.
  - *Returned value*: $V$
  - *Return type*: `types::iterator_range<vertex_iterator_type>`

- **`graph.vertex_ids() const`**:
  - *Description*: Returns a range of vertex IDs, starting from the initial vertex ID to the number of vertices in the grap.
  - *Returned value*: $(v_{id} : v \in V)$
  - *Return type*: `std::ranges::iota_view<types::id_type, types::id_type>`

- **`graph.get_vertex(vertex_id) const`**:
  - *Description*: Retrieves the vertex object associated with the given vertex ID.
  - *Returned value*: $v \in V : v_{id} = \text{vertex-id}$
  - *Parameters*:
    - `vertex_id: const types::id_type`
  - *Return type*: `const vertex_type&`

- **`graph.has_vertex(vertex_id) const`**:
  - *Description*: Checks if a vertex exists for the given vertex ID.
  - *Returned value*: $\exists!(v \in V : v_{id} = \text{vertex-id})$
  - *Parameters*:
    - `vertex_id: const types::id_type`
  - *Return type*: `bool`

- **`graph.has_vertex(vertex) const`**:
  - *Description*:  Checks if the given vertex object exists in the graph by verifying its ID as well as its memory address.
  - *Returned value*: $\exists!(v \in V : v_{id} = id(\text{vertex})) \land addr(v) = addr(\text{vertex})$
  - *Parameters*:
    - `vertex: const vertex_type&`
  - *Return type*: `bool`

- **`graph.add_vertex()`**:
  - *Description*: Adds a new vertex to the graph with default properties and returns a reference to the newly added vertex.
  - *Return type*: `const vertex_type&`

- **`graph.add_vertex(properties)`**:
  - *Description*: Adds a new vertex with specified properties and returns a reference to the newly added vertex. This overload is available when the vertex properties type is not the default.
  - *Parameters*:
    - `properties: const vertex_properties_type&` – Properties to assign to the new vertex.
  - *Return type*: `const vertex_type&`
  - *Requires*: non-default `vertex_properties_type`

- **`graph.add_vertices(n)`**:
  - *Description*: Adds `n` new vertices to the graph, each with default properties.
  - *Parameters*:
    - `n: types::size_type` – The number of vertices to add.
  - *Return type*: `void`

- **`graph.add_vertices_with(properties_range)`**:
  - *Description*: Adds multiple vertices to the graph, each with the corresponding properties from the given range. The number of vertices added is determined by the size of `properties_range`.
  - *Template parameters*:
    - `VertexPropertiesRange: type_traits::c_sized_range_of<vertex_properties_type>` – A range of vertex properties that must satisfy the size and type constraints.
  - *Parameters*:
    - `properties_range: const VertexPropertiesRange&` – A range of properties to assign to each of the new vertices.
  - *Return type*: `void`
  - *Requires*: non-default `vertex_properties_type`

> [!IMPORTANT]
> Simliarily to the graph constructors using the `add_vertices*` methods is more efficient than calling `add_vertex` multiple times which might cause more vector reallocations.

- **`graph.remove_vertex(vertex_id)`**:
  - *Description*: Removes the vertex with the given ID from the graph.
  - *Parameters*:
    - `vertex_id: types::size_type` – The ID of the vertex to remove.
  - *Return type*: `void`

- **`graph.remove_vertex(vertex)`**:
  - *Description*: Removes the specified vertex from the graph.
  - *Parameters*:
    - `vertex: const vertex_type&` – A reference to the vertex to remove.
  - *Return type*: `void`

- **`graph.remove_vertices_from(vertex_id_range)`**:
  - *Description*: Removes multiple vertices from the graph based on a range of vertex IDs. The IDs are sorted in descending order and duplicate IDs are removed before deletion.
  - *Template parameters*:
    - `IdRange: type_traits::c_sized_range_of<types::id_type>` – A range of vertex IDs, which must satisfy the size and type constraints.
  - *Parameters*:
    - `vertex_id_range: const IdRange&` – A range of vertex IDs to remove.
  - *Return type*: `void`

- **`graph.remove_vertices_from(vertex_ref_range)`**:
  - *Description*: Removes multiple vertices from the graph based on a range of vertex references. The references are sorted in descending order and duplicates are removed before deletion.
  - *Template parameters*:
    - `VertexRefRange: type_traits::c_sized_range_of<types::const_ref_wrap<vertex_type>>` – A range of vertex references that must satisfy the size and type constraints.
  - *Parameters*:
    - `vertex_ref_range: const VertexRefRange&` – A range of vertex references to remove.
  - *Return type*: `void`

- **`graph.in_degree(vertex) const`**:
  - *Description*: Returns the in-degree (number of incoming edges) of the specified vertex.
  - *Returned value*:
    - For directed graphs: $|\{(u, v) \in E : \text{vertex} = v\}|$
    - For unidirected graphs: $deg(vertex)$
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex for which to calculate the in-degree.
  - *Return type*: `types::size_type`

- **`graph.in_degree(vertex_id) const`**:
  - *Description*: Returns the in-degree (number of incoming edges) of the vertex with the specified ID.
  - *Returned value*:
    - For directed graphs: $|\{(u, v) \in E : \text{vertex-id} = v_{id}\}|$
    - For unidirected graphs: $deg(\text{vertex-id})$
  - *Parameters*:
    - `vertex_id: types::id_type` – the ID of the vertex for which to calculate the in-degree.
  - *Return type*: `types::size_type`

- **`graph.out_degree(vertex) const`**:
  - *Description*: Returns the out-degree (number of outgoing edges) of the specified vertex.
  - *Returned value*:
    - For directed graphs: $|\{(u, v) \in E : \text{vertex} = u\}|$
    - For unidirected graphs: $deg(vertex)$
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex for which to calculate the out-degree.
  - *Return type*: `types::size_type`

- **`graph.out_degree(vertex_id) const`**:
  - *Description*: Returns the out-degree (number of outgoing edges) of the vertex with the specified ID.
  - *Returned value*:
    - For directed graphs: $|\{(u, v) \in E : \text{vertex-id} = u_{id}\}|$
    - For unidirected graphs: $deg(\text{vertex-id})$
  - *Parameters*:
    - `vertex_id: types::id_type` – the ID of the vertex for which to calculate the out-degree.
  - *Return type*: `types::size_type`

- **`graph.degree(vertex) const`**:
  - *Description*: Returns the degree (number of incoming and outgoing edges) of the specified vertex.
  - *Returned value*:
    - For directed graphs: $deg_{in}(\text{vertex}) + deg_{out}(\text{vertex})$
    - For unidirected graphs:

      $2 \times |\{(u, v) \in E : u = v \land (\text{vertex} \in \{u, v\})\}| + |\{(u, v) \in E : u \ne v \land (\text{vertex} \in \{u, v\})\}|$

  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex for which to calculate the degree.
  - *Return type*: `types::size_type`

- **`graph.degree(vertex_id) const`**:
  - *Description*: Returns the degree (number of incoming and outgoing edges) of the vertex with the specified ID.
  - *Returned value*:
    - For directed graphs: $deg_{in}(vertex) + deg_{out}(vertex)$
    - For unidirected graphs:

      $2 \times |\{(u, v) \in E : u = v \land (\text{vertex-id} \in \{u_{id}, v_{id}\})\}| + |\{(u, v) \in E : u \ne v \land (\text{vertex-id} \in \{u_{id}, v_{id}\})\}|$
  - *Parameters*:
    - `vertex_id: types::id_type` – the ID of the vertex for which to calculate the degree.
  - *Return type*: `types::size_type`

<br />

### Edge Operations

- **`graph.add_edge(first_id, second_id)`**:
  - *Description*: Adds a new edge between the vertices with the specified IDs and returns a reference to the newly added edge.
  - *Parameters*:
    - `first_id: types::id_type` – the ID of the first vertex.
    - `second_id: types::id_type` – the ID of the second vertex.
  - *Return type*: `const edge_type&`

- **`graph.add_edge(first_id, second_id, properties)`**:
  - *Description*: Adds a new edge between the vertices with the specified IDs and returns a reference to the newly added edge. This overload is available when the edge properties type is not the default.
  - *Parameters*:
    - `first_id: types::id_type` – the ID of the first vertex.
    - `second_id: types::id_type` – the ID of the second vertex.
    - `properties: const edge_properties_type&` – properties to assign to the new edge.
  - *Return type*: `const edge_type&`
  - *Requires*: non-default `edge_properties_type`

- **`graph.add_edge(first, second)`**:
  - *Description*: Adds a new edge between the specified vertices and returns a reference to the newly added edge.
  - *Parameters*:
    - `first: const vertex_type&` – the first vertex.
    - `second: const vertex_type&` – the second vertex.
  - *Return type*: `const edge_type&`

- **`graph.add_edge(first, second, properties)`**:
  - *Description*: Adds a new edge between the specified vertices and returns a reference to the newly added edge. This overload is available when the edge properties type is not the default.
  - *Parameters*:
    - `first: const vertex_type&` – the first vertex.
    - `second: const vertex_type&` – the second vertex.
    - `properties: const edge_properties_type&` – properties to assign to the new edge.
  - *Return type*: `const edge_type&`
  - *Requires*: non-default `edge_properties_type`

- **`graph.add_edges_from(source_id, destination_id_range)`**:
  - *Description*: Adds multiple edges from a source vertex (specified by ID) to a range of destination vertices (also specified by IDs).
  - *Template parameters*:
    - `IdRange: type_traits::c_sized_range_of<types::id_type>` – a range of vertex IDs that must satisfy the size and type constraints.
  - *Parameters*:
    - `source_id: types::id_type` – the ID of the source vertex.
    - `destination_id_range: const IdRange&` – a range of destination vertex IDs to connect to the source vertex.
  - *Return type*: `void`

- **`graph.add_edges_from(source, destination_range)`**:
  - *Description*: Adds multiple edges from a specified source vertex to a range of destination vertices (specified by references).
  - *Template parameters*:
    - `VertexRefRange: type_traits::c_sized_range_of<types::const_ref_wrap<vertex_type>>` – a range of vertex references that must satisfy the size and type constraints.
  - *Parameters*:
    - `source: const vertex_type&` – the source vertex.
    - `destination_range: const VertexRefRange&` – a range of destination vertex references to connect to the source vertex.
  - *Return type*: `void`

> [!IMPORTANT]
> Behaviour of adding an edge between `first` and `second`:
>
> - for *directed* graphs: adds a one-directional edge where `first` is the source vertex and `second` is the destination vertex
> - for *undirected* graphs: adds a bidirectional edge where both vertices are source and destination vertices

- **`graph.has_edge(first_id, second_id) const`**:
  - *Description*: Returns true if there is an edge between the vertices with the specified IDs.
  - *Returned value*:
    - For directed graphs: $\exists((u, v) \in E : \text{first-id} = u_{id} \land \text{second-id} = v_{id})$
    - For unidirected graphs: $\exists((u, v) \in E : (\text{first-id}, \text{second-id}) \in \{(u_{id}, v_{id}), (v_{id}, u_{id})\})$
  - *Parameters*:
    - `first_id: types::id_type` – the ID of the first vertex.
    - `second_id: types::id_type` – the ID of the second vertex.
  - *Return type*: `bool`

- **`graph.has_edge(first, second) const`**:
  - *Description*: Returns true if there is an edge between the specified vertices.
  - *Returned value*:
    - For directed graphs: $\exists((u, v) \in E : \text{first} = u \land \text{second} = v)$
    - For unidirected graphs: $\exists((u, v) \in E : (\text{first}, \text{second}) \in \{(u, v), (v, u)\})$
  - *Parameters*:
    - `first: const vertex_type&` – the first vertex.
    - `second: const vertex_type&` – the second vertex.
  - *Return type*: `bool`

- **`graph.has_edge(edge) const`**:
  - *Description*: Returns true if the specified edge exists in the graph (**NOTE:** the edge is matched by it's vertices and address).
  - *Parameters*:
    - `edge: const edge_type&` – the edge to check for existence.
  - *Return type*: `bool`

- **`graph.get_edge(first_id, second_id) const`**:
  - *Description*: Returns an optional reference to the edge between the vertices with the specified IDs. If no edge exists, `std::nullopt` is returned (**NOTE:** for the `adjacency_list` implementation the first matchin edge is returned).
  - *Parameters*:
    - `first_id: types::id_type` – the ID of the first vertex.
    - `second_id: types::id_type` – the ID of the second vertex.
  - *Return type*: `types::optional_cref<edge_type>`

- **`graph.get_edge(first, second) const`**:
  - *Description*: Returns an optional reference to the edge between the specified vertices. If either vertex does not exist or if no edge exists between them, `std::nullopt` is returned for the `adjacency_list` implementation the first matchin edge is returned.
  - *Parameters*:
    - `first: const vertex_type&` – the first vertex.
    - `second: const vertex_type&` – the second vertex.
  - *Return type*: `types::optional_cref<edge_type>`

- **`graph.get_edges(first_id, second_id) const`**:
  - *Description*: Returns a vector of optional references to the edges between the vertices with the specified IDs. If no edges exist, returns an empty vector.
  - *Parameters*:
    - `first_id: types::id_type` – the ID of the first vertex.
    - `second_id: types::id_type` – the ID of the second vertex.
  - *Return type*: `std::vector<types::const_ref_wrap<edge_type>>`

- **`graph.get_edges(first, second) const`**:
  - *Description*: Returns a vector of optional references to the edges between the specified vertices. If either vertex does not exist or if no edges exist between them, returns an empty vector.
  - *Parameters*:
    - `first: const vertex_type&` – the first vertex.
    - `second: const vertex_type&` – the second vertex.
  - *Return type*: `std::vector<types::const_ref_wrap<edge_type>>`

- **`graph.remove_edge(edge)`**:
  - *Description*: Removes the specified edge from the graph.
  - *Parameters*:
    - `edge: const edge_type&` – the edge to be removed.
  - *Return type*: `void`

- **`graph.remove_edges_from(edges)`**:
  - *Description*: Removes multiple edges from the graph, as specified by the provided range of edge references.
  - *Template parameters*:
    - `EdgeRefRange: type_traits::c_range_of<types::const_ref_wrap<edge_type>>` – a range of edge references that must satisfy the type constraints.
  - *Parameters*:
    - `edges: const EdgeRefRange&` – a range of edges to remove from the graph.
  - *Return type*: `void`

- **`graph.adjacent_edges(vertex_id) const`**:
  - *Description*: Returns an iterator range of edges that are adjacent to the vertex with the specified ID.
  - *Parameters*:
    - `vertex_id: types::id_type` – the ID of the vertex for which to find adjacent edges.
  - *Return type*: `types::iterator_range<edge_iterator_type>`

- **`graph.adjacent_edges(vertex) const`**:
  - *Description*: Returns an iterator range of edges that are adjacent to the specified vertex.
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex for which to find adjacent edges.
  - *Return type*: `types::iterator_range<edge_iterator_type>`

<br />

### Incidence Operations

- **`graph.are_incident(first_id, second_id) const`**:
  - *Description*: Returns true if the vertices with the specified IDs are incident to each other or the IDs are the same.
  - *Returned value*: $\exists(e_1, e_2 \in E : \text{first-id} \in ids(e_1) \land \text{second-id} \in ids(e_2) \land vertices(e_1) \cap vertices(e_2) \ne \emptyset)$
  - *Parameters*:
    - `first_id: types::id_type` – the ID of the first vertex.
    - `second_id: types::id_type` – the ID of the second vertex.
  - *Return type*: `bool`

- **`graph.are_incident(first, second) const`**:
  - *Description*: Returns true if the specified vertices are incident to each other or if they are the same.
  - *Returned value*: $\exists(e_1, e_2 \in E : \text{first} \in vertices(e_1) \land \text{second} \in vertices(e_2) \land vertices(e_1) \cap vertices(e_2) \ne \emptyset)$
  - *Parameters*:
    - `first: const vertex_type&` – the first vertex.
    - `second: const vertex_type&` – the second vertex.
  - *Return type*: `bool`

- **`graph.are_incident(vertex, edge) const`**:
  - *Description*: Returns true if the given vertex is incident with the given edge.
  - *Returned value*: $\text{vertex} \in vertices(\text{edge})$
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex to check for incidence.
    - `edge: const edge_type&` – the edge to check against.
  - *Return type*: `bool`

- **`graph.are_incident(edge, vertex) const`**:
  - *Description*: Returns true if the given vertex is incident with the given edge.
  - *Returned value*: $\text{vertex} \in vertices(\text{edge})$
  - *Parameters*:
    - `edge: const edge_type&` – the edge to check for incidence.
    - `vertex: const vertex_type&` – the vertex to check against.
  - *Return type*: `bool`

- **`graph.are_incident(edge_1, edge_2) const`**:
  - *Description*: Returns true if the given edges are incident.
  - *Returned value*: $vertices(\text{edge-1}) \cap vertices(\text{edge-2}) \ne \emptyset$
  - *Parameters*:
    - `edge_1: const edge_type&` – the first edge.
    - `edge_2: const edge_type&` – the second edge.
  - *Return type*: `bool`

> [!CAUTION]
> All graph operations which take vertex ids, vertex references or edge references as parameters throw the `std::invalid_argument` exception if such parameter is invalid.

<br />
<br />

## Additional utility

In addition to the core functionality of the `graph` class, the [gl/graph_utility.hpp](/include/gl/graph_utility.hpp) file provides a set of utility functions and type traits that offer extended support for graph manipulation and property handling. Below is an overview of the key utilities provided.

### Type traits

To write safe and more expressive graph utility of your own, you can use the defined concepts and type traits:

> [!NOTE]
> All concepts and in the `CPP-GL` library are defined in the `gl::type_trais` namespace and are prefixed with `c_`.

| **Trait** | **Description** |
| :- | :- |
| `c_graph<T>` | Ensures that the template parameter `T` is a specialization of the `graph` class |
| `c_directed_graph<T>` | Equivalent to `c_graph<T> and is_directed_v<T>`<br/>Ensures that the template parameter `T` is a *directed* specialization of the `graph` class |
| `c_undirected_graph<T>` | Equivalent to `c_graph<T> and is_undirected_v<T>`<br/>Ensures that the template parameter `T` is an *undirected* specialization of the `graph` class |

> [!TIP]
> More (not graph class specific) type traits and concepts are defined in the [gl/types/traits/](/include/gl/types/traits/) directory.

### Graph element properties utility

#### Types

> [!NOTE]
> All of the utility types listed below are defined in the `gl::types` namespace

- `default_vertex_distance_type = std::int64_t` : Defines the default type for distance between vertices. This is used in the absence of a weight type definition in the `edge_properties_type` of a graph.
- `vertex_distance`
  - *Description*: A type trait which determines the type to be used for vertex distances based on the `c_weight_properties_type` concept:
  - *Template parameters*:
    - `GraphType: type_traits::c_graph`
    - If the graph's `edge_properties_type` includes a `weight_type` definition, that type is used
    - Otherwise, the default distance type is used.
  - *Members*:
    - `type` - the vertex distance type definition (defined as above).
- `vertex_distance_type<GraphType>` : An alias for `typename vertex_distance<GraphType>::type`

#### Functions

- `get_weight(edge)`
  - *Description*: Returns the weight of the given edge based on the `edge_properties_type` of the graph.
  - *Template parameters*:
    - `GraphType: type_traits::c_graph`
  - *Parameters*:
    - `edge: const typename GraphType::edge_type&` - the edge to get weight from
  - *Returned value*:
    - `edge.properties.weight` if the `edge_properties_type` satisfies the `type_traits::c_weight_properties_type` concept
    - `static_cast<types::default_vertex_distance_type>(1ll)` otherwise
  - *Return type*: `types::vertex_distance_type<GraphType>`

<br />
<br />

## Related pages

This section provides links to additional resources and documentation that can help you further explore the capabilities of the `CPP-GL` library.

- [The vertex and edge classes - representation of the graph's elements](/docs/graph_elements.md)
- [I/O operations](/docs/io.md)
- [Graph topology generators](/docs/topologies.md)
- [Algorithms](/docs/algoithms.md)
- [Core utility types](/docs/core_util_types.md)
