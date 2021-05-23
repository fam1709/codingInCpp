#include<iostream>
#include<type_traits>
#include<array>
#include<algorithm>
#include<functional>
#include<string>

template<typename F, typename T1>
auto bind(F functor, T1 first) {
	return[functor, first](auto...x)mutable {return std::invoke(functor, first, x...); };
}

template<typename F>
auto bind(F functor) {
	if constexpr (std::is_invocable<F, std::string, std::string>::value) {
		return[functor](auto a, auto...x)mutable {
			if constexpr (std::is_invocable<F, decltype(a), decltype(x)...>::value) {
				return std::invoke(functor, a, x...);
			}
			else {
				return bind(functor, a, x...);
			}
		};
	}
	else {
		return[functor](auto a, auto...x)mutable {
			if constexpr (std::is_invocable<F, decltype(a), decltype(x)...>::value) {
				return std::invoke(functor, a, x...);
			}
			else {
				return bind(functor, a, x...);
			}

		};
	}
}

template<typename F, typename...Prs>
auto bind(F functor, Prs...p) {
	return[functor, p...](auto a)mutable{return std::invoke(functor, a, p...); };
}

template<typename F, typename T1, typename T2>
auto bind(F functor, T1 first, T2 second) {
	return[functor, first, second](auto...x)mutable {return std::invoke(functor, first, second, x...); };
}