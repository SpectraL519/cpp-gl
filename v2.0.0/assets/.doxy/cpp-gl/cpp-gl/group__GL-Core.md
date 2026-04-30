

# Group GL-Core



[**Modules**](modules.md) **>** [**GL-Core**](group__GL-Core.md)



_Fundamental graph data structures, element descriptors, and configuration tags._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| class | [**gl::binary\_color**](classgl_1_1binary__color.md) <br>_A specialized color property for algorithms requiring binary states (e.g., bipartition)._  |
| struct | [**gl::binary\_color\_property**](structgl_1_1binary__color__property.md) <br>_A property struct wrapping a_ `binary_color` _._ |
| struct | [**gl::directed\_t**](structgl_1_1directed__t.md) <br>_The tag type representing a directed graph configuration._  |
| class | [**gl::dynamic\_properties**](classgl_1_1dynamic__properties.md) <br>_A type-safe container for heterogeneous properties stored by string keys._  |
| class | [**gl::edge\_descriptor**](classgl_1_1edge__descriptor.md) &lt;DirectionalTag, Properties, IdType&gt;<br>_A lightweight wrapper representing a graph edge with its endpoints and optional properties._  |
| struct | [**gl::empty\_properties**](structgl_1_1empty__properties.md) <br>_A stateless, empty structural tag representing an absence of properties._  |
| struct | [**gl::empty\_properties\_map**](structgl_1_1empty__properties__map.md) <br>_A stateless, empty structural tag indicating that the absence of a property map container._  |
| class | [**gl::graph**](classgl_1_1graph.md) &lt;GraphTraits&gt;<br>_The generic graph container using a policy-based architecture._  |
| struct | [**gl::graph\_traits**](structgl_1_1graph__traits.md) &lt;DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType&gt;<br>_Primary graph traits structure that encapsulates all necessary type information for graph representations._  |
| struct | [**gl::initial\_id\_t**](structgl_1_1initial__id__t.md) <br>_A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type._  |
| struct | [**gl::invalid\_id\_t**](structgl_1_1invalid__id__t.md) <br>_A helper type that can be implicitly converted to the invalid ID value for any valid ID type._  |
| struct | [**gl::name\_property**](structgl_1_1name__property.md) <br>_A property struct providing a basic string-based naming facility._  |
| struct | [**gl::repr::flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md) <br>_Tag struct for the flattened adjacency list graph representation._  |
| struct | [**gl::repr::flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md) <br>_Tag struct for the flattened adjacency matrix graph representation._  |
| struct | [**gl::repr::list\_t**](structgl_1_1repr_1_1list__t.md) <br>_Tag struct for the standard adjacency list graph representation._  |
| struct | [**gl::repr::matrix\_t**](structgl_1_1repr_1_1matrix__t.md) <br>_Tag struct for the standard adjacency matrix graph representation._  |
| struct | [**gl::undirected\_t**](structgl_1_1undirected__t.md) <br>_The tag type representing an undirected graph configuration._  |
| class | [**gl::vertex\_descriptor**](classgl_1_1vertex__descriptor.md) &lt;Properties, IdType&gt;<br>_A lightweight wrapper around a vertex identifier with optional properties._  |
| struct | [**gl::vertex\_distance**](structgl_1_1vertex__distance.md) &lt;GraphType&gt;<br>_Utility trait to resolve the underlying distance or weight numeric type for a graph._  |
| struct | [**gl::vertex\_distance&lt; GraphType &gt;**](structgl_1_1vertex__distance_3_01GraphType_01_4.md) &lt;GraphType&gt;<br>_Specialization resolving the specific weight type when edge properties contain weight attributes._  |
| struct | [**gl::weight\_property**](structgl_1_1weight__property.md) &lt;WeightType&gt;<br>_A property struct providing arithmetic weight for edges or vertices._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef typename binary\_color::value | [**bin\_color\_value**](#typedef-bin_color_value)  <br>_Alias for the underlying_ `binary_color::value` _enum._ |
| typedef std::uint32\_t | [**default\_id\_type**](#typedef-default_id_type)  <br>_The default unsigned integer type used for vertex and edge identifiers._  |
| typedef edge\_descriptor&lt; directed\_t, Properties, IdType &gt; | [**directed\_edge**](#typedef-directed_edge)  <br>_Type alias for a directed edge descriptor._  |
| typedef graph&lt; directed\_graph\_traits&lt; VertexProperties, EdgeProperties, ReprTag, IdType &gt; &gt; | [**directed\_graph**](#typedef-directed_graph)  <br>_Convenience alias for defining a directed graph._  |
| typedef graph\_traits&lt; directed\_t, VertexProperties, EdgeProperties, ReprTag, IdType &gt; | [**directed\_graph\_traits**](#typedef-directed_graph_traits)  <br>_Type alias for graph traits with an directed graph configuration._  |
| typedef graph&lt; flat\_list\_graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, IdType &gt; &gt; | [**flat\_list\_graph**](#typedef-flat_list_graph)  <br>_Convenience alias for defining a graph utilizing a flattened adjacency list representation model._  |
| typedef graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, repr::flat\_list\_t, IdType &gt; | [**flat\_list\_graph\_traits**](#typedef-flat_list_graph_traits)  <br>_Type alias for graph traits with a flattened adjacency list representation._  |
| typedef graph&lt; flat\_matrix\_graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, IdType &gt; &gt; | [**flat\_matrix\_graph**](#typedef-flat_matrix_graph)  <br>_Convenience alias for defining a graph utilizing a flattened adjacency matrix representation model._  |
| typedef graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, repr::flat\_matrix\_t, IdType &gt; | [**flat\_matrix\_graph\_traits**](#typedef-flat_matrix_graph_traits)  <br>_Type alias for graph traits with a flattened adjacency matrix representation._  |
| typedef graph&lt; list\_graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, IdType &gt; &gt; | [**list\_graph**](#typedef-list_graph)  <br>_Convenience alias for defining a graph utilizing an adjacency list representation model._  |
| typedef graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, repr::list\_t, IdType &gt; | [**list\_graph\_traits**](#typedef-list_graph_traits)  <br>_Type alias for graph traits with an adjacency list representation._  |
| typedef graph&lt; matrix\_graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, IdType &gt; &gt; | [**matrix\_graph**](#typedef-matrix_graph)  <br>_Convenience alias for defining a graph utilizing an adjacency matrix representation model._  |
| typedef graph\_traits&lt; DirectionalTag, VertexProperties, EdgeProperties, repr::matrix\_t, IdType &gt; | [**matrix\_graph\_traits**](#typedef-matrix_graph_traits)  <br>_Type alias for graph traits with an adjacency matrix representation._  |
| typedef std::size\_t | [**size\_type**](#typedef-size_type)  <br>_Type alias for the standard size type used throughout the library._  |
| typedef edge\_descriptor&lt; undirected\_t, Properties, IdType &gt; | [**undirected\_edge**](#typedef-undirected_edge)  <br>_Type alias for an undirected edge descriptor._  |
| typedef graph&lt; undirected\_graph\_traits&lt; VertexProperties, EdgeProperties, ReprTag, IdType &gt; &gt; | [**undirected\_graph**](#typedef-undirected_graph)  <br>_Convenience alias for defining a nundirected graph._  |
| typedef graph\_traits&lt; undirected\_t, VertexProperties, EdgeProperties, ReprTag, IdType &gt; | [**undirected\_graph\_traits**](#typedef-undirected_graph_traits)  <br>_Type alias for graph traits with an undirected graph configuration._  |
| typedef typename vertex\_distance&lt; GraphType &gt;::type | [**vertex\_distance\_type**](#typedef-vertex_distance_type)  <br>_Convenience alias to retrieve the appropriate distance numeric type from a graph structure._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr initial\_id\_t | [**initial\_id**](#variable-initial_id)  = `{}`<br>_An_ `initial_id_t` _tag constant that can be used to represent the initial ID value of 0 for graph elements in a type-safe manner._ |
|  constexpr IdType | [**initial\_id\_v**](#variable-initial_id_v)  &lt;IdType&gt; = `{0}`<br>_A constant representing the initial ID value of 0 for graph elements._  |
|  constexpr invalid\_id\_t | [**invalid\_id**](#variable-invalid_id)  = `{}`<br>_An_ `invalid_id_t` _tag constant that can be used to represent the invalid ID value for graph elements in a type-safe manner._ |
|  constexpr IdType | [**invalid\_id\_v**](#variable-invalid_id_v)  &lt;IdType&gt; = `{std::numeric_limits<IdType>::max()}`<br>_A constant representing the invalid ID value for graph elements, defined as the maximum value of the specified ID type._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  Graph | [**clone**](#function-clone) (const Graph & source) <br>_Creates a deep copy of the given graph._  |
|  vertex\_distance\_type&lt; GraphType &gt; | [**get\_weight**](#function-get_weight) (const typename GraphType::edge\_type & edge) <br>_Helper utility to safely retrieve the weight payload of an edge._  |
|  auto | [**to**](#function-to) (Graph && source) <br>_Converts a graph from one representation model to another._  |




























## Detailed Description


This group establishes the primary user interface for the library, centered around the highly configurable [**graph**](classgl_1_1graph.md) class. It provides the core data types, such as vertex and edge descriptors, required to safely navigate and manipulate graph elements.


Furthermore, this module defines the declarative tags (e.g., directedness and backend implementation types) that dictate both the behavioral semantics and the underlying memory layout of the instantiated graphs. 


    
## Public Types Documentation




### typedef bin\_color\_value 

_Alias for the underlying_ `binary_color::value` _enum._
```cpp
using gl::bin_color_value = typedef typename binary_color::value;
```




<hr>



### typedef default\_id\_type 

_The default unsigned integer type used for vertex and edge identifiers._ 
```cpp
using gl::default_id_type = typedef std::uint32_t;
```




<hr>



### typedef directed\_edge 

_Type alias for a directed edge descriptor._ 
```cpp
template<traits::c_properties Properties, traits::c_id_type IdType>
using gl::directed_edge = typedef edge_descriptor<directed_t, Properties, IdType>;
```



Pre-binds the `DirectionalTag` of `edge_descriptor` to `directed_t`.


#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|Properties|The type of property data attached to the edge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The underlying integer type used for the IDs.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







        

<hr>



### typedef directed\_graph 

_Convenience alias for defining a directed graph._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_graph_repr_tag ReprTag, traits::c_id_type IdType>
using gl::directed_graph = typedef graph<directed_graph_traits<VertexProperties, EdgeProperties, ReprTag, IdType> >;
```




<hr>



### typedef directed\_graph\_traits 

_Type alias for graph traits with an directed graph configuration._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_graph_repr_tag ReprTag, traits::c_id_type IdType>
using gl::directed_graph_traits = typedef graph_traits<directed_t, VertexProperties, EdgeProperties, ReprTag, IdType>;
```



This alias simplifies the specification of graph traits for directed graphs, allowing users to easily define their graph types with the desired properties and representation while defaulting to a directed configuration.



> [!NOTE] Template parameters The template parameters for this alias are the same as those for [**graph\_traits**](structgl_1_1graph__traits.md), with the `DirectionalTag` parameter fixed to [**directed\_t**](structgl_1_1directed__t.md). This means that when using `directed_graph_traits`, only the `VertexProperties`, `EdgeProperties`, 'ReprTag', and `IdType` parameters need to be specified. 
>
>



        

<hr>



### typedef flat\_list\_graph 

_Convenience alias for defining a graph utilizing a flattened adjacency list representation model._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::flat_list_graph = typedef graph<flat_list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType> >;
```




<hr>



### typedef flat\_list\_graph\_traits 

_Type alias for graph traits with a flattened adjacency list representation._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::flat_list_graph_traits = typedef graph_traits<DirectionalTag, VertexProperties, EdgeProperties, repr::flat_list_t, IdType>;
```



This alias simplifies the specification of graph traits for graphs that use a flattened adjacency list representation, allowing users to easily define their graph types with the desired properties and directionality while defaulting to the flattened adjacency list representation.



> [!NOTE] Template parameters
>
>
> The template parameters for this alias are the same as those for [**graph\_traits**](structgl_1_1graph__traits.md), with the `ReprTag` parameter fixed to [**repr::flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md). This means that when using `flat_list_graph_traits`, only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified. 
>
>



        

<hr>



### typedef flat\_matrix\_graph 

_Convenience alias for defining a graph utilizing a flattened adjacency matrix representation model._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::flat_matrix_graph = typedef graph<flat_matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType> >;
```




<hr>



### typedef flat\_matrix\_graph\_traits 

_Type alias for graph traits with a flattened adjacency matrix representation._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::flat_matrix_graph_traits = typedef graph_traits<DirectionalTag, VertexProperties, EdgeProperties, repr::flat_matrix_t, IdType>;
```



This alias simplifies the specification of graph traits for graphs that use a flattened adjacency matrix representation, allowing users to easily define their graph types with the desired properties and directionality while defaulting to the flattened adjacency matrix representation.



> [!NOTE] Template parameters
>
>
> The template parameters for this alias are the same as those for [**graph\_traits**](structgl_1_1graph__traits.md), with the `ReprTag` parameter fixed to [**repr::flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md). This means that when using `flat_matrix_graph_traits`, only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified. 
>
>



        

<hr>



### typedef list\_graph 

_Convenience alias for defining a graph utilizing an adjacency list representation model._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::list_graph = typedef graph<list_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType> >;
```




<hr>



### typedef list\_graph\_traits 

_Type alias for graph traits with an adjacency list representation._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::list_graph_traits = typedef graph_traits<DirectionalTag, VertexProperties, EdgeProperties, repr::list_t, IdType>;
```



This alias simplifies the specification of graph traits for graphs that use an adjacency list representation, allowing users to easily define their graph types with the desired properties and directionality while defaulting to the adjacency list representation.



> [!NOTE] Template parameters
>
>
> The template parameters for this alias are the same as those for [**graph\_traits**](structgl_1_1graph__traits.md), with the `ReprTag` parameter fixed to [**repr::list\_t**](structgl_1_1repr_1_1list__t.md). This means that when using `list_graph_traits`, only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified. 
>
>



        

<hr>



### typedef matrix\_graph 

_Convenience alias for defining a graph utilizing an adjacency matrix representation model._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::matrix_graph = typedef graph<matrix_graph_traits<DirectionalTag, VertexProperties, EdgeProperties, IdType> >;
```




<hr>



### typedef matrix\_graph\_traits 

_Type alias for graph traits with an adjacency matrix representation._ 
```cpp
template<traits::c_graph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_id_type IdType>
using gl::matrix_graph_traits = typedef graph_traits<DirectionalTag, VertexProperties, EdgeProperties, repr::matrix_t, IdType>;
```



This alias simplifies the specification of graph traits for graphs that use an adjacency matrix representation, allowing users to easily define their graph types with the desired properties and directionality while defaulting to the adjacency matrix representation.



> [!NOTE] Template parameters
>
>
> The template parameters for this alias are the same as those for [**graph\_traits**](structgl_1_1graph__traits.md), with the `ReprTag` parameter fixed to [**repr::matrix\_t**](structgl_1_1repr_1_1matrix__t.md). This means that when using `matrix_graph_traits`, only the `DirectionalTag`, `VertexProperties`, `EdgeProperties`, and `IdType` parameters need to be specified. 
>
>



        

<hr>



### typedef size\_type 

_Type alias for the standard size type used throughout the library._ 
```cpp
using gl::size_type = typedef std::size_t;
```



Used primarily for indices, counts, and sizes of graph components. 


        

<hr>



### typedef undirected\_edge 

_Type alias for an undirected edge descriptor._ 
```cpp
template<traits::c_properties Properties, traits::c_id_type IdType>
using gl::undirected_edge = typedef edge_descriptor<undirected_t, Properties, IdType>;
```



Pre-binds the `DirectionalTag` of `edge_descriptor` to `undirected_t`.


#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|Properties|The type of property data attached to the edge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The underlying integer type used for the IDs.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







        

<hr>



### typedef undirected\_graph 

_Convenience alias for defining a nundirected graph._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_graph_repr_tag ReprTag, traits::c_id_type IdType>
using gl::undirected_graph = typedef graph<undirected_graph_traits<VertexProperties, EdgeProperties, ReprTag, IdType> >;
```




<hr>



### typedef undirected\_graph\_traits 

_Type alias for graph traits with an undirected graph configuration._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties EdgeProperties, traits::c_graph_repr_tag ReprTag, traits::c_id_type IdType>
using gl::undirected_graph_traits = typedef graph_traits<undirected_t, VertexProperties, EdgeProperties, ReprTag, IdType>;
```



This alias simplifies the specification of graph traits for undirected graphs, allowing users to easily define their graph types with the desired properties and representation while defaulting to an undirected configuration.



> [!NOTE] Template parameters The template parameters for this alias are the same as those for [**graph\_traits**](structgl_1_1graph__traits.md), with the `DirectionalTag` parameter fixed to [**undirected\_t**](structgl_1_1undirected__t.md). This means that when using `undirected_graph_traits`, only the `VertexProperties`, `EdgeProperties`, 'ReprTag', and `IdType` parameters need to be specified. 
>
>



        

<hr>



### typedef vertex\_distance\_type 

_Convenience alias to retrieve the appropriate distance numeric type from a graph structure._ 
```cpp
template<traits::c_graph GraphType>
using gl::vertex_distance_type = typedef typename vertex_distance<GraphType>::type;
```




<hr>
## Public Attributes Documentation




### variable initial\_id 

_An_ `initial_id_t` _tag constant that can be used to represent the initial ID value of 0 for graph elements in a type-safe manner._
```cpp
constexpr initial_id_t gl::initial_id = {};
```



#### Example Usage




```cpp
using vertex_id_t = std::uint32_t;
vertex_id_t v1 = gl::initial_id; // (1)!
```



1. The `initial_id` constant can be implicitly converted to the `v1`'s type (`vertex_id_t`), resulting in `v1` being initialized to the value of 0.



#### See Also




* [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)
* [**initial\_id\_v**](group__GL-Core.md#variable-initial_id_v)
* [**initial\_id\_t**](structgl_1_1initial__id__t.md) 





        

<hr>



### variable initial\_id\_v 

_A constant representing the initial ID value of 0 for graph elements._ 
```cpp
template<traits::c_id_type IdType>
constexpr IdType gl::initial_id_v = {0};
```





**Template parameters:**


* `IdType` The type of the ID, which must satisfy the [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) concept. 




#### See Also




* [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)
* [**initial\_id\_t**](structgl_1_1initial__id__t.md)
* [**initial\_id**](group__GL-Core.md#variable-initial_id) 





        

<hr>



### variable invalid\_id 

_An_ `invalid_id_t` _tag constant that can be used to represent the invalid ID value for graph elements in a type-safe manner._
```cpp
constexpr invalid_id_t gl::invalid_id = {};
```



#### Example Usage




```cpp
using vertex_id_t = std::uint32_t;
vertex_id_t v1 = gl::invalid_id; // (1)!
```



1. The `invalid_id` constant can be implicitly converted to the `v1`'s type (`vertex_id_t`), resulting in `v1` being initialized to the maximum value of `vertex_id_t`, which represents an invalid ID.



#### See Also




* [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)
* [**invalid\_id\_v**](group__GL-Core.md#variable-invalid_id_v)
* [**invalid\_id\_t**](structgl_1_1invalid__id__t.md) 





        

<hr>



### variable invalid\_id\_v 

_A constant representing the invalid ID value for graph elements, defined as the maximum value of the specified ID type._ 
```cpp
template<traits::c_id_type IdType>
constexpr IdType gl::invalid_id_v = {std::numeric_limits<IdType>::max()};
```





**Template parameters:**


* `IdType` The type of the ID, which must satisfy the [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type) concept. 




#### See Also




* [**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)
* [**invalid\_id\_t**](structgl_1_1invalid__id__t.md)
* [**invalid\_id**](group__GL-Core.md#variable-invalid_id) 





        

<hr>
## Public Functions Documentation




### function clone 

_Creates a deep copy of the given graph._ 
```cpp
template<traits::c_graph Graph>
Graph clone (
    const Graph & source
) 
```



Friend declaration providing access for deep cloning.




**Template parameters:**


* `Graph` The type of the graph. 



**Parameters:**


* `source` The graph instance to clone. 



**Returns:**

A newly constructed graph containing identical vertices, edges and properties (if applicable). 





        

<hr>



### function get\_weight 

_Helper utility to safely retrieve the weight payload of an edge._ 
```cpp
template<traits::c_graph GraphType>
vertex_distance_type< GraphType > get_weight (
    const typename GraphType::edge_type & edge
) 
```



Automatically returns a constant default if the graph representation carries no attached weight properties.




**Template parameters:**


* `GraphType` The underlying type of the graph. 



**Parameters:**


* `edge` The edge descriptor to extract weight from. 



**Returns:**

The specified edge weight or the unweighted graph default (1). 





        

<hr>



### function to 

_Converts a graph from one representation model to another._ 
```cpp
template<traits::c_graph_repr_tag TargetImplTag, traits::c_graph Graph>
auto to (
    Graph && source
) 
```



Friend declaration providing access for graph target conversions.


This function efficiently transforms a graph's underlying memory representation (e.g., from a standard adjacency list to a flattened adjacency list) while preserving its exact topology, properties, and identifiers.


#### Template Parameters




|Parameter|Description|Constraints|
|-----|-----|-----|
|TargetImplTag|The representation tag of the desired target representation (e.g., `gl::repr::flat_list_t`)|[**c\_graph\_repr\_tag**](gl_concepts.md#gl-traits-c-graph-repr-tag)|
|Graph|The type of the source graph, which will be automatically deduced from the function argument.|[**c\_graph**](gl_concepts.md#gl-traits-c-graph)|








**Parameters:**


* `source` The graph to convert. After the operation it will be left in a valid, empty state. 



**Returns:**

A new graph containing the moved data, structured according to `TargetImplTag`.






#### See Also




* [**swap\_repr\_tag**](structgl_1_1traits_1_1swap__repr__tag.md) : For the trait used to resolve the target graph type with the swapped representation tag. 





        

<hr>

------------------------------


