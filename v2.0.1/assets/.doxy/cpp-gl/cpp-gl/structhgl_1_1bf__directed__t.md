

# Struct hgl::bf\_directed\_t



[**ClassList**](annotated.md) **>** [**hgl**](namespacehgl.md) **>** [**bf\_directed\_t**](structhgl_1_1bf__directed__t.md)



_Tag type specifying that a hypergraph is backward-forward (BF) directed._ [More...](#detailed-description)

* `#include <hgl/directional_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::type\_identity\_t&lt; [**bf\_directed\_t**](structhgl_1_1bf__directed__t.md) &gt; | [**type**](#typedef-type)  <br>_Self type identity for internal metaprogramming use._  |
















































## Detailed Description


In a BF-directed hypergraph, each hyperedge maps a distinct set of _tail_ vertices (origins) to a distinct set of _head_ vertices (destinations). 


    
## Public Types Documentation




### typedef type 

_Self type identity for internal metaprogramming use._ 
```cpp
using hgl::bf_directed_t::type =  std::type_identity_t<bf_directed_t>;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/directional_tags.hpp`

