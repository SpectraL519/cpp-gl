

# Namespace gl::io



[**Namespace List**](namespaces.md) **>** [**gl**](namespacegl.md) **>** [**io**](namespacegl_1_1io.md)




















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**append**](structgl_1_1io_1_1append.md) <br>_Tag type specifying that a file should be opened in append mode._  |
| struct | [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md) &lt;T&gt;<br>_A proxy object for concisely formatting large contiguous numeric ranges._  |
| class | [**options\_manip**](classgl_1_1io_1_1options__manip.md) <br>_A custom stream manipulator for modifying formatting options on standard I/O streams._  |
| struct | [**range\_formatter**](structgl_1_1io_1_1range__formatter.md) &lt;R&gt;<br>_A customizable stream proxy for formatting and printing C++ ranges._  |
| struct | [**write**](structgl_1_1io_1_1write.md) <br>_Tag type specifying that a file should be opened in strictly write (truncate) mode._  |






















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**range\_formatter**](#function-range_formatter) (R && r) <br> |
|   | [**range\_formatter**](#function-range_formatter) (R && r, std::string\_view) <br> |
|   | [**range\_formatter**](#function-range_formatter) (R && r, std::string\_view, std::string\_view) <br> |
|   | [**range\_formatter**](#function-range_formatter) (R && r, std::string\_view, std::string\_view, std::string\_view) <br> |




























## Public Functions Documentation




### function range\_formatter 

```cpp
template<std::ranges::range R>
gl::io::range_formatter (
    R && r
) 
```




<hr>



### function range\_formatter 

```cpp
template<std::ranges::range R>
gl::io::range_formatter (
    R && r,
    std::string_view
) 
```




<hr>



### function range\_formatter 

```cpp
template<std::ranges::range R>
gl::io::range_formatter (
    R && r,
    std::string_view,
    std::string_view
) 
```




<hr>



### function range\_formatter 

```cpp
template<std::ranges::range R>
gl::io::range_formatter (
    R && r,
    std::string_view,
    std::string_view,
    std::string_view
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/io/graph_fio.hpp`

