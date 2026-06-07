

# Struct gl::io::range\_formatter

**template &lt;std::ranges::range R&gt;**



[**ClassList**](annotated.md) **>** [**io**](namespacegl_1_1io.md) **>** [**range\_formatter**](structgl_1_1io_1_1range__formatter.md)



_A customizable stream proxy for formatting and printing C++ ranges._ [More...](#detailed-description)

* `#include <gl/io/ranges.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  R | [**range**](#variable-range)  <br>_The underlying range view being formatted._  |
|  std::string\_view | [**sep**](#variable-sep)  = `", "`<br>_The string separating each element._  |
|  std::string\_view | [**open**](#variable-open)  = `"["`<br>_The opening string/bracket._  |
|  std::string\_view | [**close**](#variable-close)  = `"]"`<br>_The closing string/bracket._  |












































## Detailed Description


This struct acts as a wrapper around any `std::ranges::range`, allowing it to be piped into a standard output stream with configurable separators and bounding characters.


#### Example Usage




```cpp
std::vector<int> vec = {1, 2, 3};
std::cout << gl::io::range_formatter{vec} << '\n';
std::cout << gl::io::range_formatter{vec, " | ", "<", ">"} << '\n';
```



**Output:** 
```text
[1, 2, 3]
<1 | 2 | 3>
```




#### See Also




* [**set\_formatter**](group__GL-IO.md#function-set_formatter)
* [**multiline\_set\_formatter**](group__GL-IO.md#function-multiline_set_formatter)






**Template parameters:**


* `R` The specific type of the range being formatted. 





    
## Public Attributes Documentation




### variable range 

_The underlying range view being formatted._ 
```cpp
R gl::io::range_formatter< R >::range;
```




<hr>



### variable sep 

_The string separating each element._ 
```cpp
std::string_view gl::io::range_formatter< R >::sep = ", ";
```




<hr>



### variable open 

_The opening string/bracket._ 
```cpp
std::string_view gl::io::range_formatter< R >::open = "[";
```




<hr>



### variable close 

_The closing string/bracket._ 
```cpp
std::string_view gl::io::range_formatter< R >::close = "]";
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Serializes the formatted range to an output stream._ 
```cpp
inline std::ostream & io::range_formatter::operator<< (
    std::ostream & os,
    range_formatter formatter
) 
```





**Parameters:**


* `os` The target output stream. 
* `formatter` The configured formatter instance. 



**Returns:**

The stream reference for chaining. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/io/ranges.hpp`

