

# Class gl::dynamic\_properties



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**dynamic\_properties**](classgl_1_1dynamic__properties.md)



_A type-safe container for heterogeneous properties stored by string keys._ [More...](#detailed-description)

* `#include <gl/types/properties.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::string | [**key\_type**](#typedef-key_type)  <br>_The type used for property keys (string identifiers)._  |
| typedef std::any | [**value\_type**](#typedef-value_type)  <br>_The type used for property values (type-erased storage)._  |
| typedef std::unordered\_map&lt; [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type), [**value\_type**](classgl_1_1dynamic__properties.md#typedef-value_type) &gt; | [**property\_map\_type**](#typedef-property_map_type)  <br>_The underlying map type for storing properties._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**dynamic\_properties**](#function-dynamic_properties-13) () = default<br> |
|   | [**dynamic\_properties**](#function-dynamic_properties-23) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**dynamic\_properties**](classgl_1_1dynamic__properties.md) &) = default<br> |
|   | [**dynamic\_properties**](#function-dynamic_properties-33) ([**dynamic\_properties**](classgl_1_1dynamic__properties.md) &&) noexcept<br> |
|  [**dynamic\_properties**](classgl_1_1dynamic__properties.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**dynamic\_properties**](classgl_1_1dynamic__properties.md) &) = default<br> |
|  [**dynamic\_properties**](classgl_1_1dynamic__properties.md) & | [**operator=**](#function-operator_1) ([**dynamic\_properties**](classgl_1_1dynamic__properties.md) &&) noexcept<br> |
|   | [**~dynamic\_properties**](#function-dynamic_properties) () = default<br> |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_present**](#function-is_present) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type) & key) const<br>_Checks if a property with the given key exists._  |
|  [**ValueType**](group__GL-Core.md#variable-invalid_id_v) & | [**get**](#function-get) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type) & key) <br>_Retrieves a reference to a property cast to the specified type._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**set**](#function-set) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type) & key, [**const**](group__GL-Core.md#variable-invalid_id_v) [**ValueType**](group__GL-Core.md#variable-invalid_id_v) & value) <br>_Sets or updates a property value._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**mvset**](#function-mvset) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type) & key, [**ValueType**](group__GL-Core.md#variable-invalid_id_v) && value) <br>_Moves a value into the property map._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**emplace**](#function-emplace) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type) & key, [**Args**](group__GL-Core.md#variable-invalid_id_v) &&... args) <br>_Constructs a property value in-place._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**remove**](#function-remove) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**key\_type**](classgl_1_1dynamic__properties.md#typedef-key_type) & key) <br>_Removes the property associated with the given key._  |
|  [**property\_map\_type**](classgl_1_1dynamic__properties.md#typedef-property_map_type) & | [**underlying**](#function-underlying) () <br>_Returns a reference to the underlying property map._  |




























## Detailed Description


Stores an arbitrary number of properties identified by string keys, where each property can be of any type. This allows for dynamic attachment of properties to graph elements at runtime without requiring compile-time knowledge of the property types.


The underlying container that is used to store the properties is `std::unordered_map<std::string, std::any>`. 


    
## Public Types Documentation




### typedef key\_type 

_The type used for property keys (string identifiers)._ 
```cpp
using gl::dynamic_properties::key_type =  std::string;
```




<hr>



### typedef value\_type 

_The type used for property values (type-erased storage)._ 
```cpp
using gl::dynamic_properties::value_type =  std::any;
```




<hr>



### typedef property\_map\_type 

_The underlying map type for storing properties._ 
```cpp
using gl::dynamic_properties::property_map_type =  std::unordered_map<key_type, value_type>;
```




<hr>
## Public Functions Documentation




### function dynamic\_properties [1/3]

```cpp
gl::dynamic_properties::dynamic_properties () = default
```




<hr>



### function dynamic\_properties [2/3]

```cpp
gl::dynamic_properties::dynamic_properties (
    const  dynamic_properties &
) = default
```




<hr>



### function dynamic\_properties [3/3]

```cpp
gl::dynamic_properties::dynamic_properties (
    dynamic_properties &&
) noexcept
```




<hr>



### function operator= 

```cpp
dynamic_properties & gl::dynamic_properties::operator= (
    const  dynamic_properties &
) = default
```




<hr>



### function operator= 

```cpp
dynamic_properties & gl::dynamic_properties::operator= (
    dynamic_properties &&
) noexcept
```




<hr>



### function ~dynamic\_properties 

```cpp
gl::dynamic_properties::~dynamic_properties () = default
```




<hr>



### function is\_present 

_Checks if a property with the given key exists._ 
```cpp
inline bool gl::dynamic_properties::is_present (
    const  key_type & key
) const
```




<hr>



### function get 

_Retrieves a reference to a property cast to the specified type._ 
```cpp
template<typename  ValueType>
inline ValueType & gl::dynamic_properties::get (
    const  key_type & key
) 
```





**Template parameters:**


* `ValueType` The expected type of the property. 



**Parameters:**


* `key` The string identifier for the property. 



**Exception:**


* `std::bad_any_cast` If the property type does not match ValueType. 




        

<hr>



### function set 

_Sets or updates a property value._ 
```cpp
template<typename  ValueType>
inline void gl::dynamic_properties::set (
    const  key_type & key,
    const  ValueType & value
) 
```





**Template parameters:**


* `ValueType` The type of value being stored. 




        

<hr>



### function mvset 

_Moves a value into the property map._ 
```cpp
template<typename  ValueType>
inline void gl::dynamic_properties::mvset (
    const  key_type & key,
    ValueType && value
) 
```





**Template parameters:**


* `ValueType` The type of value being stored. 




        

<hr>



### function emplace 

_Constructs a property value in-place._ 
```cpp
template<typename  ValueType, typename... Args>
inline void gl::dynamic_properties::emplace (
    const  key_type & key,
    Args &&... args
) 
```





**Template parameters:**


* `ValueType` The type of value to construct. 




        

<hr>



### function remove 

_Removes the property associated with the given key._ 
```cpp
inline void gl::dynamic_properties::remove (
    const  key_type & key
) 
```




<hr>



### function underlying 

_Returns a reference to the underlying property map._ 
```cpp
inline property_map_type & gl::dynamic_properties::underlying () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/properties.hpp`

