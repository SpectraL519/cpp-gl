

# Struct gl::util::concat\_fn



[**ClassList**](annotated.md) **>** [**util**](namespacegl_1_1util.md) **>** [**concat\_fn**](structgl_1_1util_1_1concat__fn.md)



_A function object for concatenating two viewable ranges into a_ `concat_view` _._[More...](#detailed-description)

* `#include <gl/util/ranges.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**operator()**](#function-operator) ([**R1**](group__GL-Core.md#variable-invalid_id_v) && r1, [**R2**](group__GL-Core.md#variable-invalid_id_v) && r2) const<br>_Concatenates two viewable ranges into a_ `concat_view` _._ |




























## Detailed Description


#### See Also




* [**concat\_view**](classgl_1_1util_1_1concat__view.md): The view type that represents the concatenation of two ranges.
* [**concat**](group__GL-Util.md#variable-concat): A compile-time constant instantiation of this function object for convenient use. 





    
## Public Functions Documentation




### function operator() 

_Concatenates two viewable ranges into a_ `concat_view` _._
```cpp
template<std::ranges::viewable_range R1, std::ranges::viewable_range R2>
inline constexpr  auto util::concat_fn::operator() (
    R1 && r1,
    R2 && r2
) const
```





**Template parameters:**


* `R1` The type of the first range. 
* `R2` The type of the second range. 



**Parameters:**


* `r1` The first range to concatenate. 
* `r2` The second range to concatenate. 



**Returns:**

A `concat_view` that represents the concatenation of the two ranges. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/util/ranges.hpp`

