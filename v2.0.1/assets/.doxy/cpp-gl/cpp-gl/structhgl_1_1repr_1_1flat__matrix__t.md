

# Struct hgl::repr::flat\_matrix\_t

**template &lt;traits::c\_hypergraph\_asymmetric\_layout\_tag LayoutTag, traits::c\_id\_type IdType&gt;**



[**ClassList**](annotated.md) **>** [**repr**](namespacehgl_1_1repr.md) **>** [**flat\_matrix\_t**](structhgl_1_1repr_1_1flat__matrix__t.md)



_Tag struct for the flattened incidence matrix hypergraph representation._ [More...](#detailed-description)

* `#include <hgl/repr/repr_tags.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**flat\_matrix\_t**](structhgl_1_1repr_1_1flat__matrix__t.md)&lt; [**LayoutTag**](group__HGL-Core.md#variable-invalid_id_v), [**IdType**](group__HGL-Core.md#variable-invalid_id_v) &gt; | [**type**](#typedef-type)  <br>_Self type alias._  |
| typedef [**LayoutTag**](group__HGL-Core.md#variable-invalid_id_v) | [**layout\_tag**](#typedef-layout_tag)  <br>_The configured asymmetric layout tag._  |
| typedef [**IdType**](group__HGL-Core.md#variable-invalid_id_v) | [**id\_type**](#typedef-id_type)  <br>_The configured identifier type._  |
| typedef impl::flat\_incidence\_matrix&lt; [**DirectionalTag**](group__HGL-Core.md#variable-invalid_id_v), [**type**](structhgl_1_1repr_1_1flat__matrix__t.md#typedef-type) &gt; | [**representation\_type**](#typedef-representation_type)  <br>_Type alias for the underlying flattened incidence matrix hypergraph representation based on the provided directional tag._  |
















































## Detailed Description


#### Layout Implications



Matrix implementations strictly require an asymmetric layout tag to define the row and column dimensions of the underlying matrix:



* [**hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md) (Default): Stores a \(\vert E \vert \times \vert V \vert\) flat matrix, where hyperedges are mapped to rows and vertices to columns. Retrieving the vertices incident to a specific hyperedge translates to a fast, cache-friendly contiguous memory read across a single row.
* [**vertex\_major\_t**](structhgl_1_1repr_1_1vertex__major__t.md): Stores a \(\vert V \vert \times \vert E \vert\) flat matrix, where vertices are mapped to rows and hyperedges to columns. Retrieving the hyperedges incident to a specific vertex translates to a fast, contiguous memory read.






**Template parameters:**


* `LayoutTag` Specifies the memory layout orientation for the underlying data structure (must be asymmetric). 
* `IdType` The underlying integer type used for identifiers. 





#### See Also




* [**flat\_matrix**](classgl_1_1flat__matrix.md) for the data structure used for the underlying model representation. 





    
## Public Types Documentation




### typedef type 

_Self type alias._ 
```cpp
using hgl::repr::flat_matrix_t< LayoutTag, IdType >::type =  flat_matrix_t<LayoutTag, IdType>;
```




<hr>



### typedef layout\_tag 

_The configured asymmetric layout tag._ 
```cpp
using hgl::repr::flat_matrix_t< LayoutTag, IdType >::layout_tag =  LayoutTag;
```




<hr>



### typedef id\_type 

_The configured identifier type._ 
```cpp
using hgl::repr::flat_matrix_t< LayoutTag, IdType >::id_type =  IdType;
```




<hr>



### typedef representation\_type 

_Type alias for the underlying flattened incidence matrix hypergraph representation based on the provided directional tag._ 
```cpp
template<traits::c_hypergraph_directional_tag DirectionalTag>
using hgl::repr::flat_matrix_t< LayoutTag, IdType >::representation_type =  impl::flat_incidence_matrix<DirectionalTag, type>;
```





**Template parameters:**


* `DirectionalTag` The directional tag (e.g., undirected or bf\_directed). 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/repr/repr_tags.hpp`

