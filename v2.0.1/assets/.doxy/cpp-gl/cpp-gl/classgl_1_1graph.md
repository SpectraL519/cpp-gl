

# Class gl::graph

**template &lt;traits::c\_instantiation\_of&lt; [**graph\_traits**](structgl_1_1graph__traits.md) &gt; GraphTraits&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**graph**](classgl_1_1graph.md)



_The generic graph container using a policy-based architecture._ [More...](#detailed-description)

* `#include <gl/graph.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**GraphTraits**](group__GL-Core.md#variable-invalid_id_v) | [**traits\_type**](#typedef-traits_type)  <br>_Traits type specifying the behavior and representation._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::directional\_tag | [**directional\_tag**](#typedef-directional_tag)  <br>_Type tag specifying the directionality of the graph._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::representation\_tag | [**representation\_tag**](#typedef-representation_tag)  <br>_Type tag indicating the underlying representation model._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) representation\_tag::template type&lt; [**traits\_type**](classgl_1_1graph.md#typedef-traits_type) &gt; | [**implementation\_type**](#typedef-implementation_type-12)  <br>_The underlying representation type matching the directional tag._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::id\_type | [**id\_type**](#typedef-id_type)  <br>_Integral type used to identify vertices and edges._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::vertex\_type | [**vertex\_type**](#typedef-vertex_type)  <br>_The descriptor type representing a vertex._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::vertex\_properties\_type | [**vertex\_properties\_type**](#typedef-vertex_properties_type)  <br>_Type representing the properties attached to a vertex._  |
| typedef std::conditional\_t&lt; traits::c\_empty\_properties&lt; [**vertex\_properties\_type**](classgl_1_1graph.md#typedef-vertex_properties_type) &gt;, [**empty\_properties\_map**](structgl_1_1empty__properties__map.md), std::vector&lt; [**vertex\_properties\_type**](classgl_1_1graph.md#typedef-vertex_properties_type) &gt; &gt; | [**vertex\_properties\_map\_type**](#typedef-vertex_properties_map_type)  <br>_Type mapping vertex IDs to their respective properties._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::edge\_type | [**edge\_type**](#typedef-edge_type)  <br>_The descriptor type representing an edge._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) traits\_type::edge\_properties\_type | [**edge\_properties\_type**](#typedef-edge_properties_type)  <br>_Type representing the properties attached to an edge._  |
| typedef std::conditional\_t&lt; traits::c\_empty\_properties&lt; [**edge\_properties\_type**](classgl_1_1graph.md#typedef-edge_properties_type) &gt;, [**empty\_properties\_map**](structgl_1_1empty__properties__map.md), std::vector&lt; [**edge\_properties\_type**](classgl_1_1graph.md#typedef-edge_properties_type) &gt; &gt; | [**edge\_properties\_map\_type**](#typedef-edge_properties_map_type)  <br>_Type mapping edge IDs to their respective properties._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**friend**](group__GL-Core.md#variable-invalid_id_v) | [**implementation\_type**](#variable-implementation_type-22)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**graph**](#function-graph-14) () = default<br>_Default constructor creates an empty graph._  |
|   | [**graph**](#function-graph-24) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](group__GL-Core.md#typedef-size_type) n\_vertices) <br>_Constructs an empty graph with a pre-allocated number of vertices._  |
|   | [**graph**](#function-graph-34) ([**graph**](classgl_1_1graph.md) &&) noexcept<br>_Default move constructor._  |
|  [**graph**](classgl_1_1graph.md) & | [**operator=**](#function-operator) ([**graph**](classgl_1_1graph.md) &&) noexcept<br>_Default move assignment operator._  |
|   | [**~graph**](#function-graph) () = default<br>_Default destructor._  |
|  [**graph**](classgl_1_1graph.md) & | [**operator=**](#function-operator_1) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**graph**](classgl_1_1graph.md) &) = delete<br>_Graph copy assignment is disabled to avoid accidental copies. Use_ [_**clone**_](group__GL-Core.md#function-clone) _instead._ |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**n\_vertices**](#function-n_vertices) () noexcept const<br>_Returns the total number of vertices in the graph._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**n\_edges**](#function-n_edges) () noexcept const<br>_Returns the total number of edges in the graph._  |
|  [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) | [**add\_vertex**](#function-add_vertex) () <br>_Adds a new, default-initialized vertex to the graph._  |
|  [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) | [**add\_vertex\_with**](#function-add_vertex_with) ([**vertex\_properties\_type**](classgl_1_1graph.md#typedef-vertex_properties_type) properties) <br>_Adds a new vertex with the given properties to the graph._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**add\_vertices**](#function-add_vertices) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](group__GL-Core.md#typedef-size_type) n) <br>_Adds a specified number of default-initialized vertices to the graph._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**add\_vertices\_with**](#function-add_vertices_with) ([**const**](group__GL-Core.md#variable-invalid_id_v) traits::c\_sized\_range\_of&lt; [**vertex\_properties\_type**](classgl_1_1graph.md#typedef-vertex_properties_type) &gt; [**auto**](group__GL-Core.md#variable-invalid_id_v) & properties\_rng) <br>_Adds multiple vertices based on a range of property payloads._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove\_vertex**](#function-remove_vertex-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) <br>_Removes a vertex by its ID, removing all associated incident edges._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove\_vertex**](#function-remove_vertex-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) <br>_Removes a vertex using its descriptor._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove\_vertices**](#function-remove_vertices-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classgl_1_1graph.md#typedef-id_type) &gt; [**auto**](group__GL-Core.md#variable-invalid_id_v) & vertex\_id\_rng) <br>_Removes a range of vertices using their IDs._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove\_vertices**](#function-remove_vertices-22) ([**const**](group__GL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) &gt; [**auto**](group__GL-Core.md#variable-invalid_id_v) & vertex\_rng) <br>_Removes a range of vertices using their descriptors._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**has\_vertex**](#function-has_vertex-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Checks if a vertex with the given ID exists in the graph._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**has\_vertex**](#function-has_vertex-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Checks if the given vertex descriptor is valid in the graph._  |
|  [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) | [**vertex**](#function-vertex) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Returns a vertex descriptor bounds-checked by ID._  |
|  [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) | [**at**](#function-at) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Returns a vertex descriptor bounds-checked by ID (alias for_ `vertex` _)._ |
|  [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) | [**vertex\_unchecked**](#function-vertex_unchecked) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) noexcept const<br>_Returns a vertex descriptor without bounds checking._  |
|  [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) | [**operator[]**](#function-operator_2) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) noexcept const<br>_Returns a vertex descriptor without bounds checking (array access style)._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**vertices**](#function-vertices) () noexcept const<br>_Returns a lazily evaluated view of all vertex descriptors in the graph._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**vertex\_ids**](#function-vertex_ids) () noexcept const<br>_Returns a lazily evaluated view of all active vertex IDs in the graph._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**neighbors**](#function-neighbors-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves the neighbor vertex IDs for a specific vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**neighbors**](#function-neighbors-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves the neighbor vertex descriptors for a specific vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**neighbor\_ids**](#function-neighbor_ids-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves the neighbor vertex IDs for a specific vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**neighbor\_ids**](#function-neighbor_ids-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves the neighbor vertex IDs for a specific vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**predecessors**](#function-predecessors-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves the predecessor vertex IDs for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**predecessors**](#function-predecessors-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves the predecessor vertex descriptors (incoming edges) for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**predecessor\_ids**](#function-predecessor_ids-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves the predecessor vertex IDs for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**predecessor\_ids**](#function-predecessor_ids-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves the predecessor vertex IDs for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**successors**](#function-successors-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves the successor vertex IDs for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**successors**](#function-successors-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves the successor vertex descriptors (outgoing edges) for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**successor\_ids**](#function-successor_ids-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves the successor vertex IDs for a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**successor\_ids**](#function-successor_ids-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves the successor vertex IDs for a vertex._  |
|  [**vertex\_properties\_type**](classgl_1_1graph.md#typedef-vertex_properties_type) & | [**vertex\_properties**](#function-vertex_properties) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) id) const<br>_Retrieves a mutable reference to a vertex's properties._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**vertex\_properties\_map**](#function-vertex_properties_map) () noexcept const<br>_Retrieves a random-access view over all vertex properties in the graph._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**degree**](#function-degree-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Calculates the total degree of a vertex._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**degree**](#function-degree-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Calculates the total degree of a vertex._  |
|  std::vector&lt; [**size\_type**](group__GL-Core.md#typedef-size_type) &gt; | [**degree\_map**](#function-degree_map) () const<br>_Returns a mapped array of total degrees for all vertices._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**in\_degree**](#function-in_degree-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Calculates the in-degree (incoming edges) for a vertex._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**in\_degree**](#function-in_degree-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Calculates the in-degree (incoming edges) for a vertex._  |
|  std::vector&lt; [**size\_type**](group__GL-Core.md#typedef-size_type) &gt; | [**in\_degree\_map**](#function-in_degree_map) () const<br>_Returns a mapped array of in-degrees for all vertices._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**out\_degree**](#function-out_degree-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Calculates the out-degree (outgoing edges) for a vertex._  |
|  [**size\_type**](group__GL-Core.md#typedef-size_type) | [**out\_degree**](#function-out_degree-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Calculates the out-degree (outgoing edges) for a vertex._  |
|  std::vector&lt; [**size\_type**](group__GL-Core.md#typedef-size_type) &gt; | [**out\_degree\_map**](#function-out_degree_map) () const<br>_Returns a mapped array of out-degrees for all vertices._  |
|  [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) | [**add\_edge**](#function-add_edge-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) target\_id) <br>_Creates an edge between a source and target vertex._  |
|  [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) | [**add\_edge\_with**](#function-add_edge_with-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) target\_id, [**edge\_properties\_type**](classgl_1_1graph.md#typedef-edge_properties_type) properties) <br>_Creates an edge carrying specific properties between a source and target vertex._  |
|  [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) | [**add\_edge**](#function-add_edge-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) target) <br>_Creates an edge between a source and target vertex._  |
|  [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) | [**add\_edge\_with**](#function-add_edge_with-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) target, [**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_properties\_type**](classgl_1_1graph.md#typedef-edge_properties_type) & properties) <br>_Creates an edge carrying specific properties between a source and target vertex._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**add\_edges\_from**](#function-add_edges_from-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) traits::c\_sized\_range\_of&lt; [**id\_type**](classgl_1_1graph.md#typedef-id_type) &gt; [**auto**](group__GL-Core.md#variable-invalid_id_v) & target\_id\_rng) <br>_Dispatches multiple edge insertions connecting one source to many targets._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**add\_edges\_from**](#function-add_edges_from-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**const**](group__GL-Core.md#variable-invalid_id_v) traits::c\_sized\_range\_of&lt; [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) &gt; [**auto**](group__GL-Core.md#variable-invalid_id_v) & target\_rng) <br>_Dispatches multiple edge insertions connecting one source to many targets._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove\_edge**](#function-remove_edge) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) & edge) <br>_Removes a specific edge from the graph._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove\_edges**](#function-remove_edges) ([**const**](group__GL-Core.md#variable-invalid_id_v) traits::c\_range\_of&lt; [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) &gt; [**auto**](group__GL-Core.md#variable-invalid_id_v) & edges) <br>_Removes a range of edges from the graph._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**edge\_ids**](#function-edge_ids) () noexcept const<br>_Returns a lazily evaluated view of all active edge IDs in the graph._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**has\_edge**](#function-has_edge-13) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) & edge) const<br>_Verifies if the exact specified edge exists._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**has\_edge**](#function-has_edge-23) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) target\_id) const<br>_Checks if there is any connecting edge from source to target._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**has\_edge**](#function-has_edge-33) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) target) const<br>_Checks if there is any connecting edge from source to target._  |
|  std::optional&lt; [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) &gt; | [**edge**](#function-edge-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) target\_id) const<br>_Retrieves an edge (if it exists) connecting the source to the target._  |
|  std::optional&lt; [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) &gt; | [**edge**](#function-edge-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) target) const<br>_Retrieves an edge (if it exists) connecting the source to the target._  |
|  std::vector&lt; [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) &gt; | [**edges**](#function-edges-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) target\_id) const<br>_Retrieves all parallel edges connecting the source to the target._  |
|  std::vector&lt; [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) &gt; | [**edges**](#function-edges-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) target) const<br>_Retrieves all parallel edges connecting the source to the target._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**incident\_edges**](#function-incident_edges-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves all edges incident with a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**incident\_edges**](#function-incident_edges-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves all incident with a vertex._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**in\_edges**](#function-in_edges-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves all incoming edges of a vertex (going into the vertex)._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**in\_edges**](#function-in_edges-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves all incoming edges of a vertex (going into the vertex)._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**out\_edges**](#function-out_edges-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) vertex\_id) const<br>_Retrieves all outgoing edges of a vertex (going out of the vertex)._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**out\_edges**](#function-out_edges-22) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Retrieves all outgoing edges of a vertex (going out of the vertex)._  |
|  [**edge\_properties\_type**](classgl_1_1graph.md#typedef-edge_properties_type) & | [**edge\_properties**](#function-edge_properties) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) id) const<br>_Retrieves a mutable reference to an edge's properties._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**edge\_properties\_map**](#function-edge_properties_map) () noexcept const<br>_Retrieves a view over all edge properties in the graph._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**are\_adjacent**](#function-are_adjacent-13) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) source\_id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1graph.md#typedef-id_type) target\_id) const<br> |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**are\_adjacent**](#function-are_adjacent-23) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) source, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) target) const<br> |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**are\_adjacent**](#function-are_adjacent-33) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) & edge\_1, [**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) & edge\_2) const<br>_Checks if two distinct edges share at least one incident vertex._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**are\_incident**](#function-are_incident-12) ([**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex, [**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) & edge) const<br>_Checks if a vertex forms one of the endpoints of an edge._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**are\_incident**](#function-are_incident-22) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type**](classgl_1_1graph.md#typedef-edge_type) & edge, [**vertex\_type**](classgl_1_1graph.md#typedef-vertex_type) vertex) const<br>_Checks if a vertex forms one of the endpoints of an edge._  |




























## Detailed Description


This class relies on the provided `GraphTraits` to determine its behavior, element property types, and the underlying memory representation. It exposes a unified API for adding, removing, and iterating over vertices and edges regardless of the backend.


#### Key Features




* **Policy-based design**: Behavior and representation are determined by `GraphTraits`.
* **Zero-cost Abstractions**: Core query logic is resolved at compile time through representation tags and static dispatch, removing unnecessary overhead.
* **Configurable directionality**: Support for both directed and undirected graphs.
* **Multiple representations**: Choose the underlying memory model that best suits your algorithmic and cache-locality needs:
    * [**list\_t**](structgl_1_1repr_1_1list__t.md): Standard adjacency list.
    * [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md): Flattened adjacency list.
    * [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md): Standard adjacency matrix.
    * [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md): Flattened adjacency matrix.


* **Property support**: Vertices and edges can carry arbitrary properties.
* **Unified API**: Consistent interface regardless of the underlying representation.
* **Standard Range Support**: Exposes lightweight views compliant with C++20 `std::ranges`, enabling functional-style iteration and algorithms.





#### Basic Definitions



A graph \(G = (V, E)\) consists of a set of vertices \(V\) and a set of edges \(E\).



* For undirected graphs, edges are unordered pairs \(\{u, v\}\) where \(u, v \in V\).
* For directed graphs, edges are ordered pairs \((u, v)\) where \(u, v \in V\).





#### Example Usage




```cpp
#include <gl/graph.hpp>

#include <iostream>

int main() {
    gl::directed_graph<> g; // (1)!

    auto v0 = g.add_vertex(); // (2)!
    auto v1 = g.add_vertex();
    auto v2 = g.add_vertex();

    auto e01 = g.add_edge(v0, v1); // (3)!
    auto e12 = g.add_edge(v1, v2);
    auto e20 = g.add_edge(v2, v0);

    std::cout << "Vertices: " << g.n_vertices() << '\n'; // (4)!
    std::cout << "Edges: " << g.n_edges() << '\n';

    for (auto neighbor : g.neighbors(v0)) // (5)!
        process(neighbor);

    std::cout << "Topology:\n" << g << '\n'; // (6)!

    return 0;
}
```



1. Create a directed graph with no properties.


2. Add vertices to the graph. **NOTE:** This is safe because the graph has no vertex properties and the `add_vertex` operation does not invalidate IDs.


3. Add edges to the graph. **NOTE:** This is safe because the graph has no edge properties and the `add_edge` operation does not invalidate IDs.


4. Query the graph's properties.


5. Iterate over neighbors of `v0`.


6. Utilize the builtin I/O stream support of the `graph` class to print its topology to the console.



#### API Design: IDs vs. Descriptors



The `graph` class exposes a dual API to accommodate different performance and ergonomic needs:



* **Inputs**: Most query methods are overloaded to accept either a raw `id_type` or a `vertex_type`/`edge_type` descriptor. They are functionally identical.
* **Outputs**: Methods ending in `_ids` (e.g., `neighbor_ids`) return views of raw integral IDs. Methods without this suffix (e.g., `neighbors`) automatically map those IDs to the proper descriptor objects.
* **Performance**: Descriptor-returning methods incur a slight overhead if the graph utilizes properties, as the property reference must be fetched and bound to each descriptor. If you only need topology, prefer the `_ids` variants.





#### Descriptor Invalidation Behavior



The graph maintains the following invalidation semantics:



* **Vertex addition**: Does not invalidate vertex IDs. However, property references stored in existing vertex descriptors may be invalidated. Has no effect on edge descriptors.
* **Vertex removal**: May invalidate both vertex and edge descriptors, IDs, and property references.
* **Edge addition**: Does not invalidate vertex or edge IDs. However, property references stored in edge descriptors may be invalidated.
* **Edge removal**: Invalidates edge descriptors, IDs, and property references. Has no effect on vertex descriptors.
* **Property access**: References to vertex or edge properties obtained from the map may be invalidated by modifications to the graph structure.





#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|GraphTraits|Traits struct specifying the behavior and representation of the graph.|An instantiation of [**graph\_traits**](structgl_1_1graph__traits.md)|







#### See Also




* [**directed\_graph**](group__GL-Core.md#typedef-directed_graph) : Convenience alias for directed graphs.
* [**undirected\_graph**](group__GL-Core.md#typedef-undirected_graph) : Convenience alias for undirected graphs.
* [**clone**](group__GL-Core.md#function-clone) : Create a deep copy of a graph.
* [**to**](group__GL-Core.md#function-to) : Convert a graph to a different representation.





> [!IMPORTANT] Copy Semantics
>
>
> `graph` supports move semantics but disables copy assignment to prevent accidental expensive copies. Use [**clone**](group__GL-Core.md#function-clone) to explicitly copy a graph. 
>
>

> [!WARNING] Const Correctness & Properties (API Note)
>
>
> Currently, a `const` graph guarantees **structural immutability** (vertices and edges cannot be added or removed). However, vertex and edge property maps are internally treated as `mutable`. This means that property payloads can still be modified through a `const graph&`. Strict const-correct overloads for property access are planned for a future release. Proceed with caution in multi-threaded contexts. 
>
>




    
## Public Types Documentation




### typedef traits\_type 

_Traits type specifying the behavior and representation._ 
```cpp
using gl::graph< GraphTraits >::traits_type =  GraphTraits;
```




<hr>



### typedef directional\_tag 

_Type tag specifying the directionality of the graph._ 
```cpp
using gl::graph< GraphTraits >::directional_tag =  typename traits_type::directional_tag;
```




<hr>



### typedef representation\_tag 

_Type tag indicating the underlying representation model._ 
```cpp
using gl::graph< GraphTraits >::representation_tag =  typename traits_type::representation_tag;
```




<hr>



### typedef implementation\_type [1/2]

_The underlying representation type matching the directional tag._ 
```cpp
using gl::graph< GraphTraits >::implementation_type =  typename representation_tag::template type<traits_type>;
```




<hr>



### typedef id\_type 

_Integral type used to identify vertices and edges._ 
```cpp
using gl::graph< GraphTraits >::id_type =  typename traits_type::id_type;
```




<hr>



### typedef vertex\_type 

_The descriptor type representing a vertex._ 
```cpp
using gl::graph< GraphTraits >::vertex_type =  typename traits_type::vertex_type;
```




<hr>



### typedef vertex\_properties\_type 

_Type representing the properties attached to a vertex._ 
```cpp
using gl::graph< GraphTraits >::vertex_properties_type =  typename traits_type::vertex_properties_type;
```




<hr>



### typedef vertex\_properties\_map\_type 

_Type mapping vertex IDs to their respective properties._ 
```cpp
using gl::graph< GraphTraits >::vertex_properties_map_type =  std::conditional_t< traits::c_empty_properties<vertex_properties_type>, empty_properties_map, std::vector<vertex_properties_type> >;
```




<hr>



### typedef edge\_type 

_The descriptor type representing an edge._ 
```cpp
using gl::graph< GraphTraits >::edge_type =  typename traits_type::edge_type;
```




<hr>



### typedef edge\_properties\_type 

_Type representing the properties attached to an edge._ 
```cpp
using gl::graph< GraphTraits >::edge_properties_type =  typename traits_type::edge_properties_type;
```




<hr>



### typedef edge\_properties\_map\_type 

_Type mapping edge IDs to their respective properties._ 
```cpp
using gl::graph< GraphTraits >::edge_properties_map_type =  std::conditional_t< traits::c_empty_properties<edge_properties_type>, empty_properties_map, std::vector<edge_properties_type> >;
```




<hr>
## Public Attributes Documentation




### variable implementation\_type [2/2]

```cpp
friend gl::graph< GraphTraits >::implementation_type;
```




<hr>
## Public Functions Documentation




### function graph [1/4]

_Default constructor creates an empty graph._ 
```cpp
gl::graph::graph () = default
```




<hr>



### function graph [2/4]

_Constructs an empty graph with a pre-allocated number of vertices._ 
```cpp
inline explicit gl::graph::graph (
    const  size_type n_vertices
) 
```





**Parameters:**


* `n_vertices` The initial number of vertices to allocate space for. 




        

<hr>



### function graph [3/4]

_Default move constructor._ 
```cpp
gl::graph::graph (
    graph &&
) noexcept
```




<hr>



### function operator= 

_Default move assignment operator._ 
```cpp
graph & gl::graph::operator= (
    graph &&
) noexcept
```




<hr>



### function ~graph 

_Default destructor._ 
```cpp
gl::graph::~graph () = default
```




<hr>



### function operator= 

_Graph copy assignment is disabled to avoid accidental copies. Use_ [_**clone**_](group__GL-Core.md#function-clone) _instead._
```cpp
graph & gl::graph::operator= (
    const  graph &
) = delete
```




<hr>



### function n\_vertices 

_Returns the total number of vertices in the graph._ 
```cpp
inline size_type gl::graph::n_vertices () noexcept const
```





**Returns:**

The vertex count: $\|V\|$. 





        

<hr>



### function n\_edges 

_Returns the total number of edges in the graph._ 
```cpp
inline size_type gl::graph::n_edges () noexcept const
```





**Returns:**

The edge count: $\|E\|$. 





        

<hr>



### function add\_vertex 

_Adds a new, default-initialized vertex to the graph._ 
```cpp
inline vertex_type gl::graph::add_vertex () 
```





**Returns:**

A descriptor of the newly created vertex.



> [!IMPORTANT] ID Stability
>
>
> Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated. 
>
>



        

<hr>



### function add\_vertex\_with 

_Adds a new vertex with the given properties to the graph._ 
```cpp
inline vertex_type gl::graph::add_vertex_with (
    vertex_properties_type properties
) 
```





**Parameters:**


* `properties` The property payload for the new vertex. 



**Returns:**

A descriptor of the newly created vertex.



> [!IMPORTANT] ID Stability
>
>
> Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated. 
>
>



        

<hr>



### function add\_vertices 

_Adds a specified number of default-initialized vertices to the graph._ 
```cpp
inline void gl::graph::add_vertices (
    const  size_type n
) 
```





**Parameters:**


* `n` The number of vertices to add.


> [!IMPORTANT] ID Stability
>
>
> Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated. 
>
>



        

<hr>



### function add\_vertices\_with 

_Adds multiple vertices based on a range of property payloads._ 
```cpp
inline void gl::graph::add_vertices_with (
    const traits::c_sized_range_of< vertex_properties_type > auto & properties_rng
) 
```





**Parameters:**


* `properties_rng` A range of properties to initialize the new vertices with.


> [!IMPORTANT] ID Stability
>
>
> Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated. 
>
>



        

<hr>



### function remove\_vertex [1/2]

_Removes a vertex by its ID, removing all associated incident edges._ 
```cpp
inline void gl::graph::remove_vertex (
    const  id_type vertex_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex to remove. 



**Exception:**


* `std::invalid_argument` If the ID is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a vertex invalidates:
> * All vertex descriptors and IDs for vertices with higher IDs (they shift down).
> * All edge descriptors and IDs for edges incident to this vertex.
> * All references to vertex and edge properties obtained from the property maps.
> * References to vertex properties obtained via `vertex_properties()`.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs or edge descriptors. 
>
>



        

<hr>



### function remove\_vertex [2/2]

_Removes a vertex using its descriptor._ 
```cpp
inline void gl::graph::remove_vertex (
    vertex_type vertex
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex to remove. 



**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a vertex invalidates:
> * All vertex descriptors and IDs for vertices with higher IDs (they shift down).
> * All edge descriptors and IDs for edges incident to this vertex.
> * All references to vertex and edge properties obtained from the property maps.
> * References to vertex properties obtained via `vertex_properties()`.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs or edge descriptors. 
>
>



        

<hr>



### function remove\_vertices [1/2]

_Removes a range of vertices using their IDs._ 
```cpp
inline void gl::graph::remove_vertices (
    const traits::c_forward_range_of< id_type > auto & vertex_id_rng
) 
```





**Parameters:**


* `vertex_id_rng` A forward range containing the IDs of vertices to remove. 



**Exception:**


* `std::invalid_argument` If any vertex ID in the range is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a vertex invalidates:
> * All vertex descriptors and IDs for vertices with higher IDs (they shift down).
> * All edge descriptors and IDs for edges incident to this vertex.
> * All references to vertex and edge properties obtained from the property maps.
> * References to vertex properties obtained via `vertex_properties()`.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs or edge descriptors. 
>
>



        

<hr>



### function remove\_vertices [2/2]

_Removes a range of vertices using their descriptors._ 
```cpp
inline void gl::graph::remove_vertices (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng
) 
```





**Parameters:**


* `vertex_rng` A sized range containing the descriptors of vertices to remove. 



**Exception:**


* `std::invalid_argument` If any vertex descriptor is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a vertex invalidates:
> * All vertex descriptors and IDs for vertices with higher IDs (they shift down).
> * All edge descriptors and IDs for edges incident to this vertex.
> * All references to vertex and edge properties obtained from the property maps.
> * References to vertex properties obtained via `vertex_properties()`.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs or edge descriptors. 
>
>



        

<hr>



### function has\_vertex [1/2]

_Checks if a vertex with the given ID exists in the graph._ 
```cpp
inline bool gl::graph::has_vertex (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID to check. 



**Returns:**

`true` if it exists, `false` otherwise. 





        

<hr>



### function has\_vertex [2/2]

_Checks if the given vertex descriptor is valid in the graph._ 
```cpp
inline bool gl::graph::has_vertex (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor to check. 



**Returns:**

`true` if it exists, `false` otherwise. 





        

<hr>



### function vertex 

_Returns a vertex descriptor bounds-checked by ID._ 
```cpp
inline vertex_type gl::graph::vertex (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding vertex descriptor. 




**Exception:**


* `std::invalid_argument` If the ID is invalid. 




        

<hr>



### function at 

_Returns a vertex descriptor bounds-checked by ID (alias for_ `vertex` _)._
```cpp
inline vertex_type gl::graph::at (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding vertex descriptor. 




**Exception:**


* `std::invalid_argument` If the ID is invalid. 




        

<hr>



### function vertex\_unchecked 

_Returns a vertex descriptor without bounds checking._ 
```cpp
inline vertex_type gl::graph::vertex_unchecked (
    const  id_type vertex_id
) noexcept const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding vertex descriptor.



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed. Passing an invalid ID results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a vertex descriptor without bounds checking (array access style)._ 
```cpp
inline vertex_type gl::graph::operator[] (
    const  id_type vertex_id
) noexcept const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding vertex descriptor.



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed. Passing an invalid ID results in Undefined Behavior. 
>
>



        

<hr>



### function vertices 

_Returns a lazily evaluated view of all vertex descriptors in the graph._ 
```cpp
inline auto gl::graph::vertices () noexcept const
```





**Returns:**

A view yielding descriptors for every vertex. 





        

<hr>



### function vertex\_ids 

_Returns a lazily evaluated view of all active vertex IDs in the graph._ 
```cpp
inline auto gl::graph::vertex_ids () noexcept const
```





**Returns:**

A view yielding all valid vertex IDs. 





        

<hr>



### function neighbors [1/2]

_Retrieves the neighbor vertex IDs for a specific vertex._ 
```cpp
inline auto gl::graph::neighbors (
    const  id_type vertex_id
) const
```



#### Formal Definition



The neighborhood \(N(v)\) of a vertex \(v\) is the set of all its adjacent vertices:



\[N(v) =
\begin{cases}
\{u \in V : \{u, v\} \in E\} & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E \lor (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the source vertex. 



**Returns:**

A view of all adjacent vertex descriptors. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function neighbors [2/2]

_Retrieves the neighbor vertex descriptors for a specific vertex._ 
```cpp
inline auto gl::graph::neighbors (
    vertex_type vertex
) const
```



#### Formal Definition



The neighborhood \(N(v)\) of a vertex \(v\) is the set of all its adjacent vertices:



\[N(v) =
\begin{cases}
\{u \in V : \{u, v\} \in E\} & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E \lor (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The source vertex descriptor. 



**Returns:**

A view of all adjacent vertex descriptors. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function neighbor\_ids [1/2]

_Retrieves the neighbor vertex IDs for a specific vertex._ 
```cpp
inline auto gl::graph::neighbor_ids (
    const  id_type vertex_id
) const
```



#### Formal Definition



The neighborhood \(N(v)\) of a vertex \(v\) is the set of all its adjacent vertices:



\[N(v) =
\begin{cases}
\{u \in V : \{u, v\} \in E\} & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E \lor (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the source vertex. 



**Returns:**

A view of all adjacent vertex IDs. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function neighbor\_ids [2/2]

_Retrieves the neighbor vertex IDs for a specific vertex._ 
```cpp
inline auto gl::graph::neighbor_ids (
    vertex_type vertex
) const
```



#### Formal Definition



The neighborhood \(N(v)\) of a vertex \(v\) is the set of all its adjacent vertices:



\[N(v) =
\begin{cases}
\{u \in V : \{u, v\} \in E\} & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E \lor (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The source vertex descriptor. 



**Returns:**

A view of all adjacent vertex IDs. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function predecessors [1/2]

_Retrieves the predecessor vertex IDs for a vertex._ 
```cpp
inline auto gl::graph::predecessors (
    const  id_type vertex_id
) const
```



#### Formal Definition



The set of predecessors (in-neighborhood) \(N_{in}(v)\) is defined as:



\[N_{in}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the target vertex. 



**Returns:**

A view of all predecessor vertex descriptors. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function predecessors [2/2]

_Retrieves the predecessor vertex descriptors (incoming edges) for a vertex._ 
```cpp
inline auto gl::graph::predecessors (
    vertex_type vertex
) const
```



#### Formal Definition



The set of predecessors (in-neighborhood) \(N_{in}(v)\) is defined as:



\[N_{in}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The target vertex descriptor. 



**Returns:**

A view of all predecessor vertex descriptors. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function predecessor\_ids [1/2]

_Retrieves the predecessor vertex IDs for a vertex._ 
```cpp
inline auto gl::graph::predecessor_ids (
    const  id_type vertex_id
) const
```



#### Formal Definition



The set of predecessors (in-neighborhood) \(N_{in}(v)\) is defined as:



\[N_{in}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the target vertex. 



**Returns:**

A view of all predecessor vertex IDs. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function predecessor\_ids [2/2]

_Retrieves the predecessor vertex IDs for a vertex._ 
```cpp
inline auto gl::graph::predecessor_ids (
    vertex_type vertex
) const
```



#### Formal Definition



The set of predecessors (in-neighborhood) \(N_{in}(v)\) is defined as:



\[N_{in}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (u, v) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The target vertex descriptor. 



**Returns:**

A view of all predecessor vertex IDs. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function successors [1/2]

_Retrieves the successor vertex IDs for a vertex._ 
```cpp
inline auto gl::graph::successors (
    const  id_type vertex_id
) const
```



#### Formal Definition



The set of successors (out-neighborhood) \(N_{out}(v)\) is defined as:



\[N_{out}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the source vertex. 



**Returns:**

A view of all successor vertex descriptors. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function successors [2/2]

_Retrieves the successor vertex descriptors (outgoing edges) for a vertex._ 
```cpp
inline auto gl::graph::successors (
    vertex_type vertex
) const
```



#### Formal Definition



The set of successors (out-neighborhood) \(N_{out}(v)\) is defined as:



\[N_{out}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The source vertex descriptor. 



**Returns:**

A view of all successor vertex descriptors. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function successor\_ids [1/2]

_Retrieves the successor vertex IDs for a vertex._ 
```cpp
inline auto gl::graph::successor_ids (
    const  id_type vertex_id
) const
```



#### Formal Definition



The set of successors (out-neighborhood) \(N_{out}(v)\) is defined as:



\[N_{out}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the source vertex. 



**Returns:**

A view of all successor vertex IDs. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function successor\_ids [2/2]

_Retrieves the successor vertex IDs for a vertex._ 
```cpp
inline auto gl::graph::successor_ids (
    vertex_type vertex
) const
```



#### Formal Definition



The set of successors (out-neighborhood) \(N_{out}(v)\) is defined as:



\[N_{out}(v) =
\begin{cases}
N(v) & \text{if } G \text{ is undirected}
\\\\ \{u \in V : (v, u) \in E\} & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The source vertex descriptor. 



**Returns:**

A view of all successor vertex IDs. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function vertex\_properties 

_Retrieves a mutable reference to a vertex's properties._ 
```cpp
inline vertex_properties_type & gl::graph::vertex_properties (
    const  id_type id
) const
```





**Parameters:**


* `id` The ID of the vertex. 



**Returns:**

A reference to the properties attached to the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function vertex\_properties\_map 

_Retrieves a random-access view over all vertex properties in the graph._ 
```cpp
inline auto gl::graph::vertex_properties_map () noexcept const
```





**Returns:**

A view mapping each active vertex index to its property. 





        

<hr>



### function degree [1/2]

_Calculates the total degree of a vertex._ 
```cpp
inline size_type gl::graph::degree (
    const  id_type vertex_id
) const
```



The degree is the total number of edge endpoints connected to the vertex. For both directed and undirected graphs, a self-loop contributes **2** to the total degree.


#### Formal Definition



The formal calculation, accounting for the set of loops \(L(v)\), is defined as:



\[deg(v) =
\begin{cases}
deg_{in}(v) + deg_{out}(v) & \text{if } G \text{ is directed}
\\ 2 \cdot |L(v)| + |E(v) \setminus L(v)| & \text{if } G \text{ is undirected}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The total degree. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function degree [2/2]

_Calculates the total degree of a vertex._ 
```cpp
inline size_type gl::graph::degree (
    vertex_type vertex
) const
```



The degree is the total number of edge endpoints connected to the vertex. For both directed and undirected graphs, a self-loop contributes **2** to the total degree.


#### Formal Definition



The formal calculation, accounting for the set of loops \(L(v)\), is defined as:



\[deg(v) =
\begin{cases}
deg_{in}(v) + deg_{out}(v) & \text{if } G \text{ is directed}
\\ 2 \cdot |L(v)| + |E(v) \setminus L(v)| & \text{if } G \text{ is undirected}
\end{cases}\]
   

**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

The total degree. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function degree\_map 

_Returns a mapped array of total degrees for all vertices._ 
```cpp
inline std::vector< size_type > gl::graph::degree_map () const
```





**Returns:**

A vector where the index aligns with the vertex ID containing its degree. 





        

<hr>



### function in\_degree [1/2]

_Calculates the in-degree (incoming edges) for a vertex._ 
```cpp
inline size_type gl::graph::in_degree (
    const  id_type vertex_id
) const
```



The in-degree is the number of edges directed into the vertex.


#### Formal Definition




\[deg_{in}(v) =
\begin{cases}
deg(v) & \text{if } G \text{ is undirected}
\\\\ |E_{in}(v)| = |\{u \in V : (u, v) \in E\}| & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The in-degree. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function in\_degree [2/2]

_Calculates the in-degree (incoming edges) for a vertex._ 
```cpp
inline size_type gl::graph::in_degree (
    vertex_type vertex
) const
```



The in-degree is the number of edges directed into the vertex.


#### Formal Definition




\[deg_{in}(v) =
\begin{cases}
deg(v) & \text{if } G \text{ is undirected}
\\\\ |E_{in}(v)| = |\{u \in V : (u, v) \in E\}| & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

The in-degree. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function in\_degree\_map 

_Returns a mapped array of in-degrees for all vertices._ 
```cpp
inline std::vector< size_type > gl::graph::in_degree_map () const
```





**Returns:**

A vector where the index aligns with the vertex ID containing its in-degree. 





        

<hr>



### function out\_degree [1/2]

_Calculates the out-degree (outgoing edges) for a vertex._ 
```cpp
inline size_type gl::graph::out_degree (
    const  id_type vertex_id
) const
```



The out-degree is the number of edges directed out of the vertex.


#### Formal Definition




\[deg_{out}(v) =
\begin{cases}
deg(v) & \text{if } G \text{ is undirected}
\\\\ |E_{out}(v)| = |\{u \in V : (v, u) \in E\}| & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The out-degree. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function out\_degree [2/2]

_Calculates the out-degree (outgoing edges) for a vertex._ 
```cpp
inline size_type gl::graph::out_degree (
    vertex_type vertex
) const
```



The out-degree is the number of edges directed out of the vertex.


#### Formal Definition




\[deg_{out}(v) =
\begin{cases}
deg(v) & \text{if } G \text{ is undirected}
\\\\ |E_{out}(v)| = |\{u \in V : (v, u) \in E\}| & \text{if } G \text{ is directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

The out-degree. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function out\_degree\_map 

_Returns a mapped array of out-degrees for all vertices._ 
```cpp
inline std::vector< size_type > gl::graph::out_degree_map () const
```





**Returns:**

A vector where the index aligns with the vertex ID containing its out-degree. 





        

<hr>



### function add\_edge [1/2]

_Creates an edge between a source and target vertex._ 
```cpp
inline edge_type gl::graph::add_edge (
    const  id_type source_id,
    const  id_type target_id
) 
```





**Parameters:**


* `source_id` The ID of the source vertex. 
* `target_id` The ID of the target vertex. 



**Returns:**

A descriptor representing the newly created edge. 




**Exception:**


* `std::invalid_argument` If either vertex ID is invalid.


> [!IMPORTANT] ID Stability
>
>
> Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_edge\_with [1/2]

_Creates an edge carrying specific properties between a source and target vertex._ 
```cpp
inline edge_type gl::graph::add_edge_with (
    const  id_type source_id,
    const  id_type target_id,
    edge_properties_type properties
) 
```





**Parameters:**


* `source_id` The ID of the source vertex. 
* `target_id` The ID of the target vertex. 
* `properties` The property payload to attach to the edge. 



**Returns:**

A descriptor representing the newly created edge. 




**Exception:**


* `std::invalid_argument` If either vertex ID is invalid.


> [!IMPORTANT] ID Stability
>
>
> Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_edge [2/2]

_Creates an edge between a source and target vertex._ 
```cpp
inline edge_type gl::graph::add_edge (
    vertex_type source,
    vertex_type target
) 
```





**Parameters:**


* `source` The source vertex descriptor. 
* `target` The target vertex descriptor. 



**Returns:**

A descriptor representing the newly created edge. 




**Exception:**


* `std::invalid_argument` If either vertex descriptor is invalid.


> [!IMPORTANT] ID Stability
>
>
> Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_edge\_with [2/2]

_Creates an edge carrying specific properties between a source and target vertex._ 
```cpp
inline edge_type gl::graph::add_edge_with (
    vertex_type source,
    vertex_type target,
    const  edge_properties_type & properties
) 
```





**Parameters:**


* `source` The source vertex descriptor. 
* `target` The target vertex descriptor. 
* `properties` The property payload to attach to the edge. 



**Returns:**

A descriptor representing the newly created edge. 




**Exception:**


* `std::invalid_argument` If either vertex descriptor is invalid.


> [!IMPORTANT] ID Stability
>
>
> Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_edges\_from [1/2]

_Dispatches multiple edge insertions connecting one source to many targets._ 
```cpp
inline void gl::graph::add_edges_from (
    const  id_type source_id,
    const traits::c_sized_range_of< id_type > auto & target_id_rng
) 
```





**Parameters:**


* `source_id` The ID of the source vertex. 
* `target_id_rng` A sized range of target vertex IDs. 



**Exception:**


* `std::invalid_argument` If any vertex ID is invalid.


> [!IMPORTANT] ID Stability
>
>
> Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_edges\_from [2/2]

_Dispatches multiple edge insertions connecting one source to many targets._ 
```cpp
inline void gl::graph::add_edges_from (
    vertex_type source,
    const traits::c_sized_range_of< vertex_type > auto & target_rng
) 
```





**Parameters:**


* `source` The source vertex descriptor. 
* `target_rng` A sized range of target vertex descriptors. 



**Exception:**


* `std::invalid_argument` If any vertex ID is invalid.


> [!IMPORTANT] ID Stability
>
>
> Adding edges does **not** invalidate vertex or edge IDs. **However**, property references stored in existing edge descriptors may be invalidated. 
>
>



        

<hr>



### function remove\_edge 

_Removes a specific edge from the graph._ 
```cpp
inline void gl::graph::remove_edge (
    const  edge_type & edge
) 
```





**Parameters:**


* `edge` The descriptor of the edge to remove. 



**Exception:**


* `std::invalid_argument` If the edge descriptor is invalid;


> [!WARNING] Edge Descriptor Invalidation
>
>
> Removing an edge invalidates:
> * All edge descriptors and IDs for edges with higher IDs (they shift down).
> * References to edge properties obtained via `edge_properties()`.
> * References to edge properties obtained from `edge_properties_map()`.
>
>
>
>
> Vertex descriptors and IDs remain valid. 
>
>



        

<hr>



### function remove\_edges 

_Removes a range of edges from the graph._ 
```cpp
inline void gl::graph::remove_edges (
    const traits::c_range_of< edge_type > auto & edges
) 
```





**Parameters:**


* `edges` A range containing descriptors of the edges to remove.


> [!NOTE] Operation Safety
>
>
> If the edges list is empty or contains no valid (in the context of the graph instance), the operation has no effect on the graph's structure. 
>
>




> [!WARNING] Edge Descriptor Invalidation
>
>
> Removing an edge invalidates:
> * All edge descriptors and IDs for edges with higher IDs (they shift down).
> * References to edge properties obtained via `edge_properties()`.
> * References to edge properties obtained from `edge_properties_map()`.
>
>
>
>
> Vertex descriptors and IDs remain valid. 
>
>



        

<hr>



### function edge\_ids 

_Returns a lazily evaluated view of all active edge IDs in the graph._ 
```cpp
inline auto gl::graph::edge_ids () noexcept const
```





**Returns:**

A view yielding all valid edge IDs. 





        

<hr>



### function has\_edge [1/3]

_Verifies if the exact specified edge exists._ 
```cpp
inline bool gl::graph::has_edge (
    const  edge_type & edge
) const
```





**Parameters:**


* `edge` The edge descriptor to verify. 



**Returns:**

`true` if the edge exists within the graph, `false` otherwise. 





        

<hr>



### function has\_edge [2/3]

_Checks if there is any connecting edge from source to target._ 
```cpp
inline bool gl::graph::has_edge (
    const  id_type source_id,
    const  id_type target_id
) const
```





**Parameters:**


* `source_id` The source vertex ID. 
* `target_id` The target vertex ID. 



**Returns:**

`true` if an edge exists, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either vertex ID is invalid. 




        

<hr>



### function has\_edge [3/3]

_Checks if there is any connecting edge from source to target._ 
```cpp
inline bool gl::graph::has_edge (
    vertex_type source,
    vertex_type target
) const
```





**Parameters:**


* `source` The source vertex descriptor. 
* `target` The target vertex descriptor. 



**Returns:**

`true` if an edge exists, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either vertex descriptor is invalid. 




        

<hr>



### function edge [1/2]

_Retrieves an edge (if it exists) connecting the source to the target._ 
```cpp
inline std::optional< edge_type > gl::graph::edge (
    const  id_type source_id,
    const  id_type target_id
) const
```





**Parameters:**


* `source_id` The source vertex ID. 
* `target_id` The target vertex ID. 



**Returns:**

`true` if an edge exists, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either vertex ID is invalid. 




        

<hr>



### function edge [2/2]

_Retrieves an edge (if it exists) connecting the source to the target._ 
```cpp
inline std::optional< edge_type > gl::graph::edge (
    vertex_type source,
    vertex_type target
) const
```





**Parameters:**


* `source` The source vertex descriptor. 
* `target` The target vertex descriptor. 



**Returns:**

`true` if an edge exists, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either vertex descriptor is invalid. 




        

<hr>



### function edges [1/2]

_Retrieves all parallel edges connecting the source to the target._ 
```cpp
inline std::vector< edge_type > gl::graph::edges (
    const  id_type source_id,
    const  id_type target_id
) const
```





**Parameters:**


* `source_id` The source vertex ID. 
* `target_id` The target vertex ID. 



**Returns:**

A vector populated with the descriptors of all edges linking the two vertices. 




**Exception:**


* `std::invalid_argument` If either vertex ID is invalid. 




        

<hr>



### function edges [2/2]

_Retrieves all parallel edges connecting the source to the target._ 
```cpp
inline std::vector< edge_type > gl::graph::edges (
    vertex_type source,
    vertex_type target
) const
```





**Parameters:**


* `source` The source vertex descriptor. 
* `target` The target vertex descriptor. 



**Returns:**

A vector populated with the descriptors of all edges linking the two vertices. 




**Exception:**


* `std::invalid_argument` If either vertex descriptor is invalid. 




        

<hr>



### function incident\_edges [1/2]

_Retrieves all edges incident with a vertex._ 
```cpp
inline auto gl::graph::incident_edges (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of incident edges. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function incident\_edges [2/2]

_Retrieves all incident with a vertex._ 
```cpp
inline auto gl::graph::incident_edges (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of incident edges. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function in\_edges [1/2]

_Retrieves all incoming edges of a vertex (going into the vertex)._ 
```cpp
inline auto gl::graph::in_edges (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of incoming edges. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function in\_edges [2/2]

_Retrieves all incoming edges of a vertex (going into the vertex)._ 
```cpp
inline auto gl::graph::in_edges (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of incoming edges. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function out\_edges [1/2]

_Retrieves all outgoing edges of a vertex (going out of the vertex)._ 
```cpp
inline auto gl::graph::out_edges (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of outgoing edges. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function out\_edges [2/2]

_Retrieves all outgoing edges of a vertex (going out of the vertex)._ 
```cpp
inline auto gl::graph::out_edges (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of outgoing edges. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function edge\_properties 

_Retrieves a mutable reference to an edge's properties._ 
```cpp
inline edge_properties_type & gl::graph::edge_properties (
    const  id_type id
) const
```





**Parameters:**


* `id` The ID of the edge. 



**Returns:**

A reference to the properties attached to the edge. 




**Exception:**


* `std::invalid_argument` If the edge ID is invalid. 




        

<hr>



### function edge\_properties\_map 

_Retrieves a view over all edge properties in the graph._ 
```cpp
inline auto gl::graph::edge_properties_map () noexcept const
```





**Returns:**

A view mapping each active edge index to its property. 





        

<hr>



### function are\_adjacent [1/3]

```cpp
inline bool gl::graph::are_adjacent (
    const  id_type source_id,
    const  id_type target_id
) const
```



in E$
* For directed graphs: [**$(u, v)**](group__GL-Core.md#variable-invalid_id_v) \in E \lor (v, u) \in E$






**Parameters:**


* `source_id` The ID of the source vertex. 
* `target_id` The ID of the target vertex. 



**Returns:**

`true` if the given vertices are adjacent, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either vertex ID is invalid. 




        

<hr>



### function are\_adjacent [2/3]

```cpp
inline bool gl::graph::are_adjacent (
    vertex_type source,
    vertex_type target
) const
```



in E$
* For directed graphs: [**$(u, v)**](group__GL-Core.md#variable-invalid_id_v) \in E \lor (v, u) \in E$






**Parameters:**


* `source` The source vertex descriptor. 
* `target` The target vertex descriptor. 



**Returns:**

`true` if the given vertices are adjacent, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either vertex descriptor is invalid. 




        

<hr>



### function are\_adjacent [3/3]

_Checks if two distinct edges share at least one incident vertex._ 
```cpp
inline bool gl::graph::are_adjacent (
    const  edge_type & edge_1,
    const  edge_type & edge_2
) const
```



#### Formal definition



Edges $e$ and $f$ are adjacent if they share at least one endpoint: \(e \cap f \ne \emptyset\).




**Parameters:**


* `edge_1` The first edge descriptor. 
* `edge_2` The second edge descriptor. 



**Returns:**

`true` if the given edges are adjacent, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either edge descriptor is invalid. 





        

<hr>



### function are\_incident [1/2]

_Checks if a vertex forms one of the endpoints of an edge._ 
```cpp
inline bool gl::graph::are_incident (
    vertex_type vertex,
    const  edge_type & edge
) const
```



#### Formal Definition



A vertex \(v\) is incident to an edge \(e\) if \(v\) is an element of the endpoint set of \(e\) ( \(v \in e\)). Assuming \(e\) connects vertices \(u\) and \(w\): \(v = u \lor v = w\)




**Parameters:**


* `vertex` The vertex descriptor. 
* `edge` The edge descriptor. 



**Returns:**

`true` if the vertex is incident to the edge, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either the vertex or the edge descriptor is invalid. 





        

<hr>



### function are\_incident [2/2]

_Checks if a vertex forms one of the endpoints of an edge._ 
```cpp
inline bool gl::graph::are_incident (
    const  edge_type & edge,
    vertex_type vertex
) const
```



A convenience overload of the `are_incident` method. It is equivalent to `are_incident(vertex, edge)`




**Parameters:**


* `edge` The edge descriptor. 
* `vertex` The vertex descriptor. 



**Returns:**

`true` if the vertex is incident to the edge, `false` otherwise. 




**Exception:**


* `std::invalid_argument` If either the vertex or the edge descriptor is invalid. 




        

<hr>## Friends Documentation





### friend operator== 

_Compares two graphs for strict structural and property equality._ 
```cpp
inline bool gl::graph::operator== (
    const  graph & lhs,
    const  graph & rhs
) noexcept
```





**Parameters:**


* `lhs` The left operand. 
* `rhs` The right operand. 



**Returns:**

`true` if both graphs represent the exact same topology and properties, `false` otherwise. 





        

<hr>



### friend operator&lt;&lt; 

_Formats and outputs the entire graph structure to a standard output stream._ 
```cpp
inline std::ostream & gl::graph::operator<< (
    std::ostream & os,
    const  graph & g
) 
```



The generated string representation of the graph depends on the currently active formatting options of the stream.




**Parameters:**


* `os` The target output stream. 
* `g` The graph instance to write. 



**Returns:**

The stream reference for chaining. 





        

<hr>



### friend operator&gt;&gt; 

_Deserializes graph structure data from an input stream (using the GSF format)._ 
```cpp
inline std::istream & gl::graph::operator>> (
    std::istream & is,
    graph & g
) 
```





**Parameters:**


* `is` The source input stream. 
* `g` The graph instance to populate. 



**Returns:**

The stream reference for chaining. 





        

<hr>



### friend clone 

_Friend declaration providing access for deep cloning._ 
```cpp
template<traits::c_graph Graph>
Graph gl::graph::clone (
    const  Graph & source
) 
```





**Template parameters:**


* `Graph` The type of the graph. 



**Parameters:**


* `source` The graph instance to clone. 



**Returns:**

A newly constructed graph containing identical vertices, edges and properties (if applicable). 





        

<hr>



### friend to 

_Friend declaration providing access for graph target conversions._ 
```cpp
template<traits::c_graph_repr_tag TargetImplTag, traits::c_graph Graph>
auto gl::graph::to (
    Graph && source
) 
```



This function efficiently transforms a graph's underlying memory representation (e.g., from a standard adjacency list to a flattened adjacency list) while preserving its exact topology, properties, and identifiers.


#### Template Parameters




|Parameter|Description|Constraints|
|-----|-----|-----|
|TargetImplTag|The representation tag of the desired target representation (e.g., `gl::repr::flat_list_t`)|[**c\_graph\_repr\_tag**](gl_concepts.md#gl-traits-c-graph-repr-tag)|
|Graph|The type of the source graph, which will be automatically deduced from the function argument.|[**c\_graph**](gl_concepts.md#gl-traits-c-graph)|








**Parameters:**


* `source` The graph to convert. After the operation it will be left in a valid, empty state. 



**Returns:**

A new graph containing the moved data, structured according to `TargetImplTag`.






#### See Also




* [**swap\_repr\_tag**](structgl_1_1traits_1_1swap__repr__tag.md) : For the trait used to resolve the target graph type with the swapped representation tag. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/graph.hpp`

