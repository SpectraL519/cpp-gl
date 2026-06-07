

# Struct hgl::hypergraph\_traits

**template &lt;traits::c\_hypergraph\_directional\_tag DirectionalTag, traits::c\_properties VertexProperties, traits::c\_properties HyperedgeProperties, traits::c\_hypergraph\_repr\_tag ReprTag&gt;**



[**ClassList**](annotated.md) **>** [**hgl**](namespacehgl.md) **>** [**hypergraph\_traits**](structhgl_1_1hypergraph__traits.md)



_Primary hypergraph traits structure that encapsulates all necessary type information for hypergraph representations._ [More...](#detailed-description)

* `#include <hgl/hypergraph_traits.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**DirectionalTag**](group__HGL-Core.md#variable-invalid_id_v) | [**directional\_tag**](#typedef-directional_tag)  <br>_The directional tag indicating the hypergraph's orientation._  |
| typedef [**ReprTag**](group__HGL-Core.md#variable-invalid_id_v) | [**representation\_tag**](#typedef-representation_tag)  <br>_The representation tag defining the internal storage mechanism._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) representation\_tag::layout\_tag | [**layout\_tag**](#typedef-layout_tag)  <br>_The layout tag governing major/minor element ordering._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) representation\_tag::id\_type | [**id\_type**](#typedef-id_type)  <br>_The integer type used for element identifiers._  |
| typedef [**vertex\_descriptor**](group__HGL-Core.md#typedef-vertex_descriptor)&lt; [**VertexProperties**](group__HGL-Core.md#variable-invalid_id_v), [**id\_type**](structhgl_1_1hypergraph__traits.md#typedef-id_type) &gt; | [**vertex\_type**](#typedef-vertex_type)  <br>_The fully resolved type representing a vertex descriptor._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) [**vertex\_type::properties\_type**](classgl_1_1vertex__descriptor.md#typedef-properties_type) | [**vertex\_properties\_type**](#typedef-vertex_properties_type)  <br>_The property payload type associated with vertices._  |
| typedef [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md)&lt; [**HyperedgeProperties**](group__HGL-Core.md#variable-invalid_id_v), [**id\_type**](structhgl_1_1hypergraph__traits.md#typedef-id_type) &gt; | [**hyperedge\_type**](#typedef-hyperedge_type)  <br>_The fully resolved type representing a hyperedge descriptor._  |
| typedef [**typename**](group__HGL-Core.md#variable-invalid_id_v) [**hyperedge\_type::properties\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-properties_type) | [**hyperedge\_properties\_type**](#typedef-hyperedge_properties_type)  <br>_The property payload type associated with hyperedges._  |
















































## Detailed Description


This structure serves as the central point for defining the properties and types associated with a hypergraph, including directionality, element properties, as well as representation and identifier types. It provides a clean, extensible way to customize hypergraph behavior.


#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|DirectionalTag|Specifies whether the hypergraph is undirected or bf\_directed.|[**undirected\_t**](structhgl_1_1undirected__t.md)|[**c\_hypergraph\_directional\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag)|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|ReprTag|Specifies the internal container representation for incidence.|[**repr::list\_t&lt;&gt;**](structhgl_1_1repr_1_1list__t.md)|[**c\_hypergraph\_repr\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag)|







    
## Public Types Documentation




### typedef directional\_tag 

_The directional tag indicating the hypergraph's orientation._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::directional_tag =  DirectionalTag;
```




<hr>



### typedef representation\_tag 

_The representation tag defining the internal storage mechanism._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::representation_tag =  ReprTag;
```




<hr>



### typedef layout\_tag 

_The layout tag governing major/minor element ordering._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::layout_tag =  typename representation_tag::layout_tag;
```




<hr>



### typedef id\_type 

_The integer type used for element identifiers._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::id_type =  typename representation_tag::id_type;
```




<hr>



### typedef vertex\_type 

_The fully resolved type representing a vertex descriptor._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::vertex_type =  vertex_descriptor<VertexProperties, id_type>;
```




<hr>



### typedef vertex\_properties\_type 

_The property payload type associated with vertices._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::vertex_properties_type =  typename vertex_type::properties_type;
```




<hr>



### typedef hyperedge\_type 

_The fully resolved type representing a hyperedge descriptor._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::hyperedge_type =  hyperedge_descriptor<HyperedgeProperties, id_type>;
```




<hr>



### typedef hyperedge\_properties\_type 

_The property payload type associated with hyperedges._ 
```cpp
using hgl::hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag >::hyperedge_properties_type =  typename hyperedge_type::properties_type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/hypergraph_traits.hpp`

