

# Class gl::util::concat\_view::iterator

**template &lt;[**bool**](group__GL-Core.md#variable-invalid_id_v) Const&gt;**



[**ClassList**](annotated.md) **>** [**iterator**](classgl_1_1util_1_1concat__view_1_1iterator.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::common\_type\_t&lt; std::ranges::range\_difference\_t&lt; BaseV1 &gt;, std::ranges::range\_difference\_t&lt; BaseV2 &gt; &gt; | [**difference\_type**](#typedef-difference_type)  <br> |
| typedef std::common\_type\_t&lt; std::ranges::range\_value\_t&lt; BaseV1 &gt;, std::ranges::range\_value\_t&lt; BaseV2 &gt; &gt; | [**value\_type**](#typedef-value_type)  <br> |
| typedef std::common\_reference\_t&lt; std::ranges::range\_reference\_t&lt; BaseV1 &gt;, std::ranges::range\_reference\_t&lt; BaseV2 &gt; &gt; | [**reference**](#typedef-reference)  <br> |
| typedef std::forward\_iterator\_tag | [**iterator\_category**](#typedef-iterator_category)  <br> |
| typedef std::forward\_iterator\_tag | [**iterator\_concept**](#typedef-iterator_concept)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**iterator**](#function-iterator-12) () = default<br> |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**iterator**](#function-iterator-22) (std::ranges::iterator\_t&lt; BaseV1 &gt; it1, std::ranges::sentinel\_t&lt; BaseV1 &gt; end1, std::ranges::iterator\_t&lt; BaseV2 &gt; it2) <br> |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) reference | [**operator\***](#function-operator) () const<br> |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) iterator & | [**operator++**](#function-operator_1) () <br> |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) iterator | [**operator++**](#function-operator_2) ([**int**](group__GL-Core.md#variable-invalid_id_v)) <br> |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_3) ([**const**](group__GL-Core.md#variable-invalid_id_v) iterator & other) const<br> |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_4) ([**const**](group__GL-Core.md#variable-invalid_id_v) sentinel&lt; [**Const**](group__GL-Core.md#variable-invalid_id_v) &gt; & s) const<br> |




























## Public Types Documentation




### typedef difference\_type 

```cpp
using gl::util::concat_view< V1, V2 >::iterator< Const >::difference_type =  std::common_type_t< std::ranges::range_difference_t<BaseV1>, std::ranges::range_difference_t<BaseV2> >;
```




<hr>



### typedef value\_type 

```cpp
using gl::util::concat_view< V1, V2 >::iterator< Const >::value_type =  std:: common_type_t<std::ranges::range_value_t<BaseV1>, std::ranges::range_value_t<BaseV2> >;
```




<hr>



### typedef reference 

```cpp
using gl::util::concat_view< V1, V2 >::iterator< Const >::reference =  std::common_reference_t< std::ranges::range_reference_t<BaseV1>, std::ranges::range_reference_t<BaseV2> >;
```




<hr>



### typedef iterator\_category 

```cpp
using gl::util::concat_view< V1, V2 >::iterator< Const >::iterator_category =  std::forward_iterator_tag;
```




<hr>



### typedef iterator\_concept 

```cpp
using gl::util::concat_view< V1, V2 >::iterator< Const >::iterator_concept =  std::forward_iterator_tag;
```




<hr>
## Public Functions Documentation




### function iterator [1/2]

```cpp
iterator::iterator () = default
```




<hr>



### function iterator [2/2]

```cpp
inline constexpr iterator::iterator (
    std::ranges::iterator_t< BaseV1 > it1,
    std::ranges::sentinel_t< BaseV1 > end1,
    std::ranges::iterator_t< BaseV2 > it2
) 
```




<hr>



### function operator\* 

```cpp
inline constexpr reference iterator::operator* () const
```




<hr>



### function operator++ 

```cpp
inline constexpr iterator & iterator::operator++ () 
```




<hr>



### function operator++ 

```cpp
inline constexpr iterator iterator::operator++ (
    int
) 
```




<hr>



### function operator== 

```cpp
inline constexpr  bool iterator::operator== (
    const iterator & other
) const
```




<hr>



### function operator== 

```cpp
inline constexpr  bool iterator::operator== (
    const sentinel< Const > & s
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/util/ranges.hpp`

