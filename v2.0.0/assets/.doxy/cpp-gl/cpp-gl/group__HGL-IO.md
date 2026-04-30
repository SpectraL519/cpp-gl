

# Group HGL-IO



[**Modules**](modules.md) **>** [**HGL-IO**](group__HGL-IO.md)



_I/O stream operations, formatting, and serialization of hypergraph data._ [More...](#detailed-description)


















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**gl::io::append**](structgl_1_1io_1_1append.md) | [**append**](#typedef-append)  <br>_Tag type specifying that a file should be opened in append mode._  |
| typedef [**gl::io::implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md)&lt; T &gt; | [**implicit\_range\_formatter**](#typedef-implicit_range_formatter)  <br>_A proxy object for concisely formatting large contiguous numeric ranges._  |
| typedef [**gl::io::options\_manip**](classgl_1_1io_1_1options__manip.md) | [**options\_manip**](#typedef-options_manip)  <br>_A custom stream manipulator for modifying formatting options on standard I/O streams._  |
| typedef [**gl::io::range\_formatter**](structgl_1_1io_1_1range__formatter.md)&lt; R &gt; | [**range\_formatter**](#typedef-range_formatter)  <br>_A customizable stream proxy for formatting and printing C++ ranges._  |
| typedef [**gl::io::write**](structgl_1_1io_1_1write.md) | [**write**](#typedef-write)  <br>_Tag type specifying that a file should be opened in strictly write (truncate) mode._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr options\_manip | [**concise**](#variable-concise)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable concise hypergraph formatting._ |
|  constexpr options\_manip | [**default\_options**](#variable-default_options)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to reset all custom graph formatting flags back to their default states._ |
|  constexpr options\_manip | [**spec\_fmt**](#variable-spec_fmt)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable verbose hypergraph formatting._ |
|  constexpr options\_manip | [**verbose**](#variable-verbose)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the Hypergraph Specification Format (HGSF)._ |
|  constexpr options\_manip | [**with\_hyperedge\_properties**](#variable-with_hyperedge_properties)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the processing of hyperedge properties._ |
|  constexpr options\_manip | [**with\_properties**](#variable-with_properties)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the processing of both vertex and hyperedge properties simultaneously._ |
|  constexpr options\_manip | [**with\_vertex\_properties**](#variable-with_vertex_properties)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the processing of vertex properties._ |
|  constexpr options\_manip | [**without\_hyperedge\_properties**](#variable-without_hyperedge_properties)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to disable the processing of hyperedge properties._ |
|  constexpr options\_manip | [**without\_properties**](#variable-without_properties)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to disable the processing of both vertex and hyperedge properties simultaneously._ |
|  constexpr options\_manip | [**without\_vertex\_properties**](#variable-without_vertex_properties)  <br>[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to disable the processing of vertex properties._ |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  HypergraphType | [**load**](#function-load) (const std::filesystem::path & path) <br>_Deserializes and loads a hypergraph from a file._  |
|  void | [**save**](#function-save) (const HypergraphType & hypergraph, const std::filesystem::path & path="hypergraph.hgsf", const std::initializer\_list&lt; [**gl::io::options\_manip**](classgl_1_1io_1_1options__manip.md) &gt; & options={}) <br>_Serializes and saves a hypergraph to a file._  |




























## Detailed Description


Provides the necessary infrastructure to seamlessly serialize, deserialize, and visualize complex hypergraph structures. It utilizes shared stream manipulation options and range formatting utilities to easily translate in-memory hypergraphs to and from standard streams and files. 


    
## Public Types Documentation




### typedef append 

_Tag type specifying that a file should be opened in append mode._ 
```cpp
using hgl::io::append = typedef gl::io::append;
```



When using this mode, the operation will append data to an existing file. It will throw an exception if the target file does not already exist.   


        

<hr>



### typedef implicit\_range\_formatter 

_A proxy object for concisely formatting large contiguous numeric ranges._ 
```cpp
template<std::integral T>
using hgl::io::implicit_range_formatter = typedef gl::io::implicit_range_formatter<T>;
```





**See also:** [**gl::io::implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md) 



        

<hr>



### typedef options\_manip 

_A custom stream manipulator for modifying formatting options on standard I/O streams._ 
```cpp
using hgl::io::options_manip = typedef gl::io::options_manip;
```





**See also:** [**gl::io::options\_manip**](classgl_1_1io_1_1options__manip.md) 



        

<hr>



### typedef range\_formatter 

_A customizable stream proxy for formatting and printing C++ ranges._ 
```cpp
template<std::ranges::range R>
using hgl::io::range_formatter = typedef gl::io::range_formatter<R>;
```





**See also:** [**gl::io::range\_formatter**](structgl_1_1io_1_1range__formatter.md) 



        

<hr>



### typedef write 

_Tag type specifying that a file should be opened in strictly write (truncate) mode._ 
```cpp
using hgl::io::write = typedef gl::io::write;
```



When using this mode, the operation will intentionally throw an exception if the target file already exists to prevent accidental data overwrites.   


        

<hr>
## Public Attributes Documentation




### variable concise 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable concise hypergraph formatting._
```cpp
constexpr options_manip hgl::io::concise;
```



Clears all layout-specific flags to default back to a compact representation. 


        

<hr>



### variable default\_options 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to reset all custom graph formatting flags back to their default states._
```cpp
constexpr options_manip hgl::io::default_options;
```




<hr>



### variable spec\_fmt 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable verbose hypergraph formatting._
```cpp
constexpr options_manip hgl::io::spec_fmt;
```



Modifies the stream state to output detailed structural information. 


        

<hr>



### variable verbose 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the Hypergraph Specification Format (HGSF)._
```cpp
constexpr options_manip hgl::io::verbose;
```



Modifies the stream to output or expect data matching the precise internal parsing format used for serialization and deserialization. 


        

<hr>



### variable with\_hyperedge\_properties 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the processing of hyperedge properties._
```cpp
constexpr options_manip hgl::io::with_hyperedge_properties;
```




<hr>



### variable with\_properties 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the processing of both vertex and hyperedge properties simultaneously._
```cpp
constexpr options_manip hgl::io::with_properties;
```




<hr>



### variable with\_vertex\_properties 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to enable the processing of vertex properties._
```cpp
constexpr options_manip hgl::io::with_vertex_properties;
```




<hr>



### variable without\_hyperedge\_properties 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to disable the processing of hyperedge properties._
```cpp
constexpr options_manip hgl::io::without_hyperedge_properties;
```




<hr>



### variable without\_properties 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to disable the processing of both vertex and hyperedge properties simultaneously._
```cpp
constexpr options_manip hgl::io::without_properties;
```




<hr>



### variable without\_vertex\_properties 

[_**Stream manipulator**_](group__HGL-IO.md#typedef-options_manip) _to disable the processing of vertex properties._
```cpp
constexpr options_manip hgl::io::without_vertex_properties;
```




<hr>
## Public Functions Documentation




### function load 

_Deserializes and loads a hypergraph from a file._ 
```cpp
template<traits::c_hypergraph HypergraphType>
HypergraphType load (
    const std::filesystem::path & path
) 
```



Instantiates a new hypergraph populated with the topology and properties read from the target HGSF file.




**Template parameters:**


* `HypergraphType` The target hypergraph type to construct. Must match the directional nature of the saved hypergraph. 



**Parameters:**


* `path` The filesystem path from which to load the hypergraph. 



**Returns:**

A newly constructed hypergraph populated with the file's data.




**Exception:**


* `std::filesystem::filesystem_error` If the file does not exist or is not a regular file. 
* `std::ios_base::failure` If the file cannot be opened or if the HGSF directional discriminator mismatches `HypergraphType`. 




        

<hr>



### function save 

_Serializes and saves a hypergraph to a file._ 
```cpp
template<traits::c_hypergraph HypergraphType, traits::c_io_save_mode Mode>
void save (
    const HypergraphType & hypergraph,
    const std::filesystem::path & path="hypergraph.hgsf",
    const std::initializer_list< gl::io::options_manip > & options={}
) 
```



Writes the topology and optionally the properties of the hypergraph to the specified file using the Hypergraph Specification Format (HGSF). By default, The function strictly respects the [**write**](group__HGL-IO.md#typedef-write) and [**append**](group__HGL-IO.md#typedef-append) safety guards.




**Template parameters:**


* `HypergraphType` The concrete type of the hypergraph being saved. Must satisfy [**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph). 
* `Mode` The file access mode (e.g., [**write**](group__HGL-IO.md#typedef-write) or [**append**](group__HGL-IO.md#typedef-append)). 



**Parameters:**


* `hypergraph` The hypergraph instance to serialize. 
* `path` The filesystem path where the hypergraph will be saved. Defaults to `"hypergraph.hgsf"`. 
* `options` An optional initializer list of stream manipulators to configure the output (e.g., `{ hgl::io::with_properties }`). 



**Exception:**


* `std::filesystem::filesystem_error` If file safety checks fail (e.g., overwriting an existing file in `write` mode). 
* `std::ios_base::failure` If the underlying file stream cannot be opened. 




        

<hr>

------------------------------


