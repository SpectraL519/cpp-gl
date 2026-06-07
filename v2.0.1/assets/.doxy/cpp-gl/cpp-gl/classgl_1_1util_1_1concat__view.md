

# Class gl::util::concat\_view

**template &lt;std::ranges::view V1, std::ranges::view V2&gt;**



[**ClassList**](annotated.md) **>** [**util**](namespacegl_1_1util.md) **>** [**concat\_view**](classgl_1_1util_1_1concat__view.md)



_A view concatenating two ranges sequentially (C++20 polyfill for C++26_ `std::views::concat` _)._[More...](#detailed-description)

* `#include <gl/util/ranges.hpp>`



Inherits the following classes: std::ranges::view_interface< concat_view< V1, V2 > >


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**concat\_view**](#function-concat_view-12) () = default<br>_Default constructor creates an empty concatenated view._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**concat\_view**](#function-concat_view-22) (V1 v1, V2 v2) <br>_Constructs a_ `concat_view` _from two viewable ranges._ |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**begin**](#function-begin-12) () <br>_Returns an iterator to the beginning of the concatenated view._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**begin**](#function-begin-22) () const<br>_Returns a const iterator to the beginning of the concatenated view._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**end**](#function-end-12) () <br>_Returns a sentinel representing the end of the concatenated view._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**end**](#function-end-22) () const<br>_Returns a const sentinel representing the end of the concatenated view._  |




























## Detailed Description



> [!WARNING] GCC 13/14 Bug
>
>
> Using branching views (like this or `std::ranges::filter_view`) inside complex algorithms (e.g., `std::ranges::is_permutation`) may trigger false-positive `-Wmaybe-uninitialized` warnings. To work around this, suppress the warning at the call site or materialize the view into a contiguous container like `std::vector`. 
>
>



**Template parameters:**


* `V1` First view type. 
* `V2` Second view type. 



**Todo**

Replace with `std::views::concat` (C++26). 




#### See Also




* [**concat\_fn**](structgl_1_1util_1_1concat__fn.md): A helper compile-time constant function object for creating `concat_view` instances.
* [**concat**](group__GL-Util.md#variable-concat): A compile-time constant instantiation of this function object for convenient use. 





    
## Public Functions Documentation




### function concat\_view [1/2]

_Default constructor creates an empty concatenated view._ 
```cpp
util::concat_view::concat_view () = default
```




<hr>



### function concat\_view [2/2]

_Constructs a_ `concat_view` _from two viewable ranges._
```cpp
inline constexpr util::concat_view::concat_view (
    V1 v1,
    V2 v2
) 
```





**Parameters:**


* `v1` The first range to concatenate. 
* `v2` The second range to concatenate. 




        

<hr>



### function begin [1/2]

_Returns an iterator to the beginning of the concatenated view._ 
```cpp
inline constexpr  auto util::concat_view::begin () 
```





**Returns:**

An iterator that traverses the first range followed by the second range. 





        

<hr>



### function begin [2/2]

_Returns a const iterator to the beginning of the concatenated view._ 
```cpp
inline constexpr  auto util::concat_view::begin () const
```





**Returns:**

A const iterator that traverses the first range followed by the second range. 





        

<hr>



### function end [1/2]

_Returns a sentinel representing the end of the concatenated view._ 
```cpp
inline constexpr  auto util::concat_view::end () 
```





**Returns:**

A sentinel that compares equal to an iterator when it reaches the end of the second range. 





        

<hr>



### function end [2/2]

_Returns a const sentinel representing the end of the concatenated view._ 
```cpp
inline constexpr  auto util::concat_view::end () const
```





**Returns:**

A const sentinel that compares equal to a const iterator when it reaches the end 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/util/ranges.hpp`

