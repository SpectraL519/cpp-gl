

# Struct gl::repr::list\_t



[**ClassList**](annotated.md) **>** [**repr**](namespacegl_1_1repr.md) **>** [**list\_t**](structgl_1_1repr_1_1list__t.md)



_Tag struct for the standard adjacency list graph representation._ 

* `#include <gl/repr_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef impl::adjacency\_list&lt; [**GraphTraits**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_Type alias for the adjacency list graph representation based on the provided graph traits._  |
















































## Public Types Documentation




### typedef type 

_Type alias for the adjacency list graph representation based on the provided graph traits._ 
```cpp
template<traits::c_instantiation_of< graph_traits > GraphTraits>
using gl::repr::list_t::type =  impl::adjacency_list<GraphTraits>;
```





**Template parameters:**


* `GraphTraits` The graph traits for which to define the adjacency list type. Must be an instantiation of [**graph\_traits**](structgl_1_1graph__traits.md) and have `list_t` as its representation tag. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/repr_tags.hpp`

