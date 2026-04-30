

# Struct gl::algorithm::decision



[**ClassList**](annotated.md) **>** [**algorithm**](namespacegl_1_1algorithm.md) **>** [**decision**](structgl_1_1algorithm_1_1decision.md)



_Represents a generic tri-state decision for control flow._ [More...](#detailed-description)

* `#include <hgl/algorithm/core.hpp>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum std::uint8\_t | [**eval**](#enum-eval)  <br>_The underlying evaluation states._  |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  [**eval**](structgl_1_1algorithm_1_1decision.md#enum-eval) | [**value**](#variable-value)  <br>_The stored evaluation state._  |
















## Public Functions

| Type | Name |
| ---: | :--- |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**decision**](#function-decision-12) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**eval**](structgl_1_1algorithm_1_1decision.md#enum-eval) value) <br>_Constructs a decision from an explicit evaluation state._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**decision**](#function-decision-22) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) value) <br>_Constructs a decision from a boolean value._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**decision**](structgl_1_1algorithm_1_1decision.md) & | [**operator=**](#function-operator) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) value) <br>_Assigns a boolean value to the decision._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) | [**operator bool**](#function-operator-bool) () const<br>_Evaluates the decision as a boolean._  |
|  [**constexpr**](group__GL-Core.md#variable-invalid_id_v) [**bool**](group__GL-Core.md#variable-invalid_id_v) | [**operator==**](#function-operator_1) ([**const**](group__GL-Core.md#variable-invalid_id_v) [**eval**](structgl_1_1algorithm_1_1decision.md#enum-eval) value) const<br>_Compares the decision against a specific evaluation state._  |




























## Detailed Description


Used by custom predicates to determine how to proceed with a given item, execution step, or operation. While heavily utilized in graph traversal algorithms, it is entirely decoupled from graph-specific logic and can be used in any generic context requiring explicit accept/reject/abort semantics. 


    
## Public Types Documentation




### enum eval 

_The underlying evaluation states._ 
```cpp
enum algorithm::decision::eval {
    accept,
    reject,
    abort
};
```




<hr>
## Public Attributes Documentation




### variable value 

_The stored evaluation state._ 
```cpp
eval gl::algorithm::decision::value;
```




<hr>
## Public Functions Documentation




### function decision [1/2]

_Constructs a decision from an explicit evaluation state._ 
```cpp
inline constexpr algorithm::decision::decision (
    const  eval value
) 
```





**Parameters:**


* `value` The tri-state evaluation. 




        

<hr>



### function decision [2/2]

_Constructs a decision from a boolean value._ 
```cpp
inline constexpr algorithm::decision::decision (
    const  bool value
) 
```





**Parameters:**


* `value` `true` maps to `accept`, `false` maps to `reject`. 




        

<hr>



### function operator= 

_Assigns a boolean value to the decision._ 
```cpp
inline constexpr  decision & algorithm::decision::operator= (
    const  bool value
) 
```





**Parameters:**


* `value` `true` maps to `accept`, `false` maps to `reject`. 



**Returns:**

A reference to this decision. 





        

<hr>



### function operator bool 

_Evaluates the decision as a boolean._ 
```cpp
inline constexpr algorithm::decision::operator bool () const
```





**Returns:**

`true` if the decision is `accept`, `false` otherwise. 





        

<hr>



### function operator== 

_Compares the decision against a specific evaluation state._ 
```cpp
inline constexpr  bool algorithm::decision::operator== (
    const  eval value
) const
```





**Parameters:**


* `value` The state to compare against. 



**Returns:**

`true` if the states match. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `include/gl/algorithm/core.hpp`

