# The Generic Templates

The generic templates are the workhorse engines of the CPP-GL algorithm module. Rather than implementing distinct algorithms, they implement the strict structural iteration required by specific data structures.

All engines operate on a similar fundamental principle: pop a node from a frontier container, evaluate it, process its outgoing edges, and push valid targets back into the frontier.

## The Core Engines

The library provides four primary traversal engines.

- [**`bfs` (Breadth-First Search)**](../../cpp-gl/group__GL-Algorithm.md#function-bfs): Uses a `std::queue`. Explores the graph level by level, expanding uniformly outward from the initial range.
- [**`dfs` (Depth-First Search)**](../../cpp-gl/group__GL-Algorithm.md#function-dfs): Uses a `std::stack`. Dives as deeply as possible along a branch before backtracking.
- [**`r_dfs` (Recursive DFS)**](../../cpp-gl/group__GL-Algorithm.md#function-r_dfs): Uses the C++ call stack. Instead of an initial range, it is initiated with a specific starting vertex ID. It operates identically to `dfs` but requires external logic to manage abort signals, as returning from the recursion only unwinds one level.
- [**`pfs` (Priority-First Search)**](../../cpp-gl/group__GL-Algorithm.md#function-pfs): Uses a `std::priority_queue`. Requires a custom comparator (`PQCmp`) to mathematically order the frontier. This is the underlying engine for algorithms like Dijkstra's shortest paths algorithm.

## The Callback Sequence

The true power of the generic templates lies in their callback/predicate hooks. Every iteration of the engine loop rigidly follows a defined sequence. By injecting custom callbacks (or omitting them via the [**empty_callback**](../../cpp-gl/structgl_1_1algorithm_1_1empty__callback.md)), you dictate the algorithm's behavior.

### Standard Execution Flow (`bfs`, `pfs`, and stateless `dfs`)

For a single popped node in standard traversal templates, the execution flow looks exactly like this:

1. **`visit_vertex_pred(node)`**
   Evaluated immediately after popping the node. If it returns `false`, the node is skipped entirely, and the loop moves to the next node. *(Commonly used for late-rejection of stale elements in Priority Queues or filtering already-visited vertices).*

2. **`pre_visit(vertex_id)`**
   A state-modification hook executed right before the vertex is officially marked as "visited".

3. **`visit(vertex_id, pred_id)`**
   The primary callback. If this returns `false`, the entire search is immediately aborted.

4. **Edge Iteration**
   The engine iterates over every outgoing edge connected to the `vertex_id`. For each edge it calls:

      - **`enqueue_node_pred(target_id, edge)`**

        Evaluates whether a new search node should be created for the target. Returns a [**decision**](../../cpp-gl/structgl_1_1algorithm_1_1decision.md):

          - `abort`: Kills the entire algorithm.
          - `reject`: Ignores this edge and moves to the next.
          - `accept`: Approves the target for enqueueing.

      - **`make_node(target_id, vertex_id, edge)`** *(PFS Only)*

        If the target was accepted, this hook allows you to construct a custom object to push into the search frontier.

5. **`post_visit(vertex_id)`** *(BFS/PFS only)*
   Executed after all adjacent edges have been evaluated and processed.

### True Post-Order Execution (Iterative `dfs`)

In a standard stack-based DFS, nodes are popped and discarded *before* their children are pushed. This makes executing a true post-order callback (after a node's entire subtree has been exhaustively explored) impossible with a naive implementation.

The CPP-GL `dfs` template solves this using a zero-cost abstraction:

- **Stateless Fast-Path:** If you pass an `empty_callback` for the `post_visit` hook, the engine compiles down to the standard execution flow described above, maximizing performance.
- **Stateful Stack-Frame:** If a valid `post_visit` callback is provided, the engine implicitly wraps the search nodes with a `dfs_extension` payload containing an `expanded` boolean flag.

When utilizing the stateful stack, the execution loop shifts to a two-phase lifecycle:

1. **Phase 1 (First Encounter):** The node is popped. Because `expanded == false`, the engine executes `visit_vertex_pred`, `pre_visit`, and `visit`. It then **marks the node as expanded and pushes it back onto the stack**, followed by pushing all of its valid children on top.
2. **Phase 2 (Subtree Exhausted):** Because the parent was pushed beneath its children, it surfaces again only after its entire subtree has been popped and processed. The engine pops it, sees `expanded == true`, and executes the `post_visit` callback.

### Recursive Execution (`r_dfs`)

The recursive DFS template (`r_dfs`) avoids standard container wrappers entirely and maps the generic callback sequence directly to the C++ call stack. Because of the nature of function calls, `r_dfs` achieves true post-order execution naturally without requiring stateful wrapper nodes.

Its execution flow operates as follows:

1. **Entry:** `visit_vertex_pred`, `pre_visit`, and `visit` are executed immediately upon entering the function.
2. **Recurse:** The engine iterates over outgoing edges. If `enqueue_node_pred` accepts a target, the engine immediately calls `r_dfs` nested within the current loop.
3. **Exit:** After the edge loop completes (meaning all recursive child calls have unwound), `post_visit` is naturally executed before the current function frame returns to its caller.

> [!WARNING] Aborting Recursive Searches
>
> The generic generic `abort` mechanisms (like returning `false` from `visit`) do not work the same way in `r_dfs`. Returning from a nested recursive call only unwinds a single stack frame. If you need to instantly terminate a deep `r_dfs` traversal, you must utilize external state (e.g., throwing a custom exception or checking a global cancellation flag in your predicates).

## Custom Node Injection (PFS)

While BFS and DFS templates strictly operate on the lightweight [**gl::algorithm::search_node**](../../cpp-gl/structgl_1_1algorithm_1_1search__node.md), the Priority-First Search template often requires tracking dynamic state alongside the vertex ID.

For instance, in Dijkstra's algorithm, the priority queue must sort nodes based on their accumulated distance from the starting point. You cannot sort based purely on the vertex ID.

`pfs` solves this by automatically inferring the `NodeType` from the initial queue range container. If your `NodeType` requires more than just `(target_id, pred_id)` to construct, you must provide `MakeNodeCallback` which is a `(vertex_id, pred_id, edge) -> NodeType` callback.

### Example: PFS Stateful Nodes

```cpp
struct path_node { // (1)!
    gl::default_id_type vertex_id;
    gl::default_id_type pred_id;
    int accumulated_distance;
};

std::vector<int> distance_map( // (2)!
  graph.n_vertices(), std::numeric_limits<int>::max()
);
distance_map[start_id] = 0;

auto cmp = [](const path_node& a, const path_node& b) { // (3)!
    return a.accumulated_distance > b.accumulated_distance;
};

std::vector<path_node> init_nodes = { // (4)!
    path_node{start_id, start_id, 0}
};

gl::algorithm::pfs( // (5)!
    graph,
    cmp,
    init_nodes,
    gl::algorithm::empty_callback{}, // (6)!
    gl::algorithm::empty_callback{},
    [&](auto target_id, const auto& edge) { // (7)!
        return distance_map[target_id] > distance_map[edge.source()] + edge.properties().weight;
    },
    [&](auto target_id, auto source_id, const auto& edge) { // (8)!
        int new_dist = distance_map[source_id] + edge.properties().weight;
        distance_map[target_id] = new_dist; // (9)!
        return path_node{target_id, source_id, new_dist};
    }
);
