# ICPC Template

This is supposed to be used for teams preparing for icpc
It assumes basic codes and knowledge upto a 2100 cf rated is present with the members

# Features

- balanced codes between length, readibility and performance
- tested on various platforms
- latex setup to create template pdf conforming to ICPC specifications

# Folders

- codes: contains all the codes under unmbrella topics
- latex: used to create pdf
- samples: contains use cases

# Directory Structure
```
.
└───codes
    ├── Geometry
    │   └── Code_1.h
    ├── Graph
    │   └── Code_1.h
    └── Polynomials
        ├── Code_1.h
        └── Code_2.h
.
└───samples
    ├── Geometry
    │   └── Topic1
    │   │      └── problem1_platform1.cpp
    │   │      └── problem2_platform1.cpp
    │   │      └── problem1_platform2.cpp
    │   └── Topic2
    │   │      └── problem1_platform1.cpp
    │   │      └── problem2_platform1.cpp
    │   │      └── problem1_platform2.cpp
    ├── Graph
    │   └── Topic1
    │   │      └── problem1_platform1.cpp
    │   │      └── problem2_platform1.cpp
    │   │      └── problem1_platform2.cpp
```

# Coding Guidelines

- Please adhere to the directory structures
- Try using PRs and reviews to help in better collaboration
- Each PR for sample solution should contain solution idea, time and memory complexity analysis
- Each sample soluton should contain problem link in code itself
- Each sample solution should use head path with path name from repo main file, see existing samples for details
- It is advisable to copy paste from implementation than including header file in case it helps improving readibility of code