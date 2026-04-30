

# Namespace hgl::traits



[**Namespace List**](namespaces.md) **>** [**traits**](namespacehgl_1_1traits.md)



_Traits and concepts for the HGL module._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**swap\_repr\_tag**](structhgl_1_1traits_1_1swap__repr__tag.md) &lt;HT, NewReprTag&gt;<br>_Utility trait type used to swap the representation tag of a hypergraph traits or hypergraph type._  |
| struct | [**swap\_repr\_tag&lt; hypergraph&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt; &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph_3_01hypergraph__traits_3_01Dir_00_01VP_00_5680b44d408a91533b00c1a8cd4feb1c.md) &lt;Dir, VP, EP, OldReprTag, NewReprTag&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph**_](classhgl_1_1hypergraph.md) _type._ |
| struct | [**swap\_repr\_tag&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph__traits_3_01Dir_00_01VP_00_01EP_00_01OldReprTag_01_4_00_01NewReprTag_01_4.md) &lt;Dir, VP, EP, OldReprTag, NewReprTag&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph\_traits**_](structhgl_1_1hypergraph__traits.md) _type._ |


















































## Detailed Description


This namespace contains the definitions of all hypergraph-specifc concepts and type traits used to constrain library types and pulls in all standard graph traits, concept checkers, and metaprogramming utilities from `gl::traits`. Because hypergraphs share the same underlying implementation design and mechanisms as standard graphs, they seamlessly reuse the same fundamental C++20 concepts.



> [!NOTE]
>
>
> To get a detailed overview of these shared utilities, please refer to the GL module's [**Traits & Concepts**](group__GL-Traits.md) documentation page. 
>
>



    

------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

