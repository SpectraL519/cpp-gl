

# Class gl::vertex\_descriptor

**template &lt;traits::c\_properties Properties, traits::c\_id\_type IdType&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md)



_A lightweight wrapper around a vertex identifier with optional properties._ [More...](#detailed-description)

* `#include <gl/vertex_descriptor.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**IdType**](group__GL-Core.md#variable-invalid_id_v) | [**id\_type**](#typedef-id_type)  <br>_The vertex identifier type._  |
| typedef [**Properties**](group__GL-Core.md#variable-invalid_id_v) | [**properties\_type**](#typedef-properties_type)  <br>_The type of properties associated with the vertex._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**vertex\_descriptor**](#function-vertex_descriptor-15) () <br>_Default constructor, initializes to an invalid vertex._  |
|   | [**vertex\_descriptor**](#function-vertex_descriptor-25) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1vertex__descriptor.md#typedef-id_type) id) <br>_Constructs a vertex descriptor with the given ID (for empty properties)._  |
|   | [**vertex\_descriptor**](#function-vertex_descriptor-35) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**id\_type**](classgl_1_1vertex__descriptor.md#typedef-id_type) id, [**properties\_type**](classgl_1_1vertex__descriptor.md#typedef-properties_type) & properties) <br>_Constructs a vertex descriptor with the given ID and properties._  |
|   | [**vertex\_descriptor**](#function-vertex_descriptor-45) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) &) = default<br>_Copy constructor._  |
|  [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) &) = default<br>_Copy assignment operator._  |
|   | [**vertex\_descriptor**](#function-vertex_descriptor-55) ([**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) &&) noexcept<br>_Move constructor._  |
|  [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) & | [**operator=**](#function-operator_1) ([**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) &&) noexcept<br>_Move assignment operator._  |
|   | [**~vertex\_descriptor**](#function-vertex_descriptor) () = default<br>_Destructor._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_2) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) & other) noexcept const<br>_Equality comparison operator._  |
|  std::strong\_ordering | [**operator&lt;=&gt;**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) & other) noexcept const<br>_Three-way comparison operator._  |
|   | [**operator bool**](#function-operator-bool) () noexcept const<br>_Boolean conversion operator._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**is\_valid**](#function-is_valid) () noexcept const<br>_Checks if the vertex descriptor is valid._  |
|  [**id\_type**](classgl_1_1vertex__descriptor.md#typedef-id_type) | [**id**](#function-id) () noexcept const<br>_Returns the vertex ID._  |
|  [**properties\_type**](classgl_1_1vertex__descriptor.md#typedef-properties_type) & | [**properties**](#function-properties) () const<br>_Returns a reference to the vertex properties._  |
|  [**properties\_type**](classgl_1_1vertex__descriptor.md#typedef-properties_type) \* | [**operator-&gt;**](#function-operator-) () const<br>_Arrow operator for accessing properties._  |
|  [**properties\_type**](classgl_1_1vertex__descriptor.md#typedef-properties_type) & | [**operator\***](#function-operator_3) () const<br>_Dereference operator for accessing properties._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) | [**invalid**](#function-invalid-12) () noexcept<br>_Returns an invalid vertex descriptor (for empty properties)._  |
|  [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) | [**invalid**](#function-invalid-22) () noexcept<br>_Returns an invalid vertex descriptor (for non-empty properties)._  |


























## Detailed Description


The `vertex_descriptor` class provides a type-safe and efficient way to represent vertices in graph structures. It acts as a lightweight wrapper that combines a unique identifier with optional property data, ensuring safe access and comparison operations.



> [!WARNING] This class is not intended to be instantiated directly.
>
>
> Instead, `vertex_descriptor` objects should be retrieved from the [**graph**](classgl_1_1graph.md) class instance that owns the given vertex. 
>
>



#### Example Usage




```cpp
std::cout << gl::io::with_vertex_properties; // (1)!
for (auto v : graph.vertices()) {
    if (v->parent == gl::invalid_id) // (2)!
        v->level = 0;
    else
        v->level = graph.vertex(v->parent)->level + 1; // (3)!

    std::cout << v << "\n"; // (4)!
}
```



1. Apply the stream manipulator to ensure custom property data is included in the output.


2. Use the arrow operator `->` to read custom properties attached to the vertex (assuming the vertex properties type contains `parent` and `level` members).


3. Access another vertex via the `graph` using its ID, and modify the current vertex's properties.


4. Idiomatic output formatting. _Example output:_ `5[parent: 0, level: 1]`



#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|Properties|The type of property data attached to the vertex.|[**empty\_properties**](structgl_1_1empty__properties.md)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The underlying integer type used for the vertex ID.|[**default\_id\_type**](group__GL-Core.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







#### See Also




* [**edge\_descriptor**](classgl_1_1edge__descriptor.md) : For the corresponding edge wrapper class.
* [**graph**](classgl_1_1graph.md) : For the owning graph class that manages vertex descriptors. 





    
## Public Types Documentation




### typedef id\_type 

_The vertex identifier type._ 
```cpp
using gl::vertex_descriptor< Properties, IdType >::id_type =  IdType;
```




<hr>



### typedef properties\_type 

_The type of properties associated with the vertex._ 
```cpp
using gl::vertex_descriptor< Properties, IdType >::properties_type =  Properties;
```




<hr>
## Public Functions Documentation




### function vertex\_descriptor [1/5]

_Default constructor, initializes to an invalid vertex._ 
```cpp
inline gl::vertex_descriptor::vertex_descriptor () 
```




<hr>



### function vertex\_descriptor [2/5]

_Constructs a vertex descriptor with the given ID (for empty properties)._ 
```cpp
inline explicit gl::vertex_descriptor::vertex_descriptor (
    const  id_type id
) 
```





**Parameters:**


* `id` The unique identifier for the vertex. 




        

<hr>



### function vertex\_descriptor [3/5]

_Constructs a vertex descriptor with the given ID and properties._ 
```cpp
inline explicit gl::vertex_descriptor::vertex_descriptor (
    const  id_type id,
    properties_type & properties
) 
```





**Parameters:**


* `id` The unique identifier for the vertex. 
* `properties` A reference to the property data to associate with this vertex. 




        

<hr>



### function vertex\_descriptor [4/5]

_Copy constructor._ 
```cpp
gl::vertex_descriptor::vertex_descriptor (
    const  vertex_descriptor &
) = default
```




<hr>



### function operator= 

_Copy assignment operator._ 
```cpp
vertex_descriptor & gl::vertex_descriptor::operator= (
    const  vertex_descriptor &
) = default
```




<hr>



### function vertex\_descriptor [5/5]

_Move constructor._ 
```cpp
gl::vertex_descriptor::vertex_descriptor (
    vertex_descriptor &&
) noexcept
```




<hr>



### function operator= 

_Move assignment operator._ 
```cpp
vertex_descriptor & gl::vertex_descriptor::operator= (
    vertex_descriptor &&
) noexcept
```




<hr>



### function ~vertex\_descriptor 

_Destructor._ 
```cpp
gl::vertex_descriptor::~vertex_descriptor () = default
```




<hr>



### function operator== 

_Equality comparison operator._ 
```cpp
inline bool gl::vertex_descriptor::operator== (
    const  vertex_descriptor & other
) noexcept const
```





**Parameters:**


* `other` The vertex descriptor to compare against. 



**Returns:**

`true` if both descriptors hold the same ID, `false` otherwise. 





        

<hr>



### function operator&lt;=&gt; 

_Three-way comparison operator._ 
```cpp
inline std::strong_ordering gl::vertex_descriptor::operator<=> (
    const  vertex_descriptor & other
) noexcept const
```





**Parameters:**


* `other` The vertex descriptor to compare against. 



**Returns:**

The strong ordering result based on the underlying IDs. 





        

<hr>



### function operator bool 

_Boolean conversion operator._ 
```cpp
inline gl::vertex_descriptor::operator bool () noexcept const
```





**Returns:**

`true` if the vertex is valid. 





        

<hr>



### function is\_valid 

_Checks if the vertex descriptor is valid._ 
```cpp
inline bool gl::vertex_descriptor::is_valid () noexcept const
```





**Returns:**

`true` if the ID is not equal to `invalid_id`. 





        

<hr>



### function id 

_Returns the vertex ID._ 
```cpp
inline id_type gl::vertex_descriptor::id () noexcept const
```





**Returns:**

The underlying integer ID of the vertex. 





        

<hr>



### function properties 

_Returns a reference to the vertex properties._ 
```cpp
inline properties_type & gl::vertex_descriptor::properties () const
```





**Returns:**

A reference to the associated `properties_type`. 




**Exception:**


* `std::logic_error` If the vertex descriptor is invalid. 




        

<hr>



### function operator-&gt; 

_Arrow operator for accessing properties._ 
```cpp
inline properties_type * gl::vertex_descriptor::operator-> () const
```





**Returns:**

A pointer to the associated `properties_type`. 




**Exception:**


* `std::logic_error` If the vertex descriptor is invalid. 




        

<hr>



### function operator\* 

_Dereference operator for accessing properties._ 
```cpp
inline properties_type & gl::vertex_descriptor::operator* () const
```





**Returns:**

A reference to the associated `properties_type`. 




**Exception:**


* `std::logic_error` If the vertex descriptor is invalid. 




        

<hr>
## Public Static Functions Documentation




### function invalid [1/2]

_Returns an invalid vertex descriptor (for empty properties)._ 
```cpp
static inline vertex_descriptor gl::vertex_descriptor::invalid () noexcept
```





**Returns:**

A `vertex_descriptor` holding the `invalid_id`. 





        

<hr>



### function invalid [2/2]

_Returns an invalid vertex descriptor (for non-empty properties)._ 
```cpp
static inline vertex_descriptor gl::vertex_descriptor::invalid () noexcept
```





**Returns:**

A `vertex_descriptor` holding the `invalid_id` and empty properties. 





        

<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Output stream operator for vertex descriptors._ 
```cpp
inline std::ostream & gl::vertex_descriptor::operator<< (
    std::ostream & os,
    const  vertex_descriptor & vertex
) 
```





**Parameters:**


* `os` The output stream. 
* `vertex` The vertex descriptor to write. 



**Returns:**

A reference to the output stream. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/vertex_descriptor.hpp`

