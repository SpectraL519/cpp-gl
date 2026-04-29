# Algorithms Overview

The HGL algorithms module adapts the strict architectural separation of concerns established by the GL module to the generalized topology of hypergraphs. Rather than writing monolithic functions that handle both search mechanics and specific problem-solving logic, the library splits hypergraph processing into two distinct layers: **Generic Templates (Engines)** and **Concrete Algorithms**.

This separation ensures maximum code reuse, guarantees zero-cost abstractions, and handles the inherent complexity of hypergraph traversals—where moving between nodes requires a two-step expansion (from a vertex to an incident hyperedge, and then to the adjacent vertices).

> [!NOTE] Shared Infrastructure with GL
> Because the HGL module is built strictly on top of the GL module, the algorithm architecture is highly symmetric. HGL directly imports and reuses much of the core algorithm utility from `gl::algorithm`, including callback concepts, zero-cost tags, and control-flow structs.

---

## The Architectural Separation

### 1. The Generic Templates (Engines)

Located at the lowest level, the generic templates define the strict structural execution of a search. They know nothing about specific reachability rules or the hypergraph's topology. Their only responsibility is to manage the pending elements (a queue or stack) and invoke a series of user-provided callback hooks at specific moments during the traversal.

Because traversing a hyperedge maps one-to-many (or many-to-many), the HGL engines expose specific hooks for *both* steps of the expansion. The core generic templates include:

- [**`bfs`**](templates.md#the-core-engines): A queue-based Breadth-First Search engine.
- [**`dfs`**](templates.md#the-core-engines): A stack-based, iterative Depth-First Search engine.

### 2. The Concrete Algorithms

Concrete algorithms (like `breadth_first_search`, `backward_bfs`, or `forward_dfs`) are simple wrappers around the generic engines. They are responsible for:
- Allocating and managing memory for algorithm state tracking (e.g., allocating `std::vector<bool>` arrays for tracking both visited vertices *and* visited hyperedges).
- Defining the specific logic inside the lambda callbacks and predicates.
- Managing the final return types and structures.

By separating these layers, CPP-HGL guarantees that all algorithms inherently benefit from the exact same optimized design and element management.

---

## Core Algorithm Elements

To interact with the generic templates or understand the concrete algorithms, you must be familiar with a few foundational types and tags used across the library.

### Zero-Cost Callbacks (Imported from GL)

The generic engines accept several different callbacks and predicates per invocation. However, forcing the compiler to execute or optimize away empty lambdas (e.g., `[]{}`) can add overhead in debug builds and slow down compilation.

To solve this, HGL reuses the [**empty_callback**](../../cpp-gl/structgl_1_1algorithm_1_1empty__callback.md) tag from the GL module. If a hook is not needed, the engine receives this tag, and compile-time evaluation (`if constexpr`) completely optimizes away that branch of execution.

### Tri-State Control Flow (Imported from GL)

When evaluating hyperedges or adjacent vertices during a search, algorithms often need more control than a simple `true`/`false`. HGL reuses the [**decision**](../../cpp-gl/structgl_1_1algorithm_1_1decision.md) struct to provide a tri-state evaluation:

- `decision::accept`: Proceed with the element (e.g., traverse the hyperedge, push the vertex to the queue).
- `decision::reject`: Skip this specific element, but continue the search.
- `decision::abort`: Instantly terminate the entire algorithm.

### The Hypergraph Search Node

Unlike a standard graph search node (which only tracks the current vertex and its predecessor), traversing a hypergraph requires knowing *how* a vertex was reached. By default, the active container of an HGL search engine stores `hgl::algorithm::search_node<H>` structures. This is a lightweight triplet containing:

1. `vertex_id`: The vertex currently being visited.
2. `pred_id`: The vertex from which this current vertex was reached (its parent in the traversal tree).
3. `he_id`: The specific hyperedge that was traversed to reach this vertex.

If a vertex is the starting point of a search, the library uses the imported `no_root` tag, setting the `pred_id` to itself and the `he_id` to an invalid state.

### The Result Discriminator (Imported from GL)

Certain algorithms can either return a fully constructed result (like a `search_tree<H>` mapping every vertex to its predecessor and incident hyperedge) or run purely for side-effects (like executing a custom lambda on every visited node).

HGL manages this via the imported `result_discriminator` enum:
- `ret`: The algorithm will allocate memory and return a stateful object (e.g., `search_tree<H>`).
- `noret`: The algorithm will execute purely for side-effects and return `void` (or a simple success boolean).

### Traversal Policies

To cleanly abstract traversals across undirected and BF-directed hypergraphs, HGL utilizes a `traversal_policy` injected with a `traversal_direction` tag (`forward` or `backward`). This policy dictates how the engine retrieves incident hyperedges (e.g., reading the "forward star" vs. the "backward star") and subsequent adjacent vertices (e.g., reading head nodes vs. tail nodes).

---

## Available Algorithms

Explore the specific layers of the algorithm module below:

- [**The Generic Templates**](templates.md): A detailed explanation of the core traversal engines and their two-step execution sequence.
- [**Standard Traversals**](traversal.md#standard-traversals): Unrestricted wrappers for basic component discovery across any hypergraph topology (`breadth_first_search`, `depth_first_search`).
- [**Backward Searches**](traversal.md#backward-searches-b-reachability): Traversals enforcing strict B-reachability semantics on BF-directed hypergraphs (`backward_bfs`, `backward_dfs`), where a hyperedge is only traversed once *all* of its tail vertices are visited.
- [**Forward Searches**](traversal.md#forward-searches-f-reachability): Traversals enforcing strict F-reachability semantics on BF-directed hypergraphs (`forward_bfs`, `forward_dfs`), where a hyperedge is only traversed once *all* of its head vertices are visited.
- [**Hypergraph Properties**](properties.md): Lightweight functional utilities for evaluating global structural characteristics, including degree bounds, rank, corank, regularity, and uniformity.
