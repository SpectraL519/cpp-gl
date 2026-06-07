

# Struct hgl::repr::hyperedge\_major\_t



[**ClassList**](annotated.md) **>** [**repr**](namespacehgl_1_1repr.md) **>** [**hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md)



_Layout tag designating hyperedges as the primary structural dimension of the incidence representation._ 

* `#include <hgl/repr/layout_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**hyperedge\_t**](structhgl_1_1hyperedge__t.md) | [**major\_element**](#typedef-major_element)  <br>_The major element type for this layout._  |
| typedef [**vertex\_t**](structhgl_1_1vertex__t.md) | [**minor\_element**](#typedef-minor_element)  <br>_The minor element type for this layout._  |






















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](group__HGL-Core.md#variable-invalid_id_v) T | [**major**](#function-major) ([**const**](group__HGL-Core.md#variable-invalid_id_v) T & vertex\_el, [**const**](group__HGL-Core.md#variable-invalid_id_v) T & hyperedge\_el) noexcept<br>_Retrieves the major element from the provided arguments based on the layout rules._  |
|  [**constexpr**](group__HGL-Core.md#variable-invalid_id_v) T | [**minor**](#function-minor) ([**const**](group__HGL-Core.md#variable-invalid_id_v) T & vertex\_el, [**const**](group__HGL-Core.md#variable-invalid_id_v) T & hyperedge\_el) noexcept<br>_Retrieves the minor element from the provided arguments based on the layout rules._  |
|  [**constexpr**](group__HGL-Core.md#variable-invalid_id_v) [**homogeneous\_pair**](group__HGL-Types.md#typedef-homogeneous_pair)&lt; T &gt; | [**majmin**](#function-majmin) ([**const**](group__HGL-Core.md#variable-invalid_id_v) T & vertex\_el, [**const**](group__HGL-Core.md#variable-invalid_id_v) T & hyperedge\_el) noexcept<br>_Packages data associated with the vertex and hyperedge elements into a strictly ordered major-minor pair._  |


























## Public Types Documentation




### typedef major\_element 

_The major element type for this layout._ 
```cpp
using hgl::repr::hyperedge_major_t::major_element =  hyperedge_t;
```




<hr>



### typedef minor\_element 

_The minor element type for this layout._ 
```cpp
using hgl::repr::hyperedge_major_t::minor_element =  vertex_t;
```




<hr>
## Public Static Functions Documentation




### function major 

_Retrieves the major element from the provided arguments based on the layout rules._ 
```cpp
template<std::regular T>
static inline constexpr T repr::hyperedge_major_t::major (
    const T & vertex_el,
    const T & hyperedge_el
) noexcept
```




<hr>



### function minor 

_Retrieves the minor element from the provided arguments based on the layout rules._ 
```cpp
template<std::regular T>
static inline constexpr T repr::hyperedge_major_t::minor (
    const T & vertex_el,
    const T & hyperedge_el
) noexcept
```




<hr>



### function majmin 

_Packages data associated with the vertex and hyperedge elements into a strictly ordered major-minor pair._ 
```cpp
template<std::regular T>
static inline constexpr  homogeneous_pair < T > repr::hyperedge_major_t::majmin (
    const T & vertex_el,
    const T & hyperedge_el
) noexcept
```





**Template parameters:**


* `T` The type associated with hypergraph's elements to be ordered. 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/repr/layout_tags.hpp`

