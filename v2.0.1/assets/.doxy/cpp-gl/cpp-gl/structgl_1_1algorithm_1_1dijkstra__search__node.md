

# Struct gl::algorithm::dijkstra\_search\_node

**template &lt;traits::c\_graph G&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**dijkstra\_search\_node**](structgl_1_1algorithm_1_1dijkstra__search__node.md)



_Internal node structure for Dijkstra's algorithm to snapshot distances and preserve heap invariants._ [More...](#detailed-description)

* `#include <gl/algorithm/pathfinding/dijkstra.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) G::id\_type | [**id\_type**](#typedef-id_type)  <br>_The type of the vertex ID._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**id\_type**](structgl_1_1algorithm_1_1dijkstra__search__node.md#typedef-id_type) | [**vertex\_id**](#variable-vertex_id)  <br>_The ID of the vertex represented by this node._  |
|  [**id\_type**](structgl_1_1algorithm_1_1dijkstra__search__node.md#typedef-id_type) | [**pred\_id**](#variable-pred_id)  <br>_The ID of the predecessor vertex used to reach this node._  |
|  [**vertex\_distance\_type**](group__GL-Core.md#typedef-vertex_distance_type)&lt; [**G**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**distance**](#variable-distance)  <br>_The accumulated distance from the source to this vertex at the time of enqueueing._  |












































## Detailed Description


This structure is used in the [**dijkstra\_shortest\_paths**](group__GL-Algorithm.md#function-dijkstra_shortest_paths) algorithm to capture the state of a vertex at the moment it is enqueued, ensuring that the priority queue remains stable even if the global distance map is updated during traversal.




**Template parameters:**


* `G` The type of the graph. Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept. 




    
## Public Types Documentation




### typedef id\_type 

_The type of the vertex ID._ 
```cpp
using gl::algorithm::dijkstra_search_node< G >::id_type =  typename G::id_type;
```




<hr>
## Public Attributes Documentation




### variable vertex\_id 

_The ID of the vertex represented by this node._ 
```cpp
id_type gl::algorithm::dijkstra_search_node< G >::vertex_id;
```




<hr>



### variable pred\_id 

_The ID of the predecessor vertex used to reach this node._ 
```cpp
id_type gl::algorithm::dijkstra_search_node< G >::pred_id;
```




<hr>



### variable distance 

_The accumulated distance from the source to this vertex at the time of enqueueing._ 
```cpp
vertex_distance_type<G> gl::algorithm::dijkstra_search_node< G >::distance;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/pathfinding/dijkstra.hpp`

