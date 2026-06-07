

# Struct gl::repr::flat\_matrix\_t



[**ClassList**](annotated.md) **>** [**repr**](namespacegl_1_1repr.md) **>** [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md)



_Tag struct for the flattened adjacency matrix graph representation._ [More...](#detailed-description)

* `#include <gl/repr_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef impl::adjacency\_matrix&lt; [**GraphTraits**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_Type alias for the flattened adjacency matrix graph representation based on the provided graph traits._  |
















































## Detailed Description


#### See Also




* [**flat\_matrix**](classgl_1_1flat__matrix.md) : For the data structure used for the underlying model representation. 





    
## Public Types Documentation




### typedef type 

_Type alias for the flattened adjacency matrix graph representation based on the provided graph traits._ 
```cpp
template<traits::c_instantiation_of< graph_traits > GraphTraits>
using gl::repr::flat_matrix_t::type =  impl::adjacency_matrix<GraphTraits>;
```





**Template parameters:**


* `GraphTraits` The graph traits for which to define the flattened adjacency matrix type. Must be an instantiation of [**graph\_traits**](structgl_1_1graph__traits.md) and have `flat_matrix_t` as its representation tag. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/repr_tags.hpp`

