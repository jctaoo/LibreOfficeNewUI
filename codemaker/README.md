# Language Code Generators for UNOIDL Entities

Generators for language-binding--specific representations of UNOIDL entities:

- `cppumaker` generates header (`.hdl` and `.hpp`) files for the C++ UNO language
  binding
- `javamaker` generates class files for the JVM language binding
- the codemaker for .NET is in module `cli_ure`

Some of the code is re-used by the skeletonmakers in module `unodevtools`.

Note the different terminology used by cppumaker vs. gbuild for the three
variants that can be generated by cppumaker for some of the inline functions:

    cppumaker switch: -L;    cpputype.cxx: light;          gbuild: normal;
    cppumaker switch: none;  cpputype.cxx: normal;         gbuild: bootstrap;
    cppumaker switch: -C;    cpputype.cxx: comprehensive;  gbuild: comprehensive;

which can be a source of confusion.