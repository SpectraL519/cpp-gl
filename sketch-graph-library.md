# Sketch Graph Library

## Introduction
A C++ engine implementing a compact graph representation for analyzing massive data streams. The architecture eliminates traditional adjacency lists or matrices in favor of compressed node sketches with memory complexity independent of the data size. It is based on the EdgeSketch structure described in the paper **"EdgeSketch: Efficient Analysis of Massive Graph Streams"**. This enables executing graph algorithms directly on the stored summary, without accessing the original graph, which significantly reduces the time complexity of some of them.

## Goals
*   Efficient, multiple-header C++ library for graph sketching.
*   Ability to generate a single-header file for easy integration into other projects.
*   Support for numerous graph algorithms operating directly on sketches.
*   Distributed version for large-scale graph processing.
*  Python wrapper for easy integration with data science workflows.

## EdgeSketch
*   Constructed in a fully streaming manner during a single pass over the edge stream. Instead of maintaining the full connection structure, for each node $i$, the system creates and updates a fixed-size sketch $M_i = (F_i, S_i)$, consisting of two complementary arrays of length $m$.
*   $S_i$ (Aggregation): Built upon the **FastExpSketch** algorithm, it stores aggregated information about the weights of all incident edges using the minima of exponentially distributed variables. It is used for estimating node degrees, evaluating graph density, and enabling fast set-theoretic operations (e.g., calculating unions or intersections of node sets) in $O(m)$ time.
*   $F_i$ (Sampling): Stores an explicit, weighted sample of edges connected to a given node. This component is essential for reconstructing local paths, estimating the proportion of internal edges in subgraphs, and simulating random walks.

## Graph Types
*   **Directed graphs:** Edge $(u, v)$ is hashed as $u || v$ and updates $M_u$.
*   **Undirected graphs:** Edge $(u, v)$ is hashed as $\min(u,v) || \max(u,v)$ and updates both $M_u$ and $M_v$.
*   **Weighted graphs:** Edge $(u, v)$ with weight $w$ is natively handled by the EdgeSketch algorithm.
*   **Hypergraphs:** Edge $(u_1, u_2, \dots, u_k)$ is hashed as $u_1 || u_2 || \dots || u_k$ and updates all $M_{u_i}$.
*   **Multigraphs:** Edge $(u, v)$ is hashed as $u || v || \text{counter}$.
*   **Labeled graphs:** Edge $(u, v)$ with label $l$ is hashed as $u || v || l$.

