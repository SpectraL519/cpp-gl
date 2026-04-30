

# Struct gl::algorithm::paths\_descriptor

**template &lt;traits::c\_graph G, traits::c\_arithmetic VertexDistanceType&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**paths\_descriptor**](structgl_1_1algorithm_1_1paths__descriptor.md)



_A descriptor structure holding the results of a single-source shortest path execution._ [More...](#detailed-description)

* `#include <gl/algorithm/pathfinding/dijkstra.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) G::id\_type | [**id\_type**](#typedef-id_type)  <br> |
| typedef [**VertexDistanceType**](group__GL-Core.md#variable-invalid_id_v) | [**distance\_type**](#typedef-distance_type)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**predecessors\_map**](group__GL-Algorithm.md#typedef-predecessors_map)&lt; [**G**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**predecessors**](#variable-predecessors)  <br>_The predecessor map tracking the optimal path tree._  |
|  std::vector&lt; distance\_type &gt; | [**distances**](#variable-distances)  <br>_The accumulated shortest distances to each vertex from the source._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**paths\_descriptor**](#function-paths_descriptor) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](group__GL-Core.md#typedef-size_type) n\_vertices) <br>_Constructs a descriptor sized for the given number of vertices._  |




























## Detailed Description


#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|G|The type of the graph.|Must satisfy the [**c\_graph**](gl_concepts.md#gl-traits-c-graph) concept.|
|VertexDistanceType|The numeric type used to represent accumulated path weights/distances.|Must satisfy the [**c\_arithmetic**](gl_concepts.md#gl-traits-c-arithmetic) concept.|







    
## Public Types Documentation




### typedef id\_type 

```cpp
using gl::algorithm::paths_descriptor< G, VertexDistanceType >::id_type =  typename G::id_type;
```




<hr>



### typedef distance\_type 

```cpp
using gl::algorithm::paths_descriptor< G, VertexDistanceType >::distance_type =  VertexDistanceType;
```




<hr>
## Public Attributes Documentation




### variable predecessors 

_The predecessor map tracking the optimal path tree._ 
```cpp
predecessors_map<G> gl::algorithm::paths_descriptor< G, VertexDistanceType >::predecessors;
```




<hr>



### variable distances 

_The accumulated shortest distances to each vertex from the source._ 
```cpp
std::vector<distance_type> gl::algorithm::paths_descriptor< G, VertexDistanceType >::distances;
```




<hr>
## Public Functions Documentation




### function paths\_descriptor 

_Constructs a descriptor sized for the given number of vertices._ 
```cpp
inline algorithm::paths_descriptor::paths_descriptor (
    const  size_type n_vertices
) 
```





**Parameters:**


* `n_vertices` The total number of vertices in the graph. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/pathfinding/dijkstra.hpp`

