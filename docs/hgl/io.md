# I/O Utility

The HGL module provides a robust, customizable, and type-safe I/O system for formatting, serializing, and deserializing hypergraphs. Because the HGL module is built strictly on top of the GL module, it directly reuses its entire I/O infrastructure. Most of the stream manipulators and general range formatters defined in `gl::io` are imported directly into the `hgl::io` namespace.

> [!NOTE] Range Formatting
>
> This document focuses specifically on formatting hypergraph topologies. For a detailed guide on the general-purpose range and set formatters utilized under the hood (e.g., `range_formatter`, `implicit_range`), please refer to the [**GL I/O Utility**](../gl/io.md) page.

---

## Basic I/O Operations

At its core, the library overloads the standard `operator<<` for the [**hgl::hypergraph**](../cpp-gl/classhgl_1_1hypergraph.md) and element descriptors. By default, printing any of these objects yields a compact/concise mathematical representation.

> [!NOTE] Hyperedge Formatting
>
> Because a hyperedge descriptor is a simple wrapper for the hyperedge's ID and optional properties, printing it directly would result in the exact same format as for vertex descriptors. In order to print the hyperedge as the set(s) of its incident vertices, you need to use the dedicated [**fmt**](../cpp-gl/classhgl_1_1hypergraph.md#function-fmt) method of the `hypergraph` class.

```cpp
#include <hgl/hypergraph.hpp>
#include <iostream>

int main() {
    hgl::undirected_hypergraph<> hg(4);
    auto e1 = hg.add_hyperedge({0, 1, 2});

    auto v0 = hg.vertex(0);

    std::cout << "Vertex: " << v0 << '\n'; // (1)!
    std::cout << "Hyperedge: " << hg.fmt(e1) << '\n'; // (2)!
    std::cout << "Hypergraph:\n" << hg << '\n'; // (3)!
}
```

1. Prints the vertex ID.
2. Prints the hyperedge incidence utilizing the owning hypergraph's topological context.
3. Prints the incidence structure of the entire hypergraph in a compact format.

**Output:**

```text
Vertex: 0
Hyperedge: {0, 1, 2}
Hypergraph:
V = {0, ..., 3}
E = {
  {0, 1, 2}
}
```

---

## Stream Options Manipulators

To customize how hypergraphs and their elements are formatted, the library uses Stream Options Manipulators. These manipulators inject custom formatting states directly into standard I/O streams (like `std::cout`).

### Behavior Notes

> [!WARNING] State Persistence
>
> Stream options are **persistent**. Once you apply a manipulator to a stream, that formatting state remains active for all subsequent operations on that stream until it is explicitly changed or cleared using another manipulator (e.g., `hgl::io::default_options`).

> [!IMPORTANT] Shared State Between Modules
>
> The stream manipulator system is completely shared between the GL and HGL modules. Setting an option like `hgl::io::verbose` or `hgl::io::with_properties` on a stream will affect the formatting of *both* standard graphs and hypergraphs printed to that stream.
>
> **Note:** While HGL defines a `hgl::io::with_hyperedge_properties` option for semantic readability, it maps to the exact same underlying bitmask as GL's `gl::io::with_edge_properties` manipulator.

### Available Manipulators

| Manipulator | Description |
| :---------- | :---------- |
| **`hgl::io::concise`** | (Default) Sets the layout to a compact, mathematical set notation format. |
| **`hgl::io::verbose`** | Sets the layout to a detailed, multi-line format showing full structural hierarchy. |
| **`hgl::io::spec_fmt`** | Enables the Hypergraph Specification Format (HGSF) used for raw serialization/deserialization. |
| **`hgl::io::with_vertex_properties`** | Instructs the stream to print/read custom payload properties attached to vertices. |
| **`hgl::io::without_vertex_properties`** | Disables vertex property formatting. |
| **`hgl::io::with_hyperedge_properties`** | Instructs the stream to print/read custom payload properties attached to hyperedges. |
| **`hgl::io::without_hyperedge_properties`** | Disables hyperedge property formatting. |
| **`hgl::io::with_properties`** | Enables both vertex and hyperedge properties simultaneously. |
| **`hgl::io::without_properties`** | Disables both vertex and hyperedge properties simultaneously. |
| **`hgl::io::default_options`** | Resets the stream to its default state (concise, no properties). |

---

## Formatting Examples

The exact output of a hypergraph heavily depends on its directionality model and the currently active stream manipulators.

