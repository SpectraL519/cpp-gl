

# Namespace gl



[**Namespace List**](namespaces.md) **>** [**gl**](namespacegl.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**algorithm**](namespacegl_1_1algorithm.md) <br> |
| namespace | [**io**](namespacegl_1_1io.md) <br> |
| namespace | [**repr**](namespacegl_1_1repr.md) <br> |
| namespace | [**topology**](namespacegl_1_1topology.md) <br> |
| namespace | [**traits**](namespacegl_1_1traits.md) <br> |
| namespace | [**util**](namespacegl_1_1util.md) <br> |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**binary\_color**](classgl_1_1binary__color.md) <br>_A specialized color property for algorithms requiring binary states (e.g., bipartition)._  |
| struct | [**binary\_color\_property**](structgl_1_1binary__color__property.md) <br>_A property struct wrapping a_ `binary_color` _._ |
| struct | [**directed\_t**](structgl_1_1directed__t.md) <br>_The tag type representing a directed graph configuration._  |
| class | [**dynamic\_properties**](classgl_1_1dynamic__properties.md) <br>_A type-safe container for heterogeneous properties stored by string keys._  |
| class | [**edge\_descriptor**](classgl_1_1edge__descriptor.md) &lt;DirectionalTag, Properties, IdType&gt;<br>_A lightweight wrapper representing a graph edge with its endpoints and optional properties._  |
| struct | [**empty\_properties**](structgl_1_1empty__properties.md) <br>_A stateless, empty structural tag representing an absence of properties._  |
| struct | [**empty\_properties\_map**](structgl_1_1empty__properties__map.md) <br>_A stateless, empty structural tag indicating that the absence of a property map container._  |
| class | [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) &lt;T&gt;<br>_A flattened 2D vector (jagged array) providing efficient storage for variable-length segments._  |
| class | [**flat\_matrix**](classgl_1_1flat__matrix.md) &lt;T&gt;<br>_A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements._  |
| class | [**graph**](classgl_1_1graph.md) &lt;GraphTraits&gt;<br>_The generic graph container using a policy-based architecture._  |
| struct | [**graph\_traits**](structgl_1_1graph__traits.md) &lt;DirectionalTag, VertexProperties, EdgeProperties, ReprTag, IdType&gt;<br>_Primary graph traits structure that encapsulates all necessary type information for graph representations._  |
| struct | [**initial\_id\_t**](structgl_1_1initial__id__t.md) <br>_A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type._  |
| struct | [**invalid\_id\_t**](structgl_1_1invalid__id__t.md) <br>_A helper type that can be implicitly converted to the invalid ID value for any valid ID type._  |
| struct | [**name\_property**](structgl_1_1name__property.md) <br>_A property struct providing a basic string-based naming facility._  |
| struct | [**undirected\_t**](structgl_1_1undirected__t.md) <br>_The tag type representing an undirected graph configuration._  |
| class | [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) &lt;Properties, IdType&gt;<br>_A lightweight wrapper around a vertex identifier with optional properties._  |
| struct | [**vertex\_distance**](structgl_1_1vertex__distance.md) &lt;GraphType&gt;<br>_Utility trait to resolve the underlying distance or weight numeric type for a graph._  |
| struct | [**vertex\_distance&lt; GraphType &gt;**](structgl_1_1vertex__distance_3_01GraphType_01_4.md) &lt;GraphType&gt;<br>_Specialization resolving the specific weight type when edge properties contain weight attributes._  |
| struct | [**weight\_property**](structgl_1_1weight__property.md) &lt;WeightType&gt;<br>_A property struct providing arithmetic weight for edges or vertices._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::int64\_t | [**default\_vertex\_distance\_type**](#typedef-default_vertex_distance_type)  <br>_Default numeric type representing distances between vertices in unweighted graphs._  |
















































## Public Types Documentation




### typedef default\_vertex\_distance\_type 

_Default numeric type representing distances between vertices in unweighted graphs._ 
```cpp
using gl::default_vertex_distance_type = typedef std::int64_t;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/core.hpp`

