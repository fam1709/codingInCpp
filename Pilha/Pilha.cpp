#include<iostream>


#define MAX_STACK 10

using namespace std;

class StackInt {
public:

    StackInt() {
        for (int i = 0; i < MAX_STACK; i++) {
            tab[i] = 0;
        }
        current = 0;
    }

    const StackInt& operator = (const StackInt& p) {
        current = p.current;
        for (int i = 0; i < current; i++) {
            tab[i] = p.tab[i];
        }
        return *this;
    }


    StackInt& operator << (int value) {
        push(value);
        return *this;
    }

    void push (int value) {
        tab[current++] = value;

    }
    int pop() {
        return tab[--current];

    }

    void print(ostream& o) {
        int i = 0;
        o << "[ ";
        while (current != i) {
            o << tab[i];
            i++;
            if (current > i) {
                o << ", ";

            }
        }
        o << " ]";
    }

private:
    int tab[MAX_STACK];
    int current;
};
