

# Class gl::io::options\_manip



[**ClassList**](annotated.md) **>** [**io**](namespacegl_1_1io.md) **>** [**options\_manip**](classgl_1_1io_1_1options__manip.md)



_A custom stream manipulator for modifying formatting options on standard I/O streams._ [More...](#detailed-description)

* `#include <gl/io/options_manip.hpp>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**options\_manip**](#function-options_manip-12) () = delete<br>_Default constructor is disabled._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**options\_manip**](#function-options_manip-22) ([**iword\_type**](group__GL-IO.md#typedef-iword_type) set\_mask, [**iword\_type**](group__GL-IO.md#typedef-iword_type) clear\_mask=0ul) <br>_Constructs a manipulator with specific state masks._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_option\_set**](#function-is_option_set-12) (std::ios\_base & stream, [**bit\_position\_type**](group__GL-IO.md#typedef-bit_position_type) bit\_position) <br>_Checks if a specific option bit is currently enabled on the given stream._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_option\_set**](#function-is_option_set-22) (std::ios\_base & stream, traits::c\_enum [**auto**](group__GL-Core.md#variable-invalid_id_v) bit) <br>_Checks if a specific enum-based option is currently enabled on the given stream._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**are\_options\_set**](#function-are_options_set) (std::ios\_base & stream, [**iword\_type**](group__GL-IO.md#typedef-iword_type) bitmask) <br>_Checks if a specific sequence of bits matches the exact stream state._  |


























## Detailed Description


Utilizing `std::ios_base::xalloc` and the `iword` map, this class safely injects custom formatting state directly into C++ standard streams. It acts as an intermediate proxy object produced by functions like [**set\_options**](group__GL-IO.md#function-set_options) and [**clear\_options**](group__GL-IO.md#function-clear_options). 


    
## Public Functions Documentation




### function options\_manip [1/2]

_Default constructor is disabled._ 
```cpp
io::options_manip::options_manip () = delete
```




<hr>



### function options\_manip [2/2]

_Constructs a manipulator with specific state masks._ 
```cpp
inline explicit constexpr io::options_manip::options_manip (
    iword_type set_mask,
    iword_type clear_mask=0ul
) 
```





**Parameters:**


* `set_mask` The bitmask of options to enable. 
* `clear_mask` The bitmask of options to disable (defaults to 0). 




        

<hr>
## Public Static Functions Documentation




### function is\_option\_set [1/2]

_Checks if a specific option bit is currently enabled on the given stream._ 
```cpp
static inline bool io::options_manip::is_option_set (
    std::ios_base & stream,
    bit_position_type bit_position
) 
```





**Parameters:**


* `stream` The stream to check. 
* `bit_position` The numeric position of the bit to verify. 



**Returns:**

`true` if the specified bit is set, `false` otherwise. 





        

<hr>



### function is\_option\_set [2/2]

_Checks if a specific enum-based option is currently enabled on the given stream._ 
```cpp
static inline bool io::options_manip::is_option_set (
    std::ios_base & stream,
    traits::c_enum auto bit
) 
```





**Parameters:**


* `stream` The stream to check. 
* `bit` The scoped enum value representing the bit to verify. 



**Returns:**

`true` if the specified bit is set, `false` otherwise. 





        

<hr>



### function are\_options\_set 

_Checks if a specific sequence of bits matches the exact stream state._ 
```cpp
static inline bool io::options_manip::are_options_set (
    std::ios_base & stream,
    iword_type bitmask
) 
```





**Parameters:**


* `stream` The stream to check. 
* `bitmask` The bitmask to verify against the stream's state. 



**Returns:**

`true` if all bits within the mask are strictly set, `false` otherwise. 





        

<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Applies the manipulator's formatting masks to an output stream._ 
```cpp
inline std::ostream & io::options_manip::operator<< (
    std::ostream & os,
    const  options_manip & manip
) 
```





**Parameters:**


* `os` The target output stream. 
* `manip` The manipulator containing the masks to apply. 



**Returns:**

The stream reference for chaining. 





        

<hr>



### friend operator&gt;&gt; 

_Applies the manipulator's formatting masks to an input stream._ 
```cpp
inline std::istream & io::options_manip::operator>> (
    std::istream & is,
    const  options_manip & manip
) 
```





**Parameters:**


* `is` The target input stream. 
* `manip` The manipulator containing the masks to apply. 



**Returns:**

The stream reference for chaining. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/io/options_manip.hpp`

