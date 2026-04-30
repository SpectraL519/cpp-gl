

# Group GL-Util



[**Modules**](modules.md) **>** [**GL-Util**](group__GL-Util.md)



_Practical, domain-agnostic C++ helpers, mathematical functions, and polyfills._ [More...](#detailed-description)
















## Classes

| Type | Name |
| ---: | :--- |
| struct | [**gl::util::concat\_fn**](structgl_1_1util_1_1concat__fn.md) <br>_A function object for concatenating two viewable ranges into a_ `concat_view` _._ |
| class | [**gl::util::concat\_view**](classgl_1_1util_1_1concat__view.md) &lt;V1, V2&gt;<br>_A view concatenating two ranges sequentially (C++20 polyfill for C++26_ `std::views::concat` _)._ |






## Public Attributes

| Type | Name |
| ---: | :--- |
|  constexpr concat\_fn | [**concat**](#variable-concat)  = `{}`<br>_Concatenates two viewable ranges into a_ `concat_view` _._ |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  constexpr bool | [**all\_equal**](#function-all_equal) (R && range, const std::ranges::range\_value\_t&lt; R &gt; & value) noexcept<br>_Checks if all elements in a range are equal to a given value._  |
|  constexpr bool | [**is\_constant**](#function-is_constant) (R && range) noexcept<br>_Checks if all elements in a range are equal._  |
|  constexpr auto | [**range\_size**](#function-range_size) (R && r) <br>_Safely determines the size of a range._  |
|  constexpr std::ptrdiff\_t | [**to\_diff**](#function-to_diff) (std::integral auto i) noexcept<br>_Converts an integral value to a standard pointer difference type._  |
|  constexpr size\_type | [**to\_idx**](#function-to_idx) (traits::c\_id\_type auto id) noexcept<br>_Converts a valid identifier to a standard size type (index)._  |
|  constexpr size\_type | [**upow**](#function-upow) (size\_type base, size\_type exp) <br>_Computes the value of_ `base` _raised to the power of_`exp` _using exponentiation by squaring._ |
|  constexpr size\_type | [**upow\_sum**](#function-upow_sum) (const size\_type base, size\_type i\_begin, size\_type i\_end) <br>_Computes the sum of powers of_ `base` _from_`base^i_begin` _to_`base^i_end` _inclusive._ |




























## Detailed Description


While originally engineered to support the library's internal algorithms and graph modeling operations, these utilities are completely independent of graph theory semantics. They provide a curated set of highly reusable, general-purpose tools that can smoothly integrate into any modern C++ codebase. 


    
## Public Attributes Documentation




### variable concat 

_Concatenates two viewable ranges into a_ `concat_view` _._
```cpp
constexpr concat_fn gl::util::concat = {};
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




* [**concat\_view**](classgl_1_1util_1_1concat__view.md): The view type that represents the concatenation of two ranges.
* [**concat\_fn**](structgl_1_1util_1_1concat__fn.md): A helper compile-time constant function object for creating `concat_view` instances. 





        

<hr>
## Public Functions Documentation




### function all\_equal 

_Checks if all elements in a range are equal to a given value._ 
```cpp
template<std::ranges::forward_range R>
constexpr bool all_equal (
    R && range,
    const std::ranges::range_value_t< R > & value
) noexcept
```





**Template parameters:**


* `R` The type of the range. 



**Parameters:**


* `range` The range to check. 
* `value` The value to compare against. 



**Returns:**

`true` if all elements in the range are equal to the given value or the range is empty, otherwise `false`.



> [!INFO] Time Complexity
>
>
> [**$O(N)**](group__GL-Core.md#variable-invalid_id_v)$ where $N$ is the number of elements in the range. 
>
>



        

<hr>



### function is\_constant 

_Checks if all elements in a range are equal._ 
```cpp
template<std::ranges::forward_range R>
constexpr bool is_constant (
    R && range
) noexcept
```





**Template parameters:**


* `R` The type of the range. 



**Parameters:**


* `range` The range to check. 



**Returns:**

`true` if all elements in the range are equal or the range is empty, otherwise `false`.



> [!INFO] Time Complexity
>
>
> [**$O(N)**](group__GL-Core.md#variable-invalid_id_v)$ where $N$ is the number of elements in the range. 
>
>



        

<hr>



### function range\_size 

_Safely determines the size of a range._ 
```cpp
template<std::ranges::range R>
constexpr auto range_size (
    R && r
) 
```



This function returns the size of a range if it is a sized range, otherwise it computes the distance between the beginning and end of the range. Note that computing the distance for non-sized ranges may be expensive.



> [!WARNING] This function will consume input ranges that are not sized, as it needs to iterate through them to count the elements. 
>
>



**Template parameters:**


* `R` The type of the range. 



**Parameters:**


* `r` The range to measure. 



**Returns:**

The size of the range if it is a sized range, otherwise the distance between the beginning and end of the range.



> [!INFO] Time Complexity
>
>
> [**$O(1)**](group__GL-Core.md#variable-invalid_id_v)$ if the range is a sized range, otherwise [**$O(N)**](group__GL-Core.md#variable-invalid_id_v)$ where $N$ is the number of elements in the range. 
>
>



        

<hr>



### function to\_diff 

_Converts an integral value to a standard pointer difference type._ 
```cpp
constexpr std::ptrdiff_t to_diff (
    std::integral auto i
) noexcept
```



Useful for safe pointer arithmetic and offset calculations within graph data structures.




**Parameters:**


* `i` The integral value to convert. 



**Returns:**

The value statically cast to `std::ptrdiff_t`. 





        

<hr>



### function to\_idx 

_Converts a valid identifier to a standard size type (index)._ 
```cpp
constexpr size_type to_idx (
    traits::c_id_type auto id
) noexcept
```



Provides a safe, explicit cast from any unsigned integral ID type into a `size_type`, commonly used for array/vector lookups.




**Parameters:**


* `id` The identifier to convert. 



**Returns:**

The identifier statically cast to `size_type`. 





        

<hr>



### function upow 

_Computes the value of_ `base` _raised to the power of_`exp` _using exponentiation by squaring._
```cpp
inline constexpr size_type upow (
    size_type base,
    size_type exp
) 
```





**Parameters:**


* `base` The base value to be raised to a power. 
* `exp` The exponent to which the base is raised. 



**Returns:**

The result of `base` raised to the power of `exp`.



> [!INFO] Time Complexity
>
>
> $O(\log(\text{exp}))$ due to halving the exponent at each step. 
>
>



        

<hr>



### function upow\_sum 

_Computes the sum of powers of_ `base` _from_`base^i_begin` _to_`base^i_end` _inclusive._
```cpp
inline constexpr size_type upow_sum (
    const size_type base,
    size_type i_begin,
    size_type i_end
) 
```





**Parameters:**


* `base` The base value for the powers. 
* `i_begin` The starting exponent (inclusive). 
* `i_end` The ending exponent (inclusive). 



**Returns:**

The sum of `base^i` for all `i` in the range `[i_begin, i_end]`.



> [!INFO] Time Complexity
>
>
> \(O(\log(\text{max}(\text{i_begin}, \text{i_end})))\) due to the use of the closed-form formula for geometric series. 
>
>



        

<hr>

------------------------------


