

# Class gl::flat\_matrix

**template &lt;std::semiregular T&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**flat\_matrix**](classgl_1_1flat__matrix.md)



_A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements._ [More...](#detailed-description)

* `#include <gl/types/flat_matrix.hpp>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) &lt;Const&gt;<br>_Random access iterator over the rows of the_ `flat_matrix` _._ |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef T | [**value\_type**](#typedef-value_type)  <br>_Type of elements stored in the matrix._  |
| typedef std::size\_t | [**size\_type**](#typedef-size_type)  <br>_Unsigned integral type used for sizes and indices._  |
| typedef std::vector&lt; [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) &gt; | [**container\_type**](#typedef-container_type)  <br>_The underlying contiguous storage container._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::reference | [**reference**](#typedef-reference)  <br>_Reference to an element._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::const\_reference | [**const\_reference**](#typedef-const_reference)  <br>_Const reference to an element._  |
| typedef std::ranges::subrange&lt; [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::iterator &gt; | [**row\_type**](#typedef-row_type)  <br>_Subrange type representing a non-owning uniform row of elements._  |
| typedef std::ranges::subrange&lt; [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::const\_iterator &gt; | [**const\_row\_type**](#typedef-const_row_type)  <br>_Const subrange type representing a non-owning uniform const row of elements._  |
| typedef [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md)&lt; [**false**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**iterator**](#typedef-iterator)  <br>_Mutable random access iterator over rows._  |
| typedef [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md)&lt; [**true**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**const\_iterator**](#typedef-const_iterator)  <br>_Const random access iterator over rows._  |
| typedef std::reverse\_iterator&lt; [**iterator**](classgl_1_1flat__matrix.md#typedef-iterator) &gt; | [**reverse\_iterator**](#typedef-reverse_iterator)  <br>_Reverse mutable iterator over rows._  |
| typedef std::reverse\_iterator&lt; [**const\_iterator**](classgl_1_1flat__matrix.md#typedef-const_iterator) &gt; | [**const\_reverse\_iterator**](#typedef-const_reverse_iterator)  <br>_Reverse const iterator over rows._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**flat\_matrix**](#function-flat_matrix-16) () = default<br>_Default constructor creates an empty_ `flat_matrix` _._ |
|   | [**flat\_matrix**](#function-flat_matrix-26) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**flat\_matrix**](classgl_1_1flat__matrix.md) &) = default<br>_Copy constructor creates a deep copy of another_ `flat_matrix` _._ |
|  [**flat\_matrix**](classgl_1_1flat__matrix.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**flat\_matrix**](classgl_1_1flat__matrix.md) &) = default<br>_Copy assignment creates a deep copy of another_ `flat_matrix` _._ |
|   | [**flat\_matrix**](#function-flat_matrix-36) ([**flat\_matrix**](classgl_1_1flat__matrix.md) && other) noexcept<br>_Move constructor transfers ownership of data from another_ `flat_matrix` _._ |
|  [**flat\_matrix**](classgl_1_1flat__matrix.md) & | [**operator=**](#function-operator_1) ([**flat\_matrix**](classgl_1_1flat__matrix.md) && other) noexcept<br>_Move assignment transfers ownership of data from another_ `flat_matrix` _._ |
|   | [**flat\_matrix**](#function-flat_matrix-46) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) n\_rows, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) n\_cols, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) & value=[**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type){}) <br>_Constructs a_ `flat_matrix` _with specified dimensions._ |
|   | [**flat\_matrix**](#function-flat_matrix-56) (std::initializer\_list&lt; std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) &gt; &gt; ilist) <br>_Constructs a_ `flat_matrix` _from an initializer list of rows._ |
|   | [**flat\_matrix**](#function-flat_matrix-66) (R && r) <br>_Constructs a_ `flat_matrix` _from a 2D range of ranges._ |
|   | [**~flat\_matrix**](#function-flat_matrix) () = default<br>_Destructor cleans up all managed memory._  |
|  [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) | [**size**](#function-size) () noexcept const<br>_Returns the number of rows in the matrix._  |
|  [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) | [**n\_rows**](#function-n_rows) () noexcept const<br>_Returns the number of rows in the matrix._  |
|  [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) | [**n\_cols**](#function-n_cols) () noexcept const<br>_Returns the number of columns in the matrix._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**empty**](#function-empty) () noexcept const<br>_Checks if the container is entirely empty._  |
|  [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) | [**data\_capacity**](#function-data_capacity) () noexcept const<br>_Returns the current capacity for data elements._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**reserve\_data**](#function-reserve_data) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) n) <br>_Reserves space for at least n total elements without changing the dimensions._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**shrink\_to\_fit**](#function-shrink_to_fit) () <br>_Reduces capacity of the internal array to match the current data size._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**resize**](#function-resize) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) new\_rows, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) new\_cols, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) & value=[**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type){}) <br>_Resizes the mathematical dimensions of the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**clear**](#function-clear) () <br>_Removes all dimensions and elements, leaving the matrix empty._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) | [**index**](#function-index) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) noexcept const<br>_Computes the underlying flattened 1D index for a 2D coordinate._  |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**operator[]**](#function-operator_2) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r) <br>_Returns the row at the given index without bounds checking._  |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**operator[]**](#function-operator_3) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r) const<br>_Returns a const row at the given index without bounds checking._  |
|  [**reference**](classgl_1_1flat__matrix.md#typedef-reference) | [**operator[]**](#function-operator_4) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) <br>_Returns a reference to an element without bounds checking._  |
|  [**const\_reference**](classgl_1_1flat__matrix.md#typedef-const_reference) | [**operator[]**](#function-operator_5) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) const<br>_Returns a const reference to an element without bounds checking._  |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**at**](#function-at-14) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r) <br>_Returns the row at the given index with bounds checking._  |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**at**](#function-at-24) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r) const<br>_Returns a const row at the given index with bounds checking._  |
|  [**reference**](classgl_1_1flat__matrix.md#typedef-reference) | [**at**](#function-at-34) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) <br>_Returns a reference to an element with bounds checking._  |
|  [**const\_reference**](classgl_1_1flat__matrix.md#typedef-const_reference) | [**at**](#function-at-44) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r, [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) const<br>_Returns a const reference to an element with bounds checking._  |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**front**](#function-front-12) () noexcept<br>_Returns the first row without bounds checking._  |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**front**](#function-front-22) () noexcept const<br>_Returns a const reference to the first row without bounds checking._  |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**back**](#function-back-12) () noexcept<br>_Returns the last row without bounds checking._  |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**back**](#function-back-22) () noexcept const<br>_Returns a const reference to the last row without bounds checking._  |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**front\_row**](#function-front_row-12) () noexcept<br>_Explicitly named alias for_ `front()` _yielding the first row._ |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**front\_row**](#function-front_row-22) () noexcept const<br>_Explicitly named alias for_ `front()` _yielding the first const row._ |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**back\_row**](#function-back_row-12) () noexcept<br>_Explicitly named alias for_ `back()` _yielding the last row._ |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**back\_row**](#function-back_row-22) () noexcept const<br>_Explicitly named alias for_ `back()` _yielding the last const row._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**front\_col**](#function-front_col-12) () noexcept<br>_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access view over the first column._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**front\_col**](#function-front_col-22) () noexcept const<br>_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access const view over the first column._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**back\_col**](#function-back_col-12) () noexcept<br>_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access view over the last column._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**back\_col**](#function-back_col-22) () noexcept const<br>_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access const view over the last column._ |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**row**](#function-row-12) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r) <br>_Semantically symmetric alias for_ `at(r)` _returning a bounds-checked row._ |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**row**](#function-row-22) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) r) const<br>_Semantically symmetric alias for_ `at(r)` _returning a bounds-checked const row._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**col**](#function-col-12) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) <br>_Returns a bounds-checked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access view over a specific column._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**col**](#function-col-22) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) c) const<br>_Returns a bounds-checked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access const view over a specific column._ |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**rows**](#function-rows-12) () noexcept<br>_Returns a view of all rows for iteration._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**rows**](#function-rows-22) () noexcept const<br>_Returns a const view of all rows for iteration._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**cols**](#function-cols-12) () noexcept<br>_Returns a view of all columns for iteration._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**cols**](#function-cols-22) () noexcept const<br>_Returns a const view of all columns for iteration._  |
|  [**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) | [**data\_size**](#function-data_size) () noexcept const<br>_Returns the total number of elements structurally stored in the matrix._  |
|  [**row\_type**](classgl_1_1flat__matrix.md#typedef-row_type) | [**data\_view**](#function-data_view-12) () noexcept<br>_Returns a subrange of all element data in flattened 1D form._  |
|  [**const\_row\_type**](classgl_1_1flat__matrix.md#typedef-const_row_type) | [**data\_view**](#function-data_view-22) () noexcept const<br>_Returns a const subrange of all element data in flattened 1D form._  |
|  [**container\_type**](classgl_1_1flat__matrix.md#typedef-container_type) & | [**data\_storage**](#function-data_storage-12) () noexcept<br>_Returns a reference to the underlying flat data container._  |
|  [**const**](group__GL-Core.md#variable-invalid_id_v) [**container\_type**](classgl_1_1flat__matrix.md#typedef-container_type) & | [**data\_storage**](#function-data_storage-22) () noexcept const<br>_Returns a const reference to the underlying flat data container._  |
|  [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) \* | [**data\_ptr**](#function-data_ptr-12) () noexcept<br>_Returns a raw pointer to the underlying flat data array._  |
|  [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) \* | [**data\_ptr**](#function-data_ptr-22) () noexcept const<br>_Returns a const raw pointer to the underlying flat data array._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_row**](#function-push_row-13) (R && r) <br>_Appends a range as a new row at the bottom of the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_row**](#function-push_row-23) (std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) &gt; ilist) <br>_Appends an initializer list as a new row at the bottom of the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_row**](#function-push_row-33) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) & value) <br>_Appends a newly created row filled with a specific value._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert\_row**](#function-insert_row-13) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos, R && r) <br>_Inserts a new row at the specified position from a range._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert\_row**](#function-insert_row-23) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos, std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) &gt; ilist) <br>_Inserts a new row at the specified position from an initializer list._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert\_row**](#function-insert_row-33) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) & value) <br>_Inserts a newly created row filled with a specific value at the specified position._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**pop\_row**](#function-pop_row) () <br>_Removes the last row from the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**erase\_row**](#function-erase_row) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos) <br>_Erases the row at the specified position._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_col**](#function-push_col-13) (R && r) <br>_Appends a range as a new column at the right edge of the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_col**](#function-push_col-23) (std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) &gt; ilist) <br>_Appends an initializer list as a new column at the right edge of the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_col**](#function-push_col-33) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) & value) <br>_Appends a newly created column filled with a specific value at the right edge._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert\_col**](#function-insert_col-13) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos, R && r) <br>_Inserts a new column at the specified position from a range._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert\_col**](#function-insert_col-23) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos, std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) &gt; ilist) <br>_Inserts a new column at the specified position from an initializer list._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert\_col**](#function-insert_col-33) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__matrix.md#typedef-value_type) & value) <br>_Inserts a newly created column filled with a specific value at the specified position._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**pop\_col**](#function-pop_col) () <br>_Removes the last column from the matrix._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**erase\_col**](#function-erase_col) ([**size\_type**](classgl_1_1flat__matrix.md#typedef-size_type) pos) <br>_Erases the column at the specified position._  |
|  [**iterator**](classgl_1_1flat__matrix.md#typedef-iterator) | [**begin**](#function-begin-12) () noexcept<br>_Returns a mutable iterator to the first row._  |
|  [**iterator**](classgl_1_1flat__matrix.md#typedef-iterator) | [**end**](#function-end-12) () noexcept<br>_Returns a mutable iterator past the last row (end sentinel)._  |
|  [**const\_iterator**](classgl_1_1flat__matrix.md#typedef-const_iterator) | [**begin**](#function-begin-22) () noexcept const<br>_Returns a const iterator to the first row._  |
|  [**const\_iterator**](classgl_1_1flat__matrix.md#typedef-const_iterator) | [**end**](#function-end-22) () noexcept const<br>_Returns a const iterator past the last row (end sentinel)._  |
|  [**const\_iterator**](classgl_1_1flat__matrix.md#typedef-const_iterator) | [**cbegin**](#function-cbegin) () noexcept const<br>_Returns a const iterator to the first row (explicit const form)._  |
|  [**const\_iterator**](classgl_1_1flat__matrix.md#typedef-const_iterator) | [**cend**](#function-cend) () noexcept const<br>_Returns a const iterator past the last row (explicit const form)._  |
|  [**reverse\_iterator**](classgl_1_1flat__matrix.md#typedef-reverse_iterator) | [**rbegin**](#function-rbegin-12) () noexcept<br>_Returns a reverse iterator to the last row._  |
|  [**reverse\_iterator**](classgl_1_1flat__matrix.md#typedef-reverse_iterator) | [**rend**](#function-rend-12) () noexcept<br>_Returns a reverse iterator before the first row (end sentinel)._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__matrix.md#typedef-const_reverse_iterator) | [**rbegin**](#function-rbegin-22) () noexcept const<br>_Returns a const reverse iterator to the last row._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__matrix.md#typedef-const_reverse_iterator) | [**rend**](#function-rend-22) () noexcept const<br>_Returns a const reverse iterator before the first row (end sentinel)._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__matrix.md#typedef-const_reverse_iterator) | [**crbegin**](#function-crbegin) () noexcept const<br>_Returns a const reverse iterator to the last row (explicit const form)._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__matrix.md#typedef-const_reverse_iterator) | [**crend**](#function-crend) () noexcept const<br>_Returns a const reverse iterator before the first row (explicit const form)._  |
|  [**flat\_matrix**](classgl_1_1flat__matrix.md) | [**transpose**](#function-transpose) () const<br>_Transposes the matrix mathematically (rows become columns, columns become rows)._  |




























## Detailed Description


This container stores all elements in a single contiguous memory block (_data_) of size `n_rows * n_cols` using row-major ordering. Row accesses are contiguous in memory, while column accesses are resolved mathematically via strided views. Both provide [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$ random access and native compatibility with C++20/23 ranges.


#### Example Usage




```cpp
#include <gl/types/flat_matrix.hpp>

#include <iostream>

int main() {
    gl::flat_matrix<int> mat = { // (1)!
        {1, 2, 3},
        {4, 5, 6}
    };

    mat[1uz, 2uz] = 99; // (2)!
    mat.push_row({7, 8, 9}); // (3)!
    mat.push_col({10, 11, 12}); // (4)!

    for (const auto row : mat) { // (5)!
        for (const auto element : row)
            std::cout << element << "\t";
        std::cout << '\n';
    }

    return 0;
}
```



1. Initialize the `flat_matrix` with an initializer list of rows, where each row is a list of elements.


2. Modify an element using 2D coordinates (row 1, col 2).


3. Append a new row to the bottom.


4. Append a new column to the right.


5. Iterate over the matrix.


**Output:** 
```text
1    2    3    10
4    5    99   11
7    8    9    12
```




> [!IMPORTANT] Iterator Invalidation Policy
>
>
> Iterator invalidation follows `std::vector` semantics: modifying the dimensions or structural capacity of the matrix invalidates all iterators, pointers, and references to its elements. 
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|T|The type of the elements|Must be **semiregular** (copy-constructible and assignable)|







#### TODO




* Implement the row\_unchecked and col\_unchecked methods. 





    
## Public Types Documentation




### typedef value\_type 

_Type of elements stored in the matrix._ 
```cpp
using gl::flat_matrix< T >::value_type =  T;
```




<hr>



### typedef size\_type 

_Unsigned integral type used for sizes and indices._ 
```cpp
using gl::flat_matrix< T >::size_type =  std::size_t;
```




<hr>



### typedef container\_type 

_The underlying contiguous storage container._ 
```cpp
using gl::flat_matrix< T >::container_type =  std::vector<value_type>;
```




<hr>



### typedef reference 

_Reference to an element._ 
```cpp
using gl::flat_matrix< T >::reference =  typename container_type::reference;
```




<hr>



### typedef const\_reference 

_Const reference to an element._ 
```cpp
using gl::flat_matrix< T >::const_reference =  typename container_type::const_reference;
```




<hr>



### typedef row\_type 

_Subrange type representing a non-owning uniform row of elements._ 
```cpp
using gl::flat_matrix< T >::row_type =  std::ranges::subrange<typename container_type::iterator>;
```




<hr>



### typedef const\_row\_type 

_Const subrange type representing a non-owning uniform const row of elements._ 
```cpp
using gl::flat_matrix< T >::const_row_type =  std::ranges::subrange<typename container_type::const_iterator>;
```




<hr>



### typedef iterator 

_Mutable random access iterator over rows._ 
```cpp
using gl::flat_matrix< T >::iterator =  row_iterator<false>;
```




<hr>



### typedef const\_iterator 

_Const random access iterator over rows._ 
```cpp
using gl::flat_matrix< T >::const_iterator =  row_iterator<true>;
```




<hr>



### typedef reverse\_iterator 

_Reverse mutable iterator over rows._ 
```cpp
using gl::flat_matrix< T >::reverse_iterator =  std::reverse_iterator<iterator>;
```




<hr>



### typedef const\_reverse\_iterator 

_Reverse const iterator over rows._ 
```cpp
using gl::flat_matrix< T >::const_reverse_iterator =  std::reverse_iterator<const_iterator>;
```




<hr>
## Public Functions Documentation




### function flat\_matrix [1/6]

_Default constructor creates an empty_ `flat_matrix` _._
```cpp
gl::flat_matrix::flat_matrix () = default
```





**Postcondition:**

`empty() == true`, `n_rows() == 0`, `n_cols() == 0`, `data_size() == 0`. 





        

<hr>



### function flat\_matrix [2/6]

_Copy constructor creates a deep copy of another_ `flat_matrix` _._
```cpp
gl::flat_matrix::flat_matrix (
    const  flat_matrix &
) = default
```





**Parameters:**


* `other` The `flat_matrix` to copy. 



**Postcondition:**

`*this == other` 





        

<hr>



### function operator= 

_Copy assignment creates a deep copy of another_ `flat_matrix` _._
```cpp
flat_matrix & gl::flat_matrix::operator= (
    const  flat_matrix &
) = default
```





**Parameters:**


* `other` The source `flat_matrix`. 



**Returns:**

Reference to `*this`. 




**Postcondition:**

`*this == other` 





        

<hr>



### function flat\_matrix [3/6]

_Move constructor transfers ownership of data from another_ `flat_matrix` _._
```cpp
inline gl::flat_matrix::flat_matrix (
    flat_matrix && other
) noexcept
```





**Parameters:**


* `other` The source `flat_matrix` (left in an empty state). 




#### Postconditions:




1. `other.empty() == true`
2. All data is transferred to `*this`





> [!WARNING] Invalidates all iterators, pointers, and references to other instance's elements. 
>
>




        

<hr>



### function operator= 

_Move assignment transfers ownership of data from another_ `flat_matrix` _._
```cpp
inline flat_matrix & gl::flat_matrix::operator= (
    flat_matrix && other
) noexcept
```





**Parameters:**


* `other` The source `flat_matrix`. 



**Returns:**

Reference to `*this`. 





#### Postconditions:




1. `other.empty() == true`
2. All data from `other` is transferred to `*this`





> [!INFO] Safety
>
>
> This operator safely handles self-assignment. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to this container's elements. 
>
>




        

<hr>



### function flat\_matrix [4/6]

_Constructs a_ `flat_matrix` _with specified dimensions._
```cpp
inline gl::flat_matrix::flat_matrix (
    size_type n_rows,
    size_type n_cols,
    const  value_type & value=value_type {}
) 
```





**Parameters:**


* `n_rows` The number of rows. 
* `n_cols` The number of columns. 
* `value` The value to initialize all elements with (default constructed if omitted). 




#### Postconditions




1. `n_rows() == n_rows`
2. `n_cols() == n_cols`
3. All elements equal to `value` 

**Exception:**


    * `std::bad_alloc` May throw if memory allocation fails. 







        

<hr>



### function flat\_matrix [5/6]

_Constructs a_ `flat_matrix` _from an initializer list of rows._
```cpp
inline gl::flat_matrix::flat_matrix (
    std::initializer_list< std::initializer_list< value_type > > ilist
) 
```





**Parameters:**


* `ilist` Initializer list of initializer lists, each representing a row. 



**Postcondition:**

Dimensions are established based on the list geometry. 




**Exception:**


* `std::invalid_argument` If the rows in the list do not have identical lengths. 
* `std::bad_alloc` May throw if memory allocation fails.


> [!WARNING] Invalidates all iterators, pointers, and references after construction. 
>
>



        

<hr>



### function flat\_matrix [6/6]

_Constructs a_ `flat_matrix` _from a 2D range of ranges._
```cpp
template<std::ranges::input_range R>
inline explicit gl::flat_matrix::flat_matrix (
    R && r
) 
```



The matrix establishes its column count from the size of the first extracted row. All subsequent rows must perfectly match this dimension.




**Template parameters:**


* `R` A range type whose elements are input ranges of `value_type`. 



**Parameters:**


* `r` The 2D range to initialize from. 



**Postcondition:**

Dimensions match the structure of `r`. 




**Exception:**


* `std::invalid_argument` If any extracted row size mismatches the first row's size. 
* `std::bad_alloc` May throw if memory allocation fails.


> [!NOTE] Exception safety
>
>
> Provides strong exception guarantee if the source is an unsized pure `input_range` and fails validation mid-extraction. 
>
>



        

<hr>



### function ~flat\_matrix 

_Destructor cleans up all managed memory._ 
```cpp
gl::flat_matrix::~flat_matrix () = default
```




<hr>



### function size 

_Returns the number of rows in the matrix._ 
```cpp
inline size_type gl::flat_matrix::size () noexcept const
```





**Returns:**

The count of rows.



> [!NOTE] Implementation
>
>
> Equivalent to `n_rows()`. Required to idiomaticaly satisfy `std::ranges::sized_range`. 
>
>



        

<hr>



### function n\_rows 

_Returns the number of rows in the matrix._ 
```cpp
inline size_type gl::flat_matrix::n_rows () noexcept const
```





**Returns:**

The count of rows. 





        

<hr>



### function n\_cols 

_Returns the number of columns in the matrix._ 
```cpp
inline size_type gl::flat_matrix::n_cols () noexcept const
```





**Returns:**

The count of columns. 





        

<hr>



### function empty 

_Checks if the container is entirely empty._ 
```cpp
inline bool gl::flat_matrix::empty () noexcept const
```





**Returns:**

`true` if `data_size() == 0`, `false` otherwise. 





        

<hr>



### function data\_capacity 

_Returns the current capacity for data elements._ 
```cpp
inline size_type gl::flat_matrix::data_capacity () noexcept const
```





**Returns:**

The number of total elements that can be stored in _data_ without reallocation. 





        

<hr>



### function reserve\_data 

_Reserves space for at least n total elements without changing the dimensions._ 
```cpp
inline void gl::flat_matrix::reserve_data (
    size_type n
) 
```





**Parameters:**


* `n` The total number of matrix elements to reserve space for. 



**Postcondition:**

`data_capacity() >= n`



> [!WARNING] Invalidates all iterators and pointers to elements if reallocation occurs. 
>
>



        

<hr>



### function shrink\_to\_fit 

_Reduces capacity of the internal array to match the current data size._ 
```cpp
inline void gl::flat_matrix::shrink_to_fit () 
```





**Postcondition:**

`data_capacity() == data_size()`



> [!WARNING] Invalidates all iterators, pointers, and references to elements if reallocation occures. 
>
>



        

<hr>



### function resize 

_Resizes the mathematical dimensions of the matrix._ 
```cpp
inline void gl::flat_matrix::resize (
    size_type new_rows,
    size_type new_cols,
    const  value_type & value=value_type {}
) 
```



If the new dimensions require structural changes (e.g. changing the number of columns), the mathematical grid is rebuilt and existing items are relocated to their new coordinate slots.




**Parameters:**


* `new_rows` The new number of rows. 
* `new_cols` The new number of columns. 
* `value` The value to initialize any newly exposed slots with. 



**Postcondition:**

`n_rows() == new_rows` and `n_cols() == new_cols`.



> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are the new dimensions, due to remapping elements in 2D space. If only the row count changes, it is [**$O(E)**](group__GL-Core.md#variable-invalid_id_v)$ where $E$ is the number of inserted or removed trailing elements. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references. 
>
>



        

<hr>



### function clear 

_Removes all dimensions and elements, leaving the matrix empty._ 
```cpp
inline void gl::flat_matrix::clear () 
```





**Postcondition:**

`n_rows() == 0`, `n_cols() == 0`, `data_size() == 0`



> [!WARNING] Invalidates all iterators, pointers, and references to elements. 
>
>



        

<hr>



### function index 

_Computes the underlying flattened 1D index for a 2D coordinate._ 
```cpp
inline constexpr  size_type gl::flat_matrix::index (
    size_type r,
    size_type c
) noexcept const
```





**Parameters:**


* `r` The row index. 
* `c` The column index. 



**Returns:**

The 1D index mapping for _data_.



> [!INFO] Provides [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$ constant time lookup calculation. 
>
>



        

<hr>



### function operator[] 

_Returns the row at the given index without bounds checking._ 
```cpp
inline row_type gl::flat_matrix::operator[] (
    size_type r
) 
```





**Parameters:**


* `r` The index of the row to access. 



**Returns:**

A subrange representing the row at index r. 




**Precondition:**

`r <  n_rows()`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(r)` for bounds-checked access. Calling on an out-of-bounds index results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a const row at the given index without bounds checking._ 
```cpp
inline const_row_type gl::flat_matrix::operator[] (
    size_type r
) const
```





**Parameters:**


* `r` The index of the row to access. 



**Returns:**

A const subrange representing the row at index r. 




**Precondition:**

`r <  n_rows()`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(r)` for bounds-checked access. Calling on an out-of-bounds index results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a reference to an element without bounds checking._ 
```cpp
inline reference gl::flat_matrix::operator[] (
    size_type r,
    size_type c
) 
```





**Parameters:**


* `r` The row index. 
* `c` The column index. 



**Returns:**

Reference to the element at the given coordinates. 




**Precondition:**

`r <  n_rows()` and `c <  n_cols()`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(r, c)` for bounds-checked access. Out-of-bounds access results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a const reference to an element without bounds checking._ 
```cpp
inline const_reference gl::flat_matrix::operator[] (
    size_type r,
    size_type c
) const
```





**Parameters:**


* `r` The row index. 
* `c` The column index. 



**Returns:**

Const reference to the element at the given coordinates. 




**Precondition:**

`r <  n_rows()` and `c <  n_cols()`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(r, c)` for bounds-checked access. Out-of-bounds access results in Undefined Behavior. 
>
>



        

<hr>



### function at [1/4]

_Returns the row at the given index with bounds checking._ 
```cpp
inline row_type gl::flat_matrix::at (
    size_type r
) 
```





**Parameters:**


* `r` The index of the row. 



**Returns:**

A subrange representing the row at index r. 




**Exception:**


* `std::out_of_range` If `r >=  n_rows()`. 




        

<hr>



### function at [2/4]

_Returns a const row at the given index with bounds checking._ 
```cpp
inline const_row_type gl::flat_matrix::at (
    size_type r
) const
```





**Parameters:**


* `r` The index of the row. 



**Returns:**

A const subrange representing the row at index r. 




**Exception:**


* `std::out_of_range` If `r >=  n_rows()`. 




        

<hr>



### function at [3/4]

_Returns a reference to an element with bounds checking._ 
```cpp
inline reference gl::flat_matrix::at (
    size_type r,
    size_type c
) 
```





**Parameters:**


* `r` The row index. 
* `c` The column index. 



**Returns:**

Reference to the element. 




**Exception:**


* `std::out_of_range` If `r >=  n_rows()` or `c >=  n_cols()`. 




        

<hr>



### function at [4/4]

_Returns a const reference to an element with bounds checking._ 
```cpp
inline const_reference gl::flat_matrix::at (
    size_type r,
    size_type c
) const
```





**Parameters:**


* `r` The row index. 
* `c` The column index. 



**Returns:**

Const reference to the element. 




**Exception:**


* `std::out_of_range` If `r >=  n_rows()` or `c >=  n_cols()`. 




        

<hr>



### function front [1/2]

_Returns the first row without bounds checking._ 
```cpp
inline row_type gl::flat_matrix::front () noexcept
```





**Returns:**

A subrange representing the first row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function front [2/2]

_Returns a const reference to the first row without bounds checking._ 
```cpp
inline const_row_type gl::flat_matrix::front () noexcept const
```





**Returns:**

A const subrange representing the first row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function back [1/2]

_Returns the last row without bounds checking._ 
```cpp
inline row_type gl::flat_matrix::back () noexcept
```





**Returns:**

A subrange representing the last row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function back [2/2]

_Returns a const reference to the last row without bounds checking._ 
```cpp
inline const_row_type gl::flat_matrix::back () noexcept const
```





**Returns:**

A const subrange representing the last row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function front\_row [1/2]

_Explicitly named alias for_ `front()` _yielding the first row._
```cpp
inline row_type gl::flat_matrix::front_row () noexcept
```





**Returns:**

A subrange representing the first row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function front\_row [2/2]

_Explicitly named alias for_ `front()` _yielding the first const row._
```cpp
inline const_row_type gl::flat_matrix::front_row () noexcept const
```





**Returns:**

A const subrange representing the first row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function back\_row [1/2]

_Explicitly named alias for_ `back()` _yielding the last row._
```cpp
inline row_type gl::flat_matrix::back_row () noexcept
```





**Returns:**

A subrange representing the last row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function back\_row [2/2]

_Explicitly named alias for_ `back()` _yielding the last const row._
```cpp
inline const_row_type gl::flat_matrix::back_row () noexcept const
```





**Returns:**

A const subrange representing the last row. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function front\_col [1/2]

_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access view over the first column._
```cpp
inline auto gl::flat_matrix::front_col () noexcept
```





**Returns:**

A strided view representing the first column. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function front\_col [2/2]

_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access const view over the first column._
```cpp
inline auto gl::flat_matrix::front_col () noexcept const
```





**Returns:**

A strided view representing the const first column. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function back\_col [1/2]

_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access view over the last column._
```cpp
inline auto gl::flat_matrix::back_col () noexcept
```





**Returns:**

A strided view representing the last column. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function back\_col [2/2]

_Returns an unchecked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access const view over the last column._
```cpp
inline auto gl::flat_matrix::back_col () noexcept const
```





**Returns:**

A strided view representing the const last column. 




**Precondition:**

Matrix must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if matrix is empty. 
>
>







        

<hr>



### function row [1/2]

_Semantically symmetric alias for_ `at(r)` _returning a bounds-checked row._
```cpp
inline row_type gl::flat_matrix::row (
    size_type r
) 
```





**Parameters:**


* `r` The row index. 



**Returns:**

A subrange representing the row. 




**Exception:**


* `std::out_of_range` If `r >=  n_rows()`. 




        

<hr>



### function row [2/2]

_Semantically symmetric alias for_ `at(r)` _returning a bounds-checked const row._
```cpp
inline const_row_type gl::flat_matrix::row (
    size_type r
) const
```





**Parameters:**


* `r` The row index. 



**Returns:**

A const subrange representing the row. 




**Exception:**


* `std::out_of_range` If `r >=  n_rows()`. 




        

<hr>



### function col [1/2]

_Returns a bounds-checked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access view over a specific column._
```cpp
inline auto gl::flat_matrix::col (
    size_type c
) 
```





**Parameters:**


* `c` The column index. 



**Returns:**

A strided view representing the column. 




**Exception:**


* `std::out_of_range` If `c >=  n_cols()`. 




        

<hr>



### function col [2/2]

_Returns a bounds-checked_ [_**$O(1)**_](group__GL-Core.md#variable-invalid_id_v) _$ random-access const view over a specific column._
```cpp
inline auto gl::flat_matrix::col (
    size_type c
) const
```





**Parameters:**


* `c` The column index. 



**Returns:**

A strided view representing the const column. 




**Exception:**


* `std::out_of_range` If `c >=  n_cols()`. 




        

<hr>



### function rows [1/2]

_Returns a view of all rows for iteration._ 
```cpp
inline auto gl::flat_matrix::rows () noexcept
```





**Returns:**

A random-access view of all row spans. 





        

<hr>



### function rows [2/2]

_Returns a const view of all rows for iteration._ 
```cpp
inline auto gl::flat_matrix::rows () noexcept const
```





**Returns:**

A random-access const view of all row const spans. 





        

<hr>



### function cols [1/2]

_Returns a view of all columns for iteration._ 
```cpp
inline auto gl::flat_matrix::cols () noexcept
```





**Returns:**

A random-access view of all column strided-views. 





        

<hr>



### function cols [2/2]

_Returns a const view of all columns for iteration._ 
```cpp
inline auto gl::flat_matrix::cols () noexcept const
```





**Returns:**

A random-access view of all const column strided-views. 





        

<hr>



### function data\_size 

_Returns the total number of elements structurally stored in the matrix._ 
```cpp
inline size_type gl::flat_matrix::data_size () noexcept const
```





**Returns:**

The result of `n_rows() * n_cols()`. 





        

<hr>



### function data\_view [1/2]

_Returns a subrange of all element data in flattened 1D form._ 
```cpp
inline row_type gl::flat_matrix::data_view () noexcept
```





**Returns:**

A subrange of all elements in the underlying _data_ array. 
> [!NOTE] Allows direct access to the flattened data representation. 
>
>







        

<hr>



### function data\_view [2/2]

_Returns a const subrange of all element data in flattened 1D form._ 
```cpp
inline const_row_type gl::flat_matrix::data_view () noexcept const
```





**Returns:**

A const subrange of all elements in the underlying _data_ array. 
> [!NOTE] Allows direct access to the flattened data representation. 
>
>







        

<hr>



### function data\_storage [1/2]

_Returns a reference to the underlying flat data container._ 
```cpp
inline container_type & gl::flat_matrix::data_storage () noexcept
```





**Returns:**

A mutable reference to the underlying _data_ array.



> [!CAUTION] Use with extreme caution
>
>
> Modifying this vector directly can fatally corrupt the matrix structure. Use for advanced operations only. 
>
>



        

<hr>



### function data\_storage [2/2]

_Returns a const reference to the underlying flat data container._ 
```cpp
inline const  container_type & gl::flat_matrix::data_storage () noexcept const
```





**Returns:**

A const reference to the underlying _data_ array. 





        

<hr>



### function data\_ptr [1/2]

_Returns a raw pointer to the underlying flat data array._ 
```cpp
inline value_type * gl::flat_matrix::data_ptr () noexcept
```





**Returns:**

A raw pointer to the first element in the _data_ array. 
> [!IMPORTANT] Not available for boolean matrices. 
>
>







        

<hr>



### function data\_ptr [2/2]

_Returns a const raw pointer to the underlying flat data array._ 
```cpp
inline const  value_type * gl::flat_matrix::data_ptr () noexcept const
```





**Returns:**

A const raw pointer to the first element in the _data_ array. 
> [!IMPORTANT] Not available for boolean matrices. 
>
>







        

<hr>



### function push\_row [1/3]

_Appends a range as a new row at the bottom of the matrix._ 
```cpp
template<std::ranges::input_range R>
inline void gl::flat_matrix::push_row (
    R && r
) 
```





**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `r` The range to append. 



**Postcondition:**

`n_rows()` increases by 1. 




**Exception:**


* `std::invalid_argument` If the row size does not match `n_cols()` (for non-empty matrices). 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> Amortized [**$O(C)**](group__GL-Core.md#variable-invalid_id_v)$ where $C$ is the number of columns. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references if reallocation occurs. 
>
>



        

<hr>



### function push\_row [2/3]

_Appends an initializer list as a new row at the bottom of the matrix._ 
```cpp
inline void gl::flat_matrix::push_row (
    std::initializer_list< value_type > ilist
) 
```





**Parameters:**


* `ilist` The list to append. 



**Postcondition:**

`n_rows()` increases by 1. 




**Exception:**


* `std::invalid_argument` If the list size does not match `n_cols()`. 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> Amortized [**$O(C)**](group__GL-Core.md#variable-invalid_id_v)$ where $C$ is the number of columns. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references if reallocation occurs. 
>
>



        

<hr>



### function push\_row [3/3]

_Appends a newly created row filled with a specific value._ 
```cpp
inline void gl::flat_matrix::push_row (
    const  value_type & value
) 
```



If the matrix is empty (has no columns), this operation has no effect, as the size of the new row will be determined as 0.




**Parameters:**


* `value` The value to fill the new row with. 



**Postcondition:**

`n_rows()` increases by 1. 




**Exception:**


* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> Amortized [**$O(C)**](group__GL-Core.md#variable-invalid_id_v)$ where $C$ is the number of columns. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references if reallocation occurs. 
>
>



        

<hr>



### function insert\_row [1/3]

_Inserts a new row at the specified position from a range._ 
```cpp
template<std::ranges::input_range R>
inline void gl::flat_matrix::insert_row (
    size_type pos,
    R && r
) 
```





**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `pos` The row position where the elements will be inserted. 
* `r` The range to insert. 




#### Postconditions




1. `n_rows()` increases by 1.
2. Rows at and after `pos` are shifted down. 

**Exception:**


    * `std::out_of_range` If `pos >  n_rows()`. 
    * `std::invalid_argument` If the range size does not match `n_cols()`. 
    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > $O(E + C)$ where $E$ is the number of total elements from `pos` onward and $C$ is the size of the inserted row. 
    >
    >

    > [!NOTE] Exception safety
    >
    >
    > Provides strong exception guarantee if size validation fails for unsized ranges. 
    >
    >

    > [!WARNING] Invalidates all iterators, pointers, and references after the insertion point. 
    >
    >






        

<hr>



### function insert\_row [2/3]

_Inserts a new row at the specified position from an initializer list._ 
```cpp
inline void gl::flat_matrix::insert_row (
    size_type pos,
    std::initializer_list< value_type > ilist
) 
```





**Parameters:**


* `pos` The row position where the elements will be inserted. 
* `ilist` The list to insert. 




#### Postconditions




1. `n_rows()` increases by 1.
2. Rows at and after `pos` are shifted down. 

**Exception:**


    * `std::out_of_range` If `pos >  n_rows()`. 
    * `std::invalid_argument` If the list size does not match `n_cols()`. 
    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > $O(E + C)$ where $E$ is the number of total elements from `pos` onward and $C$ is the size of the inserted row. 
    >
    >

    > [!WARNING] Invalidates all iterators, pointers, and references after the insertion point. 
    >
    >






        

<hr>



### function insert\_row [3/3]

_Inserts a newly created row filled with a specific value at the specified position._ 
```cpp
inline void gl::flat_matrix::insert_row (
    size_type pos,
    const  value_type & value
) 
```



If the matrix is empty (has no columns), this operation has no effect, as the size of the new row will be determined as 0.




**Parameters:**


* `pos` The row position where the elements will be inserted. 
* `value` The value to fill the new row with. 




#### Postconditions




1. `n_rows()` increases by 1.
2. Rows at and after `pos` are shifted down. 

**Exception:**


    * `std::out_of_range` If `pos >  n_rows()`. 
    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > $O(E + C)$ where $E$ is the number of total elements from `pos` onward and $C$ is the size of the inserted row. 
    >
    >

    > [!WARNING] Invalidates all iterators, pointers, and references after the insertion point. 
    >
    >






        

<hr>



### function pop\_row 

_Removes the last row from the matrix._ 
```cpp
inline void gl::flat_matrix::pop_row () 
```





**Postcondition:**

If not empty, `n_rows()` decreases by 1.



> [!INFO] Time Complexity
>
>
> [**$O(C)**](group__GL-Core.md#variable-invalid_id_v)$ to truncate the underlying _data_ vector. 
>
>

> [!NOTE] Safety
>
>
> Safe to call on an empty matrix (no-op). 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to elements in the last row. 
>
>



        

<hr>



### function erase\_row 

_Erases the row at the specified position._ 
```cpp
inline void gl::flat_matrix::erase_row (
    size_type pos
) 
```





**Parameters:**


* `pos` The position of the row to erase. 




#### Postconditions




1. The row is removed; subsequent rows are shifted up.
2. `n_rows()` decreases by 1. 

**Exception:**


    * `std::out_of_range` If `pos >=  n_rows()`.


    > [!INFO] Time Complexity
    >
    >
    > $O(E + C)$ where $E$ is the number of elements after the erased row and $C$ is the number of columns (the size of the erased row). 
    >
    >

    > [!WARNING] Invalidates all iterators, pointers, and references at or after the erased position. 
    >
    >






        

<hr>



### function push\_col [1/3]

_Appends a range as a new column at the right edge of the matrix._ 
```cpp
template<std::ranges::input_range R>
inline void gl::flat_matrix::push_col (
    R && r
) 
```





**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `r` The range to append. 



**Postcondition:**

`n_cols()` increases by 1. 




**Exception:**


* `std::invalid_argument` If the column size does not match `n_rows()`. 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a full reallocation and architectural shift of the mathematical grid. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function push\_col [2/3]

_Appends an initializer list as a new column at the right edge of the matrix._ 
```cpp
inline void gl::flat_matrix::push_col (
    std::initializer_list< value_type > ilist
) 
```





**Parameters:**


* `ilist` The list to append. 



**Postcondition:**

`n_cols()` increases by 1. 




**Exception:**


* `std::invalid_argument` If the list size does not match `n_rows()`. 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a full reallocation and architectural shift of the mathematical grid. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function push\_col [3/3]

_Appends a newly created column filled with a specific value at the right edge._ 
```cpp
inline void gl::flat_matrix::push_col (
    const  value_type & value
) 
```



If the matrix is empty (has no rows), this operation has no effect, as the size of the new column will be determined as 0.




**Parameters:**


* `value` The value to fill the new column with. 



**Postcondition:**

`n_cols()` increases by 1. 




**Exception:**


* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a full reallocation and architectural shift of the mathematical grid. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function insert\_col [1/3]

_Inserts a new column at the specified position from a range._ 
```cpp
template<std::ranges::input_range R>
inline void gl::flat_matrix::insert_col (
    size_type pos,
    R && r
) 
```





**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `pos` The column position where elements will be inserted. 
* `r` The range to insert. 



**Postcondition:**

`n_cols()` increases by 1. 




**Exception:**


* `std::out_of_range` If `pos >  n_cols()`. 
* `std::invalid_argument` If the range size does not match `n_rows()`. 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a full reallocation and architectural shift of the mathematical grid. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function insert\_col [2/3]

_Inserts a new column at the specified position from an initializer list._ 
```cpp
inline void gl::flat_matrix::insert_col (
    size_type pos,
    std::initializer_list< value_type > ilist
) 
```





**Parameters:**


* `pos` The column position where elements will be inserted. 
* `ilist` The list to insert. 



**Postcondition:**

`n_cols()` increases by 1. 




**Exception:**


* `std::out_of_range` If `pos >  n_cols()`. 
* `std::invalid_argument` If the list size does not match `n_rows()`. 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a full reallocation and architectural shift of the mathematical grid. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function insert\_col [3/3]

_Inserts a newly created column filled with a specific value at the specified position._ 
```cpp
inline void gl::flat_matrix::insert_col (
    size_type pos,
    const  value_type & value
) 
```



If the matrix is empty (has no rows), this operation has no effect, as the size of the new column will be determined as 0.




**Parameters:**


* `pos` The column position where elements will be inserted. 
* `value` The value to fill the new column with. 



**Postcondition:**

`n_cols()` increases by 1. 




**Exception:**


* `std::out_of_range` If `pos >  n_cols()`. 
* `std::bad_alloc` If memory allocation fails.


> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a full reallocation and architectural shift of the mathematical grid. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function pop\_col 

_Removes the last column from the matrix._ 
```cpp
inline void gl::flat_matrix::pop_col () 
```





**Postcondition:**

If not empty, `n_cols()` decreases by 1.



> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
>
>

> [!NOTE] Safety
>
>
> Safe to call on an empty matrix (no-op). 
>
>

> [!WARNING] Iterator Invalidation
>
>
> This operation forces a reallocation and structural shift. All iterators, pointers, and references are invalidated. 
>
>



        

<hr>



### function erase\_col 

_Erases the column at the specified position._ 
```cpp
inline void gl::flat_matrix::erase_col (
    size_type pos
) 
```





**Parameters:**


* `pos` The position of the column to erase. 




#### Postconditions




1. The column is removed; subsequent columns are mathematically shifted left.
2. `n_cols()` decreases by 1. 

**Exception:**


    * `std::out_of_range` If `pos >=  n_cols()`.


    > [!INFO] Time Complexity
    >
    >
    > $O(R \cdot C)$ where $R$ and $C$ are dimensions of the matrix. 
    >
    >

    > [!WARNING] Iterator Invalidation
    >
    >
    > This operation forces a reallocation and structural shift. All iterators, pointers, and references are invalidated. 
    >
    >






        

<hr>



### function begin [1/2]

_Returns a mutable iterator to the first row._ 
```cpp
inline iterator gl::flat_matrix::begin () noexcept
```





**Returns:**

Iterator to the first row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function end [1/2]

_Returns a mutable iterator past the last row (end sentinel)._ 
```cpp
inline iterator gl::flat_matrix::end () noexcept
```





**Returns:**

Iterator one position past the last row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function begin [2/2]

_Returns a const iterator to the first row._ 
```cpp
inline const_iterator gl::flat_matrix::begin () noexcept const
```





**Returns:**

Const iterator to the first row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function end [2/2]

_Returns a const iterator past the last row (end sentinel)._ 
```cpp
inline const_iterator gl::flat_matrix::end () noexcept const
```





**Returns:**

Const iterator one position past the last row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function cbegin 

_Returns a const iterator to the first row (explicit const form)._ 
```cpp
inline const_iterator gl::flat_matrix::cbegin () noexcept const
```





**Returns:**

Const iterator to the first row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function cend 

_Returns a const iterator past the last row (explicit const form)._ 
```cpp
inline const_iterator gl::flat_matrix::cend () noexcept const
```





**Returns:**

Const iterator one past the last row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rbegin [1/2]

_Returns a reverse iterator to the last row._ 
```cpp
inline reverse_iterator gl::flat_matrix::rbegin () noexcept
```





**Returns:**

Reverse iterator starting at the last row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rend [1/2]

_Returns a reverse iterator before the first row (end sentinel)._ 
```cpp
inline reverse_iterator gl::flat_matrix::rend () noexcept
```





**Returns:**

Reverse iterator one position before the first row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rbegin [2/2]

_Returns a const reverse iterator to the last row._ 
```cpp
inline const_reverse_iterator gl::flat_matrix::rbegin () noexcept const
```





**Returns:**

Const reverse iterator starting at the last row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rend [2/2]

_Returns a const reverse iterator before the first row (end sentinel)._ 
```cpp
inline const_reverse_iterator gl::flat_matrix::rend () noexcept const
```





**Returns:**

Const reverse iterator one position before the first row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function crbegin 

_Returns a const reverse iterator to the last row (explicit const form)._ 
```cpp
inline const_reverse_iterator gl::flat_matrix::crbegin () noexcept const
```





**Returns:**

Const reverse iterator starting at the last row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function crend 

_Returns a const reverse iterator before the first row (explicit const form)._ 
```cpp
inline const_reverse_iterator gl::flat_matrix::crend () noexcept const
```





**Returns:**

Const reverse iterator one position before the first row. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function transpose 

_Transposes the matrix mathematically (rows become columns, columns become rows)._ 
```cpp
inline flat_matrix gl::flat_matrix::transpose () const
```





**Returns:**

A new `flat_matrix` instance containing the transposed data.



> [!INFO] Time Complexity
>
>
> $O(R \cdot C)$ to generate and fill the new matrix. 
>
>



        

<hr>## Friends Documentation





### friend operator== 

_Tests equality of two_ `flat_matrix` _instances._
```cpp
bool gl::flat_matrix::operator== (
    const  flat_matrix &,
    const  flat_matrix &
) = default
```





**Parameters:**


* `lhs` Left operand. 
* `rhs` Right operand. 



**Returns:**

`true` if dimensions and all elements match. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/flat_matrix.hpp`

