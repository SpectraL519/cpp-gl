

# File conversion.hpp



[**FileList**](files.md) **>** [**hgl**](dir_ccf941bb532a4e695b4e5490ff77f56c.md) **>** [**conversion.hpp**](hgl_2conversion_8hpp.md)



_Defines utilities for hypergraph representation model conversion and projection into standard graphs._ 

* `#include "gl/decl/repr_tags.hpp"`
* `#include "gl/graph.hpp"`
* `#include "gl/repr_tags.hpp"`
* `#include "gl/traits.hpp"`
* `#include "gl/types/core.hpp"`
* `#include <utility>`
* `#include "gl/attributes/force_inline.hpp"`
* `#include "gl/conversion.hpp"`
* `#include "hgl/directional_tags.hpp"`
* `#include "hgl/hypergraph.hpp"`
* `#include "hgl/types.hpp"`
* `#include <algorithm>`
* `#include <ranges>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**gl**](namespacegl.md) <br> |
| namespace | [**traits**](namespacegl_1_1traits.md) <br> |
| namespace | [**hgl**](namespacehgl.md) <br> |
| namespace | [**traits**](namespacehgl_1_1traits.md) <br>_Traits and concepts for the HGL module._  |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**swap\_repr\_tag&lt; graph&lt; graph\_traits&lt; Dir, VP, EP, OldImplTag, IdType &gt; &gt;, NewImplTag &gt;**](structgl_1_1traits_1_1swap__repr__tag_3_01graph_3_01graph__traits_3_01Dir_00_01VP_00_01EP_00_01O2a1c51ed02510394fe4450b6c0754649.md) &lt;Dir, VP, EP, OldImplTag, NewImplTag, IdType&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**graph**_](classgl_1_1graph.md) _class._ |
| struct | [**swap\_repr\_tag&lt; graph\_traits&lt; Dir, VP, EP, OldImplTag, IdType &gt;, NewImplTag &gt;**](structgl_1_1traits_1_1swap__repr__tag_3_01graph__traits_3_01Dir_00_01VP_00_01EP_00_01OldImplTag_9a28fe1b0e4026b5274857dfd47a2822.md) &lt;Dir, VP, EP, OldImplTag, NewImplTag, IdType&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**graph\_traits**_](structgl_1_1graph__traits.md) _type._ |
| struct | [**swap\_repr\_tag&lt; hypergraph&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt; &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph_3_01hypergraph__traits_3_01Dir_00_01VP_00_5680b44d408a91533b00c1a8cd4feb1c.md) &lt;Dir, VP, EP, OldReprTag, NewReprTag&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph**_](classhgl_1_1hypergraph.md) _type._ |
| struct | [**swap\_repr\_tag&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph__traits_3_01Dir_00_01VP_00_01EP_00_01OldReprTag_01_4_00_01NewReprTag_01_4.md) &lt;Dir, VP, EP, OldReprTag, NewReprTag&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph\_traits**_](structhgl_1_1hypergraph__traits.md) _type._ |



















































------------------------------
The documentation for this class was generated from the following file `include/hgl/conversion.hpp`

