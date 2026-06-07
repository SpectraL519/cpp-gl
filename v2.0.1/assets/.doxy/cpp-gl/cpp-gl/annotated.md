
# Class List


Here are the classes, structs, unions and interfaces with brief descriptions:

* **namespace** [**gl**](namespacegl.md)     
    * **namespace** [**algorithm**](namespacegl_1_1algorithm.md)     
        * **struct** [**decision**](structgl_1_1algorithm_1_1decision.md) _Represents a generic tri-state decision for control flow._     
        * **struct** [**dijkstra\_search\_node**](structgl_1_1algorithm_1_1dijkstra__search__node.md) _Internal node structure for Dijkstra's algorithm to snapshot distances and preserve heap invariants._     
        * **struct** [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md) _A tag type used to explicitly indicate the absence of a callback function._ 
        * **struct** [**mst\_descriptor**](structgl_1_1algorithm_1_1mst__descriptor.md) _A descriptor structure holding the results of a Minimum Spanning Tree (MST) execution._     
        * **struct** [**no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md) _Tag type providing an implicit conversion to the appropriate_ `no_root_v` _for any numeric ID type._    
        * **struct** [**paths\_descriptor**](structgl_1_1algorithm_1_1paths__descriptor.md) _A descriptor structure holding the results of a single-source shortest path execution._     
        * **struct** [**search\_node**](structgl_1_1algorithm_1_1search__node.md) _Represents an active node in a search container (e.g., a BFS queue or DFS stack)._     
    * **class** [**binary\_color**](classgl_1_1binary__color.md) _A specialized color property for algorithms requiring binary states (e.g., bipartition)._     
    * **struct** [**binary\_color\_property**](structgl_1_1binary__color__property.md) _A property struct wrapping a_ `binary_color` _._    
    * **struct** [**directed\_t**](structgl_1_1directed__t.md) _The tag type representing a directed graph configuration._     
    * **class** [**dynamic\_properties**](classgl_1_1dynamic__properties.md) _A type-safe container for heterogeneous properties stored by string keys._     
    * **class** [**edge\_descriptor**](classgl_1_1edge__descriptor.md) _A lightweight wrapper representing a graph edge with its endpoints and optional properties._     
    * **struct** [**empty\_properties**](structgl_1_1empty__properties.md) _A stateless, empty structural tag representing an absence of properties._ 
    * **struct** [**empty\_properties\_map**](structgl_1_1empty__properties__map.md) _A stateless, empty structural tag indicating that the absence of a property map container._ 
    * **class** [**flat\_jagged\_vector**](classgl_1_1flat__jagged__vector.md) _A flattened 2D vector (jagged array) providing efficient storage for variable-length segments._     
        * **class** [**segment\_iterator**](classgl_1_1flat__jagged__vector_1_1segment__iterator.md) _Random access iterator over segments of the_ `flat_jagged_vector` _._    
    * **class** [**flat\_matrix**](classgl_1_1flat__matrix.md) _A flattened 2D matrix providing efficient storage and uniform access for a rectangular grid of elements._     
        * **class** [**row\_iterator**](classgl_1_1flat__matrix_1_1row__iterator.md) _Random access iterator over the rows of the_ `flat_matrix` _._    
    * **class** [**graph**](classgl_1_1graph.md) _The generic graph container using a policy-based architecture._     
    * **struct** [**graph\_traits**](structgl_1_1graph__traits.md) _Primary graph traits structure that encapsulates all necessary type information for graph representations._     
    * **struct** [**initial\_id\_t**](structgl_1_1initial__id__t.md) _A helper type that can be implicitly converted to the initial ID value of 0 for any valid ID type._     
    * **struct** [**invalid\_id\_t**](structgl_1_1invalid__id__t.md) _A helper type that can be implicitly converted to the invalid ID value for any valid ID type._     
    * **namespace** [**io**](namespacegl_1_1io.md)     
        * **struct** [**append**](structgl_1_1io_1_1append.md) _Tag type specifying that a file should be opened in append mode._ 
        * **struct** [**implicit\_range\_formatter**](structgl_1_1io_1_1implicit__range__formatter.md) _A proxy object for concisely formatting large contiguous numeric ranges._     
        * **class** [**options\_manip**](classgl_1_1io_1_1options__manip.md) _A custom stream manipulator for modifying formatting options on standard I/O streams._     
        * **struct** [**range\_formatter**](structgl_1_1io_1_1range__formatter.md) _A customizable stream proxy for formatting and printing C++ ranges._     
        * **struct** [**write**](structgl_1_1io_1_1write.md) _Tag type specifying that a file should be opened in strictly write (truncate) mode._ 
    * **struct** [**name\_property**](structgl_1_1name__property.md) _A property struct providing a basic string-based naming facility._     
    * **namespace** [**repr**](namespacegl_1_1repr.md)     
        * **struct** [**flat\_list\_t**](structgl_1_1repr_1_1flat__list__t.md) _Tag struct for the flattened adjacency list graph representation._     
        * **struct** [**flat\_matrix\_t**](structgl_1_1repr_1_1flat__matrix__t.md) _Tag struct for the flattened adjacency matrix graph representation._     
        * **struct** [**list\_t**](structgl_1_1repr_1_1list__t.md) _Tag struct for the standard adjacency list graph representation._     
        * **struct** [**matrix\_t**](structgl_1_1repr_1_1matrix__t.md) _Tag struct for the standard adjacency matrix graph representation._     
    * **namespace** [**topology**](namespacegl_1_1topology.md) 
    * **namespace** [**traits**](namespacegl_1_1traits.md)     
        * **struct** [**is\_instantiation\_of**](structgl_1_1traits_1_1is__instantiation__of.md) _Type trait to check if a type is an instantiation of a specific class template._ 
        * **struct** [**is\_instantiation\_of&lt; U&lt; Args... &gt;, U &gt;**](structgl_1_1traits_1_1is__instantiation__of_3_01U_3_01Args_8_8_8_01_4_00_01U_01_4.md) _Specialization of the_ [_**is\_instantiation\_of**_](structgl_1_1traits_1_1is__instantiation__of.md) _trait for when the type is an instantiation of the provided class template._
        * **struct** [**swap\_repr\_tag**](structgl_1_1traits_1_1swap__repr__tag.md) _Utility trait type used to swap the representation tag of a graph traits or graph type._ 
        * **struct** [**swap\_repr\_tag&lt; graph&lt; graph\_traits&lt; Dir, VP, EP, OldImplTag, IdType &gt; &gt;, NewImplTag &gt;**](structgl_1_1traits_1_1swap__repr__tag_3_01graph_3_01graph__traits_3_01Dir_00_01VP_00_01EP_00_01O2a1c51ed02510394fe4450b6c0754649.md) _Specialization of_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**graph**_](classgl_1_1graph.md) _class._    
        * **struct** [**swap\_repr\_tag&lt; graph\_traits&lt; Dir, VP, EP, OldImplTag, IdType &gt;, NewImplTag &gt;**](structgl_1_1traits_1_1swap__repr__tag_3_01graph__traits_3_01Dir_00_01VP_00_01EP_00_01OldImplTag_9a28fe1b0e4026b5274857dfd47a2822.md) _Specialization of_ [_**swap\_repr\_tag**_](structgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**graph\_traits**_](structgl_1_1graph__traits.md) _type._    
    * **struct** [**undirected\_t**](structgl_1_1undirected__t.md) _The tag type representing an undirected graph configuration._     
    * **namespace** [**util**](namespacegl_1_1util.md)     
        * **struct** [**concat\_fn**](structgl_1_1util_1_1concat__fn.md) _A function object for concatenating two viewable ranges into a_ `concat_view` _._    
        * **class** [**concat\_view**](classgl_1_1util_1_1concat__view.md) _A view concatenating two ranges sequentially (C++20 polyfill for C++26_ `std::views::concat` _)._    
    * **class** [**vertex\_descriptor**](classgl_1_1vertex__descriptor.md) _A lightweight wrapper around a vertex identifier with optional properties._     
    * **struct** [**vertex\_distance**](structgl_1_1vertex__distance.md) _Utility trait to resolve the underlying distance or weight numeric type for a graph._     
    * **struct** [**vertex\_distance&lt; GraphType &gt;**](structgl_1_1vertex__distance_3_01GraphType_01_4.md) _Specialization resolving the specific weight type when edge properties contain weight attributes._     
    * **struct** [**weight\_property**](structgl_1_1weight__property.md) _A property struct providing arithmetic weight for edges or vertices._     
