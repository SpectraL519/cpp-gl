

# Group HGL-Algorithm



[**Modules**](modules.md) **>** [**HGL-Algorithm**](group__HGL-Algorithm.md)



_Generic hypergraph algorithms, search templates, and related utilities._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**hgl::algorithm::search\_node**](structhgl_1_1algorithm_1_1search__node.md) &lt;H&gt;<br>_Represents an active node in a search container (e.g., a BFS queue or DFS stack) for hypergraph traversals._  |
| struct | [**hgl::algorithm::traversal\_policy**](structhgl_1_1algorithm_1_1traversal__policy.md) &lt;H, Dir&gt;<br>_Policy defining how to extract incident hyperedges and target vertices during traversal._  |
| struct | [**hgl::algorithm::traversal\_policy&lt; H, Dir &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01Dir_01_4.md) &lt;H, Dir&gt;<br>_Traversal policy specialization for undirected hypergraphs._  |
| struct | [**hgl::algorithm::traversal\_policy&lt; H, traversal\_direction::backward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1backward_01_4.md) &lt;H&gt;<br>_Traversal policy specialization for backward searches on BF-directed hypergraphs._  |
| struct | [**hgl::algorithm::traversal\_policy&lt; H, traversal\_direction::forward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1forward_01_4.md) &lt;H&gt;<br>_Traversal policy specialization for forward searches on BF-directed hypergraphs._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**gl::algorithm::decision**](structgl_1_1algorithm_1_1decision.md) | [**decision**](#typedef-decision)  <br>_Represents a generic tri-state decision for control flow._  |
| typedef [**gl::algorithm::empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md) | [**empty\_callback**](#typedef-empty_callback)  <br>_A tag type used to explicitly indicate the absence of a callback function._  |
| typedef [**gl::algorithm::no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md) | [**no\_root\_t**](#typedef-no_root_t)  <br>_Tag type providing an implicit conversion to the appropriate_ `no_root_v` _for any numeric ID type._ |
| typedef [**gl::algorithm::non\_void\_result\_type**](group__GL-Algorithm.md#typedef-non_void_result_type)&lt; Result, ResultType &gt; | [**non\_void\_result\_type**](#typedef-non_void_result_type)  <br>_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`std::monostate` _._ |
| typedef [**gl::algorithm::result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator) | [**result\_discriminator**](#typedef-result_discriminator)  <br>_Tag used to statically dictate whether an algorithm should return a constructed result or execute purely for side effects._  |
| typedef [**gl::algorithm::result\_type**](group__GL-Algorithm.md#typedef-result_type)&lt; Result, ResultType &gt; | [**result\_type**](#typedef-result_type)  <br>_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`void` _._ |
| typedef std::vector&lt; search\_node&lt; H &gt; &gt; | [**search\_tree**](#typedef-search_tree)  <br>_A flat, index-mapped representation of a hypergraph search tree._  |
| enum bool | [**traversal\_direction**](#enum-traversal_direction)  <br>_Specifies the direction of traversal for_ _BF-directed_ _hypergraphs._ |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr no\_root\_t | [**no\_root**](#variable-no_root)  = `gl::algorithm::no_root`<br>_Global constant representing the absence of a root vertex._  |
|  constexpr IdType | [**no\_root\_v**](#variable-no_root_v)  &lt;IdType&gt; = `gl::algorithm::no_root_v<IdType>`<br>_Constant representing the absence of a root vertex ID for a specific ID type._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**backward\_bfs**](#function-backward_bfs) (...) <br>_Executes a Breadth-First B-Search (B-BFS) traversal over a bf-directed hypergraph._  |
|  result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**backward\_dfs**](#function-backward_dfs) (...) <br>_Executes a Depth-First B-Search (B-DFS) traversal over a bf-directed hypergraph._  |
|  bool | [**bfs**](#function-bfs) (...) <br>_A highly customizable, generic Breadth-First Search (BFS) algorithm engine for hypergraphs._  |
|  auto | [**blocking\_traverse\_hyperedge\_predicate**](#function-blocking_traverse_hyperedge_predicate) (std::vector&lt; size\_type &gt; & counter\_map) <br>_Generates a lambda predicate that blocks hyperedge traversal until its associated counter reaches zero._  |
|  result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**breadth\_first\_search**](#function-breadth_first_search) (...) <br>_Executes a concrete Breadth-First Search (BFS) traversal over the hypergraph._  |
|  size\_type | [**corank**](#function-corank) (const traits::c\_hypergraph auto & hypergraph) noexcept<br>_Calculates the corank (minimum size of any hyperedge) of a hypergraph._  |
|  auto | [**default\_enqueue\_predicate**](#function-default_enqueue_predicate) (std::vector&lt; bool &gt; & visited\_v) <br>_Generates a default lambda predicate that checks if a node corresponding to an adjacent vertex should be enqueued._  |
|  auto | [**default\_traverse\_hyperedge\_predicate**](#function-default_traverse_hyperedge_predicate) (std::vector&lt; bool &gt; & visited\_he) <br>_Generates a default lambda predicate that tracks traversed hyperedges to prevent redundant exploration._  |
|  auto | [**default\_visit\_callback**](#function-default_visit_callback) (...) <br>_Generates a default lambda callback executed upon officially visiting a vertex._  |
|  auto | [**default\_visit\_predicate**](#function-default_visit_predicate) (std::vector&lt; bool &gt; & visited\_v) <br>_Generates a default lambda predicate that checks if a popped search node has already been visited._  |
|  result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**depth\_first\_search**](#function-depth_first_search) (...) <br>_Executes a concrete Depth-First Search (DFS) traversal over the hypergraph._  |
|  bool | [**dfs**](#function-dfs) (...) <br>_A highly customizable, generic Depth-First Search (DFS) algorithm engine for hypergraphs._  |
|  result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**forward\_bfs**](#function-forward_bfs) (...) <br>_Executes a Breadth-First F-Search (F-BFS) traversal over a bf-directed hypergraph._  |
|  result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**forward\_dfs**](#function-forward_dfs) (...) <br>_Executes a Depth-First F-Search (F-DFS) traversal over a bf-directed hypergraph._  |
|  std::vector&lt; search\_node&lt; H &gt; &gt; | [**init\_node\_range**](#function-init_node_range) (typename H::id\_type root\_vertex\_id) <br>_Initializes a container with a starting set of root search nodes._  |
|  non\_void\_result\_type&lt; Result, search\_tree&lt; H &gt; &gt; | [**init\_search\_tree**](#function-init_search_tree) (const H & hypergraph) <br>_Initializes a search tree based on the static result discriminator._  |
|  bool | [**is\_head\_uniform**](#function-is_head_uniform) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph, const size\_type k) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is head-$k$-uniform (all hyperedges have a head size of $k$)._ |
|  bool | [**is\_head\_uniform**](#function-is_head_uniform) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is head-uniform (all hyperedges have the exact same head size)._ |
|  bool | [**is\_in\_regular**](#function-is_in_regular) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph, const size\_type k) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is in-$k$-regular (all vertices have an in-degree of $k$)._ |
|  bool | [**is\_in\_regular**](#function-is_in_regular) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is in-regular (all vertices have the same in-degree)._ |
|  bool | [**is\_out\_regular**](#function-is_out_regular) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph, const size\_type k) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is out-$k$-regular (all vertices have an out-degree of $k$)._ |
|  bool | [**is\_out\_regular**](#function-is_out_regular) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is out-regular (all vertices have the same out-degree)._ |
|  bool | [**is\_reachable**](#function-is_reachable) (const traits::c\_search\_tree auto & tree, traits::c\_id\_type auto vertex\_id) noexcept<br>_Checks if a specific vertex was reached during the traversal._  |
|  bool | [**is\_regular**](#function-is_regular) (const traits::c\_hypergraph auto & hypergraph, const size\_type k) noexcept<br>_Evaluates whether the given hypergraph is $k$-regular (all vertices have a degree of $k$)._  |
|  bool | [**is\_regular**](#function-is_regular) (const traits::c\_hypergraph auto & hypergraph) noexcept<br>_Evaluates whether the given hypergraph is structurally regular (all vertices have the same degree)._  |
|  bool | [**is\_tail\_uniform**](#function-is_tail_uniform) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph, const size\_type k) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is tail-$k$-uniform (all hyperedges have a tail size of $k$)._ |
|  bool | [**is\_tail\_uniform**](#function-is_tail_uniform) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Evaluates whether the given_ _BF-directed_ _hypergraph is tail-uniform (all hyperedges have the exact same tail size)._ |
|  bool | [**is\_uniform**](#function-is_uniform) (const traits::c\_hypergraph auto & hypergraph, const size\_type k) noexcept<br>_Evaluates whether the given hypergraph is $k$-uniform (all hyperedges have a size of $k$)._  |
|  bool | [**is\_uniform**](#function-is_uniform) (const traits::c\_hypergraph auto & hypergraph) noexcept<br>_Evaluates whether the given hypergraph is structurally uniform (all hyperedges have the exact same size)._  |
|  size\_type | [**max\_degree**](#function-max_degree) (const traits::c\_hypergraph auto & hypergraph) noexcept<br>_Calculates the maximum degree among all vertices in a hypergraph._  |
|  size\_type | [**max\_head\_size**](#function-max_head_size) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the maximum head size among all hyperedges in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**max\_in\_degree**](#function-max_in_degree) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the maximum in-degree among all vertices in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**max\_out\_degree**](#function-max_out_degree) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the maximum out-degree among all vertices in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**max\_tail\_size**](#function-max_tail_size) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the maximum tail size among all hyperedges in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**min\_degree**](#function-min_degree) (const traits::c\_hypergraph auto & hypergraph) noexcept<br>_Calculates the minimum degree among all vertices in a hypergraph._  |
|  size\_type | [**min\_head\_size**](#function-min_head_size) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the minimum head size among all hyperedges in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**min\_in\_degree**](#function-min_in_degree) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the minimum in-degree among all vertices in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**min\_out\_degree**](#function-min_out_degree) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the minimum out-degree among all vertices in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**min\_tail\_size**](#function-min_tail_size) (const traits::c\_bf\_directed\_hypergraph auto & hypergraph) noexcept<br>_Calculates the minimum tail size among all hyperedges in a_ _BF-directed_ _hypergraph._ |
|  size\_type | [**rank**](#function-rank) (const traits::c\_hypergraph auto & hypergraph) noexcept<br>_Calculates the rank (maximum size of any hyperedge) of a hypergraph._  |




























## Detailed Description


This module provides hypergraph-specific algorithmic templates and implementations (such as forward and backward searches) designed to natively navigate the complex, high-order structure of hyperedges. 


    
## Public Types Documentation




### typedef decision 

_Represents a generic tri-state decision for control flow._ 
```cpp
using hgl::algorithm::decision = typedef gl::algorithm::decision;
```



#### See Also




* [**gl::algorithm::decision**](structgl_1_1algorithm_1_1decision.md) : For the full type definition. 





        

<hr>



### typedef empty\_callback 

_A tag type used to explicitly indicate the absence of a callback function._ 
```cpp
using hgl::algorithm::empty_callback = typedef gl::algorithm::empty_callback;
```



#### See Also




* [**gl::algorithm::empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md) : For a more detailed description. 





        

<hr>



### typedef no\_root\_t 

_Tag type providing an implicit conversion to the appropriate_ `no_root_v` _for any numeric ID type._
```cpp
using hgl::algorithm::no_root_t = typedef gl::algorithm::no_root_t;
```





**See also:** [**gl::algorithm::no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md) 



        

<hr>



### typedef non\_void\_result\_type 

_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`std::monostate` _._
```cpp
template<result_discriminator Result, typename ResultType>
using hgl::algorithm::non_void_result_type = typedef gl::algorithm::non_void_result_type<Result, ResultType>;
```





**See also:** [**gl::algorithm::non\_void\_result\_type**](group__GL-Algorithm.md#typedef-non_void_result_type) : [**For**](group__HGL-Core.md#variable-invalid_id_v) [**the**](group__HGL-Core.md#variable-invalid_id_v) [**full**](group__HGL-Core.md#variable-invalid_id_v) type [**definition**](group__HGL-Core.md#variable-invalid_id_v). 



        

<hr>



### typedef result\_discriminator 

_Tag used to statically dictate whether an algorithm should return a constructed result or execute purely for side effects._ 
```cpp
using hgl::algorithm::result_discriminator = typedef gl::algorithm::result_discriminator;
```



#### See Also




* [**gl::algorithm::result\_discriminator**](group__GL-Algorithm.md#enum-result_discriminator) : For the full type definition. 





        

<hr>



### typedef result\_type 

_Resolves to the specified_ `ResultType` _if_`Result` _is_`ret` _, otherwise resolves to_`void` _._
```cpp
template<result_discriminator Result, typename ResultType>
using hgl::algorithm::result_type = typedef gl::algorithm::result_type<Result, ResultType>;
```



#### See Also




* [**gl::algorithm::result\_type**](group__GL-Algorithm.md#typedef-result_type) : For the full type definition. 





        

<hr>



### typedef search\_tree 

_A flat, index-mapped representation of a hypergraph search tree._ 
```cpp
template<traits::c_hypergraph H>
using hgl::algorithm::search_tree = typedef std::vector<search_node<H> >;
```



The $i$-th element corresponds to the vertex with `id == i`. The tree topology is formed implicitly, as each [**search\_node**](structhgl_1_1algorithm_1_1search__node.md) stores the ID of its predecessor and the connecting hyperedge, enabling \(O(1)\) lookups and and \(O(\vert V \vert)\) path reconstruction.




**Template parameters:**


* `H` The type of the hypergraph being searched. 




        

<hr>



### enum traversal\_direction 

_Specifies the direction of traversal for_ _BF-directed_ _hypergraphs._
```cpp
enum traversal_direction {
    forward,
    backward
};
```




> [!IMPORTANT] API Note
>
>
>
> * To ensure API simplicity, the `traversal_direction` is used for undirected hypergraphs as well by the generic traversal templates. However, due to the structural nature of undirected hypergraphs, both direction values implicitly yield the exact same traversal pattern for undirected hypergraphs.
> * The library utilizes C++20's `using enum` feature for the `traversal_direction` enum type, allowing you to access these tags directly via `hgl::algorithm::forward` and `hgl::algorithm::backward`. 
>
>
>
>



        

<hr>
## Public Attributes Documentation




### variable no\_root 

_Global constant representing the absence of a root vertex._ 
```cpp
constexpr no_root_t hgl::algorithm::no_root = gl::algorithm::no_root;
```



#### See Also




* [**gl::algorithm::no\_root**](group__GL-Algorithm.md#variable-no_root) 





        

<hr>



### variable no\_root\_v 

_Constant representing the absence of a root vertex ID for a specific ID type._ 
```cpp
template<traits::c_id_type IdType>
constexpr IdType hgl::algorithm::no_root_v = gl::algorithm::no_root_v<IdType>;
```



#### See Also




* [**gl::algorithm::no\_root\_v**](group__GL-Algorithm.md#variable-no_root_v) 





        

<hr>
## Public Functions Documentation




### function backward\_bfs 

_Executes a Breadth-First B-Search (B-BFS) traversal over a bf-directed hypergraph._ 
```cpp
template<result_discriminator Result, traits::c_bf_directed_hypergraph H, traits::c_forward_range_of< typename H::id_type > RootRange, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
result_type< Result, search_tree< H > > backward_bfs (...) 
```



This algorithm implements B-reachability semantics using Breadth-First Search for BF-directed hypergraphs. Unlike a standard traversal (where reaching a single tail vertex is sufficient to traverse an outgoing hyperedge), a backward search (B-search) uses a blocking predicate. A hyperedge is only traversed, and its head vertices enqueued, after **all** of its tail (source) vertices have been visited.


#### Example Usage




```cpp
std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
auto search_tree = hgl::algorithm::backward_bfs(hypergraph, roots); // (1)!

hgl::algorithm::backward_bfs<hgl::algorithm::noret>( // (2)!
    hypergraph,
    roots,
    [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
    [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
);
```



1. Executes a B-BFS returning a search tree mapped to the components B-reachable from the specified roots.


2. Executes a B-BFS purely for side-effects (callbacks) without allocating memory for a search tree.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`).|Must be a valid [**result\_discriminator**](group__HGL-Algorithm.md#typedef-result_discriminator) enum value.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_bf\_directed\_hypergraph**](hgl_concepts.md#hgl-traits-c-bf-directed-hypergraph) concept.|
|RootRange|The type of the container providing the initial roots to enqueue.|Must satisfy [**c\_forward\_range\_of**](gl_concepts.md#gl-traits-c-forward-range-of) over the hypergraph's `id_type`.|
|PreVisitCallback|Type of the callable executed immediately before visiting a vertex.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The bf-directed hypergraph to traverse. 
* `root_vertices` A range of initial vertex IDs to start the search from. 
* `pre_visit` Hook executed immediately before visiting the vertex. 
* `post_visit` Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated. 



**Returns:**

A [**search\_tree**](group__HGL-Algorithm.md#typedef-search_tree) if `Result == ret`, otherwise nothing (`void`).    






        

<hr>



### function backward\_dfs 

_Executes a Depth-First B-Search (B-DFS) traversal over a bf-directed hypergraph._ 
```cpp
template<result_discriminator Result, traits::c_bf_directed_hypergraph H, traits::c_forward_range_of< typename H::id_type > RootRange, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
result_type< Result, search_tree< H > > backward_dfs (...) 
```



This algorithm implements B-reachability semantics using a stack-based Depth-First Search for BF-directed hypergraphs. Unlike a standard traversal (where reaching a single tail vertex is sufficient to traverse an outgoing hyperedge), a backward search (B-search) uses a blocking predicate. A hyperedge is only traversed, and its head vertices enqueued, after **all** of its tail (source) vertices have been visited.


#### Example Usage




```cpp
std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
auto search_tree = hgl::algorithm::backward_dfs(hypergraph, roots); // (1)!

hgl::algorithm::backward_dfs<hgl::algorithm::noret>( // (2)!
    hypergraph,
    roots,
    [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
    [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
);
```



1. Executes a B-DFS returning a search tree mapped to the components B-reachable from the specified roots.


2. Executes a B-DFS purely for side-effects (callbacks) without allocating memory for a search tree.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`).|Must be a valid [**result\_discriminator**](group__HGL-Algorithm.md#typedef-result_discriminator) enum value.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_bf\_directed\_hypergraph**](hgl_concepts.md#hgl-traits-c-bf-directed-hypergraph) concept.|
|RootRange|The type of the container providing the initial roots to enqueue.|Must satisfy [**c\_forward\_range\_of**](gl_concepts.md#gl-traits-c-forward-range-of) over the hypergraph's `id_type`.|
|PreVisitCallback|Type of the callable executed immediately before officially visiting a vertex.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The bf-directed hypergraph to traverse. 
* `root_vertices` A range of initial vertex IDs to start the search from. 
* `pre_visit` Hook executed immediately before officially visiting the vertex. 
* `post_visit` Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated. 



**Returns:**

A [**search\_tree**](group__HGL-Algorithm.md#typedef-search_tree) if `Result == ret`, otherwise nothing (`void`).    






        

<hr>



### function bfs 

_A highly customizable, generic Breadth-First Search (BFS) algorithm engine for hypergraphs._ 
```cpp
template<traversal_direction Dir, traits::c_hypergraph H, traits::c_forward_range_of< search_node< H > > InitQueueRangeType, traits::c_optional_predicate< const search_node< H > & > VisitPredicate, traits::c_optional_predicate< const search_node< H > & > VisitCallback, traits::c_optional_decision_predicate< typename H::id_type, typename H::id_type > TraverseHePredicate, traits::c_decision_predicate< const search_node< H > & > EnqueuePredicate, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
bool bfs (...) 
```



This template provides the strict structural execution of a queue-based Breadth-First Search over a hypergraph's topology. Because a hypergraph traversal inherently requires a two-step expansion (from a vertex to its incident hyperedges, and then to the adjacent vertices), this engine exposes specific hooks for both steps. Concrete algorithms are constructed by injecting logic into the provided callback and predicate hooks.


#### Example Usage




```cpp
std::vector<bool> visited(hypergraph.n_vertices(), false); // (1)!

bool completed = hgl::algorithm::bfs(
    hypergraph,
    hgl::algorithm::init_node_range<H>(start_id), // (2)!
    [&](const auto& node) { return not visited[node.vertex_id]; }, // (3)!
    [&](const auto& node) { // (4)!
        visited[node.vertex_id] = true;
        std::cout << "Visited vertex " << node.vertex_id << '\n';
        return true; // Continue search
    },
    hgl::empty_callback{}, // (5)!
    [&](const auto& tgt_node) { return not visited[tgt_node.vertex_id]; } // (6)!
);
```



1. Provide an external state array to track the visited vertices.


2. Initialize the search queue with a root [**search node**](structhgl_1_1algorithm_1_1search__node.md) representing the starting point.


3. The _visit predicate_ ensures vertices are not processed multiple times.


4. The concrete vertex visiting logic - marks the vertex as visited and logs it to the console.


5. Traverse all hyperedges unconditionally.


6. The _enqueue predicate_ filters out already visited adjacent vertices before they enter the queue.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Dir|The [**traversal direction**](group__HGL-Algorithm.md#enum-traversal_direction) (i.e., `forward` or `backward`). Relevant only for BF-directed hypergraphs.|Defaults to `forward`.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) concept.|
|InitQueueRangeType|A forward range of `search_node <H>` used to prime the BFS queue.|Must be a _forward range_ of [**search nodes**](structhgl_1_1algorithm_1_1search__node.md).|
|VisitPredicate|Type of the callable deciding if a popped node should be processed.|Must be one of:<br>- A `(const  search_node <H>&) -> bool` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|VisitCallback|Type of the callable executed when a vertex is officially visited.|Must be one of:<br>- A `(const  search_node <H>&) -> bool` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|TraverseHePredicate|Type of the callable deciding if an incident hyperedge should be traversed.|Must be one of:<br>- An `(id_type, id_type) -> decision` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|EnqueuePredicate|Type of the callable deciding if a target vertex should be enqueued via a specific hyperedge.|Must be one of:<br>- A `(const  search_node <H>&) -> decision` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PreVisitCallback|Type of the callable executed immediately before `VisitCallback`.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The hypergraph to traverse. 
* `initial_queue_content` The initial set of search nodes to begin the traversal from. 
* `visit_pred` Predicate to filter nodes immediately after popping them from the queue. 
* `visit` Primary callback for node processing. 
* `traverse_he_pred` Predicate to determine if an incident hyperedge should be traversed. Returns a [**decision**](group__HGL-Algorithm.md#typedef-decision):
    * `accept` to traverse the hyperedge,
    * `reject` to skip the hyperedge,
    * `abort` to terminate the BFS entirely. 


* `enqueue_pred` Predicate to determine if an adjacent vertex should be queued via a hyperedge. Returns a [**decision**](group__HGL-Algorithm.md#typedef-decision):
    * `accept` to enqueue the vertex's search node,
    * `reject` to skip the node,
    * `abort` to terminate the BFS entirely. 


* `pre_visit` Callback executed prior to the primary visit logic. 
* `post_visit` Callback executed after all adjacent elements of the current node have been processed. 



**Returns:**

`true` if the search completed normally, `false` if it was explicitly aborted via a callback.    






        

<hr>



### function blocking\_traverse\_hyperedge\_predicate 

_Generates a lambda predicate that blocks hyperedge traversal until its associated counter reaches zero._ 
```cpp
auto blocking_traverse_hyperedge_predicate (
    std::vector< size_type > & counter_map
) 
```



This predicate is particularly useful in topological sorting or multi-dependency algorithms for _BF-directed_ hypergraphs, where a hyperedge should only be evaluated once all of its dependencies (e.g., all vertices in its tail) have been saturated or visited.




**Parameters:**


* `counter_map` A reference to an array of size or dependency counters mapped to hyperedge IDs. 



**Returns:**

A callable predicate returning a [**decision**](group__HGL-Algorithm.md#typedef-decision) (`accept` if the decremented counter reaches 0, `reject` otherwise). 





        

<hr>



### function breadth\_first\_search 

_Executes a concrete Breadth-First Search (BFS) traversal over the hypergraph._ 
```cpp
template<result_discriminator Result, traits::c_hypergraph H, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
result_type< Result, search_tree< H > > breadth_first_search (...) 
```



This function utilizes the generic [**bfs**](group__HGL-Algorithm.md#function-bfs) template to perform a standard queue-based traversal. It automatically manages the visited states (for both vertices and hyperedges), search tree tracking, and queue initialization.


If a specific `root_vertex_id` is provided, the algorithm explores only the connected component reachable from that root. If `no_root` is used, it iteratively ensures that every disconnected component in the entire hypergraph is fully traversed.


#### Example Usage




```cpp
auto search_tree
    = hgl::algorithm::breadth_first_search(hypergraph, start_id); // (1)!

hgl::algorithm::breadth_first_search<hgl::algorithm::noret>( // (2)!
    hypergraph,
    hgl::algorithm::no_root, // (3)!
    [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
    [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
);
```



1. Executes a standard BFS returning a search tree mapped to the components reachable from `start_id`.


2. Executes a BFS purely for side-effects (callbacks) without allocating memory for a search tree.


3. Passing `no_root` forces the algorithm to iterate over all vertices, ensuring disjoint components are traversed.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`).|Must be a valid [**result\_discriminator**](group__HGL-Algorithm.md#typedef-result_discriminator) enum value.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) concept.|
|PreVisitCallback|Type of the callable executed immediately before visiting a vertex.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The hypergraph to traverse. 
* `root_vertex_id` The ID of the vertex to start the search from. If `no_root`, searches the entire hypergraph. 
* `pre_visit` Hook executed immediately before visiting the vertex. 
* `post_visit` Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated. 



**Returns:**

A [**search\_tree**](group__HGL-Algorithm.md#typedef-search_tree) if `Result == ret`, otherwise nothing (`void`).    






        

<hr>



### function corank 

_Calculates the corank (minimum size of any hyperedge) of a hypergraph._ 
```cpp
size_type corank (
    const traits::c_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function default\_enqueue\_predicate 

_Generates a default lambda predicate that checks if a node corresponding to an adjacent vertex should be enqueued._ 
```cpp
template<traits::c_hypergraph H, bool AsResult>
auto default_enqueue_predicate (
    std::vector< bool > & visited_v
) 
```





**Template parameters:**


* `H` The type of the hypergraph. 
* `AsResult` If `true`, the generated predicate returns a [**decision**](group__HGL-Algorithm.md#typedef-decision) instead of a raw boolean. 



**Parameters:**


* `visited_v` A reference to the boolean array tracking visited vertices. 



**Returns:**

A callable predicate that returns `true` (or `decision::accept`) if the adjacent vertex has not been visited. 





        

<hr>



### function default\_traverse\_hyperedge\_predicate 

_Generates a default lambda predicate that tracks traversed hyperedges to prevent redundant exploration._ 
```cpp
auto default_traverse_hyperedge_predicate (
    std::vector< bool > & visited_he
) 
```



Records hyperedges as they are encountered and rejects any that have already been traversed during the search.




**Parameters:**


* `visited_he` A reference to the boolean array tracking visited hyperedges. 



**Returns:**

A callable predicate returning a [**decision**](group__HGL-Algorithm.md#typedef-decision) (`accept` if not previously visited, `reject` if already visited). 





        

<hr>



### function default\_visit\_callback 

_Generates a default lambda callback executed upon officially visiting a vertex._ 
```cpp
template<traits::c_hypergraph H, result_discriminator Result>
auto default_visit_callback (...) 
```



Marks the vertex as visited. If `Result == ret`, it also records the search node into the provided search tree.




**Template parameters:**


* `H` The type of the hypergraph. 
* `Result` The compilation tag dictating whether to populate the search tree. 



**Parameters:**


* `visited_v` A reference to the boolean array tracking visited vertices. 
* `pred_map` A reference to the search tree being populated (or a dummy if `Result == noret`). 



**Returns:**

A callable callback returning `true` to unconditionally continue the traversal.    





        

<hr>



### function default\_visit\_predicate 

_Generates a default lambda predicate that checks if a popped search node has already been visited._ 
```cpp
template<traits::c_hypergraph H>
auto default_visit_predicate (
    std::vector< bool > & visited_v
) 
```





**Template parameters:**


* `H` The type of the hypergraph. 



**Parameters:**


* `visited_v` A reference to the boolean array tracking visited vertices. 



**Returns:**

A callable predicate that returns `true` if the vertex in the node has not been visited, `false` otherwise. 





        

<hr>



### function depth\_first\_search 

_Executes a concrete Depth-First Search (DFS) traversal over the hypergraph._ 
```cpp
template<result_discriminator Result, traits::c_hypergraph H, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
result_type< Result, search_tree< H > > depth_first_search (...) 
```



This function utilizes the generic [**dfs**](group__HGL-Algorithm.md#function-dfs) template to perform a standard stack-based traversal. It automatically manages the visited states (for both vertices and hyperedges), search tree tracking, and stack initialization.


If a specific `root_vertex_id` is provided, the algorithm explores only the connected component reachable from that root. If `no_root` is used, it iteratively ensures that every disconnected component in the entire hypergraph is fully traversed.


#### Example Usage




```cpp
auto search_tree
    = hgl::algorithm::depth_first_search(hypergraph, start_id); // (1)!

hgl::algorithm::depth_first_search<hgl::algorithm::noret>( // (2)!
    hypergraph,
    hgl::algorithm::no_root, // (3)!
    [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
    [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
);
```



1. Executes a standard DFS returning a search tree mapped to the components reachable from `start_id`.


2. Executes a DFS purely for side-effects (callbacks) without allocating memory for a search tree.


3. Passing `no_root` forces the algorithm to iterate over all vertices, ensuring disjoint components are traversed.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`).|Must be a valid [**result\_discriminator**](group__HGL-Algorithm.md#typedef-result_discriminator) enum value.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) concept.|
|PreVisitCallback|Type of the callable executed immediately before visiting a vertex.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The hypergraph to traverse. 
* `root_vertex_id` The ID of the vertex to start the search from. If `no_root`, searches the entire hypergraph. 
* `pre_visit` Hook executed immediately before visiting the vertex. 
* `post_visit` Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated. 



**Returns:**

A [**search\_tree**](group__HGL-Algorithm.md#typedef-search_tree) if `Result == ret`, otherwise nothing (`void`).    






        

<hr>



### function dfs 

_A highly customizable, generic Depth-First Search (DFS) algorithm engine for hypergraphs._ 
```cpp
template<traversal_direction Dir, traits::c_hypergraph H, traits::c_forward_range_of< search_node< H > > InitQueueRangeType, traits::c_optional_predicate< const search_node< H > & > VisitPredicate, traits::c_optional_predicate< const search_node< H > & > VisitCallback, traits::c_optional_decision_predicate< typename H::id_type, typename H::id_type > TraverseHePredicate, traits::c_decision_predicate< const search_node< H > & > EnqueuePredicate, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
bool dfs (...) 
```



This template provides the strict structural execution of a stack-based Depth-First Search over a hypergraph's topology. Because a hypergraph traversal inherently requires a two-step expansion (from a vertex to its incident hyperedges, and then to the adjacent vertices), this engine exposes specific hooks for both steps. Concrete algorithms are constructed by injecting logic into the provided callback and predicate hooks.


#### Example Usage




```cpp
std::vector<bool> visited(hypergraph.n_vertices(), false); // (1)!

bool completed = hgl::algorithm::dfs(
    hypergraph,
    hgl::algorithm::init_node_range<H>(start_id), // (2)!
    [&](const auto& node) { return not visited[node.vertex_id]; }, // (3)!
    [&](const auto& node) { // (4)!
        visited[node.vertex_id] = true;
        std::cout << "Visited vertex " << node.vertex_id << '\n';
        return true; // Continue search
    },
    hgl::empty_callback{}, // (5)!
    [&](const auto& tgt_node) { return not visited[tgt_node.vertex_id]; } // (6)!
);
```



1. Provide an external state array to track the visited vertices.


2. Initialize the search stack with a root [**search node**](structhgl_1_1algorithm_1_1search__node.md) representing the starting point.


3. The _visit predicate_ ensures vertices are not processed multiple times.


4. The concrete vertex visiting logic - marks the vertex as visited and logs it to the console.


5. Traverse all hyperedges unconditionally.


6. The _enqueue predicate_ filters out already visited adjacent vertices before they are pushed to the stack.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Dir|The [**traversal direction**](group__HGL-Algorithm.md#enum-traversal_direction) (i.e., `forward` or `backward`). Relevant only for BF-directed hypergraphs.|Defaults to `forward`.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph) concept.|
|InitQueueRangeType|A forward range of `search_node <H>` used to prime the DFS stack.|Must be a _forward range_ of [**search nodes**](structhgl_1_1algorithm_1_1search__node.md).|
|VisitPredicate|Type of the callable deciding if a popped node should be processed.|Must be one of:<br>- A `(const  search_node <H>&) -> bool` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|VisitCallback|Type of the callable executed when a vertex is officially visited.|Must be one of:<br>- A `(const  search_node <H>&) -> bool` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|TraverseHePredicate|Type of the callable deciding if an incident hyperedge should be traversed.|Must be one of:<br>- An `(id_type, id_type) -> decision` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|EnqueuePredicate|Type of the callable deciding if a target vertex should be pushed to the stack via a specific hyperedge.|Must be one of:<br>- A `(const  search_node <H>&) -> decision` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PreVisitCallback|Type of the callable executed immediately before `VisitCallback`.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The hypergraph to traverse. 
* `initial_queue_content` The initial set of search nodes to begin the traversal from. 
* `visit_pred` Predicate to filter nodes immediately after popping them from the stack. 
* `visit` Primary callback for node processing. 
* `traverse_he_pred` Predicate to determine if an incident hyperedge should be traversed. Returns a [**decision**](group__HGL-Algorithm.md#typedef-decision):
    * `accept` to traverse the hyperedge,
    * `reject` to skip the hyperedge,
    * `abort` to terminate the DFS entirely. 


* `enqueue_pred` Predicate to determine if an adjacent vertex should be pushed to the stack via a hyperedge. Returns a [**decision**](group__HGL-Algorithm.md#typedef-decision):
    * `accept` to push the vertex's search node,
    * `reject` to skip the node,
    * `abort` to terminate the DFS entirely. 


* `pre_visit` Callback executed prior to the primary visit logic. 
* `post_visit` Callback executed after all adjacent elements of the current node have been processed. 



**Returns:**

`true` if the search completed normally, `false` if it was explicitly aborted via a callback.    






        

<hr>



### function forward\_bfs 

_Executes a Breadth-First F-Search (F-BFS) traversal over a bf-directed hypergraph._ 
```cpp
template<result_discriminator Result, traits::c_bf_directed_hypergraph H, traits::c_forward_range_of< typename H::id_type > RootRange, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
result_type< Result, search_tree< H > > forward_bfs (...) 
```



This algorithm implements F-reachability semantics using Breadth-First Search for BF-directed hypergraphs. Unlike a standard backward traversal (where reaching a single head vertex is sufficient to traverse an incoming hyperedge), a forward search (F-search) uses a blocking predicate. A hyperedge is only traversed, and its tail vertices enqueued, after **all** of its head (destination) vertices have been visited.


#### Example Usage




```cpp
std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
auto search_tree = hgl::algorithm::forward_bfs(hypergraph, roots); // (1)!

hgl::algorithm::forward_bfs<hgl::algorithm::noret>( // (2)!
    hypergraph,
    roots,
    [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
    [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
);
```



1. Executes an F-BFS returning a search tree mapped to the components F-reachable from the specified roots.


2. Executes an F-BFS purely for side-effects (callbacks) without allocating memory for a search tree.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`).|Must be a valid [**result\_discriminator**](group__HGL-Algorithm.md#typedef-result_discriminator) enum value.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_bf\_directed\_hypergraph**](hgl_concepts.md#hgl-traits-c-bf-directed-hypergraph) concept.|
|RootRange|The type of the container providing the initial roots to enqueue.|Must satisfy [**c\_forward\_range\_of**](gl_concepts.md#gl-traits-c-forward-range-of) over the hypergraph's `id_type`.|
|PreVisitCallback|Type of the callable executed immediately before officially visiting a vertex.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The bf-directed hypergraph to traverse. 
* `root_vertices` A range of initial vertex IDs to start the search from. 
* `pre_visit` Hook executed immediately before officially visiting the vertex. 
* `post_visit` Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated. 



**Returns:**

A [**search\_tree**](group__HGL-Algorithm.md#typedef-search_tree) if `Result == ret`, otherwise nothing (`void`).    






        

<hr>



### function forward\_dfs 

_Executes a Depth-First F-Search (F-DFS) traversal over a bf-directed hypergraph._ 
```cpp
template<result_discriminator Result, traits::c_bf_directed_hypergraph H, traits::c_forward_range_of< typename H::id_type > RootRange, traits::c_optional_callback< void, const search_node< H > & > PreVisitCallback, traits::c_optional_callback< void, const search_node< H > & > PostVisitCallback>
result_type< Result, search_tree< H > > forward_dfs (...) 
```



This algorithm implements F-reachability semantics using a stack-based Depth-First Search for BF-directed hypergraphs. Unlike a standard backward traversal (where reaching a single head vertex is sufficient to traverse an incoming hyperedge), a forward search (F-search) uses a blocking predicate. A hyperedge is only traversed, and its tail vertices pushed to the stack, after **all** of its head (destination) vertices have been officially visited.


#### Example Usage




```cpp
std::vector<hgl::size_type> roots = { start_id_1, start_id_2 };
auto search_tree = hgl::algorithm::forward_dfs(hypergraph, roots); // (1)!

hgl::algorithm::forward_dfs<hgl::algorithm::noret>( // (2)!
    hypergraph,
    roots,
    [](const auto& node) { std::cout << "Pre-visit: " << node.vertex_id << '\n'; },
    [](const auto& node) { std::cout << "Post-visit: " << node.vertex_id << '\n'; }
);
```



1. Executes an F-DFS returning a search tree mapped to the components F-reachable from the specified roots.


2. Executes an F-DFS purely for side-effects (callbacks) without allocating memory for a search tree.



#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|Result|Controls whether the algorithm builds and returns a search tree (`ret`) or evaluates purely for side effects (`noret`).|Must be a valid [**result\_discriminator**](group__HGL-Algorithm.md#typedef-result_discriminator) enum value.|
|H|The type of the hypergraph being searched.|Must satisfy the [**c\_bf\_directed\_hypergraph**](hgl_concepts.md#hgl-traits-c-bf-directed-hypergraph) concept.|
|RootRange|The type of the container providing the initial roots to enqueue.|Must satisfy [**c\_forward\_range\_of**](gl_concepts.md#gl-traits-c-forward-range-of) over the hypergraph's `id_type`.|
|PreVisitCallback|Type of the callable executed immediately before officially visiting a vertex.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|
|PostVisitCallback|Type of the callable executed after all adjacent elements are evaluated.|Must be one of:<br>- A `(const  search_node <H>&) -> void` callable<br>- An [**empty\_callback**](group__HGL-Algorithm.md#typedef-empty_callback)|








**Parameters:**


* `hypergraph` The bf-directed hypergraph to traverse. 
* `root_vertices` A range of initial vertex IDs to start the search from. 
* `pre_visit` Hook executed immediately before officially visiting the vertex. 
* `post_visit` Hook executed after all adjacent hyperedges and target vertices of the current node have been evaluated. 



**Returns:**

A [**search\_tree**](group__HGL-Algorithm.md#typedef-search_tree) if `Result == ret`, otherwise nothing (`void`).    






        

<hr>



### function init\_node\_range 

_Initializes a container with a starting set of root search nodes._ 
```cpp
template<traits::c_hypergraph H>
std::vector< search_node< H > > init_node_range (
    typename H::id_type root_vertex_id
) 
```





**Template parameters:**


* `H` The type of the hypergraph. 



**Parameters:**


* `root_vertex_id` The ID of the starting vertex. 



**Returns:**

A `std::vector` containing a single root [**search\_node**](structhgl_1_1algorithm_1_1search__node.md). 





        

<hr>



### function init\_search\_tree 

_Initializes a search tree based on the static result discriminator._ 
```cpp
template<result_discriminator Result, traits::c_hypergraph H>
non_void_result_type< Result, search_tree< H > > init_search_tree (
    const H & hypergraph
) 
```





**Template parameters:**


* `Result` The compilation tag determining if the tree should actually be built. 
* `H` The type of the hypergraph. 



**Parameters:**


* `hypergraph` The hypergraph instance to size the search tree against. 



**Returns:**

A fully sized and initialized `search_tree` if `Result == ret`, otherwise a dummy `std::monostate`. 





        

<hr>



### function is\_head\_uniform 

_Evaluates whether the given_ _BF-directed_ _hypergraph is head-$k$-uniform (all hyperedges have a head size of $k$)._
```cpp
bool is_head_uniform (
    const traits::c_bf_directed_hypergraph auto & hypergraph,
    const size_type k
) noexcept
```




<hr>



### function is\_head\_uniform 

_Evaluates whether the given_ _BF-directed_ _hypergraph is head-uniform (all hyperedges have the exact same head size)._
```cpp
bool is_head_uniform (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function is\_in\_regular 

_Evaluates whether the given_ _BF-directed_ _hypergraph is in-$k$-regular (all vertices have an in-degree of $k$)._
```cpp
bool is_in_regular (
    const traits::c_bf_directed_hypergraph auto & hypergraph,
    const size_type k
) noexcept
```




<hr>



### function is\_in\_regular 

_Evaluates whether the given_ _BF-directed_ _hypergraph is in-regular (all vertices have the same in-degree)._
```cpp
bool is_in_regular (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function is\_out\_regular 

_Evaluates whether the given_ _BF-directed_ _hypergraph is out-$k$-regular (all vertices have an out-degree of $k$)._
```cpp
bool is_out_regular (
    const traits::c_bf_directed_hypergraph auto & hypergraph,
    const size_type k
) noexcept
```




<hr>



### function is\_out\_regular 

_Evaluates whether the given_ _BF-directed_ _hypergraph is out-regular (all vertices have the same out-degree)._
```cpp
bool is_out_regular (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function is\_reachable 

_Checks if a specific vertex was reached during the traversal._ 
```cpp
bool is_reachable (
    const traits::c_search_tree auto & tree,
    traits::c_id_type auto vertex_id
) noexcept
```





**Parameters:**


* `tree` The computed search tree resulting from a traversal. 
* `vertex_id` The identifier of the vertex to check. 



**Returns:**

`true` if the vertex has a valid predecessor in the tree, `false` otherwise. 





        

<hr>



### function is\_regular 

_Evaluates whether the given hypergraph is $k$-regular (all vertices have a degree of $k$)._ 
```cpp
bool is_regular (
    const traits::c_hypergraph auto & hypergraph,
    const size_type k
) noexcept
```




<hr>



### function is\_regular 

_Evaluates whether the given hypergraph is structurally regular (all vertices have the same degree)._ 
```cpp
bool is_regular (
    const traits::c_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function is\_tail\_uniform 

_Evaluates whether the given_ _BF-directed_ _hypergraph is tail-$k$-uniform (all hyperedges have a tail size of $k$)._
```cpp
bool is_tail_uniform (
    const traits::c_bf_directed_hypergraph auto & hypergraph,
    const size_type k
) noexcept
```




<hr>



### function is\_tail\_uniform 

_Evaluates whether the given_ _BF-directed_ _hypergraph is tail-uniform (all hyperedges have the exact same tail size)._
```cpp
bool is_tail_uniform (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function is\_uniform 

_Evaluates whether the given hypergraph is $k$-uniform (all hyperedges have a size of $k$)._ 
```cpp
bool is_uniform (
    const traits::c_hypergraph auto & hypergraph,
    const size_type k
) noexcept
```




<hr>



### function is\_uniform 

_Evaluates whether the given hypergraph is structurally uniform (all hyperedges have the exact same size)._ 
```cpp
bool is_uniform (
    const traits::c_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function max\_degree 

_Calculates the maximum degree among all vertices in a hypergraph._ 
```cpp
size_type max_degree (
    const traits::c_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function max\_head\_size 

_Calculates the maximum head size among all hyperedges in a_ _BF-directed_ _hypergraph._
```cpp
size_type max_head_size (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function max\_in\_degree 

_Calculates the maximum in-degree among all vertices in a_ _BF-directed_ _hypergraph._
```cpp
size_type max_in_degree (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function max\_out\_degree 

_Calculates the maximum out-degree among all vertices in a_ _BF-directed_ _hypergraph._
```cpp
size_type max_out_degree (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function max\_tail\_size 

_Calculates the maximum tail size among all hyperedges in a_ _BF-directed_ _hypergraph._
```cpp
size_type max_tail_size (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function min\_degree 

_Calculates the minimum degree among all vertices in a hypergraph._ 
```cpp
size_type min_degree (
    const traits::c_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function min\_head\_size 

_Calculates the minimum head size among all hyperedges in a_ _BF-directed_ _hypergraph._
```cpp
size_type min_head_size (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function min\_in\_degree 

_Calculates the minimum in-degree among all vertices in a_ _BF-directed_ _hypergraph._
```cpp
size_type min_in_degree (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function min\_out\_degree 

_Calculates the minimum out-degree among all vertices in a_ _BF-directed_ _hypergraph._
```cpp
size_type min_out_degree (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function min\_tail\_size 

_Calculates the minimum tail size among all hyperedges in a_ _BF-directed_ _hypergraph._
```cpp
size_type min_tail_size (
    const traits::c_bf_directed_hypergraph auto & hypergraph
) noexcept
```




<hr>



### function rank 

_Calculates the rank (maximum size of any hyperedge) of a hypergraph._ 
```cpp
size_type rank (
    const traits::c_hypergraph auto & hypergraph
) noexcept
```




<hr>

------------------------------


