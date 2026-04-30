

# Namespace hgl



[**Namespace List**](namespaces.md) **>** [**hgl**](namespacehgl.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**algorithm**](namespacehgl_1_1algorithm.md) <br> |
| namespace | [**io**](namespacehgl_1_1io.md) <br> |
| namespace | [**repr**](namespacehgl_1_1repr.md) <br> |
| namespace | [**traits**](namespacehgl_1_1traits.md) <br>_Traits and concepts for the HGL module._  |
| namespace | [**util**](namespacehgl_1_1util.md) <br>_General utilities, ranges, and helpers for the HGL module (originating in the GL module)._  |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**bf\_directed\_t**](structhgl_1_1bf__directed__t.md) <br>_Tag type specifying that a hypergraph is backward-forward (BF) directed._  |
| class | [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) &lt;Properties, IdType&gt;<br>_A lightweight wrapper representing a hypergraph edge with optional properties._  |
| struct | [**hyperedge\_t**](structhgl_1_1hyperedge__t.md) <br>_Tag type representing a hyperedge element in a hypergraph._  |
| class | [**hypergraph**](classhgl_1_1hypergraph.md) &lt;HypergraphTraits&gt;<br>_The generic hypergraph container using a policy-based design._  |
| struct | [**hypergraph\_traits**](structhgl_1_1hypergraph__traits.md) &lt;DirectionalTag, VertexProperties, HyperedgeProperties, ReprTag&gt;<br>_Primary hypergraph traits structure that encapsulates all necessary type information for hypergraph representations._  |
| struct | [**undirected\_t**](structhgl_1_1undirected__t.md) <br>_Tag type specifying that a hypergraph is undirected._  |
| struct | [**vertex\_t**](structhgl_1_1vertex__t.md) <br>_Tag type representing a vertex element in a hypergraph._  |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**G**](group__HGL-Core.md#variable-invalid_id_v) | [**incidence\_graph**](#function-incidence_graph) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_undirected\_hypergraph [**auto**](group__HGL-Core.md#variable-invalid_id_v) & h) <br> |
|  [**G**](group__HGL-Core.md#variable-invalid_id_v) | [**incidence\_graph**](#function-incidence_graph) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_bf\_directed\_hypergraph [**auto**](group__HGL-Core.md#variable-invalid_id_v) & h) <br> |
|  [**G**](group__HGL-Core.md#variable-invalid_id_v) | [**projection**](#function-projection) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_undirected\_hypergraph [**auto**](group__HGL-Core.md#variable-invalid_id_v) & h) <br> |
|  [**G**](group__HGL-Core.md#variable-invalid_id_v) | [**projection**](#function-projection) ([**const**](group__HGL-Core.md#variable-invalid_id_v) traits::c\_bf\_directed\_hypergraph [**auto**](group__HGL-Core.md#variable-invalid_id_v) & h) <br> |




























## Public Functions Documentation




### function incidence\_graph 

```cpp
template<gl::traits::c_undirected_graph G>
G hgl::incidence_graph (
    const traits::c_undirected_hypergraph auto & h
) 
```




<hr>



### function incidence\_graph 

```cpp
template<gl::traits::c_directed_graph G>
G hgl::incidence_graph (
    const traits::c_bf_directed_hypergraph auto & h
) 
```




<hr>



### function projection 

```cpp
template<gl::traits::c_undirected_graph G>
G hgl::projection (
    const traits::c_undirected_hypergraph auto & h
) 
```




<hr>



### function projection 

```cpp
template<gl::traits::c_directed_graph G>
G hgl::projection (
    const traits::c_bf_directed_hypergraph auto & h
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

