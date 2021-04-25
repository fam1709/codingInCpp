/* apply function
* params: an iterable of variable type, function(functions, lamda expr and functors ) that changes the iterable elements
* 
* returns: a vector of the same type as the return type of the function that is passed as argument
* */

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <initializer_list>


using namespace std;

//VECTORS

template<typename vec_type, typename fun_ret_type, typename fun_arg_type>
vector<fun_ret_type> apply(vector<vec_type> n, fun_ret_type(*f)(fun_arg_type)) {
	vector<fun_ret_type> v;
	for (int i = 0; i < n.size(); i++) {
		auto x = (*f)(n[i]);
		v.push_back(x);
	}
	return v;

}

//INT V[]

template<typename vec_type, typename fun_ret_type, typename fun_arg_type, int tam>
vector<fun_ret_type> apply(vec_type(&n)[tam], fun_ret_type(*f)(fun_arg_type)) {
	vector<fun_ret_type> v;
	for (int i = 0; i < tam; i++) {
		auto x = (*f)(n[i]);
		v.push_back(x);
	}
	return v;

}

//INITIALIZER_LIST

template<typename vec_type, typename fun_ret_type, typename fun_arg_type>
vector<fun_ret_type> apply(initializer_list<vec_type>n, fun_ret_type(*f)(fun_arg_type)) {
	vector<fun_ret_type> v;
	for (auto a : n) {
		auto x = (*f)(a);
		v.push_back(x);
	}
	return v;

}

//FUNCTORS & LAMBDAS

template<typename vec_type, typename Lam>
auto apply(initializer_list<vec_type>n, Lam l) {
	int x = 1;
	vector<decltype(l(x))> v;
	for (auto a : n) {
		auto x = l(a);
		v.push_back(x);
	}
	return v;

}

template<typename vec_type, typename Lam>
auto apply(vector<vec_type> n, Lam l) {
	vector<decltype(l(n[0])) > vec;
	for (int i = 0; i < n.size(); i++) {
		auto a = l(n[i]);
		vec.push_back(a);
	}

	return vec;

}





