# Additional functionality

- [Force inlining](#force-inlining)
- [Ranges utility](#ranges-utility)

<br />

## Force inlining

The `CPP-GL` library provides an option to enable force inlining of seleced functions and methods in the implementation. This is defined through the `gl_attr_force_inline` macro, which by default is equivalent to the `inline` keyword.

To enable the force inlining functionality you have to add the following in your program:

```c++
#define GL_CONFIG_FORCE_INLINE
```

> [!NOTE]
> Force inlining is supported only for the GNU G++ and CLang++ compilers.

<br />
<br />

## Ranges utility

Small helpers for working with C++20 ranges.

> [!NOTE]
> - Header: [gl/util/ranges.hpp](/include/gl/util/ranges.hpp)
> - Namespace: `gl::util`

### View adapters

- **`deref_view`**
	- *Description*: A transform view that dereferences pointer-like elements in a range. Useful for turning a range of pointers or smart pointers into a range of references.
	- *Usage*: `range | gl::util::deref_view`
	- *Works with*: `T*`, `std::unique_ptr<T>`, `std::shared_ptr<T>`, and generally pointer-like objects supporting unary `*`.
	- *Return type*: A lazy transformed view whose reference type is `decltype(*p)` of the underlying elements.
	- *Example*:
		```cpp
		std::vector<std::unique_ptr<int>> v; /* ... */
		for (int& x : v | gl::util::deref_view) {
				// use x as an int&
		}
		```

### Functions

- **`range_size(r)`**
	- *Template parameters*:
		- `R: std::ranges::range`
	- *Description*: Returns the size of a range. If `R` models `std::ranges::sized_range`, it forwards to `std::ranges::size(r)` in O(1). Otherwise, it computes `std::ranges::distance(std::begin(r), std::end(r))` in O(n).
	- *Parameters*:
		- `r: R&&` – the input range.
	- *Return type*: Same type as produced by `std::ranges::size(r)` for sized ranges or `std::ranges::distance(...)` otherwise.
	- *Complexity*: O(1) for sized ranges; O(n) otherwise.
	- *Exceptions*: No specific exceptions; propagates iterator operations if they throw.

> [!CAUTION]
> For non-sized, single-pass/input ranges `range_size` will consume the range when computing distance.
