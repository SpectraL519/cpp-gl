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

Because hypergraphs are generalizations of graphs, their topological data cannot be stored the same way as standard graphs (Adjacency Models). Instead, CPP-HGL relies on **Incidence Models** to capture the higher-order connections between vertices and hyperedges. The HGL module categorizes its memory representations via the `ImplTag` and strictly controls their memory orientation with the `LayoutTag`.

### Fundamental Representations

At their core, hypergraph data structures differ in how they map vertices to the hyperedges they belong to, or vice versa. The primary incidence models in the HGL module are based on the following architectures:

- **Incidence Lists**: This model stores only the active incidence relationships. It works by mapping an element (e.g., a vertex) to a dynamic, list of its associated elements (e.g., the hyperedges it belongs to). This approach is highly space-efficient for sparse hypergraphs and allows rapid iteration over local incidence sets.
- **Incidence Matrices**: This model allocates a full $|V| \times |E|$ 2D grid, where each cell represents a potential connection between a specific vertex and a specific hyperedge. While they consume significantly more memory ($O(|V| \times |E|)$), they provide instant $O(1)$ incidence verification, making them ideal for dense, heavily interconnected hypergraphs.

### Representation Layouts (Orientation)

The `LayoutTag` dictates the *primary indexing dimension* of the incidence structure. Because hypergraphs map two distinctly different sets ($V$ and $E$), changing the primary index massively impacts query speeds and memory footprints:

- [**bidirectional_t**](../cpp-gl/structhgl_1_1impl_1_1bidirectional__t.md): Maintains *two* internal mappings simultaneously (Vertex-to-Hyperedges AND Hyperedge-to-Vertices). Offers optimal $O(1)$ access for both vertex degrees and hyperedge sizes, and fast iteration in both directions, at the cost of doubled memory consumption. **(Compatible only with Incidence Lists)**.
- [**hyperedge_major_t**](../cpp-gl/structhgl_1_1impl_1_1hyperedge__major__t.md): The primary index is the Hyperedge. Querying the vertices within a specific hyperedge is instantaneous, but finding which hyperedges a vertex belongs to may require an expensive full-structure scan.
- [**vertex_major_t**](../cpp-gl/structhgl_1_1impl_1_1vertex__major__t.md): The primary index is the Vertex. Querying the hyperedges connected to a specific vertex is instantaneous, but finding which vertices belong to a specific hyperedge may require an expensive, full-structure scan.

> [!NOTE] Matrices and Asymmetry
>
> Incidence matrices fundamentally represent an asymmetric $|V| \times |E|$ grid. Therefore, matrix representations strictly require an asymmetric layout (`vertex_major_t` or `hyperedge_major_t`), mapping rows to the major element and columns to the minor element.

### Standard Models

Standard models are heap-allocated, nested structures (e.g., `std::vector<std::vector<T>>`) that prioritize flexibility and dynamic structural modification. Because the inner containers can grow independently, they handle topological changes gracefully.

- [**list_t**](../cpp-gl/structhgl_1_1impl_1_1list__t.md): A standard Incidence List model implemented using traditional nested containers.
- [**matrix_t**](../cpp-gl/structhgl_1_1impl_1_1matrix__t.md): A standard Incidence Matrix model implemented using traditional nested containers.

<div align="center" markdown="1">

