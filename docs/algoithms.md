# Algorithms

The `CPP-GL` library provides a set of customizable graph algorithms, which are defined in the [gl/algorithms.hpp](/include/gl/algorithms.hpp) header file or in the specific header files in the [gl/algorithm/](/include/gl/algorithm/) directory.

<br />

## Table of content

- [Algorithm-specific types and concepts](#algorithm-specific-types-and-concepts)
  - [Types](#types)
  - [Concepts](#concepts)
- [The defined algorithms](#the-defined-algorithms)
  - [Depth-first search](#depth-first-search)
  - [Breadth-first search](#breadth-first-search)
  - [Graph coloring](#graph-coloring)
  - [Dijkstra shortest paths](#dijkstra-shortest-paths)
  - [Topological sorting](#topological-sorting)
  - [MST finding](#mst-finding)
- [Writing custom algorithms](#writing-custom-algorithms)
- [Related pages](#related-pages)

<br />
<br />

## Algorithm-specific types and concepts

This section covers the specific types and type traits used for the algorithm implementation. These are designed to make the available algorithms highly customizable and easy to work with.

### Types

> [!NOTE]
> All types listed below are defined in the `gl::algorithm` namespace

- `no_return` - A placeholder type to represent algorithms that do not return a value. It is primarily used in type traits to conditionally handle return types.
- `empty_callback` - Represents an empty callback, used as a default value where no callback functionality is needed.

- `vertex_callback`
  - *Description*: A type alias for a function that operates on vertices of a graph, accepting a vertex of type `const typename GraphType::vertex_type&` and additional arguments.
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the graph on which the callback will operate.
    - `ReturnType` - the return type of the callback function.
    - `Args...` - variadic template representing additional arguments passed to the callback.

- `edge_callback`
  - *Description*: A type alias for a function that operates on edges of a graph, accepting a vertex of type `const typename GraphType::edge_type&` and additional arguments.
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - the type of the graph on which the callback will operate.
    - `ReturnType` - the return type of the callback function.
    - `Args...` - variadic template representing additional arguments passed to the callback.

- `vertex_info`
  - *Description*: Holds information about a vertex. If `id == source_id`, the `id` represents the starting vertex.
  - *Constructors*:
    - `vertex_info(types::id_type id)` - initializes the object with the same value for `id` and `source_id` representing a starting vertex
    - `vertex_info(types::id_type id, types::id_type source_id)`
  - *Member variables*:
    - `id: types::id_type` - the ID of the vertex.
    - `source_id: types::id_type` - the ID of the source vertex, typically used during algorithms.

- `edge_info`
  - *Description*: Holds information about an edge, including the edge itself and its source vertex's ID.
  - *Template parameters*:
    - `EdgeType: type_traits::c_instantiation_of<edge_descriptor>` - the type of the edge.
  - *Constructors*:
    - `vertex_info(types::id_type id)` - initializes the object with the same value for `id` and `source_id` representing a starting vertex
    - `vertex_info(types::id_type id, types::id_type source_id)`
  - *Member variables*:
    - `edge: types::const_ref_wrap<EdgeType>` - a constant reference wrapper for the edge.
    - `source_id: types::id_type` - the ID of the source vertex of the held edge.

- `predecessors_descriptor`
  - *Description*: A structure that holds a collection of predecessors for a set of vertices.
  - *Type definitions*:
    - `predecessor_type: std::optional<types::id_type>` - the type of the predecessor, which can either be a valid vertex ID or empty (no predecessor).

    **NOTE:** An empty predecessor means that a vertex is nor reachable from the source vertex, while `predecessor[ID] == ID` means that the vertex with such ID is the source vertex.

  - *Constructors*:
    - `predecessors_descriptor(types::size_type n_vertices)` - initializes the object with a vector of optional predecessors, sized to `n_vertices`.
  - *Destructor*:
    - `virtual ~predecessors_descriptor()` - default destructor.
  - *Member variables*:
    - `predecessors: std::vector<std::optional<types::id_type>>` - a vector of optional IDs, where each element represents a predecessor for a vertex. If a vertex has no predecessor, the corresponding element is empty.
  - *Member functions*:
    - `is_reachable(types::id_type vertex_id) const -> bool` - checks if a vertex is reachable by verifying if its predecessor exists (i.e., if the optional value is set).
    - `operator[](types::size_type i) const` - returns a constant reference to the predecessor at index `i`.
    - `operator[](types::size_type i)` - returns a reference to the predecessor at index `i`.
    - `at(types::size_type i) const` - returns a constant reference to the predecessor at index `i`, with bounds checking.
    - `at(types::size_type i)` - returns a reference to the predecessor at index `i`, with bounds checking.

### Concepts

> [!NOTE]
> All concepts listed below are defined in the `gl::type_traits` namespace

- `c_alg_no_return_type`
  - *Description*: Checks if the type `T` is `algorithm::no_return`. Used to identify algorithms that do not return a value.
  - *Template parameters*:
    - `T` - the type to check.
  - *Equivalent to*: `std::same_as<T, algorithm::no_return>`

- `c_alg_return_graph_type`
  - *Description*: Checks if the type `T` is a valid return type for graph algorithms, i.e., either a graph type or `no_return`.
  - *Template parameters*:
    - `T` - the type to check.
  - *Equivalent to*: `c_graph<T> or c_alg_no_return_type<T>`

- `c_empty_callback`
  - *Description*: Checks if a callback type is `algorithm::empty_callback`. Used to determine when no callback is needed for an algorithm.
  - *Template parameters*:
    - `F` - the callback type to check.
  - *Equivalent to*: `std::same_as<F, algorithm::empty_callback>`

- `c_optional_callback`
  - *Description*: Checks if the given type is a valid callback or `algorithm::empty_callback`.
  - *Template parameters*:
    - `F` - the callback type to check.
    - `ReturnType` - the expected return type of the callback type.
    - `Args...` - variadic template representing arguments of the callback type.
  - *Equivalent to*: `c_empty_callback<F> or std::is_invocable_r_v<ReturnType, F, Args...>`

- `c_vertex_callback`
  - *Description*: Checks if a function is a valid vertex callback.
  - *Template parameters*:
    - `F` - the function type to check.
    - `GraphType` - the graph type associated with the callback.
    - `ReturnType` - the return type of the callback.
    - `Args...` - variadic template representing additional arguments of the callback type.
  - *Equivalent to*: `std::is_invocable_r_v<ReturnType, F, const typename GraphType::vertex_type&, Args...>`

- `c_edge_callback`
  - *Description*: Checks if a function is a valid edge callback.
  - *Template parameters*:
    - `F` - the function type to check.
    - `GraphType` - the graph type associated with the callback.
    - `ReturnType` - the return type of the callback.
    - `Args...` - variadic template representing additional arguments of the callback type.
  - *Equivalent to*: `std::is_invocable_r_v<ReturnType, F, const typename GraphType::edge_type&, Args...>`

- `c_optional_vertex_callback`
  - *Description*: Checks if a function is a valid (optional) vertex callback.
  - *Template parameters*:
    - `F` - the function type to check.
    - `GraphType` - the graph type associated with the callback.
    - `ReturnType` - the return type of the callback.
    - `Args...` - variadic template representing additional arguments of the callback type.
  - *Equivalent to*: `c_optional_callback<F, ReturnType, const typename GraphType::vertex_type&, Args...>`

- `c_optional_edge_callback`
  - *Description*: Checks if a function is a valid (optional) edge callback.
  - *Template parameters*:
    - `F` - the function type to check.
    - `GraphType` - the graph type associated with the callback.
    - `ReturnType` - the return type of the callback.
    - `Args...` - variadic template representing additional arguments of the callback type.
  - *Equivalent to*: `c_optional_callback<F, ReturnType, const typename GraphType::edge_type&, Args...>`

<br />
<br />

## The defined algorithms

> [!NOTE]
> All topology generator functions are defined in the `gl::algorithm` namespace.

### Depth-first search

- `depth_first_search(graph, root_vertex_id_opt, pre_visit, post_visit)`
  - *Description*: Performs an iterative depth-first search (DFS) on the specified graph and conditionally returns a `predecessors_descriptor` instance.

  - *Template parameters*:
    - `AlgReturnType: type_traits::c_alg_return_type` (default = `algorithm::default_return`) - Specifies whether the algorrithm should return the predecessors descriptor or not (can be eigher `algorithm::default_return` or `algorithm::no_return`).
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `root_vertex_id_opt: const std::optional<types::id_type>&` (default = `no_root_vertex`) - The optional ID of the root vertex to start the search from. If not provided, the search will start from all vertices.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `impl::alg_return_type<AlgReturnType, predecessors_descriptor>` - If `AlgReturnType` is `algorithm::no_return` - nothing will be returned (`void`). Otherwise the algorithm will return an instance of `predecessors_descriptor`.

  - *Defined in*: [gl/algorithm/depth_first_search.hpp](/include/gl/algorithm/deapth_first_search.hpp)

- `recursive_depth_first_search(graph, root_vertex_id_opt, pre_visit, post_visit)`
  - *Description*: Performs a recursive depth-first search (DFS) on the specified graph and conditionally returns a `predecessors_descriptor` instance.

    **NOTE:** This algoithm has the same template parameters, parameters and return type as the iterative version (`depth_first_search`)

  - *Defined in*: [gl/algorithm/depth_first_search.hpp](/include/gl/algorithm/deapth_first_search.hpp)

### Breadth-first search

- `breadth_first_search(graph, root_vertex_id_opt, pre_visit, post_visit)`
  - *Description*: Performs an breadth-first search (BFS) on the specified graph and conditionally returns a `predecessors_descriptor` instance.

  - *Template parameters*:
    - `AlgReturnType: type_traits::c_alg_return_type` (default = `algorithm::default_return`) - Specifies whether the algorrithm should return the predecessors descriptor or not (can be eigher `algorithm::default_return` or `algorithm::no_return`).
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform BFS on.
    - `root_vertex_id_opt: const std::optional<types::id_type>&` (default = `no_root_vertex`) - The optional ID of the root vertex to start the search from. If not provided, the search will start from all vertices.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `impl::alg_return_type<AlgReturnType, predecessors_descriptor>` - If `AlgReturnType` is `algorithm::no_return` - nothing will be returned (`void`). Otherwise the algorithm will return an instance of `predecessors_descriptor`.

  - *Defined in*: [gl/algorithm/breadth_first_search.hpp](/include/gl/algorithm/breadth_first_search.hpp)

### Graph coloring

> [!NOTE]
> The `bicoloring_type` used by the binary coloring algorithms is defined as `std::vector<types::binary_color>`.

- `bipartite_coloring(graph, pre_visit, post_visit)`
  - *Description*: Performs a BFS-based graph bipartite coloring on the specified graph.

  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform the coloring on.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `std::optional<bicoloring_type>` - The binary coloring of the graph: `std::nullopt` if the given graph is *not bipartite* or a list of `types::binary_color` values where the color at index $i$ corresponds to the vertex $v$ such that $v_{id} = i$

  - *Defined in*: [gl/algorithm/coloring.hpp](/include/gl/algorithm/coloring.hpp)

- `is_bipartite(graph)`
  - *Description*: Checks whether a graph is bipartite by performin a BFS-based binary coloring on it.
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the check is performed.
  - *Parameters*:
    - `graph: const GraphType&` - the graph to check.
  - *Return type*: `bool`
  - *Equivalent to*: `bipartite_coloring(graph).has_value()`
  - *Defined in*: [gl/algorithm/coloring.hpp](/include/gl/algorithm/coloring.hpp)

- `apply_coloring(graph, color_range)`
  - *Description*: Applies the given coloring to the graph by setting the color poperty of $v$ to `color_range(v.id())`. If the coloring cannot be applied to the given graph due to a size mismatch, `false` is returned. Otherwise `true` will be returned.
  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the operation is performed.
    - `ColorRange: c_sized_range_of<types::binary_color>` - The color range type.
  - *Constraints*:
    - `type_traits::c_binary_color_properties_type<typename GraphType::vertex_properties_type>` - vertices must have a binary color property
  - *Parameters*:
    - `graph: GraphType&` - the graph to apply the coloring to.
    - `color_range: const ColorRange&` - the range of binary color values to apply to the graph
  - *Return type*: `bool`
  - *Defined in*: [gl/algorithm/coloring.hpp](/include/gl/algorithm/coloring.hpp)

### Dijkstra shortest paths

- `dijkstra_shortest_paths(graph, source_id, pre_visit, post_visit)`
  - *Description*: Performs the [Dijkstra's shortest path finding algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm) on the specified graph.

  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform the search on.
    - `source_id: types::id_type` - The ID of the source vertex to start the search from.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `algorithm::paths_descriptor<types::vertex_distance_type<GraphType>>` - A shortest paths descriptor structure (detailed information can be found in the note below)

  - *Throws*: `std::invalid_argument` if an edge with a negative weight is found during the graph search.

  - *Defined in*: [gl/algorithm/dijkstra.hpp](/include/gl/algorithm/dijkstra.hpp)

> [!NOTE]
> The `algorithm::paths_descriptor` structure is defined as follows:
>
> - *Template parameters*:
>   - `VertexDistanceType: type_traits::c_basic_arithmetic` - The type of the distance between vertices in the graph.
> - *Type definitions*:
>   - `distance_type` - An alias for `VertexDistanceType`.
> - *Constructors*:
>   - `paths_descriptor(types::size_type n_vertices)` - Initializes both `predecessors` and `distances` lists with default values and of size `n_vertices`
> - *Member variables*:
>   - `predecessors: std::vector<std::optional<types::id_type>>` - A list of vertex predecessors in the graph's search.
>     **NOTE:** If a predecessor at index $i$ is null, then the vertex $v$ in the graph such that $v_{id} = i$ is unreachable.
>   - `distances: std::vector<distance_type>` - A list of vertex distances from a source vertex.
> - *Member functions*:
>   - `is_reachable(types::id_type vertex_id) const -> bool` - checks if a vertex is reachable by verifying if its predecessor exists (i.e., if the optional value is set).
>   - `operator[](types::size_type i) const` - returns a pair of constant references to the predecessor and distance at index `i`.
>   - `operator[](types::size_type i)` - returns a pair of references to the predecessor and distance at index `i`.
>   - `at(types::size_type i) const` - returns a pair of constant references to the predecessor and distance at index `i`, with bounds checking.
>   - `at(types::size_type i)` - returns a pair of references to the predecessor and distance at index `i`, with bounds checking.

- `reconstruct_path(predecessor_map, vetex_id)`
  - *Description*: Reconstructs a path to the search vertex $v$ such that $v_{id} = \text{vertex-id}$ from the given predecessor map
  - *Template parameters*:
    - `IdRange: type_traits::c_random_access_range_of<std::optional<types::id_type>>` - The predecessor map type.
  - *Parameters*:
    - `predecessor_map: const IdRange&` - The predecessor map.
    - `vertex_id: const types::size_type` - The ID of the vertex the path to which will be reconstructed.
  - *Return type*: `std::deque<types::id_type>`
  - *Throws:* `std::invalid_argument` if the vertex with the given ID is unreachable.

### Topological sorting

- `topological_sort(graph, pre_visit, post_visit)`
  - *Description*: Returns the [topological ordering](https://en.wikipedia.org/wiki/Topological_sorting) of the graph's vertices.

  - *Template parameters*:
    - `GraphType: type_traits::c_directed_graph` - The type of the graph on which the sorting is performed (must be directed).
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform the coloring on.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `std::optional<std::vector<types::id_type>>` - The [topological ordering](https://en.wikipedia.org/wiki/Topological_sorting) of the graph's vertices: `std::nullopt` if the given graph is *not acyclic* or a list of vertex IDs representing the proper ordering of the vertices.

  - *Defined in*: [gl/algorithm/topological_sort.hpp](/include/gl/algorithm/topological_sort.hpp)

### MST finding

- `edge_heap_prim_mst(graph, pre_visit, post_visit)`
  - *Description*:
    - Returns an `mst_descriptor` object containing the [Minimum Spanning Tree](https://en.wikipedia.org/wiki/Minimum_spanning_tree) edges and its total weight.
    - Performs the Prim's algorithm using a minimum binary heap of edges.

  - *Template parameters*:
    - `GraphType: type_traits::c_undirected_graph` - The type of the graph on which the search is performed (must be undirected).
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform the search on.
    - `root_id_opt: const std::optional<types::id_type>` - An optional root vertex ID. If set, the search will begin with edges adjacent to the vertex $v$ such that $v_{id} = \text{root-id-opt.value()}$. Otherwise the first vertex of the graph will be used as root.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `algorithm::mst_descriptor<GraphType>` - An [MST](https://en.wikipedia.org/wiki/Minimum_spanning_tree) desciptor object (detailed information can be found in the note below).

  - *Defined in*: [gl/algorithm/mst.hpp](/include/gl/algorithm/mst.hpp)

- `vertex_heap_prim_mst(graph, pre_visit, post_visit)`
  - *Description*:
    - Returns an `mst_descriptor` object containing the [Minimum Spanning Tree](https://en.wikipedia.org/wiki/Minimum_spanning_tree) edges and its total weight.
    - Performs the Prim's algorithm using a minimum binary heap of vertices.
    > **NOTE:** The binary heap implementation in C++ doesn't allow modifying a single node within the heap, therefore the `std::make_heap` - $O(n)$ - function has to be called to rebuild the heap.

  - *Template parameters*:
    - `GraphType: type_traits::c_undirected_graph` - The type of the graph on which the search is performed (must be undirected).
    - `PreVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_optional_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform the search on.
    - `root_id_opt: const std::optional<types::id_type>` - An optional root vertex ID. If set, the search will begin with edges adjacent to the vertex $v$ such that $v_{id} = \text{root-id-opt.value()}$. Otherwise the first vertex of the graph will be used as root.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*:
    - `algorithm::mst_descriptor<GraphType>` - An [MST](https://en.wikipedia.org/wiki/Minimum_spanning_tree) desciptor object (detailed information can be found in the note below).

  - *Defined in*: [gl/algorithm/mst.hpp](/include/gl/algorithm/mst.hpp)

> [!NOTE]
> The `algorithm::mst_descriptor` structure is defined as follows:
>
> - *Template parameters*:
>   - `GraphType: type_traits::c_undirected_graph` - The graph type for which the [MST](https://en.wikipedia.org/wiki/Minimum_spanning_tree) is determined.
> - *Type definitions*:
>   - `graph_type` - An alias for `GraphType`.
>   - `edge_typpe` - An alias for `typename graph_type::edge_type`.
>   - `weight_type` - An alias for `types::vertex_distance_type<graph_type>`.
> - *Constructors*:
>   - `mst_descriptor(types::size_type n_vertices)` - Initializes an empty `edges` list with the capacity of $\text{n-vertices} - 1$ and the total weight is set to $0$.
> - *Member variables*:
>   - `edges: std::vector<types::const_ref_wrap<edge_type>>` - A list of constant edge references representing the edges of the spanning tree.
>   - `weight: weight_type` - The total weight of all edges of the spanning tree.

<br />
<br />

## Writing custom algorithms

To write custom graph algorithms you can use the types and concepts described in the [Algorithm-specific types and concepts](#algorithm-specific-types-and-concepts) as well as the general graph utility described in the [graph class documentation page](/docs/graph.md#additional-utility).

Additionaly you can use the depth-first/breadth-first search algorithm templates which are defined in the `gl::algorithm::impl` namespace:

### Depth-first search templates

> [!NOTE]
> The DFS algorithm templates are defined in the [gl/algorithm/impl/dfs.hpp](/include/gl/algorithm/impl/dfs.hpp) file.

- `dfs(graph, root_vertex, visit_vertex_pred, visit, enque_vertex_pred, pre_visit, post_visit)`
  - *Desciption*: An iterative DFS algoithm template.

  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `VisitVertexPredicate: type_traits::c_optional_vertex_callback<GraphType, bool>` - The vertex visiting unary predicate type.
    - `VisitCallback: type_traits::c_vertex_callback<GraphType, bool, types::id_type>` - The vertex visting callback type (arguments: `vertex, source_id`).
    - `EnqueueVertexPred: type_traits::c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>` - The vertex enqueue predicate type (arguments: `vertex, in_edge`)
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `root_vertex: const typename GraphType::vertex_type&` - The vertex from which the search will be started.
    - `visit_vertex_pred: const VisitVertexPredicate&` - A predicate used to determine whether a vertex should be visited based on the vertex itself and its source/parent vertex's ID.
    - `visit: const VisitCallback&` - The vertex visiting function.
    - `enque_vertex_pred: const EnqueueVertexPred&` - A predicate used to determine whether a vertex should be pushed to the search stack based on the vertex itself and its source edge.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*: `void`

- `r_dfs(graph, vertex, source_id, visit_vertex_pred, visit, enque_vertex_pred, pre_visit, post_visit)`
  - *Description*: A recursive DFS algorithm template.

  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `VisitVertexPredicate: type_traits::c_optional_vertex_callback<GraphType, bool>` - The vertex visiting unary predicate type.
    - `VisitCallback: type_traits::c_vertex_callback<GraphType, bool, types::id_type>` - The vertex visting callback type (arguments: `vertex, source_id`).
    - `EnqueueVertexPred: type_traits::c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>` - The vertex enqueue predicate type (arguments: `vertex, in_edge`)
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `vertex: const typename GraphType::vertex_type&` - The vertex to search in the current recursive call.
    - `source_id: const types::id_type` - The ID of the parent/source vertex of the currently searched vertex.
    - `visit_vertex_pred: const VisitVertexPredicate&` - A predicate used to determine whether a vertex should be visited based on the vertex itself and its source/parent vertex's ID.
    - `visit: const VisitCallback&` - The vertex visiting function.
    - `enque_vertex_pred: const EnqueueVertexPred&` - A predicate used to determine whether a vertex should be pushed to the search stack based on the vertex itself and its source edge.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Return type*: `void`

### Breadth-first search template

> [!NOTE]
> The BFS algorithm template is defined in the [gl/algorithm/impl/bfs.hpp](/include/gl/algorithm/impl/bfs.hpp) file.

- `bfs(graph, initial_queue_content, visit_vertex_pred, visit, enque_vertex_pred, pre_visit, post_visit)`
  - *Desciption*: A breadth-first search algorithm template.

  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `InitQueueRangeType: type_traits::c_sized_range_of<algorithm::vertex_info>` (default = `std::vector<algorithm::vertex_info>`) - The type of the `vertex_info` range which will be inserted into the queue at the beginning of the algorithm.
    - `VisitVertexPredicate: type_traits::c_optional_vertex_callback<GraphType, bool>` - The vertex visiting unary predicate type.
    - `VisitCallback: type_traits::c_vertex_callback<GraphType, bool, types::id_type>` - The vertex visting callback type (arguments: `vertex, source_id`).
    - `EnqueueVertexPred: type_traits::c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>` - The vertex enqueue predicate type (arguments: `vertex, in_edge`)
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `initial_queue_content: const InitQueueRangeType&` - The `vertex_info` range which will be inserted into the queue at the beginning of the algorithm.
    - `visit_vertex_pred: const VisitVertexPredicate&` - A predicate used to determine whether a vertex should be visited based on the vertex itself and its source/parent vertex's ID.
    - `visit: const VisitCallback&` - The vertex visiting function.
    - `enque_vertex_pred: const EnqueueVertexPred&` - A predicate used to determine whether a vertex should be pushed to the search stack based on the vertex itself and its source edge.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Queue type:* `std::queue<algorithm::vertex_info>`

  - *Return type*: `void`

### Priority-first search template

> [!NOTE]
> The PFS algorithm template is defined in the [gl/algorithm/impl/pfs.hpp](/include/gl/algorithm/impl/pfs.hpp) file.

- `pfs(graph, pq_compare, initial_queue_content, visit_vertex_pred, visit, enque_vertex_pred, pre_visit, post_visit)`
  - *Desciption*: A priority-first search algorithm template - modification of the BFS algorithm with a heap-based priority queue instead of a standard *FIFO* structure (queue).

  - *Template parameters*:
    - `GraphType: type_traits::c_graph` - The type of the graph on which the search is performed.
    - `PQCompare: std::predicate<algorithm::vertex_info, algorithm::vertex_info>` - The type of the vertex priority queue comparator.
    - `InitQueueRangeType: type_traits::c_sized_range_of<algorithm::vertex_info>` (default = `std::vector<algorithm::vertex_info>`) - The type of the `vertex_info` range which will be inserted into the queue at the beginning of the algorithm.
    - `VisitVertexPredicate: type_traits::c_optional_vertex_callback<GraphType, bool>` - The vertex visiting unary predicate type.
    - `VisitCallback: type_traits::c_vertex_callback<GraphType, bool, types::id_type>` - The vertex visting callback type (arguments: `vertex, source_id`).
    - `EnqueueVertexPred: type_traits::c_vertex_callback<GraphType, std::optional<bool>, const typename GraphType::edge_type&>` - The vertex enqueue predicate type (arguments: `vertex, in_edge`)
    - `PreVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called before visiting a vertex.
    - `PostVisitCallback: type_traits::c_vertex_callback<GraphType, void>` (default = `algorithm::empty_callback`) - The type of the callback function called after visiting a vertex.

  - *Parameters*:
    - `graph: const GraphType&` - The graph to perform DFS on.
    - `pq_compare: const PQCompare&` - The vertex priority queue comparator.
    - `initial_queue_content: const InitQueueRangeType&` - The `vertex_info` range which will be inserted into the queue at the beginning of the algorithm.
    - `visit_vertex_pred: const VisitVertexPredicate&` - A predicate used to determine whether a vertex should be visited based on the vertex itself and its source/parent vertex's ID.
    - `visit: const VisitCallback&` - The vertex visiting function.
    - `enque_vertex_pred: const EnqueueVertexPred&` - A predicate used to determine whether a vertex should be pushed to the search stack based on the vertex itself and its source edge.
    - `pre_visit: const PreVisitCallback&` (default = `{}`) - The callback function to be called before visiting a vertex.
    - `post_visit: const PostVisitCallback&` (default = `{}`) - The callback function to be called after visiting a vertex.

  - *Queue type:* `std::priority_queue<algorithm::vertex_info, std::vector<algorithm::vertex_info>, PQCompare>`

  - *Return type*: `void`

<br />
<br />

## Related pages

- [The graph class - library's core](/docs/graph.md)
- [Graph topology generators](/docs/topologies.md)
- [I/O operations](/docs/io.md)
- [Core utility types](/docs/core_util_types.md)
