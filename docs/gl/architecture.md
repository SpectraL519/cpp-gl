# Architecture & Basics

## GL Module Overview

The **GL (Graph Library)** module is engineered around a singular philosophy: provide zero-cost abstractions that do not compromise on flexibility or ergonomics. By strictly utilizing C++20 concepts and template metaprogramming, the library ensures that all configuration—from memory layout to element payloads—is resolved at compile time.

This section explores the core architectural decisions of the GL module:

- [Core Concepts](#core-concepts): Learn how the `gl::graph` template operates, the difference between IDs and descriptors, and how to navigate topologies.
- [Graph Representation Models](#graph-representation-models): Understand the diverse memory models available and their performance characteristics.
- [Properties & Custom Data](#properties-custom-data): Discover how to inject arbitrary data directly into your graph elements with strict type safety.

---

## Core Concepts

### The `gl::graph<Traits>` Template

At the heart of the library is the [**gl::graph**](../cpp-gl/classgl_1_1graph.md) class. Unlike traditional object-oriented designs that rely on virtual inheritance, CPP-GL uses a policy-based design. A single template parameter, `GraphTraits`, dictates the graph's entire structural and behavioral identity.

The [**gl::graph_traits**](../cpp-gl/structgl_1_1graph__traits.md) struct configures five key policies:

1. **Directionality**: Directed vs. Undirected.
2. **Vertex Properties**: The data payload attached to each vertex.
3. **Edge Properties**: The data payload attached to each edge.
4. **Implementation Tag**: The underlying memory layout (e.g., Adjacency List).
5. **ID Type**: The integer type used for internal indexing (defaults to `std::uint32_t`).

To reduce boilerplate, the library provides several generic type aliases for the most common configurations:

- Based on the directional tag:

    - [**gl::directed_graph<VP, EP, Impl, IdType>**](../cpp-gl/group__GL-Core.md#typedef-directed_graph)
    - [**gl::undirected_graph<VP, EP, Impl, IdType>**](../cpp-gl/group__GL-Core.md#typedef-undirected_graph)

- Based on the implementation tag:

    - [**gl::list_graph<Dir, VP, EP, IdType>**](../cpp-gl/group__GL-Core.md#typedef-list_graph)
    - [**gl::flat_list_graph<Dir, VP, EP, IdType>**](../cpp-gl/group__GL-Core.md#typedef-flat_list_graph)
    - [**gl::matrix_graph<Dir, VP, EP, IdType>**](../cpp-gl/group__GL-Core.md#typedef-matrix_graph)
    - [**gl::flat_matrix_graph<Dir, VP, EP, IdType>**](../cpp-gl/group__GL-Core.md#typedef-flat_matrix_graph)

### Graph Directionality

The directional behavior of a graph is governed by injecting a dedicated directional tag into the graph's traits. This configuration dictates the type of the graph and the mathematical nature of its edge set and influences how traversal and degree calculations are performed.

Formally, a graph $G = (V, E)$ consists of a set of vertices $V$ and a set of edges $E$:

- [**directed_t**](../cpp-gl/structgl_1_1directed__t.md) : Specifies a **directed graph** configuration where edges are defined as ordered pairs $(u, v)$ such that $u, v \in V$. In this graph type, a connection from vertex $u$ to vertex $v$ is structurally distinct from a connection from $v$ to $u$, and the existence of one directed edge does not imply the existence of the other.

- [**gl::undirected_t**](../cpp-gl/structgl_1_1undirected__t.md) : Edges are unordered pairs $\{u, v\}$ where $u, v \in V$. The library automatically manages the bidirectional nature of these connections, ensuring that an edge between $u$ and $v$ is recognized during both traversal and structural degree calculations regardless of the order of endpoints.

### IDs vs. Descriptors

The library exposes a dual API to balance raw performance with ergonomic property access:

- **IDs** (`id_type`): Raw integers used to uniquely identify vertices and edges. Methods suffixing in `_ids` (e.g., `vertex_ids`, `neighbor_ids`) operate strictly on these identifiers.

- **Descriptors**: Lightweight wrapper objects providing a higher-level interface for graph elements:

    - [**vertex_descriptor**](../cpp-gl/classgl_1_1vertex__descriptor.md): Binds a vertex ID with an optional reference to its property payload.

    - [**edge_descriptor**](../cpp-gl/classgl_1_1edge__descriptor.md): Binds an edge ID, the IDs of its source and target vertices, and an optional reference to its property payload.

    Descriptors are returned by non-suffixed traversal methods (e.g., `vertices`, `neighbors`, `out_edges`).

> [!NOTE] API Performance
>
> If your algorithms only require topological information (like connectivity or degrees), prefer the ID-based API to avoid the slight overhead of constructing descriptors and binding property references.

### Descriptor Lifetimes and Invalidation

Because CPP-GL prioritizes cache-friendly contiguous memory, operations that modify the graph's topology may invalidate previously returned descriptors. Understanding these rules is critical for writing safe and stable graph mutations.

#### 1. Insertion Stability (Adding Elements)

When you add new vertices or edges to the graph, the library guarantees that existing IDs remain perfectly stable. However, descriptor stability depends entirely on your property configuration:

- **Without Properties:** If your graph elements carry no properties (use [**gl::empty_properties**](../cpp-gl/structgl_1_1empty__properties.md)), descriptors are **completely insertion-stable**.

    > [!TIP]
    >
    > Because a property-less vertex descriptor is essentially just a wrapper around an ID, there is zero overhead to using it instead of a raw ID, and it will never invalidate when new elements are added.
    >
    > This means that performing operations like the following are completely safe:
    > ```cpp
    > const auto v1 = graph.add_vertex();
    > const auto v2 = graph.add_vertex();
    > const auto e1 = graph.add_edge(v1, v2);
    > const auto v3 = graph.add_vertex();
    > const auto e2 = graph.add_edge(v2, v3);
    > ```

- **With Properties:** If your graph elements carry custom properties, adding new elements may trigger a memory reallocation in the underlying property vectors. While the IDs remain valid, the memory references stored inside existing descriptors will dangle. Therefore, property-bound descriptors are **not insertion-stable**.

#### 2. Erasure Stability (Removing Elements)

Because removing elements causes the internal memory to shift and pack tightly to prevent fragmentation, descriptors and IDs are **not erase-stable**:

- **Without Properties:** Invalidates all previously stored edge descriptors and edge IDs (as higher IDs shift down). Vertex descriptors remain completely valid.

- **Removing a Vertex:** Invalidates both vertex and edge descriptors, as well as all IDs.

> [!IMPORTANT] Best Practice: Store IDs, not Descriptors
>
> As a general architectural rule: if your algorithm modifies the graph topology, or if you need to store references to graph elements long-term, store the stable element IDs (id_type). Fetch fresh descriptors via `graph.vertex(id)` or `graph.edge(...)` only at the exact moment you need to access or mutate the properties.

### Edge Endpoints & Incidence

The [**edge_descriptor**](../cpp-gl/classgl_1_1edge__descriptor.md) class provides a rich interface for querying endpoint connectivity and incidence without needing to reference the parent graph:

- **`.source()`**: Returns the ID of the vertex where the edge begins.
- **`.target()`**: Returns the ID of the vertex where the edge ends.
- **`.other(vertex_id)`**: Given one incident vertex ID, returns the opposite endpoint.
- **`.is_incident_with(vertex_id)`**: Returns `true` if the given vertex is either the source or the target.
- **`.is_loop()`**: Returns `true` if the source and target are the exact same vertex.

### Basic Iteration

The [**gl::graph**](../cpp-gl/classgl_1_1graph.md) provides STL-compatible, lazily evaluated views for seamless traversal using range-based for loops or `<ranges>` algorithms:

```cpp
for (auto vertex : graph.vertices()) { // (1)!
    for (auto edge : graph.out_edges(vertex)) { // (2)!
        // ...
    }
}

for (auto neighbor_id : graph.neighbor_ids(source_id)) { // (3)!
    // ...
}
```

1. Iterate over all vertex descriptors in the graph.
2. Iterate over all edges leaving the current vertex.
3. Iterate over the neighbor IDs of a specific vertex

---

## Graph Representation Models

Choosing the correct memory layout is critical for algorithmic performance. CPP-GL abstracts this choice entirely behind the `ImplTag`, allowing you to swap layouts without altering a single line of traversal code.

### Fundamental Representations

At their core, graph data structures differ in how they map vertices to their connections. While the library may expand to include other formats (such as CSR, Edge Lists, or Incidence Matrices), the primary models are based on the following architectures:

Consider the following graph:

<div align="center" markdown="1">

![Undirected Graph Light](../img/doc/light/undir-graph.svg#only-light){: width="300" }
![Undirected Graph Dark](../img/doc/dark/undir-graph.svg#only-dark){: width="300" }

</div>

- **Adjacency Lists**: This model stores only the vertices and the edges that actively exist in the graph. By mapping each vertex to a dynamic list of its immediate neighbors, this approach is highly space-efficient for sparse graphs and allows rapid iteration over local neighborhoods.
- **Adjacency Matrices**: This model allocates a full $\vert V \vert \times \vert V \vert$ grid, where each cell represents a potential connection. While they consume significantly more memory ($O(\vert V \vert^2)$), they provide instant $O(1)$ edge-existence lookups, making them ideal for dense networks.

### Available Representation Models

CPP-GL currently categorizes its memory layouts into two primary families based on their underlying memory allocation strategy.

> [!NOTE] All representation model tag types are defined in the `gl::impl` namespace.

#### Standard Models

Heap-allocated, nested structures that prioritize flexibility and dynamic structural modification.

- [**list_t**](../cpp-gl/structgl_1_1impl_1_1list__t.md): A standard Adjacency List model implemented using traditional nested containers (e.g., `std::vector<std::vector<T>>`).
- [**matrix_t**](../cpp-gl/structgl_1_1impl_1_1matrix__t.md): A standard Adjacency Matrix model implemented using traditional nested containers.

### Flat Models

Contiguous 1D memory blocks that prioritize cache locality and maximum traversal speed over modification speed.

- [**flat_list_t**](../cpp-gl/structgl_1_1impl_1_1flat__list__t.md): A flattened Adjacency List model implemented using the generic [**flat_jagged_vector**](../cpp-gl/classgl_1_1flat__jagged__vector.md) data structure.
- [**flat_matrix_t**](../cpp-gl/structgl_1_1impl_1_1flat__matrix__t.md): A flattened Adjacency Matrix model implemented using the generic [**flat_matrix**](../cpp-gl/classgl_1_1flat__matrix.md) data structure.

### Topology Support: Simple Graphs and Multigraphs

The chosen representation model strictly dictates the graph's capability to store multiple edges between the exact same pair of vertices:

- **Simple Graphs** allow at most one edge between any distinct pair of vertices.
- **Multigraphs** allow multiedges (multiple connections between a given pair of vertices $(u, v)$).

Adjacency Matrix models use a single, distinct cell for any given $(u, v)$ pair. Because of this, they inherently enforce a **simple graph** topology.

Adjacency List models append edges sequentially, inherently supporting **multigraphs**.

> [!NOTE] Adjacency Lists and Simple Graphs
>
> It is entirely possible to represent a simple graph using a list-based model. However, the data structure itself will not prevent the insertion of duplicate edges. Hence, when using a model capable of representing multigraphs to represent a simple graph, the responsibility of ensuring no multiedges are added to the graph falls entirely on the user.

### Edge-Aware Representations

Traditional graph data structures often store only boolean values or raw weights. However, the core models currently implemented in CPP-GL are explicitly **edge-aware**.

Being "edge-aware" means that the internal representation stores the specific `edge_id` alongside the topological connection. This architecture guarantees that the set of edge IDs remains a tightly packed, contiguous sequence (from $0$ to $\vert E \vert - 1$). This allows users to store custom edge properties in standard, flat `std::vector`s and access them with instant $O(1)$ performance directly using the `edge_id`, eliminating the need for expensive secondary map lookups.

> [!Note] Edge-Unaware Representations
>
> Future iterations of the library may introduce **edge-unaware** representations for maximum memory efficiency, such as a packed boolean adjacency matrices where the edge ID is calculated mathematically as $u \times \vert V \vert + v$, rather than explicitly stored in memory.

#### Standard Memory Models

The standard implementations utilize traditional, nested 2D containers (e.g., `std::vector<std::vector<T>>`).

These models are highly flexible. Because the inner containers can grow independently, they handle structural modifications, like adding vertices or edges, gracefully. The trade-off is that the memory is fragmented across the heap, which can lead to cache misses during heavy graph traversals.

<div align="center" markdown="1">

![Undirected Graph Standard Representation](../img/doc/light/undir-graph-repr-std.svg#only-light){: width="700" }
![Undirected Graph Standard Representation](../img/doc/dark/undir-graph-repr-std.svg#only-dark){: width="700" }

</div>

#### Flat Memory Models

To maximize cache locality, the flat representations map the logical 2D structures into contiguous 1D memory blocks.

By keeping all vertex and edge data in adjacent memory blocks, these models provide the absolute maximum traversal speed. However, this cache-friendliness comes at a structural cost: because all data is packed tightly, modifying an inner segment (such as adding a new edge to a vertex's list) requires shifting the entire remainder of the flat container in memory - unlike standard models, which only shift the localized inner container. Furthermore, modifications that exceed the pre-allocated contiguous capacity (like adding vertices) often require an expensive reallocation of the entire underlying memory block.

<div align="center" markdown="1">

![Undirected Graph Flat Representation](../img/doc/light/undir-graph-repr-flat.svg#only-light){: width="700" }
![Undirected Graph Flat Representation](../img/doc/dark/undir-graph-repr-flat.svg#only-dark){: width="700" }

</div>

> [!NOTE] Flat List Model Performance
>
> While the flat adjacency list model is highly efficient for graph storage and traversal, it is highly inefficient to construct element-by-element. The most efficient approach for utilizing flat list graphs is to construct your graph using the standard list model first, and then convert it into the flat list model using the generic [**gl::to**](../cpp-gl/group__GL-Core.md#function-to) conversion function. This exact methodology is utilized internally by the [**graph topology generators**](topologies.md) defined within the library.

### Operation Complexity

Depending on the chosen representation model, the computational complexity of standard graph operations will differ. The table below outlines these complexities.

| Operation                                  | Standard List                      | Flat List                          | Standard Matrix      | Flat Matrix          |
| :----------------------------------------- | :--------------------------------- | :--------------------------------- | :------------------- | :------------------- |
| Add Vertex                                 | $O(1)$ amortized                   | $O(1)$ amortized                   | $O(\vert V \vert)$   | $O(\vert V \vert^2)$ |
| Remove Vertex                              | $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert^2)$ | $O(\vert V \vert^2)$ |
| Add Edge                                   | $O(1)$ amortized                   | $O(\vert E \vert)$                 | $O(1)$               | $O(1)$               |
| Remove Edge                                | $O(deg(v))$                        | $O(\vert E \vert)$                 | $O(1)$               | $O(1)$               |
| Check Edge Exists                          | $O(deg(v))$                        | $O(deg(v))$                        | $O(1)$               | $O(1)$               |
| Iterate Out-Edges                          | $O(deg(v))$                        | $O(deg(v))$                        | $O(\vert V \vert)$   | $O(\vert V \vert)$   |
| Iterate In-Edges <br/> (Undirected Graphs) | $O(deg(v))$                        | $O(deg(v))$                        | $O(\vert V \vert)$   | $O(\vert V \vert)$   |
| Iterate In-Edges <br/> (Directed Graphs)   | $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert)$   | $O(\vert V \vert)$   |
| Iterate All Edges                          | $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert + \vert E \vert)$ | $O(\vert V \vert^2)$ | $O(\vert V \vert^2)$ |

### Choosing the Layout

Selecting the right `ImplTag` is a balance of your specific operational needs - use:

- [**list_t**](../cpp-gl/structgl_1_1impl_1_1list__t.md) for highly dynamic, sparse graphs where the topology changes frequently.
- [**flat_list_t**](../cpp-gl/structgl_1_1impl_1_1flat__list__t.md) for static, sparse graphs where traversal speed and cache locality are paramount.
- [**matrix_t**](../cpp-gl/structgl_1_1impl_1_1matrix__t.md) (or [**flat_matrix_t**](../cpp-gl/structgl_1_1impl_1_1flat__matrix__t.md) if structure is entirely static) for highly dense graphs (where $\vert E \vert \approx \vert V \vert^2$) when instant $O(1)$ edge lookups are strictly required and memory footprint is not a bottleneck.

---

## Properties & Custom Data

To make the library useful for real-world applications (like game development or network analysis), CPP-GL allows you to seamlessly inject arbitrary data directly into vertices and edges.

### Injection and Access

Properties are defined via the `VertexProperties` and `EdgeProperties` template parameters of [**gl::graph_traits**](../cpp-gl/structgl_1_1graph__traits.md). If a graph has no properties, it utilizes the zero-overhead [**gl::empty_properties**](../cpp-gl/structgl_1_1empty__properties.md) tag.

When custom properties are defined, they can be accessed and mutated through three primary interfaces:

1. **Descriptor Access:** If you hold a valid vertex or edge descriptor, you can interact with its payload directly using the overloaded arrow (`->`) and dereference (`*`) operators, or explicitly via the `.properties()` method.

2. **Direct ID Lookups:** You can fetch mutable property references directly from the graph using an element's raw ID via `graph.vertex_properties(id)` and `graph.edge_properties(id)`.

3. **Global Property Maps:** For algorithms that require viewing or iterating over all payloads simultaneously, you can retrieve a lazily-evaluated, random-access view of the entire underlying property collection via `graph.vertex_properties_map()` and `graph.edge_properties_map()`.

### Built-In Properties

The library includes several robust, ready-to-use property structures, including:

- [**gl::name_property**](../cpp-gl/structgl_1_1name__property.md): Injects a simple quoted std::string identifier mapped to the element.

- [**gl::weight_property**](../cpp-gl/structgl_1_1weight__property.md): Injects an arithmetic weight field (defaults to double). Used extensively by pathfinding and MST algorithms.

- [**gl::binary_color_property**](../cpp-gl/structgl_1_1binary__color__property.md): Injects a lightweight, byte-sized state indicator (Black, White, Unset). It is specifically designed to support graph coloring algorithms, bipartition checks, and cycle detection.

- [**gl::dynamic_properties**](../cpp-gl/classgl_1_1dynamic__properties.md): Injects a type-safe container for heterogeneous properties stored via string keys. This allows for the dynamic attachment and retrieval of arbitrary, type-erased (std::any) data at runtime without requiring compile-time knowledge.

### Injecting Custom Property Types

CPP-GL's concepts only require that your custom property type is *semiregular* (default constructible and copyable). This allows you to attach custom, complex property types directly into the graph without requiring associative look-up tables.

```cpp
struct GameNode { // (1)!
    std::string name;
    int hit_points = 100;
    bool is_safe_zone = false;
};

using traits_t = gl::directed_graph_traits<GameNode, gl::weight_property<int>>; // (2)!
gl::graph<traits_t> graph;

auto v1_id = graph.add_vertex_with(GameNode{"Spawn Point", 999, true}).id(); // (3)!
auto v2_id = graph.add_vertex_with(GameNode{"Enemy Camp", 50, false}).id();

graph[v2_id]->hit_points -= 10; // (4)!
```

1. Define a custom vertex/node data payload.
2. Inject the custom properties into the graph. Here `GameNode` represents the vertex properties and `gl::weight_property<int>` the edge properties.
3. Add vertices and store their IDs (**NOTE:** Property-holding descriptors are not insertion-stable).
4. Fetch a fresh vertex descriptor using the `[]` operator to safely access or mutate the property.
