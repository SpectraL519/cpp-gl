# Concepts API Reference {: #concepts-api-reference }

This page serves as the central index for all C++20 concepts used across the library to enforce type safety and template constraints.

> [!IMPORTANT]
> This page is maintained manually and separately from the auto-generated API reference as the [MkDoxy](https://github.com/JakubAndrysek/MkDoxy) tool does not currently support generating documentation for concepts.

---

## Graph Library (GL) Concepts

The Graph Library (GL) module relies on concepts to validate identifier types and property structures at compile time.

- **[GL Traits & Concepts Documentation](gl_traits.md)**: Full API reference for the GL module.
    - [`gl::traits::c_id_type`](gl_traits.md#gl-traits-c-id-type): Concept defining the requirements for an identifier type.
