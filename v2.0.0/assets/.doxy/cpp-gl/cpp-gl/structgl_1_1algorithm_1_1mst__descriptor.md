

# Struct gl::algorithm::mst\_descriptor

**template &lt;traits::c\_undirected\_graph G&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**mst\_descriptor**](structgl_1_1algorithm_1_1mst__descriptor.md)



_A descriptor structure holding the results of a Minimum Spanning Tree (MST) execution._ [More...](#detailed-description)

* `#include <gl/algorithm/spanning_tree/prim_mst.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**G**](group__GL-Core.md#variable-invalid_id_v) | [**graph\_type**](#typedef-graph_type)  <br>_The type of the graph._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) graph\_type::edge\_type | [**edge\_type**](#typedef-edge_type)  <br>_The type of the edges stored in the graph._  |
| typedef [**vertex\_distance\_type**](group__GL-Core.md#typedef-vertex_distance_type)&lt; [**graph\_type**](structgl_1_1algorithm_1_1mst__descriptor.md#typedef-graph_type) &gt; | [**weight\_type**](#typedef-weight_type)  <br>_The numeric type used to represent accumulated tree weights._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  std::vector&lt; [**edge\_type**](structgl_1_1algorithm_1_1mst__descriptor.md#typedef-edge_type) &gt; | [**edges**](#variable-edges)  <br>_The sequence of edges that form the Minimum Spanning Tree._  |
|  [**weight\_type**](structgl_1_1algorithm_1_1mst__descriptor.md#typedef-weight_type) | [**weight**](#variable-weight)  = `static_cast < weight_type >(0)`<br>_The accumulated minimum weight/cost of the entire spanning tree._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**mst\_descriptor**](#function-mst_descriptor) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](group__GL-Core.md#typedef-size_type) n\_vertices) <br>_Constructs a descriptor sized to hold the resulting tree edges._  |




























## Detailed Description




**Template parameters:**


* `G` The type of the undirected graph. Must satisfy the [**c\_undirected\_graph**](gl_concepts.md#gl-traits-c-undirected-graph) concept. 




    
## Public Types Documentation




### typedef graph\_type 

_The type of the graph._ 
```cpp
using gl::algorithm::mst_descriptor< G >::graph_type =  G;
```




<hr>



### typedef edge\_type 

_The type of the edges stored in the graph._ 
```cpp
using gl::algorithm::mst_descriptor< G >::edge_type =  typename graph_type::edge_type;
```




<hr>



### typedef weight\_type 

_The numeric type used to represent accumulated tree weights._ 
```cpp
using gl::algorithm::mst_descriptor< G >::weight_type =  vertex_distance_type<graph_type>;
```




<hr>
## Public Attributes Documentation




### variable edges 

_The sequence of edges that form the Minimum Spanning Tree._ 
```cpp
std::vector<edge_type> gl::algorithm::mst_descriptor< G >::edges;
```




<hr>



### variable weight 

_The accumulated minimum weight/cost of the entire spanning tree._ 
```cpp
weight_type gl::algorithm::mst_descriptor< G >::weight = static_cast < weight_type >(0);
```




<hr>
## Public Functions Documentation




### function mst\_descriptor 

_Constructs a descriptor sized to hold the resulting tree edges._ 
```cpp
inline algorithm::mst_descriptor::mst_descriptor (
    const  size_type n_vertices
) 
```





**Parameters:**


* `n_vertices` The total number of vertices in the graph. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/spanning_tree/prim_mst.hpp`

