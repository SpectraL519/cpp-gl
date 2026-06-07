

# Struct gl::directed\_t



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**directed\_t**](structgl_1_1directed__t.md)



_The tag type representing a directed graph configuration._ [More...](#detailed-description)

* `#include <gl/directional_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::type\_identity\_t&lt; [**directed\_t**](structgl_1_1directed__t.md) &gt; | [**type**](#typedef-type)  <br>_A type identity alias for the_ [_**directed\_t**_](structgl_1_1directed__t.md) _tag, allowing for easier type comparisons and trait evaluations._ |






















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_incident\_from**](#function-is_incident_from) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**EdgeType**](group__GL-Core.md#variable-invalid_id_v) & edge, [**typename**](group__GL-Core.md#variable-invalid_id_v) EdgeType::id\_type vertex\_id) <br>_Determines if a given edge is incident from a specified vertex in a directed graph._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_incident\_to**](#function-is_incident_to) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**EdgeType**](group__GL-Core.md#variable-invalid_id_v) & edge, [**typename**](group__GL-Core.md#variable-invalid_id_v) EdgeType::id\_type vertex\_id) <br>_Determines if a given edge is incident to a specified vertex in a directed graph._  |


























## Detailed Description


This tag is used to indicate that a graph is directed, meaning that its edges have a specific direction from a source vertex to a target vertex.


#### See Also




* [**undirected\_t**](structgl_1_1undirected__t.md) : For the tag representing an undirected graph configuration.
* [**c\_graph\_directional\_tag**](gl_concepts.md#gl-traits-c-graph-directional-tag) : For the concept used to validate graph directional tags.
* [**edge\_descriptor**](classgl_1_1edge__descriptor.md) : For the edge descriptor type defined based on the graph traits, which includes the directional tag as part of its definition. 





    
## Public Types Documentation




### typedef type 

_A type identity alias for the_ [_**directed\_t**_](structgl_1_1directed__t.md) _tag, allowing for easier type comparisons and trait evaluations._
```cpp
using gl::directed_t::type =  std::type_identity_t<directed_t>;
```




<hr>
## Public Static Functions Documentation




### function is\_incident\_from 

_Determines if a given edge is incident from a specified vertex in a directed graph._ 
```cpp
template<traits::c_directed_edge EdgeType>
static inline bool gl::directed_t::is_incident_from (
    const  EdgeType & edge,
    typename EdgeType::id_type vertex_id
) 
```



Validates whether the provided vertex ID corresponds specifically to the **source** vertex of the edge.




**Template parameters:**


* `EdgeType` The type of the edge descriptor, which must be an instantiation of [**edge\_descriptor**](classgl_1_1edge__descriptor.md) with the directed tag. 



**Returns:**

For a directed edge [**$(u, v)**](group__GL-Core.md#variable-invalid_id_v)$ and a vertex ID `vertex_id`, returns `true` if `vertex_id` corresponds to $u$ (the source vertex), and `false` otherwise. 





        

<hr>



### function is\_incident\_to 

_Determines if a given edge is incident to a specified vertex in a directed graph._ 
```cpp
template<traits::c_directed_edge EdgeType>
static inline bool gl::directed_t::is_incident_to (
    const  EdgeType & edge,
    typename EdgeType::id_type vertex_id
) 
```



Validates whether the provided vertex ID corresponds specifically to the **target** vertex of the edge.




**Template parameters:**


* `EdgeType` The type of the edge descriptor, which must be an instantiation of [**edge\_descriptor**](classgl_1_1edge__descriptor.md) with the directed tag. 



**Returns:**

For a directed edge [**$(u, v)**](group__GL-Core.md#variable-invalid_id_v)$ and a vertex ID `vertex_id`, returns `true` if `vertex_id` corresponds to $v$ (the target vertex), and `false` otherwise. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/directional_tags.hpp`

