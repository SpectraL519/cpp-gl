# Spanning Trees

A Minimum Spanning Tree (MST) is a subset of the edges of a connected, edge-weighted undirected graph that connects all the vertices together without any cycles, while minimizing the total possible edge weight.

Because graph representations can vary drastically in density (sparse lists vs. dense matrices), a single algorithmic approach cannot guarantee optimal performance across all topologies. To uphold its zero-cost philosophy, CPP-GL provides two highly optimized, standalone implementations of Prim's algorithm tailored for specific memory models.

Both algorithms return a [**gl::algorithm::mst_descriptor<G>**](../../cpp-gl/structgl_1_1algorithm_1_1mst__descriptor.md), which contains the accumulated minimal `weight` and a `std::vector` of the `edges` that form the tree.

---

## Edge-Heap Prim's Algorithm

The [**edge_heap_prim_mst**](../../cpp-gl/group__GL-Algorithm.md#function-edge_heap_prim_mst) implementation utilizes a standard binary min-heap (`std::priority_queue`) to store and evaluate discovered edges based strictly on their individual weight.

As the tree expands, the algorithm evaluates all edges incident to the newly added vertex. If an edge leads to a vertex that is not yet part of the MST, the entire raw edge descriptor is pushed directly into the heap.

### Example Usage

```cpp
auto mst = gl::algorithm::edge_heap_prim_mst(graph, start_id); // (1)!
std::cout << "Total MST Weight: " << mst.weight // (2)!
          << "\nEdges: "
          << gl::io::muliline_set_formatter(mst.edges)
          << '\n';
```

1. Computes the MST starting from the given start_id. If gl::invalid_id is passed, it automatically defaults to the graph's initial_id.
2. Prints the accumulated minimal weight of the spanning tree and the raw edge descriptors that form the tree.

> [!INFO] Algorithmic Complexity
>
> The time complexity is bounded by the heap operations performed on the edges: $O(|E| \log |E|)$.
> - **Adjacency List Representations:** In simple graphs, this simplifies to $O(|E| \log |V|)$. However, because list models natively support multigraphs (parallel edges), the queue size and operations scale strictly with $O(|E|)$.
> - **Adjacency Matrix Representations:** Matrices require scanning the entire $|V|$-length row to find incident edges, altering the complexity to $O(|V|^2 + |E| \log |V|)$.
### When to use:

This is the optimal choice for Sparse Graphs (where $|E|$ is much closer to $|V|$ than $|V|^2$). It is highly recommended for Adjacency List models, especially when the topology contains parallel edges.

---

## Vertex-Heap Prim's Algorithm

The [**vertex_heap_prim_mst**](../../cpp-gl/group__GL-Algorithm.md#function-vertex_heap_prim_mst) implementation avoids queueing edges entirely. Instead, it maintains a strictly sized array of vertex IDs, where the priority is defined by the minimum known connection cost to that vertex.

Because standard C++ binary heaps do not natively support a decrease_key operation, this implementation dynamically manages a raw `std::vector` using `std::make_heap` and `std::pop_heap`. Whenever a cheaper path to an unvisited vertex is discovered, the connection cost is updated, and the entire heap is rebuilt in $O(|V|)$ time to re-establish the invariants.

### Example usage:

```cpp
auto mst = gl::algorithm::vertex_heap_prim_mst(graph, start_id); // (1)!
std::cout << "Total MST Weight: " << mst.weight // (2)!
          << "\nEdges: "
          << gl::io::muliline_set_formatter(mst.edges)
          << '\n';
```

1. Computes the MST starting from the given start_id. If gl::invalid_id is passed, it automatically defaults to the graph's initial_id.
2. Prints the accumulated minimal weight of the spanning tree and the raw edge descriptors that form the tree.

> [!INFO] Algorithmic Complexity
>
> Due to rebuilding the heap (an $O(|V|)$ operation) up to $|V|$ times, combined with evaluating every edge, the strict overall time complexity is $O(|V|^2 + |E|)$.
> - **Adjacency Matrix Representations:** Because matrix models inherently represent simple graphs (where $|E| \le |V|^2$), the edge traversal is completely absorbed, and the complexity strictly simplifies to a guaranteed $O(|V|^2)$.
> - **Adjacency List Representations:** For multigraphs, the edge count $|E|$ can far exceed $|V|^2$, meaning the edge traversal phase will bottleneck the performance.

### When to use:

This is the optimal, zero-cost choice for Dense Graphs (where $|E| \approx |V|^2$). It is strictly recommended for Adjacency Matrix models, as it completely eliminates the overhead of tracking an $O(|E|)$ sized queue.
