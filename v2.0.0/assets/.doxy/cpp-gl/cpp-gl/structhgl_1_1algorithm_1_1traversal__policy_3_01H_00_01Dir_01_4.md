

# Struct hgl::algorithm::traversal\_policy&lt; H, Dir &gt;

**template &lt;traits::c\_undirected\_hypergraph H, [**traversal\_direction**](group__HGL-Algorithm.md#enum-traversal_direction) Dir&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacehgl_1_1algorithm.md) **>** [**traversal\_policy&lt; H, Dir &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01Dir_01_4.md)



_Traversal policy specialization for undirected hypergraphs._ 

* `#include <hgl/algorithm/core.hpp>`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**target\_hyperedges**](#function-target_hyperedges) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**H**](group__HGL-Core.md#variable-invalid_id_v) & h, [**typename**](group__HGL-Core.md#variable-invalid_id_v) H::id\_type v\_id) <br>_Retrieves the hyperedges incident to the given vertex._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**target\_vertices**](#function-target_vertices) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**H**](group__HGL-Core.md#variable-invalid_id_v) & h, [**typename**](group__HGL-Core.md#variable-invalid_id_v) H::id\_type he\_id) <br>_Retrieves the vertices incident to the given hyperedge._  |


























## Public Static Functions Documentation




### function target\_hyperedges 

_Retrieves the hyperedges incident to the given vertex._ 
```cpp
static inline auto algorithm::traversal_policy< H, Dir >::target_hyperedges (
    const  H & h,
    typename H::id_type v_id
) 
```




<hr>



### function target\_vertices 

_Retrieves the vertices incident to the given hyperedge._ 
```cpp
static inline auto algorithm::traversal_policy< H, Dir >::target_vertices (
    const  H & h,
    typename H::id_type he_id
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

