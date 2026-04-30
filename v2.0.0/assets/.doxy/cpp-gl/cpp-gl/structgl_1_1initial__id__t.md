

# Struct gl::initial\_id\_t



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**initial\_id\_t**](structgl_1_1initial__id__t.md)



_A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type._ [More...](#detailed-description)

* `#include <hgl/constants.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**operator IdType**](#function-operator-idtype) () noexcept const<br>_Implicitly converts to the initial ID value of 0 for any type that satisfies the_ [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) _concept._ |




























## Detailed Description


#### See Also




* [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)
* [**initial\_id\_v**](group__GL-Core.md#variable-initial_id_v)
* [**initial\_id**](group__GL-Core.md#variable-initial_id) 





    
## Public Functions Documentation




### function operator IdType 

_Implicitly converts to the initial ID value of 0 for any type that satisfies the_ [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) _concept._
```cpp
template<traits::c_id_type IdType>
inline constexpr gl::initial_id_t::operator IdType () noexcept const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/constants.hpp`