* **struct** [**concise\_target\_formatter**](structgl_1_1graph_1_1concise__target__formatter.md)     
* **class** [**iterator**](classgl_1_1util_1_1concat__view_1_1iterator.md)     
* **class** [**sentinel**](classgl_1_1util_1_1concat__view_1_1sentinel.md)     
* **namespace** [**hgl**](namespacehgl.md)     
    * **namespace** [**algorithm**](namespacehgl_1_1algorithm.md)     
        * **struct** [**search\_node**](structhgl_1_1algorithm_1_1search__node.md) _Represents an active node in a search container (e.g., a BFS queue or DFS stack) for hypergraph traversals._     
        * **struct** [**traversal\_policy**](structhgl_1_1algorithm_1_1traversal__policy.md) _Policy defining how to extract incident hyperedges and target vertices during traversal._ 
        * **struct** [**traversal\_policy&lt; H, Dir &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01Dir_01_4.md) _Traversal policy specialization for undirected hypergraphs._     
        * **struct** [**traversal\_policy&lt; H, traversal\_direction::backward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1backward_01_4.md) _Traversal policy specialization for backward searches on BF-directed hypergraphs._     
        * **struct** [**traversal\_policy&lt; H, traversal\_direction::forward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1forward_01_4.md) _Traversal policy specialization for forward searches on BF-directed hypergraphs._     
    * **struct** [**bf\_directed\_t**](structhgl_1_1bf__directed__t.md) _Tag type specifying that a hypergraph is backward-forward (BF) directed._     
    * **class** [**hyperedge\_descriptor**](classhgl_1_1hyperedge__descriptor.md) _A lightweight wrapper representing a hypergraph edge with optional properties._     
    * **struct** [**hyperedge\_t**](structhgl_1_1hyperedge__t.md) _Tag type representing a hyperedge element in a hypergraph._ 
    * **class** [**hypergraph**](classhgl_1_1hypergraph.md) _The generic hypergraph container using a policy-based design._     
        * **struct** [**hyperedge\_formatter**](structhgl_1_1hypergraph_1_1hyperedge__formatter.md) _Helper structure used to properly format an individual hyperedge within the hypergraph's context into an output stream._     
    * **struct** [**hypergraph\_traits**](structhgl_1_1hypergraph__traits.md) _Primary hypergraph traits structure that encapsulates all necessary type information for hypergraph representations._     
    * **namespace** [**io**](namespacehgl_1_1io.md) 
    * **namespace** [**repr**](namespacehgl_1_1repr.md)     
        * **struct** [**bidirectional\_t**](structhgl_1_1repr_1_1bidirectional__t.md) _Layout tag representing a symmetric or fully bidirectional incidence structural layout._ 
        * **struct** [**flat\_list\_t**](structhgl_1_1repr_1_1flat__list__t.md) _Tag struct for the flattened incidence list hypergraph representation._     
        * **struct** [**flat\_matrix\_t**](structhgl_1_1repr_1_1flat__matrix__t.md) _Tag struct for the flattened incidence matrix hypergraph representation._     
        * **struct** [**hyperedge\_major\_t**](structhgl_1_1repr_1_1hyperedge__major__t.md) _Layout tag designating hyperedges as the primary structural dimension of the incidence representation._     
        * **struct** [**list\_t**](structhgl_1_1repr_1_1list__t.md) _Tag struct for the standard incidence list hypergraph representation._     
        * **struct** [**matrix\_t**](structhgl_1_1repr_1_1matrix__t.md) _Tag struct for the standard incidence matrix hypergraph representation._     
        * **struct** [**vertex\_major\_t**](structhgl_1_1repr_1_1vertex__major__t.md) _Layout tag designating vertices as the primary structural dimension of the incidence representation._     
    * **namespace** [**traits**](namespacehgl_1_1traits.md) _Traits and concepts for the HGL module._     
        * **struct** [**swap\_repr\_tag**](structhgl_1_1traits_1_1swap__repr__tag.md) _Utility trait type used to swap the representation tag of a hypergraph traits or hypergraph type._ 
        * **struct** [**swap\_repr\_tag&lt; hypergraph&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt; &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph_3_01hypergraph__traits_3_01Dir_00_01VP_00_5680b44d408a91533b00c1a8cd4feb1c.md) _Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph**_](classhgl_1_1hypergraph.md) _type._    
        * **struct** [**swap\_repr\_tag&lt; hypergraph\_traits&lt; Dir, VP, EP, OldReprTag &gt;, NewReprTag &gt;**](structhgl_1_1traits_1_1swap__repr__tag_3_01hypergraph__traits_3_01Dir_00_01VP_00_01EP_00_01OldReprTag_01_4_00_01NewReprTag_01_4.md) _Specialization of_ [_**swap\_repr\_tag**_](structhgl_1_1traits_1_1swap__repr__tag.md) _for the_[_**hypergraph\_traits**_](structhgl_1_1hypergraph__traits.md) _type._    
    * **struct** [**undirected\_t**](structhgl_1_1undirected__t.md) _Tag type specifying that a hypergraph is undirected._     
    * **namespace** [**util**](namespacehgl_1_1util.md) _General utilities, ranges, and helpers for the HGL module (originating in the GL module)._ 
    * **struct** [**vertex\_t**](structhgl_1_1vertex__t.md) _Tag type representing a vertex element in a hypergraph._ 

