#include "nfaTest.h"
#include "frontend/nfa/Nfa.h"
#include <cassert>
#include <vector>
#include <set>

void nfaTest::nfaInitTest01() {
    Nfa opa = Nfa::NfaGeneration("a");

    assert(opa.s.size() == 2);

    auto beg = opa.s.begin();
    int id0 = *(beg)++;
    int id1 = *(beg)++;

    assert(opa.alpha.size() == 1);
    assert(opa.alpha.find('a') != opa.alpha.end());

    assert(opa.s0 == id0);

    assert(opa.target.size() == 1);
    assert(opa.target.find(id1) != opa.target.end());

    assert(opa.edges.size() == 1);

    Edge se = *opa.edges.begin();
    assert(se.start == id0);
    assert(se.target == id1);
    assert(se.alpha == 'a');
}

void nfaTest::unionTest01() {
    Nfa opa = Nfa::NfaGeneration("ab|");

    assert(opa.s.size() == 6);

    std::vector<int> v;
    for(int ts: opa.s) v.emplace_back(ts);

    assert(opa.alpha.size() == 3);
    assert(opa.alpha.find('@') != opa.alpha.end());
    assert(opa.alpha.find('a') != opa.alpha.end());
    assert(opa.alpha.find('b') != opa.alpha.end());

    assert(opa.s0 == v[4]);

    assert(opa.target.size() == 1);
    assert(opa.target.find(v[5]) != opa.target.end());

    assert(opa.edges.size() == 6);
    std::set<Edge> es;
    es.insert(*(new Edge(v[4], v[0], '@')));
    es.insert(*(new Edge(v[4], v[2], '@')));
    es.insert(*(new Edge(v[0], v[1], 'a')));
    es.insert(*(new Edge(v[2], v[3], 'b')));
    es.insert(*(new Edge(v[1], v[5], '@')));
    es.insert(*(new Edge(v[3], v[5], '@')));

    assert(es == opa.edges);
}