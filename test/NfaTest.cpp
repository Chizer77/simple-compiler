#include "NfaTest.h"
#include "frontend/Nfa.h"
#include "util/ParseUtil.h"
#include <cassert>
#include <vector>
#include <algorithm>

void NfaTest::nfaInitTest01() {
    Nfa opa = Nfa::NfaGeneration("a");

    assert(opa.s.size() == 2);

    auto beg = opa.s.begin();
    int id1 = *(beg)++;
    int id0 = *(beg)++;

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

void NfaTest::kleeneTest01() {
    Nfa opa = Nfa::NfaGeneration("a^");

    assert(opa.s.size() == 4);

    std::vector<int> v;
    for(int ts: opa.s) v.emplace_back(ts);
    sort(v.begin(), v.end());

    assert(opa.alpha.size() == 2);
    assert(opa.alpha.find('@') != opa.alpha.end());
    assert(opa.alpha.find('a') != opa.alpha.end());

    assert(opa.s0 == v[2]);

    assert(opa.target.size() == 1);
    assert(opa.target.find(v[3]) != opa.target.end());

    assert(opa.edges.size() == 5);
    std::unordered_set<Edge, Edge::EdgeHasher> es;
    es.insert(*(new Edge(v[0], v[1], 'a')));
    es.insert(*(new Edge(v[1], v[0], '@')));
    es.insert(*(new Edge(v[1], v[3], '@')));
    es.insert(*(new Edge(v[2], v[3], '@')));
    es.insert(*(new Edge(v[2], v[0], '@')));

    assert(es == opa.edges);
}


void NfaTest::concatenationTest01() {
    Nfa opa = Nfa::NfaGeneration("a&b");

    assert(opa.s.size() == 4);

    std::vector<int> v;
    for(int ts: opa.s) v.emplace_back(ts);
    sort(v.begin(), v.end());

    assert(opa.alpha.size() == 3);
    assert(opa.alpha.find('@') != opa.alpha.end());
    assert(opa.alpha.find('a') != opa.alpha.end());
    assert(opa.alpha.find('b') != opa.alpha.end());

    assert(opa.s0 == v[0]);

    assert(opa.target.size() == 1);
    assert(opa.target.find(v[3]) != opa.target.end());

    assert(opa.edges.size() == 3);
    std::unordered_set<Edge, Edge::EdgeHasher> es;
    es.insert(*(new Edge(v[0], v[1], 'a')));
    es.insert(*(new Edge(v[1], v[2], '@')));
    es.insert(*(new Edge(v[2], v[3], '@')));

    assert(es == opa.edges);
}

void NfaTest::unionTest01() {
    Nfa opa = Nfa::NfaGeneration("a|b");

    assert(opa.s.size() == 6);

    std::vector<int> v;
    for(int ts: opa.s) v.emplace_back(ts);
    sort(v.begin(), v.end());

    assert(opa.alpha.size() == 3);
    assert(opa.alpha.find('@') != opa.alpha.end());
    assert(opa.alpha.find('a') != opa.alpha.end());
    assert(opa.alpha.find('b') != opa.alpha.end());

    assert(opa.s0 == v[4]);

    assert(opa.target.size() == 1);
    assert(opa.target.find(v[5]) != opa.target.end());

    assert(opa.edges.size() == 6);
    std::unordered_set<Edge, Edge::EdgeHasher> es;
    es.insert(*(new Edge(v[4], v[0], '@')));
    es.insert(*(new Edge(v[4], v[2], '@')));
    es.insert(*(new Edge(v[0], v[1], 'a')));
    es.insert(*(new Edge(v[2], v[3], 'b')));
    es.insert(*(new Edge(v[1], v[5], '@')));
    es.insert(*(new Edge(v[3], v[5], '@')));

    assert(es == opa.edges);
}

void NfaTest::parseUtilTest() {
    std::string str1 = ParseUtil::toSuffix("a|(a|b)^cd");
    assert(str1 == "aab|^cd&&|");

    str1 = ParseUtil::toSuffix("(a|b)^(cd)");
    assert(str1 == "ab|^cd&&");

    str1 = ParseUtil::toSuffix("a|bc^d");
    assert(str1 == "abc^d&&|");

    str1 = ParseUtil::toSuffix("a(bc)d");
    assert(str1 == "abc&d&&");

    str1 = ParseUtil::toSuffix("(a|b)^^(c^d^)^");
    assert(str1 == "ab|^^c^d^&^&");
}