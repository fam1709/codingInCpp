#include <initializer_list>
#include <iostream>
#include <memory>

using namespace std;


class AbstractPair {
public:

    AbstractPair() {}

    virtual void print(ostream& o) = 0;

    virtual ~AbstractPair() {}

};

template <typename A, typename B>
class PairImpl : public AbstractPair {
public:

    PairImpl(const A& a, const B& b) : par1(a), par2(b) {
    }

    ~PairImpl() {}

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


    void print(ostream& o) {
        pont->print(o);
    }

    ~Pair() {}

private:
    shared_ptr<AbstractPair> pont = nullptr;
};

void print(ostream& o, const initializer_list<Pair>& lista) {
    for (auto i : lista) {
        i.print(o);
    }
}