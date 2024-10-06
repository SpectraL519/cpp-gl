# The vertex and edge classes - representation of the graph's elements

This section provides an overview of the `vertex_descriptor` and `edge_descriptor` classes, which are core components of the graph structure. These classes are responsible for representing the vertices and edges in the graph, along with any associated properties. Both classes are designed to be lightweight, highly customizable, and provide essential operations for managing graph elements.

<br />

## Table of contents

- [The vertex class](#the-vertex-class)
- [The edge class](#the-edge-class)
- [Related pages](#related-pages)

<br />

## The vertex class

The `vertex_descriptor` class represents the identity of a vertex in the graph, optionally carrying additional properties as specified by the user. This class is designed for use within the `Graph` class and serves as a lightweight identifier for vertices, providing an interface for accessing vertex properties, comparing vertex descriptors, and outputting vertex information.

By default, the `vertex_descriptor` class does not carry any properties. However, properties can be associated with each vertex by passing a custom type as the template parameter `Properties`, making it highly flexible and customizable for various graph use cases.

### Template parameters

- **`Properties`**: A type that defines the properties associated with each vertex.
  - *Default value*:  `types::empty_properties`
  - *Constraints*: must satisfy the **`type_traits::c_properties`** concept

### Member types

- **`type`**: Alias for the `vertex_descriptor` itself.
- **`properties_type`**: Type of the vertex properties as defined by the `Properties` template parameter.

### Constructors

- **`vertex_descriptor(const types::id_type id)`**:
  - Constructs a `vertex_descriptor` with a unique vertex ID.
- **`vertex_descriptor(const types::id_type id, const properties_type& properties)`**:
  - Constructs a `vertex_descriptor` with a unique ID and specified properties.
  - *Constraints*: the `properties_type` must be non-default.
- **Move constructor and assignment operator**: *default*

- **Deleted**:
  - **Default constructor**: prevent creating multiple vertices with the default ID within a graph.
  - **Copy constructor and assignment operator**: prevent copying of `vertex_descriptor` instances, as they are meant to be identificators.

### Desctructor

The destructor is *defaulted*, allowing proper cleanup of the `vertex_descriptor` when it goes out of scope.

### Member functions

- **`id() const`**:
  - *Description*: Returns the unique identifier of the vertex.
  - *Return type*: `types::id_type`

- **`operator==(const vertex_descriptor& other) const`**:
  - *Description*: Compares two `vertex_descriptor` objects for equality based on their vertex IDs.
  - *Parameters*:
    - `other: const vertex_descriptor&` – the vertex descriptor to compare with.
  - *Return type*: `bool`

- **`operator<=>(const vertex_descriptor& other) const`**:
  - *Description*: Provides three-way comparison for `vertex_descriptor` objects, based on the vertex ID.
  - *Parameters*:
    - `other: const vertex_descriptor&` – the vertex descriptor to compare with.
  - *Return type*: `std::strong_ordering`

### Member variables

- **`properties`**:
  - *Description*: A mutable member that stores the properties associated with the vertex.
  - *Type*: Defined by the `Properties` template parameter.

### Additional utility

- **`vertex`**: A convenient alias for `vertex_descriptor` with customizable properties.

  ```cpp
  template <type_traits::c_properties Properties = types::empty_properties>
  using vertex = vertex_descriptor<Properties>;
  ```

<br />

## The edge class

The `edge_descriptor` class represents an edge between two vertices $(u, v)$ in the graph, with optional properties and support for both directed and undirected edges. This class is designed to work within the `Graph` class, handling relationships between vertex descriptors and providing a flexible interface for edge manipulation, comparison, and output.

By default, the `edge_descriptor` class does not carry any properties and assumes the edge is directed. However, these can be customized by passing appropriate types for the template parameters `DirectionalTag` and `Properties`.

### Template parameters

- **`VertexType`**: The type of the vertex descriptors connected by this edge.
  - *Constraints*: must satisfy the **`type_traits::c_instantiation_of<vertex_descriptor>`** concept
- **`DirectionalTag`**: A tag type indicating whether the edge is directed or undirected.
  - *Default value*: `directed_t`
  - *Constraints*: must satisfy the **`type_traits::c_edge_directional_tag`** concept (either `directed_t` or `undirected_t`)
- **`Properties`**: A type that defines the properties associated with each edge.
  - *Default value*: `types::empty_properties`
  - *Constraints*: must satisfy the **`type_traits::c_properties`** concept

### Member types

- **`type`**: Alias for the `edge_descriptor` itself.
- **`vertex_type`**: Type of the vertex descriptors connected by this edge.
- **`directional_tag`**: The tag indicating whether the edge is directed or undirected.
- **`properties_type`**: Type of the edge properties as defined by the `Properties` template parameter.

### Constructors

- **`edge_descriptor(const vertex_type& first, const vertex_type& second)`**:
  - Constructs an edge between two vertices.
- **`edge_descriptor(const vertex_type& first, const vertex_type& second, const properties_type& properties)`**:
  - Constructs an edge between two vertices with specified properties.
  - *Constraints*: the `properties_type` must be non-default.
- **Move constructor and assignment operator**: *default*

- **Deleted**:
  - **Default constructor**: prevents creating edges without specifying incident vertices.
  - **Copy constructor and assignment operator**: prevent copying of `edge_descriptor` instances, as they are meant to uniquely identify edges.

### Destructor

The destructor is *defaulted*, allowing proper cleanup of the `edge_descriptor` when it goes out of scope.

### Member functions

- **`is_directed() const`**:
  - *Description*: Returns `true` if the edge is directed.
  - *Return type*: `bool`

- **`is_undirected() const`**:
  - *Description*: Returns `true` if the edge is undirected.
  - *Return type*: `bool`

- **`incident_vertices() const`**:
  - *Description*: Returns a pair of references to the two vertices connected by the edge.
  - *Returned value*: $(u, v)$
  - *Return type*: `types::homogeneous_pair<const vertex_type&>`

- **`first() const`**:
  - *Description*: Returns a reference to the first vertex of the edge.
  - *Returned value*: $u$
  - *Return type*: `const vertex_type&`

- **`second() const`**:
  - *Description*: Returns a reference to the second vertex of the edge.
  - *Returned value*: $v$
  - *Return type*: `const vertex_type&`

- **`incident_vertex_ids() const`**:
  - *Description*: Returns the unique IDs of the vertices connected by the edge.
  - *Returned value*: $(u_{id}, v_{id})$
  - *Return type*: `types::homogeneous_pair<types::id_type>`

- **`first_id() const`**:
  - *Description*: Returns the ID of the first vertex.
  - *Returned value*: $u_{id}$
  - *Return type*: `types::id_type`

- **`second_id() const`**:
  - *Description*: Returns the ID of the second vertex.
  - *Returned value*: $v_{id}$
  - *Return type*: `types::id_type`

- **`incident_vertex(const vertex_type& vertex) const`**:
  - *Description*: Returns the vertex on the other end of the edge relative to the provided vertex. Throws an error if the provided vertex is not incident with the edge.
  - *Returned value*: $\begin{cases} v & \text{: vertex} = u \\ u & \text{: otherwise} \end{cases}$
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex for which the opposite incident vertex is requested.
  - *Return type*: `const vertex_type&`

- **`is_incident_with(const vertex_type& vertex) const`**:
  - *Description*: Returns `true` if the provided vertex is connected to the edge.
  - *Returned value*: $\text{vertex} \in {u, v}$
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex to check for incidence with the edge.
  - *Return type*: `bool`

- **`is_incident_from(const vertex_type& vertex) const`**:
  - *Description*: Returns `true` if the provided vertex is the source of the edge (for directed edges).
  - *Returned value*:
    - For directed edges: $\text{vertex} = u$
    - For undirected edges: `is_incident_with(vertex)`
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex to check if it is the source.
  - *Return type*: `bool`

- **`is_incident_to(const vertex_type& vertex) const`**:
  - *Description*: Returns `true` if the provided vertex is the destination of the edge (for directed edges).
  - *Returned value*:
    - For directed edges: $\text{vertex} = v$
    - For undirected edges: `is_incident_with(vertex)`
  - *Parameters*:
    - `vertex: const vertex_type&` – the vertex to check if it is the destination.
  - *Return type*: `bool`

- **`is_loop() const`**:
  - *Description*: Returns `true` if the edge is a loop (i.e., both vertices are the same).
  - *Returned value*: $u = v$
  - *Return type*: `bool`

### Member variables

- **`properties`**:
  - *Description*: A mutable member that stores the properties associated with the edge.
  - *Type*: Defined by the `Properties` template parameter.

### Additional utility

- **`edge`**: A convenient alias for `edge_descriptor`, with customizable properties and directionality.

  ```cpp
  template <
      type_traits::c_instantiation_of<vertex_descriptor> VertexType,
      type_traits::c_edge_directional_tag DirectionalTag = directed_t,
      type_traits::c_properties Properties = types::empty_properties>
  using edge = edge_descriptor<VertexType, DirectionalTag, Properties>;
  ```

- `directed_edge`: An alias for `edge_descriptor` specifically for directed edges.

  ```cpp
  template <
      type_traits::c_instantiation_of<vertex_descriptor> VertexType,
      type_traits::c_properties Properties = types::empty_properties>
  using directed_edge = edge_descriptor<VertexType, directed_t, Properties>;
  ```

- `undirected_edge`: An alias for `edge_descriptor` specifically for undirected edges.

  ```cpp
  template <
      type_traits::c_instantiation_of<vertex_descriptor> VertexType,
      type_traits::c_properties Properties = types::empty_properties>
  using undirected_edge = edge_descriptor<VertexType, undirected_t, Properties>;
  ```

<br />
<br />

## Related pages

- [The graph class - library's core](/docs/graph.md)
- [I/O operations](/docs/io.md)
<!-- TODO: types page -->