```cpp
#include <hgl/hypergraph.hpp>
#include <hgl/io/core.hpp>
#include <iostream>
#include <format>

int main() {
    using traits_t = hgl::undirected_hypergraph<hgl::name_property, hgl::name_property>;

    hgl::hypergraph<traits_t> hg(4); // (1)!
    hg.add_hyperedge({0, 1, 2});
    hg.add_hyperedge({1, 2, 3});
    hg.add_hyperedge({0, 3});

    for (const auto& vertex : hg.vertices()) // (2)!
        vertex->name = std::format("vertex_{}", vertex.id() + 1);
    for (const auto& hyperedge : hg.hyperedges()) // (3)!
        hyperedge->name = std::format("hyperedge_{}", hyperedge.id() + 1);

    std::cout << hg << std::endl; // (4)!
    std::cout << hgl::io::with_properties << hg << std::endl; // (5)!
    std::cout << hgl::io::verbose << hg << std::endl; // (6)!
}
```

1. Initialize the hypergraph.
2. Set the names of the hypergraph's vertices.
3. Set the names of the hypergraph's hyperedges.
4. Prints the hypergraph using the default options (concise format, without element properties).
5. Prints the hypergraph with both vertex and hyperedge properties, but still in a concise format (options persistency).
6. Prints the hypergraph in a verbose format, with element properties (options persistency).

**Output (Concise, Without Properties):**
```text
V = {0, ..., 3}
E = {
  {0, 1, 2},
  {1, 2, 3},
  {0, 3}
}
```

**Output (Concise, With Properties):**
```text
V = {
  0["vertex_1"],
  1["vertex_2"],
  2["vertex_3"],
  3["vertex_4"]
}
E = {
  {0, 1, 2}["hyperedge_1"],
  {1, 2, 3}["hyperedge_2"],
  {0, 3}["hyperedge_3"]
}
```

**Output (Verbose, With Properties):**
```text
type: undirected, |V| = 4, |E| = 3
vertices:
  - [id: 0 | "vertex_1"]
  - [id: 1 | "vertex_2"]
  - [id: 2 | "vertex_3"]
  - [id: 3 | "vertex_4"]
hyperedges:
  - [id: 0 | vertices: {0, 1, 2} | "hyperedge_1"]
  - [id: 1 | vertices: {1, 2, 3} | "hyperedge_2"]
  - [id: 2 | vertices: {0, 3} | "hyperedge_3"]
```

> [!NOTE] Directionality Differences
>
> If the same formatting principles are applied to a **BF-directed** hypergraph, the output visually splits the connections into *tail* and *head* components. For example, a BF-directed concise hyperedge without properties formats as `({0, 1} -> {2})`, and a verbose one formats as `[id: 0 | tail: {0, 1}, head: {2}]`.

---

## Hypergraph Specification Format (HGSF)

For storage purposes, HGL utilizes the **Hypergraph Specification Format (HGSF)**. This is a lightweight, flat text format triggered by the `hgl::io::spec_fmt` manipulator.

### Format Structure

The HGSF structure mirrors the standard GL GSF format but adapts the edge-list block to accommodate generalized hyperedge mappings:

```text
<dir-spec> <n-vertices> <n-hyperedges> <vprop-marker> <eprop-marker>
<vertex-prop-list>
<hyperedge-list>
```

| **Element** | **Description** |
| :---------- | :-------------- |
| `dir-spec` | `0` if the hypergraph is undirected<br/>`1` if the hypergraph is BF-directed |
| `n-vertices` | The total number of vertices in the hypergraph |
| `n-hyperedges`| The total number of hyperedges in the hypergraph |
| `vprop-marker`| `1` if vertex properties are present in the specification, `0` otherwise |
| `eprop-marker`| `1` if hyperedge properties are present in the specification, `0` otherwise |
| <span style="white-space: nowrap;">`vertex-prop-list`</span> | A sequential list of length `n-vertices` where each element represents the properties of a vertex. *Omitted if `vprop-marker == 0`.* |
| `hyperedge-list` | A list of length `n-hyperedges` where each line defines a single hyperedge. The structure of these lines depends on the directionality tag (see below). |

### Hyperedge Definitions

The specification structure of each hyperedge in the `<hyperedge-list>` is encoded based on the hypergraph's directionality:

**Undirected Hypergraphs:**

```text
<e-size> <e-vertices> <e-prop>
```

*(e.g., `3 0 1 2 "hyperedge_1"` represents an undirected hyperedge connecting vertices 0, 1, and 2, with an attached string property).*

**BF-Directed Hypergraphs:**

```text
<t-size> <h-size> <t-vertices> <h-vertices> <e-prop>
```

*(e.g., `2 1 0 1 2 "hyperedge_1"` represents a BF-directed hyperedge with a tail size of 2 (vertices 0, 1) and a head size of 1 (vertex 2), with an attached string property).*

### Example HGSF Output

Outputting the fully-propertied undirected hypergraph from the previous examples using `hgl::io::spec_fmt`:

```cpp
std::cout << hgl::io::spec_fmt << hgl::io::with_properties << hg << std::endl;
```

**Output:**
```text
0 4 3 1 1
"vertex_0"
"vertex_1"
"vertex_2"
"vertex_3"
3 0 1 2 "hyperedge_0"
3 1 2 3 "hyperedge_1"
2 0 3 "hyperedge_2"
```
