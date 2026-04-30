

# Class gl::flat\_jagged\_vector

**template &lt;std::semiregular T&gt;**



[**ClassList**](annotated.md) **>** [**gl**](namespacegl.md) **>** [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md)



_A flattened 2D vector (jagged array) providing efficient storage for variable-length segments._ [More...](#detailed-description)

* `#include <gl/types/flat_jagged_vector.hpp>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) &lt;Const&gt;<br>_Random access iterator over segments of the_ `flat_jagged_vector` _._ |


## Public Types

| Type | Name |
| ---: | :--- |
| typedef T | [**value\_type**](#typedef-value_type)  <br>_Type of elements stored in segments._  |
| typedef std::size\_t | [**size\_type**](#typedef-size_type)  <br>_Unsigned integral type used for sizes and indices._  |
| typedef std::vector&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; | [**container\_type**](#typedef-container_type)  <br>_The underlying contiguous storage container._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::reference | [**reference**](#typedef-reference)  <br>_Reference to an element._  |
| typedef [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::const\_reference | [**const\_reference**](#typedef-const_reference)  <br>_Const reference to an element._  |
| typedef std::ranges::subrange&lt; [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::iterator &gt; | [**segment\_type**](#typedef-segment_type)  <br>_Subrange type representing a non-owning segment of elements._  |
| typedef std::ranges::subrange&lt; [**typename**](group__GL-Core.md#variable-invalid_id_v) container\_type::const\_iterator &gt; | [**const\_segment\_type**](#typedef-const_segment_type)  <br>_Const subrange type representing a non-owning const segment of elements._  |
| typedef [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md)&lt; [**false**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**iterator**](#typedef-iterator)  <br>_Mutable random access iterator over segments._  |
| typedef [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md)&lt; [**true**](group__GL-Core.md#variable-invalid_id_v) &gt; | [**const\_iterator**](#typedef-const_iterator)  <br>_Const random access iterator over segments._  |
| typedef std::reverse\_iterator&lt; [**iterator**](classgl_1_1flat__jagged__vector.md#typedef-iterator) &gt; | [**reverse\_iterator**](#typedef-reverse_iterator)  <br>_Reverse mutable iterator._  |
| typedef std::reverse\_iterator&lt; [**const\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_iterator) &gt; | [**const\_reverse\_iterator**](#typedef-const_reverse_iterator)  <br>_Reverse const iterator._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**flat\_jagged\_vector**](#function-flat_jagged_vector-16) () = default<br>_Default constructor creates an empty_ `flat_jagged_vector` _._ |
|   | [**flat\_jagged\_vector**](#function-flat_jagged_vector-26) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) &) = default<br>_Copy constructor creates a deep copy of another_ `flat_jagged_vector` _._ |
|  [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) &) = default<br>_Copy assignment creates a deep copy of another_ `flat_jagged_vector` _._ |
|   | [**flat\_jagged\_vector**](#function-flat_jagged_vector-36) ([**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) && other) noexcept<br>_Move constructor transfers ownership of data from another_ `flat_jagged_vector` _._ |
|  [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) & | [**operator=**](#function-operator_1) ([**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) && other) noexcept<br>_Move assignment transfers ownership of data from another_ `flat_jagged_vector` _._ |
|   | [**flat\_jagged\_vector**](#function-flat_jagged_vector-46) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n\_segments, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) segment\_size=0uz) <br>_Constructs a_ `flat_jagged_vector` _with a specified number of segments and initial segment size._ |
|   | [**flat\_jagged\_vector**](#function-flat_jagged_vector-56) (std::initializer\_list&lt; std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; &gt; ilist) <br>_Constructs a_ `flat_jagged_vector` _from an initializer list of segments._ |
|   | [**flat\_jagged\_vector**](#function-flat_jagged_vector-66) (R && r) <br>_Constructs a_ `flat_jagged_vector` _from a range of ranges._ |
|   | [**~flat\_jagged\_vector**](#function-flat_jagged_vector) () = default<br>_Destructor cleans up all managed memory._  |
|  [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) | [**size**](#function-size) () noexcept const<br>_Returns the number of segments in this container._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**empty**](#function-empty-12) () noexcept const<br>_Checks if the container is empty (contains no segments)._  |
|  [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) | [**segments\_capacity**](#function-segments_capacity) () noexcept const<br>_Returns the current capacity for segments (number of segment slots allocated)._  |
|  [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) | [**data\_capacity**](#function-data_capacity) () noexcept const<br>_Returns the current capacity for data elements._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**reserve\_segments**](#function-reserve_segments) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n) <br>_Reserves space for at least n additional segments in the_ _offsets_ _storage without changing the size._ |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**reserve\_data**](#function-reserve_data) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n) <br>_Reserves space for at least n additional data elements without changing the size._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**shrink\_to\_fit**](#function-shrink_to_fit) () <br>_Reduces capacity of both internal arrays to match current size._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**resize**](#function-resize-15) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n) <br>_Resizes the container to contain_ `n` _segments._ |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**resize**](#function-resize-25) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n, R && r) <br>_Resizes the container to contain_ `n` _segments, initializing any new segments with a range._ |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**resize**](#function-resize-35) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n, std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; ilist) <br>_Resizes the container to contain_ `n` _segments, initializing any new segments with an initializer list._ |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**clear**](#function-clear) () <br>_Removes all segments and elements, leaving the container empty._  |
|  [**segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-segment_type) | [**operator[]**](#function-operator_2) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) <br>_Returns the segment at the given index without bounds checking._  |
|  [**const\_segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-const_segment_type) | [**operator[]**](#function-operator_3) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) const<br>_Returns a const segment at the given index without bounds checking._  |
|  [**reference**](classgl_1_1flat__jagged__vector.md#typedef-reference) | [**operator[]**](#function-operator_4) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos) <br>_Returns a reference to an element within a segment without bounds checking._  |
|  [**const\_reference**](classgl_1_1flat__jagged__vector.md#typedef-const_reference) | [**operator[]**](#function-operator_5) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos) const<br>_Returns a const reference to an element within a segment without bounds checking._  |
|  [**segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-segment_type) | [**at**](#function-at-14) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) <br>_Returns the segment at the given index with bounds checking._  |
|  [**const\_segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-const_segment_type) | [**at**](#function-at-24) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) const<br>_Returns a const segment at the given index with bounds checking._  |
|  [**reference**](classgl_1_1flat__jagged__vector.md#typedef-reference) | [**at**](#function-at-34) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos) <br>_Returns a reference to an element within a segment with bounds checking._  |
|  [**const\_reference**](classgl_1_1flat__jagged__vector.md#typedef-const_reference) | [**at**](#function-at-44) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos) const<br>_Returns a const reference to an element within a segment with bounds checking._  |
|  [**segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-segment_type) | [**front**](#function-front-14) () noexcept<br>_Returns the first segment without bounds checking._  |
|  [**const\_segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-const_segment_type) | [**front**](#function-front-24) () noexcept const<br>_Returns a const reference to the first segment without bounds checking._  |
|  [**segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-segment_type) | [**back**](#function-back-14) () noexcept<br>_Returns the last segment without bounds checking._  |
|  [**const\_segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-const_segment_type) | [**back**](#function-back-24) () noexcept const<br>_Returns a const reference to the last segment without bounds checking._  |
|  [**reference**](classgl_1_1flat__jagged__vector.md#typedef-reference) | [**front**](#function-front-34) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) noexcept<br>_Returns a reference to the first element in a segment without bounds checking._  |
|  [**const\_reference**](classgl_1_1flat__jagged__vector.md#typedef-const_reference) | [**front**](#function-front-44) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) noexcept const<br>_Returns a const reference to the first element in a segment without bounds checking._  |
|  [**reference**](classgl_1_1flat__jagged__vector.md#typedef-reference) | [**back**](#function-back-34) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) noexcept<br>_Returns a reference to the last element in a segment without bounds checking._  |
|  [**const\_reference**](classgl_1_1flat__jagged__vector.md#typedef-const_reference) | [**back**](#function-back-44) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) noexcept const<br>_Returns a const reference to the last element in a segment without bounds checking._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**segments**](#function-segments-12) () noexcept<br>_Returns a view of all segments for iteration._  |
|  [**auto**](group__GL-Core.md#variable-invalid_id_v) | [**segments**](#function-segments-22) () noexcept const<br>_Returns a const view of all segments for iteration._  |
|  [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**empty**](#function-empty-22) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) noexcept const<br>_Checks if a specific segment is empty without bounds checking._  |
|  [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) | [**segment\_size**](#function-segment_size) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) noexcept const<br>_Returns the number of elements in a specific segment without bounds checking._  |
|  [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) | [**data\_size**](#function-data_size) () noexcept const<br>_Returns the total number of elements across all segments._  |
|  [**segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-segment_type) | [**data\_view**](#function-data_view-12) () noexcept<br>_Returns a subrange of all element data in flattened form._  |
|  [**const\_segment\_type**](classgl_1_1flat__jagged__vector.md#typedef-const_segment_type) | [**data\_view**](#function-data_view-22) () noexcept const<br>_Returns a const subrange of all element data in flattened form._  |
|  std::vector&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; & | [**data\_storage**](#function-data_storage-12) () noexcept<br>_Returns a reference to the underlying flat data container._  |
|  [**const**](group__GL-Core.md#variable-invalid_id_v) std::vector&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; & | [**data\_storage**](#function-data_storage-22) () noexcept const<br>_Returns a const reference to the underlying flat data container._  |
|  [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) \* | [**data\_ptr**](#function-data_ptr-12) () noexcept<br>_Returns a raw pointer to the underlying flat data array._  |
|  [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) \* | [**data\_ptr**](#function-data_ptr-22) () noexcept const<br>_Returns a const raw pointer to the underlying flat data array._  |
|  std::span&lt; [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) &gt; | [**offsets\_view**](#function-offsets_view-12) () noexcept<br>_Returns a span over the segment offset array._  |
|  std::span&lt; [**const**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) &gt; | [**offsets\_view**](#function-offsets_view-22) () noexcept const<br>_Returns a const span over the segment offset array._  |
|  std::vector&lt; [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) &gt; & | [**offsets\_storage**](#function-offsets_storage-12) () noexcept<br>_Returns a reference to the underlying segment offset container._  |
|  [**const**](group__GL-Core.md#variable-invalid_id_v) std::vector&lt; [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) &gt; & | [**offsets\_storage**](#function-offsets_storage-22) () noexcept const<br>_Returns a const reference to the underlying segment offset container._  |
|  [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) \* | [**offsets\_ptr**](#function-offsets_ptr-12) () noexcept<br>_Returns a raw pointer to the underlying segment offset array._  |
|  [**const**](group__GL-Core.md#variable-invalid_id_v) [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) \* | [**offsets\_ptr**](#function-offsets_ptr-22) () noexcept const<br>_Returns a const raw pointer to the underlying segment offset array._  |
|  [**iterator**](classgl_1_1flat__jagged__vector.md#typedef-iterator) | [**begin**](#function-begin-12) () noexcept<br>_Returns a mutable iterator to the first segment._  |
|  [**iterator**](classgl_1_1flat__jagged__vector.md#typedef-iterator) | [**end**](#function-end-12) () noexcept<br>_Returns a mutable iterator past the last segment (end sentinel)._  |
|  [**const\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_iterator) | [**begin**](#function-begin-22) () noexcept const<br>_Returns a const iterator to the first segment._  |
|  [**const\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_iterator) | [**end**](#function-end-22) () noexcept const<br>_Returns a const iterator past the last segment (end sentinel)._  |
|  [**const\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_iterator) | [**cbegin**](#function-cbegin) () noexcept const<br>_Returns a const iterator to the first segment (explicit const form)._  |
|  [**const\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_iterator) | [**cend**](#function-cend) () noexcept const<br>_Returns a const iterator past the last segment (explicit const form)._  |
|  [**reverse\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-reverse_iterator) | [**rbegin**](#function-rbegin-12) () noexcept<br>_Returns a reverse iterator to the last segment._  |
|  [**reverse\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-reverse_iterator) | [**rend**](#function-rend-12) () noexcept<br>_Returns a reverse iterator before the first segment (end sentinel)._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_reverse_iterator) | [**rbegin**](#function-rbegin-22) () noexcept const<br>_Returns a const reverse iterator to the last segment._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_reverse_iterator) | [**rend**](#function-rend-22) () noexcept const<br>_Returns a const reverse iterator before the first segment (end sentinel)._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_reverse_iterator) | [**crbegin**](#function-crbegin) () noexcept const<br>_Returns a const reverse iterator to the last segment (explicit const form)._  |
|  [**const\_reverse\_iterator**](classgl_1_1flat__jagged__vector.md#typedef-const_reverse_iterator) | [**crend**](#function-crend) () noexcept const<br>_Returns a const reverse iterator before the first segment (explicit const form)._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_back**](#function-push_back-13) (R && r) <br>_Appends a new segment to the end of the container._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_back**](#function-push_back-23) (std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; ilist) <br>_Appends a segment from an initializer list._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**pop\_back**](#function-pop_back-12) () <br>_Removes the last segment from the container._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert**](#function-insert-13) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos, R && r) <br>_Inserts a new segment at the specified position._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert**](#function-insert-23) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos, std::initializer\_list&lt; [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) &gt; ilist) <br>_Inserts a segment from an initializer list at the specified position._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**erase**](#function-erase-12) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos) <br>_Erases the segment at the specified position._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**push\_back**](#function-push_back-33) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) & value) <br>_Appends a copy of an element to the end of a specific segment._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**emplace\_back**](#function-emplace_back) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**Args**](group__GL-Core.md#variable-invalid_id_v) &&... args) <br>_Constructs an element in-place at the end of a specific segment._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**pop\_back**](#function-pop_back-22) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg) <br>_Removes the last element from a specific segment._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**insert**](#function-insert-33) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) & value) <br>_Inserts an element at a specific position within a segment._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**emplace**](#function-emplace) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos, [**Args**](group__GL-Core.md#variable-invalid_id_v) &&... args) <br>_Constructs an element in-place at a specific position within a segment._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**erase**](#function-erase-22) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) pos) <br>_Removes an element at a specific position within a segment._  |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**resize**](#function-resize-45) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n) <br>_Resizes a specific segment to contain_ `n` _elements._ |
|  [**void**](group__GL-Core.md#variable-invalid_id_v) | [**resize**](#function-resize-55) ([**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) seg, [**size\_type**](classgl_1_1flat__jagged__vector.md#typedef-size_type) n, [**const**](group__GL-Core.md#variable-invalid_id_v) [**value\_type**](classgl_1_1flat__jagged__vector.md#typedef-value_type) & value) <br>_Resizes a specific segment to contain_ `n` _elements, initializing new elements with_`value` _._ |




























## Detailed Description


This container stores all elements in a single contiguous memory block (_data_) while maintaining an offsets array (_offsets_) to track segment boundaries. This design provides excellent cache locality and efficient iteration over individual segments.


#### Example Usage




```cpp
#include <gl/types/flat_jagged_vector.hpp>

#include <iostream>

int main() {
    gl::flat_jagged_vector<int> vec = { // (1)!
        {1, 2, 3},
        {4, 5},
        {6, 7, 8, 9}
    };

    vec.push_back({10, 11}); // (2)!
    vec.push_back(1uz, 99); // (3)!

    for (const auto [i, segment] : vec) { // (4)!
        std::cout << "Segment " << i << ": ";
        for (int value : segment)
            std::cout << value << " ";
        std::cout << '\n';
    }

    return 0;
}
```



1. Initialize the `flat_jagged_vector` with an initializer list of segments, where each segment can have a different length.


2. Append a new segment to the end of the container using `push_back()`.


3. Append an element to the second segment (index 1).


4. Iterate over the segments and their elements using the `operator[]` to access each segment as a range.


**Output:** 
```text
Segment 0: 1 2 3
Segment 1: 4 5 99
Segment 2: 6 7 8 9
Segment 3: 10 11
```




> [!NOTE] Container behaviour
>
>
> Behavior is similar to `std::vector<std::vector<T>>` but with flattened memory layout. 
>
>

> [!IMPORTANT] Iterator Invalidation Policy
>
>
> Iterator invalidation follows `std::vector` semantics: modifying the structure or elements invalidates all iterators, pointers, and references to the container's elements. 
>
>




#### Template Parameters




|Parameter|Description|Constraint|
|-----|-----|-----|
|T|The type of the elements|Must be **semiregular** (default constructible and copyable)|







#### TODO




* Implement assign, and swap methods.
* Implement iterator-based insert, emplace and erase methods.
* Add `operator<<` overload for `std::ostream` and specialize `std::formatter`.
* Use `std::ptrdiff_t` instead of `std::size_t` for offset values. 





    
## Public Types Documentation




### typedef value\_type 

_Type of elements stored in segments._ 
```cpp
using gl::flat_jagged_vector< T >::value_type =  T;
```




<hr>



### typedef size\_type 

_Unsigned integral type used for sizes and indices._ 
```cpp
using gl::flat_jagged_vector< T >::size_type =  std::size_t;
```




<hr>



### typedef container\_type 

_The underlying contiguous storage container._ 
```cpp
using gl::flat_jagged_vector< T >::container_type =  std::vector<value_type>;
```




<hr>



### typedef reference 

_Reference to an element._ 
```cpp
using gl::flat_jagged_vector< T >::reference =  typename container_type::reference;
```




<hr>



### typedef const\_reference 

_Const reference to an element._ 
```cpp
using gl::flat_jagged_vector< T >::const_reference =  typename container_type::const_reference;
```




<hr>



### typedef segment\_type 

_Subrange type representing a non-owning segment of elements._ 
```cpp
using gl::flat_jagged_vector< T >::segment_type =  std::ranges::subrange<typename container_type::iterator>;
```




<hr>



### typedef const\_segment\_type 

_Const subrange type representing a non-owning const segment of elements._ 
```cpp
using gl::flat_jagged_vector< T >::const_segment_type =  std::ranges::subrange<typename container_type::const_iterator>;
```




<hr>



### typedef iterator 

_Mutable random access iterator over segments._ 
```cpp
using gl::flat_jagged_vector< T >::iterator =  segment_iterator<false>;
```




<hr>



### typedef const\_iterator 

_Const random access iterator over segments._ 
```cpp
using gl::flat_jagged_vector< T >::const_iterator =  segment_iterator<true>;
```




<hr>



### typedef reverse\_iterator 

_Reverse mutable iterator._ 
```cpp
using gl::flat_jagged_vector< T >::reverse_iterator =  std::reverse_iterator<iterator>;
```




<hr>



### typedef const\_reverse\_iterator 

_Reverse const iterator._ 
```cpp
using gl::flat_jagged_vector< T >::const_reverse_iterator =  std::reverse_iterator<const_iterator>;
```




<hr>
## Public Functions Documentation




### function flat\_jagged\_vector [1/6]

_Default constructor creates an empty_ `flat_jagged_vector` _._
```cpp
gl::flat_jagged_vector::flat_jagged_vector () = default
```





**Postcondition:**

`empty() == true and size() == 0 and data_size() == 0` 





        

<hr>



### function flat\_jagged\_vector [2/6]

_Copy constructor creates a deep copy of another_ `flat_jagged_vector` _._
```cpp
gl::flat_jagged_vector::flat_jagged_vector (
    const  flat_jagged_vector &
) = default
```





**Parameters:**


* `other` The `flat_jagged_vector` to copy. 



**Postcondition:**

`*this == other` 





        

<hr>



### function operator= 

_Copy assignment creates a deep copy of another_ `flat_jagged_vector` _._
```cpp
flat_jagged_vector & gl::flat_jagged_vector::operator= (
    const  flat_jagged_vector &
) = default
```





**Parameters:**


* `other` The source `flat_jagged_vector`. 



**Returns:**

Reference to `*this`. 




**Postcondition:**

`*this == other` 





        

<hr>



### function flat\_jagged\_vector [3/6]

_Move constructor transfers ownership of data from another_ `flat_jagged_vector` _._
```cpp
inline gl::flat_jagged_vector::flat_jagged_vector (
    flat_jagged_vector && other
) noexcept
```





**Parameters:**


* `other` The source `flat_jagged_vector` (left in a valid but unspecified state).




#### Postconditions:




1. `other.empty() == true`
2. All data from `other` is transferred to `*this`





> [!WARNING] Invalidates all iterators, pointers, and references to other instance's elements. 
>
>




        

<hr>



### function operator= 

_Move assignment transfers ownership of data from another_ `flat_jagged_vector` _._
```cpp
inline flat_jagged_vector & gl::flat_jagged_vector::operator= (
    flat_jagged_vector && other
) noexcept
```





**Parameters:**


* `other` The source `flat_jagged_vector`. 



**Returns:**

Reference to `*this`. 





#### Postconditions:




1. `other.empty() == true`
2. All data from `other` is transferred to `*this`





> [!INFO] Safety
>
>
> This function safely handles self-assignment and if `this != &other` the operator will leave the `other` instance in a valid, empty state. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to this container's elements. 
>
>




        

<hr>



### function flat\_jagged\_vector [4/6]

_Constructs a_ `flat_jagged_vector` _with a specified number of segments and initial segment size._
```cpp
inline gl::flat_jagged_vector::flat_jagged_vector (
    size_type n_segments,
    size_type segment_size=0uz
) 
```





**Parameters:**


* `n_segments` The number of segments to create. 
* `segment_size` The initial size of each segment (default is 0). 



**Postcondition:**

`size() == n_segments` and each segment is initialized with `segment_size` default-constructed elements. 




**Exception:**


* `std::bad_alloc` May throw if memory allocation fails. 




        

<hr>



### function flat\_jagged\_vector [5/6]

_Constructs a_ `flat_jagged_vector` _from an initializer list of segments._
```cpp
inline gl::flat_jagged_vector::flat_jagged_vector (
    std::initializer_list< std::initializer_list< value_type > > ilist
) 
```





**Parameters:**


* `ilist` Initializer list of initializer lists, each representing a segment. 



**Postcondition:**

`size() == ilist.size()` and `data_size()` equals the sum of all segment sizes. 




**Exception:**


* `std::bad_alloc` May throw if memory allocation fails. 




        

<hr>



### function flat\_jagged\_vector [6/6]

_Constructs a_ `flat_jagged_vector` _from a range of ranges._
```cpp
template<std::ranges::input_range R>
inline explicit gl::flat_jagged_vector::flat_jagged_vector (
    R && r
) 
```



This constructor accepts any input range of input ranges convertible to `value_type`, enabling flexible initialization from various container types.




**Template parameters:**


* `R` A range type whose elements are input ranges of `value_type`. 



**Parameters:**


* `r` The range of ranges to initialize from. 



**Postcondition:**

`size()` equals the number of outer range elements; `data_size()` is the sum of all element counts. 




**Exception:**


* `std::bad_alloc` May throw if memory allocation fails. 




        

<hr>



### function ~flat\_jagged\_vector 

_Destructor cleans up all managed memory._ 
```cpp
gl::flat_jagged_vector::~flat_jagged_vector () = default
```




<hr>



### function size 

_Returns the number of segments in this container._ 
```cpp
inline size_type gl::flat_jagged_vector::size () noexcept const
```





**Returns:**

The number of segments. 





        

<hr>



### function empty [1/2]

_Checks if the container is empty (contains no segments)._ 
```cpp
inline bool gl::flat_jagged_vector::empty () noexcept const
```





**Returns:**

`true` if there are no elements in the container, `false` otherwise. 





        

<hr>



### function segments\_capacity 

_Returns the current capacity for segments (number of segment slots allocated)._ 
```cpp
inline size_type gl::flat_jagged_vector::segments_capacity () noexcept const
```





**Returns:**

The number of segments that can be stored without reallocation of the _offsets_ storage. 





        

<hr>



### function data\_capacity 

_Returns the current capacity for data elements._ 
```cpp
inline size_type gl::flat_jagged_vector::data_capacity () noexcept const
```





**Returns:**

The number of elements that can be stored in _data_ storage without reallocation. 





        

<hr>



### function reserve\_segments 

_Reserves space for at least n additional segments in the_ _offsets_ _storage without changing the size._
```cpp
inline void gl::flat_jagged_vector::reserve_segments (
    size_type n
) 
```





**Parameters:**


* `n` The number of segments to reserve space for. 



**Postcondition:**

`segments_capacity() >= n + size()` 
> [!WARNING] Invalidates all iterators and pointers to elements if reallocation occurs. 
>
>







        

<hr>



### function reserve\_data 

_Reserves space for at least n additional data elements without changing the size._ 
```cpp
inline void gl::flat_jagged_vector::reserve_data (
    size_type n
) 
```





**Parameters:**


* `n` The number of elements to reserve space for. 



**Postcondition:**

`data_capacity() >= n + data_size()` 
> [!WARNING] Invalidates all iterators and pointers to elements if reallocation occurs. 
>
>







        

<hr>



### function shrink\_to\_fit 

_Reduces capacity of both internal arrays to match current size._ 
```cpp
inline void gl::flat_jagged_vector::shrink_to_fit () 
```



#### Postconditions




1. `segments_capacity() == size()`
2. `data_capacity() == data_size()` 
    > [!WARNING] Invalidates all iterators, pointers, and references to elements. 
    >
    >






        

<hr>



### function resize [1/5]

_Resizes the container to contain_ `n` _segments._
```cpp
inline void gl::flat_jagged_vector::resize (
    size_type n
) 
```




* If the current size is greater than `n`, the container is reduced to its first `n` segments.
* If the current size is less than `n`, additional empty segments are appended.
* If the current size is equal to `n`, the container is unchanged.






**Parameters:**


* `n` The new number of segments. 



**Postcondition:**

`size() == n`.



> [!WARNING] Iterator Invalidation
>
>
> This operation invalidates all iterators, pointers, and references to elements if reallocation occurs, or if the container shrinks (invalidating removed segments). 
>
>

> [!INFO] Time Complexity
>
>
> [**$O(E)**](group__GL-Core.md#variable-invalid_id_v)$ when shrinking (where $E$ is the total number of elements in the removed segments), or amortized [**$O(S)**](group__GL-Core.md#variable-invalid_id_v)$ when growing (where $S$ is the number of new empty segments). 
>
>



        

<hr>



### function resize [2/5]

_Resizes the container to contain_ `n` _segments, initializing any new segments with a range._
```cpp
template<std::ranges::input_range R>
inline void gl::flat_jagged_vector::resize (
    size_type n,
    R && r
) 
```




* If the current size is greater than `n`, the container is reduced to its first `n` segments.
* If the current size is less than `n`, new segments are appended, each containing the elements in `r`.
* If the current size is equal to `n`, the container is unchanged.






**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `n` The new number of segments. 
* `r` The range to initialize any newly appended segments with. 



**Postcondition:**

`size() == n` 




**Exception:**


* `std::bad_alloc` If memory allocation fails.


> [!WARNING] Iterator Invalidation
>
>
> This operation invalidates all iterators, pointers, and references to elements if reallocation occurs, or if the container shrinks (invalidating removed segments). 
>
>

> [!INFO] Time Complexity
>
>
> [**$O(E)**](group__GL-Core.md#variable-invalid_id_v)$ when shrinking (where $E$ is the total number of elements in the removed segments), or amortized \(O(S \cdot E_r)\) when growing (where $S$ is the number of new empty segments and \(E_r\) is the number of elements in the input range `r`). 
>
>



        

<hr>



### function resize [3/5]

_Resizes the container to contain_ `n` _segments, initializing any new segments with an initializer list._
```cpp
inline void gl::flat_jagged_vector::resize (
    size_type n,
    std::initializer_list< value_type > ilist
) 
```




* If the current size is greater than `n`, the container is reduced to its first `n` segments.
* If the current size is less than `n`, new segments are appended, each containing the elements in `ilist`.
* If the current size is equal to `n`, the container is unchanged.






**Parameters:**


* `n` The new number of segments. 
* `ilist` The initializer list to initialize any newly appended segments with.


> [!INFO] Implementation
>
>
> This function is equivalent to calling `resize(n, std::span<const value_type>{ilist})`. 
>
>



        

<hr>



### function clear 

_Removes all segments and elements, leaving the container empty._ 
```cpp
inline void gl::flat_jagged_vector::clear () 
```





**Postcondition:**

`empty() == true` but the capacity remains unchanged. 
> [!WARNING] Invalidates all iterators, pointers, and references to elements. 
>
>







        

<hr>



### function operator[] 

_Returns the segment at the given index without bounds checking._ 
```cpp
inline segment_type gl::flat_jagged_vector::operator[] (
    size_type seg
) 
```





**Parameters:**


* `i` The index of the segment to access. 



**Returns:**

A subrange representing the segment at index i. 




**Precondition:**

`seg <  size()`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(seg)` for bounds-checked access. Calling on an out-of-bounds index results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a const segment at the given index without bounds checking._ 
```cpp
inline const_segment_type gl::flat_jagged_vector::operator[] (
    size_type seg
) const
```





**Parameters:**


* `seg` The index of the segment to access. 



**Returns:**

A const subrange representing the segment at index i. 




**Precondition:**

`seg <  size()`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(seg)` for bounds-checked access. Calling on an out-of-bounds index results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a reference to an element within a segment without bounds checking._ 
```cpp
inline reference gl::flat_jagged_vector::operator[] (
    size_type seg,
    size_type pos
) 
```





**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment. 



**Returns:**

Reference to the element at the given segment and position. 




**Precondition:**

`seg <  size()` and `pos < segment_size(seg)`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(seg, pos)` for bounds-checked access. Out-of-bounds access results in Undefined Behavior. 
>
>



        

<hr>



### function operator[] 

_Returns a const reference to an element within a segment without bounds checking._ 
```cpp
inline const_reference gl::flat_jagged_vector::operator[] (
    size_type seg,
    size_type pos
) const
```





**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment. 



**Returns:**

Const reference to the element at the given segment and position. 




**Precondition:**

`seg <  size()` and `pos < segment_size(seg)`



> [!WARNING] Undefined Behavior
>
>
> No bounds checking is performed for performance. Use `at(seg, pos)` for bounds-checked access. Out-of-bounds access results in Undefined Behavior. 
>
>



        

<hr>



### function at [1/4]

_Returns the segment at the given index with bounds checking._ 
```cpp
inline segment_type gl::flat_jagged_vector::at (
    size_type seg
) 
```





**Parameters:**


* `seg` The index of the segment. 



**Returns:**

A subrange representing the segment at index `seg`. 




**Exception:**


* `std::out_of_range` If `seg >=  size()`. 




        

<hr>



### function at [2/4]

_Returns a const segment at the given index with bounds checking._ 
```cpp
inline const_segment_type gl::flat_jagged_vector::at (
    size_type seg
) const
```





**Parameters:**


* `seg` The index of the segment. 



**Returns:**

A const subrange representing the segment at index `seg`. 




**Exception:**


* `std::out_of_range` If `seg >=  size()`. 




        

<hr>



### function at [3/4]

_Returns a reference to an element within a segment with bounds checking._ 
```cpp
inline reference gl::flat_jagged_vector::at (
    size_type seg,
    size_type pos
) 
```





**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment. 



**Returns:**

Reference to the element at the given segment and position. 




**Exception:**


* `std::out_of_range` If `seg >=  size()` or `pos >= segment_size(seg)`. 




        

<hr>



### function at [4/4]

_Returns a const reference to an element within a segment with bounds checking._ 
```cpp
inline const_reference gl::flat_jagged_vector::at (
    size_type seg,
    size_type pos
) const
```





**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment. 



**Returns:**

Const reference to the element at the given segment and position. 




**Exception:**


* `std::out_of_range` If `seg >=  size()` or `pos >= segment_size(seg)`. 




        

<hr>



### function front [1/4]

_Returns the first segment without bounds checking._ 
```cpp
inline segment_type gl::flat_jagged_vector::front () noexcept
```





**Returns:**

A subrange representing the first segment. 




**Precondition:**

Container must not be empty 
> [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty. 
>
>







        

<hr>



### function front [2/4]

_Returns a const reference to the first segment without bounds checking._ 
```cpp
inline const_segment_type gl::flat_jagged_vector::front () noexcept const
```





**Returns:**

A const subrange representing the first segment. 




**Precondition:**

Container must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty. 
>
>







        

<hr>



### function back [1/4]

_Returns the last segment without bounds checking._ 
```cpp
inline segment_type gl::flat_jagged_vector::back () noexcept
```





**Returns:**

A subrange representing the last segment. 




**Precondition:**

Container must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty. 
>
>







        

<hr>



### function back [2/4]

_Returns a const reference to the last segment without bounds checking._ 
```cpp
inline const_segment_type gl::flat_jagged_vector::back () noexcept const
```





**Returns:**

A const subrange representing the last segment. 




**Precondition:**

Container must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if container is empty. 
>
>







        

<hr>



### function front [3/4]

_Returns a reference to the first element in a segment without bounds checking._ 
```cpp
inline reference gl::flat_jagged_vector::front (
    size_type seg
) noexcept
```





**Parameters:**


* `seg` The segment number. 



**Returns:**

Reference to the first element in the segment. 




**Precondition:**

`seg <  size()` and the segment must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied. 
>
>







        

<hr>



### function front [4/4]

_Returns a const reference to the first element in a segment without bounds checking._ 
```cpp
inline const_reference gl::flat_jagged_vector::front (
    size_type seg
) noexcept const
```





**Parameters:**


* `seg` The segment number. 



**Returns:**

Const reference to the first element in the segment. 




**Precondition:**

`seg <  size()` and the segment must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied. 
>
>







        

<hr>



### function back [3/4]

_Returns a reference to the last element in a segment without bounds checking._ 
```cpp
inline reference gl::flat_jagged_vector::back (
    size_type seg
) noexcept
```





**Parameters:**


* `seg` The segment number. 



**Returns:**

Reference to the last element in the segment. 




**Precondition:**

`seg <  size()` and the segment must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied. 
>
>







        

<hr>



### function back [4/4]

_Returns a const reference to the last element in a segment without bounds checking._ 
```cpp
inline const_reference gl::flat_jagged_vector::back (
    size_type seg
) noexcept const
```





**Parameters:**


* `seg` The segment number. 



**Returns:**

Const reference to the last element in the segment. 




**Precondition:**

`seg <  size()` and the segment must not be empty. 
> [!WARNING] No bounds checking. Results in Undefined Behavior if the precondition is not satisfied. 
>
>







        

<hr>



### function segments [1/2]

_Returns a view of all segments for iteration._ 
```cpp
inline auto gl::flat_jagged_vector::segments () noexcept
```





**Returns:**

A range-adaptable, random-access view of all segments. 
> [!NOTE] This creates a lazy view; iterating yields segments as subranges. 
>
>







        

<hr>



### function segments [2/2]

_Returns a const view of all segments for iteration._ 
```cpp
inline auto gl::flat_jagged_vector::segments () noexcept const
```





**Returns:**

A const range-adaptable, random-access view of all segments. 
> [!NOTE] This creates a lazy view; iterating yields const segments as const subranges. 
>
>







        

<hr>



### function empty [2/2]

_Checks if a specific segment is empty without bounds checking._ 
```cpp
inline bool gl::flat_jagged_vector::empty (
    size_type seg
) noexcept const
```





**Parameters:**


* `seg` The segment number. 



**Returns:**

`true` if the segment is empty, `false` otherwise. 




**Precondition:**

`seg <  size()` 
> [!WARNING] No bounds checking. Results in Undefined Behavior if segment index is out of bounds. 
>
>







        

<hr>



### function segment\_size 

_Returns the number of elements in a specific segment without bounds checking._ 
```cpp
inline size_type gl::flat_jagged_vector::segment_size (
    size_type seg
) noexcept const
```





**Parameters:**


* `seg` The segment number. 



**Returns:**

The count of elements in the segment. 




**Precondition:**

`seg <  size()` 
> [!WARNING] No bounds checking. Results in Undefined Behavior if segment index is out of bounds. 
>
>







        

<hr>



### function data\_size 

_Returns the total number of elements across all segments._ 
```cpp
inline size_type gl::flat_jagged_vector::data_size () noexcept const
```





**Returns:**

The sum of sizes of all segments. 





        

<hr>



### function data\_view [1/2]

_Returns a subrange of all element data in flattened form._ 
```cpp
inline segment_type gl::flat_jagged_vector::data_view () noexcept
```





**Returns:**

A subrange of all elements in the underlying _data_ array. 
> [!NOTE] Allows direct access to the flattened data representation. 
>
>







        

<hr>



### function data\_view [2/2]

_Returns a const subrange of all element data in flattened form._ 
```cpp
inline const_segment_type gl::flat_jagged_vector::data_view () noexcept const
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
inline std::vector< value_type > & gl::flat_jagged_vector::data_storage () noexcept
```





**Returns:**

A mutable reference to the underlying _data_ array.



> [!CAUTION] Use with extreme caution
>
>
> Modifying this vector directly can corrupt the structure of the container. If possible, use `data_view()` instead. This method is intended for advanced operations. 
>
>



        

<hr>



### function data\_storage [2/2]

_Returns a const reference to the underlying flat data container._ 
```cpp
inline const std::vector< value_type > & gl::flat_jagged_vector::data_storage () noexcept const
```





**Returns:**

A const reference to the underlying _data_ array. 





        

<hr>



### function data\_ptr [1/2]

_Returns a raw pointer to the underlying flat data array._ 
```cpp
inline value_type * gl::flat_jagged_vector::data_ptr () noexcept
```





**Returns:**

A raw pointer to the first element in the _data_ array. 
> [!IMPORTANT] Not available for boolean flat vectors. 
>
>







        

<hr>



### function data\_ptr [2/2]

_Returns a const raw pointer to the underlying flat data array._ 
```cpp
inline const  value_type * gl::flat_jagged_vector::data_ptr () noexcept const
```





**Returns:**

A const raw pointer to the first element in the _data_ array. 
> [!IMPORTANT] Not available for boolean flat vectors. 
>
>







        

<hr>



### function offsets\_view [1/2]

_Returns a span over the segment offset array._ 
```cpp
inline std::span< size_type > gl::flat_jagged_vector::offsets_view () noexcept
```





**Returns:**

A span representing the boundaries of all segments.



> [!CAUTION] Use with extreme caution
>
>
> Modifying the offset values will corrupt the container's structural routing. 
>
>



        

<hr>



### function offsets\_view [2/2]

_Returns a const span over the segment offset array._ 
```cpp
inline std::span< const  size_type > gl::flat_jagged_vector::offsets_view () noexcept const
```





**Returns:**

A const span representing the boundaries of all segments. 





        

<hr>



### function offsets\_storage [1/2]

_Returns a reference to the underlying segment offset container._ 
```cpp
inline std::vector< size_type > & gl::flat_jagged_vector::offsets_storage () noexcept
```





**Returns:**

A mutable reference to the _offsets_ vector.



> [!CAUTION] Use with extreme caution
>
>
> Modifying this vector directly (resizing or altering values) may corrupt the container's integrity. 
>
>



        

<hr>



### function offsets\_storage [2/2]

_Returns a const reference to the underlying segment offset container._ 
```cpp
inline const std::vector< size_type > & gl::flat_jagged_vector::offsets_storage () noexcept const
```





**Returns:**

A const reference to the _offsets_ vector. 





        

<hr>



### function offsets\_ptr [1/2]

_Returns a raw pointer to the underlying segment offset array._ 
```cpp
inline size_type * gl::flat_jagged_vector::offsets_ptr () noexcept
```





**Returns:**

A raw pointer to the first element in the _offsets_ array.



> [!CAUTION] Use with extreme caution
>
>
> Modifying the offsets data through this function may corrupt the container's structural routing. 
>
>



        

<hr>



### function offsets\_ptr [2/2]

_Returns a const raw pointer to the underlying segment offset array._ 
```cpp
inline const  size_type * gl::flat_jagged_vector::offsets_ptr () noexcept const
```





**Returns:**

A const raw pointer to the first element in the _offsets_ array. 





        

<hr>



### function begin [1/2]

_Returns a mutable iterator to the first segment._ 
```cpp
inline iterator gl::flat_jagged_vector::begin () noexcept
```





**Returns:**

Iterator to the first segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function end [1/2]

_Returns a mutable iterator past the last segment (end sentinel)._ 
```cpp
inline iterator gl::flat_jagged_vector::end () noexcept
```





**Returns:**

Iterator one position past the last segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function begin [2/2]

_Returns a const iterator to the first segment._ 
```cpp
inline const_iterator gl::flat_jagged_vector::begin () noexcept const
```





**Returns:**

Const iterator to the first segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function end [2/2]

_Returns a const iterator past the last segment (end sentinel)._ 
```cpp
inline const_iterator gl::flat_jagged_vector::end () noexcept const
```





**Returns:**

Const iterator one position past the last segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function cbegin 

_Returns a const iterator to the first segment (explicit const form)._ 
```cpp
inline const_iterator gl::flat_jagged_vector::cbegin () noexcept const
```





**Returns:**

Const iterator to the first segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function cend 

_Returns a const iterator past the last segment (explicit const form)._ 
```cpp
inline const_iterator gl::flat_jagged_vector::cend () noexcept const
```





**Returns:**

Const iterator one past the last segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rbegin [1/2]

_Returns a reverse iterator to the last segment._ 
```cpp
inline reverse_iterator gl::flat_jagged_vector::rbegin () noexcept
```





**Returns:**

Reverse iterator starting at the last segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rend [1/2]

_Returns a reverse iterator before the first segment (end sentinel)._ 
```cpp
inline reverse_iterator gl::flat_jagged_vector::rend () noexcept
```





**Returns:**

Reverse iterator one position before the first segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rbegin [2/2]

_Returns a const reverse iterator to the last segment._ 
```cpp
inline const_reverse_iterator gl::flat_jagged_vector::rbegin () noexcept const
```





**Returns:**

Const reverse iterator starting at the last segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function rend [2/2]

_Returns a const reverse iterator before the first segment (end sentinel)._ 
```cpp
inline const_reverse_iterator gl::flat_jagged_vector::rend () noexcept const
```





**Returns:**

Const reverse iterator one position before the first segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function crbegin 

_Returns a const reverse iterator to the last segment (explicit const form)._ 
```cpp
inline const_reverse_iterator gl::flat_jagged_vector::crbegin () noexcept const
```





**Returns:**

Const reverse iterator starting at the last segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function crend 

_Returns a const reverse iterator before the first segment (explicit const form)._ 
```cpp
inline const_reverse_iterator gl::flat_jagged_vector::crend () noexcept const
```





**Returns:**

Const reverse iterator one position before the first segment. 
> [!NOTE] Iterator invalidated by structural modifications. 
>
>







        

<hr>



### function push\_back [1/3]

_Appends a new segment to the end of the container._ 
```cpp
template<std::ranges::input_range R>
inline void gl::flat_jagged_vector::push_back (
    R && r
) 
```



This method efficiently adds a segment from any input range. If the range has a known size, appropriate pre-allocation is performed.




**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `r` The range to append as a new segment. 




#### Postconditions




1. `size()` is incremented by 1
2. `data_size()` increases by the range size. 

**Exception:**


    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > Amortized [**$O(N)**](group__GL-Core.md#variable-invalid_id_v)$ where $N$ is the size of the pushed range. 
    >
    >

    > [!NOTE] Exception safety
    >
    >
    > Provides strong exception guarantee: if an exception occurs, the internal _offsets_ and _data_ remain synchronized and the container is unchanged. 
    >
    >

    > [!WARNING] Invalidates all iterators, pointers, and references to elements if reallocation occurs. 
    >
    >






        

<hr>



### function push\_back [2/3]

_Appends a segment from an initializer list._ 
```cpp
inline void gl::flat_jagged_vector::push_back (
    std::initializer_list< value_type > ilist
) 
```





**Parameters:**


* `ilist` The initializer list to append as a segment. 



**Postcondition:**

`size()` is incremented by 1; `data_size()` increases by the list size. 




**Exception:**


* `std::bad_alloc` If memory allocation fails.


> [!INFO] Implementation
>
>
> This function is equivalent to calling `push_back(std::span<const value_type>{ilist})`. 
>
>



        

<hr>



### function pop\_back [1/2]

_Removes the last segment from the container._ 
```cpp
inline void gl::flat_jagged_vector::pop_back () 
```





**Postcondition:**

If container was not empty, `size()` is decremented by 1 and `data_size()` decreases by the size of the removed segment. If empty, this function has no effect.



> [!INFO] Time Complexity
>
>
> [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$ amortized for the container overhead, plus [**$O(N)**](group__GL-Core.md#variable-invalid_id_v)$ to truncate the underlying _data_ vector where N is the size of the removed segment. 
>
>

> [!NOTE] Safety
>
>
> A call to `pop_back()` on an empty container is safe (no-op). 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to elements in the last segment. 
>
>



        

<hr>



### function insert [1/3]

_Inserts a new segment at the specified position._ 
```cpp
template<std::ranges::input_range R>
inline void gl::flat_jagged_vector::insert (
    size_type pos,
    R && r
) 
```



This method inserts a range as a new segment at the given position, shifting all subsequent segments and updating their offsets accordingly.




**Template parameters:**


* `R` An input range of elements convertible to `value_type`. 



**Parameters:**


* `pos` The position where the segment will be inserted (must satisfy `pos <=  size()`). 
* `r` The range to insert as a segment. 




#### Postconditions




1. `size()` is incremented by 1
2. Segments at and after `pos` are shifted and offsets are updated. 

**Exception:**


    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > $O(N + M)$ where $N$ is the total number of elements from `pos` onward and $M$ is the size of the inserted range, due to element shifting and offset updates. Insertion at the end is amortized [**$O(M)**](group__GL-Core.md#variable-invalid_id_v)$. 
    >
    >

    > [!NOTE] Exception safety
    >
    >
    > Provides strong exception guarantee: if an exception occurs, the internal _offsets_ and _data_ remain synchronized and the container is unchanged. 
    >
    >

    > [!WARNING] Invalidates all iterators, pointers, and references to elements after the insertion point. 
    >
    >






        

<hr>



### function insert [2/3]

_Inserts a segment from an initializer list at the specified position._ 
```cpp
inline void gl::flat_jagged_vector::insert (
    size_type pos,
    std::initializer_list< value_type > ilist
) 
```





**Parameters:**


* `pos` The position where the segment will be inserted (must satisfy `pos <=  size()`). 
* `ilist` The initializer list to insert as a segment. 



**Exception:**


* `std::bad_alloc` If memory allocation fails.


> [!INFO] Implementation
>
>
> This function is equivalent to calling `insert(pos, std::span<const value_type>{ilist})`. 
>
>



        

<hr>



### function erase [1/2]

_Erases the segment at the specified position._ 
```cpp
inline void gl::flat_jagged_vector::erase (
    size_type pos
) 
```



This method removes a segment and all subsequent segments are shifted backward, with their offsets updated accordingly.




**Parameters:**


* `pos` The position of the segment to erase (must satisfy `pos <  size()`). 




#### Postconditions




1. The segment at `pos` is removed.
2. `size()` is decremented by 1.





> [!INFO] Time Complexity
>
>
> $O(E + S + L)$ where $E$ is the number of elements after the erased segment in the underlying vector, $S$ is the number of segments after `pos`, and $L$ is the size of the erased segment. Erasing the **last** segment is [**$O(L)**](group__GL-Core.md#variable-invalid_id_v)$. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to elements at or after the erased position. 
>
>




        

<hr>



### function push\_back [3/3]

_Appends a copy of an element to the end of a specific segment._ 
```cpp
inline void gl::flat_jagged_vector::push_back (
    size_type seg,
    const  value_type & value
) 
```





**Parameters:**


* `seg` The segment number where the element will be appended. 
* `value` The value to append. 




#### Postconditions




1. The segment size increases by 1.
2. `data_size()` increases by 1. 

**Exception:**


    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point in the underlying vector, and $S$ is the number of segments after `seg`. Appending to the **last** segment is amortized [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$. 
    >
    >

    > [!WARNING] Iterator Invalidation
    >
    >
    > If reallocation occurs, all iterators, pointers, and references are invalidated. Otherwise, only those pointing to elements at or after the insertion point are invalidated. 
    >
    >






        

<hr>



### function emplace\_back 

_Constructs an element in-place at the end of a specific segment._ 
```cpp
template<class... Args>
inline void gl::flat_jagged_vector::emplace_back (
    size_type seg,
    Args &&... args
) 
```





**Template parameters:**


* `Args` Perfect forwarding types for the constructor of `T`. 



**Parameters:**


* `seg` The segment number where the element will be constructed. 
* `args` Arguments to forward to the `T` constructor. 




#### Postconditions




1. The segment size increases by 1.
2. `data_size()` increases by 1. 

**Exception:**


    * `Any` exception thrown by the `T` constructor, or std::bad\_alloc.


    > [!INFO] Time Complexity
    >
    >
    > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point in the underlying vector, and $S$ is the number of segments after `seg`. Appending to the **last** segment is amortized [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$. 
    >
    >

    > [!WARNING] Iterator Invalidation
    >
    >
    > If reallocation occurs, all iterators, pointers, and references are invalidated. Otherwise, only those pointing to elements at or after the insertion point are invalidated. 
    >
    >






        

<hr>



### function pop\_back [2/2]

_Removes the last element from a specific segment._ 
```cpp
inline void gl::flat_jagged_vector::pop_back (
    size_type seg
) 
```





**Parameters:**


* `seg` The segment number from which to remove the last element. 



**Postcondition:**

If the segment was not empty, its size decreases by 1 and `data_size()` decreases by 1. If empty, this function has no effect.



> [!INFO] Time Complexity
>
>
> $O(E + S)$ where $E$ is the number of elements after the removed element in the underlying vector, and $S$ is the number of segments after `seg`. Popping from the **last** segment is [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to elements at or after the removed element. 
>
>



        

<hr>



### function insert [3/3]

_Inserts an element at a specific position within a segment._ 
```cpp
inline void gl::flat_jagged_vector::insert (
    size_type seg,
    size_type pos,
    const  value_type & value
) 
```





**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment where the element will be inserted. 
* `value` value to insert. 




#### Postconditions




1. The segment size increases by 1.
2. `data_size()` increases by 1. 

**Exception:**


    * `std::bad_alloc` If memory allocation fails.


    > [!INFO] Time Complexity
    >
    >
    > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point in the underlying vector, and $S$ is the number of segments after `seg`. 
    >
    >

    > [!WARNING] Iterator Invalidation
    >
    >
    > If reallocation occurs, all iterators, pointers, and references are invalidated. Otherwise, only those pointing to elements at or after the insertion point are invalidated. 
    >
    >






        

<hr>



### function emplace 

_Constructs an element in-place at a specific position within a segment._ 
```cpp
template<class... Args>
inline void gl::flat_jagged_vector::emplace (
    size_type seg,
    size_type pos,
    Args &&... args
) 
```





**Template parameters:**


* `Args` Perfect forwarding types for the constructor of `T`. 



**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment. 
* `args` Arguments to forward to the `T` constructor. 




#### Postconditions




1. The segment size increases by 1.
2. `data_size()` increases by 1. 

**Exception:**


    * `Any` exception thrown by the `T` constructor, or std::bad\_alloc.


    > [!INFO] Time Complexity
    >
    >
    > Amortized $O(E + S)$ where $E$ is the number of elements after the insertion point in the underlying vector, and $S$ is the number of segments after `seg`. 
    >
    >

    > [!WARNING] Iterator Invalidation
    >
    >
    > If reallocation occurs, all iterators, pointers, and references are invalidated. Otherwise, only those pointing to elements at or after the insertion point are invalidated. 
    >
    >






        

<hr>



### function erase [2/2]

_Removes an element at a specific position within a segment._ 
```cpp
inline void gl::flat_jagged_vector::erase (
    size_type seg,
    size_type pos
) 
```





**Parameters:**


* `seg` The segment number. 
* `pos` The position within the segment of the element to remove. 



**Postcondition:**

The segment size decreases by 1; `data_size()` decreases by 1; offsets updated.



> [!INFO] Time Complexity
>
>
> $O(E + S)$ where $E$ is the number of elements after the erased position in the underlying vector, and $S$ is the number of segments after `seg`. 
>
>

> [!WARNING] Invalidates all iterators, pointers, and references to elements at or after the removed position. 
>
>



        

<hr>



### function resize [4/5]

_Resizes a specific segment to contain_ `n` _elements._
```cpp
inline void gl::flat_jagged_vector::resize (
    size_type seg,
    size_type n
) 
```




* If the segment's current size is greater than `n`, it is reduced to its first `n` elements.
* If the segment's current size is less than `n`, additional default-inserted elements are appended.
* If the segment's current size is equal to `n`, the segment is unchanged.






**Parameters:**


* `seg` The segment number to resize. 
* `n` The new size for the segment. 



**Precondition:**

`seg <  size()` 




**Exception:**


* `std::bad_alloc` If memory allocation fails during growth.


> [!INFO] Implementation
>
>
> This function is equivalent to calling `resize(seg, n, value_type{})`. 
>
>



        

<hr>



### function resize [5/5]

_Resizes a specific segment to contain_ `n` _elements, initializing new elements with_`value` _._
```cpp
inline void gl::flat_jagged_vector::resize (
    size_type seg,
    size_type n,
    const  value_type & value
) 
```




* If the segment's current size is greater than `n`, it is reduced to its first `n` elements.
* If the segment's current size is less than `n`, additional copies of `value` are appended.
* If the segment's current size is equal to `n`, the segment is unchanged.






**Parameters:**


* `seg` The segment number to resize. 
* `n` The new size for the segment. 
* `value` The value to initialize new elements with. 



**Precondition:**

`seg <  size()` 




**Exception:**


* `std::bad_alloc` If memory allocation fails during growth.


> [!INFO] Time Complexity
>
>
> Amortized $O(E + S + K)$ where $E$ is the number of elements after the modification point in the underlying vector, $S$ is the number of segments after `seg`, and $K$ is the number of elements added or removed. 
>
>

> [!WARNING] No bounds checking. Results in Undefined Behavior if segment index is out of bounds. 
>
>

> [!WARNING] Iterator Invalidation
>
>
> If reallocation occurs, all iterators, pointers, and references are invalidated. Otherwise, only those pointing to elements at or after the insertion point are invalidated. 
>
>



        

<hr>## Friends Documentation





### friend operator== 

_Tests equality of two_ `flat_jagged_vector` _instances._
```cpp
bool gl::flat_jagged_vector::operator== (
    const  flat_jagged_vector &,
    const  flat_jagged_vector &
) = default
```





**Parameters:**


* `lhs` Left operand. 
* `rhs` Right operand. 



**Returns:**

`true` if both vectors have the same structure and elements. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/types/flat_jagged_vector.hpp`

