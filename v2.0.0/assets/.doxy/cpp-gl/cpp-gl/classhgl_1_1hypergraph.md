

# Class hgl::hypergraph

**template &lt;traits::c\_instantiation\_of&lt; [**hypergraph\_traits**](structhgl_1_1hypergraph__traits.md) &gt; HypergraphTraits&gt;**



[**ClassList**](annotated.md) **>** [**hgl**](namespacehgl.md) **>** [**hypergraph**](classhgl_1_1hypergraph.md)



_The generic hypergraph container using a policy-based design._ [More...](#detailed-description)

* `#include <hgl/hypergraph.hpp>`















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**hyperedge\_formatter**](structhgl_1_1hypergraph_1_1hyperedge__formatter.md) <br>_Helper structure used to properly format an individual hyperedge within the hypergraph's context into an output stream._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**HypergraphTraits**](group__HGL-Core.md#variable-invalid_id_v) | [**traits\_type**](#typedef-traits_type)  <br>_The traits type specifying the hypergraph's behavior and representation._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::directional\_tag | [**directional\_tag**](#typedef-directional_tag)  <br>_Type tag specifying the directionality of the hypergraph._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::representation\_tag | [**representation\_tag**](#typedef-representation_tag)  <br>_Type tag indicating the underlying representation model._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) representation\_tag::template [**representation\_type**](classhgl_1_1hypergraph.md#typedef-representation_type)&lt; [**directional\_tag**](classhgl_1_1hypergraph.md#typedef-directional_tag) &gt; | [**representation\_type**](#typedef-representation_type)  <br>_The underlying representation type matching the directional tag._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::id\_type | [**id\_type**](#typedef-id_type)  <br>_Integral type used to identify vertices and hyperedges._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::vertex\_type | [**vertex\_type**](#typedef-vertex_type)  <br>_The descriptor type representing a vertex._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::vertex\_properties\_type | [**vertex\_properties\_type**](#typedef-vertex_properties_type)  <br>_The user-defined property payload type associated with vertices._  |
| typedef std::conditional\_t&lt; traits::c\_empty\_properties&lt; [**vertex\_properties\_type**](classhgl_1_1hypergraph.md#typedef-vertex_properties_type) &gt;, [**empty\_properties\_map**](group__HGL-Core.md#typedef-empty_properties_map), std::vector&lt; [**vertex\_properties\_type**](classhgl_1_1hypergraph.md#typedef-vertex_properties_type) &gt; &gt; | [**vertex\_properties\_map\_type**](#typedef-vertex_properties_map_type)  <br>_The container type used for storing the vertex properties mapping._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::hyperedge\_type | [**hyperedge\_type**](#typedef-hyperedge_type)  <br>_The descriptor type representing a hyperedge._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) traits\_type::hyperedge\_properties\_type | [**hyperedge\_properties\_type**](#typedef-hyperedge_properties_type)  <br>_The user-defined property payload type associated with hyperedges._  |
| typedef std::conditional\_t&lt; traits::c\_empty\_properties&lt; [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) &gt;, [**empty\_properties\_map**](group__HGL-Core.md#typedef-empty_properties_map), std::vector&lt; [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) &gt; &gt; | [**hyperedge\_properties\_map\_type**](#typedef-hyperedge_properties_map_type)  <br>_The container type used for storing hyperedge properties mapping._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**hypergraph**](#function-hypergraph-13) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**size\_type**](group__HGL-Types.md#typedef-size_type) n\_vertices=0uz, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**size\_type**](group__HGL-Types.md#typedef-size_type) n\_hyperedges=0uz) <br>_Constructs a hypergraph with the given number of vertices and hyperedges (empty by default)._  |
|   | [**hypergraph**](#function-hypergraph-23) ([**hypergraph**](classhgl_1_1hypergraph.md) &&) noexcept<br>_Default move constructor._  |
|  [**hypergraph**](classhgl_1_1hypergraph.md) & | [**operator=**](#function-operator) ([**hypergraph**](classhgl_1_1hypergraph.md) &&) noexcept<br>_Default move assignment operator._  |
|   | [**~hypergraph**](#function-hypergraph) () = default<br>_Default destructor._  |
|  [**hypergraph**](classhgl_1_1hypergraph.md) & | [**operator=**](#function-operator_1) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**hypergraph**](classhgl_1_1hypergraph.md) &) = delete<br>_Hypergraph copy assignment is disabled to avoid accidental copies. Use_ [_**clone**_](group__HGL-Core.md#function-clone) _instead._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**n\_vertices**](#function-n_vertices) () noexcept const<br>_Returns the total number of vertices in the hypergraph._  |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**n\_hyperedges**](#function-n_hyperedges) () noexcept const<br>_Returns the total number of hyperedges in the hypergraph._  |
|  [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) | [**add\_vertex**](#function-add_vertex) () <br>_Adds a new, default-initialized vertex to the hypergraph._  |
|  [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) | [**add\_vertex\_with**](#function-add_vertex_with) ([**vertex\_properties\_type**](classhgl_1_1hypergraph.md#typedef-vertex_properties_type) properties) <br>_Adds a new vertex with the given properties to the hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**add\_vertices**](#function-add_vertices) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**size\_type**](group__HGL-Types.md#typedef-size_type) n) <br>_Adds multiple default-initialized vertices to the hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**add\_vertices\_with**](#function-add_vertices_with) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_sized\_range\_of&lt; [**vertex\_properties\_type**](classhgl_1_1hypergraph.md#typedef-vertex_properties_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & properties\_rng) <br>_Adds multiple vertices based on a range of property payloads._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_vertex**](#function-remove_vertex-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) <br>_Removes a vertex by its ID, unbinding it from its incident hyperedges._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_vertex**](#function-remove_vertex-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) <br>_Removes a vertex using its descriptor, unbinding it from its incident hyperedges._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_vertices**](#function-remove_vertices-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_id\_rng) <br>_Removes a range of vertices using their IDs._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_vertices**](#function-remove_vertices-22) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_rng) <br>_Removes a range of vertices using their descriptors._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**has\_vertex**](#function-has_vertex-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Checks if a vertex with the given ID exists in the hypergraph._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**has\_vertex**](#function-has_vertex-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Checks if the vertex referenced by the provided descriptor exists in the hypergraph._  |
|  [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) | [**vertex**](#function-vertex) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Returns a descriptor of the vertex with the given_ _bounds-checked_ _ID._ |
|  [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) | [**at**](#function-at-12) ([**vertex\_t**](structhgl_1_1vertex__t.md), [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Returns a descriptor of the vertex with the given bounds-checked ID._  |
|  [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) | [**vertex\_unchecked**](#function-vertex_unchecked) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Returns a descriptor of the vertex with the given ID without bounds checking._  |
|  [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) | [**operator[]**](#function-operator_2) ([**vertex\_t**](structhgl_1_1vertex__t.md), [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Returns a descriptor of the vertex with the given ID without bounds checking._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**vertices**](#function-vertices) () noexcept const<br>_Returns a lazily evaluated, random-access view of all vertex descriptors in the hypergraph._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**vertex\_ids**](#function-vertex_ids) () noexcept const<br>_Returns a lazily evaluated, random-access view of all active vertex IDs in the hypergraph._  |
|  [**vertex\_properties\_type**](classhgl_1_1hypergraph.md#typedef-vertex_properties_type) & | [**vertex\_properties**](#function-vertex_properties) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Retrieves a reference to the properties of a specified vertex._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**vertex\_properties\_map**](#function-vertex_properties_map) () noexcept const<br>_Retrieves a lazily evaluated, random-access view over all vertex properties in the hypergraph._  |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-19) () <br>_Adds a new, default-initialized hyperedge to the hypergraph._  |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-19) ([**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new hyperedge with the given properties to the hypergraph._  |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-29) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_id\_rng) <br>_Adds a new_ _undirected_ _hyperedge and immediately binds a range of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-39) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; vertex\_ids) <br>_Adds a new_ _undirected_ _hyperedge and immediately binds a list of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-49) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_rng) <br>_Adds a new_ _undirected_ _hyperedge and immediately binds a range of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-59) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; vertices) <br>_Adds a new_ _undirected_ _hyperedge and immediately binds a list of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-29) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_id\_rng, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a range of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-39) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; vertex\_ids, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a list of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-49) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_rng, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a range of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-59) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; vertices, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a list of vertices to it._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-69) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & tail\_id\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & head\_id\_rng) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-79) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; tail\_ids, std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; head\_ids) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-89) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & tail\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & head\_rng) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge**](#function-add_hyperedge-99) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; tail, std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; head) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-69) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & tail\_id\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & head\_id\_rng, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-79) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; tail\_ids, std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; head\_ids, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-89) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & tail\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & head\_rng, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**add\_hyperedge\_with**](#function-add_hyperedge_with-99) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; tail, std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; head, [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) properties) <br>_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**add\_hyperedges**](#function-add_hyperedges) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**size\_type**](group__HGL-Types.md#typedef-size_type) n) <br>_Adds multiple default-initialized hyperedges to the hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**add\_hyperedges\_with**](#function-add_hyperedges_with) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_sized\_range\_of&lt; [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & properties\_rng) <br>_Adds multiple hyperedges based on a range of property payloads._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_hyperedge**](#function-remove_hyperedge-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Removes a hyperedge by its ID, unbinding it from its incident vertices._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_hyperedge**](#function-remove_hyperedge-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Removes a hyperedge using its descriptor, unbinding it from its incident vertices._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_hyperedges\_from**](#function-remove_hyperedges_from-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_id\_rng) <br>_Removes a range of hyperedges using their IDs._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**remove\_hyperedges\_from**](#function-remove_hyperedges_from-22) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_rng) <br>_Removes a range of hyperedges using their descriptors._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**has\_hyperedge**](#function-has_hyperedge-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Checks if a hyperedge with the given ID exists in the hypergraph._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**has\_hyperedge**](#function-has_hyperedge-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Checks if the hyperedge referenced by the provided descriptor exists in the hypergraph._  |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**hyperedge**](#function-hyperedge) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Returns a descriptor of the hyperedge with the given_ _bounds-checked_ _ID._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**at**](#function-at-22) ([**hyperedge\_t**](structhgl_1_1hyperedge__t.md), [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Returns a descriptor of the hyperedge with the given_ _bounds-checked_ _ID._ |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**hyperedge\_unchecked**](#function-hyperedge_unchecked) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Returns a descriptor of the hyperedge with the given ID without bounds checking._  |
|  [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**operator[]**](#function-operator_3) ([**hyperedge\_t**](structhgl_1_1hyperedge__t.md), [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Returns a descriptor of the hyperedge with the given ID without bounds checking._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**hyperedges**](#function-hyperedges) () noexcept const<br>_Returns a lazily evaluated, random-access view of all hyperedge descriptors in the hypergraph._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**hyperedge\_ids**](#function-hyperedge_ids) () noexcept const<br>_Returns a lazily evaluated, random-access view of all active hyperedge IDs in the hypergraph._  |
|  [**hyperedge\_properties\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_properties_type) & | [**hyperedge\_properties**](#function-hyperedge_properties) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves a reference to the properties of a specified hyperedge._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**hyperedge\_properties\_map**](#function-hyperedge_properties_map) () noexcept const<br>_Retrieves a lazily evaluated, random-access view over all hyperedge properties in the hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-110) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a vertex to a hyperedge in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-210) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a vertex to a hyperedge in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-310) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_id\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a range of vertices to a single hyperedge in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-410) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; vertex\_ids, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a list of vertices to a single hyperedge in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-510) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_rng, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a range of vertices to a single hyperedge in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-610) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; vertices, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a list of vertices to a single hyperedge in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-710) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_id\_rng) <br>_Binds a single vertex to a range of hyperedges in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-810) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; hyperedge\_ids) <br>_Binds a single vertex to a list of hyperedges in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-910) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_rng) <br>_Binds a single vertex to a range of hyperedges in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind**](#function-bind-1010) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, std::initializer\_list&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; hyperedges) <br>_Binds a single vertex to a list of hyperedges in an undirected hypergraph._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-110) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a vertex to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-210) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a vertex to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-310) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_id\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a range of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-410) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; vertex\_ids, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a list of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-510) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_rng, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a range of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-610) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; vertices, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a range of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-710) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_id\_rng) <br>_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-810) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; hyperedge\_ids) <br>_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-910) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_rng) <br>_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_tail**](#function-bind_tail-1010) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, std::initializer\_list&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; hyperedges) <br>_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-110) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a vertex to the_ _head_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-210) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a vertex to the_ _head_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-310) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_id\_rng, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a range of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-410) (std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; vertex\_ids, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Binds a list of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-510) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & vertex\_rng, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a range of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-610) (std::initializer\_list&lt; [**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) &gt; vertices, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Binds a range of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-710) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_id\_rng) <br>_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-810) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, std::initializer\_list&lt; [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) &gt; hyperedge\_ids) <br>_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-910) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_forward\_range\_of&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; [**auto**](group__HGL-Core.md#variable-invalid_id_v) & hyperedge\_rng) <br>_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**bind\_head**](#function-bind_head-1010) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, std::initializer\_list&lt; [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) &gt; hyperedges) <br>_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._ |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**unbind**](#function-unbind-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) <br>_Unbinds a vertex from a hyperedge._  |
|  [**void**](group__HGL-Core.md#variable-invalid_id_v) | [**unbind**](#function-unbind-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) <br>_Unbinds a vertex from a hyperedge._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**are\_incident**](#function-are_incident-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Evaluates whether the given vertex and hyperedge are incident._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**are\_incident**](#function-are_incident-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Evaluates whether a vertex and a hyperedge are currently incident using descriptors._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**is\_tail**](#function-is_tail-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Evaluates whether a vertex belongs to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**is\_tail**](#function-is_tail-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Evaluates whether a vertex belongs to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**is\_head**](#function-is_head-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id, [**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Evaluates whether a vertex belongs to the_ _head_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**is\_head**](#function-is_head-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex, [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Evaluates whether a vertex belongs to the_ _head_ _of a hyperedge in a BF-directed hypergraph._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_hyperedges**](#function-incident_hyperedges-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) <br>_Retrieves all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_hyperedges**](#function-incident_hyperedges-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) <br>_Retrieves all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_hyperedge\_ids**](#function-incident_hyperedge_ids-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Retrieves IDs of all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_hyperedge\_ids**](#function-incident_hyperedge_ids-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Retrieves IDs of all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**degree**](#function-degree-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Calculates the degree of a vertex in the hypergraph._  |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**degree**](#function-degree-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Calculates the degree of a vertex in the hypergraph._  |
|  std::vector&lt; [**size\_type**](group__HGL-Types.md#typedef-size_type) &gt; | [**degree\_map**](#function-degree_map) () const<br>_Returns a mapped array of degrees for all vertices._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**out\_hyperedges**](#function-out_hyperedges-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Retrieves all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**out\_hyperedges**](#function-out_hyperedges-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Retrieves all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**out\_hyperedge\_ids**](#function-out_hyperedge_ids-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Retrieves IDs of all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**out\_hyperedge\_ids**](#function-out_hyperedge_ids-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Retrieves IDs of all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**out\_degree**](#function-out_degree-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Calculates the out-degree of a vertex in the_ _BF-directed_ _hypergraph._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**out\_degree**](#function-out_degree-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Calculates the out-degree of a vertex in the_ _BF-directed_ _hypergraph._ |
|  std::vector&lt; [**size\_type**](group__HGL-Types.md#typedef-size_type) &gt; | [**out\_degree\_map**](#function-out_degree_map) () const<br>_Returns a mapped array of out-degrees for all vertices._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**in\_hyperedges**](#function-in_hyperedges-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Retrieves all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**in\_hyperedges**](#function-in_hyperedges-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Retrieves all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**in\_hyperedge\_ids**](#function-in_hyperedge_ids-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Retrieves IDs of all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**in\_hyperedge\_ids**](#function-in_hyperedge_ids-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Retrieves IDs of all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**in\_degree**](#function-in_degree-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) vertex\_id) const<br>_Calculates the in-degree of a vertex in the_ _BF-directed_ _hypergraph._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**in\_degree**](#function-in_degree-22) ([**vertex\_type**](classhgl_1_1hypergraph.md#typedef-vertex_type) vertex) const<br>_Calculates the in-degree of a vertex in the_ _BF-directed_ _hypergraph._ |
|  std::vector&lt; [**size\_type**](group__HGL-Types.md#typedef-size_type) &gt; | [**in\_degree\_map**](#function-in_degree_map) () const<br>_Returns a mapped array of in-degrees for all vertices._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_vertices**](#function-incident_vertices-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves all vertices incident with a hyperedge ($e$)._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_vertices**](#function-incident_vertices-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves all vertices incident with a hyperedge ($e$)._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_vertex\_ids**](#function-incident_vertex_ids-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves IDs of all vertices incident with a hyperedge ($e$)._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**incident\_vertex\_ids**](#function-incident_vertex_ids-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves IDs of all vertices incident with a hyperedge ($e$)._  |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**hyperedge\_size**](#function-hyperedge_size-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves the size (number of incident vertices) of the given hyperedge._  |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**hyperedge\_size**](#function-hyperedge_size-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves the size (number of incident vertices) of the given hyperedge._  |
|  std::vector&lt; [**size\_type**](group__HGL-Types.md#typedef-size_type) &gt; | [**hyperedge\_size\_map**](#function-hyperedge_size_map) () const<br>_Returns a mapped array of hyperedge sizes._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**tail**](#function-tail-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**tail**](#function-tail-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**tail\_ids**](#function-tail_ids-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves IDs all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**tail\_ids**](#function-tail_ids-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves IDs all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**tail\_size**](#function-tail_size-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves the size of the_ _BF-directed_ _hyperedge's__tail_ _set ($\vert T(e) \vert)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**tail\_size**](#function-tail_size-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves the size of the_ _BF-directed_ _hyperedge's__tail_ _set ($\vert T(e) \vert)._ |
|  std::vector&lt; [**size\_type**](group__HGL-Types.md#typedef-size_type) &gt; | [**tail\_size\_map**](#function-tail_size_map) () const<br>_Returns a mapped array of sizes of the_ _tail_ _sets of hyperedged in a__BF-directed_ _hypergraph._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**head**](#function-head-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**head**](#function-head-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**head\_ids**](#function-head_ids-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves IDs all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**head\_ids**](#function-head_ids-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves IDs all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**head\_size**](#function-head_size-12) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hypergraph.md#typedef-id_type) hyperedge\_id) const<br>_Retrieves the size of the_ _BF-directed_ _hyperedge's__head_ _set ($\vert_[_**H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _\vert)._ |
|  [**size\_type**](group__HGL-Types.md#typedef-size_type) | [**head\_size**](#function-head_size-22) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Retrieves the size of the_ _BF-directed_ _hyperedge's__head_ _set ($\vert_[_**H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _\vert)._ |
|  std::vector&lt; [**size\_type**](group__HGL-Types.md#typedef-size_type) &gt; | [**head\_size\_map**](#function-head_size_map) () const<br>_Returns a mapped array of sizes of the_ _head_ _sets of hyperedged in a__BF-directed_ _hypergraph._ |
|  [**hyperedge\_formatter**](structhgl_1_1hypergraph_1_1hyperedge__formatter.md) | [**display**](#function-display) ([**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) hyperedge) const<br>_Returns a formatter object that safely encapsulates a hyperedge for stream output._  |




























## Detailed Description


This class relies on the provided `HypergraphTraits` to determine its behavior, element property types, and the underlying memory representation. It exposes a unified API for adding, removing, and iterating over vertices and hyperedges regardless of the backend.


#### Key Features




* **Policy-based design**: Behavior and representation are determined by `HypergraphTraits`.
* **Zero-cost Abstractions**: Core query logic is resolved at compile time through representation tags and static dispatch, removing unnecessary overhead.
* **Configurable directionality**: Support for both undirected and BF-directed hypergraphs.
* **Multiple representations**: Choose the underlying memory model and its layout to achieve the best performance for your needs:
    * [**list\_t**](structhgl_1_1repr_1_1list__t.md): Standard incidence list.
    * [**flat\_list\_t**](structhgl_1_1repr_1_1flat__list__t.md): Flattened incidence list.
    * [**matrix\_t**](structhgl_1_1repr_1_1matrix__t.md): Standard incidence matrix.
    * [**flat\_matrix\_t**](structhgl_1_1repr_1_1flat__matrix__t.md): Flattened incidence matrix.


* **Property support**: Vertices and hyperedges can carry arbitrary properties.
* **Unified API**: Consistent interface regardless of the underlying representation.
* **Standard Range Support**: Exposes lightweight views compliant with C++20 `std::ranges`, enabling functional-style iteration and algorithms.





#### Basic Definitions



A hypergraph \(G = (V, E)\) consists of a set of vertices \(V\) and a set of hyperedges \(E\).



* For undirected graphs, a hyperedge is a subset of the vertex set. Formally \(E \subseteq 2^V\) and \(e \in E \implies e \subseteq V\).
* For BF-directed graphs, a hyperedge is an ordered pair of disjoint subsets of the vertex set - the _tail_ (sources) and _head_ (targets) of the hyperedge. Formally \(e = (T_e, H_e)\) where \(T_e, H_e \subset V \land T_e \cap H_e = \emptyset\).





#### Example Usage




```cpp
hgl::undirected_hypergraph<> h(4, 2); // (1)!

h.bind({0, 1, 2}, 0); // (2)!

auto v_first = h.vertex(0); // (3)!
auto v_last = h.vertex(3);
auto e1 = h.hyperedge(1);
h.bind({v_first, v_last}, e1); // (4)!

std::cout << "Vertices: " << h.n_vertices() << '\n'; // (5)!
std::cout << "Hyperedges: " << h.n_hyperedges() << '\n';

for (auto v : h.vertices()) { // (6)!
    for (auto e : h.incident_hyperedges(v)) {
        process(v, e);
    }
}

std::cout << "Topology:\n" << h << '\n'; // (7)!
```



1. Instantiate an undirected hypergraph with 4 vertices and 2 hyperedges.


2. Bind vertices with IDs 0, 1 and 2 to the hyperedge with ID 0.


3. Retrieve the vertex and hyperedge descriptors in the hypergraph using the dedicated getters.


4. Bind the given vertices to the edge using descriptor objects.


5. Query the hypergraph's properties.


6. Iterate over the hypergraph's vertices and then over the incident edges of each vertex.


7. Utilize the builtin I/O stream support of the `hypergraph` class to print its topology to the console.



#### API Design: IDs vs. Descriptors



The `hypergraph` class exposes a dual API to accommodate different performance and ergonomic needs:



* **Inputs**: Most query methods are overloaded to accept either a raw `id_type` or a `vertex_type`/`hyperedge_type` descriptor. They are functionally identical.
* **Outputs**: Methods ending in `_ids` (e.g., `incident_vertex_ids`) return views of raw integral IDs. Methods without this suffix (e.g., `incident_vertices`) automatically map those IDs to the proper descriptor objects.
* **Performance**: Descriptor-returning methods incur a slight overhead if the hypergraph utilizes properties, as the property reference must be fetched and bound to each descriptor. If you only need topology, prefer the `_ids` variants.





#### Descriptor Invalidation Behavior



The hypergraph maintains the following invalidation semantics:



* **Vertex addition**: Does not invalidate vertex IDs. However, property references stored in existing vertex descriptors may be invalidated. Has no effect hyperedge descriptors.
* **Vertex removal**: May invalidate vertex descriptors, IDs, and property references. Has no effect hyperedge descriptors.
* **Edge addition**: Does not invalidate hyperedge IDs. However, property references stored in existing hyperedge descriptors may be invalidated. Has no effect on vertex descriptors.
* **Edge removal**: Invalidates hyperedge descriptors, IDs, and property references. Has no effect on vertex descriptors.
* **Property access**: References to vertex or edge properties obtained from the map may be invalidated by modifications to the hypergraph structure.





#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|HypergraphTraits|The core configuration type specifying the behavior and representation of the hypergraph.|[**c\_instantiation\_of&lt;hypergraph\_traits&gt;**](gl_concepts.md#gl-traits-c-instantiation-of)|







#### See Also




* [**undirected\_hypergraph**](group__HGL-Core.md#typedef-undirected_hypergraph) : Convenience alias for undirected hypergraphs.
* [**bf\_directed\_hypergraph**](group__HGL-Core.md#typedef-bf_directed_hypergraph) : Convenience alias for BF-directed hypergraphs.
* [**clone**](group__HGL-Core.md#function-clone) : Create a deep copy of a hypergraph.
* [**to**](group__HGL-Core.md#function-to) : Convert a hypergraph to a different representation.





> [!IMPORTANT] Copy Semantics
>
>
> `hypergraph` supports move semantics but disables copy assignment to prevent accidental expensive copies. Use [**clone**](group__HGL-Core.md#function-clone) to explicitly copy a hypergraph. 
>
>

> [!WARNING] Const Correctness & Properties (API Note)
>
>
> Currently, a `const` hypergraph guarantees **structural immutability** (vertices and hyperedges cannot be added or removed). However, vertex and hyperedge property maps are internally treated as `mutable`. This means that property payloads can still be modified through a `const hypergraph&`. Strict const-correct overloads for property access are planned for a future release. Proceed with caution in multi-threaded contexts. 
>
>




    
## Public Types Documentation




### typedef traits\_type 

_The traits type specifying the hypergraph's behavior and representation._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::traits_type =  HypergraphTraits;
```




<hr>



### typedef directional\_tag 

_Type tag specifying the directionality of the hypergraph._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::directional_tag =  typename traits_type::directional_tag;
```




<hr>



### typedef representation\_tag 

_Type tag indicating the underlying representation model._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::representation_tag =  typename traits_type::representation_tag;
```




<hr>



### typedef representation\_type 

_The underlying representation type matching the directional tag._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::representation_type =  typename representation_tag::template representation_type<directional_tag>;
```




<hr>



### typedef id\_type 

_Integral type used to identify vertices and hyperedges._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::id_type =  typename traits_type::id_type;
```




<hr>



### typedef vertex\_type 

_The descriptor type representing a vertex._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::vertex_type =  typename traits_type::vertex_type;
```




<hr>



### typedef vertex\_properties\_type 

_The user-defined property payload type associated with vertices._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::vertex_properties_type =  typename traits_type::vertex_properties_type;
```




<hr>



### typedef vertex\_properties\_map\_type 

_The container type used for storing the vertex properties mapping._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::vertex_properties_map_type =  std::conditional_t< traits::c_empty_properties<vertex_properties_type>, empty_properties_map, std::vector<vertex_properties_type> >;
```




<hr>



### typedef hyperedge\_type 

_The descriptor type representing a hyperedge._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::hyperedge_type =  typename traits_type::hyperedge_type;
```




<hr>



### typedef hyperedge\_properties\_type 

_The user-defined property payload type associated with hyperedges._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::hyperedge_properties_type =  typename traits_type::hyperedge_properties_type;
```




<hr>



### typedef hyperedge\_properties\_map\_type 

_The container type used for storing hyperedge properties mapping._ 
```cpp
using hgl::hypergraph< HypergraphTraits >::hyperedge_properties_map_type =  std::conditional_t< traits::c_empty_properties<hyperedge_properties_type>, empty_properties_map, std::vector<hyperedge_properties_type> >;
```




<hr>
## Public Functions Documentation




### function hypergraph [1/3]

_Constructs a hypergraph with the given number of vertices and hyperedges (empty by default)._ 
```cpp
inline explicit hgl::hypergraph::hypergraph (
    const  size_type n_vertices=0uz,
    const  size_type n_hyperedges=0uz
) 
```





**Parameters:**


* `n_vertices` The initial number of vertices. 
* `n_hyperedges` The initial number of hyperedges.


> [!NOTE] Memory Management
>
>
> Constructing a non-empty hypergraph and retrieving descriptors of the hypergraph's elements provides significantly better performance than adding elements one by one (especially for large hypergraphs) due to the ability to allocate the required memory for the internal incidence representation, which drastically reduces the overhead associated with reallocations associated with adding elements sequentially. 
>
>



        

<hr>



### function hypergraph [2/3]

_Default move constructor._ 
```cpp
hgl::hypergraph::hypergraph (
    hypergraph &&
) noexcept
```




<hr>



### function operator= 

_Default move assignment operator._ 
```cpp
hypergraph & hgl::hypergraph::operator= (
    hypergraph &&
) noexcept
```




<hr>



### function ~hypergraph 

_Default destructor._ 
```cpp
hgl::hypergraph::~hypergraph () = default
```




<hr>



### function operator= 

_Hypergraph copy assignment is disabled to avoid accidental copies. Use_ [_**clone**_](group__HGL-Core.md#function-clone) _instead._
```cpp
hypergraph & hgl::hypergraph::operator= (
    const  hypergraph &
) = delete
```




<hr>



### function n\_vertices 

_Returns the total number of vertices in the hypergraph._ 
```cpp
inline size_type hgl::hypergraph::n_vertices () noexcept const
```





**Returns:**

The vertex count: $\|V\|$. 





        

<hr>



### function n\_hyperedges 

_Returns the total number of hyperedges in the hypergraph._ 
```cpp
inline size_type hgl::hypergraph::n_hyperedges () noexcept const
```





**Returns:**

The hyperedge count: $\|E\|$. 





        

<hr>



### function add\_vertex 

_Adds a new, default-initialized vertex to the hypergraph._ 
```cpp
inline vertex_type hgl::hypergraph::add_vertex () 
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

_Adds a new vertex with the given properties to the hypergraph._ 
```cpp
inline vertex_type hgl::hypergraph::add_vertex_with (
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

_Adds multiple default-initialized vertices to the hypergraph._ 
```cpp
inline void hgl::hypergraph::add_vertices (
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
inline void hgl::hypergraph::add_vertices_with (
    const traits::c_sized_range_of< vertex_properties_type > auto & properties_rng
) 
```





**Parameters:**


* `properties_rng` A forward range of properties to initialize the new vertices with.


> [!IMPORTANT] ID Stability
>
>
> Adding vertices does **not** invalidate existing vertex IDs. **However**, property references stored in existing vertex descriptors may be invalidated. 
>
>



        

<hr>



### function remove\_vertex [1/2]

_Removes a vertex by its ID, unbinding it from its incident hyperedges._ 
```cpp
inline void hgl::hypergraph::remove_vertex (
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
> * References to all properties associated with the vertices with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs, descriptors or properties. 
>
>



        

<hr>



### function remove\_vertex [2/2]

_Removes a vertex using its descriptor, unbinding it from its incident hyperedges._ 
```cpp
inline void hgl::hypergraph::remove_vertex (
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
> * References to all properties associated with the vertices with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs, descriptors or properties. 
>
>



        

<hr>



### function remove\_vertices [1/2]

_Removes a range of vertices using their IDs._ 
```cpp
inline void hgl::hypergraph::remove_vertices (
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
> * References to all properties associated with the vertices with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs, descriptors or properties. 
>
>



        

<hr>



### function remove\_vertices [2/2]

_Removes a range of vertices using their descriptors._ 
```cpp
inline void hgl::hypergraph::remove_vertices (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng
) 
```





**Parameters:**


* `vertex_rng` A forward range containing the descriptors of vertices to remove. 



**Exception:**


* `std::invalid_argument` If any vertex descriptor is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a vertex invalidates:
> * All vertex descriptors and IDs for vertices with higher IDs (they shift down).
> * References to all properties associated with the vertices with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external vertex IDs, descriptors or properties. 
>
>



        

<hr>



### function has\_vertex [1/2]

_Checks if a vertex with the given ID exists in the hypergraph._ 
```cpp
inline bool hgl::hypergraph::has_vertex (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID to check. 



**Returns:**

`true` if the vertex exists, `false` otherwise. 





        

<hr>



### function has\_vertex [2/2]

_Checks if the vertex referenced by the provided descriptor exists in the hypergraph._ 
```cpp
inline bool hgl::hypergraph::has_vertex (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The descriptor to check. 



**Returns:**

`true` if the vertex exists, `false` otherwise. 





        

<hr>



### function vertex 

_Returns a descriptor of the vertex with the given_ _bounds-checked_ _ID._
```cpp
inline vertex_type hgl::hypergraph::vertex (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding vertex descriptor. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function at [1/2]

_Returns a descriptor of the vertex with the given bounds-checked ID._ 
```cpp
inline vertex_type hgl::hypergraph::at (
    vertex_t,
    const  id_type vertex_id
) const
```




> [!NOTE] API Note
>
>
> Calling `hypergraph.at(hgl::vertex, id)` is equivalent to calling `hypergraph.vertex(id)`. However, the `at` methods of the `hypergraph` class are designed to be called in generic functions that can operate on vertices and hyperedges alike. 
>
>



**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding `vertex_descriptor`. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function vertex\_unchecked 

_Returns a descriptor of the vertex with the given ID without bounds checking._ 
```cpp
inline vertex_type hgl::hypergraph::vertex_unchecked (
    const  id_type vertex_id
) const
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

_Returns a descriptor of the vertex with the given ID without bounds checking._ 
```cpp
inline vertex_type hgl::hypergraph::operator[] (
    vertex_t,
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The corresponding vertex descriptor.



> [!NOTE] API Note
>
>
> Calling `hypergraph[ hgl::vertex , id]` is equivalent to calling `hypergraph.vertex_unchecked(id)`. However, the subscript operators of the `hypergraph` class are designed to be called in generic functions that can operate on vertices and hyperedges alike. 
>
>

> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed. Passing an invalid ID results in Undefined Behavior. 
>
>



        

<hr>



### function vertices 

_Returns a lazily evaluated, random-access view of all vertex descriptors in the hypergraph._ 
```cpp
inline auto hgl::hypergraph::vertices () noexcept const
```





**Returns:**

A view yielding descriptors for every vertex. 





        

<hr>



### function vertex\_ids 

_Returns a lazily evaluated, random-access view of all active vertex IDs in the hypergraph._ 
```cpp
inline auto hgl::hypergraph::vertex_ids () noexcept const
```





**Returns:**

A view yielding all valid vertex IDs. 





        

<hr>



### function vertex\_properties 

_Retrieves a reference to the properties of a specified vertex._ 
```cpp
inline vertex_properties_type & hgl::hypergraph::vertex_properties (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

A mutable reference to the vertex's properties. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function vertex\_properties\_map 

_Retrieves a lazily evaluated, random-access view over all vertex properties in the hypergraph._ 
```cpp
inline auto hgl::hypergraph::vertex_properties_map () noexcept const
```





**Returns:**

A view mapping each active vertex index to its property. 





        

<hr>



### function add\_hyperedge [1/9]

_Adds a new, default-initialized hyperedge to the hypergraph._ 
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge () 
```





**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [1/9]

_Adds a new hyperedge with the given properties to the hypergraph._ 
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [2/9]

_Adds a new_ _undirected_ _hyperedge and immediately binds a range of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    const traits::c_forward_range_of< id_type > auto & vertex_id_rng
) 
```





**Parameters:**


* `vertex_id_rng` A forward range of vertex IDs to bind to the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [3/9]

_Adds a new_ _undirected_ _hyperedge and immediately binds a list of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    std::initializer_list< id_type > vertex_ids
) 
```





**Parameters:**


* `vertex_ids` An initializer list of vertex IDs to bind to the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [4/9]

_Adds a new_ _undirected_ _hyperedge and immediately binds a range of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng
) 
```





**Parameters:**


* `vertex_rng` A forward range of vertex descriptors to bind to the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [5/9]

_Adds a new_ _undirected_ _hyperedge and immediately binds a list of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    std::initializer_list< vertex_type > vertices
) 
```





**Parameters:**


* `vertices` An initializer list of vertex descriptors to bind to the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [2/9]

_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a range of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    const traits::c_forward_range_of< id_type > auto & vertex_id_rng,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `vertex_id_rng` A forward range of vertex IDs to bind to the new hyperedge. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [3/9]

_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a list of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    std::initializer_list< id_type > vertex_ids,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `vertex_ids` An initializer list of vertex IDs to bind to the new hyperedge. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [4/9]

_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a range of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `vertex_rng` A forward range of vertex descriptors to bind to the new hyperedge. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [5/9]

_Adds a new_ _undirected_ _hyperedge with the given properties and immediately binds a list of vertices to it._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    std::initializer_list< vertex_type > vertices,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `vertices` An initializer list of vertex descriptors to bind to the new hyperedge. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [6/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    const traits::c_forward_range_of< id_type > auto & tail_id_rng,
    const traits::c_forward_range_of< id_type > auto & head_id_rng
) 
```





**Parameters:**


* `tail_id_rng` A forward range of vertex IDs to bind to the new hyperedge's _tail_. 
* `head_id_rng` A forward range of vertex IDs to bind to the new hyperedge's _head_. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [7/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    std::initializer_list< id_type > tail_ids,
    std::initializer_list< id_type > head_ids
) 
```





**Parameters:**


* `tail_ids` An initializer of vertex IDs to bind to the new hyperedge's _tail_. 
* `head_ids` An initializer of vertex IDs to bind to the new hyperedge's _head_. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [8/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    const traits::c_forward_range_of< vertex_type > auto & tail_rng,
    const traits::c_forward_range_of< vertex_type > auto & head_rng
) 
```





**Parameters:**


* `tail_rng` A forward range of vertex descriptors to bind to the new hyperedge's _tail_. 
* `head_rng` A forward range of vertex descriptors to bind to the new hyperedge's _head_. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge [9/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge (
    std::initializer_list< vertex_type > tail,
    std::initializer_list< vertex_type > head
) 
```





**Parameters:**


* `tail` An intializer list of vertex descriptors to bind to the new hyperedge's _tail_. 
* `head` An intializer list of vertex descriptors to bind to the new hyperedge's _head_. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [6/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    const traits::c_forward_range_of< id_type > auto & tail_id_rng,
    const traits::c_forward_range_of< id_type > auto & head_id_rng,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `tail_id_rng` A forward range of vertex IDs to bind to the new hyperedge's _tail_. 
* `head_id_rng` A forward range of vertex IDs to bind to the new hyperedge's _head_. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [7/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    std::initializer_list< id_type > tail_ids,
    std::initializer_list< id_type > head_ids,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `tail_ids` An initializer list of vertex IDs to bind to the new hyperedge's _tail_. 
* `head_ids` An initializer list of vertex IDs to bind to the new hyperedge's _head_. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [8/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    const traits::c_forward_range_of< vertex_type > auto & tail_rng,
    const traits::c_forward_range_of< vertex_type > auto & head_rng,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `tail_rng` A forward range of vertex descriptors to bind to the new hyperedge's _tail_. 
* `head_rng` A forward range of vertex descriptors to bind to the new hyperedge's _head_. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedge\_with [9/9]

_Adds a new_ _BF-directed_ _hyperedge and immediately binds vertices to its__tail_ _and__head_ _._
```cpp
inline hyperedge_type hgl::hypergraph::add_hyperedge_with (
    std::initializer_list< vertex_type > tail,
    std::initializer_list< vertex_type > head,
    hyperedge_properties_type properties
) 
```





**Parameters:**


* `tail` An initializer list of vertex descriptor to bind to the new hyperedge's _tail_. 
* `head` An initializer list of vertex descriptor to bind to the new hyperedge's _head_. 
* `properties` The property payload for the new hyperedge. 



**Returns:**

A descriptor of the newly created hyperedge.



> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedges 

_Adds multiple default-initialized hyperedges to the hypergraph._ 
```cpp
inline void hgl::hypergraph::add_hyperedges (
    const  size_type n
) 
```





**Parameters:**


* `n` The number of hyperedges to add.


> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function add\_hyperedges\_with 

_Adds multiple hyperedges based on a range of property payloads._ 
```cpp
inline void hgl::hypergraph::add_hyperedges_with (
    const traits::c_sized_range_of< hyperedge_properties_type > auto & properties_rng
) 
```





**Parameters:**


* `properties_rng` A forward range of properties to initialize the new hyperedges with.


> [!IMPORTANT] ID Stability
>
>
> Adding hyperedges does **not** invalidate existing hyperedge IDs. **However**, property references stored in existing hyperedge descriptors may be invalidated. 
>
>



        

<hr>



### function remove\_hyperedge [1/2]

_Removes a hyperedge by its ID, unbinding it from its incident vertices._ 
```cpp
inline void hgl::hypergraph::remove_hyperedge (
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge to remove. 



**Exception:**


* `std::invalid_argument` If the ID is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a hyperedge invalidates:
> * All hyperedge descriptors and IDs for hyperedges with higher IDs (they shift down).
> * References to all properties associated with the hyperedges with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external hyperedge IDs, descriptors, or properties. 
>
>



        

<hr>



### function remove\_hyperedge [2/2]

_Removes a hyperedge using its descriptor, unbinding it from its incident vertices._ 
```cpp
inline void hgl::hypergraph::remove_hyperedge (
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `hyperedge` The descriptor of the hyperedge to remove. 



**Exception:**


* `std::invalid_argument` If the ID is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a hyperedge invalidates:
> * All hyperedge descriptors and IDs for hyperedges with higher IDs (they shift down).
> * References to all properties associated with the hyperedges with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external hyperedge IDs, descriptors, or properties. 
>
>



        

<hr>



### function remove\_hyperedges\_from [1/2]

_Removes a range of hyperedges using their IDs._ 
```cpp
inline void hgl::hypergraph::remove_hyperedges_from (
    const traits::c_forward_range_of< id_type > auto & hyperedge_id_rng
) 
```





**Parameters:**


* `hyperedge_id_rng` A forward range containing the IDs of hyperedges to remove. 



**Exception:**


* `std::invalid_argument` If any hyperedge ID in the range is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a hyperedge invalidates:
> * All hyperedge descriptors and IDs for hyperedges with higher IDs (they shift down).
> * References to all properties associated with the hyperedges with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external hyperedge IDs, descriptors, or properties. 
>
>



        

<hr>



### function remove\_hyperedges\_from [2/2]

_Removes a range of hyperedges using their descriptors._ 
```cpp
inline void hgl::hypergraph::remove_hyperedges_from (
    const traits::c_forward_range_of< hyperedge_type > auto & hyperedge_rng
) 
```





**Parameters:**


* `hyperedge_id_rng` A forward range containing the descriptors of hyperedges to remove. 



**Exception:**


* `std::invalid_argument` If any hyperedge ID in the range is invalid.


> [!WARNING] Descriptor and ID Invalidation
>
>
> Removing a hyperedge invalidates:
> * All hyperedge descriptors and IDs for hyperedges with higher IDs (they shift down).
> * References to all properties associated with the hyperedges with IDs shifted as a result of the removal operation.
>
>
>
>
> Proceed with caution when maintaining external hyperedge IDs, descriptors, or properties. 
>
>



        

<hr>



### function has\_hyperedge [1/2]

_Checks if a hyperedge with the given ID exists in the hypergraph._ 
```cpp
inline bool hgl::hypergraph::has_hyperedge (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID to check. 



**Returns:**

`true` if the hyperedge exists, `false` otherwise. 





        

<hr>



### function has\_hyperedge [2/2]

_Checks if the hyperedge referenced by the provided descriptor exists in the hypergraph._ 
```cpp
inline bool hgl::hypergraph::has_hyperedge (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The descriptor to check. 



**Returns:**

`true` if the hyperedge exists, `false` otherwise. 





        

<hr>



### function hyperedge 

_Returns a descriptor of the hyperedge with the given_ _bounds-checked_ _ID._
```cpp
inline hyperedge_type hgl::hypergraph::hyperedge (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The corresponding hyperedge descriptor. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function at [2/2]

_Returns a descriptor of the hyperedge with the given_ _bounds-checked_ _ID._
```cpp
inline hyperedge_type hgl::hypergraph::at (
    hyperedge_t,
    const  id_type hyperedge_id
) const
```




> [!NOTE] API Note
>
>
> Calling `hypergraph.at(hgl::hyperedge, id)` is equivalent to calling `hypergraph.hyperedge(id)`. However, the `at` methods of the `hypergraph` class are designed to be called in generic functions that can operate on hyperedges and vertices alike. 
>
>



**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The corresponding hyperedge descriptor. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function hyperedge\_unchecked 

_Returns a descriptor of the hyperedge with the given ID without bounds checking._ 
```cpp
inline hyperedge_type hgl::hypergraph::hyperedge_unchecked (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The corresponding hyperedge descriptor.



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed. Passing an invalid ID results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a descriptor of the hyperedge with the given ID without bounds checking._ 
```cpp
inline hyperedge_type hgl::hypergraph::operator[] (
    hyperedge_t,
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The corresponding hyperedge descriptor.



> [!NOTE] API Note
>
>
> Calling `hypergraph[ hgl::hyperedge , id]` is equivalent to calling `hypergraph.hyperedge_unchecked(id)`. However, the subscript operators of the `hypergraph` class are designed to be called in generic functions that can operate on hyperedges and vertices alike. 
>
>

> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed. Passing an invalid ID results in Undefined Behavior. 
>
>



        

<hr>



### function hyperedges 

_Returns a lazily evaluated, random-access view of all hyperedge descriptors in the hypergraph._ 
```cpp
inline auto hgl::hypergraph::hyperedges () noexcept const
```





**Returns:**

A view yielding descriptors for every hyperedge. 





        

<hr>



### function hyperedge\_ids 

_Returns a lazily evaluated, random-access view of all active hyperedge IDs in the hypergraph._ 
```cpp
inline auto hgl::hypergraph::hyperedge_ids () noexcept const
```





**Returns:**

A view yielding all valid hyperedge IDs. 





        

<hr>



### function hyperedge\_properties 

_Retrieves a reference to the properties of a specified hyperedge._ 
```cpp
inline hyperedge_properties_type & hgl::hypergraph::hyperedge_properties (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

A mutable reference to the hyperedge's properties. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function hyperedge\_properties\_map 

_Retrieves a lazily evaluated, random-access view over all hyperedge properties in the hypergraph._ 
```cpp
inline auto hgl::hypergraph::hyperedge_properties_map () noexcept const
```





**Returns:**

A view mapping each active hyperedge index to its property. 





        

<hr>



### function bind [1/10]

_Binds a vertex to a hyperedge in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either ID is invalid. 




        

<hr>



### function bind [2/10]

_Binds a vertex to a hyperedge in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    vertex_type vertex,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either descriptor is invalid. 




        

<hr>



### function bind [3/10]

_Binds a range of vertices to a single hyperedge in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    const traits::c_forward_range_of< id_type > auto & vertex_id_rng,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id_rng` A forward range of vertex IDs. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge ID or any of the vertex IDs is invalid. 




        

<hr>



### function bind [4/10]

_Binds a list of vertices to a single hyperedge in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    std::initializer_list< id_type > vertex_ids,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_ids` An initializer list of vertex IDs. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge ID or any of the vertex IDs is invalid. 




        

<hr>



### function bind [5/10]

_Binds a range of vertices to a single hyperedge in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex_rng` A forward range of vertex descriptors. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge descriptor or any of the vertex descriptors is invalid. 




        

<hr>



### function bind [6/10]

_Binds a list of vertices to a single hyperedge in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    std::initializer_list< vertex_type > vertices,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertices` An initializer list of vertex descriptors. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge descriptor or any of the vertex descriptors is invalid. 




        

<hr>



### function bind [7/10]

_Binds a single vertex to a range of hyperedges in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    const  id_type vertex_id,
    const traits::c_forward_range_of< id_type > auto & hyperedge_id_rng
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id_rng` A forward range of hyperedge IDs. 



**Exception:**


* `std::invalid_argument` If either the vertex ID or any of the hyperedge IDs is invalid. 




        

<hr>



### function bind [8/10]

_Binds a single vertex to a list of hyperedges in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    const  id_type vertex_id,
    std::initializer_list< id_type > hyperedge_ids
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id_rng` An initializer list of hyperedge IDs. 



**Exception:**


* `std::invalid_argument` If either the vertex ID or any of the hyperedge IDs is invalid. 




        

<hr>



### function bind [9/10]

_Binds a single vertex to a range of hyperedges in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    vertex_type vertex,
    const traits::c_forward_range_of< hyperedge_type > auto & hyperedge_rng
) 
```





**Parameters:**


* `vertex_id` The descriptor of the vertex. 
* `hyperedge_id_rng` A forward range of hyperedge descriptors. 



**Exception:**


* `std::invalid_argument` If either the vertex descriptor or any of the hyperedge descriptors is invalid. 




        

<hr>



### function bind [10/10]

_Binds a single vertex to a list of hyperedges in an undirected hypergraph._ 
```cpp
inline void hgl::hypergraph::bind (
    vertex_type vertex,
    std::initializer_list< hyperedge_type > hyperedges
) 
```





**Parameters:**


* `vertex_id` The descriptor of the vertex. 
* `hyperedge_id_rng` An initializer list of hyperedge descriptors. 



**Exception:**


* `std::invalid_argument` If either the vertex descriptor or any of the hyperedge descriptors is invalid. 




        

<hr>



### function bind\_tail [1/10]

_Binds a vertex to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either ID is invalid. 




        

<hr>



### function bind\_tail [2/10]

_Binds a vertex to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    vertex_type vertex,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either descriptor is invalid. 




        

<hr>



### function bind\_tail [3/10]

_Binds a range of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    const traits::c_forward_range_of< id_type > auto & vertex_id_rng,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id_rng` A forward range of vertex IDs. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge ID or any of the vertex IDs is invalid. 




        

<hr>



### function bind\_tail [4/10]

_Binds a list of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    std::initializer_list< id_type > vertex_ids,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_ids` An initializer list of vertex IDs. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge ID or any of the vertex IDs is invalid. 




        

<hr>



### function bind\_tail [5/10]

_Binds a range of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex_rng` A forward range of vertex descriptors. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge descriptor or any of the vertex descriptors is invalid. 




        

<hr>



### function bind\_tail [6/10]

_Binds a range of vertices to the_ _tail_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    std::initializer_list< vertex_type > vertices,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertices` An initializer list of vertex descriptors. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge descriptor or any of the vertex descriptors is invalid. 




        

<hr>



### function bind\_tail [7/10]

_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    const  id_type vertex_id,
    const traits::c_forward_range_of< id_type > auto & hyperedge_id_rng
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id_rng` A forward range of hyperedge IDs. 



**Exception:**


* `std::invalid_argument` If either the vertex ID or any of the hyperedge IDs is invalid. 




        

<hr>



### function bind\_tail [8/10]

_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    const  id_type vertex_id,
    std::initializer_list< id_type > hyperedge_ids
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_ids` An initializer list of hyperedge IDs. 



**Exception:**


* `std::invalid_argument` If either the vertex ID or any of the hyperedge IDs is invalid. 




        

<hr>



### function bind\_tail [9/10]

_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    vertex_type vertex,
    const traits::c_forward_range_of< hyperedge_type > auto & hyperedge_rng
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedge_rng` A forward range of hyperedge descriptors. 



**Exception:**


* `std::invalid_argument` If either the vertex descriptor or any of the hyperedge descriptors is invalid. 




        

<hr>



### function bind\_tail [10/10]

_Binds a single vertex to the_ _tail_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_tail (
    vertex_type vertex,
    std::initializer_list< hyperedge_type > hyperedges
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedges` An initializer list of hyperedge descriptors. 



**Exception:**


* `std::invalid_argument` If either the vertex descriptor or any of the hyperedge descriptors is invalid. 




        

<hr>



### function bind\_head [1/10]

_Binds a vertex to the_ _head_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either ID is invalid. 




        

<hr>



### function bind\_head [2/10]

_Binds a vertex to the_ _head_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    vertex_type vertex,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either descriptor is invalid. 




        

<hr>



### function bind\_head [3/10]

_Binds a range of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    const traits::c_forward_range_of< id_type > auto & vertex_id_rng,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id_rng` A forward range of vertex IDs. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge ID or any of the vertex IDs is invalid. 




        

<hr>



### function bind\_head [4/10]

_Binds a list of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    std::initializer_list< id_type > vertex_ids,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_ids` An initializer list of vertex IDs. 
* `hyperedge_id` The ID of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge ID or any of the vertex IDs is invalid. 




        

<hr>



### function bind\_head [5/10]

_Binds a range of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    const traits::c_forward_range_of< vertex_type > auto & vertex_rng,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex_rng` A forward range of vertex descriptors. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge descriptor or any of the vertex descriptors is invalid. 




        

<hr>



### function bind\_head [6/10]

_Binds a range of vertices to the_ _head_ _of a single hyperedge in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    std::initializer_list< vertex_type > vertices,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertices` An initializer list of vertex descriptors. 
* `hyperedge` The descriptor of the hyperedge. 



**Exception:**


* `std::invalid_argument` If either the hyperedge descriptor or any of the vertex descriptors is invalid. 




        

<hr>



### function bind\_head [7/10]

_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    const  id_type vertex_id,
    const traits::c_forward_range_of< id_type > auto & hyperedge_id_rng
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id_rng` A forward range of hyperedge IDs. 



**Exception:**


* `std::invalid_argument` If either the vertex ID or any of the hyperedge IDs is invalid. 




        

<hr>



### function bind\_head [8/10]

_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    const  id_type vertex_id,
    std::initializer_list< id_type > hyperedge_ids
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_ids` An initializer list of hyperedge IDs. 



**Exception:**


* `std::invalid_argument` If either the vertex ID or any of the hyperedge IDs is invalid. 




        

<hr>



### function bind\_head [9/10]

_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    vertex_type vertex,
    const traits::c_forward_range_of< hyperedge_type > auto & hyperedge_rng
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedge_rng` A forward range of hyperedge descriptors. 



**Exception:**


* `std::invalid_argument` If either the vertex descriptor or any of the hyperedge descriptors is invalid. 




        

<hr>



### function bind\_head [10/10]

_Binds a single vertex to the_ _head_ _of a range of hyperedges in a BF-directed hypergraph._
```cpp
inline void hgl::hypergraph::bind_head (
    vertex_type vertex,
    std::initializer_list< hyperedge_type > hyperedges
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedges` An initializer list of hyperedge descriptors. 



**Exception:**


* `std::invalid_argument` If either the vertex descriptor or any of the hyperedge descriptors is invalid. 




        

<hr>



### function unbind [1/2]

_Unbinds a vertex from a hyperedge._ 
```cpp
inline void hgl::hypergraph::unbind (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 




        

<hr>



### function unbind [2/2]

_Unbinds a vertex from a hyperedge._ 
```cpp
inline void hgl::hypergraph::unbind (
    vertex_type vertex,
    hyperedge_type hyperedge
) 
```





**Parameters:**


* `vertex` The descriptor of the vertex. 
* `hyperedge` The descriptor of the hyperedge. 




        

<hr>



### function are\_incident [1/2]

_Evaluates whether the given vertex and hyperedge are incident._ 
```cpp
inline bool hgl::hypergraph::are_incident (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) const
```



#### Formal Definition




* **Undirected Hypergraphs:** A vertex $v$ and a hyperedge $e$ are incident if \(v \in e\).
* **BF-directed Hypergraphs:** A vertex $v$ and a hyperedge $e$ are incident if \(v \in T(e) \lor v \in H(e)\).






**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

`true` if the vertex belongs to the hyperedge (in any direction), `false` otherwise. 






        

<hr>



### function are\_incident [2/2]

_Evaluates whether a vertex and a hyperedge are currently incident using descriptors._ 
```cpp
inline bool hgl::hypergraph::are_incident (
    vertex_type vertex,
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `vertex` The `vertex_descriptor` mapping to the vertex. 
* `hyperedge` The `hyperedge_descriptor` mapping to the hyperedge. 



**Returns:**

`true` if the vertex belongs to the hyperedge, `false` otherwise. 





        

<hr>



### function is\_tail [1/2]

_Evaluates whether a vertex belongs to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline bool hgl::hypergraph::is_tail (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) const
```



Formally, the function evaluates whether \(v \in T(e)\) or is one of the sources of the hyperedge.




**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

`true` if the vertex is in the _tail_ set of the hyperedge, `false` otherwise. 





        

<hr>



### function is\_tail [2/2]

_Evaluates whether a vertex belongs to the_ _tail_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline bool hgl::hypergraph::is_tail (
    vertex_type vertex,
    hyperedge_type hyperedge
) const
```



Formally, the function evaluates whether \(v \in T(e)\) or is one of the sources of the hyperedge.




**Parameters:**


* `vertex` The `vertex_descriptor` mapping to the vertex. 
* `hyperedge` The `hyperedge_descriptor` mapping to the hyperedge. 



**Returns:**

`true` if the vertex is in the _tail_ set of the hyperedge, `false` otherwise. 





        

<hr>



### function is\_head [1/2]

_Evaluates whether a vertex belongs to the_ _head_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline bool hgl::hypergraph::is_head (
    const  id_type vertex_id,
    const  id_type hyperedge_id
) const
```



Formally, the function evaluates whether \(v \in H(e)\) or is one of the targets of the hyperedge.




**Parameters:**


* `vertex_id` The ID of the vertex. 
* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

`true` if the vertex is in the _head_ set of the hyperedge, `false` otherwise. 





        

<hr>



### function is\_head [2/2]

_Evaluates whether a vertex belongs to the_ _head_ _of a hyperedge in a BF-directed hypergraph._
```cpp
inline bool hgl::hypergraph::is_head (
    vertex_type vertex,
    hyperedge_type hyperedge
) const
```



Formally, the function evaluates whether \(v \in H(e)\) or is one of the targets of the hyperedge.




**Parameters:**


* `vertex` The `vertex_descriptor` mapping to the vertex. 
* `hyperedge` The `hyperedge_descriptor` mapping to the hyperedge. 



**Returns:**

`true` if the vertex is in the _head_ set of the hyperedge, `false` otherwise. 





        

<hr>



### function incident\_hyperedges [1/2]

_Retrieves all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._
```cpp
inline auto hgl::hypergraph::incident_hyperedges (
    const  id_type vertex_id
) 
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of incident hyperedges. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function incident\_hyperedges [2/2]

_Retrieves all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._
```cpp
inline auto hgl::hypergraph::incident_hyperedges (
    vertex_type vertex
) 
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of incident hyperedges. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function incident\_hyperedge\_ids [1/2]

_Retrieves IDs of all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._
```cpp
inline auto hgl::hypergraph::incident_hyperedge_ids (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of incident hyperedge IDs. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function incident\_hyperedge\_ids [2/2]

_Retrieves IDs of all hyperedges incident with a vertex (_ \(\{e in E : v \in e\}\) _)._
```cpp
inline auto hgl::hypergraph::incident_hyperedge_ids (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of incident hyperedge IDs. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function degree [1/2]

_Calculates the degree of a vertex in the hypergraph._ 
```cpp
inline size_type hgl::hypergraph::degree (
    const  id_type vertex_id
) const
```



#### Formal Definition



The degree of a vertex in a hypergraph is the total number of hyperedges incident with a vertex.



\[deg(v) =
\begin{cases}
\vert\{e \in E : v \in e\}\vert & \text{if } H \text{ is undirected}
\\ deg_{in}(v) + deg_{out}(v) & \text{if } H \text{ is BF-directed}
\end{cases}\]
   

**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The degree of the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function degree [2/2]

_Calculates the degree of a vertex in the hypergraph._ 
```cpp
inline size_type hgl::hypergraph::degree (
    vertex_type vertex
) const
```



#### Formal Definition



The degree of a vertex in a hypergraph is the total number of hyperedges incident with a vertex.



\[deg(v) =
\begin{cases}
\vert\{e \in E : v \in e\}\vert & \text{if } H \text{ is undirected}
\\ deg_{in}(v) + deg_{out}(v) & \text{if } H \text{ is BF-directed}
\end{cases}\]
   

**Parameters:**


* `vertex` The descriptor of the vertex. 



**Returns:**

The degree of the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function degree\_map 

_Returns a mapped array of degrees for all vertices._ 
```cpp
inline std::vector< size_type > hgl::hypergraph::degree_map () const
```





**Returns:**

A vector where the index aligns with the vertex ID containing its degree. 





        

<hr>



### function out\_hyperedges [1/2]

_Retrieves all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::out_hyperedges (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of outgoing hyperedges. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function out\_hyperedges [2/2]

_Retrieves all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::out_hyperedges (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of outgoing hyperedges. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function out\_hyperedge\_ids [1/2]

_Retrieves IDs of all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::out_hyperedge_ids (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of outgoing hyperedge IDs. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function out\_hyperedge\_ids [2/2]

_Retrieves IDs of all outgoing (tail-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in T(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::out_hyperedge_ids (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of outgoing hyperedge IDs. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function out\_degree [1/2]

_Calculates the out-degree of a vertex in the_ _BF-directed_ _hypergraph._
```cpp
inline size_type hgl::hypergraph::out_degree (
    const  id_type vertex_id
) const
```



#### Formal Definition



The out-degree of a vertex in a _BF-directed_ hypergraph is the number of hyperedges backward-incident with the vertex (such that the vertex belongs to the tail of the hyperedge).



\[deg_{in}(v) = \vert\{e \in E : v \in H(e)\}\vert\]
   

**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The out-degree of the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function out\_degree [2/2]

_Calculates the out-degree of a vertex in the_ _BF-directed_ _hypergraph._
```cpp
inline size_type hgl::hypergraph::out_degree (
    vertex_type vertex
) const
```



#### Formal Definition



The out-degree of a vertex in a _BF-directed_ hypergraph is the number of hyperedges backward-incident with the vertex (such that the vertex belongs to the tail of the hyperedge).



\[deg_{in}(v) = \vert\{e \in E : v \in H(e)\}\vert\]
   

**Parameters:**


* `vertex` The descriptor of the vertex. 



**Returns:**

The out-degree of the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function out\_degree\_map 

_Returns a mapped array of out-degrees for all vertices._ 
```cpp
inline std::vector< size_type > hgl::hypergraph::out_degree_map () const
```





**Returns:**

A vector where the index aligns with the vertex ID containing its out-degree. 





        

<hr>



### function in\_hyperedges [1/2]

_Retrieves all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::in_hyperedges (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of incoming hyperedges. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function in\_hyperedges [2/2]

_Retrieves all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::in_hyperedges (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of incoming hyperedges. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function in\_hyperedge\_ids [1/2]

_Retrieves IDs of all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::in_hyperedge_ids (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The vertex ID. 



**Returns:**

A view representing the set of incoming hyperedge IDs. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 




        

<hr>



### function in\_hyperedge\_ids [2/2]

_Retrieves IDs of all incoming (head-bound) hyperedges of a vertex in a_ _BF-directed_ _hypergraph (_\(\{e \in E : v \in H(e)\}\) _)._
```cpp
inline auto hgl::hypergraph::in_hyperedge_ids (
    vertex_type vertex
) const
```





**Parameters:**


* `vertex` The vertex descriptor. 



**Returns:**

A view representing the set of incoming hyperedge IDs. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 




        

<hr>



### function in\_degree [1/2]

_Calculates the in-degree of a vertex in the_ _BF-directed_ _hypergraph._
```cpp
inline size_type hgl::hypergraph::in_degree (
    const  id_type vertex_id
) const
```



#### Formal Definition



The in-degree of a vertex in a _BF-directed_ hypergraph is the number of hyperedges forward-incident with the vertex (such that the vertex belongs to the head of the hyperedge).



\[deg_{in}(v) = \vert\{e \in E : v \in H(e)\}\vert\]
   

**Parameters:**


* `vertex_id` The ID of the vertex. 



**Returns:**

The in-degree of the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex ID is invalid. 





        

<hr>



### function in\_degree [2/2]

_Calculates the in-degree of a vertex in the_ _BF-directed_ _hypergraph._
```cpp
inline size_type hgl::hypergraph::in_degree (
    vertex_type vertex
) const
```



#### Formal Definition



The in-degree of a vertex in a _BF-directed_ hypergraph is the number of hyperedges forward-incident with the vertex (such that the vertex belongs to the head of the hyperedge).



\[deg_{in}(v) = \vert\{e \in E : v \in H(e)\}\vert\]
   

**Parameters:**


* `vertex` The descriptor of the vertex. 



**Returns:**

The in-degree of the vertex. 




**Exception:**


* `std::invalid_argument` If the vertex descriptor is invalid. 





        

<hr>



### function in\_degree\_map 

_Returns a mapped array of in-degrees for all vertices._ 
```cpp
inline std::vector< size_type > hgl::hypergraph::in_degree_map () const
```





**Returns:**

A vector where the index aligns with the vertex ID containing its in-degree. 





        

<hr>



### function incident\_vertices [1/2]

_Retrieves all vertices incident with a hyperedge ($e$)._ 
```cpp
inline auto hgl::hypergraph::incident_vertices (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The hyperedge ID. 



**Returns:**

A view representing the set of incident vertices. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function incident\_vertices [2/2]

_Retrieves all vertices incident with a hyperedge ($e$)._ 
```cpp
inline auto hgl::hypergraph::incident_vertices (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hyperedge descriptor. 



**Returns:**

A view representing the set of incident vertices. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function incident\_vertex\_ids [1/2]

_Retrieves IDs of all vertices incident with a hyperedge ($e$)._ 
```cpp
inline auto hgl::hypergraph::incident_vertex_ids (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The hyperedge ID. 



**Returns:**

A view representing the set of incident vertex IDs. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function incident\_vertex\_ids [2/2]

_Retrieves IDs of all vertices incident with a hyperedge ($e$)._ 
```cpp
inline auto hgl::hypergraph::incident_vertex_ids (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hyperedge descriptor. 



**Returns:**

A view representing the set of incident vertex IDs. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function hyperedge\_size [1/2]

_Retrieves the size (number of incident vertices) of the given hyperedge._ 
```cpp
inline size_type hgl::hypergraph::hyperedge_size (
    const  id_type hyperedge_id
) const
```




* For undirected hypergraphs this is equivalent to $\vert e \vert$.
* For BF-directed hypergraphs this is equivalent to $\vert T(e) \vert + \vert [**H(e)**](group__HGL-Core.md#variable-invalid_id_v) \vert$






**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The size of the hyperedge. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function hyperedge\_size [2/2]

_Retrieves the size (number of incident vertices) of the given hyperedge._ 
```cpp
inline size_type hgl::hypergraph::hyperedge_size (
    hyperedge_type hyperedge
) const
```




* For undirected hypergraphs this is equivalent to $\vert e \vert$.
* For BF-directed hypergraphs this is equivalent to $\vert T(e) \vert + \vert [**H(e)**](group__HGL-Core.md#variable-invalid_id_v) \vert$






**Parameters:**


* `hyperedge` The descriptor of the hyperedge. 



**Returns:**

The size of the hyperedge. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function hyperedge\_size\_map 

_Returns a mapped array of hyperedge sizes._ 
```cpp
inline std::vector< size_type > hgl::hypergraph::hyperedge_size_map () const
```





**Returns:**

A vector where the index aligns with the hyperedge ID containing its size. 





        

<hr>



### function tail [1/2]

_Retrieves all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::tail (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The hypepredge ID. 



**Returns:**

A view representing the set of the hyperedge's tail vertices. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function tail [2/2]

_Retrieves all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::tail (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hyperedge descriptor. 



**Returns:**

A view representing the set of the hyperedge's tail vertices. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function tail\_ids [1/2]

_Retrieves IDs all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::tail_ids (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The hypepredge ID. 



**Returns:**

A view representing the set of the hyperedge's tail vertex IDs. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function tail\_ids [2/2]

_Retrieves IDs all vertices in the_ _tail_ _set of a hyperedge (_[_**$T(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::tail_ids (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hypepredge descriptor. 



**Returns:**

A view representing the set of the hyperedge's tail vertex IDs. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function tail\_size [1/2]

_Retrieves the size of the_ _BF-directed_ _hyperedge's__tail_ _set ($\vert T(e) \vert)._
```cpp
inline size_type hgl::hypergraph::tail_size (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The size of the hyperedge's _tail_ set. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function tail\_size [2/2]

_Retrieves the size of the_ _BF-directed_ _hyperedge's__tail_ _set ($\vert T(e) \vert)._
```cpp
inline size_type hgl::hypergraph::tail_size (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The descriptor of the hyperedge. 



**Returns:**

The size of the hyperedge's _tail_ set. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function tail\_size\_map 

_Returns a mapped array of sizes of the_ _tail_ _sets of hyperedged in a__BF-directed_ _hypergraph._
```cpp
inline std::vector< size_type > hgl::hypergraph::tail_size_map () const
```





**Returns:**

A vector where the index aligns with the hyperedge ID containing its _tail_ set size. 





        

<hr>



### function head [1/2]

_Retrieves all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::head (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The hypepredge ID. 



**Returns:**

A view representing the set of the hyperedge's head vertices. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function head [2/2]

_Retrieves all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::head (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hyperedge descriptor. 



**Returns:**

A view representing the set of the hyperedge's head vertices. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function head\_ids [1/2]

_Retrieves IDs all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::head_ids (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The hypepredge ID. 



**Returns:**

A view representing the set of the hyperedge's head vertex IDs. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function head\_ids [2/2]

_Retrieves IDs all vertices in the_ _head_ _set of a hyperedge (_[_**$H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _$)._
```cpp
inline auto hgl::hypergraph::head_ids (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hypepredge descriptor. 



**Returns:**

A view representing the set of the hyperedge's head vertex IDs. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function head\_size [1/2]

_Retrieves the size of the_ _BF-directed_ _hyperedge's__head_ _set ($\vert_[_**H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _\vert)._
```cpp
inline size_type hgl::hypergraph::head_size (
    const  id_type hyperedge_id
) const
```





**Parameters:**


* `hyperedge_id` The ID of the hyperedge. 



**Returns:**

The size of the hyperedge's _head_ set. 




**Exception:**


* `std::invalid_argument` If the hyperedge ID is invalid. 




        

<hr>



### function head\_size [2/2]

_Retrieves the size of the_ _BF-directed_ _hyperedge's__head_ _set ($\vert_[_**H(e)**_](group__HGL-Core.md#variable-invalid_id_v) _\vert)._
```cpp
inline size_type hgl::hypergraph::head_size (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The descriptor of the hyperedge. 



**Returns:**

The size of the hyperedge's _head_ set. 




**Exception:**


* `std::invalid_argument` If the hyperedge descriptor is invalid. 




        

<hr>



### function head\_size\_map 

_Returns a mapped array of sizes of the_ _head_ _sets of hyperedged in a__BF-directed_ _hypergraph._
```cpp
inline std::vector< size_type > hgl::hypergraph::head_size_map () const
```





**Returns:**

A vector where the index aligns with the hyperedge ID containing its _head_ set size. 





        

<hr>



### function display 

_Returns a formatter object that safely encapsulates a hyperedge for stream output._ 
```cpp
inline hyperedge_formatter hgl::hypergraph::display (
    hyperedge_type hyperedge
) const
```





**Parameters:**


* `hyperedge` The hyperedge to format. 



**Returns:**

A [**hyperedge\_formatter**](structhgl_1_1hypergraph_1_1hyperedge__formatter.md) structure prepared for standard stream insertion. 





        

<hr>## Friends Documentation





### friend operator== 

_Compares two hypergraphs for strict structural and property equality._ 
```cpp
inline bool hgl::hypergraph::operator== (
    const  hypergraph & lhs,
    const  hypergraph & rhs
) noexcept
```





**Parameters:**


* `lhs` The left operand. 
* `rhs` The right operand. 



**Returns:**

`true` if both graphs represent the exact same topology and properties, `false` otherwise. 





        

<hr>



### friend operator&lt;&lt; 

_Formats and outputs the entire hypergraph structure to a standard output stream._ 
```cpp
inline std::ostream & hgl::hypergraph::operator<< (
    std::ostream & os,
    const  hypergraph & hg
) 
```



The generated string representation of the hypergraph depends on the currently active formatting options of the stream.




**Parameters:**


* `os` The target output stream. 
* `hg` The hypergraph instance to write. 



**Returns:**

The stream reference for chaining. 





        

<hr>



### friend operator&gt;&gt; 

_Deserializes hypergraph structure data from an input stream (using the HGSF format)._ 
```cpp
inline std::istream & hgl::hypergraph::operator>> (
    std::istream & is,
    hypergraph & hg
) 
```





**Parameters:**


* `is` The source input stream. 
* `g` The hypergraph instance to populate. 



**Returns:**

The stream reference for chaining. 





        

<hr>



### friend clone 

_Creates a deep copy of a given hypergraph._ 
```cpp
template<traits::c_hypergraph Hypergraph>
Hypergraph hgl::hypergraph::clone (
    const  Hypergraph & source
) 
```



Creates a deep copy of the given hypergraph.




**Template parameters:**


* `Hypergraph` The concrete hypergraph type. 



**Parameters:**


* `source` The hypergraph to clone. 



**Returns:**

A duplicated instance of the source hypergraph.




**Template parameters:**


* `Hypergraph` The type of the hypergraph. 



**Parameters:**


* `source` The hypergraph instance to clone. 



**Returns:**

A newly constructed hypergraph containing identical vertices, hyperedges and properties (if applicable). 





        

<hr>



### friend to 

_Converts a hypergraph to a different representation type._ 
```cpp
template<traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph Hypergraph>
auto hgl::hypergraph::to (
    Hypergraph && source
) 
```



Converts a hypergraph to a different representation type (e.g., from incidence list to incidence matrix).


This function efficiently transforms a hypergraph's underlying memory representation (e.g., from a standard incidence list to a flattened incidence list) while preserving its exact topology, properties, and identifiers.


#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|TargetReprTag|The representation tag of the desired target representation (e.g., `hgl::repr::flat_list_t`).|[**c\_hypergraph\_repr\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag)|
|Hypergraph|The type of the source hypergraph, which will be automatically deduced from the function argument.|[**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph)|








**Parameters:**


* `source` The hypergraph to convert. After the operation it will be left in a valid, empty state. 



**Returns:**

A new hypergraph containing the moved data, structured according to `TargetReprTag`.






#### See Also




* [**swap\_repr\_tag**](structhgl_1_1traits_1_1swap__repr__tag.md) : For the trait used to resolve the target hypergraph type with the swapped representation tag.






**Template parameters:**


* `TargetReprTag` The representation tag defining the target storage mechanism. 
* `Hypergraph` The concrete hypergraph type of the source. 



**Parameters:**


* `source` The hypergraph to convert. 



**Returns:**

A new hypergraph matching the target representation type with identical topology and properties. 






        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/hypergraph.hpp`

