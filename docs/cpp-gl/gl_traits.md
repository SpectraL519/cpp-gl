# GL Traits & Concepts Documentation {: #gl-traits-concepts-documentation }

This page documents the C++20 concepts and type traits used to constrain templates across the GL library.

> [!IMPORTANT]
> This page is maintained manually and separately from the auto-generated API reference as the [MkDoxy](https://github.com/JakubAndrysek/MkDoxy) tool does not currently support generating documentation for concepts.

---

## **gl::traits::c_id_type** {: #gl-traits-c-id-type }

**Module:** Part of the [GL-Traits](group__GL-Traits.md) group.

Concept defining the requirements for an identifier type.

### Detailed Description

Ensures that any custom ID type provided to the graph library is an unsigned integral type to guarantee safe indexing and representation.

### Template Parameters

| Parameter | Description |
| :--- | :--- |
| `T` | The type to evaluate against the concept. |

### Definition

```cpp
template <typename T>
concept c_id_type = std::unsigned_integral<T>;
```

---

## **gl::traits::c_properties** {: #gl-traits-c-properties }

**Module:** Part of the [GL-Traits](group__GL-Traits.md) group.

Defines the minimal requirements for a type to be used as a property.

### Detailed Description

A valid property type must be **semiregular** (default constructible and copyable).

### Template Parameters

| Parameter | Description |
| :--- | :--- |
| `T` | The type to evaluate against the concept. |

### Definition

```cpp
template <typename T>
concept c_properties = std::semiregular<T>;
```

---

## **gl::traits::c_empty_properties** {: #gl-traits-c-empty-properties }

**Module:** Part of the [GL-Traits](group__GL-Traits.md) group.

Validates if a type is specifically the `gl::empty_properties` tag.

### Detailed Description

This concept is used to specialize behavior for graph components that do not carry any user-defined data.
