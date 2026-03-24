# Core utility types

This section describes a set of fundamental types provided by the library. While these types are essential components within the library itself, they are designed to be highly reusable, making them valuable even in non-graph-related code where similar functionality is required.

<br />

## Table of content

- [Common type traits](#common-type-traits)
- [General type aliases](#general-type-aliases)
- [Graph element property types](#graph-element-property-types)
- [I/O types](#io-types)

<br />
<br />

## Common type traits

The library relies heavily on concepts and type traits evaluated at compile-time to make the code more robust and readable and to reduce some of the overhead associated with abstraction.

The common concepts and type traits used in the library are defined in the `gl::traits` namespace and most of them are declared in the [gl/traits.hpp](/include/gl/traits.hpp) header file.

<br />
<br />

## General type aliases

The table below contains the basic type aliases defined in the library.

| **Type alias** | **Underlying type** |
| :- | :- |
| `size_type` | `std::uint64_t` |
| `id_type` | `size_type` |
| `homogeneous_pair<T>` | `std::pair<T, T>` |

> [!NOTE]
> All types in the table above are defined in the [gl/types/core.hpp](/include/gl/types/core.hpp) header file.

<br />
<br />

## Graph element property types

> [!NOTE]
> All types listed in this section are defined in the [gl/types/properties.hpp](/include/gl/types/properties.hpp) header file.

### Basic property types

- `empty_properties` - An alias for `std::monostate` representing the default properties type which holds no data.

### `struct name_property`

- *Description*:
  The `name_property` struct is a simple wrapper around a `std::string`, designed to store and manage an element's name.

- *Type definitions*:
  - `value_type` - An alias for `std::string`, representing the type of the name stored in the property.

- *Assignment operators*:
  - `operator=(std::string_view name) -> name_property&` - Assigns a new name to the property, allowing for efficient assignment from string literals and `std::string_view`.

- *Member functions*:
  - `operator==(const name_property&) const -> bool` - Equality operator (*default*).
  - `operator<=>(const name_property&) const -> auto` - Three-way comparison operator (*default*).
  - `operator==(std::string_view name) const -> bool` - Compares the property name with a string view for equality.
  - `operator<=>(std::string_view name) const -> auto` - Compares the property name with a string view for ordering.

- *Friend functions*:
  - `operator<<(std::ostream&, const name_property&) -> std::ostream&` - Outputs the `name_property`'s name to an output stream, **enclosed in quotes**.
  - `operator>>(std::istream&, name_property&) -> std::istream&` - Inputs a `name_property`'s name from an input stream, **expecting a quoted string**.

### `class dynamic_properties`

- *Description*:
  The `dynamic_properties` class provides a flexible mechanism for storing key-value pairs where the values can be of any type. It uses `std::any` to accommodate different value types and allows dynamic addition, retrieval, and removal of properties.

- *Type definitions*:
  - `key_type` - An alias for `std::string`, representing the type of keys used in the property map.
  - `value_type` - An alias for `std::any`, representing the type of values stored in the property map.
  - `property_map_type` - An alias for `std::unordered_map<key_type, value_type>`, representing the underlying container used for storing properties.

- *Constructors*:
  - `dynamic_properties()` - Default constructor (*default*).
  - `dynamic_properties(const dynamic_properties&)` - Copy constructor (*default*).
  - `dynamic_properties(dynamic_properties&&)` - Move constructor (*default*).

- *Assignment operators*:
  - `operator=(const dynamic_properties&) -> dynamic_properties&` - Copy assignment operator (*default*).
  - `operator=(dynamic_properties&&) -> dynamic_properties&` - Move assignment operator (*default*).

- *Destructor*:
  - `~dynamic_properties()` - (*default*).

- *Member functions*:
  - `is_present(const key_type& key) const -> bool` - Checks if a property identified by `key` is present in the property map.
  - `get(const key_type& key) -> ValueType&`
    - *Template parameters*:
      - `ValueType` - The type to which the stored value will be cast.
    - Retrieves the value associated with `key`, casting it to the specified type.
  - `set(const key_type& key, const ValueType& value)`
    - *Template parameters*:
      - `ValueType` - The type of the value to be stored.
    - Sets the value for the specified `key`, allowing copy construction of the value.
  - `mvset(const key_type& key, ValueType&& value)`
    - *Template parameters*:
      - `ValueType` - The type of the value to be stored.
    - Sets the value for the specified `key`, allowing move construction of the value.
  - `emplace(const key_type& key, Args&&... args)`
    - *Template parameters*:
      - `ValueType` - The type of the value to be constructed and inserted.
      - `Args` - The types of the arguments forwarded to construct the value.
    - Constructs and inserts a value into the property map associated with `key`, forwarding the arguments.
  - `remove(const key_type& key)` - Removes the property identified by `key` from the property map.
  - `underlying() -> property_map_type&` - Returns a reference to the underlying property map.

### `class binary_color`

- *Description*:
  The `binary_color` class represents a color that can be either black or white. It also allows for creating a color with an unset state/value.

- *Type definitions*:
  - `value` - An enumeration representing the possible colors:
    - `black` - Represents the color black.
    - `white` - Represents the color white.
    - `unset` - Represents an unset state (neither black nor white).

- *Constructors*:
  - `binary_color()` - Default constructor (*default*).
  - `binary_color(value value)` - Initializes the binary color with the specified `value`.

- *Assignment operators*:
  - `operator=(const binary_color&) -> binary_color&` - Copy assignment operator (*default*).
  - `operator=(binary_color&&) -> binary_color&` - Move assignment operator (*default*).
  - `operator=(value value) -> binary_color&` - Assigns a new value to the binary color, restricting it to valid values.

- *Destructor*:
  - `~binary_color()` - (*default*).

- *Member functions*:
  - `operator bool() const` - Converts the binary color to a boolean, indicating whether its state/value is set.
  - `is_set() const -> bool` - Checks if the color's state/value is set.
  - `to_underlying() const -> std::underlying_type_t<value>` - Returns the underlying integer value of the current color.
  - `next() const -> binary_color` - Returns the next color in the sequence (unset -> black -> white -> black -> ...).

- *Comparison operators*:
  - `operator<=>(const binary_color&) const` - Three-way comparison operator (*default*).
  - `operator==(const binary_color&) const -> bool` - Equality operator (*default*).

- *Associated type definitions*:
  - `gl::bin_color_value = typename binary_color::value`

### `struct binary_color_property`

- *Description*:
  The `binary_color_property` structure encapsulates a `binary_color`, allowing it to be associated with other elements.

- *Type definitions*:
  - `color_type` - An alias for `binary_color`, representing the type of color stored in the property.

- *Member variables*:
  - `color: color_type` - The binary color associated with this property.

### `struct weight_property`

- *Description*:
  The `weight_property` struct represents a property that holds a weight value of a specified arithmetic type. It provides functionality for input and output operations, making it easy to read and write the weight to and from streams.

- *Template parameters*:
  - `WeightType` - The type of the weight, constrained to basic arithmetic types (default is `double`).

- *Type definitions*:
  - `weight_type` - An alias for `WeightType`, representing the type of the weight stored in the property.

- *Member variables*:
  - `weight: weight_type` - The weight value, initialized to `1` by default.

- *Friend functions*:
  - `operator<<(std::ostream&, const weight_property&)`
    - Writes the weight property to an output stream.
    - *Constraints*: `weight_type` must be writable (`traits::c_writable<weight_type>`).

  - `operator>>(std::istream&, weight_property&)`
    - Reads the weight property from an input stream.
    - *Constraints*: `weight_type` must be readable (`traits::c_readable<weight_type>`).

### Associated type traits

This section describes the type traits that are associated with the property types defined in the library. These traits help ensure that properties meet specific requirements and can be used correctly within the library.

> [!NOTE]
> All type traits listed in this subsection are defined in the `gl::traits` namespace

- `c_properties<T>`
  - *Description*: A concept that checks if a given type `T` is a valid properties type, which means that it is semiregular, move constructible, and assignable from a const reference to itself.

  ```cpp
  template <typename T>
  concept c_properties =
      std::semiregular<T> and std::move_constructible<T> and std::assignable_from<T&, const T&>;
  ```

- `c_empty_properties<T>`
  - *Description*: A concept that checks if a given type `T` is an empty properties type - `empty_properties = std::monostate`.

  ```cpp
  template <typename T>
  concept c_empty_properties = c_properties<T> and std::same_as<T, gl::empty_properties>;
  ```

- `c_non_empty_properties<T>`
  - *Description*: A concept that checks if a given type `T` is a non-empty properties type.

  ```cpp
  template <typename T>
  concept c_non_empty_properties = c_properties<T> and not c_empty_properties<T>;
  ```

- `c_has_empty_properties<T>`
  - *Description*: A concept that checks if a given type `T` has an `empty_properties` type alias.

  ```cpp
  template <typename T>
  concept c_has_empty_properties =
      requires { typename T::properties_type; } and c_empty_properties<typename T::properties_type>;
  ```

- `c_has_non_empty_properties<T>`
  - *Description*: A concept that checks if a given type `T` has a `non_empty_properties` type alias.

  ```cpp
  template <typename T>
  concept c_has_non_empty_properties = requires {
      typename T::properties_type;
  } and not c_empty_properties<typename T::properties_type>;
  ```

- `c_binary_color_properties_type<Properties>`
  - *Description*: A concept that checks if a given `Properties` type is a valid binary color properties type.

  ```cpp
  template <typename Properties>
  concept c_binary_color_properties_type = c_properties<Properties> and requires(Properties p) {
      typename Properties::color_type;
      { p.color } -> std::same_as<typename Properties::color_type&>;
      { p.color == binary_color{} } -> std::convertible_to<bool>;
      requires std::constructible_from<typename Properties::color_type, binary_color>;
  };
  ```

- `c_weight_properties_type<Properties>`
  - *Description*: A concept that checks if a given `Properties` type is a valid weight properties type.

  ```cpp
  template <typename Properties>
  concept c_weight_properties_type = c_properties<Properties> && requires(Properties p) {
      typename Properties::weight_type;
      { p.weight } -> std::same_as<typename Properties::weight_type&>;
      requires c_arithmetic<typename Properties::weight_type>;
  };
  ```

<br />
<br />

## I/O types

This section provides the description of I/O types and utility defined in the library.

> [!NOTE]
> All elements described in this section are defined in the `gl::io` namespace.

### General type definitions

| **Type alias** | **Underlying type** |
| :- | :- |
| `index_type` | `int` |
| `iword_type` | `long` |
| `bit_position_type` | `unsigned` |

> [!NOTE]
> The library also defines the `inline constexpr iword_type iword_bit = 1ul;` value alias.

<br />

## `class stream_options_manipulator`

- *Description*:
  The `stream_options_manipulator` class is designed to manage stream options using bitwise operations. It allows setting and unsetting specific options in streams through bit manipulation on the `iword` attribute of a stream at a specifically allocated index.

- *Constructors*:
  - `stream_options_manipulator()` - Default constructor (*deleted*).
  - `explicit stream_options_manipulator(iword_type bitmask, bool operation = set)` - Initializes the manipulator with a bitmask and an operation (set or unset).

- *Member functions*:
  - `static from_bit_position(bit_position, operation = set)`
    - *Description*: Creates a `stream_options_manipulator` instance from a bit position value.
    - *Parameters*:
      - `bit_position: bit_position_type`: The bit position to set or unset.
      - `operation: bool` (default = `set`): The operation (set or unset).
    - *Return type*: `stream_options_manipulator`.

  - `friend operator<<(os, property_manipulator)`
    - *Description*: Overloads the output stream operator to set/unset options in an output stream.
    - *Parameters*:
      - `os: std::ostream&`: The output stream.
      - `property_manipulator: const stream_options_manipulator&`: The manipulator object.
    - *Return type*: `std::ostream&`.

  - `friend operator>>(is, property_manipulator)`
    - *Description*: Overloads the input stream operator to set/unset options in an input stream.
    - *Parameters*:
      - `is: std::istream&`: The input stream.
      - `property_manipulator: const stream_options_manipulator&`: The manipulator object.
    - *Return type*: `std::istream&`.

  - `is_option_set(stream, bit_position)`
    - *Description*: Checks if a specific option is set in the given stream.
    - *Parameters*:
      - `stream: std::ios_base&`: The stream to check.
      - `bit_position: bit_position_type`: The bit position of the option.
    - *Return type*: `bool`.

  - `are_options_set(stream, bitmask)`
    - *Description*: Checks if all options specified by the bitmask are set in the given stream.
    - *Parameters*:
      - `stream: std::ios_base&`: The stream to check.
      - `bitmask: iword_type`: The bitmask representing the options.
    - *Return type*: `bool`.

- *Constants*:
  - `static constexpr bool set = true;` - Represents the set operation.
  - `static constexpr bool unset = false;` - Represents the unset operation.

### Associated utility functions

#### Option setters

- `set_options(bitmask)`
  - *Description*: Creates a `stream_options_manipulator` to set options specified by the bitmask.
  - *Parameters*:
    - `bitmask: iword_type`: The bitmask representing the options to set.
  - *Return type*: `stream_options_manipulator`.

- `set_options(bit_positions)`
  - *Description*: Creates a `stream_options_manipulator` to set options from a list of bit positions.
  - *Parameters*:
    - `bit_positions: const std::initializer_list<bit_position_type>&`: The list of bit positions.
  - *Return type*: `stream_options_manipulator`.

- `set_options(bit_positions)`
  - *Description*: Creates a `stream_options_manipulator` to set options from a list of enumerated bit positions.
  - *Template parameters*:
    - `BitPosition: detail::c_bit_position_enum BitPosition` - The bit position enum type
  - *Parameters*:
    - `bit_positions: const std::initializer_list<BitPosition>&`: The list of enumerated bit positions.
  - *Return type*: `stream_options_manipulator`.

- `set_option(bit_position)`
  - *Description*: Creates a `stream_options_manipulator` to set a specific option.
  - *Parameters*:
    - `bit_position: bit_position_type`: The bit position of the option to set.
  - *Return type*: `stream_options_manipulator`.

- `set_option(bit_position)`
  - *Description*: Creates a `stream_options_manipulator` to set a specific enumerated option.
  - *Template parameters*:
    - `BitPosition: detail::c_bit_position_enum BitPosition` - The bit position enum type
  - *Parameters*:
    - `bit_position`: The enumerated bit position of the option to set.
  - *Return type*: `stream_options_manipulator`.

#### Option unsetters

- `unset_options(bitmask)`
  - *Description*: Creates a `stream_options_manipulator` to unset options specified by the bitmask.
  - *Parameters*:
    - `bitmask: iword_type`: The bitmask representing the options to unset.
  - *Return type*: `stream_options_manipulator`.

- `unset_options(bit_positions)`
  - *Description*: Creates a `stream_options_manipulator` to unset options from a list of bit positions.
  - *Parameters*:
    - `bit_positions: const std::initializer_list<bit_position_type>&`: The list of bit positions.
  - *Return type*: `stream_options_manipulator`.

- `unset_options(bit_positions)`
  - *Description*: Creates a `stream_options_manipulator` to unset options from a list of enumerated bit positions.
  - *Template parameters*:
    - `BitPosition: detail::c_bit_position_enum BitPosition` - The bit position enum type
  - *Parameters*:
    - `bit_positions: const std::initializer_list<BitPosition>&`: The list of enumerated bit positions.
  - *Return type*: `stream_options_manipulator`.

- `unset_option(bit_position)`
  - *Description*: Creates a `stream_options_manipulator` to unset a specific option.
  - *Parameters*:
    - `bit_position: bit_position_type`: The bit position of the option to unset.
  - *Return type*: `stream_options_manipulator`.

- `unset_option(bit_position)`
  - *Description*: Creates a `stream_options_manipulator` to unset a specific enumerated option.
  - *Template parameters*:
    - `BitPosition: detail::c_bit_position_enum BitPosition` - The bit position enum type
  - *Parameters*:
    - `bit_position`: The enumerated bit position of the option to unset.
  - *Return type*: `stream_options_manipulator`.

### Option query fuctions

- `are_options_set(stream, bitmask)`
  - *Description*: Checks if all options specified by the bitmask are set in the given stream.
  - *Parameters*:
    - `stream: std::ios_base&`: The stream to check.
    - `bitmask: iword_type`: The bitmask representing the options.
  - *Return type*: `bool`.

- `are_options_set(stream, bit_positions)`
  - *Description*: Checks if all options specified by the bit positions are set in the given stream.
  - *Parameters*:
    - `stream: std::ios_base&`: The stream to check.
    - `bit_positions: const std::initializer_list<bit_position_type>&`: The list of bit positions.
  - *Return type*: `bool`.

- `are_options_set(stream, bit_positions)`
  - *Description*: Checks if all options specified by the enumerated bit positions are set in the given stream.
  - *Template parameters*:
    - `BitPosition: detail::c_bit_position_enum BitPosition` - The bit position enum type
  - *Parameters*:
    - `stream: std::ios_base&`: The stream to check.
    - `bit_positions: const std::initializer_list<BitPosition>&`: The list of enumerated bit positions.
  - *Return type*: `bool`.

- `is_option_set(stream, bit_position)`
  - *Description*: Checks if a specific option is set in the given stream.
  - *Parameters*:
    - `stream: std::ios_base&`: The stream to check.
    - `bit_position: bit_position_type`: The bit position of the option.
  - *Return type*: `bool`.

- `is_option_set(stream, bit_position)`
  - *Description*: Checks if a specific enumerated option is set in the given stream.
  - *Template parameters*:
    - `BitPosition: detail::c_bit_position_enum BitPosition` - The bit position enum type
  - *Parameters*:
    - `stream: std::ios_base&`: The stream to check.
    - `bit_position: BitPosition`: The enumerated bit position of the option.
  - *Return type*: `bool`.
