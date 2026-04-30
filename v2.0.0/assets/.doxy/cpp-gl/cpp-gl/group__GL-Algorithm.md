

# Group GL-Algorithm



[**Modules**](modules.md) **>** [**GL-Algorithm**](group__GL-Algorithm.md)



_Generic graph algorithms, foundational search templates, and related utilities._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**gl::algorithm::decision**](structgl_1_1algorithm_1_1decision.md) <br>_Represents a generic tri-state decision for control flow._  |
| struct | [**gl::algorithm::dijkstra\_search\_node**](structgl_1_1algorithm_1_1dijkstra__search__node.md) &lt;G&gt;<br>_Internal node structure for Dijkstra's algorithm to snapshot distances and preserve heap invariants._  |
| struct | [**gl::algorithm::empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md) <br>_A tag type used to explicitly indicate the absence of a callback function._  |
| struct | [**gl::algorithm::mst\_descriptor**](structgl_1_1algorithm_1_1mst__descriptor.md) &lt;G&gt;<br>_A descriptor structure holding the results of a Minimum Spanning Tree (MST) execution._  |
| struct | [**gl::algorithm::no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md) <br>_Tag type providing an implicit conversion to the appropriate_ `no_root_v` _for any numeric ID type._ |
| struct | [**gl::algorithm::paths\_descriptor**](structgl_1_1algorithm_1_1paths__descriptor.md) &lt;G, VertexDistanceType&gt;<br>_A descriptor structure holding the results of a single-source shortest path execution._  |
| struct | [**gl::algorithm::search\_node**](structgl_1_1algorithm_1_1search__node.md) &lt;GraphType&gt;<br>_Represents an active node in a search container (e.g., a BFS queue or DFS stack)._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::vector&lt; binary\_color &gt; | [**bicoloring\_type**](#typedef-bicoloring_type)  <br>_Alias for a container mapping vertex indices to their calculated binary (bipartite) colors._  |
| typedef std::conditional\_t&lt; Result==algorithm::ret, ResultType, std::monostate &gt; | [**non\_void\_result\_type**](#typedef-non_void_result_type)  <br>_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`std::monostate` _._ |
| typedef paths\_descriptor&lt; G, vertex\_distance\_type&lt; G &gt; &gt; | [**paths\_descriptor\_type**](#typedef-paths_descriptor_type)  <br>_An alias for_ [_**paths\_descriptor**_](structgl_1_1algorithm_1_1paths__descriptor.md) _that automatically deduces the appropriate distance type for the graph._ |
| typedef std::vector&lt; typename GraphType::id\_type &gt; | [**predecessors\_map**](#typedef-predecessors_map)  <br>_Maps a vertex ID to its predecessor's ID in a traversal tree._  |
| enum bool | [**result\_discriminator**](#enum-result_discriminator)  <br>_Tag used to statically dictate whether an algorithm should return a constructed result or execute purely for side effects._  |
| typedef std::conditional\_t&lt; Result==algorithm::ret, ResultType, void &gt; | [**result\_type**](#typedef-result_type)  <br>_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`void` _._ |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr no\_root\_t | [**no\_root**](#variable-no_root)  = `{}`<br>_Global constant representing the absence of a root vertex._  |
|  constexpr IdType | [**no\_root\_v**](#variable-no_root_v)  &lt;IdType&gt; = `invalid_id_v<IdType>`<br>_Constant representing the absence of a root vertex ID for a specific ID type._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  bool | [**apply\_coloring**](#function-apply_coloring) (G & graph, const ColorRange & color\_range) <br>_Applies a computed range of binary colors to the property payload of each vertex in the graph._  |
|  bool | [**bfs**](#function-bfs) (...) <br>_A highly customizable, generic Breadth-First Search (BFS) algorithm engine._  |
|  std::optional&lt; bicoloring\_type &gt; | [**bipartite\_coloring**](#function-bipartite_coloring) (...) <br>_Attempts to compute a valid bipartite (2-color) coloring for the given graph._  |
|  result\_type&lt; Result, predecessors\_map&lt; G &gt; &gt; | [**breadth\_first\_search**](#function-breadth_first_search) (...) <br>_Executes a concrete Breadth-First Search (BFS) traversal over the graph._  |
|  auto | [**default\_enqueue\_node\_predicate**](#function-default_enqueue_node_predicate) (std::vector&lt; bool &gt; & visited) <br>_Generates a default lambda predicate that checks if a node corresponding to an adjacent vertex should be enqueued into the search container._  |
|  auto | [**default\_visit\_callback**](#function-default_visit_callback) (...) <br>_Generates a default lambda callback that marks a vertex as visited and updates the predecessor map._  |
|  auto | [**default\_visit\_vertex\_predicate**](#function-default_visit_vertex_predicate) (std::vector&lt; bool &gt; & visited) <br>_Generates a default lambda predicate that checks if a vertex has not yet been visited._  |
|  result\_type&lt; Result, predecessors\_map&lt; G &gt; &gt; | [**depth\_first\_search**](#function-depth_first_search) (...) <br>_Executes a concrete iterative Depth-First Search (DFS) traversal over the graph._  |
|  bool | [**dfs**](#function-dfs) (...) <br>_A highly customizable, generic iterative Depth-First Search (DFS) algorithm engine._  |
|  paths\_descriptor\_type&lt; G &gt; | [**dijkstra\_shortest\_paths**](#function-dijkstra_shortest_paths) (...) <br>_Computes the shortest paths from a single source vertex to all reachable vertices using Dijkstra's algorithm._  |
|  mst\_descriptor&lt; G &gt; | [**edge\_heap\_prim\_mst**](#function-edge_heap_prim_mst) (...) <br>_Computes the Minimum Spanning Tree (MST) of an undirected graph using Prim's algorithm with an edge-based priority queue._  |
|  InitRangeType | [**init\_node\_range**](#function-init_node_range) (typename G::id\_type root\_vertex\_id) <br>_Initializes a search container with the starting root vertex._  |
|  non\_void\_result\_type&lt; Result, predecessors\_map&lt; G &gt; &gt; | [**init\_predecessors\_map**](#function-init_predecessors_map) (const G & graph) <br>_Initializes a predecessor map based on the static result discriminator._  |
|  bool | [**is\_bipartite**](#function-is_bipartite) (const traits::c\_graph auto & graph) <br>_Convenience wrapper for the_ [_**bipartite\_coloring**_](group__GL-Algorithm.md#function-bipartite_coloring) _algorithm to check if a graph is bipartite without extracting the exact coloring map._ |
|  bool | [**is\_reachable**](#function-is_reachable) (...) noexcept<br>_Checks if a specific vertex was reached during a traversal._  |
|  paths\_descriptor\_type&lt; G &gt; | [**make\_paths\_descriptor**](#function-make_paths_descriptor) (const G & graph) <br>_Factory function to create an initialized paths descriptor sized for the given graph._  |
|  bool | [**pfs**](#function-pfs) (...) <br>_A highly customizable, generic Priority-First Search (PFS) algorithm engine._  |
|  void | [**r\_dfs**](#function-r_dfs) (...) <br>_A highly customizable, generic recursive Depth-First Search (DFS) algorithm engine._  |
|  std::vector&lt; IdType &gt; | [**reconstruct\_path**](#function-reconstruct_path) (...) <br>_Reconstructs the sequence of vertices forming a path to a specific target._  |
|  result\_type&lt; Result, predecessors\_map&lt; G &gt; &gt; | [**recursive\_depth\_first\_search**](#function-recursive_depth_first_search) (...) <br>_Executes a concrete recursive Depth-First Search (DFS) traversal over the graph._  |
|  std::optional&lt; std::vector&lt; typename G::id\_type &gt; &gt; | [**topological\_sort**](#function-topological_sort) (...) <br>_Computes a topological ordering of the vertices in a Directed Acyclic Graph (DAG)._  |
|  mst\_descriptor&lt; G &gt; | [**vertex\_heap\_prim\_mst**](#function-vertex_heap_prim_mst) (...) <br>_Computes the Minimum Spanning Tree (MST) of an undirected graph using Prim's algorithm with a vertex-based array heap._  |




























## Detailed Description


This module provides a dual-layered approach to graph algorithms. At its core, it features highly generic and extensible search templates (such as BFS, DFS, and Priority-First Search). These templates serve as the foundational building blocks of the library, empowering users to inject custom behavior via callbacks and predicates to build entirely new algorithms with minimal effort.


Alongside these templates, the module offers a suite of concrete, ready-to-use algorithms. Rather than a flat list of functions, the algorithms are categorized into broad domains, including basic graph traversal, pathfinding, minimum spanning trees, and topological operations like sorting and coloring. All implementations are designed to operate seamlessly with generic graph types and property maps. 


    
## Public Types Documentation




### typedef bicoloring\_type 

_Alias for a container mapping vertex indices to their calculated binary (bipartite) colors._ 
```cpp
using gl::algorithm::bicoloring_type = typedef std::vector<binary_color>;
```




<hr>



### typedef non\_void\_result\_type 

_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`std::monostate` _._
```cpp
template<result_discriminator Result, typename ResultType>
using gl::algorithm::non_void_result_type = typedef std::conditional_t<Result == algorithm::ret, ResultType, std::monostate>;
```



Useful for returning dummy values from conditionally compiled algorithm branches.


#### See Also




* [**result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator)
* [**result\_type**](group__GL-Algorithm.md#typedef-result_type) 





        

<hr>



### typedef paths\_descriptor\_type 

_An alias for_ [_**paths\_descriptor**_](structgl_1_1algorithm_1_1paths__descriptor.md) _that automatically deduces the appropriate distance type for the graph._
```cpp
template<traits::c_graph G>
using gl::algorithm::paths_descriptor_type = typedef paths_descriptor<G, vertex_distance_type<G> >;
```





**Template parameters:**


* `G` The type of the graph. 




        

<hr>



### typedef predecessors\_map 

_Maps a vertex ID to its predecessor's ID in a traversal tree._ 
```cpp
template<traits::c_graph GraphType>
using gl::algorithm::predecessors_map = typedef std::vector<typename GraphType::id_type>;
```





**Template parameters:**


* `GraphType` The type of the graph being traversed. 




        

<hr>



### enum result\_discriminator 

_Tag used to statically dictate whether an algorithm should return a constructed result or execute purely for side effects._ 
```cpp
enum result_discriminator {
    ret = true,
    noret = false
};
```




> [!NOTE] Namespace Availability
>
>
> Because this enum uses the `using enum` declaration, its members (`ret` and `noret`) are injected directly into the `gl::algorithm` namespace. Hence, you can use `gl::algorithm::ret` and `gl::algorithm::noret` directly without the `result_discriminator::` scope. 
>
>



#### See Also




* [**result\_type**](group__GL-Algorithm.md#typedef-result_type)
* [**non\_void\_result\_type**](group__GL-Algorithm.md#typedef-non_void_result_type) 





        

<hr>



### typedef result\_type 

_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`void` _._
```cpp
template<result_discriminator Result, typename ResultType>
using gl::algorithm::result_type = typedef std::conditional_t<Result == algorithm::ret, ResultType, void>;
```



#### See Also




* [**result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator)
* [**non\_void\_result\_type**](group__GL-Algorithm.md#typedef-non_void_result_type) 





        

<hr>
## Public Attributes Documentation




### variable no\_root 

_Global constant representing the absence of a root vertex._ 
```cpp
constexpr no_root_t gl::algorithm::no_root = {};
```



#### See Also




* [**no\_root\_v**](group__GL-Algorithm.md#variable-no_root_v)
* [**no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md) 





        

<hr>



### variable no\_root\_v 

_Constant representing the absence of a root vertex ID for a specific ID type._ 
```cpp
template<traits::c_id_type IdType>
constexpr IdType gl::algorithm::no_root_v = invalid_id_v<IdType>;
```



#### See Also




* [**gl::invalid\_id\_v**](group__GL-Core.md#variable-invalid_id_v)
* [**no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md)
* [**no\_root**](group__GL-Algorithm.md#variable-no_root) 





        

<hr>
## Public Functions Documentation




### function apply\_coloring 

_Applies a computed range of binary colors to the property payload of each vertex in the graph._ 
```cpp
template<traits::c_graph G, traits::c_sized_range_of< binary_color > ColorRange>
bool apply_coloring (
    G & graph,
    const ColorRange & color_range
) 
```



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph to modify. Must have compatible color properties.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept, and its properties must satisfy [**c\_binary\_color\_properties\_type**](gl_concepts.md#gl-traits-c-binary-color-properties-type).|
|ColorRange|The type of the range containing the computed colors.|Must satisfy the [**c\_sized\_range\_of**](gl_concepts.md#gl-traits-c-sized-range-of) concept for `binary_color`.|








**Parameters:**


* `graph` The mutable graph instance whose properties will be updated. 
* `color_range` A sized range (e.g., [**bicoloring\_type**](group__GL-Algorithm.md#typedef-bicoloring_type)) matching the vertex count. 



**Returns:**

`true` if the coloring was successfully applied, `false` if the size of the range does not match the graph's vertex count. 






#### See Also




* [**bipartite\_coloring**](group__GL-Algorithm.md#function-bipartite_coloring)
* [**is\_bipartite**](group__GL-Algorithm.md#function-is_bipartite) 





        

<hr>



### function bfs 

_A highly customizable, generic Breadth-First Search (BFS) algorithm engine._ 
```cpp
template<traits::c_graph G, traits::c_forward_range_of< search_node< G > > InitQueueRangeType, traits::c_optional_predicate< typename G::id_type > VisitVertexPredicate, traits::c_optional_predicate< typename G::id_type, typename G::id_type > VisitCallback, traits::c_decision_predicate< typename G::id_type, const typename G::edge_type & > EnqueueNodePred, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
bool bfs (...) 
```



This template does not implement a specific algorithm (like finding a shortest path). Instead, it provides the strict structural execution of a queue-based Breadth-First Search. Concrete algorithms are constructed by injecting logic into the provided callback and predicate hooks.


#### Example Usage




```cpp
std::vector<bool> visited(graph.n_vertices(), false); // (1)!

bool completed = gl::algorithm::bfs(
    graph,
    gl::algorithm::init_node_range<graph_type>(start_id), // (2)!
    gl::algorithm::default_visit_vertex_predicate(visited), // (3)!
    [&](auto v, auto p) { // (4)!
        std::cout << "Visited vertex " << v << '\n';
        return true; // Continue search
    },
    gl::algorithm::default_enqueue_node_predicate<graph_type, true>(visited) // (5)!
);
```



1. Tracks discovered vertices.


2. Initializes the search queue with the starting vertex.


3. Predicate ensuring we don't process a vertex if it was already marked visited.


4. The main visit callback. Here we just print the ID. Returning `false` would abort the search.


5. Predicate ensuring we only enqueue adjacent vertices that haven't been visited yet, returning a [**decision**](structgl_1_1algorithm_1_1decision.md).



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|InitQueueRangeType|The type of the container providing the initial roots to enqueue.|Must be a _forward range_ of [**search nodes**](structgl_1_1algorithm_1_1search__node.md).|
|VisitVertexPredicate|Type of the callable deciding if a popped vertex should be processed.|Must be one of:<br>- An `(id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|VisitCallback|Type of the callable executed when a vertex is officially visited.|Must be one of:<br>- An `(id_type, id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|EnqueueNodePred|Type of the callable deciding if a node corresponding to an adjacent vertex should be pushed to the queue.|Must be one of:<br>- An `(id_type, const edge_type&) -> decision` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PreVisitCallback|Type of the callable executed immediately before `VisitCallback`.|Must be one of:<br>- An `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges are evaluated.|Must be one of:<br>- An `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `initial_queue_content` A range of initial [**search nodes**](structgl_1_1algorithm_1_1search__node.md) to seed the BFS queue. 
* `visit_vertex_pred` Predicate evaluated immediately after popping a vertex. If it returns `false`, the vertex is skipped. 
* `visit` Callback invoked when a vertex is officially visited. If it returns `false`, the entire BFS immediately aborts. 
* `enqueue_node_pred` Predicate evaluated for each outgoing edge. Returns a [**decision**](structgl_1_1algorithm_1_1decision.md):
    * `accept` to enqueue,
    * `reject` to skip,
    * `abort` to terminate the BFS entirely. 


* `pre_visit` Hook executed immediately before the `visit` callback. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been evaluated. 



**Returns:**

`true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.    






        

<hr>



### function bipartite\_coloring 

_Attempts to compute a valid bipartite (2-color) coloring for the given graph._ 
```cpp
template<traits::c_graph G, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
std::optional< bicoloring_type > bipartite_coloring (...) 
```



This algorithm utilizes the generic [**bfs**](group__GL-Algorithm.md#function-bfs) template to traverse the graph and alternate colors between adjacent vertices. If an edge connects two vertices of the same color (indicating an odd-length cycle), the graph is not bipartite, and the search immediately aborts.


#### Example Usage




```cpp
if (auto coloring = gl::algorithm::bipartite_coloring(graph)) // (1)!
    gl::algorithm::apply_coloring(graph, *coloring); // (2)!
else
    std::cout << "Graph contains an odd cycle and is not bipartite.\n";
```



1. Attempts to find a valid 2-coloring for the graph. Returns `std::nullopt` if impossible.


2. If successful, directly modifies the graph's internal vertex properties to store the colors. **NOTE:** This operation is only available if the property type of the graph's vertices satisfies [**c\_binary\_color\_properties\_type**](gl_concepts.md#gl-traits-c-binary-color-properties-type).



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends entirely on the underlying representation of `GraphType`:
> * **Adjacency List Representations**: \(O(|V| + |E|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over adjacent vertices requires scanning the entire \(|V|\)-length matrix row. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|PreVisitCallback|Type of the callable executed immediately before a vertex is officially visited.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges of a vertex are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to evaluate. 
* `pre_visit` Hook executed immediately before the internal visit logic. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been enqueued. 



**Returns:**

An `std::optional` containing the [**bicoloring\_type**](group__GL-Algorithm.md#typedef-bicoloring_type) map if the graph is bipartite. Returns `std::nullopt` otherwise. 






#### See Also




* [**is\_bipartite**](group__GL-Algorithm.md#function-is_bipartite)
* [**apply\_coloring**](group__GL-Algorithm.md#function-apply_coloring)    





        

<hr>



### function breadth\_first\_search 

_Executes a concrete Breadth-First Search (BFS) traversal over the graph._ 
```cpp
template<result_discriminator Result, traits::c_graph G, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
result_type< Result, predecessors_map< G > > breadth_first_search (...) 
```



This function utilizes the generic [**bfs**](group__GL-Algorithm.md#function-bfs) template to perform a standard queue-based traversal. It automatically manages the visited states, predecessor tracking, and queue initialization.


If a specific `root_vertex_id` is provided, the algorithm explores only the connected component reachable from that root. If `no_root` is used, it iteratively ensures that every disconnected component in the entire graph is fully traversed.


#### Example Usage




```cpp
auto pred_map
    = gl::algorithm::breadth_first_search(graph, start_id); // (1)!

gl::algorithm::breadth_first_search<gl::algorithm::noret>( // (2)!
    graph,
    gl::algorithm::no_root, // (3)!
    [](auto v) { std::cout << "Discovered: " << v << '\n'; } // (4)!
);
```



1. Standard execution returning a predecessor map from a specific root. Traverses only the component reachable from `start_id` and builds a predecessor tree.


2. Execution purely for side-effects over the entire graph. Uses the [**noret**](group__GL-Algorithm.md#enum-result_discriminator) discriminator to completely compile away the predecessor map allocations.


3. Traverses all vertices in the graph, regardless of disconnected components.


4. A custom `PreVisitCallback` executed when a vertex is popped from the queue.



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends entirely on the underlying representation of `GraphType`:
> * **Adjacency List Representations**: \(O(|V| + |E|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over adjacent vertices requires scanning the entire \(|V|\)-length matrix row. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Discriminator dictating if the algorithm should return a predecessor map (`ret`) or `void` (`noret`).|Must be a valid [**result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator) enum value.|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|PreVisitCallback|Type of the callable executed immediately before a vertex is officially visited.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges of a vertex are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `root_vertex_id` The starting vertex for the search. Defaults to [**no\_root**](group__GL-Algorithm.md#variable-no_root) to traverse the entire graph. 
* `pre_visit` Hook executed immediately before the internal visit logic. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been enqueued. 



**Returns:**

A [**predecessors\_map**](group__GL-Algorithm.md#typedef-predecessors_map) mapping each visited vertex to its parent if `Result == ret`. Returns `void` otherwise.    






        

<hr>



### function default\_enqueue\_node\_predicate 

_Generates a default lambda predicate that checks if a node corresponding to an adjacent vertex should be enqueued into the search container._ 
```cpp
template<traits::c_graph G, bool AsDecision>
auto default_enqueue_node_predicate (
    std::vector< bool > & visited
) 
```





**Template parameters:**


* `G` The type of the graph. 
* `AsDecision` If `true`, the generated predicate returns a [**decision**](structgl_1_1algorithm_1_1decision.md) instead of a raw boolean. 



**Parameters:**


* `visited` A reference to the boolean array tracking visited vertices. 



**Returns:**

A callable predicate that returns `true` (or `decision::accept`) if the adjacent vertex has not been visited. 





        

<hr>



### function default\_visit\_callback 

_Generates a default lambda callback that marks a vertex as visited and updates the predecessor map._ 
```cpp
template<traits::c_graph G, result_discriminator Result>
auto default_visit_callback (...) 
```





**Template parameters:**


* `G` The type of the graph. 
* `Result` The static discriminator indicating if the predecessor map should be updated. 



**Parameters:**


* `visited` A reference to the boolean array tracking visited vertices. 
* `pred_map` A reference to the active predecessor map. 



**Returns:**

A callable callback that executes state updates upon visiting a vertex.    





        

<hr>



### function default\_visit\_vertex\_predicate 

_Generates a default lambda predicate that checks if a vertex has not yet been visited._ 
```cpp
auto default_visit_vertex_predicate (
    std::vector< bool > & visited
) 
```





**Parameters:**


* `visited` A reference to the boolean array tracking visited vertices. 



**Returns:**

A callable predicate evaluating to `true` if the vertex is unvisited. 





        

<hr>



### function depth\_first\_search 

_Executes a concrete iterative Depth-First Search (DFS) traversal over the graph._ 
```cpp
template<result_discriminator Result, traits::c_graph G, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
result_type< Result, predecessors_map< G > > depth_first_search (...) 
```



This function utilizes the generic [**dfs**](group__GL-Algorithm.md#function-dfs) template to perform a standard, stack-based traversal. It automatically manages the visited states and predecessor tracking.


If a specific `root_vertex_id` is provided, the algorithm explores only the connected component reachable from that root. If `no_root` is used, it iteratively ensures that every disconnected component in the entire graph is fully traversed.


#### Example Usage




```cpp
// Standard execution returning a predecessor map for the whole graph
auto pred_map = gl::algorithm::depth_first_search(graph); // (1)!
```



1. Explores every vertex in the graph and builds a complete predecessor forest.



#### Example Usage




```cpp
auto pred_map
    = gl::algorithm::depth_first_search(graph, start_id); // (1)!

gl::algorithm::depth_first_search<gl::algorithm::noret>( // (2)!
    graph,
    gl::algorithm::no_root, // (3)!
    [](auto v) { std::cout << "Discovered: " << v << '\n'; } // (4)!
);
```



1. Standard execution returning a predecessor map from a specific root. Traverses only the component reachable from `start_id` and builds a predecessor tree.


2. Execution purely for side-effects over the entire graph. Uses the [**noret**](group__GL-Algorithm.md#enum-result_discriminator) discriminator to completely compile away the predecessor map allocations.


3. Traverses all vertices in the graph, regardless of disconnected components.


4. A custom `PreVisitCallback` executed when a vertex is popped from the stack.



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends entirely on the underlying representation of `GraphType`:
> * **Adjacency List Representations**: \(O(|V| + |E|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over adjacent vertices requires scanning the entire \(|V|\)-length matrix row. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|[**Discriminator**](group__GL-Algorithm.md#enum-result_discriminator) dictating if the algorithm should return a predecessor map (`ret`) or `void` (`noret`).|Must be a valid [**result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator) enum value.|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|PreVisitCallback|Type of the callable executed immediately before a vertex is officially visited.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges of a vertex are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `root_vertex_id` The starting vertex for the search. Defaults to [**no\_root**](group__GL-Algorithm.md#variable-no_root) to traverse the entire graph. 
* `pre_visit` Hook executed immediately before the internal visit logic. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been evaluated. 



**Returns:**

A [**predecessors\_map**](group__GL-Algorithm.md#typedef-predecessors_map) mapping each visited vertex to its parent if `Result == ret`. Returns `void` otherwise.    






        

<hr>



### function dfs 

_A highly customizable, generic iterative Depth-First Search (DFS) algorithm engine._ 
```cpp
template<traits::c_graph G, traits::c_forward_range_of< search_node< G > > InitStackRangeType, traits::c_optional_predicate< typename G::id_type > VisitVertexPredicate, traits::c_optional_predicate< typename G::id_type, typename G::id_type > VisitCallback, traits::c_decision_predicate< typename G::id_type, const typename G::edge_type & > EnqueueNodePred, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
bool dfs (...) 
```



This engine provides the strict structural execution of a stack-based Depth-First Search. Concrete algorithms (like cycle detection or topological sorting) are built by injecting specific logic into the provided callback hooks.


#### Example Usage




```cpp
std::vector<bool> visited(graph.n_vertices(), false); // (1)!

bool completed = gl::algorithm::dfs(
    graph,
    gl::algorithm::init_node_range<graph_type>(start_id), // (2)!
    gl::algorithm::default_visit_vertex_predicate(visited), // (3)!
    [&](auto v, auto p) { // (4)!
        std::cout << "Visited vertex " << v << '\n';
        return true; // Continue search
    },
    gl::algorithm::default_enqueue_node_predicate<graph_type, true>(visited) // (5)!
);
```



1. Tracks discovered vertices.


2. Initializes the search stack with the starting vertex.


3. Predicate ensuring we don't process a vertex if it was already marked visited.


4. The main visit callback. Returning `false` would abort the search.


5. Predicate ensuring we only push adjacent, unvisited vertices to the stack, returning a [**decision**](structgl_1_1algorithm_1_1decision.md).



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|InitStackRangeType|The type of the container providing the initial roots to push to the stack.|Must be a _forward range_ of [**search nodes**](structgl_1_1algorithm_1_1search__node.md).|
|VisitVertexPredicate|Type of the callable deciding if a popped vertex should be processed.|Must be one of:<br>- An `(id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|VisitCallback|Type of the callable executed when a vertex is officially visited.|Must be one of:<br>- An `(id_type, id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|EnqueueNodePred|Type of the callable deciding if a node corresponding to an adjacent vertex should be pushed to the stack.|Must be one of:<br>- An `(id_type, const edge_type&) -> decision` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PreVisitCallback|Type of the callable executed immediately before `VisitCallback`.|Must be one of:<br>- An `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges are evaluated.|Must be one of:<br>- An `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `initial_stack_content` A range of initial [**search nodes**](structgl_1_1algorithm_1_1search__node.md) to seed the DFS stack. 
* `visit_vertex_pred` Predicate evaluated immediately after popping a vertex. If it returns `false`, the vertex is skipped. 
* `visit` Callback invoked when a vertex is officially visited. If it returns `false`, the entire DFS immediately aborts. 
* `enqueue_node_pred` Predicate evaluated for each outgoing edge. Returns a [**decision**](structgl_1_1algorithm_1_1decision.md):
    * `accept` to enqueue,
    * `reject` to skip,
    * `abort` to terminate the DFS entirely. 


* `pre_visit` Hook executed immediately before the `visit` callback. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been evaluated. 



**Returns:**

`true` if the stack was exhausted naturally, `false` if the search was aborted early.    






        

<hr>



### function dijkstra\_shortest\_paths 

_Computes the shortest paths from a single source vertex to all reachable vertices using Dijkstra's algorithm._ 
```cpp
template<traits::c_graph G, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
paths_descriptor_type< G > dijkstra_shortest_paths (...) 
```



This algorithm utilizes the generic [**pfs**](group__GL-Algorithm.md#function-pfs) template using the dedicated [**serch node type**](structgl_1_1algorithm_1_1dijkstra__search__node.md) to perform a priority-first search based on accumulated edge weights. It strictly requires non-negative edge weights; if a negative weight is encountered during traversal, the algorithm immediately throws an exception.


#### Example Usage




```cpp
auto paths = gl::algorithm::dijkstra_shortest_paths(graph, source_id); // (1)!

auto path_to_target
    = gl::algorithm::reconstruct_path(paths.predecessors, target_id); // (2)!
std::cout << "Path: "
          << gl::io::range_formatter(path_to_target, " -> ", "", "") // (3)!
          << "\nDistance: " << paths.distances[target_id] << '\n'; // (4)!
```



1. Executes the shortest path calculation from the given `source_id`.


2. Reconstructs the exact sequence of vertices from the source to the `target_id` using the [**reconstruct\_path**](group__GL-Algorithm.md#function-reconstruct_path) function.


3. Prints the path to the target vertex using the [**range\_formatter**](structgl_1_1io_1_1range__formatter.md) helper.


4. Retrievs the total distance to the target vertex from the [**paths descriptor**](structgl_1_1algorithm_1_1paths__descriptor.md) object returned by Dijkstra's algorithm.



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends on the underlying representation of `GraphType` and the priority queue overhead:
> * **Adjacency List Representations**: \(O((|V| + |E|) \log |V|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2 + |E| \log |V|)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over adjacent vertices requires scanning the entire \(|V|\)-length matrix row. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph being traversed. Must define a valid distance/weight property.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|PreVisitCallback|Type of the callable executed immediately before a vertex is officially visited.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges of a vertex are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to evaluate. 
* `source_id` The starting vertex ID for the shortest path calculation. 
* `pre_visit` Hook executed immediately before the internal visit logic. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been enqueued. 



**Returns:**

A [**paths\_descriptor**](structgl_1_1algorithm_1_1paths__descriptor.md) containing the accumulated distances and predecessor map. 




**Exception:**


* `std::invalid_argument` If an edge with a negative weight is encountered.    





        

<hr>



### function edge\_heap\_prim\_mst 

_Computes the Minimum Spanning Tree (MST) of an undirected graph using Prim's algorithm with an edge-based priority queue._ 
```cpp
template<traits::c_undirected_graph G>
mst_descriptor< G > edge_heap_prim_mst (...) 
```



This implementation uses a standard binary heap (`std::priority_queue`) to store and sort edges based on their weight. It pushes newly discovered adjacent edges into the queue and safely ignores those that lead to already-visited vertices.


#### Example Usage




```cpp
auto mst = gl::algorithm::edge_heap_prim_mst(graph, start_id); // (1)!
std::cout << "Total MST Weight: " << mst.weight
          << "\nMST Edges: " << gl::io::set_formatter(mst.edges) << '\n';
```



1. Computes the MST starting from the given `start_id`. If `invalid_id` is passed, it defaults to the graph's `initial_id`.



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends on the underlying representation of `GraphType` and the queue overhead:
> * **Adjacency List Representations**: \(O(|E| \log |E|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>     * _Note:_ In simple graphs, this simplifies to \(O(|E| \log |V|)\). However, because list models allow multigraphs, the queue size and operations scale strictly with \(|E|\).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2 + |E| \log |V|)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over incident edges requires scanning the entire \(|V|\)-length matrix row. Since matrices represent simple graphs, the heap operations safely simplify to \(O(\log |V|)\). 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the undirected graph being traversed.|Must satisfy the [**c\_undirected\_graph**](gl_concepts.md#gl-traits-c-undirected-graph) concept.|








**Parameters:**


* `graph` The undirected graph to evaluate. 
* `root_id` The starting vertex ID for the MST calculation. Defaults to the graph's `initial_id` if `invalid_id` is passed. 



**Returns:**

A [**mst\_descriptor**](structgl_1_1algorithm_1_1mst__descriptor.md) containing the accumulated minimum weight and the sequence of edges forming the tree. 






#### See Also




* [**vertex\_heap\_prim\_mst**](group__GL-Algorithm.md#function-vertex_heap_prim_mst) For the vertex-heap variant of the Prim's MST finding algorithm.    





        

<hr>



### function init\_node\_range 

_Initializes a search container with the starting root vertex._ 
```cpp
template<traits::c_graph G, traits::c_forward_range_of< search_node< G > > InitRangeType>
InitRangeType init_node_range (
    typename G::id_type root_vertex_id
) 
```





**Template parameters:**


* `G` The type of the graph. 
* `InitRangeType` The underlying container type for the container. 



**Parameters:**


* `root_vertex_id` The ID of the starting vertex. 



**Returns:**

A container initialized with a single [**search\_node**](structgl_1_1algorithm_1_1search__node.md) for the root vertex. 





        

<hr>



### function init\_predecessors\_map 

_Initializes a predecessor map based on the static result discriminator._ 
```cpp
template<result_discriminator Result, traits::c_graph G>
non_void_result_type< Result, predecessors_map< G > > init_predecessors_map (
    const G & graph
) 
```





**Template parameters:**


* `Result` The compilation tag determining if the map should actually be built. 
* `G` The type of the graph. 



**Parameters:**


* `graph` The graph instance to size the map against. 



**Returns:**

A fully sized and initialized `predecessors_map` if `Result == ret`, otherwise a dummy `std::monostate`. 





        

<hr>



### function is\_bipartite 

_Convenience wrapper for the_ [_**bipartite\_coloring**_](group__GL-Algorithm.md#function-bipartite_coloring) _algorithm to check if a graph is bipartite without extracting the exact coloring map._
```cpp
bool is_bipartite (
    const traits::c_graph auto & graph
) 
```





**Parameters:**


* `graph` The graph to evaluate. 



**Returns:**

`true` if the graph is bipartite (2-colorable), `false` otherwise. 





#### See Also




* [**apply\_coloring**](group__GL-Algorithm.md#function-apply_coloring) 





        

<hr>



### function is\_reachable 

_Checks if a specific vertex was reached during a traversal._ 
```cpp
template<traits::c_id_type IdType>
bool is_reachable (...) noexcept
```





**Template parameters:**


* `IdType` The integral type of the vertex ID. 



**Parameters:**


* `pred_map` The predecessor map populated by the traversal. 
* `vertex_id` The vertex ID to query. 



**Returns:**

`true` if the vertex has a valid assigned predecessor, `false` otherwise.    





        

<hr>



### function make\_paths\_descriptor 

_Factory function to create an initialized paths descriptor sized for the given graph._ 
```cpp
template<traits::c_graph G>
paths_descriptor_type< G > make_paths_descriptor (
    const G & graph
) 
```





**Template parameters:**


* `G` The type of the graph. 



**Parameters:**


* `graph` The graph to size the descriptor against. 



**Returns:**

A [**paths\_descriptor**](structgl_1_1algorithm_1_1paths__descriptor.md) initialized with invalid predecessors and default-constructed distances. 





        

<hr>



### function pfs 

_A highly customizable, generic Priority-First Search (PFS) algorithm engine._ 
```cpp
template<traits::c_graph G, typename PQCmp, typename InitQueueRangeType, typename NodeType, traits::c_optional_predicate< NodeType > VisitVertexPredicate, traits::c_optional_predicate< typename G::id_type, typename G::id_type > VisitCallback, traits::c_decision_predicate< typename G::id_type, const typename G::edge_type & > EnqueueNodePred, traits::c_optional_callback< NodeType, typename G::id_type, typename G::id_type, const typename G::edge_type & > MakeNodeCallback, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
bool pfs (...) 
```



This template provides the strict structural execution of a priority queue-based search. It acts as the underlying engine for algorithms like Dijkstra's Shortest Path Search. Concrete algorithms are constructed by injecting logic into the provided callbacks and defining the custom priority comparator.


#### Example Usage




```cpp
std::vector<bool> visited(graph.n_vertices(), false); // (1)!

bool completed = gl::algorithm::pfs(
    graph,
    [](const auto& lhs, const auto& rhs) { // (2)!
        return lhs.vertex_id > rhs.vertex_id;
    },
    gl::algorithm::init_node_range<graph_type>(start_id), // (3)!
    gl::algorithm::default_visit_vertex_predicate(visited // (4)!
    [&](auto v, auto p) { // (5)!
        std::cout << "Priority visited vertex " << v << '\n';
        return true; // Continue search
    },
    gl::algorithm::default_enqueue_node_predicate<graph_type, true>(visited) // (6)!
);
```



1. Tracks discovered vertices.


2. Injects the comparator to order the search exploration. A min-heap based on vertex IDs.


3. Initializes the priority queue with the starting vertex. By default, this yields standard [**search\_nodes**](structgl_1_1algorithm_1_1search__node.md).


4. Predicate evaluated after popping the highest priority node. Accepts the entire `NodeType` to allow for snapshot inspections (like stale-node rejection).


5. The main visit callback. Returning `false` aborts the search.


6. Predicate determining if an adjacent vertex should be pushed into the priority queue.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|PQCmp|The comparator used to order elements within the priority queue.|Must be a `(NodeType, NodeType) -> bool` callable.|
|InitQueueRangeType|The container providing the initial roots to enqueue.|Must satisfy `std::ranges::forward_range`.|
|NodeType|The type of the node stored in the priority queue.|Extracted implicitly. Must be constructible from `(id_type, id_type)` unless `MakeNodeCallback` is provided.|
|VisitVertexPredicate|Decides if a popped node should be processed.|Must be one of:<br>- `(NodeType) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|VisitCallback|Executed when a vertex is officially visited.|Must be one of:<br>- `(id_type, id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|EnqueueNodePred|Decides if a node corresponding to an adjacent vertex should be pushed to the queue.|Must be one of:<br>- `(id_type, const edge_type&) -> decision` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|MakeNodeCallback|Constructs a custom `NodeType` before pushing to the queue.|Must be one of:<br>- `(id_type, id_type, const edge_type&) -> NodeType` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PreVisitCallback|Executed immediately before `VisitCallback`.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Executed after all adjacent edges are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `pq_cmp` The comparator instance used to determine priority (highest priority is popped first). 
* `initial_queue_content` A range of initial nodes to seed the priority queue. 
* `visit_vertex_pred` Predicate evaluated immediately after popping a node. If it returns `false`, the node is skipped (often used for late-rejection in Dijkstra). 
* `visit` Callback invoked when a vertex is officially visited. If it returns `false`, the entire PFS immediately aborts. 
* `enqueue_node_pred` Predicate evaluated for each outgoing edge. Returns a [**decision**](structgl_1_1algorithm_1_1decision.md):
    * `accept` to enqueue,
    * `reject` to skip,
    * `abort` to terminate the PFS entirely. 


* `make_node` Factory callback to construct a custom `NodeType` prior to enqueueing. Defaults to invoking the `NodeType(target_id, pred_id)` constructor. 
* `pre_visit` Hook executed immediately before the `visit` callback. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been evaluated. 



**Returns:**

`true` if the queue was exhausted naturally, `false` if the search was aborted early by a callback or predicate.    






        

<hr>



### function r\_dfs 

_A highly customizable, generic recursive Depth-First Search (DFS) algorithm engine._ 
```cpp
template<traits::c_graph G, traits::c_optional_predicate< typename G::id_type > VisitVertexPredicate, traits::c_optional_predicate< typename G::id_type, typename G::id_type > VisitCallback, traits::c_decision_predicate< typename G::id_type, const typename G::edge_type & > EnqueueNodePred, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
void r_dfs (...) 
```



This engine mirrors the iterative `dfs` behavior but utilizes the C++ call stack. It does not accept an initial range, but instead is kicked off for a specific root vertex. It does not return a boolean abort signal; logic flow must be managed by the injected callbacks.


#### Example Usage




```cpp
std::vector<bool> visited(graph.n_vertices(), false); // (1)!

gl::algorithm::r_dfs(
    graph,
    start_id, // (2)!
    gl::algorithm::no_root, // (3)!
    gl::algorithm::default_visit_vertex_predicate(visited), // (4)!
    [&](auto v, auto p) { // (5)!
        std::cout << "Recursively visiting vertex " << v << '\n';
        return true;
    },
    gl::algorithm::default_enqueue_node_predicate<graph_type, false>(visited) // (6)!
);
```



1. Tracks discovered vertices.


2. The ID of the starting vertex for the recursion.


3. Indicates that the starting vertex has no predecessor.


4. Predicate evaluated upon entering the recursive call to prevent duplicate processing.


5. The main visit callback.


6. Predicate evaluating whether to recursively traverse into the target vertex.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|VisitVertexPredicate|Type of the callable deciding if the current vertex should be processed.|Must be one of:<br>- An `(id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|VisitCallback|Type of the callable executed when the vertex is officially visited.|Must be one of:<br>- An `(id_type, id_type) -> bool` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|EnqueueNodePred|Type of the callable deciding if a node corresponding to an adjacent vertex should be recursed into.|Must be one of:<br>- An `(id_type, const edge_type&) -> decision` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PreVisitCallback|Type of the callable executed immediately before `VisitCallback`.|Must be one of:<br>- An `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges are evaluated.|Must be one of:<br>- An `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `vertex_id` The ID of the vertex currently being visited. 
* `pred_id` The ID of the predecessor vertex. 
* `visit_vertex_pred` Predicate evaluated immediately upon entry. If it returns `false`, recursion returns early. 
* `visit` Callback invoked when a vertex is officially visited. 
* `enqueue_node_pred` Predicate evaluated for each outgoing edge. If `true`, the target is recursed into. 
* `pre_visit` Hook executed immediately before the `visit` callback. 
* `post_visit` Hook executed after returning from all adjacent recursive calls.    





        

<hr>



### function reconstruct\_path 

_Reconstructs the sequence of vertices forming a path to a specific target._ 
```cpp
template<traits::c_id_type IdType, traits::c_random_access_range_of< IdType > IdRange>
std::vector< IdType > reconstruct_path (...) 
```



This utility walks backward through a predecessor map, starting from the `vertex_id` until it reaches the root vertex (a vertex that is its own predecessor).


#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|IdType|The type of the vertex IDs.|Must satisfy the [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) concept.|
|IdRange|The type of the random-access range containing the predecessor map.|Must satisfy the [**c\_random\_access\_range\_of**](gl_concepts.md#gl-traits-c-random-access-range-of) concept for `IdType`.|








**Parameters:**


* `predecessor_map` The predecessor map generated by a traversal algorithm (e.g., BFS, DFS, Dijkstra). 
* `vertex_id` The target vertex ID to reconstruct the path for. 



**Returns:**

A `std::vector` containing the sequence of vertex IDs from the source to the target. 




**Exception:**


* `std::invalid_argument` If the target `vertex_id` is unreachable (its predecessor is invalid).    





        

<hr>



### function recursive\_depth\_first\_search 

_Executes a concrete recursive Depth-First Search (DFS) traversal over the graph._ 
```cpp
template<result_discriminator Result, traits::c_graph G, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
result_type< Result, predecessors_map< G > > recursive_depth_first_search (...) 
```



This function relies on the generic [**r\_dfs**](group__GL-Algorithm.md#function-r_dfs) template. Instead of a heap-allocated stack, it utilizes the C++ call stack to navigate the graph.



> [!WARNING] Call Stack Depth
>
>
> While recursive DFS can be slightly faster due to the lack of heap allocations, it is vulnerable to Stack Overflow errors on extremely deep graphs (e.g., long linear paths). 
>
>



#### Example Usage




```cpp
gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>( // (1)!
    graph,
    start_id, // (2)!
    [](auto v) { std::cout << "Entering subtree of: " << v << '\n'; }, // (3)!
    [](auto v) { std::cout << "Exiting subtree of: " << v << '\n'; }   // (4)!
);
```



1. Execution purely for side-effects. Uses the [**noret**](group__GL-Algorithm.md#enum-result_discriminator) discriminator to completely compile away the predecessor map allocations.


2. Initiates the recursion from `start_id`.


3. Hook triggered before descending into the current vertex's adjacent neighbors.


4. Hook triggered after returning from all recursive calls for the current vertex.



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends entirely on the underlying representation of `GraphType`:
> * **Adjacency List Representations**: \(O(|V| + |E|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over adjacent vertices requires scanning the entire \(|V|\)-length matrix row. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|[**Discriminator**](group__GL-Algorithm.md#enum-result_discriminator) dictating if the algorithm should return a predecessor map (`ret`) or `void` (`noret`).|Must be a valid [**result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator) enum value.|
|G|The type of the graph being traversed.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|PreVisitCallback|Type of the callable executed immediately before a vertex is officially visited.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges of a vertex are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The graph to traverse. 
* `root_vertex_id` The starting vertex for the search. Defaults to [**no\_root**](group__GL-Algorithm.md#variable-no_root) to traverse the entire graph. 
* `pre_visit` Hook executed immediately before the internal visit logic. 
* `post_visit` Hook executed after returning from all recursive calls for the current vertex. 



**Returns:**

A [**predecessors\_map**](group__GL-Algorithm.md#typedef-predecessors_map) mapping each visited vertex to its parent if `Result == ret`. Returns `void` otherwise.    






        

<hr>



### function topological\_sort 

_Computes a topological ordering of the vertices in a Directed Acyclic Graph (DAG)._ 
```cpp
template<traits::c_directed_graph G, traits::c_optional_callback< void, typename G::id_type > PreVisitCallback, traits::c_optional_callback< void, typename G::id_type > PostVisitCallback>
std::optional< std::vector< typename G::id_type > > topological_sort (...) 
```



This implementation relies on Kahn's Algorithm. It utilizes the generic [**bfs**](group__GL-Algorithm.md#function-bfs) template, seeding the queue with all vertices that have an in-degree of 0. As vertices are processed, the in-degrees of adjacent vertices are iteratively decremented.


If the final sorted order does not contain all vertices in the graph, it indicates the presence of a cycle, meaning the graph is not a DAG.


#### Example Usage




```cpp
if (auto top_order = gl::algorithm::topological_sort(graph)) { // (1)!
    std::cout << "Topological Order: "
              << gl::io::range_formatter(top_order.value()) // (2)!
              << '\n';
}
else {
    std::cout << "Graph contains a cycle!\n";
}
```



1. Attempts to compute the ordering. Fails and returns `std::nullopt` if a cycle is detected.


2. Prints the topologically sorted vector of vertex IDs using the [**range\_formatter**](structgl_1_1io_1_1range__formatter.md) helper.



> [!INFO] Algorithmic Complexity
>
>
> The time complexity depends entirely on the underlying representation of `GraphType`:
> * **Adjacency List Representations**: \(O(|V| + |E|)\)
>     * _Includes:_ [**list\_t**](structgl_1_1repr_1_1list__t.md) and [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md).
>
>
> * **Adjacency Matrix Representations**: \(O(|V|^2)\)
>     * _Includes:_ [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) and [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md).
>     * _Note:_ Iterating over adjacent vertices requires scanning the entire \(|V|\)-length matrix row. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the directed graph being traversed.|Must satisfy the [**c\_directed\_graph**](gl_concepts.md#gl-traits-c-directed-graph) concept.|
|PreVisitCallback|Type of the callable executed immediately before a vertex is pushed into the sort order.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|
|PostVisitCallback|Type of the callable executed after all adjacent edges of a vertex are evaluated.|Must be one of:<br>- `(id_type) -> void` callable<br>- An [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)|








**Parameters:**


* `graph` The directed graph to evaluate. 
* `pre_visit` Hook executed immediately before the internal sort logic processes a vertex. 
* `post_visit` Hook executed after all adjacent edges of the current vertex have been evaluated and their in-degrees decremented. 



**Returns:**

An `std::optional` containing a vector of vertex IDs in topological order or `std::nullopt` if the graph is not a DAG.    






        

<hr>



### function vertex\_heap\_prim\_mst 

_Computes the Minimum Spanning Tree (MST) of an undirected graph using Prim's algorithm with a vertex-based array heap._ 
```cpp
template<traits::c_undirected_graph G>
mst_descriptor< G > vertex_heap_prim_mst (...) 
```



This variation maintains a heap of vertex IDs based on their minimum known connection cost. Because standard C++ heaps do not support a `decrease_key` operation, this implementation dynamically rebuilds the heap (`std::make_heap`) at the end of each iteration to reflect updated distances.


#### Example Usage




```cpp
auto mst = gl::algorithm::vertex_heap_prim_mst(graph, start_id); // (1)!
std::cout << "Total MST Weight: " << mst.weight
          << "\nMST Edges: " << gl::io::set_formatter(mst.edges) << '\n';
```



1. Computes the MST starting from the given `start_id`. Highly optimal for dense matrix graphs.



> [!INFO] Algorithmic Complexity
>
>
> Due to rebuilding the heap ( \(O(|V|)\)) up to \(|V|\) times, combined with evaluating every edge, the strict time complexity is \(O(|V|^2 + |E|)\):
> * **Adjacency Matrix Representations**: \(O(|V|^2)\)
>     * _Note:_ Since matrix models inherently represent simple graphs (where \(|E| \le |V|^2\)), the complexity strictly simplifies to \(O(|V|^2)\). This makes the vertex heap approach highly suitable for dense graphs.
>
>
> * **Adjacency List Representations**: \(O(|V|^2 + |E|)\)
>     * _Note:_ For multigraphs, the edge count \(|E|\) can exceed \(|V|^2\), meaning the edge traversal phase will dictate the overall performance. 
>
>
>
>
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the undirected graph being traversed.|Must satisfy the [**c\_undirected\_graph**](gl_concepts.md#gl-traits-c-undirected-graph) concept and its [**distance type**](group__GL-Core.md#typedef-vertex_distance_type) must satisfy [**c\_has\_numeric\_limits\_max**](gl_concepts.md#gl-traits-c-has-numeric-limits-max).|








**Parameters:**


* `graph` The undirected graph to evaluate. 
* `root_id` The starting vertex ID for the MST calculation. Defaults to the graph's `initial_id` if `invalid_id` is passed. 



**Returns:**

A [**mst\_descriptor**](structgl_1_1algorithm_1_1mst__descriptor.md) containing the accumulated minimum weight and the sequence of edges forming the tree. 






#### See Also




* [**edge\_heap\_prim\_mst**](group__GL-Algorithm.md#function-edge_heap_prim_mst) For the vertex-heap variant of the Prim's MST finding algorithm.    





        

<hr>

------------------------------


