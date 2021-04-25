/* Implementation of vectors and their operations:
* Adding, Subtracting, Multiplying by a scalar, Dividing by a scalar
* Dot Product and Cross Product
* 
* overriding the operators , * <<
* 
* Vector<N,T> N = dimmension
*             T = type 

*/

#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include<algorithm>


using namespace std;

template<int N, typename T>
class Vetor {
public:

    class MeioDaOperacaoAtr {

        MeioDaOperacaoAtr(Vetor* v) : vetor(v) {}

        friend class Vetor;
    public:
        MeioDaOperacaoAtr operator , (T n) {
            if (dim == 0) {
                cout << "Nao pode inserir mais de " << N << " elementos" << endl;
                return 0;
            }
            *vetor = n;
            dim--;
            return *this;
        }

    private:
        Vetor* vetor;
        //dimensao do vetor
        int dim = N - 1;

    };


    class MeioDoProdutoVetorial {

        MeioDoProdutoVetorial(Vetor* v) : vetor(v) {  }
        friend class Vetor;
    private:
        Vetor* vetor;
        //dimensao do vetor
        int dim = N - 1;

    };



    void print(ostream& o) const {
        for (auto x : vetor)
            o << x << " ";
    }

    MeioDaOperacaoAtr operator = (T n) {
        vetor.push_back(n);
        return MeioDaOperacaoAtr(this);
    }


    //operador * para *Vetor
    MeioDoProdutoVetorial operator * () {

        return MeioDoProdutoVetorial(this);
    }

    //operador ** para produto vetorial, entre Vetor( ou this) e MeioProdutoVetorial
    Vetor<N, T>& operator *(MeioDoProdutoVetorial w) {
        Vetor<N, T> aux = *w.vetor;
        Vetor<N, T> result;
        result = 0, 0, 0;
        result.vetor[0] = this->vetor[1] * aux.vetor[2] - this->vetor[2] * aux.vetor[1];
        result.vetor[1] = this->vetor[2] * aux.vetor[0] - this->vetor[0] * aux.vetor[2];
        result.vetor[2] = this->vetor[0] * aux.vetor[1] - this->vetor[1] * aux.vetor[0];

        return *this = result;

    }

    //funcao somaVetores
    Vetor<N, T> somaVetor(Vetor<N, T> v) {
        for (unsigned int i = 0; i < vetor.size(); i++) {

            vetor[i] = vetor[i] + v.vetor[i];
        }

        return *this;

    }

    //funcao multiplicacao de vetor por escalar
    Vetor<N, T> escalaVetor(double escala) {
        for (unsigned int i = 0; i < vetor.size(); i++) {

            vetor[i] = vetor[i] * escala;
        }

        return *this;

    }

    //operador / para divisao por um escalar
    Vetor<N, T>& operator / (double escalar) {
        for (int i = 0; i < vetor.size(); i++) {
            vetor[i] = vetor[i] / escalar;
        }

        return *this;
    }



    //operador - para subtrair vetores
    Vetor<N, T>& operator - (const Vetor<N, T>& w) {
        if (vetor.size() != w.vetor.size()) { cout << "Vetores de dimensoes distintas nao podem ser somados" << endl; }
        for (int i = 0; i < vetor.size(); i++) {
            vetor[i] = vetor[i] - w.vetor[i];
        }
        return *this;
    }


    //operador * para produto escalar, vetor*vetor
    double operator * (const Vetor<N, T>& w) {
        double result = 0;
        for (unsigned int i = 0; i < vetor.size(); i++) {
            result += vetor[i] * w.vetor[i];
        }
        return result;
    }



private:
    vector<T> vetor;

};

//operador + para somar vetores fora da classe
template<int N, typename T>
Vetor<N, T> operator + (Vetor<N, T> v, Vetor<N, T> w) {
    return v.somaVetor(w);
}


//operador * para multiplicar vetores por escalares fora da classe
template<int N, typename T>
Vetor<N, T> operator * (Vetor<N, T> v, double escalar) {
    return v.escalaVetor(escalar);
}


//operador * para multiplicacao por um escalar ordem inversa escalar*vetor
template<int N, typename T>
Vetor<N, T> operator * (double escalar, Vetor<N, T> v) {

    return v.escalaVetor(escalar);
}






template<int N, typename T>
ostream& operator << (ostream& o, const Vetor<N, T>& v) {
    v.print(o);
    return o;
}
