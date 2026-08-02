# The Generic Templates

The generic templates are the workhorse engines of the HGL algorithm module. Rather than implementing distinct algorithms, they implement the strict structural iteration required to safely navigate a hypergraph's topology.

Traversing a hypergraph is fundamentally more complex than traversing a standard graph. Instead of moving directly from vertex to vertex, a hypergraph traversal requires a **two-step expansion**:

1. Expand from the current vertex to its incident hyperedges.
2. Expand from those hyperedges to their *target* vertices.

All engines operate on this principle: pop a search node from a pending container, evaluate it, execute the two-step expansion across valid hyperedges and vertices, and push the discovered targets back into the container.

## The Core Engines

The library provides two primary traversal engines for hypergraphs:

- [**`bfs` (Breadth-First Search)**](../../cpp-gl/group__HGL-Algorithm.md#function-bfs): Uses a `std::queue`. Explores the hypergraph level by level, expanding uniformly outward from the initial range.
- [**`dfs` (Depth-First Search)**](../../cpp-gl/group__HGL-Algorithm.md#function-dfs): Uses a `std::stack`. Dives as deeply as possible along a branch before backtracking.

## Traversal Directions & Policies

Because **BF-directed hypergraphs** distinguish between *tail* (source) and *head* (destination) vertices, the generic engines must know how to flow through them. This is controlled via the [**traversal_direction**](../../cpp-gl/group__HGL-Algorithm.md#enum-traversal_direction) template parameter (which defaults to `forward`) and resolved by an internal `traversal_policy`.

> [!NOTE] API Note
>
> The library utilizes C++20's `using enum` feature for the `traversal_direction` enum type, allowing you to access these tags directly via `hgl::algorithm::forward` and `hgl::algorithm::backward`.

- **`forward`**:

    - Retrieves hyperedges originating from the current vertex (the **forward star** / `out_hyperedge_ids`).
    - Retrieves the vertices targeted by those hyperedges (the **head nodes** / `head_ids`).

- **`backward`**:

    - Retrieves hyperedges entering the current vertex (the **backward star** / `in_hyperedge_ids`).
    - Retrieves the vertices originating those hyperedges (the **tail nodes** / `tail_ids`).

> [!NOTE] Undirected Hypergraphs
>
> For undirected hypergraphs, the traversal policy seamlessly falls back to retrieving all `incident_hyperedge_ids` and `incident_vertex_ids` regardless of the traversal direction.

## The Callback Sequence

The true power of the generic templates lies in their callback and predicate hooks. Every iteration of the engine loop rigidly follows a defined sequence. By injecting custom callbacks (or omitting them via the imported [**empty_callback**](../../cpp-gl/group__HGL-Algorithm.md#typedef-empty_callback)), you dictate the algorithm's exact behavior.

### Standard Execution Flow (`bfs` and stateless `dfs`)

For a single popped node in the standard traversal templates, the execution flow looks exactly like this:

1. **`visit_pred(curr_node)`**
   Evaluated immediately after popping the node. If it returns `false`, the node is skipped entirely, and the loop moves to the next node in the queue or stack.

2. **`pre_visit(curr_node)`**
   A state-modification hook executed right before the vertex is officially processed.

3. **`visit(curr_node)`**
   The primary callback. If this returns `false`, the entire search is immediately aborted.

4. **Hyperedge Expansion (Step 1)**
   The engine queries the `traversal_policy` for the target hyperedges. For each `he_id`:

   - **`traverse_he_pred(he_id, curr_node.vertex_id)`**
     Evaluates whether the hyperedge should be traversed. Returns a [**decision**](../../cpp-gl/structgl_1_1algorithm_1_1decision.md):
     - `abort`: Kills the entire algorithm.
     - `reject`: Ignores this hyperedge and moves to the next.
     - `accept`: Proceeds to evaluate the hyperedge's vertices.

5. **Vertex Expansion (Step 2)**
   If the hyperedge was accepted, the engine queries the `traversal_policy` for the target vertices within that hyperedge. For each `target_id` (skipping the one we just came from):

   - **`enqueue_pred(tgt_node)`**
     Evaluates whether the newly constructed `tgt_node` (containing the `target_id`, `curr_node.vertex_id`, and `he_id`) should be pushed to the active container. Returns a [**decision**](../../cpp-gl/structgl_1_1algorithm_1_1decision.md):
     - `abort`: Kills the entire algorithm.
     - `reject`: Ignores this specific target vertex.
     - `accept`: Pushes the `tgt_node` to the queue or stack.

6. **`post_visit(curr_node)`** *(BFS only)*
   Executed after all adjacent hyperedges and their target vertices have been evaluated and processed.

### True Post-Order Execution (Iterative `dfs`)

In a standard stack-based DFS, nodes are popped and discarded *before* their children are pushed. This makes executing a true post-order callback (firing only after a node's entire subtree has been exhaustively explored) impossible with a naive implementation.

The HGL `dfs` template solves this using a zero-cost abstraction:

- **Stateless Fast-Path:** If you pass an `empty_callback` for the `post_visit` hook, the engine compiles down to the standard execution flow described above, maximizing performance.
- **Stateful Stack-Frame:** If a valid `post_visit` callback is provided, the engine implicitly wraps the search nodes with a `dfs_extension` payload containing an `expanded` boolean flag.

When utilizing the stateful stack, the execution loop shifts to a two-phase lifecycle:

1. **Phase 1 (First Encounter):** The node is popped. Because `expanded == false`, the engine executes `visit_pred`, `pre_visit`, and `visit`. It then **marks the node as expanded and pushes it back onto the stack**, followed by executing the two-step hyperedge/vertex expansion to push all of its valid children on top.
2. **Phase 2 (Subtree Exhausted):** Because the parent was pushed beneath its children, it surfaces again only after its entire subtree has been popped and processed. The engine pops it, sees `expanded == true`, slices it back to its stateless base representation, and executes the `post_visit` callback.

## Customizing the Traversal

By wiring up these hooks, you can build highly specific reachability algorithms. For instance, to ensure we do not get stuck in infinite loops, we need to track both visited vertices and visited hyperedges.

### Example: Custom Forward BFS Engine

```cpp
#include <hgl/algorithm/templates/bfs.hpp>
#include <hgl/algorithm/core.hpp>
#include <vector>

// Assume 'hg' is a populated BF-directed hypergraph and 'start_id' is valid.
std::vector<bool> visited_v(hg.n_vertices(), false); // (1)!
std::vector<bool> visited_he(hg.n_hyperedges(), false);

bool success = hgl::algorithm::bfs<hgl::algorithm::forward>( // (2)!
    hg,
    std::array{hgl::algorithm::root_node<decltype(hg)>(start_id)}, // (3)!
    [&](const auto& node) { return not visited_v[node.vertex_id]; }, // (4)!
    [&](const auto& node) { // (5)!
        visited_v[node.vertex_id] = true;
        return true; // continue search
    },
    [&](auto he_id, auto /*source_id*/) -> hgl::algorithm::decision { // (6)!
        if (visited_he[he_id])
            return hgl::algorithm::decision::reject;

        visited_he[he_id] = true;
        return hgl::algorithm::decision::accept;
    },
    [&](const auto& tgt_node) -> hgl::algorithm::decision { // (7)!
        return !visited_v[tgt_node.vertex_id];
    }
);
```

1. Initialize state-tracking vectors for both vertices and hyperedges.
2. Explicitly invoke the template with `traversal_direction::forward` (the default, but explicitly shown here for clarity).
3. Set up the initial queue range with a root node.
4. **`visit_pred`**: Reject nodes in the queue if they were already visited by an earlier, faster branch.
5. **`visit`**: Mark the vertex as visited.
6. **`traverse_he_pred`**: Check if the hyperedge was already traversed. If not, mark it traversed and `accept` it. Returning a `decision` type here is required by the generic engines.
7. **`enqueue_pred`**: Only enqueue adjacent vertices that haven't been visited yet (implicitly casts the boolean condition to a `decision::accept` / `decision::reject`).