## Algorithms
*   Edge count estimation
*   Node degree estimation
*   Graph density estimation
*   Subgraph inclusion check
*   Motif counting
*   DFS, BFS (approximated)
*   Random walks
*   Modularity and community detection (e.g., Louvain method)
*   Greedy peeling
*   Min-cut (Karger's algorithm)
*   Minimum spanning tree (MST)
*   Edge prediction / Graph reconstruction
*   Node similarity
*   ...

## Distributed System
*   gRPC + HTTP/2 + Docker
*   Lightweight custom message broker
*   Ingestors (workers) and aggregators (master nodes)
*   Memory-mapped files (`mmap`) instead of standard I/O, optimized with SIMD instructions

## Python wrapper

## Technical Details

### Memory Layout

```text
BUFFER 1: S values registry
Type: float/double
Alignment: 64 bytes

Base offset (0x0000...)
|
+--> [ Node 0 ]
|    [ S_0 | S_1 | S_2 | S_3 | S_4 | S_5 | S_6 | S_7 ] -> Perfect 1 Cache Line hit (64B)
|    [ S_8 | S_9 | ...                           | S_99]
|
+--> [ Node 1 ]
|    [ S_0 | S_1 | S_2 | S_3 | S_4 | S_5 | S_6 | S_7 ]
|    [ S_8 | S_9 | ...                           | S_99]
|
...
+--> [ Node N-1 ]
     [ S_0 ... S_99 ]

BUFFER 2: F samples registry
Type: struct { uint64_t u; uint64_t v; }
Alignment: 64 bytes

Base offset (0x8000...)
|
+--> [ Node 0 ]
|    [ (u,v)_0 | (u,v)_1 | (u,v)_2 | (u,v)_3 ] -> Perfect 1 Cache Line hit (64B)
|    [ (u,v)_4 | (u,v)_5 | ...     | (u,v)_99]
|
+--> [ Node 1 ]
|    [ (u,v)_0 | (u,v)_1 | (u,v)_2 | (u,v)_3 ]
|    [ (u,v)_4 | ...               | (u,v)_99]
|
...
```

### Implementation Example

```cpp
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>

// Edge definition (F part of the sketch)
struct Edge {
    uint64_t u;
    uint64_t v;
};

class SketchRegistry {
private:
    uint64_t m_size;         // Sketch size (e.g., 100)
    double* s_values;        // Flat buffer: N * m_size
    Edge* f_values;          // Flat buffer: N * m_size
    double* max_s_values;    // Flat buffer storing the current maximum in each sketch (FastExpSketch optimization)
    
    // Custom map: Global NodeID -> Internal Index
    // absl::flat_hash_map<uint64_t, uint64_t> id_mapping;

public:
    
    /**
     * @brief Processes a new incident edge and updates sketches.
     * 
     * For undirected graphs, we call this function twice (for u and v)
     * or modify it to internally update both pointers.
     */
    void ProcessEdge(uint64_t target_node, uint64_t edge_u, uint64_t edge_v, double weight = 1.0) {
        uint64_t idx = GetOrCreateInternalIndex(target_node);
        
        double* local_s = s_values + (idx * m_size);
        Edge*   local_f = f_values + (idx * m_size);
        double& current_max = max_s_values[idx];

        uint64_t min_node = std::min(edge_u, edge_v);
        uint64_t max_node = std::max(edge_u, edge_v);
        
        SeedRandom(min_node, max_node); 
        
        double sum = 0.0;
        bool update_max = false;

        for (uint64_t k = 1; k <= m_size; ++k) {
            double E = -log(RandomUniform()) / weight; 
            sum += E / (m_size - k + 1);
            
            // Early exit - FastExpSketch optimization
            if (sum >= current_max) {
                break; 
            }
            
            uint64_t l = GetRandomPermutationPosition();
            
            if (sum < local_s[l]) {
                if (local_s[l] == current_max) update_max = true;
                local_s[l] = sum;
                local_f[l] = {edge_u, edge_v};
            }
        }
        
        if (update_max) {
            current_max = RecalculateMax(local_s, m_size);
        }
    }

    /**
     * @brief Performs a union (merge) of the sketch.
     */
    void MergeSketch(uint64_t local_idx, const double* remote_s, const Edge* remote_f) {
        double* local_s = s_values + (local_idx * m_size);
        Edge*   local_f = f_values + (local_idx * m_size);
        
        bool max_changed = false;

        // Vectorized loop (e.g., AVX-512)
        for (uint64_t i = 0; i < m_size; ++i) {
            if (remote_s[i] < local_s[i]) {
                local_s[i] = remote_s[i];
                local_f[i] = remote_f[i];
                max_changed = true;
            }
        }

        if (max_changed) {
            max_s_values[local_idx] = RecalculateMax(local_s, m_size);
        }
    }

    /**
     * @brief Reads the estimated node degree with an O(1/m) error guarantee.
     */
    double EstimateNodeDegree(uint64_t node_id) const {
        uint64_t idx = GetInternalIndex(node_id);
        const double* local_s = s_values + (idx * m_size);
        
        double sum_s = 0.0;
        for (uint64_t i = 0; i < m_size; ++i) {
            sum_s += local_s[i];
        }
        
        // Unbiased estimator
        return (m_size - 1) / sum_s; 
    }

    /**
     * @brief Returns a weighted edge sample (F array).
     */
    const Edge* GetEdgeSample(uint64_t node_id) const {
        uint64_t idx = GetInternalIndex(node_id);

        return f_values + (idx * m_size);
    }
};
```