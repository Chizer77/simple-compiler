#include <vector>
#include <algorithm>
#include "DfaTest.h"
#include "frontend/Dfa.h"

void DfaTest::DfaMinimizeTest01() {
    Dfa *dfa = new Dfa();
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());

    std::vector<int> v(dfa->s.begin(), dfa->s.end());
    std::sort(v.begin(), v.end());

    dfa->alpha.insert('a');
    dfa->alpha.insert('b');

    int x = v[0];
    int a = v[1];
    int b = v[2];
    int c = v[3];
    int d = v[4];
    int e = v[5];
    int f = v[6];
    int g = v[7];
    int h = v[8];


    dfa->s0 = x;

    dfa->target.insert(c);
    dfa->target.insert(d);
    dfa->target.insert(f);
    dfa->target.insert(g);
    dfa->target.insert(h);

    dfa->edges.insert(*(new Edge(x, a, 'a')));
    dfa->edges.insert(*(new Edge(x, e, 'b')));
    dfa->edges.insert(*(new Edge(a, a, 'a')));
    dfa->edges.insert(*(new Edge(a, b, 'b')));
    dfa->edges.insert(*(new Edge(e, a, 'a')));
    dfa->edges.insert(*(new Edge(e, e, 'b')));
    dfa->edges.insert(*(new Edge(b, a, 'a')));
    dfa->edges.insert(*(new Edge(b, c, 'b')));
    dfa->edges.insert(*(new Edge(c, d, 'a')));
    dfa->edges.insert(*(new Edge(c, f, 'b')));
    dfa->edges.insert(*(new Edge(d, d, 'a')));
    dfa->edges.insert(*(new Edge(d, g, 'b')));
    dfa->edges.insert(*(new Edge(f, d, 'a')));
    dfa->edges.insert(*(new Edge(f, f, 'b')));
    dfa->edges.insert(*(new Edge(g, d, 'a')));
    dfa->edges.insert(*(new Edge(g, h, 'b')));
    dfa->edges.insert(*(new Edge(h, d, 'a')));
    dfa->edges.insert(*(new Edge(h, f, 'b')));

    Dfa *opa = Dfa::DfaMinimize(*dfa);

    free(opa);
}

void DfaTest::DfaMinimizeTest02() {
    Dfa *dfa = new Dfa();
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());
    dfa->s.insert(Dfa::newId());

    std::vector<int> v(dfa->s.begin(), dfa->s.end());
    std::sort(v.begin(), v.end());

    dfa->alpha.insert('a');
    dfa->alpha.insert('b');
    dfa->alpha.insert('c');
    dfa->alpha.insert('d');

    int i0 = v[0];
    int i1 = v[1];
    int i2 = v[2];
    int i3 = v[3];
    int i4 = v[4];
    int i5 = v[5];
    int i6 = v[6];

    dfa->s0 = i0;
    dfa->target.insert(i5);
    dfa->target.insert(i6);

    dfa->edges.insert(*(new Edge(i0, i2, 'a')));
    dfa->edges.insert(*(new Edge(i0, i1, 'b')));
    dfa->edges.insert(*(new Edge(i1, i3, 'a')));
    dfa->edges.insert(*(new Edge(i1, i1, 'b')));
    dfa->edges.insert(*(new Edge(i2, i5, 'b')));
    dfa->edges.insert(*(new Edge(i2, i4, 'd')));
    dfa->edges.insert(*(new Edge(i3, i6, 'b')));
    dfa->edges.insert(*(new Edge(i3, i2, 'c')));
    dfa->edges.insert(*(new Edge(i3, i4, 'd')));
    dfa->edges.insert(*(new Edge(i4, i3, 'a')));
    dfa->edges.insert(*(new Edge(i5, i5, 'b')));
    dfa->edges.insert(*(new Edge(i6, i5, 'b')));

    Dfa *opa = Dfa::DfaMinimize(*dfa);

    free(opa);
}

void DfaTest::Nfa2DfaTest01() {
    Nfa *nfa = Nfa::Generation("(a|b)^b");
    Dfa *dfa = Dfa::Nfa2Dfa(*nfa);
    free(nfa);

    free(dfa);
}


