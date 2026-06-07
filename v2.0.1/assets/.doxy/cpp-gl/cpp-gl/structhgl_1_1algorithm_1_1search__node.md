

# Struct hgl::algorithm::search\_node

**template &lt;traits::c\_hypergraph H&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacehgl_1_1algorithm.md) **>** [**search\_node**](structhgl_1_1algorithm_1_1search__node.md)



_Represents an active node in a search container (e.g., a BFS queue or DFS stack) for hypergraph traversals._ [More...](#detailed-description)

* `#include <hgl/algorithm/core.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) H::id\_type | [**id\_type**](#typedef-id_type)  <br>_The identifier type of the hypergraph elements._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) | [**vertex\_id**](#variable-vertex_id)  = `invalid_id`<br>_The ID of the current vertex._  |
|  [**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) | [**pred\_id**](#variable-pred_id)  = `invalid_id`<br>_The ID of the predecessor from which this vertex was reached._  |
|  [**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) | [**hyperedge\_id**](#variable-hyperedge_id)  = `invalid_id`<br>_The ID of the hyperedge via which this vertex was reached from the predecessor._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**search\_node**](#function-search_node-13) () = default<br>_Default constructor creates an invalid node._  |
|   | [**search\_node**](#function-search_node-23) ([**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) vertex\_id) <br>_Constructs a_ _root_ _search node (predecessor is itself, no incident hyperedge)._ |
|   | [**search\_node**](#function-search_node-33) ([**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) vertex\_id, [**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) pred\_id, [**id\_type**](structhgl_1_1algorithm_1_1search__node.md#typedef-id_type) hyperedge\_id) <br>_Constructs a search node with an explicit predecessor vertex and the connecting hyperedge._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**is\_root**](#function-is_root) () noexcept const<br>_Checks if this node is the root of a search tree._  |




























## Detailed Description




**Template parameters:**


* `H` The type of the hypergraph being searched. Must satisfy [**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph). 




    
## Public Types Documentation




### typedef id\_type 

_The identifier type of the hypergraph elements._ 
```cpp
using hgl::algorithm::search_node< H >::id_type =  typename H::id_type;
```




<hr>
## Public Attributes Documentation




### variable vertex\_id 

_The ID of the current vertex._ 
```cpp
id_type hgl::algorithm::search_node< H >::vertex_id = invalid_id;
```




<hr>



### variable pred\_id 

_The ID of the predecessor from which this vertex was reached._ 
```cpp
id_type hgl::algorithm::search_node< H >::pred_id = invalid_id;
```




<hr>



### variable hyperedge\_id 

_The ID of the hyperedge via which this vertex was reached from the predecessor._ 
```cpp
id_type hgl::algorithm::search_node< H >::hyperedge_id = invalid_id;
```




<hr>
## Public Functions Documentation




### function search\_node [1/3]

_Default constructor creates an invalid node._ 
```cpp
algorithm::search_node::search_node () = default
```




<hr>



### function search\_node [2/3]

_Constructs a_ _root_ _search node (predecessor is itself, no incident hyperedge)._
```cpp
inline algorithm::search_node::search_node (
    id_type vertex_id
) 
```





**Parameters:**


* `vertex_id` The ID of the root vertex. 




        

<hr>



### function search\_node [3/3]

_Constructs a search node with an explicit predecessor vertex and the connecting hyperedge._ 
```cpp
inline algorithm::search_node::search_node (
    id_type vertex_id,
    id_type pred_id,
    id_type hyperedge_id
) 
```





**Parameters:**


* `vertex_id` The ID of the currently reached vertex. 
* `pred_id` The ID of the predecessor vertex from which this vertex was reached. 
* `hyperedge_id` The ID of the hyperedge connecting the predecessor to this vertex. 




        

<hr>



### function is\_root 

_Checks if this node is the root of a search tree._ 
```cpp
inline bool algorithm::search_node::is_root () noexcept const
```





**Returns:**

`true` if the node is valid and its predecessor is itself, `false` otherwise. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

