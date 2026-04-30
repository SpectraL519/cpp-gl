

# Class gl::edge\_descriptor

**template &lt;traits::c\_graph\_directional\_tag DirectionalTag, traits::c\_properties Properties, traits::c\_id\_type IdType&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**edge\_descriptor**](classgl_1_1edge__descriptor.md)



_A lightweight wrapper representing a graph edge with its endpoints and optional properties._ [More...](#detailed-description)

* `#include <gl/edge_descriptor.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**edge\_descriptor**](classgl_1_1edge__descriptor.md)&lt; [**DirectionalTag**](group__GL-Core.md#variable-invalid_id_v), [**Properties**](group__GL-Core.md#variable-invalid_id_v), [**IdType**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_The fully instantiated type of this edge descriptor._  |
| typedef [**IdType**](group__GL-Core.md#variable-invalid_id_v) | [**id\_type**](#typedef-id_type)  <br>_The identifier type used for edges and vertices._  |
| typedef [**DirectionalTag**](group__GL-Core.md#variable-invalid_id_v) | [**directional\_tag**](#typedef-directional_tag-12)  <br>_The tag denoting whether the edge is directed or undirected._  |
| typedef [**Properties**](group__GL-Core.md#variable-invalid_id_v) | [**properties\_type**](#typedef-properties_type)  <br>_The type of properties associated with the edge._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**friend**](group__GL-Core.md#variable-invalid_id_v) | [**directional\_tag**](#variable-directional_tag-22)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**edge\_descriptor**](#function-edge_descriptor-15) () <br>_Default constructor, initializes to an invalid edge._  |
|   | [**edge\_descriptor**](#function-edge_descriptor-25) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) source, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) target) <br>_Constructs an edge descriptor with the given IDs (for empty properties)._  |
|   | [**edge\_descriptor**](#function-edge_descriptor-35) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) id, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) source, [**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) target, [**properties\_type**](classgl_1_1edge__descriptor.md#typedef-properties_type) & properties) <br>_Constructs an edge descriptor with the given IDs and properties._  |
|   | [**edge\_descriptor**](#function-edge_descriptor-45) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_descriptor**](classgl_1_1edge__descriptor.md) &) = default<br>_Copy constructor._  |
|  [**edge\_descriptor**](classgl_1_1edge__descriptor.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_descriptor**](classgl_1_1edge__descriptor.md) &) = default<br>_Copy assignment operator._  |
|   | [**edge\_descriptor**](#function-edge_descriptor-55) ([**edge\_descriptor**](classgl_1_1edge__descriptor.md) &&) noexcept<br>_Move constructor._  |
|  [**edge\_descriptor**](classgl_1_1edge__descriptor.md) & | [**operator=**](#function-operator_1) ([**edge\_descriptor**](classgl_1_1edge__descriptor.md) &&) noexcept<br>_Move assignment operator._  |
|   | [**~edge\_descriptor**](#function-edge_descriptor) () = default<br>_Destructor._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_2) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_descriptor**](classgl_1_1edge__descriptor.md) & other) noexcept const<br>_Equality comparison operator for directed edges._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_3) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**edge\_descriptor**](classgl_1_1edge__descriptor.md) & other) noexcept const<br>_Equality comparison operator for undirected edges._  |
|   | [**operator bool**](#function-operator-bool) () noexcept const<br>_Boolean conversion operator._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_directed**](#function-is_directed) () noexcept const<br>_Checks if the edge represents a directed connection._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_undirected**](#function-is_undirected) () noexcept const<br>_Checks if the edge represents an undirected connection._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_valid**](#function-is_valid) () noexcept const<br>_Checks if the edge descriptor is valid._  |
|  [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) | [**id**](#function-id) () noexcept const<br>_Returns the edge ID._  |
|  [**homogeneous\_pair**](group__GL-Types.md#typedef-homogeneous_pair)&lt; [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) &gt; | [**incident\_vertices**](#function-incident_vertices) () noexcept const<br>_Retrieves the endpoints of the edge as a pair._  |
|  [**homogeneous\_pair**](group__GL-Types.md#typedef-homogeneous_pair)&lt; [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) &gt; | [**incident\_vertices\_r**](#function-incident_vertices_r) () noexcept const<br>_Retrieves the reversed endpoints of the edge as a pair._  |
|  [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) | [**source**](#function-source) () noexcept const<br>_Returns the source vertex ID._  |
|  [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) | [**target**](#function-target) () noexcept const<br>_Returns the target vertex ID._  |
|  [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) | [**other**](#function-other) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) vertex\_id) const<br>_Gets the other endpoint of the edge given one of its incident vertices._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_incident\_with**](#function-is_incident_with) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) vertex\_id) noexcept const<br>_Checks if a specific vertex is incident to this edge._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_incident\_from**](#function-is_incident_from) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) vertex\_id) noexcept const<br>_Checks if the given vertex acts as the source for this edge._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_incident\_to**](#function-is_incident_to) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1edge__descriptor.md#typedef-id_type) vertex\_id) noexcept const<br>_Checks if the given vertex acts as the target for this edge._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_loop**](#function-is_loop) () noexcept const<br>_Checks if the edge is a self-loop._  |
|  [**properties\_type**](classgl_1_1edge__descriptor.md#typedef-properties_type) & | [**properties**](#function-properties) () const<br>_Returns a reference to the edge properties._  |
|  [**properties\_type**](classgl_1_1edge__descriptor.md#typedef-properties_type) \* | [**operator-&gt;**](#function-operator-) () const<br>_Arrow operator for accessing properties._  |
|  [**properties\_type**](classgl_1_1edge__descriptor.md#typedef-properties_type) & | [**operator\***](#function-operator_4) () const<br>_Dereference operator for accessing properties._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**edge\_descriptor**](classgl_1_1edge__descriptor.md) | [**invalid**](#function-invalid-12) () noexcept<br>_Returns an invalid edge descriptor (for empty properties)._  |
|  [**edge\_descriptor**](classgl_1_1edge__descriptor.md) | [**invalid**](#function-invalid-22) () noexcept<br>_Returns an invalid edge descriptor (for non-empty properties)._  |


























## Detailed Description


The `edge_descriptor` class provides a type-safe and efficient way to represent edges in both directed and undirected graph structures. It encapsulates the unique identifier of the edge, its source and target vertices, and optional property data.



> [!WARNING] This class is not intended to be instantiated directly.
>
>
> Instead, `edge_descriptor` objects should be retrieved from the [**gl::graph**](classgl_1_1graph.md) class instance that owns the given edge. 
>
>



#### Example Usage




```cpp
std::cout << gl::io::verbose << gl::io::with_edge_properties;
for (auto v : graph.vertices()) {
    for (auto e : graph.out_edges(v)) { // (1)!
        if (e.is_loop()) // (2)!
            e->weight = 0.0;
        else
            e->weight += 1.5; // (3)!

        std::cout << e << "\n"; // (4)!
    }
}
```



1. Retrieve edges using valid `graph` traversal methods like `out_edges()`.


2. Utilize built-in edge utility methods to evaluate the edge type.


3. Access and modify custom property fields via the arrow operator `->`.


4. Print the detailed, formatted edge data using the applied stream manipulators. _Example output:_ `[id: 12 | source: 3, target: 7 | weight: 1.5]`



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|DirectionalTag|Tag specifying if the edge is directed or undirected.|[**directed\_t**](structgl_1_1directed__t.md)|[**c\_graph\_directional\_tag**](gl_concepts.md#gl-traits-c-graph-directional-tag)|
|Properties|The type of property data attached to the edge.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The underlying integer type used for the IDs.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







#### See Also




* [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) : For the corresponding vertex wrapper class.
* [**graph**](classgl_1_1graph.md) : For the owning graph class that manages edge descriptors. 





    
## Public Types Documentation




### typedef type 

_The fully instantiated type of this edge descriptor._ 
```cpp
using gl::edge_descriptor< DirectionalTag, Properties, IdType >::type =  edge_descriptor<DirectionalTag, Properties, IdType>;
```




<hr>



### typedef id\_type 

_The identifier type used for edges and vertices._ 
```cpp
using gl::edge_descriptor< DirectionalTag, Properties, IdType >::id_type =  IdType;
```




<hr>



### typedef directional\_tag [1/2]

_The tag denoting whether the edge is directed or undirected._ 
```cpp
using gl::edge_descriptor< DirectionalTag, Properties, IdType >::directional_tag =  DirectionalTag;
```




<hr>



### typedef properties\_type 

_The type of properties associated with the edge._ 
```cpp
using gl::edge_descriptor< DirectionalTag, Properties, IdType >::properties_type =  Properties;
```




<hr>
## Public Attributes Documentation




### variable directional\_tag [2/2]

```cpp
friend gl::edge_descriptor< DirectionalTag, Properties, IdType >::directional_tag;
```




<hr>
## Public Functions Documentation




### function edge\_descriptor [1/5]

_Default constructor, initializes to an invalid edge._ 
```cpp
inline gl::edge_descriptor::edge_descriptor () 
```




<hr>



### function edge\_descriptor [2/5]

_Constructs an edge descriptor with the given IDs (for empty properties)._ 
```cpp
inline explicit gl::edge_descriptor::edge_descriptor (
    const  id_type id,
    const  id_type source,
    const  id_type target
) 
```





**Parameters:**


* `id` The unique identifier for the edge. 
* `source` The unique identifier for the source vertex. 
* `target` The unique identifier for the target vertex. 




        

<hr>



### function edge\_descriptor [3/5]

_Constructs an edge descriptor with the given IDs and properties._ 
```cpp
inline explicit gl::edge_descriptor::edge_descriptor (
    const  id_type id,
    const  id_type source,
    const  id_type target,
    properties_type & properties
) 
```





**Parameters:**


* `id` The unique identifier for the edge. 
* `source` The unique identifier for the source vertex. 
* `target` The unique identifier for the target vertex. 
* `properties` A reference to the property data to associate with this edge. 




        

<hr>



### function edge\_descriptor [4/5]

_Copy constructor._ 
```cpp
gl::edge_descriptor::edge_descriptor (
    const  edge_descriptor &
) = default
```




<hr>



### function operator= 

_Copy assignment operator._ 
```cpp
edge_descriptor & gl::edge_descriptor::operator= (
    const  edge_descriptor &
) = default
```




<hr>



### function edge\_descriptor [5/5]

_Move constructor._ 
```cpp
gl::edge_descriptor::edge_descriptor (
    edge_descriptor &&
) noexcept
```




<hr>



### function operator= 

_Move assignment operator._ 
```cpp
edge_descriptor & gl::edge_descriptor::operator= (
    edge_descriptor &&
) noexcept
```




<hr>



### function ~edge\_descriptor 

_Destructor._ 
```cpp
gl::edge_descriptor::~edge_descriptor () = default
```




<hr>



### function operator== 

_Equality comparison operator for directed edges._ 
```cpp
inline bool gl::edge_descriptor::operator== (
    const  edge_descriptor & other
) noexcept const
```





**Parameters:**


* `other` The edge descriptor to compare against. 



**Returns:**

`true` if IDs and exact endpoint pairs match, `false` otherwise. 





        

<hr>



### function operator== 

_Equality comparison operator for undirected edges._ 
```cpp
inline bool gl::edge_descriptor::operator== (
    const  edge_descriptor & other
) noexcept const
```





**Parameters:**


* `other` The edge descriptor to compare against. 



**Returns:**

`true` if IDs and endpoint pairs match (order independent), `false` otherwise. 





        

<hr>



### function operator bool 

_Boolean conversion operator._ 
```cpp
inline gl::edge_descriptor::operator bool () noexcept const
```





**Returns:**

`true` if the edge is valid. 





        

<hr>



### function is\_directed 

_Checks if the edge represents a directed connection._ 
```cpp
inline constexpr  bool gl::edge_descriptor::is_directed () noexcept const
```





**Returns:**

`true` if `DirectionalTag` is `directed_t`. 





        

<hr>



### function is\_undirected 

_Checks if the edge represents an undirected connection._ 
```cpp
inline constexpr  bool gl::edge_descriptor::is_undirected () noexcept const
```





**Returns:**

`true` if `DirectionalTag` is `undirected_t`. 





        

<hr>



### function is\_valid 

_Checks if the edge descriptor is valid._ 
```cpp
inline bool gl::edge_descriptor::is_valid () noexcept const
```





**Returns:**

`true` if the edge ID and both endpoints are not equal to `invalid_id`. 





        

<hr>



### function id 

_Returns the edge ID._ 
```cpp
inline id_type gl::edge_descriptor::id () noexcept const
```





**Returns:**

The underlying integer ID of the edge. 





        

<hr>



### function incident\_vertices 

_Retrieves the endpoints of the edge as a pair._ 
```cpp
inline homogeneous_pair < id_type > gl::edge_descriptor::incident_vertices () noexcept const
```





**Returns:**

A `homogeneous_pair` representing `(source, target)`. 





        

<hr>



### function incident\_vertices\_r 

_Retrieves the reversed endpoints of the edge as a pair._ 
```cpp
inline homogeneous_pair < id_type > gl::edge_descriptor::incident_vertices_r () noexcept const
```





**Returns:**

A `homogeneous_pair` representing `(target, source)`. 





        

<hr>



### function source 

_Returns the source vertex ID._ 
```cpp
inline id_type gl::edge_descriptor::source () noexcept const
```





**Returns:**

The underlying integer ID of the source vertex. 





        

<hr>



### function target 

_Returns the target vertex ID._ 
```cpp
inline id_type gl::edge_descriptor::target () noexcept const
```





**Returns:**

The underlying integer ID of the target vertex. 





        

<hr>



### function other 

_Gets the other endpoint of the edge given one of its incident vertices._ 
```cpp
inline id_type gl::edge_descriptor::other (
    const  id_type vertex_id
) const
```





**Parameters:**


* `vertex_id` The ID of one incident vertex. 



**Returns:**

The ID of the opposite vertex. 




**Exception:**


* `std::invalid_argument` If the provided `vertex_id` is not incident to this edge. 




        

<hr>



### function is\_incident\_with 

_Checks if a specific vertex is incident to this edge._ 
```cpp
inline bool gl::edge_descriptor::is_incident_with (
    const  id_type vertex_id
) noexcept const
```





**Parameters:**


* `vertex_id` The vertex ID to query. 



**Returns:**

`true` if the vertex is either the source or the target, `false` otherwise. 





        

<hr>



### function is\_incident\_from 

_Checks if the given vertex acts as the source for this edge._ 
```cpp
inline bool gl::edge_descriptor::is_incident_from (
    const  id_type vertex_id
) noexcept const
```





**Parameters:**


* `vertex_id` The vertex ID to query. 



**Returns:**

`true` if the vertex is the source (or any endpoint for undirected graphs). 





        

<hr>



### function is\_incident\_to 

_Checks if the given vertex acts as the target for this edge._ 
```cpp
inline bool gl::edge_descriptor::is_incident_to (
    const  id_type vertex_id
) noexcept const
```





**Parameters:**


* `vertex_id` The vertex ID to query. 



**Returns:**

`true` if the vertex is the target (or any endpoint for undirected graphs). 





        

<hr>



### function is\_loop 

_Checks if the edge is a self-loop._ 
```cpp
inline bool gl::edge_descriptor::is_loop () noexcept const
```





**Returns:**

`true` if the source and target are the same vertex, `false` otherwise. 





        

<hr>



### function properties 

_Returns a reference to the edge properties._ 
```cpp
inline properties_type & gl::edge_descriptor::properties () const
```





**Returns:**

A reference to the associated `properties_type`. 




**Exception:**


* `std::logic_error` If the edge descriptor is invalid. 




        

<hr>



### function operator-&gt; 

_Arrow operator for accessing properties._ 
```cpp
inline properties_type * gl::edge_descriptor::operator-> () const
```





**Returns:**

A pointer to the associated `properties_type`. 




**Exception:**


* `std::logic_error` If the edge descriptor is invalid. 




        

<hr>



### function operator\* 

_Dereference operator for accessing properties._ 
```cpp
inline properties_type & gl::edge_descriptor::operator* () const
```





**Returns:**

A reference to the associated `properties_type`. 




**Exception:**


* `std::logic_error` If the edge descriptor is invalid. 




        

<hr>
## Public Static Functions Documentation




### function invalid [1/2]

_Returns an invalid edge descriptor (for empty properties)._ 
```cpp
static inline edge_descriptor gl::edge_descriptor::invalid () noexcept
```





**Returns:**

An `edge_descriptor` holding `invalid_id` for edge and vertex IDs. 





        

<hr>



### function invalid [2/2]

_Returns an invalid edge descriptor (for non-empty properties)._ 
```cpp
static inline edge_descriptor gl::edge_descriptor::invalid () noexcept
```





**Returns:**

An `edge_descriptor` holding `invalid_id` endpoints and empty properties. 





        

<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Output stream operator for edge descriptors._ 
```cpp
inline std::ostream & gl::edge_descriptor::operator<< (
    std::ostream & os,
    const  edge_descriptor & edge
) 
```





**Parameters:**


* `os` The output stream. 
* `edge` The edge descriptor to write. 



**Returns:**

A reference to the output stream. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/edge_descriptor.hpp`

