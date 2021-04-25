#include<iostream>

using namespace std;

class StackInt {
public:

    StackInt(int size = 10) {
        tab = (int*)malloc(sizeof(int) * size);
        if (tab == NULL) exit(1);
        for (int i = 0; i < size; i++) {
            tab[i] = 0;
        }
        sz = size;
        current = 0;
    }

    ~StackInt() {
        free(tab);
    }
    //Copy Constructor
    StackInt(const StackInt& p) {
        current = p.current;
        tab = (int*)malloc(sizeof(int) * current);
        for (int i = 0; i < current; i++) {
            tab[i] = p.tab[i];
        }
    }


    const StackInt& operator = (const StackInt& p) {
        if (sz != p.sz) {
            free(tab);
            sz = p.sz;
            tab = (int*)malloc(sizeof(int) * sz);
            if (tab == NULL) exit(1);
        }
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

    void push(int value) {
        if (current == sz) {
            sz = sz * 2;
            resize(sz);
        }
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

    int size() {
        return sz;
    }

    void resize(int n) {
        while (n < current) {
            current--;
        }
        tab = (int*)realloc(tab, (n) * sizeof(int));
        sz = n;
    }


private:
    int* tab;
    int current;
    int sz;
};
