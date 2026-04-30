

# Struct gl::vertex\_distance

**template &lt;traits::c\_graph GraphType&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**vertex\_distance**](structgl_1_1vertex__distance.md)



_Utility trait to resolve the underlying distance or weight numeric type for a graph._ 

* `#include <gl/graph.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**default\_vertex\_distance\_type**](namespacegl.md#typedef-default_vertex_distance_type) | [**type**](#typedef-type)  <br>_Resolves to the default distance type if no specific weight property is detected._  |
















































## Public Types Documentation




### typedef type 

_Resolves to the default distance type if no specific weight property is detected._ 
```cpp
using gl::vertex_distance< GraphType >::type =  default_vertex_distance_type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/graph.hpp`

