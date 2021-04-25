#include<iostream>


#define MAX_PILHA 10

using namespace std;

class PilhaInt {
public:

    PilhaInt() {
        for (int i = 0; i < MAX_PILHA; i++) {
            tab[i] = 0;
        }
        atual = 0;
    }

    const PilhaInt& operator = (const PilhaInt& p) {
        atual = p.atual;
        for (int i = 0; i < atual; i++) {
            tab[i] = p.tab[i];
        }
        return *this;
    }


    PilhaInt& operator << (int valor) {
        empilha(valor);
        return *this;
    }

    void empilha(int valor) {
        tab[atual++] = valor;

    }
    int desempilha() {
        return tab[--atual];

    }

    void print(ostream& o) {
        int i = 0;
        o << "[ ";
        while (atual != i) {
            o << tab[i];
            i++;
            if (atual > i) {
                o << ", ";

            }
        }
        o << " ]";
    }

private:
    int tab[MAX_PILHA];
    int atual;
};
