

# Struct gl::name\_property



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**name\_property**](structgl_1_1name__property.md)



_A property struct providing a basic string-based naming facility._ 

* `#include <gl/types/properties.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::string | [**value\_type**](#typedef-value_type)  <br>_The underlying string type used for the name._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**value\_type**](structgl_1_1name__property.md#typedef-value_type) | [**name**](#variable-name)  <br>_The stored name string._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**name\_property**](structgl_1_1name__property.md) & | [**operator=**](#function-operator) (std::string\_view name) <br>_Assigns a new name from a string view._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_1) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**name\_property**](structgl_1_1name__property.md) &) const<br> |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**operator&lt;=&gt;**](#function-operator-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**name\_property**](structgl_1_1name__property.md) &) const<br> |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_2) ([**const**](group__GL-Core.md#variable-invalid_id_v) std::string\_view name) const<br> |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**operator&lt;=&gt;**](#function-operator-22) ([**const**](group__GL-Core.md#variable-invalid_id_v) std::string\_view name) const<br> |




























## Public Types Documentation




### typedef value\_type 

_The underlying string type used for the name._ 
```cpp
using gl::name_property::value_type =  std::string;
```




<hr>
## Public Attributes Documentation




### variable name 

_The stored name string._ 
```cpp
value_type gl::name_property::name;
```




<hr>
## Public Functions Documentation




### function operator= 

_Assigns a new name from a string view._ 
```cpp
inline name_property & gl::name_property::operator= (
    std::string_view name
) 
```




<hr>



### function operator== 

```cpp
bool gl::name_property::operator== (
    const  name_property &
) const
```




<hr>



### function operator&lt;=&gt; [1/2]

```cpp
auto gl::name_property::operator<=> (
    const  name_property &
) const
```




<hr>



### function operator== 

```cpp
inline bool gl::name_property::operator== (
    const std::string_view name
) const
```




<hr>



### function operator&lt;=&gt; [2/2]

```cpp
inline auto gl::name_property::operator<=> (
    const std::string_view name
) const
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Serializes the name property to an output stream using quoted formatting._ 
```cpp
inline std::ostream & gl::name_property::operator<< (
    std::ostream & os,
    const  name_property & property
) 
```




<hr>



### friend operator&gt;&gt; 

_Deserializes the name property from an input stream using quoted formatting._ 
```cpp
inline std::istream & gl::name_property::operator>> (
    std::istream & is,
    name_property & property
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/properties.hpp`

