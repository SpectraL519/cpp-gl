

# Struct gl::algorithm::empty\_callback



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**empty\_callback**](structgl_1_1algorithm_1_1empty__callback.md)



_A tag type used to explicitly indicate the absence of a callback function._ [More...](#detailed-description)

* `#include <hgl/algorithm/core.hpp>`

































































## Detailed Description



> [!NOTE] Performance vs. Empty Lambdas
>
>
> Passing `empty_callback {}` is **not** the same as passing an empty lambda (e.g., `[]{}`). Using this explicit tag type allows internal algorithms to use compile-time checks to completely eliminate the callback invocation branch at compile time. This guarantees strict zero-cost abstractions, vastly improves performance in unoptimized/debug builds, and speeds up compilation times. 
>
>



    

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/core.hpp`

