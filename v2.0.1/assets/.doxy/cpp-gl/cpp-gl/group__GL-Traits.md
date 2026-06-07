

# Group GL-Traits



[**Modules**](modules.md) **>** [**GL-Traits**](group__GL-Traits.md)



_Type traits, template constraints, and compile-time metaprogramming utilities._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**gl::traits::is\_instantiation\_of**](structgl_1_1traits_1_1is__instantiation__of.md) &lt;T, Template&gt;<br>_Type trait to check if a type is an instantiation of a specific class template._  |
| struct | [**gl::traits::is\_instantiation\_of&lt; U&lt; Args... &gt;, U &gt;**](structgl_1_1traits_1_1is__instantiation__of_3_01U_3_01Args_8_8_8_01_4_00_01U_01_4.md) &lt;U, Args&gt;<br>_Specialization of the_ [_**is\_instantiation\_of**_](structgl_1_1traits_1_1is__instantiation__of.md) _trait for when the type is an instantiation of the provided class template._ |
| struct | [**gl::traits::swap\_repr\_tag**](structgl_1_1traits_1_1swap__repr__tag.md) &lt;GT, NewImplTag&gt;<br>_Utility trait type used to swap the representation tag of a graph traits or graph type._  |
| struct | [**gl::traits::swap\_repr\_tag&lt; graph&lt; graph\_traits&lt; Dir, VP, EP, OldImplTag, IdType &gt; &gt;, NewImplTag &gt;**](structgl_1_1traits_1_1swap__repr__tag_3_01graph_3_01graph__traits_3_01Dir_00_01VP_00_01EP_00_01O2a1c51ed02510394fe4450b6c0754649.md) &lt;Dir, VP, EP, OldImplTag, NewImplTag, IdType&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**graph**_](classgl_1_1graph.md) _class._ |
| struct | [**gl::traits::swap\_repr\_tag&lt; graph\_traits&lt; Dir, VP, EP, OldImplTag, IdType &gt;, NewImplTag &gt;**](structgl_1_1traits_1_1swap__repr__tag_3_01graph__traits_3_01Dir_00_01VP_00_01EP_00_01OldImplTag_9a28fe1b0e4026b5274857dfd47a2822.md) &lt;Dir, VP, EP, OldImplTag, NewImplTag, IdType&gt;<br>_Specialization of_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**graph\_traits**_](structgl_1_1graph__traits.md) _type._ |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef typename swap\_repr\_tag&lt; GT, NewImplTag &gt;::type | [**swap\_repr\_tag\_t**](#typedef-swap_repr_tag_t)  <br>_Alias template for easier usage of the_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _trait to resolve the swapped type directly._ |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr bool | [**is\_instantiation\_of\_v**](#variable-is_instantiation_of_v)  &lt;T, U&gt; = `is_instantiation_of<T, U>::value`<br>_Helper variable template for_ [_**is\_instantiation\_of**_](structgl_1_1traits_1_1is__instantiation__of.md) _trait._ |












































## Detailed Description


Encapsulating the `gl::traits` namespace, these components form the strict conceptual backbone of the library. They are used extensively to constrain template parameters, safely query type properties at compile time, and ensure that user-provided graphs and property maps meet required structural and behavioral contracts.


## Detailed Concept Specifications



For a detailed list of all GL module's concepts and their formal requirements, please refer to the [GL Concepts Documentation](gl_concepts.md#gl-concepts-documentation) page. 



    
## Public Types Documentation




### typedef swap\_repr\_tag\_t 

_Alias template for easier usage of the_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _trait to resolve the swapped type directly._
```cpp
template<typename GT, traits::c_graph_repr_tag NewImplTag>
using gl::traits::swap_repr_tag_t = typedef typename swap_repr_tag<GT, NewImplTag>::type;
```



#### See Also:




* [**to**](group__GL-Core.md#function-to) : For the function that utilizes this trait to perform graph conversions between different representations. 





        

<hr>
## Public Attributes Documentation




### variable is\_instantiation\_of\_v 

_Helper variable template for_ [_**is\_instantiation\_of**_](structgl_1_1traits_1_1is__instantiation__of.md) _trait._
```cpp
template<typename T, template< typename... > typename U>
constexpr bool gl::traits::is_instantiation_of_v = is_instantiation_of<T, U>::value;
```





**Template parameters:**


* `T` The type to check. 
* `U` The class template to match against. 




        

<hr>

------------------------------


