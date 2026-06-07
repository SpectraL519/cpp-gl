

# Struct hgl::repr::flat\_list\_t

**template &lt;traits::c\_hypergraph\_layout\_tag LayoutTag, traits::c\_id\_type IdType&gt;**



[**ClassList**](annotated.md) **>** [**repr**](namespacehgl_1_1repr.md) **>** [**flat\_list\_t**](structhgl_1_1repr_1_1flat__list__t.md)



_Tag struct for the flattened incidence list hypergraph representation._ [More...](#detailed-description)

* `#include <hgl/repr/repr_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**flat\_list\_t**](structhgl_1_1repr_1_1flat__list__t.md)&lt; [**LayoutTag**](group__HGL-Core.md#variable-invalid_id_v), [**IdType**](group__HGL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_Self type alias._  |
| typedef [**LayoutTag**](group__HGL-Core.md#variable-invalid_id_v) | [**layout\_tag**](#typedef-layout_tag)  <br>_The configured layout tag._  |
| typedef [**IdType**](group__HGL-Core.md#variable-invalid_id_v) | [**id\_type**](#typedef-id_type)  <br>_The configured identifier type._  |
| typedef impl::flat\_incidence\_list&lt; [**DirectionalTag**](group__HGL-Core.md#variable-invalid_id_v), [**type**](structhgl_1_1repr_1_1flat__list__t.md#typedef-type) &gt; | [**representation\_type**](#typedef-representation_type)  <br>_Type alias for the underlying flattened incidence list hypergraph representation based on the provided directional tag._  |
















































## Detailed Description


#### Layout Implications



The chosen layout significantly impacts memory usage and query performance:



* [**bidirectional\_t**](structhgl_1_1repr_1_1bidirectional__t.md) (Default): Maintains two internal flattened lists (vertex-to-hyperedges and hyperedge-to-vertices). Provides optimal \(O(1)\) degree/size lookups and fast traversals in both directions at the cost of doubled memory consumption.
* [**vertex\_major\_t**](structhgl_1_1repr_1_1vertex__major__t.md): Maintains only a vertex-to-hyperedges flattened list. Highly memory efficient and fast for querying vertex degrees or incident hyperedge sets, but querying hyperedge sizes or incident vertex sets requires expensive full-graph scans.
* [**hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md): Maintains only a hyperedge-to-vertices flattened list. Memory efficient and fast for hyperedge-centric queries, but querying vertex degrees or incident hyperedge sets requires full-graph scans.






**Template parameters:**


* `LayoutTag` Specifies the memory layout orientation for the underlying data structure. 
* `IdType` The underlying integer type used for identifiers. 





#### See Also




* [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) for the data structure used for the underlying model representation. 





    
## Public Types Documentation




### typedef type 

_Self type alias._ 
```cpp
using hgl::repr::flat_list_t< LayoutTag, IdType >::type =  flat_list_t<LayoutTag, IdType>;
```




<hr>



### typedef layout\_tag 

_The configured layout tag._ 
```cpp
using hgl::repr::flat_list_t< LayoutTag, IdType >::layout_tag =  LayoutTag;
```




<hr>



### typedef id\_type 

_The configured identifier type._ 
```cpp
using hgl::repr::flat_list_t< LayoutTag, IdType >::id_type =  IdType;
```




<hr>



### typedef representation\_type 

_Type alias for the underlying flattened incidence list hypergraph representation based on the provided directional tag._ 
```cpp
template<traits::c_hypergraph_directional_tag DirectionalTag>
using hgl::repr::flat_list_t< LayoutTag, IdType >::representation_type =  impl::flat_incidence_list<DirectionalTag, type>;
```





**Template parameters:**


* `DirectionalTag` The directional tag (e.g., undirected or bf\_directed). 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/repr/repr_tags.hpp`

