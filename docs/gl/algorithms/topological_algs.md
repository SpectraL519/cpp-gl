# Topological Algorithms

Topology solvers are algorithms designed to evaluate, validate, or compute the structural properties of a graph. Rather than finding a path or a sub-tree, these algorithms analyze the global layout of the vertices and edges to determine if the graph satisfies specific mathematical constraints.

CPP-GL provides highly optimized solvers for **Bipartite Coloring** and **Topological Sorting**.

---

## Cycle Detection and Failure States

Both bipartite coloring and topological sorting rely on the graph being free of certain types of cycles (odd-length cycles for bipartite graphs, and any directed cycle for DAGs).

Instead of throwing exceptions when a topology violation is encountered, CPP-GL elegantly handles these structural failures by returning a `std::optional`.

If the algorithm detects a cycle that violates the required topology, the search immediately aborts and returns `std::nullopt`. This allows you to safely use these algorithms as structural validators in your control flow.

---

## Bipartite Coloring

A bipartite graph (or 2-colorable graph) is a graph whose vertices can be divided into two disjoint sets such that every edge connects a vertex in one set to a vertex in the other. In other words, there are no edges connecting vertices of the same *color*.

The [**bipartite_coloring**](../../cpp-gl/group__GL-Algorithm.md#function-bipartite_coloring) algorithm utilizes the generic [**bfs**](templates.md#the-core-engines) engine to traverse the graph and strictly alternate colors between adjacent vertices.

<div align="center" markdown="1">

![Bipartite Coloring Example Light](../../img/doc/light/bicoloring-example.svg#only-light){: width="500" }
![Bipartite Coloring Example Dark](../../img/doc/dark/bicoloring-example.svg#only-dark){: width="500" }

</div>

### Example Usage

```cpp
if (auto coloring = gl::algorithm::bipartite_coloring(graph)) { // (1)!
    gl::algorithm::apply_coloring(graph, *coloring); // (2)!
    std::cout << "Successfully 2-colored the graph!\n";
}
else {
    std::cout << "Graph contains an odd cycle and is not bipartite.\n";
}
```

> [!TIP] Simple Validation
>
> If you only need to check if a graph is bipartite without extracting the actual color map, you can use the lightweight [**is_bipartite**](../../cpp-gl/group__GL-Algorithm.md#function-is_bipartite) convenience wrapper, which returns a simple boolean.

> [!INFO] Algorithmic Complexity
>
> The time complexity depends on the underlying representation of `GraphType`:
> - **Adjacency List Representations:** $\mathcal{O}(|V| + |E|)$.
> - **Adjacency Matrix Representations:** $\mathcal{O}(|V|^2)$. Iterating over adjacent vertices requires scanning the entire $|V|$-length matrix row.

---

## Topological Sort

A Topological Sort is a linear ordering of the vertices in a Directed Acyclic Graph (DAG) such that for every directed edge $(u, v)$ ($u \rightarrow v$), vertex $u$ comes before vertex $v$ in the ordering. This is commonly used for resolving dependency trees, task scheduling, or build systems.

The [**topological_sort**](../../cpp-gl/group__GL-Algorithm.md#function-topological_sort) function uses Kahn's Algorithm. It seeds a bfs queue with all vertices that have an in-degree of 0 (no incoming edges). As it processes vertices, it dynamically decrements the in-degrees of their adjacent neighbors.

<div align="center" markdown="1">

![Topological Sort Example Light](../../img/doc/light/topo-sort-example.svg#only-light){: width="700" }
![Topological Sort Example Dark](../../img/doc/dark/topo-sort-example.svg#only-dark){: width="700" }

</div>


### Example Usage

```cpp

if (auto top_order = gl::algorithm::topological_sort(graph)) { // (1)!
    std::cout << "Topological Order: "
              << gl::io::range_formatter(top_order.value(), " -> ", "", "") // (2)!
              << '\n';
}
else {
    std::cout << "Dependency cycle detected! Graph is not a DAG.\n";
}
```

1. Attempts to compute the ordering. If the final sorted sequence does not contain all vertices in the graph, it mathematically proves the presence of a cycle. The algorithm then safely returns `std::nullopt`.
2. Prints the topologically sorted `std::vector<id_type>` using the [**range_formatter**](../../cpp-gl/structgl_1_1io_1_1range__formatter.md) helper.

> [!INFO] Algorithmic Complexity
>
> The time complexity depends on the underlying representation of `GraphType`:
> - **Adjacency List Representations:** $\mathcal{O}(|V| + |E|)$.
> - **Adjacency Matrix Representations:** $\mathcal{O}(|V|^2)$. Iterating over adjacent vertices requires scanning the entire $|V|$-length matrix row.
