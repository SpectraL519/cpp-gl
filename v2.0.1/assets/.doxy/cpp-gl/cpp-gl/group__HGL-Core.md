

# Group HGL-Core



[**Modules**](modules.md) **>** [**HGL-Core**](group__HGL-Core.md)



_Fundamental hypergraph data structures, element descriptors, and configuration tags._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**hgl::bf\_directed\_t**](structhgl_1_1bf__directed__t.md) <br>_Tag type specifying that a hypergraph is backward-forward (BF) directed._  |
| class | [**hgl::hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) &lt;Properties, IdType&gt;<br>_A lightweight wrapper representing a hypergraph edge with optional properties._  |
| struct | [**hgl::hyperedge\_t**](structhgl_1_1hyperedge__t.md) <br>_Tag type representing a hyperedge element in a hypergraph._  |
| class | [**hgl::hypergraph**](classhgl_1_1hypergraph.md) &lt;HypergraphTraits&gt;<br>_The generic hypergraph container using a policy-based design._  |
| struct | [**hgl::hypergraph\_traits**](structhgl_1_1hypergraph__traits.md) &lt;DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag&gt;<br>_Primary hypergraph traits structure that encapsulates all necessary type information for hypergraph representations._  |
| struct | [**hgl::repr::bidirectional\_t**](structhgl_1_1repr_1_1bidirectional__t.md) <br>_Layout tag representing a symmetric or fully bidirectional incidence structural layout._  |
| struct | [**hgl::repr::flat\_list\_t**](structhgl_1_1repr_1_1flat__list__t.md) &lt;LayoutTag, IdType&gt;<br>_Tag struct for the flattened incidence list hypergraph representation._  |
| struct | [**hgl::repr::flat\_matrix\_t**](structhgl_1_1repr_1_1flat__matrix__t.md) &lt;LayoutTag, IdType&gt;<br>_Tag struct for the flattened incidence matrix hypergraph representation._  |
| struct | [**hgl::repr::hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md) <br>_Layout tag designating hyperedges as the primary structural dimension of the incidence representation._  |
| struct | [**hgl::repr::list\_t**](structhgl_1_1repr_1_1list__t.md) &lt;LayoutTag, IdType&gt;<br>_Tag struct for the standard incidence list hypergraph representation._  |
| struct | [**hgl::repr::matrix\_t**](structhgl_1_1repr_1_1matrix__t.md) &lt;LayoutTag, IdType&gt;<br>_Tag struct for the standard incidence matrix hypergraph representation._  |
| struct | [**hgl::repr::vertex\_major\_t**](structhgl_1_1repr_1_1vertex__major__t.md) <br>_Layout tag designating vertices as the primary structural dimension of the incidence representation._  |
| struct | [**hgl::undirected\_t**](structhgl_1_1undirected__t.md) <br>_Tag type specifying that a hypergraph is undirected._  |
| struct | [**hgl::vertex\_t**](structhgl_1_1vertex__t.md) <br>_Tag type representing a vertex element in a hypergraph._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef hypergraph&lt; bf\_directed\_hypergraph\_traits&lt; VertexProperties, HyperedgeProperties, ReprTag &gt; &gt; | [**bf\_directed\_hypergraph**](#typedef-bf_directed_hypergraph)  <br>_Convenience alias for a BF-directed hypergraph._  |
| typedef hypergraph\_traits&lt; bf\_directed\_t, VertexProperties, HyperedgeProperties, ReprTag &gt; | [**bf\_directed\_hypergraph\_traits**](#typedef-bf_directed_hypergraph_traits)  <br>_Type alias for bf-directed hypergraph traits with configurable properties and representation._  |
| typedef [**gl::dynamic\_properties**](classgl_1_1dynamic__properties.md) | [**dynamic\_properties**](#typedef-dynamic_properties)  <br>_A type-safe container for heterogeneous properties stored by string keys._  |
| typedef [**gl::empty\_properties**](structgl_1_1empty__properties.md) | [**empty\_properties**](#typedef-empty_properties)  <br>_A stateless, empty structural tag representing an absence of properties._  |
| typedef [**gl::empty\_properties\_map**](structgl_1_1empty__properties__map.md) | [**empty\_properties\_map**](#typedef-empty_properties_map)  <br>_A stateless, empty structural tag indicating that the absence of a property map container._  |
| typedef hypergraph&lt; flat\_list\_hypergraph\_traits&lt; LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType &gt; &gt; | [**flat\_list\_hypergraph**](#typedef-flat_list_hypergraph)  <br>_Convenience alias for a hypergraph utilizing a flat incidence list representation model._  |
| typedef hypergraph\_traits&lt; DirectionalTag, VertexProperties, HyperedgeProperties, repr::flat\_list\_t&lt; LayoutTag, IdType &gt; &gt; | [**flat\_list\_hypergraph\_traits**](#typedef-flat_list_hypergraph_traits)  <br>_Convenience alias for_ `hypergraph_traits` _using a flattened incidence list representation._ |
| typedef hypergraph&lt; flat\_matrix\_hypergraph\_traits&lt; LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType &gt; &gt; | [**flat\_matrix\_hypergraph**](#typedef-flat_matrix_hypergraph)  <br>_Convenience alias for a hypergraph utilizing a flat incidence matrix representation model._  |
| typedef hypergraph\_traits&lt; DirectionalTag, VertexProperties, HyperedgeProperties, repr::flat\_matrix\_t&lt; LayoutTag, IdType &gt; &gt; | [**flat\_matrix\_hypergraph\_traits**](#typedef-flat_matrix_hypergraph_traits)  <br>_Convenience alias for_ `hypergraph_traits` _using a flattened incidence matrix representation._ |
| typedef [**gl::initial\_id\_t**](structgl_1_1initial__id__t.md) | [**initial\_id\_t**](#typedef-initial_id_t)  <br>_A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type._  |
| typedef [**gl::invalid\_id\_t**](structgl_1_1invalid__id__t.md) | [**invalid\_id\_t**](#typedef-invalid_id_t)  <br>_A helper type that can be implicitly converted to the invalid ID value for any valid ID type._  |
| typedef hypergraph&lt; list\_hypergraph\_traits&lt; LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType &gt; &gt; | [**list\_hypergraph**](#typedef-list_hypergraph)  <br>_Convenience alias for a hypergraph utilizing a standard incidence list representation model._  |
| typedef hypergraph\_traits&lt; DirectionalTag, VertexProperties, HyperedgeProperties, repr::list\_t&lt; LayoutTag, IdType &gt; &gt; | [**list\_hypergraph\_traits**](#typedef-list_hypergraph_traits)  <br>_Convenience alias for_ `hypergraph_traits` _using a standard incidence list representation._ |
| typedef hypergraph&lt; matrix\_hypergraph\_traits&lt; LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType &gt; &gt; | [**matrix\_hypergraph**](#typedef-matrix_hypergraph)  <br>_Convenience alias for a hypergraph utilizing a standard incidence matrix representation model._  |
| typedef hypergraph\_traits&lt; DirectionalTag, VertexProperties, HyperedgeProperties, repr::matrix\_t&lt; LayoutTag, IdType &gt; &gt; | [**matrix\_hypergraph\_traits**](#typedef-matrix_hypergraph_traits)  <br>_Convenience alias for_ `hypergraph_traits` _using a standard incidence matrix representation._ |
| typedef [**gl::name\_property**](structgl_1_1name__property.md) | [**name\_property**](#typedef-name_property)  <br>_A property struct providing a basic string-based naming facility._  |
| typedef hypergraph&lt; undirected\_hypergraph\_traits&lt; VertexProperties, HyperedgeProperties, ReprTag &gt; &gt; | [**undirected\_hypergraph**](#typedef-undirected_hypergraph)  <br>_Convenience alias for an undirected hypergraph._  |
| typedef hypergraph\_traits&lt; undirected\_t, VertexProperties, HyperedgeProperties, ReprTag &gt; | [**undirected\_hypergraph\_traits**](#typedef-undirected_hypergraph_traits)  <br>_Type alias for undirected hypergraph traits with configurable properties and representation._  |
| typedef [**gl::vertex\_descriptor**](classgl_1_1vertex__descriptor.md)&lt; Properties, IdType &gt; | [**vertex\_descriptor**](#typedef-vertex_descriptor)  <br>_Type alias adapting the standard graph vertex descriptor for hypergraphs._  |
| typedef [**gl::weight\_property**](structgl_1_1weight__property.md)&lt; WeightType &gt; | [**weight\_property**](#typedef-weight_property)  <br>_A property struct providing arithmetic weight for hyperedges or vertices._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr hyperedge\_t | [**hyperedge**](#variable-hyperedge)  = `{}`<br>_A constant instance of_ `hyperedge_t` _used for tagging and generic dispatching._ |
|  constexpr initial\_id\_t | [**initial\_id**](#variable-initial_id)  = `gl::initial_id`<br>_An_ `initial_id_t` _tag constant that can be used to represent the initial ID value of 0 for hypergraph elements in a type-safe manner._ |
|  constexpr IdType | [**initial\_id\_v**](#variable-initial_id_v)  &lt;IdType&gt; = `gl::initial_id_v<IdType>`<br>_A constant representing the initial ID value of 0 for hypergraph elements._  |
|  constexpr invalid\_id\_t | [**invalid\_id**](#variable-invalid_id)  = `gl::invalid_id`<br>_An_ `invalid_id_t` _tag constant that can be used to represent the invalid ID value for hypergraph elements in a type-safe manner._ |
|  constexpr IdType | [**invalid\_id\_v**](#variable-invalid_id_v)  &lt;IdType&gt; = `gl::invalid_id_v<IdType>`<br>_A constant representing the invalid ID value for hypergraph elements, defined as the maximum value of the specified ID type._  |
|  constexpr vertex\_t | [**vertex**](#variable-vertex)  = `{}`<br>_A constant instance of_ `vertex_t` _used for tagging and generic dispatching._ |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  Hypergraph | [**clone**](#function-clone) (const Hypergraph & source) <br>_Creates a deep copy of a given hypergraph._  |
|  G | [**incidence\_graph**](#function-incidence_graph) (const traits::c\_undirected\_hypergraph auto & h) <br>_Computes the bipartite incidence graph representation of an_ _undirected_ _hypergraph._ |
|  G | [**incidence\_graph**](#function-incidence_graph) (const traits::c\_bf\_directed\_hypergraph auto & h) <br>_Computes the bipartite incidence graph representation of a_ _BF-directed_ _hypergraph._ |
|  G | [**projection**](#function-projection) (const traits::c\_undirected\_hypergraph auto & h) <br>_Computes the projection (clique expansion) of an_ _undirected_ _hypergraph._ |
|  G | [**projection**](#function-projection) (const traits::c\_bf\_directed\_hypergraph auto & h) <br>_Computes the projection of a_ _BF-directed_ _hypergraph._ |
|  auto | [**to**](#function-to) (Hypergraph && source) <br>_Converts a hypergraph from one representation model to another._  |




























## Detailed Description


This group establishes the primary user interface for the HGL module, centered around the highly configurable [**hypergraph**](classhgl_1_1hypergraph.md) class. It provides the core data types, such as vertex and hyperedge descriptors, required to safely navigate and manipulate graph elements.


Furthermore, this module defines the declarative tags (e.g., directedness and backend implementation types) that dictate both the behavioral semantics and the underlying memory layout of the instantiated hypergraphs. 


    
## Public Types Documentation




### typedef bf\_directed\_hypergraph 

_Convenience alias for a BF-directed hypergraph._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_hypergraph_repr_tag ReprTag>
using hgl::bf_directed_hypergraph = typedef hypergraph<bf_directed_hypergraph_traits<VertexProperties, HyperedgeProperties, ReprTag> >;
```




<hr>



### typedef bf\_directed\_hypergraph\_traits 

_Type alias for bf-directed hypergraph traits with configurable properties and representation._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_hypergraph_repr_tag ReprTag>
using hgl::bf_directed_hypergraph_traits = typedef hypergraph_traits<bf_directed_t, VertexProperties, HyperedgeProperties, ReprTag>;
```



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|ReprTag|Specifies the internal container representation for incidence.|[**repr::list\_t&lt;&gt;**](structhgl_1_1repr_1_1list__t.md)|[**c\_hypergraph\_repr\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag)|







        

<hr>



### typedef dynamic\_properties 

_A type-safe container for heterogeneous properties stored by string keys._ 
```cpp
using hgl::dynamic_properties = typedef gl::dynamic_properties;
```



#### See Also




* [**gl::dynamic\_properties**](classgl_1_1dynamic__properties.md) : For the full type definition. 





        

<hr>



### typedef empty\_properties 

_A stateless, empty structural tag representing an absence of properties._ 
```cpp
using hgl::empty_properties = typedef gl::empty_properties;
```




> [!IMPORTANT]
>
>
> This type is used as a default `properties_type` for hypergraph components that do not require any user-defined data. It serves as a marker to indicate that the component is _property-less_ and can be optimized accordingly. 
>
>



#### See Also




* [**gl::empty\_properties**](structgl_1_1empty__properties.md) : For the full type definition. 





        

<hr>



### typedef empty\_properties\_map 

_A stateless, empty structural tag indicating that the absence of a property map container._ 
```cpp
using hgl::empty_properties_map = typedef gl::empty_properties_map;
```




> [!NOTE]
>
>
> This type is used internally by the library to optimize storage for hypergraph components that have no properties. 
>
>



#### See Also




* [**gl::empty\_properties\_map**](structgl_1_1empty__properties__map.md) : For the full type definition. 





        

<hr>



### typedef flat\_list\_hypergraph 

_Convenience alias for a hypergraph utilizing a flat incidence list representation model._ 
```cpp
template<traits::c_hypergraph_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::flat_list_hypergraph = typedef hypergraph<flat_list_hypergraph_traits< LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType> >;
```




<hr>



### typedef flat\_list\_hypergraph\_traits 

_Convenience alias for_ `hypergraph_traits` _using a flattened incidence list representation._
```cpp
template<traits::c_hypergraph_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::flat_list_hypergraph_traits = typedef hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, repr::flat_list_t<LayoutTag, IdType> >;
```



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|LayoutTag|Memory orientation for the flat list structures.|[**bidirectional\_t**](structhgl_1_1repr_1_1bidirectional__t.md)|[**c\_hypergraph\_layout\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-layout-tag)|
|DirectionalTag|Specifies whether the hypergraph is undirected or bf\_directed.|[**undirected\_t**](structhgl_1_1undirected__t.md)|[**c\_hypergraph\_directional\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag)|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The integer type used for element identifiers.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







        

<hr>



### typedef flat\_matrix\_hypergraph 

_Convenience alias for a hypergraph utilizing a flat incidence matrix representation model._ 
```cpp
template<traits::c_hypergraph_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::flat_matrix_hypergraph = typedef hypergraph<flat_matrix_hypergraph_traits< LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType> >;
```




<hr>



### typedef flat\_matrix\_hypergraph\_traits 

_Convenience alias for_ `hypergraph_traits` _using a flattened incidence matrix representation._
```cpp
template<traits::c_hypergraph_asymmetric_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::flat_matrix_hypergraph_traits = typedef hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, repr::flat_matrix_t<LayoutTag, IdType> >;
```



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|LayoutTag|Memory orientation (must be asymmetric).|[**hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md)|[**c\_hypergraph\_asymmetric\_layout\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-asymmetric-layout-tag)|
|DirectionalTag|Specifies whether the hypergraph is undirected or bf\_directed.|[**undirected\_t**](structhgl_1_1undirected__t.md)|[**c\_hypergraph\_directional\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag)|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The integer type used for element identifiers.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







        

<hr>



### typedef initial\_id\_t 

_A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type._ 
```cpp
using hgl::initial_id_t = typedef gl::initial_id_t;
```





**See also:** [**gl::initial\_id\_t**](structgl_1_1initial__id__t.md) 



        

<hr>



### typedef invalid\_id\_t 

_A helper type that can be implicitly converted to the invalid ID value for any valid ID type._ 
```cpp
using hgl::invalid_id_t = typedef gl::invalid_id_t;
```





**See also:** [**gl::invalid\_id\_t**](structgl_1_1invalid__id__t.md) 



        

<hr>



### typedef list\_hypergraph 

_Convenience alias for a hypergraph utilizing a standard incidence list representation model._ 
```cpp
template<traits::c_hypergraph_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::list_hypergraph = typedef hypergraph< list_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType> >;
```




<hr>



### typedef list\_hypergraph\_traits 

_Convenience alias for_ `hypergraph_traits` _using a standard incidence list representation._
```cpp
template<traits::c_hypergraph_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::list_hypergraph_traits = typedef hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, repr::list_t<LayoutTag, IdType> >;
```



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|LayoutTag|Memory orientation for the list structures.|[**bidirectional\_t**](structhgl_1_1repr_1_1bidirectional__t.md)|[**c\_hypergraph\_layout\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-layout-tag)|
|DirectionalTag|Specifies whether the hypergraph is undirected or bf\_directed.|[**undirected\_t**](structhgl_1_1undirected__t.md)|[**c\_hypergraph\_directional\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag)|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The integer type used for element identifiers.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







        

<hr>



### typedef matrix\_hypergraph 

_Convenience alias for a hypergraph utilizing a standard incidence matrix representation model._ 
```cpp
template<traits::c_hypergraph_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::matrix_hypergraph = typedef hypergraph< matrix_hypergraph_traits<LayoutTag, DirectionalTag, VertexProperties, HyperedgeProperties, IdType> >;
```




<hr>



### typedef matrix\_hypergraph\_traits 

_Convenience alias for_ `hypergraph_traits` _using a standard incidence matrix representation._
```cpp
template<traits::c_hypergraph_asymmetric_layout_tag LayoutTag, traits::c_hypergraph_directional_tag DirectionalTag, traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_id_type IdType>
using hgl::matrix_hypergraph_traits = typedef hypergraph_traits< DirectionalTag, VertexProperties, HyperedgeProperties, repr::matrix_t<LayoutTag, IdType> >;
```



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|LayoutTag|Memory orientation (must be asymmetric).|[**hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md)|[**c\_hypergraph\_asymmetric\_layout\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-asymmetric-layout-tag)|
|DirectionalTag|Specifies whether the hypergraph is undirected or bf\_directed.|[**undirected\_t**](structhgl_1_1undirected__t.md)|[**c\_hypergraph\_directional\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-directional-tag)|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The integer type used for element identifiers.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







        

<hr>



### typedef name\_property 

_A property struct providing a basic string-based naming facility._ 
```cpp
using hgl::name_property = typedef gl::name_property;
```



#### See Also




* [**gl::name\_property**](structgl_1_1name__property.md) : For the full type definition. 





        

<hr>



### typedef undirected\_hypergraph 

_Convenience alias for an undirected hypergraph._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_hypergraph_repr_tag ReprTag>
using hgl::undirected_hypergraph = typedef hypergraph<undirected_hypergraph_traits<VertexProperties, HyperedgeProperties, ReprTag> >;
```




<hr>



### typedef undirected\_hypergraph\_traits 

_Type alias for undirected hypergraph traits with configurable properties and representation._ 
```cpp
template<traits::c_properties VertexProperties, traits::c_properties HyperedgeProperties, traits::c_hypergraph_repr_tag ReprTag>
using hgl::undirected_hypergraph_traits = typedef hypergraph_traits<undirected_t, VertexProperties, HyperedgeProperties, ReprTag>;
```



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|VertexProperties|The type of properties attached to each vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|HyperedgeProperties|The type of properties attached to each hyperedge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|ReprTag|Specifies the internal container representation for incidence.|[**repr::list\_t&lt;&gt;**](structhgl_1_1repr_1_1list__t.md)|[**c\_hypergraph\_repr\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag)|







        

<hr>



### typedef vertex\_descriptor 

_Type alias adapting the standard graph vertex descriptor for hypergraphs._ 
```cpp
template<traits::c_properties Properties, traits::c_id_type IdType>
using hgl::vertex_descriptor = typedef gl::vertex_descriptor<Properties, IdType>;
```



Because hypergraphs and standard graphs share the same fundamental vertex representation, this alias imports the [**gl::vertex\_descriptor**](classgl_1_1vertex__descriptor.md) type into the HGL module. It provides a type-safe wrapper combining a unique identifier with optional property data.



> [!WARNING] This class is not intended to be instantiated directly.
>
>
> Instead, `vertex_descriptor` objects should be retrieved from the [**hgl::hypergraph**](classhgl_1_1hypergraph.md) class instance that owns the given vertex. 
>
>



#### Example Usage




```cpp
std::cout << gl::io::verbose << gl::io::with_vertex_properties; // (1)!

for (const auto& vertex : hypergraph.vertices()) {
    const auto deg = hypergraph.degree(vertex); // (2)!

    if (deg == 0uz)
        vertex->description = "Isolated node"; // (3)!
    else
        vertex->description = std::format("Connected to {} hyperedges", deg);

    std::cout << "Vertex details: " << vertex << '\n';
}
```




1. Stream manipulators inject persistent formatting state globally before iterating.
2. Use the owning hypergraph to query topological information about the vertex.
3. Safely access and modify the underlying properties via the `->` operator (assuming the vertex properties type contains a `description` member).





#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|Properties|The type of property data attached to the vertex.|[**empty\_properties**](group__HGL-Core.md#typedef-empty_properties)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The underlying integer type used for the IDs.|[**default\_id\_type**](group__HGL-Types.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







#### See Also




* [**gl::vertex\_descriptor**](classgl_1_1vertex__descriptor.md) for the full definition of the type.
* [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) for the corresponding hyperedge wrapper class.
* [**hypergraph**](classhgl_1_1hypergraph.md) for the owning hypergraph class that manages vertex descriptors. 





        

<hr>



### typedef weight\_property 

_A property struct providing arithmetic weight for hyperedges or vertices._ 
```cpp
template<traits::c_arithmetic WeightType>
using hgl::weight_property = typedef gl::weight_property<WeightType>;
```



#### See Also




* [**gl::weight\_property**](structgl_1_1weight__property.md) : For the full type definition. 





        

<hr>
## Public Attributes Documentation




### variable hyperedge 

_A constant instance of_ `hyperedge_t` _used for tagging and generic dispatching._
```cpp
constexpr hyperedge_t hgl::hyperedge = {};
```




<hr>



### variable initial\_id 

_An_ `initial_id_t` _tag constant that can be used to represent the initial ID value of 0 for hypergraph elements in a type-safe manner._
```cpp
constexpr initial_id_t hgl::initial_id = gl::initial_id;
```





**See also:** [**gl::initial\_id**](group__GL-Core.md#variable-initial_id) 



        

<hr>



### variable initial\_id\_v 

_A constant representing the initial ID value of 0 for hypergraph elements._ 
```cpp
template<traits::c_id_type IdType>
constexpr IdType hgl::initial_id_v = gl::initial_id_v<IdType>;
```





**See also:** [**gl::initial\_id\_v**](group__GL-Core.md#variable-initial_id_v) 



        

<hr>



### variable invalid\_id 

_An_ `invalid_id_t` _tag constant that can be used to represent the invalid ID value for hypergraph elements in a type-safe manner._
```cpp
constexpr invalid_id_t hgl::invalid_id = gl::invalid_id;
```





**See also:** [**gl::invalid\_id**](group__GL-Core.md#variable-invalid_id) 



        

<hr>



### variable invalid\_id\_v 

_A constant representing the invalid ID value for hypergraph elements, defined as the maximum value of the specified ID type._ 
```cpp
template<traits::c_id_type IdType>
constexpr IdType hgl::invalid_id_v = gl::invalid_id_v<IdType>;
```





**See also:** [**gl::invalid\_id\_v**](group__GL-Core.md#variable-invalid_id_v) 



        

<hr>



### variable vertex 

_A constant instance of_ `vertex_t` _used for tagging and generic dispatching._
```cpp
constexpr vertex_t hgl::vertex = {};
```




<hr>
## Public Functions Documentation




### function clone 

_Creates a deep copy of a given hypergraph._ 
```cpp
template<traits::c_hypergraph Hypergraph>
Hypergraph clone (
    const Hypergraph & source
) 
```



Creates a deep copy of the given hypergraph.




**Template parameters:**


* `Hypergraph` The concrete hypergraph type. 



**Parameters:**


* `source` The hypergraph to clone. 



**Returns:**

A duplicated instance of the source hypergraph.




**Template parameters:**


* `Hypergraph` The type of the hypergraph. 



**Parameters:**


* `source` The hypergraph instance to clone. 



**Returns:**

A newly constructed hypergraph containing identical vertices, hyperedges and properties (if applicable). 





        

<hr>



### function incidence\_graph 

_Computes the bipartite incidence graph representation of an_ _undirected_ _hypergraph._
```cpp
template<gl::traits::c_undirected_graph G>
G incidence_graph (
    const traits::c_undirected_hypergraph auto & h
) 
```



Converts the hypergraph into a standard bipartite graph where both the original vertices and the original hyperedges are represented as standard graph vertices. Undirected edges are created between a vertex node and a hyperedge node if they are incident.



> [!NOTE] ID Shifting
>
>
> To ensure uniqueness in the resulting graph, the IDs of the hyperedge nodes are shifted by `h.n_vertices()`. For example, hyperedge ID `0` becomes vertex ID `h.n_vertices() + 0` in the resulting graph. 
>
>

> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested target graph `G` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**gl::to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `G` The target standard graph type to construct. Must satisfy [**c\_undirected\_graph**](gl_concepts.md#gl-traits-c-undirected-graph). 



**Parameters:**


* `h` The source undirected hypergraph. 



**Returns:**

A standard undirected bipartite graph representing the incidence structure. 





        

<hr>



### function incidence\_graph 

_Computes the bipartite incidence graph representation of a_ _BF-directed_ _hypergraph._
```cpp
template<gl::traits::c_directed_graph G>
G incidence_graph (
    const traits::c_bf_directed_hypergraph auto & h
) 
```



Converts the hypergraph into a standard bipartite directed graph. Directed edges are created from original _tail_ vertex nodes to the hyperedge nodes, and from the hyperedge nodes to the original _head_ vertex nodes.



> [!NOTE] ID Shifting
>
>
> To ensure uniqueness in the resulting graph, the IDs of the hyperedge nodes are shifted by `h.n_vertices()`. For example, hyperedge ID `0` becomes vertex ID `h.n_vertices() + 0` in the resulting graph. 
>
>

> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested target graph `G` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**gl::to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `G` The target standard graph type to construct. Must satisfy [**c\_directed\_graph**](gl_concepts.md#gl-traits-c-directed-graph). 



**Parameters:**


* `h` The source bf-directed hypergraph. 



**Returns:**

A standard directed bipartite graph representing the incidence structure. 





        

<hr>



### function projection 

_Computes the projection (clique expansion) of an_ _undirected_ _hypergraph._
```cpp
template<gl::traits::c_undirected_graph G>
G projection (
    const traits::c_undirected_hypergraph auto & h
) 
```



Projects the hypergraph onto a standard undirected graph. Each hyperedge in the hypergraph is expanded into a clique (a fully connected subgraph) connecting all of its incident vertices in the resulting graph. Duplicate edges generated by multiple hyperedges overlapping on the same vertices are collapsed into a single edge.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested target graph `G` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**gl::to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `G` The target standard graph type to construct. Must satisfy [**c\_undirected\_graph**](gl_concepts.md#gl-traits-c-undirected-graph). 



**Parameters:**


* `h` The source undirected hypergraph. 



**Returns:**

A standard undirected graph representing the projection. 





        

<hr>



### function projection 

_Computes the projection of a_ _BF-directed_ _hypergraph._
```cpp
template<gl::traits::c_directed_graph G>
G projection (
    const traits::c_bf_directed_hypergraph auto & h
) 
```



Projects the hypergraph onto a standard directed graph. For each hyperedge, directed edges are created from every vertex in the hyperedge's tail (source) to every vertex in its head (destination). Duplicate edges generated by multiple hyperedges overlapping on the same vertices are collapsed into a single edge.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested target graph `G` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**gl::to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `G` The target standard graph type to construct. Must satisfy [**c\_directed\_graph**](gl_concepts.md#gl-traits-c-directed-graph). 



**Parameters:**


* `h` The source bf-directed hypergraph. 



**Returns:**

A standard directed graph representing the projection. 





        

<hr>



### function to 

_Converts a hypergraph from one representation model to another._ 
```cpp
template<traits::c_hypergraph_repr_tag TargetReprTag, traits::c_hypergraph Hypergraph>
auto to (
    Hypergraph && source
) 
```



Converts a hypergraph to a different representation type.


Converts a hypergraph to a different representation type (e.g., from incidence list to incidence matrix).


This function efficiently transforms a hypergraph's underlying memory representation (e.g., from a standard incidence list to a flattened incidence list) while preserving its exact topology, properties, and identifiers.


#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|TargetReprTag|The representation tag of the desired target representation (e.g., `hgl::repr::flat_list_t`).|[**c\_hypergraph\_repr\_tag**](hgl_concepts.md#hgl-traits-c-hypergraph-repr-tag)|
|Hypergraph|The type of the source hypergraph, which will be automatically deduced from the function argument.|[**c\_hypergraph**](hgl_concepts.md#hgl-traits-c-hypergraph)|








**Parameters:**


* `source` The hypergraph to convert. After the operation it will be left in a valid, empty state. 



**Returns:**

A new hypergraph containing the moved data, structured according to `TargetReprTag`.






#### See Also




* [**swap\_repr\_tag**](structhgl_1_1traits_1_1swap__repr__tag.md) : For the trait used to resolve the target hypergraph type with the swapped representation tag.






**Template parameters:**


* `TargetReprTag` The representation tag defining the target storage mechanism. 
* `Hypergraph` The concrete hypergraph type of the source. 



**Parameters:**


* `source` The hypergraph to convert. 



**Returns:**

A new hypergraph matching the target representation type with identical topology and properties. 






        

<hr>

------------------------------


