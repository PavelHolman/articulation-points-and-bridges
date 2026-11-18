# Articulation points and bridges

Finds the **articulation points** (cut vertices) and **bridges** (cut edges) of an undirected graph in linear time,
using depth-first search with discovery times and low-link values (Tarjan's algorithm). C++20 (GCC), one file.

Removing an articulation point or a bridge disconnects the graph, so they mark its single points of failure.

## How it works

The DFS records when each vertex is first reached (`in`) and the earliest discovery time reachable from its subtree
through at most one back edge (`low`). For a tree edge `v → w`:

- `low[w] == in[w]` means nothing in `w`'s subtree reaches above `w`, so `v–w` is a **bridge**;
- `low[w] ≥ in[v]` means `v` separates `w`'s subtree, so `v` is an **articulation point** (non-root case);
- the DFS root is an articulation point exactly when it has more than one child in the DFS tree.

The search restarts from every unvisited vertex, so disconnected graphs work too.

## Run

```sh
g++ -std=c++20 -O2 main.cpp -o articulations && ./articulations
```

It uses `<bits/stdc++.h>`, so build it with GCC. `main` runs the built-in test graphs and prints `i/n` as each one passes.
