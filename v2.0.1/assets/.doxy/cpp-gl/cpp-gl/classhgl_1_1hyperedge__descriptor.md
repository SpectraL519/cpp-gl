

# Class hgl::hyperedge\_descriptor

**template &lt;traits::c\_properties Properties, traits::c\_id\_type IdType&gt;**



[**ClassList**](annotated.md) **>** [**hgl**](namespacehgl.md) **>** [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md)



_A lightweight wrapper representing a hypergraph edge with optional properties._ [More...](#detailed-description)

* `#include <hgl/hypergraph_elements.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md)&lt; [**Properties**](group__HGL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_Self type alias._  |
| typedef [**IdType**](group__HGL-Core.md#variable-invalid_id_v) | [**id\_type**](#typedef-id_type)  <br>_The underlying integer type used for the hyperedge identifier._  |
| typedef [**Properties**](group__HGL-Core.md#variable-invalid_id_v) | [**properties\_type**](#typedef-properties_type)  <br>_The type of property data attached to the hyperedge._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**hyperedge\_descriptor**](#function-hyperedge_descriptor-15) () <br>_Default constructor creating an invalid hyperedge descriptor._  |
|   | [**hyperedge\_descriptor**](#function-hyperedge_descriptor-25) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-id_type) id) <br>_Constructs a property-less hyperedge descriptor from a raw ID._  |
|   | [**hyperedge\_descriptor**](#function-hyperedge_descriptor-35) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**id\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-id_type) id, [**properties\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-properties_type) & properties) <br>_Constructs a hyperedge descriptor binding an ID to its properties._  |
|   | [**hyperedge\_descriptor**](#function-hyperedge_descriptor-45) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) &) = default<br>_Default copy constructor._  |
|  [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) & | [**operator=**](#function-operator) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) &) = default<br>_Default copy assignment operator._  |
|   | [**hyperedge\_descriptor**](#function-hyperedge_descriptor-55) ([**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) &&) noexcept<br>_Default move constructor._  |
|  [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) & | [**operator=**](#function-operator_1) ([**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) &&) noexcept<br>_Default move assignment operator._  |
|   | [**~hyperedge\_descriptor**](#function-hyperedge_descriptor) () = default<br>_Default destructor._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_2) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) & other) noexcept const<br>_Compares two hyperedge descriptors for equality._  |
|   | [**operator bool**](#function-operator-bool) () noexcept const<br>_Contextually converts the descriptor to a boolean._  |
|  std::strong\_ordering | [**operator&lt;=&gt;**](#function-operator) ([**const**](group__HGL-Core.md#variable-invalid_id_v) [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) & other) noexcept const<br>_Compares two hyperedge descriptors to establish a strict ordering based on their IDs._  |
|  [**bool**](group__HGL-Core.md#variable-invalid_id_v) | [**is\_valid**](#function-is_valid) () noexcept const<br>_Checks if the descriptor represents a valid hyperedge._  |
|  [**id\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-id_type) | [**id**](#function-id) () noexcept const<br>_Retrieves the raw underlying ID of the hyperedge._  |
|  [**properties\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-properties_type) & | [**properties**](#function-properties) () const<br>_Retrieves a reference to the property payload attached to the hyperedge._  |
|  [**properties\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-properties_type) \* | [**operator-&gt;**](#function-operator-) () const<br>_Arrow operator providing direct access to the hyperedge's properties._  |
|  [**properties\_type**](classhgl_1_1hyperedge__descriptor.md#typedef-properties_type) & | [**operator\***](#function-operator_3) () const<br>_Dereference operator providing direct access to the hyperedge's properties._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) | [**invalid**](#function-invalid-12) () noexcept<br>_Returns a special descriptor representing an invalid or uninitialized property-less hyperedge._  |
|  [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) | [**invalid**](#function-invalid-22) () noexcept<br>_Returns a special descriptor representing an invalid or uninitialized hyperedge with properties._  |


























## Detailed Description


The `hyperedge_descriptor` class provides a type-safe and efficient way to represent hyperedges in hypergraph structures. It acts as a lightweight wrapper that encapsulates the unique identifier of the hyperedge and its optional property data, ensuring safe access and comparison operations.



> [!WARNING] This class is not intended to be instantiated directly.
>
>
> Instead, `hyperedge_descriptor` objects should be retrieved from the [**hgl::hypergraph**](classhgl_1_1hypergraph.md) class instance that owns the given hyperedge. 
>
>



#### Example Usage




```cpp
std::cout << gl::io::verbose << gl::io::with_hyperedge_properties; // (1)!

for (const auto& hyperedge : hypergraph.hyperedges()) {
    const auto size = hypergraph.hyperedge_size(hyperedge); // (2)!
    hyperedge->weight = static_cast<double>(size) * 1.5; // (3)!
    std::cout << "Hyperedge details: " << hyperedge << '\n';
}
```




1. Stream manipulators like `with_hyperedge_properties` apply persistently to `std::cout`.
2. Use the owning hypergraph to query topological information about the hyperedge.
3. Update the payload properties through the overloaded `->` operator (assuming the hyperedge properties type contains a `weight` member).





#### Template Parameters




|Parameter|Description|Default|Constraint|
|-----|-----|-----|-----|
|Properties|The type of property data attached to the hyperedge.|[**empty\_properties**](group__HGL-Core.md#typedef-empty_properties)|[**c\_properties**](gl_concepts.md#gl-traits-c-properties)|
|IdType|The underlying integer type used for the IDs.|[**default\_id\_type**](group__HGL-Types.md#typedef-default_id_type)|[**c\_id\_type**](gl_concepts.md#gl-traits-c-id-type)|







#### See Also




* [**vertex\_descriptor**](group__HGL-Core.md#typedef-vertex_descriptor) for the corresponding vertex wrapper class.
* [**hypergraph**](classhgl_1_1hypergraph.md) for the owning hypergraph class that manages hyperedge descriptors. 





    
## Public Types Documentation




### typedef type 

_Self type alias._ 
```cpp
using hgl::hyperedge_descriptor< Properties, IdType >::type =  hyperedge_descriptor<Properties>;
```




<hr>



### typedef id\_type 

_The underlying integer type used for the hyperedge identifier._ 
```cpp
using hgl::hyperedge_descriptor< Properties, IdType >::id_type =  IdType;
```




<hr>



### typedef properties\_type 

_The type of property data attached to the hyperedge._ 
```cpp
using hgl::hyperedge_descriptor< Properties, IdType >::properties_type =  Properties;
```




<hr>
## Public Functions Documentation




### function hyperedge\_descriptor [1/5]

_Default constructor creating an invalid hyperedge descriptor._ 
```cpp
inline hgl::hyperedge_descriptor::hyperedge_descriptor () 
```




<hr>



### function hyperedge\_descriptor [2/5]

_Constructs a property-less hyperedge descriptor from a raw ID._ 
```cpp
inline explicit hgl::hyperedge_descriptor::hyperedge_descriptor (
    const  id_type id
) 
```





**Parameters:**


* `id` The raw identifier of the hyperedge. 




        

<hr>



### function hyperedge\_descriptor [3/5]

_Constructs a hyperedge descriptor binding an ID to its properties._ 
```cpp
inline explicit hgl::hyperedge_descriptor::hyperedge_descriptor (
    const  id_type id,
    properties_type & properties
) 
```





**Parameters:**


* `id` The raw identifier of the hyperedge. 
* `properties` A reference to the underlying properties payload. 




        

<hr>



### function hyperedge\_descriptor [4/5]

_Default copy constructor._ 
```cpp
hgl::hyperedge_descriptor::hyperedge_descriptor (
    const  hyperedge_descriptor &
) = default
```




<hr>



### function operator= 

_Default copy assignment operator._ 
```cpp
hyperedge_descriptor & hgl::hyperedge_descriptor::operator= (
    const  hyperedge_descriptor &
) = default
```




<hr>



### function hyperedge\_descriptor [5/5]

_Default move constructor._ 
```cpp
hgl::hyperedge_descriptor::hyperedge_descriptor (
    hyperedge_descriptor &&
) noexcept
```




<hr>



### function operator= 

_Default move assignment operator._ 
```cpp
hyperedge_descriptor & hgl::hyperedge_descriptor::operator= (
    hyperedge_descriptor &&
) noexcept
```




<hr>



### function ~hyperedge\_descriptor 

_Default destructor._ 
```cpp
hgl::hyperedge_descriptor::~hyperedge_descriptor () = default
```




<hr>



### function operator== 

_Compares two hyperedge descriptors for equality._ 
```cpp
inline bool hgl::hyperedge_descriptor::operator== (
    const  hyperedge_descriptor & other
) noexcept const
```





**Parameters:**


* `other` The descriptor to compare against. 



**Returns:**

`true` if both descriptors hold the same ID, `false` otherwise. 





        

<hr>



### function operator bool 

_Contextually converts the descriptor to a boolean._ 
```cpp
inline hgl::hyperedge_descriptor::operator bool () noexcept const
```





**Returns:**

`true` if the descriptor is valid, `false` otherwise. 





        

<hr>



### function operator&lt;=&gt; 

_Compares two hyperedge descriptors to establish a strict ordering based on their IDs._ 
```cpp
inline std::strong_ordering hgl::hyperedge_descriptor::operator<=> (
    const  hyperedge_descriptor & other
) noexcept const
```





**Parameters:**


* `other` The descriptor to compare against. 



**Returns:**

The result of the three-way comparison between the underlying IDs. 





        

<hr>



### function is\_valid 

_Checks if the descriptor represents a valid hyperedge._ 
```cpp
inline bool hgl::hyperedge_descriptor::is_valid () noexcept const
```





**Returns:**

`true` if the underlying ID is not the invalid ID constant, `false` otherwise. 





        

<hr>



### function id 

_Retrieves the raw underlying ID of the hyperedge._ 
```cpp
inline id_type hgl::hyperedge_descriptor::id () noexcept const
```





**Returns:**

The hyperedge's integer ID. 





        

<hr>



### function properties 

_Retrieves a reference to the property payload attached to the hyperedge._ 
```cpp
inline properties_type & hgl::hyperedge_descriptor::properties () const
```





**Returns:**

A mutable reference to the underlying properties. 




**Exception:**


* `std::logic_error` If the descriptor is invalid. 




        

<hr>



### function operator-&gt; 

_Arrow operator providing direct access to the hyperedge's properties._ 
```cpp
inline properties_type * hgl::hyperedge_descriptor::operator-> () const
```





**Returns:**

A pointer to the underlying properties. 




**Exception:**


* `std::logic_error` If the descriptor is invalid. 




        

<hr>



### function operator\* 

_Dereference operator providing direct access to the hyperedge's properties._ 
```cpp
inline properties_type & hgl::hyperedge_descriptor::operator* () const
```





**Returns:**

A reference to the underlying properties. 




**Exception:**


* `std::logic_error` If the descriptor is invalid. 




        

<hr>
## Public Static Functions Documentation




### function invalid [1/2]

_Returns a special descriptor representing an invalid or uninitialized property-less hyperedge._ 
```cpp
static inline hyperedge_descriptor hgl::hyperedge_descriptor::invalid () noexcept
```





**Returns:**

An invalid `hyperedge_descriptor`. 





        

<hr>



### function invalid [2/2]

_Returns a special descriptor representing an invalid or uninitialized hyperedge with properties._ 
```cpp
static inline hyperedge_descriptor hgl::hyperedge_descriptor::invalid () noexcept
```





**Returns:**

An invalid `hyperedge_descriptor`. 





        

<hr>## Friends Documentation





### friend operator&lt;&lt; 

_Serializes the hyperedge descriptor to an output stream._ 
```cpp
inline std::ostream & hgl::hyperedge_descriptor::operator<< (
    std::ostream & os,
    const  hyperedge_descriptor & hyperedge
) 
```



Depending on active stream flags, this outputs the descriptor in verbose or concise formats.




**Parameters:**


* `os` The target output stream. 
* `hyperedge` The hyperedge descriptor to format. 



**Returns:**

The stream reference for chaining. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/hypergraph_elements.hpp`

