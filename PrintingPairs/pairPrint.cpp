#include<initializer_list>
#include <iostream>
#include <memory>

using namespace std;


class AbstractPair {
public:

    AbstractPair() {}


    //metodo abstrato    
    virtual void print(ostream& o) = 0;

    virtual ~AbstractPair() {}

};

template <typename A, typename B>
class PairImpl : public AbstractPair {
public:

    PairImpl(A a, B b) {
        par1 = a;
        par2 = b;

    }

    void print(ostream& o) {
        o << par1 << " = " << par2 << endl;
    }

private:
    A par1;
    B par2;


};

class Pair {
public:
    template <typename A, typename B>
    Pair(A a, B b) {
        pont = make_shared<PairImpl<A, B>>(a, b);
    }


    friend void print(ostream& o, const initializer_list<Pair>& lista);

private:
    shared_ptr<AbstractPair> pont;
};

void print(ostream& o, const initializer_list<Pair>& lista) {
    const Pair* p = lista.begin();
    while (p != lista.end()) {
        p->pont->print(o);
        p++;
    }

}