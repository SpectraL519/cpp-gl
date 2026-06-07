

# Group GL-IO



[**Modules**](modules.md) **>** [**GL-IO**](group__GL-IO.md)



_I/O stream operations, formatting, and serialization of graph data._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**gl::io::append**](structgl_1_1io_1_1append.md) <br>_Tag type specifying that a file should be opened in append mode._  |
| struct | [**gl::io::implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md) &lt;T&gt;<br>_A proxy object for concisely formatting large contiguous numeric ranges._  |
| class | [**gl::io::options\_manip**](classgl_1_1io_1_1options__manip.md) <br>_A custom stream manipulator for modifying formatting options on standard I/O streams._  |
| struct | [**gl::io::range\_formatter**](structgl_1_1io_1_1range__formatter.md) &lt;R&gt;<br>_A customizable stream proxy for formatting and printing C++ ranges._  |
| struct | [**gl::io::write**](structgl_1_1io_1_1write.md) <br>_Tag type specifying that a file should be opened in strictly write (truncate) mode._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef unsigned | [**bit\_position\_type**](#typedef-bit_position_type)  <br>_Type representing the zero-indexed position of a specific option bit._  |
| typedef int | [**index\_type**](#typedef-index_type)  <br>_Type used for standard I/O stream custom index allocation (_ `std::ios_base::xalloc` _)._ |
| typedef long | [**iword\_type**](#typedef-iword_type)  <br>_Type used for storing custom option flags within standard I/O streams._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr options\_manip | [**concise**](#variable-concise)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable concise graph formatting._ |
|  constexpr options\_manip | [**default\_options**](#variable-default_options)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to reset all custom graph formatting flags back to their default states._ |
|  constexpr iword\_type | [**iword\_bit**](#variable-iword_bit)  = `1ul`<br>_Base bit representing the a single position in an_ `iword` _flag map._ |
|  constexpr options\_manip | [**spec\_fmt**](#variable-spec_fmt)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the Graph Specification Format (GSF)._ |
|  constexpr options\_manip | [**verbose**](#variable-verbose)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable verbose graph formatting._ |
|  constexpr options\_manip | [**with\_edge\_properties**](#variable-with_edge_properties)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the processing of edge properties._ |
|  constexpr options\_manip | [**with\_properties**](#variable-with_properties)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the processing of both vertex and edge properties simultaneously._ |
|  constexpr options\_manip | [**with\_vertex\_properties**](#variable-with_vertex_properties)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the processing of vertex properties._ |
|  constexpr options\_manip | [**without\_edge\_properties**](#variable-without_edge_properties)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to disable the processing of edge properties._ |
|  constexpr options\_manip | [**without\_properties**](#variable-without_properties)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to disable the processing of both vertex and edge properties simultaneously._ |
|  constexpr options\_manip | [**without\_vertex\_properties**](#variable-without_vertex_properties)  <br>[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to disable the processing of vertex properties._ |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  bool | [**are\_options\_set**](#function-are_options_set) (std::ios\_base & stream, iword\_type bitmask) <br>_Convenience wrapper to check if a combined bitmask of options is set on the stream._  |
|  bool | [**are\_options\_set**](#function-are_options_set) (std::ios\_base & stream, Args... bits) <br>_Convenience wrapper to check if multiple specific options are simultaneously set._  |
|  bool | [**are\_options\_set**](#function-are_options_set) (std::ios\_base & stream, std::initializer\_list&lt; T &gt; bits) <br>_Convenience wrapper to check if multiple specific options from a list are simultaneously set._  |
|  constexpr options\_manip | [**clear\_options**](#function-clear_options) (Args... bits) <br>_Creates a stream manipulator that disables the specified option bits._  |
|  constexpr options\_manip | [**clear\_options**](#function-clear_options) (std::initializer\_list&lt; T &gt; bits) <br>_Creates a stream manipulator that disables the specified option bits from a list._  |
|  constexpr implicit\_range\_formatter&lt; T &gt; | [**implicit\_range**](#function-implicit_range) (T first, T last, bool inclusive=false) <br>_Factory function to create an implicit range formatter with a defined start and end._  |
|  constexpr implicit\_range\_formatter&lt; T &gt; | [**implicit\_range**](#function-implicit_range) (T last, bool inclusive=false) <br>_Factory function to create an implicit range formatter starting from 0._  |
|  bool | [**is\_option\_set**](#function-is_option_set) (std::ios\_base & stream, bit\_position\_type bit\_position) <br>_Convenience wrapper to check if a specific option bit is set on the stream._  |
|  bool | [**is\_option\_set**](#function-is_option_set) (std::ios\_base & stream, traits::c\_enum auto bit) <br>_Convenience wrapper to check if a specific enum-based option is set on the stream._  |
|  GraphType | [**load**](#function-load) (const std::filesystem::path & path) <br>_Deserializes and loads a graph from a file._  |
|  auto | [**multiline\_set\_formatter**](#function-multiline_set_formatter) (R && range) <br>_Factory function to format a range as a multiline set._  |
|  void | [**save**](#function-save) (const GraphType & graph, const std::filesystem::path & path="graph.gsf", const std::initializer\_list&lt; options\_manip &gt; & options={}) <br>_Serializes and saves a graph to a file._  |
|  auto | [**set\_formatter**](#function-set_formatter) (R && range, std::string\_view sep=", ") <br>_Factory function to format a range as a set._  |
|  constexpr options\_manip | [**set\_options**](#function-set_options) (Args... bits) <br>_Creates a stream manipulator that enables the specified option bits._  |
|  constexpr options\_manip | [**set\_options**](#function-set_options) (std::initializer\_list&lt; T &gt; bits) <br>_Creates a stream manipulator that enables the specified option bits from a list._  |




























## Detailed Description


This group provides the necessary infrastructure to seamlessly serialize, deserialize, and visualize complex graph structures. By offering a robust set of stream manipulators, range formatting utilities, and configuration options, it allows users to easily translate in-memory graphs to and from standard streams, files, or custom string representations. 


    
## Public Types Documentation




### typedef bit\_position\_type 

_Type representing the zero-indexed position of a specific option bit._ 
```cpp
using gl::io::bit_position_type = typedef unsigned;
```




<hr>



### typedef index\_type 

_Type used for standard I/O stream custom index allocation (_ `std::ios_base::xalloc` _)._
```cpp
using gl::io::index_type = typedef int;
```




<hr>



### typedef iword\_type 

_Type used for storing custom option flags within standard I/O streams._ 
```cpp
using gl::io::iword_type = typedef long;
```




<hr>
## Public Attributes Documentation




### variable concise 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable concise graph formatting._
```cpp
constexpr options_manip gl::io::concise;
```



Clears all layout-specific flags to default back to a compact representation. 


        

<hr>



### variable default\_options 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to reset all custom graph formatting flags back to their default states._
```cpp
constexpr options_manip gl::io::default_options;
```




<hr>



### variable iword\_bit 

_Base bit representing the a single position in an_ `iword` _flag map._
```cpp
constexpr iword_type gl::io::iword_bit = 1ul;
```




<hr>



### variable spec\_fmt 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the Graph Specification Format (GSF)._
```cpp
constexpr options_manip gl::io::spec_fmt;
```



Modifies the stream to output or expect data matching the precise internal parsing format used for serialization and deserialization. 


        

<hr>



### variable verbose 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable verbose graph formatting._
```cpp
constexpr options_manip gl::io::verbose;
```



Modifies the stream state to output detailed structural information. 


        

<hr>



### variable with\_edge\_properties 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the processing of edge properties._
```cpp
constexpr options_manip gl::io::with_edge_properties;
```




<hr>



### variable with\_properties 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the processing of both vertex and edge properties simultaneously._
```cpp
constexpr options_manip gl::io::with_properties;
```




<hr>



### variable with\_vertex\_properties 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to enable the processing of vertex properties._
```cpp
constexpr options_manip gl::io::with_vertex_properties;
```




<hr>



### variable without\_edge\_properties 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to disable the processing of edge properties._
```cpp
constexpr options_manip gl::io::without_edge_properties;
```




<hr>



### variable without\_properties 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to disable the processing of both vertex and edge properties simultaneously._
```cpp
constexpr options_manip gl::io::without_properties;
```




<hr>



### variable without\_vertex\_properties 

[_**Stream manipulator**_](classgl_1_1io_1_1options__manip.md) _to disable the processing of vertex properties._
```cpp
constexpr options_manip gl::io::without_vertex_properties;
```




<hr>
## Public Functions Documentation




### function are\_options\_set 

_Convenience wrapper to check if a combined bitmask of options is set on the stream._ 
```cpp
bool are_options_set (
    std::ios_base & stream,
    iword_type bitmask
) 
```





**Parameters:**


* `stream` The stream to check. 
* `bitmask` The exact bitmask to verify. 



**Returns:**

`true` if all bits within the mask are strictly set, `false` otherwise. 





        

<hr>



### function are\_options\_set 

_Convenience wrapper to check if multiple specific options are simultaneously set._ 
```cpp
template<typename... Args>
bool are_options_set (
    std::ios_base & stream,
    Args... bits
) 
```





**Template parameters:**


* `Args` Variadic template arguments representing the bits. 



**Parameters:**


* `stream` The stream to check. 
* `bits` The specific options/bits to verify. 



**Returns:**

`true` if all specified bits are set, `false` otherwise. 





        

<hr>



### function are\_options\_set 

_Convenience wrapper to check if multiple specific options from a list are simultaneously set._ 
```cpp
template<typename T>
bool are_options_set (
    std::ios_base & stream,
    std::initializer_list< T > bits
) 
```





**Template parameters:**


* `T` The type of the elements in the initializer list. 



**Parameters:**


* `stream` The stream to check. 
* `bits` An initializer list containing the options/bits to verify. 



**Returns:**

`true` if all specified bits are set, `false` otherwise. 





        

<hr>



### function clear\_options 

_Creates a stream manipulator that disables the specified option bits._ 
```cpp
template<typename... Args>
constexpr options_manip clear_options (
    Args... bits
) 
```





**Template parameters:**


* `Args` Variadic template arguments representing the bits to clear. 



**Parameters:**


* `bits` The specific options/bits to disable. 



**Returns:**

An [**options\_manip**](classgl_1_1io_1_1options__manip.md) object ready to be piped into a stream. 





        

<hr>



### function clear\_options 

_Creates a stream manipulator that disables the specified option bits from a list._ 
```cpp
template<typename T>
constexpr options_manip clear_options (
    std::initializer_list< T > bits
) 
```





**Template parameters:**


* `T` The type of the bits. 



**Parameters:**


* `bits` An initializer list containing the options/bits to disable. 



**Returns:**

An [**options\_manip**](classgl_1_1io_1_1options__manip.md) object ready to be piped into a stream. 





        

<hr>



### function implicit\_range 

_Factory function to create an implicit range formatter with a defined start and end._ 
```cpp
template<std::integral T>
constexpr implicit_range_formatter< T > implicit_range (
    T first,
    T last,
    bool inclusive=false
) 
```



#### Example Usage




```cpp
std::cout << gl::io::implicit_range(10, 100) << '\n';
std::cout << gl::io::implicit_range(10, 100, true) << '\n';
```



**Output:** 
```text
{10, ..., 99}
{10, ..., 100}
```




#### See Also




* [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md)
* [**implicit\_range(last, inclusive)**](group__GL-IO.md#function-implicit_range_1)






**Template parameters:**


* `T` The integral numeric type of the range. 



**Parameters:**


* `first` The starting value of the range. 
* `last` The ending value of the range. 
* `inclusive` Set to `true` if `last` should be included in the bounds, `false` otherwise. 



**Returns:**

An [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md) representing the bounds. 






        

<hr>



### function implicit\_range 

_Factory function to create an implicit range formatter starting from 0._ 
```cpp
template<std::integral T>
constexpr implicit_range_formatter< T > implicit_range (
    T last,
    bool inclusive=false
) 
```



#### Example Usage




```cpp
std::cout << gl::io::implicit_range(100) << '\n';
std::cout << gl::io::implicit_range(100, true) << '\n';
```



**Output:** 
```text
{0, ..., 99}
{0, ..., 100}
```




#### See Also




* [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md)
* [**implicit\_range(first, last, inclusive)**](group__GL-IO.md#function-implicit_range)






**Template parameters:**


* `T` The integral numeric type of the range. 



**Parameters:**


* `last` The ending value of the range. 
* `inclusive` Set to `true` if `last` should be included in the bounds, `false` otherwise. 



**Returns:**

An [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md) representing bounds from `0` to `last`. 






        

<hr>



### function is\_option\_set 

_Convenience wrapper to check if a specific option bit is set on the stream._ 
```cpp
bool is_option_set (
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



### function is\_option\_set 

_Convenience wrapper to check if a specific enum-based option is set on the stream._ 
```cpp
bool is_option_set (
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



### function load 

_Deserializes and loads a graph from a file._ 
```cpp
template<traits::c_graph GraphType>
GraphType load (
    const std::filesystem::path & path
) 
```



Instantiates a new graph populated with the topology and properties read from the target GSF file.




**Template parameters:**


* `GraphType` The target graph type to construct. Must match the directional nature of the saved graph. 



**Parameters:**


* `path` The filesystem path from which to load the graph. 



**Returns:**

A newly constructed graph populated with the file's data.




**Exception:**


* `std::filesystem::filesystem_error` If the file does not exist or is not a standard file. 
* `std::ios_base::failure` If the file cannot be opened or if the GSF directional discriminator mismatches `GraphType`. 




        

<hr>



### function multiline\_set\_formatter 

_Factory function to format a range as a multiline set._ 
```cpp
template<std::ranges::range R>
auto multiline_set_formatter (
    R && range
) 
```



Outputs each element on a new line, bounded by curly braces `{}`.


#### Usage example




```cpp
std::vector<std::string_view> fruits = {"banana", "apple", "cherry"};
std::cout << "fruits: " << gl::io::set_formatter(vec) << '\n';
```



**Output**: 
```text
fruits: {
  banana,
  apple,
  cherry
}
```





**Todo**

Add an indent\_width parameter






**Template parameters:**


* `R` The type of the range. 



**Parameters:**


* `range` The range object to format. 



**Returns:**

A [**range\_formatter**](structgl_1_1io_1_1range__formatter.md) configured for multiline set-style output. 






#### See Also




* [**set\_formatter**](group__GL-IO.md#function-set_formatter) 





        

<hr>



### function save 

_Serializes and saves a graph to a file._ 
```cpp
template<traits::c_graph GraphType, traits::c_io_save_mode Mode>
void save (
    const GraphType & graph,
    const std::filesystem::path & path="graph.gsf",
    const std::initializer_list< options_manip > & options={}
) 
```



Saves the graph topology and optionally its properties using the Graph Specification Format (GSF). The function strictly respects the [**write**](structgl_1_1io_1_1write.md) and [**append**](structgl_1_1io_1_1append.md) safety guards.




**Template parameters:**


* `GraphType` The concrete type of the graph being saved. Must satisfy [**c\_graph**](gl_concepts.md#gl-traits-c-graph). 
* `Mode` The save behavior tag ([**write**](structgl_1_1io_1_1write.md) or [**append**](structgl_1_1io_1_1append.md)). Defaults to `write`. 



**Parameters:**


* `graph` The graph instance to serialize. 
* `path` The filesystem path where the graph will be saved. Defaults to `"graph.gsf"`. 
* `options` An initializer list of [**options\_manip**](classgl_1_1io_1_1options__manip.md) to configure the serialization (e.g., `gl::io::with_properties`).



**Exception:**


* `std::filesystem::filesystem_error` If file safety checks fail (e.g., overwriting an existing file in `write` mode). 
* `std::ios_base::failure` If the underlying file stream cannot be opened. 




        

<hr>



### function set\_formatter 

_Factory function to format a range as a set._ 
```cpp
template<std::ranges::range R>
auto set_formatter (
    R && range,
    std::string_view sep=", "
) 
```



Wraps the range output in curly braces `{}` instead of standard brackets `[]`.


#### Usage example




```cpp
std::vector<int> vec = {1, 2, 3};
std::cout << gl::io::set_formatter(vec) << '\n';
```



**Output**: 
```text
{1, 2, 3}
```





**Template parameters:**


* `R` The type of the range. 



**Parameters:**


* `range` The range object to format. 
* `sep` The separator string between elements. Defaults to `", "`. 



**Returns:**

A [**range\_formatter**](structgl_1_1io_1_1range__formatter.md) configured for set-style output. 






#### See Also




* [**multiline\_set\_formatter**](group__GL-IO.md#function-multiline_set_formatter) 





        

<hr>



### function set\_options 

_Creates a stream manipulator that enables the specified option bits._ 
```cpp
template<typename... Args>
constexpr options_manip set_options (
    Args... bits
) 
```





**Template parameters:**


* `Args` Variadic template arguments representing the bits to set. 



**Parameters:**


* `bits` The specific options/bits to enable. 



**Returns:**

An [**options\_manip**](classgl_1_1io_1_1options__manip.md) object ready to be piped into a stream. 





        

<hr>



### function set\_options 

_Creates a stream manipulator that enables the specified option bits from a list._ 
```cpp
template<typename T>
constexpr options_manip set_options (
    std::initializer_list< T > bits
) 
```





**Template parameters:**


* `T` The type of the bits. 



**Parameters:**


* `bits` An initializer list containing the options/bits to enable. 



**Returns:**

An [**options\_manip**](classgl_1_1io_1_1options__manip.md) object ready to be piped into a stream. 





        

<hr>

------------------------------


