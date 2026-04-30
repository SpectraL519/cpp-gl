# Concrete Traversals

While the [Generic Templates](templates.md) provide immense power and flexibility, manually setting up the `visited` arrays, initialization ranges, and multi-step expansion predicates for simple tasks can be overly verbose.

To solve this, HGL provides concrete traversal algorithms: **Breadth-First Search (BFS)** and **Depth-First Search (DFS)**, alongside specialized directional reachability wrappers. These functions act as simple wrappers around the core generic engines. They automatically allocate and manage the necessary state-tracking containers for both vertices and hyperedges, while still allowing you to inject custom logic via `pre_visit` and `post_visit` callbacks.

---

## Shared Behaviors and Mechanics

All concrete traversals in the HGL module share a unified design philosophy regarding return types and component handling, mirroring the GL module.

### The Result Discriminator

By default, concrete traversals are designed to build a traversal tree. However, sometimes you only want to traverse a hypergraph for its side effects (e.g., modifying vertex properties) without paying the memory allocation cost of building a predecessor map.

You can explicitly control this using the template's `Result` discriminator:

- [**`hgl::algorithm::ret`**](../../cpp-gl/group__HGL-Algorithm.md#typedef-result_discriminator) (Default): The algorithm allocates and populates an [**hgl::algorithm::search_tree<H>**](../../cpp-gl/group__HGL-Algorithm.md#typedef-search_tree). This structure records both the predecessor vertex and the specific hyperedge that was traversed to discover each vertex.
- [**`hgl::algorithm::noret`**](../../cpp-gl/group__HGL-Algorithm.md#typedef-result_discriminator): The algorithm executes purely for side effects. The search tree allocation is completely optimized away at compile time, and the function returns `void`.

### Handling Disconnected Components

Hypergraphs are rarely fully connected. Standard traversal wrappers gracefully handle this using the `root_vertex_id` parameter:

- **Specific Root:** If you pass a valid `vertex_id`, the algorithm will explore *only* the connected component reachable from that specific root.
- [**`hgl::algorithm::no_root`**](../../cpp-gl/group__HGL-Algorithm.md#variable-no_root) (Default): If you omit the root ID, the algorithm automatically iterates through the entire hypergraph, sequentially triggering searches on unvisited vertices to ensure that *every disconnected component* is fully traversed.

---

## Standard Traversals

The standard [**`breadth_first_search`**](../../cpp-gl/group__HGL-Algorithm.md#function-breadth_first_search) and [**`depth_first_search`**](../../cpp-gl/group__HGL-Algorithm.md#function-depth_first_search) algorithms provide unrestricted exploration of a hypergraph's topology.

For **undirected hypergraphs**, these algorithms simply flood through all incident hyperedges and vertices.

For **BF-directed hypergraphs**, they execute a naive directional traversal: a hyperedge is traversed immediately upon discovering *any* of its tail vertices, instantly reaching all of its head vertices.

### Example Usage

```cpp
#include <hgl/algorithm/traversal/breadth_first_search.hpp>
#include <hgl/algorithm/traversal/depth_first_search.hpp>
#include <iostream>

auto search_tree = hgl::algorithm::breadth_first_search(hypergraph, start_id); // (1)!
hgl::algorithm::depth_first_search<hgl::algorithm::noret>( // (2)!
    hypergraph,
    hgl::algorithm::no_root,
    [](const auto& node) { // (3)!
        std::cout << "Discovered vertex: " << node.vertex_id << '\n';
    }
);
```

1. Standard execution. Returns a search_tree representing the traversal from start_id.
2. Execution purely for side-effects over the entire hypergraph.
3. The `pre_visit` callback logs the discovery/visitation of a vertex to the console.

---

## Backward Searches (B-Reachability)

In many domains (such as chemical reaction networks or logic circuits), reaching a single input is not enough to trigger an event. The HGL module provides [**`backward_bfs`**](../../cpp-gl/group__HGL-Algorithm.md#function-backward_bfs) and [**`backward_dfs`**](../../cpp-gl/group__HGL-Algorithm.md#function-backward_dfs) to enforce strict **B-reachability** semantics on BF-directed hypergraphs.

This search travels forward through the network (using the forward star / outgoing hyperedges). Unlike standard traversals, a backward search utilizes a blocking predicate: **A hyperedge is only fully traversed (and its head vertices enqueued) after *all* of its tail (source) vertices have been visited.** This is exceptionally useful for forward propagation or cascading activation, determining the complete set of subsequent states or products that can be successfully activated given a specific set of initial conditions.

Because complex reachability often requires multiple initial inputs, these algorithms accept a `RootRange` of initial vertices instead of a single root.

### Example Usage

```cpp
#include <hgl/algorithm/traversal/backward_search.hpp>

std::vector<hgl::size_type> roots = { start_id_1, start_id_2 }; // (1)!
auto b_reachability_tree = hgl::algorithm::backward_bfs(hypergraph, roots); // (2)!
```

1. Supply multiple initial conditions (or inputs) to the network.
2. The algorithm blocks at hyperedges until all tail dependencies are met.

---

## Forward Searches (F-Reachability)

Conversely, [**`forward_bfs`**](../../cpp-gl/group__HGL-Algorithm.md#function-forward_bfs) and [**`forward_dfs`**](../../cpp-gl/group__HGL-Algorithm.md#function-forward_dfs) enforce strict **F-reachability** semantics on BF-directed hypergraphs.

This search travels in reverse through the network (using the backward star / incoming hyperedges). **A hyperedge is only traversed (and its tail vertices enqueued) after *all* of its head (destination) vertices have been visited.** This is exceptionally useful for backward chaining, determining what minimal set of inputs is strictly required to achieve a specific target state.

### Example Usage

```cpp
#include <hgl/algorithm/traversal/forward_search.hpp>

std::vector<hgl::size_type> targets = { target_id_1 }; // (1)!
auto f_reachability_tree = hgl::algorithm::forward_dfs(hypergraph, targets);
```

1. Define the target outputs we want to trace back from.

---

## Algorithmic Complexity and Layout Impact

Traversing a hypergraph involves evaluating vertices, hyperedges, and the full span of hyperedge incidences ($I$, defined as the total number of connections across the entire hypergraph: $I = \sum deg(v) = \sum |e|$).

Because the HGL generic templates utilize a two-step expansion (querying incident hyperedges, then querying adjacent vertices), **the algorithmic complexity of your traversal is drastically impacted by your chosen Representation Model and Layout Tag.**

### 1. Incidence Lists (`list_t` / `flat_list_t`)

- **`bidirectional_t` Layout (Optimal):** $O(|V| + |E| + I)$. Because the bidirectional layout natively supports $O(1)$ iteration from a vertex to its hyperedges AND from a hyperedge to its vertices, the engine traverses the topology effortlessly. **This is the only incidence list layout recommended for traversals.**
- **`vertex_major_t` Layout:** $O(|V| + |E| \cdot (|V| + I))$. Finding hyperedges from a vertex is fast, but step 2 (finding vertices within that hyperedge) requires a full-structure scan. Traversal performance is severely degraded.
- **`hyperedge_major_t` Layout:** $O(|E| + |V| \cdot (|E| + I))$. Finding vertices from a hyperedge is fast, but step 1 (finding incident hyperedges from a vertex) requires a full-structure scan. Traversal performance is severely degraded.

> [!WARNING] Asymmetric Layout Traversals
>
> If you are using a strictly single-major layout (`vertex_major_t` or `hyperedge_major_t`) due to memory constraints, executing a native BFS or DFS over the hypergraph will be excessively slow due to the full-structure scans required to jump between major and minor elements. If traversal is required, consider converting to a `bidirectional_t` list or using an Incidence Graph Projection via the GL module.

### 2. Incidence Matrices (`matrix_t` / `flat_matrix_t`)

- **Any Layout:** $O(|V| \cdot |E|)$. To find incident hyperedges, the algorithm must scan across $|E|$ elements for a given vertex. To find adjacent vertices, it must scan across $|V|$ elements for a given hyperedge.
