

# Class gl::flat\_jagged\_vector::segment\_iterator

**template &lt;[**bool**](group__GL-Core.md#variable-invalid_id_v) Const&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) **>** [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md)



_Random access iterator over segments of the_ `flat_jagged_vector` _._[More...](#detailed-description)

* `#include <gl/types/flat_jagged_vector.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::random\_access\_iterator\_tag | [**iterator\_concept**](#typedef-iterator_concept)  <br>_Satisfies random access iterator concept._  |
| typedef std::random\_access\_iterator\_tag | [**iterator\_category**](#typedef-iterator_category)  <br>_Legacy iterator category (random access)._  |
| typedef std::conditional\_t&lt; [**Const**](group__GL-Core.md#variable-invalid_id_v), [**const\_segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-const_segment_type), [**segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-segment_type) &gt; | [**value\_type**](#typedef-value_type)  <br>_Type of segment this iterator dereferences to (subrange or const subrange)._  |
| typedef std::ptrdiff\_t | [**difference\_type**](#typedef-difference_type)  <br>_Signed integral difference type._  |
| typedef [**void**](group__GL-Core.md#variable-invalid_id_v) | [**pointer**](#typedef-pointer)  <br>_Pointer type (void because segment iterators dereference to subranges)._  |
| typedef [**value\_type**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md#typedef-value_type) | [**reference**](#typedef-reference)  <br>_Reference type (subrange of elements)._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**segment\_iterator**](#function-segment_iterator-12) () = default<br>_Default constructor creates a null iterator._  |
|   | [**segment\_iterator**](#function-segment_iterator-22) (data\_iter\_type data\_iter, offset\_ptr\_type offset\_ptr) noexcept<br>_Constructs an iterator pointing to a specific segment._  |
|   | [**operator segment\_iterator&lt; true &gt;**](#function-operator-segment_iterator-true) () noexcept const<br>_Implicit conversion from mutable to const iterator._  |
|  [**reference**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md#typedef-reference) | [**operator\***](#function-operator) () noexcept const<br>_Dereferences the iterator to the current segment._  |
|  [**reference**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md#typedef-reference) | [**operator[]**](#function-operator_1) ([**difference\_type**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md#typedef-difference_type) n) noexcept const<br>_Random access to a segment at offset from current position._  |
|  [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) & | [**operator++**](#function-operator_2) () noexcept<br>_Pre-increment operator._  |
|  [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) | [**operator++**](#function-operator_3) ([**int**](group__GL-Core.md#variable-invalid_id_v)) noexcept<br>_Post-increment operator._  |
|  [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) & | [**operator--**](#function-operator-) () noexcept<br>_Pre-decrement operator._  |
|  [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) | [**operator--**](#function-operator-_1) ([**int**](group__GL-Core.md#variable-invalid_id_v)) noexcept<br>_Post-decrement operator._  |
|  [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) & | [**operator+=**](#function-operator_4) ([**difference\_type**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md#typedef-difference_type) n) noexcept<br>_Advances the iterator by n positions._  |
|  [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) & | [**operator-=**](#function-operator-) ([**difference\_type**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md#typedef-difference_type) n) noexcept<br>_Moves the iterator backward by n positions._  |




























## Detailed Description


This iterator dereferences to a `segment_type` (subrange of elements in a single segment), allowing efficient iteration and random access to individual segments. The iterator maintains pointers to the element data and the offsets array for dereferencing.


#### Template Parameters




|Parameter|Type|Description|
|-----|-----|-----|
|Const|`bool`|If `true`, produces const iterators; if `false`, produces mutable iterators.|







> [!NOTE] Complexity
>
>
> Provides random access semantics: \(O(1)\) for all operations. 
>
>

> [!Caution] Invalidation
>
>
> Invalidated when the referenced [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) is modified (structure changes or element insertions/deletions). 
>
>



**See also:** [**gl::flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) 




    
## Public Types Documentation




### typedef iterator\_concept 

_Satisfies random access iterator concept._ 
```cpp
using gl::flat_jagged_vector< T >::segment_iterator< Const >::iterator_concept =  std::random_access_iterator_tag;
```




<hr>



### typedef iterator\_category 

_Legacy iterator category (random access)._ 
```cpp
using gl::flat_jagged_vector< T >::segment_iterator< Const >::iterator_category =  std::random_access_iterator_tag;
```




<hr>



### typedef value\_type 

_Type of segment this iterator dereferences to (subrange or const subrange)._ 
```cpp
using gl::flat_jagged_vector< T >::segment_iterator< Const >::value_type =  std::conditional_t<Const, const_segment_type, segment_type>;
```




<hr>



### typedef difference\_type 

_Signed integral difference type._ 
```cpp
using gl::flat_jagged_vector< T >::segment_iterator< Const >::difference_type =  std::ptrdiff_t;
```




<hr>



### typedef pointer 

_Pointer type (void because segment iterators dereference to subranges)._ 
```cpp
using gl::flat_jagged_vector< T >::segment_iterator< Const >::pointer =  void;
```




<hr>



### typedef reference 

_Reference type (subrange of elements)._ 
```cpp
using gl::flat_jagged_vector< T >::segment_iterator< Const >::reference =  value_type;
```




<hr>
## Public Functions Documentation




### function segment\_iterator [1/2]

_Default constructor creates a null iterator._ 
```cpp
gl::flat_jagged_vector::segment_iterator::segment_iterator () = default
```




<hr>



### function segment\_iterator [2/2]

_Constructs an iterator pointing to a specific segment._ 
```cpp
inline gl::flat_jagged_vector::segment_iterator::segment_iterator (
    data_iter_type data_iter,
    offset_ptr_type offset_ptr
) noexcept
```





**Parameters:**


* `data_iter` Iterator to the underlying element data (may be null for null iterator). 
* `offset_ptr` Pointer to the offsets array at the position of this segment. 




        

<hr>



### function operator segment\_iterator&lt; true &gt; 

_Implicit conversion from mutable to const iterator._ 
```cpp
inline gl::flat_jagged_vector::segment_iterator::operator segment_iterator< true > () noexcept const
```





**Returns:**

A const iterator pointing to the same segment. 





        

<hr>



### function operator\* 

_Dereferences the iterator to the current segment._ 
```cpp
inline reference gl::flat_jagged_vector::segment_iterator::operator* () noexcept const
```





**Returns:**

A subrange representing the segment at the current position. 





        

<hr>



### function operator[] 

_Random access to a segment at offset from current position._ 
```cpp
inline reference gl::flat_jagged_vector::segment_iterator::operator[] (
    difference_type n
) noexcept const
```





**Parameters:**


* `n` Offset (can be negative). 



**Returns:**

Segment at offset n from the current position. 




**Precondition:**

`0 <= current_position + n < container.size()` 
> [!WARNING] The operation results in Undefined Behavior if the precondition is not satisfied. 
>
>







        

<hr>



### function operator++ 

_Pre-increment operator._ 
```cpp
inline segment_iterator & gl::flat_jagged_vector::segment_iterator::operator++ () noexcept
```





**Returns:**

Reference to this iterator after advancing to the next segment. 





        

<hr>



### function operator++ 

_Post-increment operator._ 
```cpp
inline segment_iterator gl::flat_jagged_vector::segment_iterator::operator++ (
    int
) noexcept
```





**Returns:**

A copy of this iterator before the increment. 





        

<hr>



### function operator-- 

_Pre-decrement operator._ 
```cpp
inline segment_iterator & gl::flat_jagged_vector::segment_iterator::operator-- () noexcept
```





**Returns:**

Reference to this iterator after moving to the previous segment. 





        

<hr>



### function operator-- 

_Post-decrement operator._ 
```cpp
inline segment_iterator gl::flat_jagged_vector::segment_iterator::operator-- (
    int
) noexcept
```





**Returns:**

A copy of this iterator before the decrement. 





        

<hr>



### function operator+= 

_Advances the iterator by n positions._ 
```cpp
inline segment_iterator & gl::flat_jagged_vector::segment_iterator::operator+= (
    difference_type n
) noexcept
```





**Parameters:**


* `n` Number of segments to advance (can be negative). 



**Returns:**

Reference to this iterator. 





        

<hr>



### function operator-= 

_Moves the iterator backward by n positions._ 
```cpp
inline segment_iterator & gl::flat_jagged_vector::segment_iterator::operator-= (
    difference_type n
) noexcept
```





**Parameters:**


* `n` Number of segments to move backward (can be negative). 



**Returns:**

Reference to this iterator. 





        

<hr>## Friends Documentation





### friend operator+ 

_Creates a new iterator advanced by n positions from the given iterator._ 
```cpp
inline segment_iterator gl::flat_jagged_vector::segment_iterator::operator+ (
    segment_iterator it,
    difference_type n
) noexcept
```





**Parameters:**


* `it` Iterator to advance from. 
* `n` Number of segments to advance. 



**Returns:**

New iterator at the advanced position. 





        

<hr>



### friend operator+ 

_Creates a new iterator advanced by n positions (commutative form)._ 
```cpp
inline segment_iterator gl::flat_jagged_vector::segment_iterator::operator+ (
    difference_type n,
    segment_iterator it
) noexcept
```





**Parameters:**


* `n` Number of segments to advance. 
* `it` Iterator to advance from. 



**Returns:**

New iterator at the advanced position. 





        

<hr>



### friend operator- 

_Creates a new iterator moved backward by n positions._ 
```cpp
inline segment_iterator gl::flat_jagged_vector::segment_iterator::operator- (
    segment_iterator it,
    difference_type n
) noexcept
```





**Parameters:**


* `it` Iterator to move backward from. 
* `n` Number of segments to move backward. 



**Returns:**

New iterator at the moved position. 





        

<hr>



### friend operator- 

_Computes the distance between two iterators._ 
```cpp
inline difference_type gl::flat_jagged_vector::segment_iterator::operator- (
    const  segment_iterator & lhs,
    const  segment_iterator & rhs
) noexcept
```





**Parameters:**


* `lhs` The later iterator. 
* `rhs` The earlier iterator. 



**Returns:**

Number of segments between the iterators; negative if `lhs < rhs`. 





        

<hr>



### friend operator== 

_Tests equality of two iterators._ 
```cpp
inline bool gl::flat_jagged_vector::segment_iterator::operator== (
    const  segment_iterator & lhs,
    const  segment_iterator & rhs
) noexcept
```





**Parameters:**


* `lhs` Left iterator. 
* `rhs` Right iterator. 



**Returns:**

`true` if both iterators point to the same segment. 





        

<hr>



### friend operator&lt;=&gt; 

_Three-way comparison of two iterators._ 
```cpp
inline auto gl::flat_jagged_vector::segment_iterator::operator<=> (
    const  segment_iterator & lhs,
    const  segment_iterator & rhs
) noexcept
```





**Parameters:**


* `lhs` Left iterator. 
* `rhs` Right iterator. 



**Returns:**

Comparison result indicating iterator ordering. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/flat_jagged_vector.hpp`

