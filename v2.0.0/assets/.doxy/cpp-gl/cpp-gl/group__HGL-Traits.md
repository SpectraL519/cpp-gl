

# Group HGL-Traits



[**Modules**](modules.md) **>** [**HGL-Traits**](group__HGL-Traits.md)



_Type traits, template constraints, and compile-time metaprogramming utilities._ [More...](#detailed-description)














## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**hgl::traits**](namespacehgl_1_1traits.md) <br>_Traits and concepts for the HGL module._  |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**hgl::traits::swap\_repr\_tag**](structhgl_1_1traits_1_1swap__repr__tag.md) &lt;HT, NewReprTag&gt;<br>_Utility trait type used to swap the representation tag of a hypergraph traits or hypergraph type._  |
| struct | [**hgl::traits::swap\_repr\_tag&lt; hypergraph&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt; &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph_3_01hypergraph__traits_3_01Dir_00_01VP_00_5680b44d408a91533b00c1a8cd4feb1c.md) &lt;Dir, VP, EP, OldReprTag, NewReprTag&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph**_](classhgl_1_1hypergraph.md) _type._ |
| struct | [**hgl::traits::swap\_repr\_tag&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph__traits_3_01Dir_00_01VP_00_01EP_00_01OldReprTag_01_4_00_01NewReprTag_01_4.md) &lt;Dir, VP, EP, OldReprTag, NewReprTag&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph\_traits**_](structhgl_1_1hypergraph__traits.md) _type._ |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef typename LT::major\_element | [**major\_element\_t**](#typedef-major_element_t)  <br>_Extracts the major element tag type associated with a specific asymmetric layout._  |
| typedef typename LT::minor\_element | [**minor\_element\_t**](#typedef-minor_element_t)  <br>_Extracts the minor element tag type associated with a specific asymmetric layout._  |
| typedef typename swap\_repr\_tag&lt; HT, NewReprTag &gt;::type | [**swap\_repr\_tag\_t**](#typedef-swap_repr_tag_t)  <br>_Alias template for easier usage of the swap\_repr\_tag trait to resolve the swapped type directly._  |
















































## Detailed Description


These components form the strict conceptual backbone of the HGL module, used extensively to ensure internal type safety and correctness, while also constraining user-defined types to required structural contracts.


## Detailed Concept Specifications



For a detailed list of all HGL module's concepts and their formal requirements, please refer to the [HGL Concepts Documentation](hgl_concepts.md#hgl-concepts-documentation) page.



> [!NOTE]
>
>
> Because hypergraphs share the same underlying implementation design and mechanisms as standard graphs, they seamlessly reuse the same fundamental C++20 concepts. Therefore the HGL module pulls in all standard graph traits, concept checkers, and metaprogramming utilities from the GL module. To get an overview of these traits and concepts, please refer to the GL module's [**Traits & Concepts**](group__GL-Traits.md) documentation page. 
>
>




    
## Public Types Documentation




### typedef major\_element\_t 

_Extracts the major element tag type associated with a specific asymmetric layout._ 
```cpp
template<traits::c_hypergraph_asymmetric_layout_tag LT>
using hgl::major_element_t = typedef typename LT::major_element;
```





**Template parameters:**


* `LT` The asymmetric layout tag type. 




        

<hr>



### typedef minor\_element\_t 

_Extracts the minor element tag type associated with a specific asymmetric layout._ 
```cpp
template<traits::c_hypergraph_asymmetric_layout_tag LT>
using hgl::minor_element_t = typedef typename LT::minor_element;
```





**Template parameters:**


* `LT` The asymmetric layout tag type. 




        

<hr>



### typedef swap\_repr\_tag\_t 

_Alias template for easier usage of the swap\_repr\_tag trait to resolve the swapped type directly._ 
```cpp
template<typename HT, traits::c_hypergraph_repr_tag NewReprTag>
using hgl::traits::swap_repr_tag_t = typedef typename swap_repr_tag<HT, NewReprTag>::type;
```



#### See Also:




* [**to**](group__HGL-Core.md#function-to) : For the function that utilizes this trait to perform hypergraph conversions between different representations. 





        

<hr>

------------------------------


