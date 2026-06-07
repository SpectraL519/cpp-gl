

# Group HGL-Util



[**Modules**](modules.md) **>** [**HGL-Util**](group__HGL-Util.md)



_Practical, domain-agnostic C++ helpers, mathematical functions, and polyfills._ [More...](#detailed-description)














## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**hgl::util**](namespacehgl_1_1util.md) <br>_General utilities, ranges, and helpers for the HGL module (originating in the GL module)._  |




## Public Types

| Type | Name |
| ---: | :--- |
| typedef [**gl::util::concat\_fn**](structgl_1_1util_1_1concat__fn.md) | [**concat\_fn**](#typedef-concat_fn)  <br>_A function object for concatenating two viewable ranges into a_ `concat_view` _._ |
| typedef [**gl::util::concat\_view**](classgl_1_1util_1_1concat__view.md)&lt; V1, V2 &gt; | [**concat\_view**](#typedef-concat_view)  <br>_A view concatenating two ranges sequentially (C++20 polyfill for C++26_ `std::views::concat` _)._ |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr concat\_fn | [**concat**](#variable-concat)  = `{}`<br>_Concatenates two viewable ranges into a_ `concat_view` _._ |












































## Detailed Description


These utilities provide a curated set of general-purpose tools that support the library's internal algorithms and hypergraph modeling operations, completely independent of strict hypergraph theory semantics. 


    
## Public Types Documentation




### typedef concat\_fn 

_A function object for concatenating two viewable ranges into a_ `concat_view` _._
```cpp
using hgl::util::concat_fn = typedef gl::util::concat_fn;
```



#### See Also




* [**gl::util::concat\_fn**](structgl_1_1util_1_1concat__fn.md) : For the full type definition. 





        

<hr>



### typedef concat\_view 

_A view concatenating two ranges sequentially (C++20 polyfill for C++26_ `std::views::concat` _)._
```cpp
template<std::ranges::view V1, std::ranges::view V2>
using hgl::util::concat_view = typedef gl::util::concat_view<V1, V2>;
```



#### See Also




* [**gl::util::concat\_view**](classgl_1_1util_1_1concat__view.md) : For the full type definition. 





        

<hr>
## Public Attributes Documentation




### variable concat 

_Concatenates two viewable ranges into a_ `concat_view` _._
```cpp
constexpr concat_fn hgl::util::concat = {};
```



#### Example usage




```cpp
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = {4, 5, 6};
auto concatenated = gl::util::concat(v1, v2);
for (int x : concatenated)
    std::cout << x << " "; // Output: 1 2 3 4 5 6
```





**Parameters:**


* `r1` First range to concatenate. 
* `r2` Second range to concatenate. 



**Todo**

Replace with `std::views::concat` (C++26). 





#### See Also




* [**concat\_view**](group__HGL-Util.md#typedef-concat_view): The view type that represents the concatenation of two ranges.
* [**concat\_fn**](group__HGL-Util.md#typedef-concat_fn): A helper compile-time constant function object for creating `concat_view` instances. 





        

<hr>

------------------------------


