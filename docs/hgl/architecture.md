# Architecture & Basics

## HGL Module Overview

While standard graphs define edges as simple connections between exactly two vertices, **hypergraphs** generalize this concept. In a hypergraph, a hyperedge can connect an arbitrary number of vertices simultaneously.

While higher-order connections can technically be modeled using standard bipartite graphs (a technique known as *star expansion*, where standard vertices are used to represent both the actual data elements and the connections between them), doing so pollutes the domain model. It forces developers to introduce artificial labels and boilerplate logic to distinguish between "true" vertices and "connection" vertices. This approach is not true to the natural structure of the relations and fundamentally complicates traversals, degree calculations, and property management.

The **HGL (Hypergraph Library)** module solves this by treating *both* vertices and hyperedges as true, first-class topological entities. By natively supporting n-ary relationships, HGL is exceptionally powerful for modeling complex, multi-way interactions directly as they exist in the real world—such as chemical reaction networks (multiple reactants forming multiple products), collaboration networks (multiple authors co-authoring a single research paper), or complex set systems and multi-way database joins.

This section explores the core architectural decisions of the HGL module:

- [Shared Infrastructure with GL](#shared-infrastructure-with-gl): Understand the strict unidirectional dependency and structural relationship between the GL and HGL modules.
- [Core Concepts](#core-concepts): Learn how the `hgl::hypergraph` template operates, how hyperedges differ mathematically from standard pairwise edges, and how to navigate generalized incidence.
- [Representation & Layouts](#representation-layouts): Discover the diverse memory models available for hypergraphs and how layout orientation impacts query performance.

---

## Shared Infrastructure with GL

The HGL module is built upon the exact same zero-cost abstraction philosophy as the standard GL module. It strictly utilizes C++20 concepts and template metaprogramming, meaning that all configuration is resolved at compile time.

Crucially, **the HGL module is built strictly on top of the GL module.** It acts as an architectural extension with a strict unidirectional dependency. HGL heavily relies on GL and directly reuses its core infrastructure—including ID types, type traits and concepts, I/O utilities, and underlying contiguous data structures (like [**gl::flat_jagged_vector**](../cpp-gl/classgl_1_1flat__jagged__vector.md) and [**gl::flat_matrix**](../cpp-gl/classgl_1_1flat__matrix.md)). Therefore, if you are familiar with the GL module, the HGL module's design language will feel immediately natural.

Conversely, the GL module remains completely standalone and entirely unaware of the HGL module or any of its components. This strict separation ensures that projects requiring only standard graph capabilities can utilize the GL module without incurring any compile-time dependencies, structural complexity, or overhead from the generalized hypergraph extensions.

---

## Core Concepts

### The `hgl::hypergraph<Traits>` Template

The [**hgl::hypergraph**](../cpp-gl/classhgl_1_1hypergraph.md) class is the core element of the HGL module. Its entire structural and behavioral identity is dictated by the [**hgl::hypergraph_traits**](../cpp-gl/structhgl_1_1hypergraph__traits.md) struct, which configures:

1. **Directionality**: Undirected vs. BF-Directed.
2. **Vertex Properties**: The data payload attached to each vertex.
3. **Hyperedge Properties**: The data payload attached to each hyperedge.
4. **Implementation Tag**: The underlying implementation model of the hypergraph.
    - This tag dictates the data structure used for the incidence representation (e.g., Incidence List or Incidence Matrix).
    - The implementation tags can be further specialized with:
        - **Layout Tag:** The memory layout of the data structure (e.g., Bidirectional (only for list models), Vertex-Major, or Hyperedge-Major).
        - **Id Type:** The integral type used for internal indexing (defaults to `std::uint32_t`).

To reduce boilerplate, the library provides several generic type aliases for the most common configurations:

- Based on the directional tag:

    - [**hgl::undirected_hypergraph<VP, HeP, ImplTag>**](../cpp-gl/group__HGL-Core.md#typedef-undirected_hypergraph)
    - [**hgl::bf_directed_hypergraph<VP, HeP, ImplTag>**](../cpp-gl/group__HGL-Core.md#typedef-bf_directed_hypergraph)

- Based on the implementation tag:

    - [**hgl::list_hypergraph<LayoutTag, VP, HeP, IdType>**](../cpp-gl/group__HGL-Core.md#typedef-list_hypergraph)
    - [**hgl::flat_list_hypergraph<LayoutTag, VP, HeP, IdType>**](../cpp-gl/group__HGL-Core.md#typedef-flat_list_hypergraph)
    - [**hgl::matrix_hypergraph<LayoutTag, VP, HeP, IdType>**](../cpp-gl/group__HGL-Core.md#typedef-matrix_hypergraph)
    - [**hgl::flat_matrix_hypergraph<LayoutTag, VP, HeP, IdType>**](../cpp-gl/group__HGL-Core.md#typedef-flat_matrix_hypergraph)

### Hypergraph Directionality

The mathematical structure of the [**hgl::hypergraph**](../cpp-gl/classhgl_1_1hypergraph.md) class is defined by the injected directional tag:

- [**hgl::undirected_t**](../cpp-gl/structhgl_1_1undirected__t.md) : In an undirected hypergraph, a hyperedge is simply a mathematical *set* of incident vertices. There is no distinction between the "source" or "destination" of the connection.

- [**hgl::bf_directed_t**](../cpp-gl/structhgl_1_1bf__directed__t.md) : Represents a **Backward-Forward directed** hypergraph. In this model, each hyperedge maps a distinct set of *tail* (source/backward) vertices to a distinct set of *head* (destination/forward) vertices. This model is extremely useful in scenarios like chemical reactions (reactants to products) or logic gates (multiple inputs to multiple outputs).

<div align="center" markdown="1">

![Hypergraph Examples Light](../img/doc/light/hypergraph-examples.svg#only-light){: width="700" }
![Hypergraph Examples Dark](../img/doc/dark/hypergraph-examples.svg#only-dark){: width="700" }

</div>

### IDs vs. Descriptors

Just like the GL module, HGL exposes a dual API utilizing raw **IDs** (`id_type`) and lightweight **Descriptors** ([**vertex_descriptor**](../cpp-gl/group__HGL-Core.md#typedef-vertex_descriptor) and [**hyperedge_descriptor**](../cpp-gl/classhgl_1_1hyperedge__descriptor.md)).

In contrast to the GL module, both vertex and hyperedge descriptors bind only the element's ID and an optional reference to its property payload.

> [!NOTE] API Performance
>
> If your algorithms only require topological information (like connectivity or degrees), prefer the ID-based API to avoid the slight overhead of constructing descriptors and binding property references.

### Descriptor Lifetimes and Invalidation

Because HGL prioritizes fast, cache-friendly contiguous memory allocations, operations that modify the hypergraph's topology may invalidate previously returned descriptors and IDs. Understanding the invalidation rules is critical for writing safe annd stable hypergraph mutating operations.

#### 1. Insertion Stability (Adding Elements)

When you add new vertices or hyperedges to the hypergraph, the library guarantees that existing IDs remain perfectly stable. However, descriptor stability depends entirely on your property configuration:

- **Without Properties:** If your hypergraph elements carry no properties (use `gl::empty_properties`), descriptors are **completely insertion-stable**.

    > [!TIP]
    >
    > Because a **property-less** descriptors are essentially just a wrapper around an ID, there is zero overhead to using it instead of a raw ID, and it will never invalidate when new elements are added.
    >
    > This means that performing operations like the following are completely safe:
    > ```cpp
    > const auto v1 = hg.add_vertex();
    > const auto v2 = hg.add_vertex();
    > const auto e1 = hg.add_hyperedge({v1, v2});
    > const auto v3 = hg.add_vertex();
    > const auto e2 = hg.add_hyperedge();
    > hg.bind({v2, v3}, e2);
    > ```

- **With Properties:** If your hypergraph elements carry custom properties, adding new elements may trigger a memory reallocation in the underlying property storage containers. While the IDs remain valid, the memory references stored inside existing descriptors will dangle. Therefore, property-bound descriptors are **not insertion-stable**.

#### 2. Erasure Stability (Removing Elements)

Because removing elements causes the internal memory to shift and tightly pack to prevent fragmentation, HGL elements are **not erase-stable**. The invalidation rules are symmetric for both element types:

- **Removing a Vertex:** Invalidates all vertex IDs and vertex descriptors for vertices with higher IDs (they shift down). It also invalidates references to all properties associated with the vertices whose IDs shifted as a result of the removal operation.
- **Removing a Hyperedge:** Invalidates all hyperedge IDs and hyperedge descriptors for hyperedges with higher IDs (they shift down). It also invalidates references to all properties associated with the hyperedges whose IDs shifted as a result of the removal operation.

> [!IMPORTANT] Best Practice
>
> If you need to store references to hypergraph elements long-term, store the stable element IDs (`id_type`) assuming no erasures occur. Only fetch fresh descriptors via the available getters (e.g., `hg.vertex(id)` or `hg.hyperedge(id)`) at the moment you need to access or mutate the properties or once the entire hypergraph's structure has been initialized.

### Incidence Operations

Because hyperedges connect sets of vertices rather than exactly two endpoints, constructing the topology requires generalized incidence operations. You can either construct a hyperedge and bind vertices to it incrementally, or supply the entire set of incident vertices upfront during creation.

#### Undirected Hypergraphs

In an undirected hypergraph, you simply provide a collection of vertices that the hyperedge encompasses:

```cpp
auto e1 = hg.add_hyperedge({v0, v1, v2}); // (1)!

auto e2 = hg.add_hyperedge(); // (2)!
hg.bind(v1, e2); // (3)!
hg.bind({v2, v3}, e2); // (4)!

hg.unbind(v1, e2); // (5)!
```

1. Creates a hyperedge and instantly connects it to `v0`, `v1`, and `v2`
2. Creates an empty hyperedge that needs to be bound separately.
3. Binds a single vertex to the given hyperedge.
4. Binds a range of vertices to the specified hyperedge.
5. Unbinds the vertex from the hyperedge.

#### BF-Directed Hypergraphs

In a BF-directed hypergraph, you must explicitly distinguish between the *tail* (source) vertices and *head* (destination) vertices:

```cpp
auto e1 = hg.add_hyperedge({v0}, {v1, v2}); // (1)!

auto e2 = hg.add_hyperedge(); // (2)!
hg.bind_tail(v1, e2); // (3)!
hg.bind_tail(v2, e2);
hg.bind_head({v3, v4}, e2); // (4)!

hg.unbind(v1, e2); // (5)!
hg.unbind(v3, e2);
```

1. Creates a hyperedge where `v0` is the *tail* (source) vertex and the vertices `v1` and `v2` are the *head* (destinations).
2. Creates an empty hyperedge that needs to be bound separately.
3. Binds `v1` and `v2` to the *tail* of the hyperedge individually.
4. Binds `v3` and `v4` to the *head* of the hyperedge simultaneously.
5. Unbinds the given vertices from the hyperedge, regardless of whether they are tail-bound or head-bound. <br/> **NOTE:** A given vertex cannot belong to both tail and head of a single hyperedge at the same time.

### Basic Iteration

The `hgl::hypergraph` exposes standard `<ranges>`-compatible views for generalized traversals:

```cpp
for (auto vertex : hg.vertices()) { // (1)!
    for (auto hyperedge : hg.incident_hyperedges(vertex)) { // (1)!
        for (auto adjacent_v : hg.incident_vertices(hyperedge)) { // (2)!
            // ...
        }
    }
}
```

1. Iterates over all vertices in the hypergraph.
2. Iterates over all hyperedges connected to a specific vertex.
3. Iterates over all vertices contained within a specific hyperedge.

While iterating over incident vertices or hyperedges is possible for both undirected and BF-directed hypergraphs, the BF-directed hypergraphs expose additional methods that allow you to iterate over tail-bound or head-bound elements specifically:

```cpp
for (auto tail_v : hg.tail(hyperedge)) { // (1)!
    // ...
}

for (auto in_hyperedge : hg.in_hyperedges(vertex)) { // (2)!
    // ...
}
```

1. Iterates over only the *tail* vertices of a BF-directed hyperedge.
2. Iterates over only the *incoming* hyperedges of a vertex (such that the vertex belongs to the *head* of the hyperedge).

---

## Representation & Layouts

Because hypergraphs are generalizations of graphs, their topological data cannot be stored as simple arrays of pairs. Instead, CPP-GL relies on **Incidence Models**. HGL categorizes its memory representations via `ImplTag` and strictly controls orientation via `LayoutTag`.

### Incidence Models (Standard & Flat)

- **Incidence Lists** ([`list_t`](../cpp-gl/structhgl_1_1impl_1_1list__t.md) / [`flat_list_t`](../cpp-gl/structhgl_1_1impl_1_1flat__list__t.md)): Maps elements to dynamic jagged lists. Highly space-efficient for sparse hypergraphs, as memory is only allocated for existing incidence relations.
- **Incidence Matrices** ([`matrix_t`](../cpp-gl/structhgl_1_1impl_1_1matrix__t.md) / [`flat_matrix_t`](../cpp-gl/structhgl_1_1impl_1_1flat__matrix__t.md)): Allocates a full $|V| \times |E|$ 2D grid. Memory intensive ($O(|V| \times |E|)$), but allows instant $O(1)$ verification if a given vertex belongs to a given hyperedge.

### Layout Tags (Orientation)

The `LayoutTag` dictates the *primary indexing dimension* of the incidence structure, massively impacting query speeds and memory footprints:

- [**bidirectional_t**](../cpp-gl/structhgl_1_1impl_1_1bidirectional__t.md): Maintains *two* internal mappings (Vertex-to-Hyperedges AND Hyperedge-to-Vertices). Offers optimal $O(1)$ access for both vertex degrees and hyperedge sizes, at the cost of doubled memory consumption. *(Compatible only with Incidence Lists)*.
- [**vertex_major_t**](../cpp-gl/structhgl_1_1impl_1_1vertex__major__t.md): The primary index is the Vertex. Querying the hyperedges connected to a vertex is instantaneous, but finding which vertices belong to a hyperedge requires an expensive full-graph scan.
- [**hyperedge_major_t**](../cpp-gl/structhgl_1_1impl_1_1hyperedge__major__t.md): The primary index is the Hyperedge. Querying the vertices within a hyperedge is instantaneous, but finding a vertex's degree requires a full-graph scan.

> [!NOTE] Matrices and Asymmetry
>
> Incidence matrices fundamentally represent an asymmetric $|V| \times |E|$ mathematical grid. Therefore, matrix representations strictly require an asymmetric layout (`vertex_major_t` or `hyperedge_major_t`), mapping rows to the major element and columns to the minor element.

### Operation Complexity Tables

Because performance is heavily dictated by the combination of the Representation Model (List vs. Matrix) and the Layout Tag (Bidirectional vs. Major), the complexities are grouped accordingly.

*Note: In the tables below, $|V|$ is vertex count, $|E|$ is hyperedge count, $deg(v)$ is vertex degree, and $|e|$ is hyperedge size (number of incident vertices).*

#### 1. Incidence Lists (`list_t` / `flat_list_t`)

| Operation                                | `bidirectional_t`                | `vertex_major_t`                 | `hyperedge_major_t`              |
| :--------------------------------------- | :------------------------------- | :------------------------------- | :------------------------------- |
| **Check Incidence** $(v, e)$             | $O(\min(deg(v), \vert e \vert))$ | $O(deg(v))$                      | $O(\vert e \vert)$               |
| **Iterate Incident Hyperedges** of $v$   | $O(deg(v))$                      | $O(deg(v))$                      | $O(\vert V \vert + \sum \vert e \vert)$ |
| **Iterate Incident Vertices** of $e$     | $O(\vert e \vert)$               | $O(\vert E \vert + \sum deg(v))$ | $O(\vert e \vert)$               |
| **Get Degree** of $v$                    | $O(1)$                           | $O(1)$                           | $O(\vert V \vert + \sum \vert e \vert)$ |
| **Get Size** of $e$                      | $O(1)$                           | $O(\vert E \vert + \sum deg(v))$ | $O(1)$                           |
| **Memory Footprint (Topology)** | $O(2 \times \sum deg(v))$        | $O(\sum deg(v))$                 | $O(\sum \vert e \vert)$          |

#### 2. Incidence Matrices (`matrix_t` / `flat_matrix_t`)

| Operation                                | `vertex_major_t` (Rows = V, Cols = E) | `hyperedge_major_t` (Rows = E, Cols = V) |
| :--------------------------------------- | :------------------------------------ | :--------------------------------------- |
| **Check Incidence** $(v, e)$             | $O(1)$                                | $O(1)$                                   |
| **Iterate Incident Hyperedges** of $v$   | $O(\vert E \vert)$ (Scan Row)         | $O(\vert E \vert)$ (Scan Column)         |
| **Iterate Incident Vertices** of $e$     | $O(\vert V \vert)$ (Scan Column)      | $O(\vert V \vert)$ (Scan Row)            |
| **Get Degree** of $v$                    | $O(\vert E \vert)$                    | $O(\vert E \vert)$                       |
| **Get Size** of $e$                      | $O(\vert V \vert)$                    | $O(\vert V \vert)$                       |
| **Memory Footprint (Topology)** | $O(\vert V \vert \times \vert E \vert)$ | $O(\vert V \vert \times \vert E \vert)$  |

### Choosing the Representation & Layout

Selecting the correct combination ensures your application hits peak performance:

- Use **`bidirectional_t`** Lists (the default) for general-purpose hypergraphs where you frequently query in both directions (e.g., "what nodes are in this hyperedge?" AND "what hyperedges is this node in?").
- Use **`hyperedge_major_t`** Lists when dealing with massive datasets where memory is tight, and your algorithms are strictly edge-centric (e.g., simulating isolated hyperedge reactions).
- Use **`matrix_t`** when your hypergraph is extremely dense, hyperedge sizes are close to $|V|$, and absolute instant incidence verification `are_incident(v, e)` is the algorithmic bottleneck.
```
