

# Struct gl::graph\_traits

**template &lt;traits::c\_graph\_directional\_tag DirectionalTag, traits::c\_properties VertexProperties, traits::c\_properties EdgeProperties, traits::c\_graph\_repr\_tag ReprTag, traits::c\_id\_type IdType&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**graph\_traits**](structgl_1_1graph__traits.md)



_Primary graph traits structure that encapsulates all necessary type information for graph representations._ [More...](#detailed-description)

* `#include <gl/graph_traits.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**DirectionalTag**](group__GL-Core.md#variable-invalid_id_v) | [**directional\_tag**](#typedef-directional_tag)  <br>_The tag indicating the graph's directionality (directed or undirected)._  |
| typedef [**ReprTag**](group__GL-Core.md#variable-invalid_id_v) | [**representation\_tag**](#typedef-representation_tag)  <br>_The tag indicating the graph's representation type (e.g., adjacency list, adjacency matrix)._  |
| typedef [**IdType**](group__GL-Core.md#variable-invalid_id_v) | [**id\_type**](#typedef-id_type)  <br>_The type of graph element indentifiers (i.e. vertex and edge IDs)._  |
| typedef [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md)&lt; [**VertexProperties**](group__GL-Core.md#variable-invalid_id_v), [**id\_type**](structgl_1_1graph__traits.md#typedef-id_type) &gt; | [**vertex\_type**](#typedef-vertex_type)  <br>_The vertex descriptor type associated with this graph, defined based on the specified vertex properties and identifier type._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) [**vertex\_type::properties\_type**](classgl_1_1vertex__descriptor.md#typedef-properties_type) | [**vertex\_properties\_type**](#typedef-vertex_properties_type)  <br>_The type of properties associated with the vertex descriptor._  |
| typedef [**edge\_descriptor**](classgl_1_1edge__descriptor.md)&lt; [**DirectionalTag**](group__GL-Core.md#variable-invalid_id_v), [**EdgeProperties**](group__GL-Core.md#variable-invalid_id_v), [**id\_type**](structgl_1_1graph__traits.md#typedef-id_type) &gt; | [**edge\_type**](#typedef-edge_type)  <br>_The edge descriptor type associated with this graph, defined based on the specified edge properties and identifier type._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) [**edge\_type::properties\_type**](classgl_1_1edge__descriptor.md#typedef-properties_type) | [**edge\_properties\_type**](#typedef-edge_properties_type)  <br>_The type of properties associated with the edge descriptor._  |
















































## Detailed Description


This structure serves as the central point for defining the properties and types associated with a graph, including directionality, vertex and edge properties, as well as representation and identifier types. It is designed to be flexible and extensible, allowing users to customize their graph types by specifying different traits.


#### Template Parameters




|Parameter|Description|Default value|Constraints|
|-----|-----|-----|-----|
|DirectionalTag|Specifies whether the graph is directed or undirected.|[**directed\_t**](structgl_1_1directed__t.md)|[**c\_graph\_directional\_tag**](gl_concepts.md#gl-traits-c-graph-directional-tag)|
|VertexProperties|A type representing the properties associated with vertices in the graph.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|EdgeProperties|A type representing the properties associated with edges in the graph.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|ReprTag|Specifies the graph representation type (e.g., adjacency list, adjacency matrix).|[**repr::list\_t**](structgl_1_1repr_1_1list__t.md)|[**c\_graph\_repr\_tag**](gl_concepts.md#gl-traits-c-graph-repr-tag)|
|IdType|The type used for vertex and edge identifiers.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







#### See Also




* Available representation tags:
    * [**repr::list\_t**](structgl_1_1repr_1_1list__t.md) : A standard adjacency list representation.
    * [**repr::flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md) : A flattened adjacency list representation.
    * [**repr::matrix\_t**](structgl_1_1repr_1_1matrix__t.md) : A standard adjacency matrix representation.
    * [**repr::flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md) : A flattened adjacency matrix representation.


* [**directed\_t**](structgl_1_1directed__t.md), [**undirected\_t**](structgl_1_1undirected__t.md) : For the directional tags used to specify graph directionality.
* [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) : For the vertex descriptor type defined based on the graph traits.
* [**edge\_descriptor**](classgl_1_1edge__descriptor.md) : For the edge descriptor type defined based on the graph traits.
* [**empty\_properties**](structgl_1_1empty__properties.md) : For the default empty properties type used when no custom properties are needed.
* Convenience type aliases for the `graph_traits` type:
    * [**list\_graph\_traits**](group__GL-Core.md#typedef-list_graph_traits),
    * [**flat\_list\_graph\_traits**](group__GL-Core.md#typedef-flat_list_graph_traits),
    * [**matrix\_graph\_traits**](group__GL-Core.md#typedef-matrix_graph_traits),
    * [**flat\_matrix\_graph\_traits**](group__GL-Core.md#typedef-flat_matrix_graph_traits),
    * [**directed\_graph\_traits**](group__GL-Core.md#typedef-directed_graph_traits),
    * [**undirected\_graph\_traits**](group__GL-Core.md#typedef-undirected_graph_traits) 







    
## Public Types Documentation




### typedef directional\_tag 

_The tag indicating the graph's directionality (directed or undirected)._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::directional_tag =  DirectionalTag;
```




<hr>



### typedef representation\_tag 

_The tag indicating the graph's representation type (e.g., adjacency list, adjacency matrix)._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::representation_tag =  ReprTag;
```




<hr>



### typedef id\_type 

_The type of graph element indentifiers (i.e. vertex and edge IDs)._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::id_type =  IdType;
```




<hr>



### typedef vertex\_type 

_The vertex descriptor type associated with this graph, defined based on the specified vertex properties and identifier type._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::vertex_type =  vertex_descriptor<VertexProperties, id_type>;
```




<hr>



### typedef vertex\_properties\_type 

_The type of properties associated with the vertex descriptor._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::vertex_properties_type =  typename vertex_type::properties_type;
```




<hr>



### typedef edge\_type 

_The edge descriptor type associated with this graph, defined based on the specified edge properties and identifier type._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::edge_type =  edge_descriptor<DirectionalTag, EdgeProperties, id_type>;
```




<hr>



### typedef edge\_properties\_type 

_The type of properties associated with the edge descriptor._ 
```cpp
using gl::graph_traits< DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType >::edge_properties_type =  typename edge_type::properties_type;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/graph_traits.hpp`

