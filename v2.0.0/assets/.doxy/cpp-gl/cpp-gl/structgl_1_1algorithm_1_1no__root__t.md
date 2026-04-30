

# Struct gl::algorithm::no\_root\_t



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md)



_Tag type providing an implicit conversion to the appropriate_ `no_root_v` _for any numeric ID type._[More...](#detailed-description)

* `#include <hgl/algorithm/core.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**operator IdType**](#function-operator-idtype) () noexcept const<br>_Implicitly converts to the numeric_ `no_root_v` _constant._ |




























## Detailed Description


#### See Also




* [**no\_root\_v**](group__GL-Algorithm.md#variable-no_root_v)
* [**no\_root**](group__GL-Algorithm.md#variable-no_root) 





    
## Public Functions Documentation




### function operator IdType 

_Implicitly converts to the numeric_ `no_root_v` _constant._
```cpp
template<traits::c_id_type IdType>
inline constexpr algorithm::no_root_t::operator IdType () noexcept const
```




<hr>## Friends Documentation





### friend operator== 

_Checks if a given ID matches the_ `no_root_v` _constant._
```cpp
template<traits::c_id_type IdType>
inline constexpr  bool algorithm::no_root_t::operator== (
    const  IdType & lhs,
    no_root_t
) noexcept
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/core.hpp`

