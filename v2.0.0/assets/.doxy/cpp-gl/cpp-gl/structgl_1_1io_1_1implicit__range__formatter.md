

# Struct gl::io::implicit\_range\_formatter

**template &lt;std::integral T&gt;**



[**ClassList**](annotated.md) **>** [**io**](namespacegl_1_1io.md) **>** [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md)



_A proxy object for concisely formatting large contiguous numeric ranges._ [More...](#detailed-description)

* `#include <gl/io/ranges.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  T | [**first**](#variable-first)  <br>_The starting bound of the range._  |
|  T | [**last**](#variable-last)  <br>_The exclusive ending bound of the range._  |












































## Detailed Description


Instead of iterating and printing every element of a massive sequence, this formatter intelligently truncates the output using an ellipsis (e.g., `{0, ..., 99}`).


#### Example Usage




```cpp
std::cout << gl::io::implicit_range_formatter<int>{0, 100} << '\n';
std::cout << gl::io::implicit_range_formatter<int>{5, 8} << '\n';
```



**Output:** 
```text
{0, ..., 99}
{5, ..., 7}
```




#### See Also




* [**implicit\_range(first, last, inclusive)**](group__GL-IO.md#function-implicit_range)
* [**implicit\_range(last, inclusive)**](group__GL-IO.md#function-implicit_range_1)






**Template parameters:**


* `T` The integral numeric type of the range bounds. 





    
## Public Attributes Documentation




### variable first 

_The starting bound of the range._ 
```cpp
T gl::io::implicit_range_formatter< T >::first;
```




<hr>



### variable last 

_The exclusive ending bound of the range._ 
```cpp
T gl::io::implicit_range_formatter< T >::last;
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Serializes the implicit range to an output stream._ 
```cpp
inline std::ostream & io::implicit_range_formatter::operator<< (
    std::ostream & os,
    implicit_range_formatter proxy
) 
```





**Parameters:**


* `os` The target output stream. 
* `proxy` The implicit range proxy. 



**Returns:**

The stream reference for chaining. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/io/ranges.hpp`