![Undirected Hypergraph Standard Representation Light](../img/doc/light/undir-hypergraph-repr-std.svg#only-light){: width="700" }
![Undirected Hypergraph Standard Representation Dark](../img/doc/dark/undir-hypergraph-repr-std.svg#only-dark){: width="700" }

</div>

> [!NOTE] BF-Directed Representations
>
> The diagrams above illustrate standard representations for an **undirected** hypergraph. For **BF-directed** hypergraphs, the underlying architecture adapts to capture directionality:
> - **Incidence List:** The structure maintains separate inner containers to explicitly distinguish between tail-bound and head-bound incidence relationships.
> - **Incidence Matrix:** The cells of the matrix are no longer simple boolean indicators. Instead, they store specific directional states (conceptually mapped as `-1`, `0`, and `1`) to indicate whether a vertex is in the tail of the hyperedge, not connected, or in the head of the hyperedge, respectively.

### Flat Models

To maximize cache locality, the flat representations map the logical 2D structures into contiguous 1D memory blocks. By keeping all incidence data tightly packed, these models provide the absolute maximum traversal speed. However, this cache-friendliness comes at a structural cost: modifying an inner segment often requires shifting the entire remainder of the flat container in memory.

- [**flat_list_t**](../cpp-gl/structhgl_1_1impl_1_1flat__list__t.md): A flattened Incidence List model implemented using the generic [**gl::flat_jagged_vector**](../cpp-gl/classgl_1_1flat__jagged__vector.md) data structure.
- [**flat_matrix_t**](../cpp-gl/structhgl_1_1impl_1_1flat__matrix__t.md): A flattened Incidence Matrix model implemented using the generic [**gl::flat_matrix**](../cpp-gl/classgl_1_1flat__matrix.md) data structure.

<div align="center" markdown="1">

![Undirected Hypergraph Flat Representation Light](../img/doc/light/undir-hypergraph-repr-flat.svg#only-light){: width="700" }
![Undirected Hypergraph Flat Representation Dark](../img/doc/dark/undir-hypergraph-repr-flat.svg#only-dark){: width="700" }

</div>

> [!NOTE] BF-Directed Representations
>
> Similarly to the standard models, flat architectures seamlessly adapt to directionality. A BF-directed *Flat Incidence List* utilizes separated flat storage for tail and head incidence, preserving contiguous memory reads for specific directional traversals. Meanwhile, a *Flat Incidence Matrix* packs the tri-state directional indicators directly into its contiguous 1D grid layout.

> [!NOTE] Flat List Model Performance
>
> While the flat incidence list model is highly efficient for hypergraph storage and traversal, it is also highly inefficient to construct sequentially (element-by-element and incidence-by-incidence). The most efficient approach for utilizing flat list hypergraphs is to construct your hypergraph using the standard `list_t` model first, and then convert it into the `flat_list_t` model using the generic [**hgl::to**](../cpp-gl/group__HGL-Core.md#function-to) conversion function.

### Operation Complexities

Because hypergraph performance is heavily dictated by the combination of the Representation Model (List vs. Matrix), the underlying Memory Strategy (Standard vs. Flat), and the Layout Tag (Bidirectional vs. Major), the complexities are grouped accordingly.

*Note: In the tables below, $|V|$ is the total vertex count, $|E|$ is the total hyperedge count, $deg(v)$ is the degree of a specific vertex, and $|e|$ is the size of a specific hyperedge. $I$ represents the total number of incidences across the entire hypergraph ($I = \sum deg(v) = \sum |e|$).*

> [!NOTE] Directionality and Complexity
> For **BF-directed** hypergraphs, operations like `in_degree`, `out_degree`, `tail_vertices`, and `head_vertices` follow the exact same complexity class as their undirected counterparts (`degree`, `incident_vertices`). They merely operate on isolated sub-containers (in list models) or specific tri-state indicators (in matrix models), keeping the Big-O structurally identical.

#### 1. Incidence Lists

**Topological Queries:**

The complexities of topological queries is identical for standard (`list_t`) and flat (`flat_list_t`) Incidence List models.

| Query Operation | Bidirectional | Vertex-Major | Hyperedge-Major |
| :--- | :--- | :--- | :--- |
| **Check Incidence** $(v, e)$ | $O(\log(\min(deg(v), \vert e \vert)))$ | $O(\log(deg(v)))$ | $O(\log(\vert e \vert))$ |
| **Iterate Incident Hyperedges** of $v$ | $O(deg(v))$ | $O(deg(v))$ | $O(\vert V \vert + I)$ (Scan All) |
| **Iterate Incident Vertices** of $e$ | $O(\vert e \vert)$ | $O(\vert E \vert + I)$ (Scan All) | $O(\vert e \vert)$ |
| **Get Degree** of $v$ | $O(1)$ | $O(1)$ | $O(\vert V \vert + I)$ |
| **Get Size** of $e$ | $O(1)$ | $O(\vert E \vert + I)$ | $O(1)$ |

**Structural Mutations (Standard Incidence List, `list_t`):**

| Mutation Operation | Bidirectional | Vertex-Major |Hyperedge-Major |
| :--- | :--- | :--- | :--- |
| **Add Vertex** | $O(1)$ amortized | $O(1)$ amortized | $O(1)$ amortized |
| **Add Hyperedge** | $O(1)$ amortized | $O(1)$ amortized | $O(1)$ amortized |
| **Remove Vertex** | $O(\vert V \vert + \vert E \vert + I)$ | $O(\vert V \vert)$ *(Major)* | $O(\vert E \vert + I)$ *(Minor)* |
| **Remove Hyperedge** | $O(\vert V \vert + \vert E \vert + I)$ | $O(\vert V \vert + I)$ *(Minor)* | $O(\vert E \vert)$ *(Major)* |
| **Add Incidence** (Bind) | $O(deg(v) + \vert e \vert)$ | $O(deg(v))$ | $O(\vert e \vert)$ |
| **Remove Incidence** (Unbind)| $O(deg(v) + \vert e \vert)$ | $O(deg(v))$ | $O(\vert e \vert)$ |

**Structural Mutations (Flat Incidence List, `flat_list_t`):**

| Mutation Operation | Bidirectional | Vertex-Major |Hyperedge-Major |
| :--- | :--- | :--- | :--- |
| **Add Vertex** | $O(1)$ amortized | $O(1)$ amortized | $O(1)$ amortized |
| **Add Hyperedge** | $O(1)$ amortized | $O(1)$ amortized | $O(1)$ amortized |
| **Remove Vertex** | $O(\vert V \vert + \vert E \vert + I)$ | $O(\vert V \vert + I)$ *(Major)* | $O(\vert E \vert + I)$ *(Minor)* |
| **Remove Hyperedge** | $O(\vert V \vert + \vert E \vert + I)$ | $O(\vert V \vert + I)$ *(Minor)* | $O(\vert E \vert + I)$ *(Major)* |
| **Add Incidence** (Bind) | $O(I)$ | $O(I)$ | $O(I)$ |
| **Remove Incidence** (Unbind)| $O(I)$ | $O(I)$ | $O(I)$ |

*(Note: Adding/removing incidences in flat lists is always $O(I)$ because modifying any internal jagged segment requires shifting the remainder of the massive contiguous 1D data array).*

#### 2. Incidence Matrices

**Topological Queries:**

The complexities of topological queries is identical for standard (`matrix_t`) and flat (`flat_matrix_t`) Incidence Matrix models.

| Query Operation | Vertex-Major <br/> (Rows: V, Cols: E) |Hyperedge-Major <br/> (Rows: E, Cols: V) |
| :--- | :--- | :--- |
| **Check Incidence** $(v, e)$ | $O(1)$ | $O(1)$ |
| **Iterate Incident Hyperedges** of $v$ | $O(\vert E \vert)$ (Scan Row) | $O(\vert E \vert)$ (Scan Column) |
| **Iterate Incident Vertices** of $e$ | $O(\vert V \vert)$ (Scan Column) | $O(\vert V \vert)$ (Scan Row) |
| **Get Degree** of $v$ | $O(\vert E \vert)$ (Scan Row) | $O(\vert E \vert)$ (Scan Column) |
| **Get Size** of $e$ | $O(\vert V \vert)$ (Scan Column) | $O(\vert V \vert)$ (Scan Row) |

**Structural Mutations:**

| Mutation Operation | `matrix_t` (Standard) | `flat_matrix_t` (Flat) |
| :--- | :--- | :--- |
| **Add Vertex** | $O(\vert E \vert)$ amortized if `vertex_major_t`<br>$O(\vert V \vert \cdot \vert E \vert)$ if `hyperedge_major_t` | $O(\vert V \vert \cdot \vert E \vert)$ |
| **Add Hyperedge** | $O(\vert V \vert \cdot \vert E \vert)$ if `vertex_major_t`<br>$O(\vert V \vert)$ amortized if `hyperedge_major_t` | $O(\vert V \vert \cdot \vert E \vert)$ |
| **Remove Vertex / Hyperedge** | $O(\vert V \vert \cdot \vert E \vert)$ (Shift rows/cols) | $O(\vert V \vert \cdot \vert E \vert)$ |
| **Add / Remove Incidence** (Bind/Unbind) | $O(1)$ | $O(1)$ |

> [!NOTE] Matrix Layouts and Cache Locality
>
> While the theoretical Big-O complexities appear identical across matrix layouts, **real-world performance is heavily bound by cache locality.** Matrices map one dimension to contiguous rows and the other to strided columns.
>
> - **Query Direction:** Choose the layout that aligns your most frequent traversal with contiguous memory reads. If your algorithm primarily iterates over the vertices within specific hyperedges, `hyperedge_major_t` will drastically outperform `vertex_major_t` by avoiding cache misses.
> - **Dimension Imbalance:** If your hypergraph is highly asymmetric (e.g., $|V| \gg |E|$), scanning the larger dimension across strided memory can degrade performance. Align the major layout with the dimension you need to scan most efficiently.

### Choosing the Representation & Layout

Selecting the optimal combination of Representation Model and Layout Tag requires balancing memory constraints, topology density, and your specific traversal patterns to achieve peak performance:

- **`bidirectional_t` Lists (Default):** The most versatile choice for general-purpose hypergraphs. Use this when your algorithms require fast, symmetric traversals (e.g., frequently oscillating between querying "which vertices belong to this hyperedge?" and "which hyperedges contain this vertex?"). It trades a doubled memory footprint for optimal $O(1)$ degree and size lookups, ensuring fluid bidirectional navigation.
- **Single-Major Lists (`hyperedge_major_t` / `vertex_major_t`):** Ideal for massive, sparse datasets operating under strict memory limits. Use these layouts when your algorithm's traversal logic is heavily asymmetrical. For instance, if an algorithm strictly simulates isolated chemical reactions and rarely needs to compute a vertex's degree, `hyperedge_major_t` eliminates the redundant backward-mapping overhead.
- **Matrices (`matrix_t` / `flat_matrix_t`):** Reserved for exceptionally dense hypergraphs where hyperedge sizes consistently approach $|V|$. Choose a matrix representation only when absolute, instant $O(1)$ incidence verification (`are_incident(v, e)`) is the primary algorithmic bottleneck and the $O(|V| \times |E|)$ memory consumption is an acceptable trade-off.
