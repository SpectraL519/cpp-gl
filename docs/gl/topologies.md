# Building Topologies (Generators)

The CPP-GL library includes a suite of highly optimized topology generators located in the `gl::topology` namespace. These generators are designed to instantly produce standard mathematical graph structures, which is exceptionally useful for benchmarking, unit testing, algorithmic validation, and building complex networks from foundational primitives.

> [!TIP]
>
> While each topology has its generator functions defined in a separate file int the `gl/topology` directory, you can use the provided `gl/topology.hpp` umbrella header which includes all available topology generators.

> [!NOTE] Flat List Performance Optimization
>
> Constructing a flat adjacency list (`flat_list_t`) element-by-element is highly inefficient due to contiguous memory shifts. To solve this, all generators in this module are heavily specialized.
>
> If you request a generator to output a graph that satisfies the `c_flat_list_graph` concept, the generator will automatically perform an optimized internal fast-path: it will first construct the topology using a standard `list_t` graph, and then instantly flatten it into contiguous memory using the `gl::to` conversion utility.

---

## Path Graphs

A path graph (or linear graph) consists of a simple linear sequence of vertices. Vertex $v_i$ is connected to vertex $v_{i+1}$, terminating at the final vertex.

For directed graphs, the [**path**](../cpp-gl/group__GL-Topology.md#function-path) generator creates a one-way chain from the first to the last vertex, whereas [**bidirectional_path**](../cpp-gl/group__GL-Topology.md#function-bidirectional_path) creates reciprocal edges allowing traversal in both directions.

For undirected graphs, both functions produce the same inherently bidirectional topology.

<div align="center" markdown="1">

![Path Graph Light](../img/doc/light/topology-path.svg#only-light){: width="750" }
![Path Graph Dark](../img/doc/dark/topology-path.svg#only-dark){: width="750" }

</div>

### Example Usage

```cpp
#include <gl/graph.hpp>
#include <gl/topology/path.hpp>

auto dir_path = gl::topology::path<gl::directed_graph<>>(5); // (1)!
auto undir_path
    = gl::topology::path<gl::flat_list_graph<gl::undirected_t>>(100); // (2)!
```

1. Generates a directed one-way path of 5 vertices: $v_0 \rightarrow v_1 \rightarrow v_2 \rightarrow v_3 \rightarrow v_4$
2. Generates an undirected path of 100 vertices represented using the flat-list model.

---

## Cycle Graphs (Rings)

A cycle graph consists of a single closed chain of vertices. Vertex $v_i$ is connected to vertex $v_{i+1}$, with the final vertex connecting back to the starting vertex $v_0$.

For directed graphs, the [**cycle**](../cpp-gl/group__GL-Topology.md#function-cycle) generator creates a unidirectional loop, whereas the [**bidirectional_cycle**](../cpp-gl/group__GL-Topology.md#function-bidirectional_cycle) generator creates a reciprocal edge for every forward edge in the chain, forming a bidirectional ring.

For undirected graphs, both generator functions produce the same inherently bidirectional topology.

<div align="center" markdown="1">

![Cycle Graph Light](../img/doc/light/topology-cycle.svg#only-light){: width="700" }
![Cycle Graph Dark](../img/doc/dark/topology-cycle.svg#only-dark){: width="700" }

</div>

### Example Usage

```cpp
#include <gl/graph.hpp>
#include <gl/topology/cycle.hpp>

auto loop = gl::topology::cycle<gl::directed_graph<>>(6); // (1)!
auto ring = gl::topology::cycle<gl::undirected_graph<>>(20); // (2)!
```

1. Generates a directed loop of 6 vertices.
2. Generates an undirected ring of 20 vertices.

---

## Complete Graphs (Cliques)

A complete graph ([**clique**](../cpp-gl/group__GL-Topology.md#function-clique)) is a simple graph in which every pair of distinct vertices is connected by an edge.

If the requested graph type is directed, this function generates fully bidirectional edges between every distinct pair of vertices.

<div align="center" markdown="1">

![Complete Graph Light](../img/doc/light/topology-clique.svg#only-light){: width="700" }
![Complete Graph Dark](../img/doc/dark/topology-clique.svg#only-dark){: width="700" }

</div>

### Example Usage

```cpp
#include <gl/graph.hpp>
#include <gl/topology/clique.hpp>

auto k_5 = gl::topology::clique<gl::undirected_graph<>>(5); // (1)!
```

1. Generates a $K_5$ graph (a complete graph with 5 vertices and 10 undirected edges)

---

## Complete Bipartite Graphs (Bicliques)

A complete bipartite graph ([**biclique**](../cpp-gl/group__GL-Topology.md#function-biclique)) is a graph whose vertices are partitioned into two disjoint sets, $A$ and $B$, such that every vertex in $A$ connects to every vertex in $B$, with no internal connections within the sets themselves.

If the requested graph type is directed, this function generates reciprocal edges (from $A$ to $B$ and from $B$ to $A$) to mimic full undirected connectivity.

<div align="center" markdown="1">

![Complete Bipartite Graph Light](../img/doc/light/topology-biclique.svg#only-light){: width="700" }
![Complete Bipartite Graph Dark](../img/doc/dark/topology-biclique.svg#only-dark){: width="700" }

</div>

### Example Usage

```cpp
#include <gl/graph.hpp>
#include <gl/topology/bipartite.hpp>

auto k_3_2 = gl::topology::biclique<gl::undirected_graph<>>(3, 2);
```

1. Generates a $K_{3,2}$ graph (with 3 vertices in set $A$ and 2 vertices in set $B$).

---

## Regular Binary Trees

A regular binary tree is a tree topology where all internal vertices have exactly two children, and all leaf vertices are at the exact same depth. For a given depth $d$, the graph will contain exactly $2^d - 1$ vertices.

For directed graphs, the [**regular_binary_tree**](../cpp-gl/group__GL-Topology.md#function-regular_binary_tree) generator creates edges pointing strictly from the parent vertex to its children, while the [**bidirectional_regular_binary_tree**](../cpp-gl/group__GL-Topology.md#function-bidirectional_regular_binary_tree) ensures that for every parent-to-child edge, a reciprocal child-to-parent edge is also created.

For undirected graphs, both generator functions produce the same inherently bidirectional topology.

<div align="center" markdown="1">

![Regular Binary Tree Graph Light](../img/doc/light/topology-bintree.svg#only-light){: width="700" }
![Regular Binary Tree Graph Dark](../img/doc/dark/topology-bintree.svg#only-dark){: width="700" }

</div>

### Example Usage

```cpp
#include <gl/graph.hpp>
#include <gl/topology/binary_tree.hpp>

auto tree = gl::topology::regular_binary_tree<gl::directed_graph<>>(4); // (1)!
```

1. Generates a directed binary tree with a depth of $4$ and $2^4 - 1 = 15$ total vertices. Edges *flow* strictly downwards from the root to the leaves.
