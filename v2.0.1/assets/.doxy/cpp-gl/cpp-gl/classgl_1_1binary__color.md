

# Class gl::binary\_color



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**binary\_color**](classgl_1_1binary__color.md)



_A specialized color property for algorithms requiring binary states (e.g., bipartition)._ 

* `#include <gl/types/properties.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint8\_t | [**value**](#enum-value)  <br>_The discrete states available for binary coloring._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**binary\_color**](#function-binary_color-14) () = default<br> |
|   | [**binary\_color**](#function-binary_color-24) ([**value**](classgl_1_1binary__color.md#enum-value) value) <br> |
|   | [**binary\_color**](#function-binary_color-34) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**binary\_color**](classgl_1_1binary__color.md) &) = default<br> |
|   | [**binary\_color**](#function-binary_color-44) ([**binary\_color**](classgl_1_1binary__color.md) &&) noexcept<br> |
|  [**binary\_color**](classgl_1_1binary__color.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**binary\_color**](classgl_1_1binary__color.md) &) = default<br> |
|  [**binary\_color**](classgl_1_1binary__color.md) & | [**operator=**](#function-operator_1) ([**binary\_color**](classgl_1_1binary__color.md) &&) noexcept<br> |
|   | [**~binary\_color**](#function-binary_color) () = default<br> |
|  [**binary\_color**](classgl_1_1binary__color.md) & | [**operator=**](#function-operator_2) ([**value**](classgl_1_1binary__color.md#enum-value) value) <br> |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**operator&lt;=&gt;**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**binary\_color**](classgl_1_1binary__color.md) &) const<br> |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_3) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**binary\_color**](classgl_1_1binary__color.md) &) const<br> |
|   | [**operator bool**](#function-operator-bool) () const<br>_Returns_ `true` _if the color is either Black or White (i.e., not Unset)._ |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_set**](#function-is_set) () const<br>_Explicit check to see if the color state is not_ `unset` _._ |
|  std::underlying\_type\_t&lt; [**value**](classgl_1_1binary__color.md#enum-value) &gt; | [**to\_underlying**](#function-to_underlying) () const<br>_Returns the integer representation of the current color state._  |
|  [**binary\_color**](classgl_1_1binary__color.md) | [**next**](#function-next) () const<br>_Returns a new_ `binary_color` _representing the opposite state (Black &lt;-&gt; White)._ |




























## Public Types Documentation




### enum value 

_The discrete states available for binary coloring._ 
```cpp
enum gl::binary_color::value {
    black = static_cast<std::uint8_t>(0),
    white = static_cast<std::uint8_t>(1),
    unset = static_cast<std::uint8_t>(2)
};
```




<hr>
## Public Functions Documentation




### function binary\_color [1/4]

```cpp
gl::binary_color::binary_color () = default
```




<hr>



### function binary\_color [2/4]

```cpp
inline gl::binary_color::binary_color (
    value value
) 
```




<hr>



### function binary\_color [3/4]

```cpp
gl::binary_color::binary_color (
    const  binary_color &
) = default
```




<hr>



### function binary\_color [4/4]

```cpp
gl::binary_color::binary_color (
    binary_color &&
) noexcept
```




<hr>



### function operator= 

```cpp
binary_color & gl::binary_color::operator= (
    const  binary_color &
) = default
```




<hr>



### function operator= 

```cpp
binary_color & gl::binary_color::operator= (
    binary_color &&
) noexcept
```




<hr>



### function ~binary\_color 

```cpp
gl::binary_color::~binary_color () = default
```




<hr>



### function operator= 

```cpp
inline binary_color & gl::binary_color::operator= (
    value value
) 
```




<hr>



### function operator&lt;=&gt; 

```cpp
auto gl::binary_color::operator<=> (
    const  binary_color &
) const
```




<hr>



### function operator== 

```cpp
bool gl::binary_color::operator== (
    const  binary_color &
) const
```




<hr>



### function operator bool 

_Returns_ `true` _if the color is either Black or White (i.e., not Unset)._
```cpp
inline gl::binary_color::operator bool () const
```




<hr>



### function is\_set 

_Explicit check to see if the color state is not_ `unset` _._
```cpp
inline bool gl::binary_color::is_set () const
```




<hr>



### function to\_underlying 

_Returns the integer representation of the current color state._ 
```cpp
inline std::underlying_type_t< value > gl::binary_color::to_underlying () const
```




<hr>



### function next 

_Returns a new_ `binary_color` _representing the opposite state (Black &lt;-&gt; White)._
```cpp
inline binary_color gl::binary_color::next () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/properties.hpp`

