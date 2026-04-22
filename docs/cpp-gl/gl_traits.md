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
