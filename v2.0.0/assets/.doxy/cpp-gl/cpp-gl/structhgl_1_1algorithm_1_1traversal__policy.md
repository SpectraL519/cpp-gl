

# Struct hgl::algorithm::traversal\_policy

**template &lt;traits::c\_hypergraph H, [**traversal\_direction**](group__HGL-Algorithm.md#enum-traversal_direction) Dir&gt;**



[**ClassList**](annotated.md) **>** [**algorithm**](namespacehgl_1_1algorithm.md) **>** [**traversal\_policy**](structhgl_1_1algorithm_1_1traversal__policy.md)



_Policy defining how to extract incident hyperedges and target vertices during traversal._ [More...](#detailed-description)


































































## Detailed Description


This template is specialized based on the hypergraph's directionality to route standard traversal algorithms over the correct incidence structures (e.g., following tails to heads).




**Template parameters:**


* `H` The type of the hypergraph. 
* `Dir` The direction of traversal. 




    

------------------------------
The documentation for this class was generated from the following file `include/hgl/algorithm/core.hpp`

