# Algorithms Overview

The CPP-GL algorithms module is designed with a strict architectural separation of concerns. Rather than writing monolithic functions that handle both search mechanics and specific problem-solving logic, the library splits graph processing into two distinct layers: **Generic Templates (Engines)** and **Concrete Algorithms**.

This separation ensures maximum code reuse, provides strict zero-cost abstractions, and allows power users to inject highly customized logic directly into the traversal control flow.

## The Architectural Separation

### 1. The Generic Templates (Engines)

Located at the lowest level, the generic templates (`bfs`, `dfs`, `r_dfs`, `pfs`) define the strict structural execution of a search. They know nothing about "shortest paths", "spanning trees", or "cycle detection". Their only responsibility is to manage the search frontier (a queue, stack, or priority queue) and invoke a series of user-provided callback hooks at specific moments during the traversal.

### 2. The Concrete Algorithms

Concrete algorithms (like `dijkstra_shortest_paths`, `topological_sort`, or `breadth_first_search`) are essentially lightweight wrappers around the generic engines. They are responsible for:
- Allocating and managing memory for algorithm state tracking (e.g., `std::vector<bool> visited` arrays, distance maps).
- Defining the specific logic inside the lambda callbacks and predicates.
- Managing the final return types and structures.

By separating these layers, CPP-GL guarantees that all algorithms inherently benefit from the exact same optimized design and frontier management.

## Core Algorithm Elements

To interact with the generic templates or understand the concrete algorithms, you must be familiar with a few foundational types and tags used uniformly across the library.

### Zero-Cost Callbacks

The generic engines accept up to 5 or 6 different callbacks per invocation. However, forcing the compiler to execute or optimize away empty lambdas (e.g., `[]{}`) can add overhead in debug builds and slow down compilation.

To solve this, CPP-GL uses the [**gl::algorithm::empty_callback**](../../cpp-gl/structgl_1_1algorithm_1_1empty__callback.md) tag. If a hook is not needed, the engine receives this tag, and template metaprogramming (`if constexpr`) physically eliminates that branch of execution at compile time.

### Tri-State Control Flow

When evaluating adjacent edges during a search, algorithms often need more control than a simple `true`/`false`. The [**gl::algorithm::decision**](../../cpp-gl/structgl_1_1algorithm_1_1decision.md) struct provides a tri-state evaluation:

- `decision::accept`: Proceed with the element (e.g., push it to the queue).
- `decision::reject`: Skip this specific element, but continue the search.
- `decision::abort`: Instantly terminate the entire algorithm.

For convenience, `decision` implicitly constructs from a boolean, where `true` maps to `accept` and `false` maps to `reject`.

### The Search Node

By default, the active frontier of a search engine stores [**gl::algorithm::search_node<G>**](../../cpp-gl/structgl_1_1algorithm_1_1search__node.md) structures. This is a lightweight pair containing:

1. `vertex_id`: The vertex currently being visited.
2. `pred_id`: The vertex from which this current vertex was reached (its parent in the traversal tree).

If a vertex is the starting point of a search, its `pred_id` is set to itself, making it a "root" node. The library provides the [**gl::algorithm::no_root**](../../cpp-gl/group__GL-Algorithm.md#variable-no_root) tag to explicitly identify states where a node lacks a predecessor.

### The Result Discriminator

Certain algorithms can either return a fully constructed result (like a tree mapping every vertex to its predecessor) or run purely for side-effects (like executing a lambda on every visited node).

CPP-GL manages this via the [**gl::algorithm::result_discriminator**](../../cpp-gl/group__GL-Algorithm.md#enum-result_discriminator) enum.
- `ret`: The algorithm will allocate memory and return a stateful object (e.g., `predecessors_map<G>`).
- `noret`: The algorithm will execute purely for side-effects and return `void` (or a simple success boolean).

## Algorithm Categories

Explore the specific layers of the algorithm module below:

- [**The Generic Templates**](templates.md): Deep dive into the callback architecture of `bfs`, `dfs`, `r_dfs`, and `pfs`.
- [**Concrete Traversals**](traversal.md): Standard wrappers for basic component discovery and state-tracked searching.
- [**Pathfinding**](pathfinding.md): Algorithms for finding the shortest path between nodes (e.g., Dijkstra).
- [**Spanning Trees**](spanning_trees.md): Algorithms for calculating Minimum Spanning Trees (e.g., Prim's).
- [**Topology Solvers**](topology_solvers.md): Graph analysis tools like topological sorting and bipartite coloring.
