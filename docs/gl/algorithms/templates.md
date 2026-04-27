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

The true power of the generic templates lies in their callback/predicate hooks. Every iteration of the engine loop rigidly follows a defined sequence. By injecting custom lambdas (or omitting them via the [**empty_callback**](../../cpp-gl/structgl_1_1algorithm_1_1empty__callback.md)), you dictate the algorithm's behavior.

### Execution Flowchart

For a single popped node in `bfs`, `dfs`, or `pfs`, the execution flow looks exactly like this:

1. **`visit_vertex_pred(node)`**
   Evaluated immediately after popping the node. If it returns `false`, the node is skipped entirely, and the loop moves to the next node. *(Commonly used for late-rejection of stale elements in Priority Queues).*

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

5. **`post_visit(vertex_id)`**
   Executed after all adjacent edges have been evaluated and processed.

## Custom Node Injection (PFS)

While BFS and DFS templates strictly operate on the lightweight [**gl::algorithm::search_node<G>**](../../cpp-gl/structgl_1_1algorithm_1_1search__node.md), the Priority-First Search template often requires tracking dynamic state alongside the vertex ID.

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
```

1. Define a custom stateful node tracking the distance accumulated so far.
2. Initialize a global distance map with "infinity", setting the start vertex distance to 0.
3. Define the priority comparator for a distance-based Min-Heap.
4. Setup the initial range containing the root node.
5. Run the Priority-First Search engine.
6. Define an empty vertex visit predicate and vertex visit callback.
7. Define the node enqueue predicate to only enqueue nodes that could yield paths shorter than those already discovered.
8. Define the callback which constructs a stateful node for the algorithm queue.
9. Update the global distance map to reflect the newly discovered shorter path.

> [!NOTE] Algorithm Desing
>
> The example above is very similar, though not the same, to how the Dijkstra's algorithm implementation is designed within the library.
