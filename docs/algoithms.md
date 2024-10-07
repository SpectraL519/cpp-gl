# Algorithms

The `CPP-GL` library provides a set of customizable graph algorithms, which are defined in the [gl/algorithms.hpp](/include/gl/algorithms.hpp) header file or in the specific header files in the [gl/algorithm/](/include/gl/algorithm/) directory.

<br />

## Table of content

- [Algorithm-specific types and concepts](#algorithm-specific-types-and-concepts)
- [The defined algorithms](#the-defined-algorithms)
  - [Depth-first search](#depth-first-search)
  - [Breadth-first search](#breadth-first-search)
- [Example](#example)
- [Writing custom algorithms](#writing-custom-algorithms)
- [Related pages](#related-pages)

<br />
<br />

## Algorithm-specific types and concepts

This section covers the specific types and type traits used for the algorithm implementation. These are designed to make the available algorithms highly customizable and easy to work with.

### Types

> [!NOTE]
> All types listed below are defined in the `gl::types` namespace

- `no_return` - A placeholder type to represent algorithms that do not return a value. It is primarily used in type traits to conditionally handle return types.
- `empty_callback` - Represents an empty callback, used as a default value where no callback functionality is needed.

- `vertex_callback`
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the graph on which the callback will operate.
    - `ReturnType` - the return type of the callback function.
    - `Args...` - variadic template representing additional arguments passed to the callback.
  - *Description*: A type alias for a function that operates on vertices of a graph, accepting a vertex of type `const typename GraphType::vertex_type&` and additional arguments.

- `edge_callback`
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the graph on which the callback will operate.
    - `ReturnType` - the return type of the callback function.
    - `Args...` - variadic template representing additional arguments passed to the callback.
  - *Description*: A type alias for a function that operates on edges of a graph, accepting a vertex of type `const typename GraphType::edge_type&` and additional arguments.

- `vertex_info`
  - *Description*: Holds information about a vertex. If `id == source_id`, the `id` represents the starting vertex.
  - *Constructors*:
    - `vertex_info(types::id_type id)` - initializes the object with the same value for `id` and `source_id` representing a starting vertex
    - `vertex_info(types::id_type id, types::id_type source_id)`
  - *Members*:
    - `id: types::id_type` - the ID of the vertex.
    - `source_id: types::id_type` - the ID of the source vertex, typically used during algorithms.

- `edge_info`
  - *Template parameters*:
    - `EdgeType: type_traits::c_instantiation_of<edge_descriptor>` - the type of the edge.
  - *Description*: Holds information about an edge, including the edge itself and its source vertex's ID.
- *Parameters*:
  - *Constructors*:
    - `vertex_info(types::id_type id)` - initializes the object with the same value for `id` and `source_id` representing a starting vertex
    - `vertex_info(types::id_type id, types::id_type source_id)`
  - *Members*:
    - `edge: types::const_ref_wrap<EdgeType>` - a constant reference wrapper for the edge.
    - `source_id: types::id_type` - the ID of the source vertex of the held edge.

### Concepts

> [!NOTE]
> All concepts listed below are defined in the `gl::type_traits` namespace

- `c_alg_no_return_type`
  - *Template parameters*:
    - `T` - the type to check.
  - *Description*: Checks if the type `T` is `types::no_return`. Used to identify algorithms that do not return a value.
  - *Equivalent to*: `std::same_as<T, types::no_return>`

- `c_alg_return_graph_type`
  - *Template parameters*:
    - `T` - the type to check.
  - *Description*: Checks if the type `T` is a valid return type for graph algorithms, i.e., either a graph type or `no_return`.
  - *Equivalent to*: `c_graph<T> or c_alg_no_return_type<T>`

- `c_empty_callback`
  - *Template parameters*:
    - `F` - the callback type to check.
  - *Description*: Checks if a callback type is `types::empty_callback`. Used to determine when no callback is needed for an algorithm.
  - *Equivalent to*: `std::same_as<F, types::empty_callback>`

- `c_vertex_callback`
  - *Template parameters*:
    - `F` - the function type to check.
    - `GraphType` - the graph type associated with the callback.
    - `ReturnType` - the return type of the callback.
    - `Args...` - variadic template representing additional arguments passed to the callback.
  - *Description*: Checks if a function is a valid vertex callback.
  - *Equivalent to*:

    ```cpp
    c_empty_callback<F> or std::convertible_to<F, types::vertex_callback<GraphType, ReturnType, Args...>>
    ```

- `c_edge_callback`
  - *Template parameters*:
    - `F` - the function type to check.
    - `GraphType` - the graph type associated with the callback.
    - `ReturnType` - the return type of the callback.
    - `Args...` - variadic template representing additional arguments passed to the callback.
  - *Description*: Checks if a function is a valid edge callback.
  - *Equivalent to*:

    ```cpp
    c_empty_callback<F> or std::convertible_to<F, types::edge_callback<GraphType, ReturnType, Args...>>
    ```

<br />
<br />

## The defined algorithms

> [!NOTE]
> All topology generator functions are defined in the `gl::algorithm` namespace.

### Depth-first search

- `depth_first_search(graph, root_vertex_id_opt, pre_visit, post_visit)`
  - *Template parameters*:
    - `SearchTreeType: type_traits::c_alg_return_graph_type` (default = `types::no_return`) - The type of the search tree to return, or `types::no_return`.
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `types::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `types::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Description*: Performs an iterative depth-first search (DFS) on the specified graph.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `root_vertex_id_opt: const std::optional<types::id_type>&` (default = `no_root_vertex`) - The optional ID of the root vertex to start the search from. If not provided, the search will start from all vertices.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `detail::alg_return_graph_type<SearchTreeType>` - If `SearchTreeType` is not `types::no_return`, returns a search tree of type `SearchTreeType`. Otherwise, the return type is `void`.

  - *Defined in*: [gl/algorithm/depth_first_search.hpp](/include/gl/algorithm/deapth_first_search.hpp)

- `recursive_depth_first_search(graph, root_vertex_id_opt, pre_visit, post_visit)`
  - *Description*: Performs a recursive depth-first search (DFS) on the specified graph.

    **NOTE:** This algoithm has the same template parameters, parameters and return type as the iterative version (`depth_first_search`)

  - *Defined in*: [gl/algorithm/depth_first_search.hpp](/include/gl/algorithm/deapth_first_search.hpp)

### Breadth-first search

- `breadth_first_search(graph, root_vertex_id_opt, pre_visit, post_visit)`
  - *Template parameters*:
    - `SearchTreeType: type_traits::c_alg_return_graph_type` (default = `types::no_return`) - The type of the search tree to return, or `types::no_return`.
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `types::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `types::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Description*: Performs an breadth-first search (BFS) on the specified graph.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `root_vertex_id_opt: const std::optional<types::id_type>&` (default = `no_root_vertex`) - The optional ID of the root vertex to start the search from. If not provided, the search will start from all vertices.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `detail::alg_return_graph_type<SearchTreeType>` - If `SearchTreeType` is not `types::no_return`, returns a search tree of type `SearchTreeType`. Otherwise, the return type is `void`.

  - *Defined in*: [gl/algorithm/depth_first_search.hpp](/include/gl/algorithm/deapth_first_search.hpp)

### Graph coloring

> [!CAUTION]
> TODO: `stash@{0}: On yt-cppgl-47-docs: coloring return optimization`

---

> [!NOTE]
> The `bicoloring_type` used by the binary coloring algorithms is defined as `std::vector<types::binary_color>`.

- `bipartite_coloring(graph, pre_visit, post_visit)`
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `types::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `types::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Description*: Performs a BFS-based graph bipartite coloring on the specified graph.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform the coloring on.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `std::optional<bicoloring_type>` - The binary coloring of the graph:
      - `std::nullopt` if the given graph is not bipartite
      - a list of `types::binary_color` values where the color at index $i$ corresponds to the vertex $v$ such that $v_{id} = i$

  - *Defined in*: [gl/algorithm/coloring.hpp](/include/gl/algorithm/coloring.hpp)

- `is_bipartite(graph)`
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the check is performed.
  - *Description*: Checks whether a graph is bipartite by performin a BFS-based binary coloring on it.
  - *Parameters*:
    - `graph: const GraphType&` - the graph to check.
  - *Return type*: `bool`
  - *Equivalent to*: `bipartite_coloring(graph).has_value()`
  - *Defined in*: [gl/algorithm/coloring.hpp](/include/gl/algorithm/coloring.hpp)

- `apply_coloring(graph, color_range)`
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the operation is performed.
    - `ColorRange: c_sized_range_of<types::binary_color>` - The color range type.
  - *Constraints*:
    - `type_traits::c_binary_color_properties_type<typename GraphType::vertex_properties_type>` - vertices must have a binary color property
  - *Description*: Applies the given coloring to the graph by setting the color poperty of $v$ to `color_range(v.id())`. If the coloring cannot be applied to the given graph due to a size mismatch, `false` is returned. Otherwise `true` will be returned.
  - *Parameters*:
    - `graph: GraphType&` - the graph to apply the coloring to.
    - `color_range: const ColorRange&` - the range of binary color values to apply to the graph
  - *Return type*: `bool`
  - *Defined in*: [gl/algorithm/coloring.hpp](/include/gl/algorithm/coloring.hpp)

<br />
<br />

## Example

The program below demostrates the graphs returned by the topology generator functions for a *directed* graph type:

```cpp

```

The output of this program is:

```txt

```

<br />
<br />

## Writing custom algorithms

<br />
<br />

## Related pages

- [The graph class - library's core](/docs/graph.md)
- [Graph topology generators](/docs/topologies.md)
- [I/O operations](/docs/io.md)
<!-- TODO: types page -->
