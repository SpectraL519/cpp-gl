

# Struct gl::weight\_property

**template &lt;traits::c\_arithmetic WeightType&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**weight\_property**](structgl_1_1weight__property.md)



_A property struct providing arithmetic weight for edges or vertices._ [More...](#detailed-description)

* `#include <gl/types/properties.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**WeightType**](group__GL-Core.md#variable-invalid_id_v) | [**weight\_type**](#typedef-weight_type)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  weight\_type | [**weight**](#variable-weight)  = `static_cast <weight_type>(1 ll )`<br>_The stored numeric weight._  |












































## Detailed Description


#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|WeightType|The numeric type used to store the weight value.|`double`|[**c\_arithmetic**](gl_concepts.md#gl-traits-c-arithmetic)|







    
## Public Types Documentation




### typedef weight\_type 

```cpp
using gl::weight_property< WeightType >::weight_type =  WeightType;
```




<hr>
## Public Attributes Documentation




### variable weight 

_The stored numeric weight._ 
```cpp
weight_type gl::weight_property< WeightType >::weight = static_cast <weight_type>(1 ll );
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

```cpp
inline std::ostream & gl::weight_property::operator<< (
    std::ostream & os,
    const  weight_property & property
) 
```




<hr>



### friend operator&gt;&gt; 

```cpp
inline std::istream & gl::weight_property::operator>> (
    std::istream & is,
    weight_property & property
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/properties.hpp`

