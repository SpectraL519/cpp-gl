# The Generic Templates

The generic templates are the workhorse engines of the CPP-GL algorithm module. Rather than implementing distinct algorithms, they implement the strict structural iteration required by specific data structures.

All engines operate on a similar fundamental principle: pop a node from a frontier container, evaluate it, process its outgoing edges, and push valid targets back into the frontier.

## The Core Engines

The library provides four primary traversal engines.

* [**`bfs` (Breadth-First Search)**](../../cpp-gl/group__GL-Algorithm.md#function-bfs): Uses a `std::queue`. Explores the graph level by level, expanding uniformly outward from the initial range.
* [**`dfs` (Depth-First Search)**](../../cpp-gl/group__GL-Algorithm.md#function-dfs): Uses a `std::stack`. Plunges as deeply as possible along a branch before backtracking.
* [**`r_dfs` (Recursive DFS)**](../../cpp-gl/group__GL-Algorithm.md#function-r_dfs): Uses the C++ call stack. Instead of an initial range, it is initiated with a specific `start_id`. It operates identically to `dfs` but requires external logic to manage abort signals, as returning from the recursion only unwinds one level.
* [**`pfs` (Priority-First Search)**](../../cpp-gl/group__GL-Algorithm.md#function-pfs): Uses a `std::priority_queue`. Requires a custom comparator (`PQCmp`) to mathematically order the frontier. This is the underlying engine for algorithms like Dijkstra and Prim.

## The Callback Sequence

The true power of the generic templates lies in their callback hooks. Every iteration of the engine loop rigidly follows a defined sequence. By injecting custom lambdas (or omitting them via `empty_callback`), you dictate the algorithm's behavior.

### Execution Flowchart

For a single popped node in `bfs`, `dfs`, or `pfs`, the execution flow looks exactly like this:

1. **`visit_vertex_pred(node)`**
   Evaluated immediately after popping the node. If it returns `false`, the node is skipped entirely, and the loop moves to the next node. *(Commonly used for late-rejection of stale elements in Priority Queues).*

2. **`pre_visit(vertex_id)`**
   A state-modification hook executed right before the vertex is officially marked as "visited".

3. **`visit(vertex_id, pred_id)`**
   The primary callback. If this returns `false`, the entire search is immediately aborted.

4. **Edge Iteration**
   The engine iterates over every outgoing edge connected to the `vertex_id`. For each edge:

   * **`enqueue_vertex_pred(target_id, edge)`**
     Evaluates the target vertex. Returns a `gl::algorithm::decision`:
     * `abort`: Kills the entire algorithm.
     * `reject`: Ignores this edge and moves to the next.
     * `accept`: Approves the target for enqueueing.

   * **`make_node(target_id, vertex_id, edge)`** *(PFS Only)*
     If the target was accepted, this hook allows you to construct a custom object to push into the search frontier.

5. **`post_visit(vertex_id)`**
   Executed after all adjacent edges have been evaluated and processed.

## Custom Node Injection (`pfs`)

While `bfs` and `dfs` strictly operate on the lightweight `gl::algorithm::search_node`, the `pfs` (Priority-First Search) engine often requires tracking dynamic state alongside the vertex ID.

For instance, in Dijkstra's algorithm, the priority queue must sort nodes based on their accumulated distance from the starting point. You cannot sort based purely on the vertex ID.

`pfs` solves this by automatically inferring the `NodeType` from the initial queue range container. If your `NodeType` requires more than just `(target_id, pred_id)` to construct, you must provide a `MakeNodeCallback`.

### Example: PFS Stateful Nodes

```cpp
// 1. Define a custom stateful node
struct path_node {
    gl::default_id_type vertex_id;
    gl::default_id_type pred_id;
    int accumulated_distance;
};

// 2. Define the priority comparator
auto cmp = [](const path_node& a, const path_node& b) {
    return a.accumulated_distance > b.accumulated_distance; // Min-Heap
};

// 3. Setup the initial range
std::vector<path_node> init_range = { path_node{start_id, start_id, 0} };

// 4. Run the PFS engine
gl::algorithm::pfs(
    graph,
    cmp,
    init_range,
    gl::algorithm::empty_callback{}, // visit_vertex_pred
    gl::algorithm::empty_callback{}, // visit

    // enqueue_vertex_pred
    [&](auto target_id, const auto& edge) {
        return distance_map[target_id] > current_dist + edge.properties().weight;
    },

    // make_node - Construct the stateful object for the queue
    [&](auto target_id, auto source_id, const auto& edge) {
        int new_dist = current_dist + edge.properties().weight;
        return path_node{target_id, source_id, new_dist};
    }
);
