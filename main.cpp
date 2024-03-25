#include <bits/stdc++.h>

template <typename F, typename S>
struct std::hash <std::pair <F, S>> {
    std::size_t operator () (const std::pair <F, S> & p) const noexcept {
        return (std::hash <F>()(p.first)) ^ (std::hash <S>()(p.second) << 1);
    }
};

using Vertex = std::string;
using Edge = std::pair <Vertex, Vertex>;

using Vertices = std::unordered_set <Vertex>;
using Edges = std::unordered_set <Edge>;

using AdjacencyList = std::unordered_map <Vertex, Vertices>;

using Time = size_t;
using VertexTime = std::unordered_map <Vertex, Time>;

class ArticulationsAndBridges {
    AdjacencyList _graph;
    Vertices _articulations;
    Edges _bridges;
    VertexTime _in;
    VertexTime _low;
    Time _timer = 0;

    explicit ArticulationsAndBridges (AdjacencyList graph)
            : _graph(std::move(graph)) {
    }

    size_t DFS (const Vertex & v, const Vertex & u) {
        size_t n_neighbors = 0;
        _low[v] = _in[v] = ++ _timer;
        for (const Vertex & w: _graph[v]) {
            if (! _in.contains(w)) { // Tree edge
                DFS(w, v);
                if (_low[w] == _in[w]) {
                    _bridges.emplace(v, w);
                }
                if (_low[w] >= _in[v] && u != v) { // Non-Root
                    _articulations.emplace(v);
                }
                _low[v] = std::min(_low[v], _low[w]);

                n_neighbors++;
            } else if (w != u && _in[w] < _in[v]) { // Backwards edge
                _low[v] = std::min(_low[v], _in[w]);
            }
        }

        return n_neighbors;
    }

public:
    static std::pair <Vertices, Edges> Solve (const AdjacencyList & graph) {
        ArticulationsAndBridges ab(graph);
        for (const auto & [v0, _]: graph) {
            if (ab._in.contains(v0)) { continue; }
            if (ab.DFS(v0, v0) > 1) { // Root
                ab._articulations.emplace(v0);
            }
        }
        return {ab._articulations, ab._bridges};
    }
};

using Test = std::pair <std::pair <Vertices, Edges>, AdjacencyList>;

std::array TESTS = {
        Test {
                {
                        {"Hradcanska", "Malostranska", "Staromestska", "Mustek"},
                        {{"Dejvicka", "Hradcanska"}, {"Hradcanska", "Malostranska"}, {"Malostranska", "Staromestska"},
                         {"Staromestska", "Mustek"}, {"Mustek", "Muzeum"}}
                },
                {
                        {"Dejvicka", {"Hradcanska"}},
                        {"Hradcanska", {"Dejvicka", "Malostranska"}},
                        {"Malostranska", {"Hradcanska", "Staromestska"}},
                        {"Staromestska", {"Malostranska", "Mustek"}},
                        {"Mustek", {"Staromestska", "Muzeum"}},
                        {"Muzeum", {"Mustek"}},
                }
        },
        Test {
                {
                        {"2", "3", "4", "5", "02", "03", "04", "05"},
                        {{"1", "2"}, {"2", "3"}, {"3", "4"}, {"4", "5"}, {"5", "6"},
                         {"01", "02"}, {"02", "03"}, {"03", "04"}, {"04", "05"}, {"05", "06"}}
                },
                {
                        {"1", {"2"}},
                        {"2", {"1", "3"}},
                        {"3", {"2", "4"}},
                        {"4", {"3", "5"}},
                        {"5", {"4", "6"}},
                        {"6", {"5"}},
                        {"01", {"02"}},
                        {"02", {"01", "03"}},
                        {"03", {"02", "04"}},
                        {"04", {"03", "05"}},
                        {"05", {"04", "06"}},
                        {"06", {"05"}},
                }
        },
};

void test (const auto & tests) {
    size_t i = 0;
    for (const auto & test: tests) {
        const auto & [abExp, graph] = test;
        const auto & [artExp, briExp] = abExp;
        auto [art, bri] = ArticulationsAndBridges::Solve(graph);
        assert(std::is_permutation(std::cbegin(art), std::cend(art), std::cbegin(artExp)));
        assert(std::is_permutation(std::cbegin(bri), std::cend(bri), std::cbegin(briExp),
                                   [] (const Edge & e1, const Edge & e2) {
                                       auto [u1, v1] = e1;
                                       auto [u2, v2] = e2;
                                       return (u1 == u2 && v1 == v2) || (u1 == v2 && u2 == v1);
                                   }));
        std::cout << ++ i << "/" << tests.size() << std::endl;
    }
}

int main () {
    test(TESTS);
    return 0;
}
