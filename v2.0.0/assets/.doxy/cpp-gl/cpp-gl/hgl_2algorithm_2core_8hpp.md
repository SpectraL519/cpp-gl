

# File core.hpp



[**FileList**](files.md) **>** [**algorithm**](dir_c5eea9bccd937d2a7d22b2a641da6258.md) **>** [**core.hpp**](hgl_2algorithm_2core_8hpp.md)



_Core data structures and types used to control and track hypergraph algorithm execution._ 

* `#include "gl/attributes/force_inline.hpp"`
* `#include "gl/constants.hpp"`
* `#include "gl/graph.hpp"`
* `#include "gl/types/core.hpp"`
* `#include <functional>`
* `#include "gl/algorithm/core.hpp"`
* `#include "gl/algorithm/traits.hpp"`
* `#include "gl/algorithm/util.hpp"`
* `#include "gl/traits.hpp"`
* `#include "hgl/hypergraph.hpp"`
* `#include "hgl/traits.hpp"`
* `#include "hgl/types.hpp"`
* `#include <ranges>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**gl**](namespacegl.md) <br> |
| namespace | [**algorithm**](namespacegl_1_1algorithm.md) <br> |
| namespace | [**hgl**](namespacehgl.md) <br> |
| namespace | [**algorithm**](namespacehgl_1_1algorithm.md) <br> |
| namespace | [**traits**](namespacehgl_1_1traits.md) <br>_Traits and concepts for the HGL module._  |


## Classes

| Type | Name |
| ---: | :--- |
| struct | [**decision**](structgl_1_1algorithm_1_1decision.md) <br>_Represents a generic tri-state decision for control flow._  |
| struct | [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md) <br>_A tag type used to explicitly indicate the absence of a callback function._  |
| struct | [**no\_root\_t**](structgl_1_1algorithm_1_1no__root__t.md) <br>_Tag type providing an implicit conversion to the appropriate_ `no_root_v` _for any numeric ID type._ |
| struct | [**search\_node**](structgl_1_1algorithm_1_1search__node.md) &lt;GraphType&gt;<br>_Represents an active node in a search container (e.g., a BFS queue or DFS stack)._  |
| struct | [**search\_node**](structhgl_1_1algorithm_1_1search__node.md) &lt;H&gt;<br>_Represents an active node in a search container (e.g., a BFS queue or DFS stack) for hypergraph traversals._  |
| struct | [**traversal\_policy&lt; H, Dir &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01Dir_01_4.md) &lt;H, Dir&gt;<br>_Traversal policy specialization for undirected hypergraphs._  |
| struct | [**traversal\_policy&lt; H, traversal\_direction::backward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1backward_01_4.md) &lt;H&gt;<br>_Traversal policy specialization for backward searches on BF-directed hypergraphs._  |
| struct | [**traversal\_policy&lt; H, traversal\_direction::forward &gt;**](structhgl_1_1algorithm_1_1traversal__policy_3_01H_00_01traversal__direction_1_1forward_01_4.md) &lt;H&gt;<br>_Traversal policy specialization for forward searches on BF-directed hypergraphs._  |



















































------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

