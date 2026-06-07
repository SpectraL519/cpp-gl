

# Struct hgl::algorithm::traversal\_policy&lt; H, traversal\_direction::backward &gt;

**template &lt;traits::c\_bf\_directed\_hypergraph H&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacehgl_1_1algorithm.md) **>** [**traversal\_policy&lt; H, traversal\_direction::backward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1backward_01_4.md)



_Traversal policy specialization for backward searches on BF-directed hypergraphs._ 

* `#include <hgl/algorithm/core.hpp>`







































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**target\_hyperedges**](#function-target_hyperedges) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**H**](group__HGL-Core.md#variable-invalid_id_v) & h, [**typename**](group__HGL-Core.md#variable-invalid_id_v) H::id\_type v\_id) <br>_Retrieves the hyperedges entering the given vertex (backward star)._  |
|  [**auto**](group__HGL-Core.md#variable-invalid_id_v) | [**target\_vertices**](#function-target_vertices) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**H**](group__HGL-Core.md#variable-invalid_id_v) & h, [**typename**](group__HGL-Core.md#variable-invalid_id_v) H::id\_type he\_id) <br>_Retrieves the vertices originating the given hyperedge (tail nodes)._  |


























## Public Static Functions Documentation




### function target\_hyperedges 

_Retrieves the hyperedges entering the given vertex (backward star)._ 
```cpp
static inline auto algorithm::traversal_policy< H, traversal_direction::backward >::target_hyperedges (
    const  H & h,
    typename H::id_type v_id
) 
```




<hr>



### function target\_vertices 

_Retrieves the vertices originating the given hyperedge (tail nodes)._ 
```cpp
static inline auto algorithm::traversal_policy< H, traversal_direction::backward >::target_vertices (
    const  H & h,
    typename H::id_type he_id
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

