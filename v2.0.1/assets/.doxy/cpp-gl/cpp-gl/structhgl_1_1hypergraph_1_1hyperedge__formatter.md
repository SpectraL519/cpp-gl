

# Struct hgl::hypergraph::hyperedge\_formatter



[**ClassList**](annotated.md) **>** [**hgl**](namespacehgl.md) **>** [**hypergraph**](classhgl_1_1hypergraph.md) **>** [**hyperedge\_formatter**](structhgl_1_1hypergraph_1_1hyperedge__formatter.md)



_Helper structure used to properly format an individual hyperedge within the hypergraph's context into an output stream._ 

* `#include <hgl/hypergraph.hpp>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**const**](group__HGL-Core.md#variable-invalid_id_v) [**hypergraph**](classhgl_1_1hypergraph.md) & | [**hg**](#variable-hg)  <br>_The hypergraph owning the hyperedge._  |
|  [**const**](group__HGL-Core.md#variable-invalid_id_v) [**hyperedge\_type**](classhgl_1_1hypergraph.md#typedef-hyperedge_type) | [**hyperedge**](#variable-hyperedge)  <br>_The hyperedge to be formatted._  |












































## Public Attributes Documentation




### variable hg 

_The hypergraph owning the hyperedge._ 
```cpp
const hypergraph& hgl::hypergraph< HypergraphTraits >::hyperedge_formatter::hg;
```




<hr>



### variable hyperedge 

_The hyperedge to be formatted._ 
```cpp
const hyperedge_type hgl::hypergraph< HypergraphTraits >::hyperedge_formatter::hyperedge;
```




<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Stream insertion operator for undirected hyperedges._ 
```cpp
inline std::ostream & hgl::hypergraph::hyperedge_formatter::operator<< (
    std::ostream & os,
    const  hyperedge_formatter & proxy
) 
```




<hr>



### friend operator&lt;&lt; 

_Stream insertion operator for BF-directed hyperedges._ 
```cpp
inline std::ostream & hgl::hypergraph::hyperedge_formatter::operator<< (
    std::ostream & os,
    const  hyperedge_formatter & proxy
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/hypergraph.hpp`

