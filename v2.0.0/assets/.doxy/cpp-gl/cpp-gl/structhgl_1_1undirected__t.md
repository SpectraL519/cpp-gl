

# Struct hgl::undirected\_t



[**ClassList**](annotated.md) **>** [**hgl**](namespacehgl.md) **>** [**undirected\_t**](structhgl_1_1undirected__t.md)



_Tag type specifying that a hypergraph is undirected._ [More...](#detailed-description)

* `#include <hgl/directional_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::type\_identity\_t&lt; [**undirected\_t**](structhgl_1_1undirected__t.md) &gt; | [**type**](#typedef-type)  <br>_Self type identity for internal metaprogramming use._  |
















































## Detailed Description


In an undirected hypergraph, a hyperedge is strictly defined as a set of incident vertices, with no distinction between origin and destination. 


    
## Public Types Documentation




### typedef type 

_Self type identity for internal metaprogramming use._ 
```cpp
using hgl::undirected_t::type =  std::type_identity_t<undirected_t>;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/directional_tags.hpp`

