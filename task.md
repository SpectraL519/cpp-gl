# CPP-GL Graph Element Representation and Storage Refactor

## Task

- [YouTrack Task](https://spectral519.youtrack.cloud/issue/CPPGL-56/Replace-the-usage-of-pointers-for-vertex-and-edge-storage-to-simple-composite-types)
- Description:
  ```
  - The `graph` class should conditionally contain vertex and edge property maps. Each entry in these maps should be bound to the element with the given id.
  - The `vertex_descriptor` and `edge_descriptor` classes should contain a reference to the proper property map entry instead of containing the properties directly and define a mutable `properties()` getter method
  - The `graph` class should initialize new vertex/edge elements without the use of pointers and delegate the proper element handling/conversion to the implementation types
  - The vertex/edge getter methods of the `graph` class should return the elements by value
  - The `adjacency_list` model should store the edges without the use of pointers
  - Align the algorithms to match the new structure of the graph elements and their storage
  ```

## Ideas

1. Lightweight descriptor object
- Instead of storing the vertex set, store only the number of vertices and a properties map
- Create the descriptor object only when necessary
- After removing a vertex align the ids of descriptors stored by graph edges:
  - Requires a warning/caution doc stating that removing a vertex invalidates the ids of returned descriptors
