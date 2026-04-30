

# Group GL-Topology



[**Modules**](modules.md) **>** [**GL-Topology**](group__GL-Topology.md)



_Generators and evaluators for standard structural graph topologies._ [More...](#detailed-description)






































## Public Functions

| Type | Name |
| ---: | :--- |
|  GraphType | [**biclique**](#function-biclique) (size\_type n\_vertices\_a, size\_type n\_vertices\_b) <br>_Generates a complete bipartite graph (biclique)._  |
|  GraphType | [**bidirectional\_cycle**](#function-bidirectional_cycle) (size\_type n\_vertices) <br>_Generates a cycle graph with bidirectional edges._  |
|  GraphType | [**bidirectional\_path**](#function-bidirectional_path) (size\_type n\_vertices) <br>_Generates a path graph with bidirectional edges._  |
|  GraphType | [**bidirectional\_regular\_binary\_tree**](#function-bidirectional_regular_binary_tree) (size\_type depth) <br>_Generates a regular binary tree with bidirectional edges._  |
|  GraphType | [**clique**](#function-clique) (size\_type n\_vertices) <br>_Generates a complete graph (clique)._  |
|  GraphType | [**cycle**](#function-cycle) (size\_type n\_vertices) <br>_Generates a regular cycle graph (ring topology)._  |
|  GraphType | [**path**](#function-path) (size\_type n\_vertices) <br>_Generates a regular path graph (linear topology)._  |
|  GraphType | [**regular\_binary\_tree**](#function-regular_binary_tree) (size\_type depth) <br>_Generates a regular (regular) binary tree of a specified depth._  |




























## Detailed Description


A collection of lightweight utilities designed to quickly instantiate classic graph topologies. Featuring out-of-the-box support for cliques (complete graphs), bipartite graphs, binary trees, paths, and cycles, these generators are ideal for rapid prototyping, algorithm benchmarking, and automated testing. 


    
## Public Functions Documentation




### function biclique 

_Generates a complete bipartite graph (biclique)._ 
```cpp
template<traits::c_graph GraphType>
GraphType biclique (
    size_type n_vertices_a,
    size_type n_vertices_b
) 
```



A complete bipartite graph is a graph whose vertices are partitioned into two disjoint sets, \(A\) and \(B\), such that every vertex in \(A\) connects to every vertex in \(B\).


If the requested `GraphType` is directed, this function generates reciprocal edges (from \(A\) to \(B\) and from \(B\) to \(A\)) to mimic full undirected connectivity.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `n_vertices_a` The number of vertices in partition \(A\). 
* `n_vertices_b` The number of vertices in partition \(B\). 



**Returns:**

A newly constructed graph representing the biclique. 





        

<hr>



### function bidirectional\_cycle 

_Generates a cycle graph with bidirectional edges._ 
```cpp
template<traits::c_graph GraphType>
GraphType bidirectional_cycle (
    size_type n_vertices
) 
```



For directed graphs, this function creates a reciprocal edge for every forward edge in the closed chain, forming a bidirectional ring.


For undirected graphs, this function simply falls back to [**cycle**](group__GL-Topology.md#function-cycle), as undirected edges are inherently bidirectional.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `n_vertices` The total number of vertices in the cycle. 



**Returns:**

A newly constructed graph representing the bidirectional cycle. 





        

<hr>



### function bidirectional\_path 

_Generates a path graph with bidirectional edges._ 
```cpp
template<traits::c_graph GraphType>
GraphType bidirectional_path (
    size_type n_vertices
) 
```



For directed graphs, this function creates reciprocal edges along the linear sequence, allowing traversal in both directions.


For undirected graphs, this function simply falls back to [**path**](group__GL-Topology.md#function-path), as undirected edges are inherently bidirectional.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `n_vertices` The total number of vertices in the path. 



**Returns:**

A newly constructed graph representing the bidirectional path. 





        

<hr>



### function bidirectional\_regular\_binary\_tree 

_Generates a regular binary tree with bidirectional edges._ 
```cpp
template<traits::c_graph GraphType>
GraphType bidirectional_regular_binary_tree (
    size_type depth
) 
```



For directed graphs, this function ensures that for every parent-to-child edge, a reciprocal child-to-parent edge is also created.


For undirected graphs, this function simply falls back to [**regular\_binary\_tree**](group__GL-Topology.md#function-regular_binary_tree), as undirected edges are inherently bidirectional.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `depth` The depth of the binary tree. 



**Returns:**

A newly constructed graph representing the bidirectional binary tree. 





        

<hr>



### function clique 

_Generates a complete graph (clique)._ 
```cpp
template<traits::c_graph GraphType>
GraphType clique (
    size_type n_vertices
) 
```



A complete graph is a simple graph in which every pair of distinct vertices is connected by an edge.


If the requested `GraphType` is directed, this function generates fully bidirectional edges between every pair of vertices.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `n_vertices` The total number of vertices in the clique. 



**Returns:**

A newly constructed graph representing the clique. 





        

<hr>



### function cycle 

_Generates a regular cycle graph (ring topology)._ 
```cpp
template<traits::c_graph GraphType>
GraphType cycle (
    size_type n_vertices
) 
```



A cycle graph consists of a single closed chain of vertices. Vertex \(v_i\) is connected to vertex \(v_{i+1}\), with the final vertex connecting back to vertex \(v_0\).


For directed graphs, this creates a unidirectional loop.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `n_vertices` The total number of vertices in the cycle. 



**Returns:**

A newly constructed graph representing the cycle. 





        

<hr>



### function path 

_Generates a regular path graph (linear topology)._ 
```cpp
template<traits::c_graph GraphType>
GraphType path (
    size_type n_vertices
) 
```



A path graph consists of a simple linear sequence of vertices. Vertex \(v_i\) is connected to vertex \(v_{i+1}\), terminating at the final vertex.


For directed graphs, this creates a one-way chain from the first to the last vertex.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `n_vertices` The total number of vertices in the path. 



**Returns:**

A newly constructed graph representing the linear path. 





        

<hr>



### function regular\_binary\_tree 

_Generates a regular (regular) binary tree of a specified depth._ 
```cpp
template<traits::c_graph GraphType>
GraphType regular_binary_tree (
    size_type depth
) 
```



A regular binary tree is a tree where all internal vertices have exactly two children and all leaf vertices are at the same depth. For a given depth \(d\), the graph will contain exactly \(2^d - 1\) vertices.


If the requested `GraphType` is directed, edges are created pointing from the parent vertex to its children.



> [!NOTE] Performance for Flat List Graphs
>
>
> If the requested `GraphType` satisfies [**c\_flat\_list\_graph**](gl_concepts.md#gl-traits-c-flat-list-graph), an optimized overload is automatically selected. It internally constructs a standard adjacency list graph first, and then utilizes the [**to**](group__GL-Core.md#function-to) conversion to flatten it. This is significantly faster than inserting edges one-by-one into a flat representation. 
>
>



**Template parameters:**


* `GraphType` The target graph type to generate. 



**Parameters:**


* `depth` The depth (number of levels) of the binary tree. A depth of `1` yields a single root vertex. 



**Returns:**

A newly constructed graph representing the regular binary tree. 





        

<hr>

------------------------------


