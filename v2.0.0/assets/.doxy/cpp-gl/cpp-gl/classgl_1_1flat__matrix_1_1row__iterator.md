

# Class gl::flat\_matrix::row\_iterator

**template &lt;[**bool**](group__GL-Core.md#variable-invalid_id_v) Const&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**flat\_matrix**](classgl_1_1flat__matrix.md) **>** [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md)



_Random access iterator over the rows of the_ `flat_matrix` _._[More...](#detailed-description)

* `#include <gl/types/flat_matrix.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef std::random\_access\_iterator\_tag | [**iterator\_concept**](#typedef-iterator_concept)  <br>_Satisfies random access iterator concept._  |
| typedef std::random\_access\_iterator\_tag | [**iterator\_category**](#typedef-iterator_category)  <br>_Legacy iterator category (random access)._  |
| typedef std::conditional\_t&lt; [**Const**](group__GL-Core.md#variable-invalid_id_v), [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type), [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) &gt; | [**value\_type**](#typedef-value_type)  <br>_Type of row this iterator dereferences to (subrange or const subrange)._  |
| typedef std::ptrdiff\_t | [**difference\_type**](#typedef-difference_type)  <br>_Signed integral difference type._  |
| typedef [**void**](group__GL-Core.md#variable-invalid_id_v) | [**pointer**](#typedef-pointer)  <br>_Pointer type (void because iterators dereference to spans)._  |
| typedef [**value\_type**](classgl_1_1flat__matrix_1_1row__iterator.md#typedef-value_type) | [**reference**](#typedef-reference)  <br>_Reference type (subrange of elements)._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**row\_iterator**](#function-row_iterator-12) () = default<br>_Default constructor creates a null iterator._  |
|   | [**row\_iterator**](#function-row_iterator-22) (data\_iter\_type data\_iter, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) n\_cols, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) row\_idx) noexcept<br>_Constructs an iterator pointing to a specific row._  |
|   | [**operator row\_iterator&lt; true &gt;**](#function-operator-row_iterator-true) () noexcept const<br>_Implicit conversion from mutable to const iterator._  |
|  [**reference**](classgl_1_1flat__matrix_1_1row__iterator.md#typedef-reference) | [**operator\***](#function-operator) () noexcept const<br>_Dereferences the iterator to the current row._  |
|  [**reference**](classgl_1_1flat__matrix_1_1row__iterator.md#typedef-reference) | [**operator[]**](#function-operator_1) ([**difference\_type**](classgl_1_1flat__matrix_1_1row__iterator.md#typedef-difference_type) n) noexcept const<br>_Random access to a row at an offset from the current position._  |
|  [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) & | [**operator++**](#function-operator_2) () noexcept<br>_Pre-increment operator._  |
|  [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) | [**operator++**](#function-operator_3) ([**int**](group__GL-Core.md#variable-invalid_id_v)) noexcept<br>_Post-increment operator._  |
|  [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) & | [**operator--**](#function-operator-) () noexcept<br>_Pre-decrement operator._  |
|  [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) | [**operator--**](#function-operator-_1) ([**int**](group__GL-Core.md#variable-invalid_id_v)) noexcept<br>_Post-decrement operator._  |
|  [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) & | [**operator+=**](#function-operator_4) ([**difference\_type**](classgl_1_1flat__matrix_1_1row__iterator.md#typedef-difference_type) n) noexcept<br>_Advances the iterator by n rows._  |
|  [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) & | [**operator-=**](#function-operator-) ([**difference\_type**](classgl_1_1flat__matrix_1_1row__iterator.md#typedef-difference_type) n) noexcept<br>_Moves the iterator backward by n rows._  |




























## Detailed Description


This iterator dereferences to a `row_type` (subrange of elements representing a single matrix row), allowing efficient iteration and random access. It calculates the memory offsets mathematically based on the column dimension.


#### Template Parameters




|Parameter|Type|Description|
|-----|-----|-----|
|Const|`bool`|If `true`, produces const iterators; if `false`, produces mutable iterators.|







> [!NOTE] Complexity
>
>
> Provides random access semantics: [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$ for all operations except construction. 
>
>

> [!CAUTION] Invalidation
>
>
> Invalidated when the `flat_matrix` structural dimensions are modified or memory is reallocated. 
>
>



**See also:** [**gl::flat\_matrix**](classgl_1_1flat__matrix.md) 




    
## Public Types Documentation




### typedef iterator\_concept 

_Satisfies random access iterator concept._ 
```cpp
using gl::flat_matrix< T >::row_iterator< Const >::iterator_concept =  std::random_access_iterator_tag;
```




<hr>



### typedef iterator\_category 

_Legacy iterator category (random access)._ 
```cpp
using gl::flat_matrix< T >::row_iterator< Const >::iterator_category =  std::random_access_iterator_tag;
```




<hr>



### typedef value\_type 

_Type of row this iterator dereferences to (subrange or const subrange)._ 
```cpp
using gl::flat_matrix< T >::row_iterator< Const >::value_type =  std::conditional_t<Const, const_row_type, row_type>;
```




<hr>



### typedef difference\_type 

_Signed integral difference type._ 
```cpp
using gl::flat_matrix< T >::row_iterator< Const >::difference_type =  std::ptrdiff_t;
```




<hr>



### typedef pointer 

_Pointer type (void because iterators dereference to spans)._ 
```cpp
using gl::flat_matrix< T >::row_iterator< Const >::pointer =  void;
```




<hr>



### typedef reference 

_Reference type (subrange of elements)._ 
```cpp
using gl::flat_matrix< T >::row_iterator< Const >::reference =  value_type;
```




<hr>
## Public Functions Documentation




### function row\_iterator [1/2]

_Default constructor creates a null iterator._ 
```cpp
gl::flat_matrix::row_iterator::row_iterator () = default
```




<hr>



### function row\_iterator [2/2]

_Constructs an iterator pointing to a specific row._ 
```cpp
inline gl::flat_matrix::row_iterator::row_iterator (
    data_iter_type data_iter,
    size_type n_cols,
    size_type row_idx
) noexcept
```





**Parameters:**


* `data_iter` Iterator to the underlying flat element data. 
* `n_cols` The number of columns in the matrix. 
* `row_idx` The index of the row this iterator currently points to. 




        

<hr>



### function operator row\_iterator&lt; true &gt; 

_Implicit conversion from mutable to const iterator._ 
```cpp
inline gl::flat_matrix::row_iterator::operator row_iterator< true > () noexcept const
```





**Returns:**

A const iterator pointing to the same row. 





        

<hr>



### function operator\* 

_Dereferences the iterator to the current row._ 
```cpp
inline reference gl::flat_matrix::row_iterator::operator* () noexcept const
```





**Returns:**

A subrange representing the row at the current position. 





        

<hr>



### function operator[] 

_Random access to a row at an offset from the current position._ 
```cpp
inline reference gl::flat_matrix::row_iterator::operator[] (
    difference_type n
) noexcept const
```





**Parameters:**


* `n` Offset (can be negative). 



**Returns:**

Row at offset n from the current position. 




**Precondition:**

`0 <= current_position + n <  n_rows()` 
> [!WARNING] The operation results in Undefined Behavior if the precondition is not satisfied. 
>
>







        

<hr>



### function operator++ 

_Pre-increment operator._ 
```cpp
inline row_iterator & gl::flat_matrix::row_iterator::operator++ () noexcept
```





**Returns:**

Reference to this iterator after advancing to the next row. 





        

<hr>



### function operator++ 

_Post-increment operator._ 
```cpp
inline row_iterator gl::flat_matrix::row_iterator::operator++ (
    int
) noexcept
```





**Returns:**

A copy of this iterator before the increment. 





        

<hr>



### function operator-- 

_Pre-decrement operator._ 
```cpp
inline row_iterator & gl::flat_matrix::row_iterator::operator-- () noexcept
```





**Returns:**

Reference to this iterator after moving to the previous row. 





        

<hr>



### function operator-- 

_Post-decrement operator._ 
```cpp
inline row_iterator gl::flat_matrix::row_iterator::operator-- (
    int
) noexcept
```





**Returns:**

A copy of this iterator before the decrement. 





        

<hr>



### function operator+= 

_Advances the iterator by n rows._ 
```cpp
inline row_iterator & gl::flat_matrix::row_iterator::operator+= (
    difference_type n
) noexcept
```





**Parameters:**


* `n` Number of rows to advance (can be negative). 



**Returns:**

Reference to this iterator. 





        

<hr>



### function operator-= 

_Moves the iterator backward by n rows._ 
```cpp
inline row_iterator & gl::flat_matrix::row_iterator::operator-= (
    difference_type n
) noexcept
```





**Parameters:**


* `n` Number of rows to move backward (can be negative). 



**Returns:**

Reference to this iterator. 





        

<hr>## Friends Documentation





### friend operator+ 

_Creates a new iterator advanced by n rows from the given iterator._ 
```cpp
inline row_iterator gl::flat_matrix::row_iterator::operator+ (
    row_iterator it,
    difference_type n
) noexcept
```





**Parameters:**


* `it` Iterator to advance from. 
* `n` Number of rows to advance. 



**Returns:**

New iterator at the advanced position. 





        

<hr>



### friend operator+ 

_Creates a new iterator advanced by n rows (commutative form)._ 
```cpp
inline row_iterator gl::flat_matrix::row_iterator::operator+ (
    difference_type n,
    row_iterator it
) noexcept
```





**Parameters:**


* `n` Number of rows to advance. 
* `it` Iterator to advance from. 



**Returns:**

New iterator at the advanced position. 





        

<hr>



### friend operator- 

_Creates a new iterator moved backward by n rows._ 
```cpp
inline row_iterator gl::flat_matrix::row_iterator::operator- (
    row_iterator it,
    difference_type n
) noexcept
```





**Parameters:**


* `it` Iterator to move backward from. 
* `n` Number of rows to move backward. 



**Returns:**

New iterator at the moved position. 





        

<hr>



### friend operator- 

_Computes the distance between two iterators._ 
```cpp
inline difference_type gl::flat_matrix::row_iterator::operator- (
    const  row_iterator & lhs,
    const  row_iterator & rhs
) noexcept
```





**Parameters:**


* `lhs` The later iterator. 
* `rhs` The earlier iterator. 



**Returns:**

Number of rows between the iterators; negative if lhs &lt; rhs. 





        

<hr>



### friend operator== 

_Tests equality of two iterators._ 
```cpp
inline bool gl::flat_matrix::row_iterator::operator== (
    const  row_iterator & lhs,
    const  row_iterator & rhs
) noexcept
```





**Parameters:**


* `lhs` Left iterator. 
* `rhs` Right iterator. 



**Returns:**

`true` if both iterators point to the same row index. 





        

<hr>



### friend operator&lt;=&gt; 

_Three-way comparison of two iterators._ 
```cpp
inline auto gl::flat_matrix::row_iterator::operator<=> (
    const  row_iterator & lhs,
    const  row_iterator & rhs
) noexcept
```





**Parameters:**


* `lhs` Left iterator. 
* `rhs` Right iterator. 



**Returns:**

Comparison result indicating iterator ordering. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/flat_matrix.hpp`

