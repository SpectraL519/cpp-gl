

# Group GL-Types



[**Modules**](modules.md) **>** [**GL-Types**](group__GL-Types.md)



_Independent, high-performance data structures and utility types._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**gl::algorithm::decision**](structgl_1_1algorithm_1_1decision.md) <br>_Represents a generic tri-state decision for control flow._  |
| class | [**gl::flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) &lt;T&gt;<br>_A flattened 2D vector (jagged array) providing efficient storage for variable-length segments._  |
| class | [**gl::flat\_matrix**](classgl_1_1flat__matrix.md) &lt;T&gt;<br>_A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements._  |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::pair&lt; T, T &gt; | [**homogeneous\_pair**](#typedef-homogeneous_pair)  <br>_A type alias for a_ `std::pair` _where both elements are of the exact same type._ |
















































## Detailed Description


This module houses a variety of general-purpose data structures and types. While it features highly optimized memory containers (such as [**flat\_matrix**](classgl_1_1flat__matrix.md) and [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md)) that serve as the contiguous-memory backbones for the library's graphs, it also encompasses other fundamental type utilities. Because all components in this group are carefully decoupled from graph-specific logic, they can be seamlessly extracted and utilized in broader C++ programming contexts. 


    
## Public Types Documentation




### typedef homogeneous\_pair 

_A type alias for a_ `std::pair` _where both elements are of the exact same type._
```cpp
template<typename T>
using gl::homogeneous_pair = typedef std::pair<T, T>;
```





**Template parameters:**


* ``T`` The type of both the `first` and `second` elements in the pair. 




        

<hr>

------------------------------


