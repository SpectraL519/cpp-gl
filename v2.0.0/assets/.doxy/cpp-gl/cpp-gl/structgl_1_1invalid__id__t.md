

# Struct gl::invalid\_id\_t



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**invalid\_id\_t**](structgl_1_1invalid__id__t.md)



_A helper type that can be implicitly converted to the invalid ID value for any valid ID type._ [More...](#detailed-description)

* `#include <hgl/constants.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**operator IdType**](#function-operator-idtype) () noexcept const<br>_Implicitly converts to the invalid ID value for any type that satisfies the_ [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) _concept._ |




























## Detailed Description


#### See Also




* [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)
* [**invalid\_id\_v**](group__GL-Core.md#variable-invalid_id_v)
* [**invalid\_id**](group__GL-Core.md#variable-invalid_id) 





    
## Public Functions Documentation




### function operator IdType 

_Implicitly converts to the invalid ID value for any type that satisfies the_ [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) _concept._
```cpp
template<traits::c_id_type IdType>
inline constexpr gl::invalid_id_t::operator IdType () noexcept const
```




<hr>## Friends Documentation





### friend operator== 

_Equality comparison operator to compare an ID value with the invalid ID constant._ 
```cpp
template<traits::c_id_type IdType>
inline constexpr  bool gl::invalid_id_t::operator== (
    const  IdType & lhs,
    invalid_id_t
) noexcept
```



This operator allows for direct comparison between an ID value and the `invalid_id` constant, enabling easy checking if a given ID is invalid without needing to explicitly reference the `invalid_id_v` constant for the specific ID type.




**Template parameters:**


* `IdType` The type of the ID, which must satisfy the [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) concept. 



**Parameters:**


* `lhs` The ID value to compare againsyt the invalid ID constant. 
* `rhs` The `invalid_id` constant (of type `invalid_id_t`) to compare with the ID value. 



**Returns:**

Returns `true` if `lhs` is equal to the invalid ID value for its type, and `false` otherwise. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/constants.hpp`

