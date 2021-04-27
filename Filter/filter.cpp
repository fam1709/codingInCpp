#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include<algorithm>
#include<type_traits>
#include<iterator>


using namespace std;

void print(int x) { cout << x << " "; }



template<typename V, typename L, int tam>
vector<V> operator | (V(&v)[tam], L lambda) {
	vector<V> aux;
	//filter for L lambda = boolean
	if constexpr (is_same<decltype(lambda(v[0])), bool>::value) {
		for (int i = 0; i < tam; i++) {
			if (lambda(v[i])) {
				aux.push_back(v[i]);
			}
		}

		return aux;
	}
	//stream
	else {
		for (int i = 0; i < tam; i++) {
			lambda(v[i]);
		}
		return aux;
	}
}


template<typename V, typename L>
auto operator | (vector<V> v, L lambda) {

	//filter for L lambda = &string::length
	if constexpr (is_same<decltype(invoke(lambda, v[0])), size_t>::value) {
		vector<int> aux;
		for (auto x : v) {
			int a = invoke(lambda, x);
			cout << a << endl;
			aux.push_back(a);

		}

		return aux;
	}
	//filter for L lambda = boolean
	else if constexpr (is_same<decltype(invoke(lambda, v[0])), bool>::value) {
		vector<int> aux;
		for (unsigned int i = 0; i < v.size(); i++) {
			if (lambda(v[i])) {
				aux.push_back(v[i]);
			}
		}

		return aux;
	}
	//filter for L lambda = string
	else if constexpr (is_same<decltype(invoke(lambda, v[0])), string>::value) {
		vector<string> ss;

		for (auto x : v) {
			ss.push_back(invoke(lambda, x));

		}
		return ss;
	}

	//stream
	else {

		for (unsigned int i = 0; i < v.size(); i++) {
			lambda(v[i]);
		}
		return v;
	}


}

template<typename V, typename X, typename L>
auto operator | (map<V, X> m, L lambda) {


	//filter for L lambda = pair<string,double> , returns map
	if constexpr (is_same< decltype(invoke(lambda, *m.begin())), pair<string, double> >::value) {
		map<string, double> aux;
		for (auto x : m) {
			pair<string, double> p = invoke(lambda, x);
			aux.insert(p);
		}

		return aux;

	}

	//filter for L lambda = bool, returns map
	else if constexpr (is_same< decltype(invoke(lambda, *m.begin())), bool >::value) {
		map<string, string> aux2;

		for (auto x : m) {
			if (invoke(lambda, x)) {
				pair<string, string> p = x;
				aux2.insert(p);
			}

		}
		return aux2;
	}

	//filter for L lambda = string, returns vector
	else if constexpr (is_same< decltype(invoke(lambda, *m.begin())), string >::value) {
		vector<string> s;
		for (auto x : m) {
			s.push_back(invoke(lambda, x));
		}

		return s;
	}

	//stream
	else {
		for (auto x : m) {
			lambda(x);
		}
		return m;
	}

}
