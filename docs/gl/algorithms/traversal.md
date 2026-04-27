# Concrete Traversals

While the [Generic Templates](templates.md) provide immense power and flexibility, setting up the `visited` arrays, predecessor maps, and standard predicates manually for simple tasks can be overly verbose.

To solve this, CPP-GL provides concrete traversal algorithms: **Breadth-First Search (BFS)** and **Depth-First Search (DFS)**. These functions act as lightweight, zero-cost wrappers around the core generic engines. They automatically allocate and manage the necessary state-tracking containers while still allowing you to inject custom logic via `pre_visit` and `post_visit` hooks.

---

## Shared Behaviors and Mechanics

All concrete traversals in CPP-GL share a unified design philosophy regarding return types and component handling.

### The Result Discriminator

By default, concrete traversals are designed to build a traversal tree. However, sometimes you only want to traverse a graph for its side effects (e.g., printing node names) without paying the memory allocation cost of building a predecessor map.

You can explicitly control this using the template's `Result` discriminator:

- [**gl::algorithm::ret**](../../cpp-gl/group__GL-Algorithm.md#enum-result_discriminator) (Default): The algorithm allocates and populates a [**predecessors_map**](../../cpp-gl/group__GL-Algorithm.md#typedef-predecessors_map). It returns `std::vector<id_type>` where each index holds the ID of the vertex that discovered it.
- [**gl::algorithm::noret**](../../cpp-gl/group__GL-Algorithm.md#enum-result_discriminator): The algorithm executes purely for side effects. The predecessor map is completely optimized away at compile time, and the function returns `void`.

### Handling Disconnected Components

Graphs are not always fully connected. The traversal wrappers gracefully handle this using the `root_vertex_id` parameter:

- **Specific Root:** If you pass a valid `vertex_id`, the algorithm will explore *only* the connected component reachable from that specific root.
- [**gl::algorithm::no_root**](../../cpp-gl/group__GL-Algorithm.md#variable-no_root) (Default): If you omit the root ID, the algorithm automatically iterates through the entire graph, sequentially triggering searches on unvisited vertices to ensure that *every disconnected component* is fully traversed.

---

## Breadth-First Search (BFS)

The [**breadth_first_search**](../../cpp-gl/group__GL-Algorithm.md#function-breadth_first_search) algorithm explores the graph level by level, expanding uniformly outward from the starting vertex.

It utilizes the generic [**bfs**](templates.md#the-core-engines) engine under the hood, making it ideal for discovering the shortest path in unweighted graphs.

### Example Usage

```cpp
auto pred_map = gl::algorithm::breadth_first_search(graph, start_id); // (1)!

gl::algorithm::breadth_first_search<gl::algorithm::noret>( // (2)!
    graph,
    gl::algorithm::no_root, // (3)!
    [](auto v) { std::cout << "Discovered: " << v << '\n'; } // (4)!
);
```

1. Standard execution. Returns a predecessor map representing the traversal tree from start_id.
2. Execution purely for side-effects over the entire graph. The noret discriminator completely compiles away the predecessor map allocations.
3. Traverses all vertices in the graph, automatically jumping to new roots if disconnected components are found.
4. A custom PreVisitCallback executed exactly when a vertex is marked as visited.

<!-- TODO: Diagram -->

---

## Depth-First Search (DFS)

Depth-First Search dives as deeply as possible along a single branch before backtracking. CPP-GL provides two distinct implementations of DFS depending on your memory constraints and performance needs.

### 1. Iterative DFS

The standard [**depth_first_search**](../../cpp-gl/group__GL-Algorithm.md#function-depth_first_search) function utilizes the generic [**dfs**](templates.md#the-core-engines) engine, which manages a heap-allocated `std::stack`. This is the safest and most robust way to perform a depth-first traversal, as it is immune to stack overflow errors regardless of the graph's depth.

```cpp
auto pred_map = gl::algorithm::depth_first_search(graph); // (1)!
```

1. Executes a safe, iterative DFS over all components, returning the predecessor forest.

### 2. Recursive DFS

The [**recursive_depth_first_search**](../../cpp-gl/group__GL-Algorithm.md#function-recursive_depth_first_search) function utilizes the [**r_dfs**](templates.md#the-core-engines) engine. Instead of a heap-allocated stack, it uses the native C++ call stack.

```cpp
gl::algorithm::recursive_depth_first_search<gl::algorithm::noret>(
    graph,
    start_id,
    [](auto v) { std::cout << "Entering subtree of: " << v << '\n'; }, // (1)!
    [](auto v) { std::cout << "Exiting subtree of: " << v << '\n'; }   // (2)!
);
```

1. Hook triggered before descending recursively into the current vertex's adjacent neighbors.
2. Hook triggered after returning from all recursive calls for the current vertex.

> [!WARNING] Call Stack Depth (Stack Overflow)
>
> While `recursive_depth_first_search` avoids heap allocations and can be marginally faster for small graphs, it is highly vulnerable to Stack Overflow errors. If you execute this on an extremely deep graph (such as a linear path topology with 100,000 vertices), the C++ call stack will exceed its limit and crash your application.
>
> For large or untrusted graphs, always prefer the iterative `depth_first_search`.

---

## Visual Comparison: Traversal Order

The following diagram illustrates the fundamental difference in exploration order between Breadth-First and Depth-First search. The numbers indicate the exact chronological sequence in which vertices are discovered.

<div align="center" markdown="1">

![Traversal Example Light](../../img/doc/light/traversal-example.svg#only-light){: width="700" }
![Traversal Example Dark](../../img/doc/dark/traversal-example.svg#only-dark){: width="700" }

</div>

---

## Algorithmic complexity

The time complexity for all three traversals (BFS, Iterative DFS, and Recursive DFS) depends entirely on the underlying representation of `GraphType`:

- **Adjacency List Representations:** $\mathcal{O}(|V| + |E|)$. The algorithm strictly evaluates existing edges, making this optimal for sparse graphs.

- **Adjacency Matrix Representations:** $\mathcal{O}(|V|^2)$. To find adjacent unvisited vertices, the algorithm must scan the entire $|V|$-length matrix row for every visited vertex, shifting the bottleneck from edge evaluation to row traversal.
