/* Redefining operator |. Same as using for_each

for_each( tab, tab+10,  []( int x ) { cout << x*x << endl; } );*/

#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include<algorithm>


using namespace std;

void print(int x) { cout << x << " "; }


template<typename V, typename L, int tam>
V* operator | (V(&v)[tam], L lambda) {
	for (int i = 0; i < tam; i++) {
		lambda(v[i]);

	}

	return v;
}

template<typename V, typename L>
vector<V> operator | (vector<V> v, L lambda) {
	for (int i = 0; i < v.size(); i++) {
		lambda(v[i]);

	}

	return v;
}

