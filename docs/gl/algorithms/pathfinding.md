# Pathfinding

Pathfinding algorithms are used to find the optimal route between vertices in a graph. The most common variation is the **Single-Source Shortest Path (SSSP)** problem, which calculates the shortest possible path from one specific starting vertex to all other reachable vertices in a weighted graph.

To solve this, CPP-GL provides a highly optimized implementation of Dijkstra's algorithm, built directly on top of the library's zero-cost Priority-First Search (`pfs`) engine.

---

## Dijkstra's Algorithm Overview

Dijkstra's algorithm is a greedy search method that systematically expands outward from the source vertex. It maintains a priority queue of discovered vertices, sorted by their accumulated distance from the start.

1. **Initialize:** The algorithm assigns a distance of `0` to the starting vertex and conceptually assigns an *infinite* distance to all other vertices.
2. **Evaluate:** It repeatedly extracts the vertex with the lowest accumulated distance from the queue.
3. **Update Distances:** For every outgoing edge from the extracted vertex, it calculates the *potential* new distance to the target vertex. If this new distance is strictly shorter than the previously recorded distance to that target, the distance is updated, and the target is pushed into the queue.
4. **Repeat:** The process continues until the priority queue is empty.

<div align="center" markdown="1">

![Dijkstra Example Light](../../img/doc/light/dijkstra-example.svg#only-light){: width="700" }
![Dijkstra Example Dark](../../img/doc/dark/dijkstra-example.svg#only-dark){: width="700" }

</div>

> [!WARNING] Negative Edge Weights
>
> Dijkstra's algorithm operates on the mathematical assumption that adding an edge to a path can never decrease its total length. Therefore, it strictly requires **non-negative edge weights**.
>
> To ensure safety, `gl::algorithm::dijkstra_shortest_paths` actively checks edge weights during traversal. If it encounters a negative weight, it will immediately abort the search and throw a `std::invalid_argument` exception.

---

## The Paths Descriptor

Executing the algorithm returns a [**gl::algorithm::paths_descriptor<G, VertexDistanceType>**](../../cpp-gl/structgl_1_1algorithm_1_1paths__descriptor.md). This object acts as a snapshot of the optimal traversal tree, containing two aligned arrays:

- **`predecessors`**: A map linking each vertex ID to the ID of the vertex that precedes it on the shortest path.
- **`distances`**: A map containing the absolute shortest distance from the source to each respective vertex.

If a vertex is completely unreachable from the source, its predecessor will be marked as `gl::invalid_id`.

---

## Executing the Search

The [**dijkstra_shortest_paths**](../../cpp-gl/group__GL-Algorithm.md#function-dijkstra_shortest_paths) algorithm computes the optimal routes. Because it is built on the `pfs` template, it evaluates edges and dynamically updates the container without duplicating standard template logic.

### Example Usage

```cpp
auto paths = gl::algorithm::dijkstra_shortest_paths(graph, source_id); // (1)!

const auto target_id = 5uz; // (2)!
auto path_to_target = gl::algorithm::reconstruct_path(paths.predecessors, target_id);

std::cout << "Path: "
          << gl::io::range_formatter(path_to_target, " -> ", "", "") // (3)!
          << "\nTotal Distance: " << paths.distances[target_id] << '\n'; // (4)!
```

1. Executes the shortest path calculation from the given source_id. It automatically deduces the proper distance type based on the graph's edge properties.
2. Reconstructs the exact sequence of vertices from the source to the target_id using the predecessor map.
3. Prints the sequence (e.g., 0 -> 2 -> 3 -> 5).
4. Retrieves the total accumulated distance from the paths.distances array.

> [!INFO] Algorithmic Complexity
>
> The time complexity depends heavily on the underlying representation of GraphType and the priority queue overhead:
> - **Adjacency List Representations:** $O((|V| + |E|) \log |V|)$. The queue operations scale with the number of edges, making this optimal for sparse graphs.
> - **Adjacency Matrix Representations:** $O(|V|^2 + |E| \log |V|)$. Iterating over adjacent vertices requires scanning the entire $|V|$-length matrix row, shifting the bottleneck to row traversal.

---

## Reconstructing Paths

While the `paths_descriptor` stores the raw traversal tree, users usually need the explicit sequence of vertices from the source to a specific destination.

The [**gl::algorithm::reconstruct_path**](../../cpp-gl/group__GL-Algorithm.md#function-reconstruct_path) utility function automatically walks backward through the predecessors map, starting from the target vertex, until it reaches the root. It then reverses the sequence and returns a `std::vector<id_type>` representing the forward path. This is an $O(\vert V \vert)$ operation.

> [!WARNING] Unreachable Vertices
>
> You must ensure the target vertex was successfully reached during the traversal. If you attempt to reconstruct a path to a vertex whose predecessor is `gl::invalid_id` (meaning it is unreachable from the source), `reconstruct_path` will throw a `std::invalid_argument` exception.
>
> **NOTE:** With a valid predecessors map you can easily validate whether a vertex is reachable using the [**gl::algorithm::is_reachable**](../../cpp-gl/group__GL-Algorithm.md#function-is_reachable) function in $O(1)$ time.
