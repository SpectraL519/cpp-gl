# Hypergraph Properties

Beyond hypergraph traversals, the HGL algorithms module provides a suite of lightweight, functional utilities to evaluate the global structural properties of a hypergraph. These algorithms allow you to quickly determine bounds, uniformity, and regularity across the topology.

These algorithms are grouped into four primary categories:

- [Vertex Degree Bounds](#vertex-degree-bounds): Functions for evaluating the minimum and maximum connectivity of vertices.
- [Hyperedge Size Bounds](#hyperedge-size-bounds-rank--corank): Functions for evaluating the scale of hyperedges (Rank and Corank).
- [Regularity](#regularity): Functions to check if all vertices share the same degree.
- [Uniformity](#uniformity): Functions to check if all hyperedges contain the same number of vertices.

> [!NOTE] Algorithmic Complexity
>
> All property algorithms in this module operate by requesting a `degree_map()` or `hyperedge_size_map()` from the underlying hypergraph structure, and then performing a linear $\mathcal{O}(N)$ reduction (like `std::ranges::max_element` or a constant-check loop). Thus, the actual performance of these algorithms is entirely dictated by the complexity of generating the size/degree map, which varies depending on your chosen [Representation Layout](../architecture.md#representation-layouts).

---

## Vertex Degree Bounds

These functions evaluate the minimum and maximum degrees present within the hypergraph.

### Formal Definitions

For an undirected hypergraph $H = (V, E)$:

- **Maximum Degree** $\Delta(H)$: The largest degree of any vertex in the hypergraph. $\Delta(H) = \max\limits_{v \in V} deg(v)$
- **Minimum Degree** $\delta(H)$: The smallest degree of any vertex in the hypergraph. $\delta(H) = \min\limits_{v \in V} deg(v)$

For BF-directed hypergraphs, these concepts are further split into **in-degree** (the number of incoming/head hyperedges) and **out-degree** (the number of outgoing/tail hyperedges).

### Available Functions

**Undirected & Directed (Total Degree):**

- [**max_degree(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-max_degree): Returns the maximum degree $\Delta(H)$ among all vertices.
- [**min_degree(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-min_degree): Returns the minimum degree $\delta(H)$ among all vertices.

**BF-Directed Specifically:**

- [**max_out_degree(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-max_out_degree): Returns the maximum out-degree among all vertices.
- [**min_out_degree(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-min_out_degree): Returns the minimum out-degree among all vertices.
- [**max_in_degree(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-max_in_degree): Returns the maximum in-degree among all vertices.
- [**min_in_degree(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-min_in_degree): Returns the minimum in-degree among all vertices.

**Note:** If the hypergraph is empty, these functions safely return `0`.

---

## Hyperedge Size Bounds (Rank & Corank)

Unlike standard graphs where all edges have a size of exactly 2, hyperedges can encompass any number of vertices. The bounds of these sizes define the hypergraph's Rank and Corank.

### Formal Definitions

For an undirected hypergraph $H = (V, E)$:

- **Rank** $r(H)$: The maximum size (cardinality) of any hyperedge in the hypergraph. $r(H) = \max\limits_{e \in E} |e|$
- **Corank** $cr(H)$: The minimum size (cardinality) of any hyperedge in the hypergraph. $cr(H) = \min\limits_{e \in E} |e|$

### Available Functions

**Undirected & Directed (Total Size):**

- [**rank(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-rank): Calculates the maximum size of any hyperedge.
- [**corank(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-corank): Calculates the minimum size of any hyperedge.

**BF-Directed Specifically:**

- [**max_tail_size(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-max_tail_size): Calculates the maximum number of tail (source) vertices among all hyperedges.
- [**min_tail_size(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-min_tail_size): Calculates the minimum number of tail (source) vertices among all hyperedges.
- [**max_head_size(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-max_head_size): Calculates the maximum number of head (destination) vertices among all hyperedges.
- [**min_head_size(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-min_head_size): Calculates the minimum number of head (destination) vertices among all hyperedges.

---

## Regularity

A hypergraph is evaluated based on the consistency of its vertices' connectivity.

### Formal Definitions

- **$k$-Regular:** A hypergraph $H$ is $k$-regular if every vertex $v \in V$ is incident to exactly $k$ hyperedges: $(\forall v \in V)(deg(v) = k)$
- **Regular:** A hypergraph is structurally regular if there exists some integer $k$ such that the hypergraph is $k$-regular (i.e., all vertices have the exact same degree): $(\exists k \in \mathbb{N})(\forall v \in V)(deg(v) = k)$

### Available Functions

**Undirected & Directed (Total Degree):**

- [**is_regular(hypergraph, k)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_regular): Evaluates if all vertices have a degree of exactly `k`.
- [**is_regular(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_regular): Evaluates if all vertices share the same degree, regardless of what that specific degree is.

**BF-Directed Specifically:**

- [**is_out_regular(hypergraph, k)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_out_regular) / [**is_out_regular(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_out_regular): Evaluates if the hypergraph is out-$k$-regular or structurally out-regular (based strictly on out-degrees).
- [**is_in_regular(hypergraph, k)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_in_regular) / [**is_in_regular(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_in_regular): Evaluates if the hypergraph is in-$k$-regular or structurally in-regular (based strictly on in-degrees).

---

## Uniformity

A hypergraph is evaluated based on the consistency of its hyperedges' sizes.

### Formal Definitions

- **$k$-Uniform:** A hypergraph $H$ is $k$-uniform if every hyperedge $e \in E$ contains exactly $k$ vertices: $(\forall e \in E)(|e| = k)$
  *Note: A 2-uniform undirected hypergraph is equivalent to a standard undirected graph.*
- **Uniform:** A hypergraph is structurally uniform if there exists some integer $k$ such that the hypergraph is $k$-uniform (i.e., all hyperedges have the exact same size): $(\exists k \in \mathbb{N})(\forall e \in E)(|e| = k)$

### Available Functions

**Undirected & Directed (Total Size):**

- [**is_uniform(hypergraph, k)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_uniform): Evaluates if all hyperedges contain exactly `k` vertices.
- [**is_uniform(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_uniform): Evaluates if all hyperedges are the exact same size.

**BF-Directed Specifically:**

- [**is_tail_uniform(hypergraph, k)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_tail_uniform) / [**is_tail_uniform(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_tail_uniform): Evaluates if all hyperedges have a tail size of exactly `k`, or share the same tail size.
- [**is_head_uniform(hypergraph, k)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_head_uniform) / [**is_head_uniform(hypergraph)**](../../cpp-gl/group__HGL-Algorithm.md#function-is_head_uniform): Evaluates if all hyperedges have a head size of exactly `k`, or share the same head size.
