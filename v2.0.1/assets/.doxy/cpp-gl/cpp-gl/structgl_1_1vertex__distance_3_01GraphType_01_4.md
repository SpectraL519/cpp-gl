

# Struct gl::vertex\_distance&lt; GraphType &gt;

**template &lt;traits::c\_graph GraphType&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**vertex\_distance&lt; GraphType &gt;**](structgl_1_1vertex__distance_3_01GraphType_01_4.md)



_Specialization resolving the specific weight type when edge properties contain weight attributes._ 

* `#include <gl/graph.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) GraphType::edge\_properties\_type::weight\_type | [**type**](#typedef-type)  <br>_Resolves to the_ `weight_type` _inherently mapped to the graph's edges._ |
















































## Public Types Documentation




### typedef type 

_Resolves to the_ `weight_type` _inherently mapped to the graph's edges._
```cpp
using gl::vertex_distance< GraphType >::type =  typename GraphType::edge_properties_type::weight_type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/graph.hpp`

