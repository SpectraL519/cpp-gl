

# Group HGL-Types



[**Modules**](modules.md) **>** [**HGL-Types**](group__HGL-Types.md)



_Independent, high-performance data structures and utility types._ [More...](#detailed-description)


















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**gl::default\_id\_type**](group__GL-Core.md#typedef-default_id_type) | [**default\_id\_type**](#typedef-default_id_type)  <br>_The default unsigned integer type used for vertex and hyperedge identifiers._  |
| typedef [**gl::flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md)&lt; T &gt; | [**flat\_jagged\_vector**](#typedef-flat_jagged_vector)  <br>_A flattened 2D vector (jagged array) providing efficient storage for variable-length segments._  |
| typedef [**gl::flat\_matrix**](classgl_1_1flat__matrix.md)&lt; T &gt; | [**flat\_matrix**](#typedef-flat_matrix)  <br>_A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements._  |
| typedef [**gl::homogeneous\_pair**](group__GL-Types.md#typedef-homogeneous_pair)&lt; T &gt; | [**homogeneous\_pair**](#typedef-homogeneous_pair)  <br>_A type alias for a_ `std::pair` _where both elements are of the exact same type._ |
| typedef [**gl::size\_type**](group__GL-Core.md#typedef-size_type) | [**size\_type**](#typedef-size_type)  <br>_Type alias for the standard size type used throughout the library._  |
















































## Detailed Description


This module houses a variety of general-purpose data structures and types. Though they are utilized natively by hypergraph implementations, these components are carefully decoupled from specific hypergraph logic, they can be seamlessly extracted and utilized in broader contexts. 


    
## Public Types Documentation




### typedef default\_id\_type 

_The default unsigned integer type used for vertex and hyperedge identifiers._ 
```cpp
using hgl::default_id_type = typedef gl::default_id_type;
```





**See also:** [**gl::default\_id\_type**](group__GL-Core.md#typedef-default_id_type) 



        

<hr>



### typedef flat\_jagged\_vector 

_A flattened 2D vector (jagged array) providing efficient storage for variable-length segments._ 
```cpp
template<std::semiregular T>
using hgl::flat_jagged_vector = typedef gl::flat_jagged_vector<T>;
```



#### See Also




* [**gl::flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) : For the full type definition. 





        

<hr>



### typedef flat\_matrix 

_A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements._ 
```cpp
template<std::semiregular T>
using hgl::flat_matrix = typedef gl::flat_matrix<T>;
```



#### See Also




* [**gl::flat\_matrix**](classgl_1_1flat__matrix.md) : For the full type definition. 





        

<hr>



### typedef homogeneous\_pair 

_A type alias for a_ `std::pair` _where both elements are of the exact same type._
```cpp
template<typename T>
using hgl::homogeneous_pair = typedef gl::homogeneous_pair<T>;
```



#### See Also




* [**gl::homogeneous\_pair**](group__GL-Types.md#typedef-homogeneous_pair) : For the full type definition. 





        

<hr>



### typedef size\_type 

_Type alias for the standard size type used throughout the library._ 
```cpp
using hgl::size_type = typedef gl::size_type;
```



Used primarily for indices, counts, and sizes of hypergraph components. 

**See also:** [**gl::size\_type**](group__GL-Core.md#typedef-size_type) 



        

<hr>

------------------------------


