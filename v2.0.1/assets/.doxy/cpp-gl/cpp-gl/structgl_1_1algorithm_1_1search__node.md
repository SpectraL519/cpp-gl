

# Struct gl::algorithm::search\_node

**template &lt;traits::c\_graph GraphType&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**search\_node**](structgl_1_1algorithm_1_1search__node.md)



_Represents an active node in a search container (e.g., a BFS queue or DFS stack)._ [More...](#detailed-description)

* `#include <hgl/algorithm/core.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) GraphType::id\_type | [**id\_type**](#typedef-id_type)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  id\_type | [**vertex\_id**](#variable-vertex_id)  <br>_The ID of the vertex currently being searched._  |
|  id\_type | [**pred\_id**](#variable-pred_id)  <br>_The ID of the predecessor from which this vertex was reached._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**search\_node**](#function-search_node-12) (id\_type vertex\_id) <br>_Constructs a search node acting as a root (predecessor is itself)._  |
|   | [**search\_node**](#function-search_node-22) (id\_type vertex\_id, id\_type pred\_id) <br>_Constructs a search node with an explicit predecessor._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_root**](#function-is_root) () noexcept const<br>_Checks if this node is the root of a search tree._  |




























## Detailed Description




**Template parameters:**


* `GraphType` The type of the graph being searched. 




    
## Public Types Documentation




### typedef id\_type 

```cpp
using gl::algorithm::search_node< GraphType >::id_type =  typename GraphType::id_type;
```




<hr>
## Public Attributes Documentation




### variable vertex\_id 

_The ID of the vertex currently being searched._ 
```cpp
id_type gl::algorithm::search_node< GraphType >::vertex_id;
```




<hr>



### variable pred\_id 

_The ID of the predecessor from which this vertex was reached._ 
```cpp
id_type gl::algorithm::search_node< GraphType >::pred_id;
```




<hr>
## Public Functions Documentation




### function search\_node [1/2]

_Constructs a search node acting as a root (predecessor is itself)._ 
```cpp
inline algorithm::search_node::search_node (
    id_type vertex_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 




        

<hr>



### function search\_node [2/2]

_Constructs a search node with an explicit predecessor._ 
```cpp
inline algorithm::search_node::search_node (
    id_type vertex_id,
    id_type pred_id
) 
```





**Parameters:**


* `vertex_id` The ID of the vertex. 
* `pred_id` The ID of the vertex's predecessor. 




        

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
The documentation for this class was generated from the following file `include/gl/algorithm/core.hpp`

