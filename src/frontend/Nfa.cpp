#include "frontend/Nfa.h"
#include "util/ParseUtil.h"
#include "string"

Nfa Nfa::NfaGeneration(const std::string& exp) {
    std::string parseExp = ParseUtil::toSuffix(exp);
    for(char c : parseExp) {
        switch (c) {
            case KLEENE_STATE: Kleene();break;
            case CONNECTION_STATE: Connection();break;
            case UNION_STATE: Union();break;
            default: NfaInit(c);
        }
    }
    if(OP_STACK.empty()) exit(1);

    Nfa nfa = OP_STACK.top();
    OP_STACK.pop();
    return nfa;
}

void Nfa::Kleene() {
    if (OP_STACK.empty()) exit(1);
    Nfa n = OP_STACK.top();
    OP_STACK.pop();

    int s = Nfa::newId();
    int t = Nfa::newId();

    Nfa *nnfa = new Nfa();

    for(int ns: n.s) nnfa->s.insert(ns);
    nnfa->s.insert(s);
    nnfa->s.insert(t);

    char empty_state = Empty_STATE;
    for(char c: n.alpha) nnfa->alpha.insert(c);
    nnfa->alpha.insert(empty_state);

    nnfa->s0 = s;

    nnfa->target.insert(t);

    for(Edge e: n.edges) nnfa->edges.insert(e);
    nnfa->edges.insert(*(new Edge(s, n.s0, empty_state)));
    nnfa->edges.insert(*(new Edge(s, t, empty_state)));
    for(int tar: n.target) nnfa->edges.insert(*(new Edge(tar, t, empty_state)));
    for(int tar: n.target) nnfa->edges.insert(*(new Edge(tar, n.s0, empty_state)));
    OP_STACK.push(*nnfa);
    free(nnfa);
}

void Nfa::Connection() {
    if (OP_STACK.empty()) exit(1);
    Nfa n1 = OP_STACK.top();    //先出来的是后面的那个
    OP_STACK.pop();
    if (OP_STACK.empty()) exit(1);
    Nfa n2 = OP_STACK.top();
    OP_STACK.pop();

    Nfa *nnfa = new Nfa();

    for(int ns: n1.s) nnfa->s.insert(ns);
    for(int ns: n2.s) nnfa->s.insert(ns);

    for(char c: n1.alpha) nnfa->alpha.insert(c);
    for(char c: n2.alpha) nnfa->alpha.insert(c);
    char empty_state = Empty_STATE;
    nnfa->alpha.insert(empty_state);

    nnfa->s0 = n2.s0;

    for(int tar: n1.target) nnfa->target.insert(tar);

    for(Edge e: n1.edges) nnfa->edges.insert(e);
    for(Edge e: n2.edges) nnfa->edges.insert(e);
    for(int tar:n2.target) {
        nnfa->edges.insert(*(new Edge(tar,n1.s0, empty_state)));
    }

    OP_STACK.push(*nnfa);
    free(nnfa);
}

void Nfa::Union() {
    if (OP_STACK.empty()) exit(1);
    Nfa a1 = OP_STACK.top();
    OP_STACK.pop();
    if (OP_STACK.empty()) exit(1);
    Nfa a2 = OP_STACK.top();
    OP_STACK.pop();

    int s = Nfa::newId();
    int t = Nfa::newId();

    Nfa *fina = new Nfa();
    for(int as: a1.s) fina->s.insert(as);
    for(int as: a2.s) fina->s.insert(as);
    fina->s.insert(s);
    fina->s.insert(t);

    for(char ap: a1.alpha) fina->alpha.insert(ap);
    for(char ap: a2.alpha) fina->alpha.insert(ap);
    char empty_state = Empty_STATE;
    fina->alpha.insert(empty_state);

    fina->s0 = s;

    fina->target.insert(t);

    for(Edge e: a1.edges) fina->edges.insert(e);
    for(Edge e: a2.edges) fina->edges.insert(e);
    fina->edges.insert(*(new Edge(s, a1.s0, empty_state)));
    fina->edges.insert(*(new Edge(s, a2.s0, empty_state)));

    for(int tar: a1.target) fina->edges.insert(*(new Edge(tar, t, empty_state)));
    for(int tar: a2.target) fina->edges.insert(*(new Edge(tar, t, empty_state)));
    OP_STACK.push(*fina);
    free(fina);
}

void Nfa::NfaInit(char c) {
    Nfa *a = new Nfa();

    int s = Nfa::newId();
    int t = Nfa::newId();

    a->s.insert(s);
    a->s.insert(t);
    a->alpha.insert(c);
    a->s0 = s;
    a->target.insert(t);
    a->edges.insert(*(new Edge(s, t, c)));

    OP_STACK.push(*a);
    free(a);
}


