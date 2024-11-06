# Additional functionality

- [Force inlining](#force-inlining)

<br />

## Force inlining

The `CPP-GL` library provides an option to enable force inlining of seleced functions and methods in the implementation. This is defined through the `gl_attr_force_inline` macro, which by default is equivalent to the `inline` keyword.

To enable the force inlining functionality you have to add the following in your program:

```c++
#define GL_CONFIG_FORCE_INLINE
```

> [!NOTE]
> Force inlining is supported only for the GNU G++ and CLang++ compilers.
