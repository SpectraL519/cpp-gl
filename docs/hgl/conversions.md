# Graph Conversions

While the **HGL** module treats hypergraphs as first-class, n-ary topological entities, there are many scenarios where you may need to interface with traditional graph algorithms, export data for standard graph visualization tools, or simplify relations.

To bridge this gap, CPP-GL provides zero-cost abstractions to convert or project hypergraphs from the HGL module into standard graphs from the **GL** module.

This section covers the two primary conversion methodologies and the fundamental differences in how they preserve or discard structural data:

- [Incidence Graph Conversion](#incidence-graph-conversion): Lossless conversion to a bipartite graph.
- [Projection Conversions](#projection-conversions): Lossy conversion that collapses hyperedges.

---

## Information Loss & Structural Integrity

When converting a hypergraph to a standard graph, you must decide how to handle the n-ary nature of hyperedges.

---

## Incidence Graph Conversion

The incidence graph conversion (also known as *star expansion*) translates a hypergraph $H = (V, E)$ into a standard bipartite graph $G = (V', E')$, where $V' = V \cup E$. Every hyperedge in the original hypergraph becomes a distinct vertex in the new graph, and standard edges are drawn between the original vertices and the new "hyperedge" vertices based on their incidence.

> [!IMPORTANT] Topological Preservation
>
> **Incidence Graph Conversions** are **lossless**. They preserve the exact mathematical topology of the hypergraph by converting hyperedges into secondary "dummy" vertices, creating a strict Bipartite Graph.

### Undirected Hypergraphs

For an undirected hypergraph, the resulting incidence graph is a standard **Undirected Bipartite Graph**. If a vertex $v$ belongs to a hyperedge $e$, an undirected edge $\{v, e\}$ is created.

#### Formal Definition:

Given an undirected hypergraph $H = (V, E)$, its incidence graph is an undirected bipartite graph $G = (V \cup E, E')$, where the new edge set is defined as:

$$
E' = \big\{ \{v, e\} : v \in V \land e \in E \land v \in e \big\}
$$

<div align="center" markdown="1">

![Undirected Hypergraph - Star Expansion - Light](../img/doc/light/undir-star-exp.svg#only-light){: width="700" }
![Undirected Hypergraph - Star Expansion - Dark](../img/doc/dark/undir-star-exp.svg#only-dark){: width="700" }

</div>

### BF-Directed Hypergraphs

For a BF-directed hypergraph, the resulting incidence graph is a **Directed Bipartite Graph**. The directional flow is strictly preserved:
- If a vertex $v$ is in the **tail** of $e$, a directed edge $(v, e)$ is created.
- If a vertex $v$ is in the **head** of $e$, a directed edge $(e, v)$ is created.

#### Formal Definition:

Given a BF-directed hypergraph $H = (V, E)$, its incidence graph is a directed bipartite graph $G = (V \cup E, E')$, where the new directed edge set $E'$ connects *tail* vertices to hyperedge-nodes and hyperedge-nodes to *head* vertices:

$$
E' = \big\{ (v, e) : v \in V \land e \in E \land v \in T(e) \big\} \cup \big\{ (e, v) : v \in V \land e \in E \land v \in H(e) \big\}
$$

<div align="center" markdown="1">

![BF-Directed Hypergraph - Star Expansion - Light](../img/doc/light/bfdir-star-exp.svg#only-light){: width="700" }
![BF-Directed Hypergraph - Star Expansion - Dark](../img/doc/dark/bfdir-star-exp.svg#only-dark){: width="700" }

</div>

### Example Usage

The [**hgl::incidence_graph**](../cpp-gl/group__HGL-Core.md#function-incidence_graph) utility handles this transformation automatically, ensuring IDs are safely shifted to prevent collisions between the original vertices and the newly promoted hyperedge vertices.

```cpp
#include <hgl/conversion.hpp>

auto bipartite_graph = hgl::incidence_graph<gl::undirected_graph<>>(hg); // (1)!
```

1. We assume `hg` is an instance of a *undirected* hypergraph.

---

## Projection Conversions

Projections are useful when you only care about the direct relationships between the actual data elements and want to discard the concept of the hyperedges entirely.

> [!IMPORTANT] Topological Preservation
>
> **Projection Conversions** are **lossy**. They collapse the hyperedges, creating direct edges between the original vertices. This inherently destroys the higher-order grouping information (e.g., you can no longer tell if three vertices were connected by one 3-ary hyperedge or three separate pairwise edges).

### Clique Expansion (Undirected Hypergraphs)

When projecting an undirected hypergraph, every hyperedge is replaced by a "clique" (a fully connected subgraph) amongst its incident vertices. If vertices $v_1, v_2, v_3$ share a hyperedge, the projection generates pairwise edges $\{v_1, v_2\}, \{v_2, v_3\}$, and $\{v_1, v_3\}$. Hyperedges containing only a single vertex typically project to a self-loop.

#### Formal Definition:

Given an undirected hypergraph $H = (V, E)$, its clique expansion is an undirected graph $G = (V, E')$, where every hyperedge is replaced by a fully connected subgraph of its incident vertices:

$$
E' = \big\{ \{u, v\} : (\exists e \in E)(u, v \in e)\big\}
$$

<div align="center" markdown="1">

![Undirected Hypergraph - Clique Expansion - Light](../img/doc/light/undir-clique-exp.svg#only-light){: width="700" }
![Undirected Hypergraph - Clique Expansion - Dark](../img/doc/dark/undir-clique-exp.svg#only-dark){: width="700" }

</div>

### Flow Graph / Bipartite Projection (BF-Directed)

When projecting a BF-directed hypergraph, the hyperedge acts as a directional bridge. The projection generates a standard directed edge from every vertex in the hyperedge's *tail* to every vertex in its *head*.

If a hyperedge has tail vertices $\{v_1, v_2\}$ and head vertices $\{v_3, v_4\}$, the projection yields four directed edges: $(v_1, v_3), (v_1, v_4), (v_2, v_3)$, and $(v_2, v_4)$.

#### Formal Definition:

Given a BF-directed hypergraph $H = (V, E)$, its flow graph projection is a directed graph $G = (V, E')$, where every hyperedge creates a directed Cartesian product from its tail vertices to its head vertices:

$$
E' = \big\{ (u, v) : (\exists e \in E)(u \in T(e) \land v \in H(e))\big\}
$$

<div align="center" markdown="1">

![BF-Directed Hypergraph - Flow Graph - Light](../img/doc/light/bfdir-flow-graph.svg#only-light){: width="700" }
![BF-Directed Hypergraph - Flow Graph - Dark](../img/doc/dark/bfdir-flow-graph.svg#only-dark){: width="700" }

</div>

### Example Usage

The [**hgl::projection**](../cpp-gl/group__HGL-Core.md#function-projection) utility handles this transformation automatically, ensuring no duplicate edges and properly handling self-loops (for undirected hypergraphs).

```cpp
#include <hgl/conversion.hpp>

auto flow_graph = hgl::projection<gl::directed_graph<>>(hg); // (1)!
```

1. We assume `hg` is an instance of a *BF-directed* hypergraph.
