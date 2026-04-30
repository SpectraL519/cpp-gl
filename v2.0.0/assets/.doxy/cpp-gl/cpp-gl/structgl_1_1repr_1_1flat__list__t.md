

# Struct gl::repr::flat\_list\_t



[**ClassList**](annotated.md) **>** [**repr**](namespacegl_1_1repr.md) **>** [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md)



_Tag struct for the flattened adjacency list graph representation._ [More...](#detailed-description)

* `#include <gl/repr_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef impl::adjacency\_list&lt; [**GraphTraits**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_Type alias for the flattened adjacency list graph representation based on the provided graph traits._  |
















































## Detailed Description


#### See Also




* [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) : For the data structure used for the underlying model representation. 





    
## Public Types Documentation




### typedef type 

_Type alias for the flattened adjacency list graph representation based on the provided graph traits._ 
```cpp
template<traits::c_instantiation_of< graph_traits > GraphTraits>
using gl::repr::flat_list_t::type =  impl::adjacency_list<GraphTraits>;
```





**Template parameters:**


* `GraphTraits` The graph traits for which to define the flattened adjacency list type. Must be an instantiation of [**graph\_traits**](structgl_1_1graph__traits.md) and have `flat_list_t` as its representation tag. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/repr_tags.hpp`

