#include <iostream>
#include <type_traits>


class X {
public:
    double operator()(double v) {
        return v;
    }

    double dx(double v) {
        return 1;
    }

};

X x;

class Cte {
public:
    Cte(double c) : c(c) {}

    double operator()(double v) {
        return c;
    }

    double dx(double v) {
        return 0;
    }

private:
    double c;
};

template <typename F1, typename F2>
class Multiply {
public:

    Multiply(F1 f1, F2 f2) : f1(f1), f2(f2) { }

    double operator()(double v) {
        return f1(v) * f2(v);
    }

    double dx(double v) {
        return f1(v) * f2.dx(v) + f1.dx(v) * f2(v);
    }

    double e(double v) {
        return f1(v) * f2(v);
    }


private:
    F1 f1;
    F2 f2;

};

template <typename F1, typename F2>
Multiply<F1, F2> operator * (F1 f1, F2 f2) {

    return Multiply<F1, F2>(f1, f2);
}


template <typename F2>
Multiply<Cte, F2> operator * (double n, F2 f2) {
    return Multiply<Cte, F2>(n, f2);
}

template <typename F2>
Multiply<Cte, F2> operator * (int n, F2 f2) {
    return Multiply<Cte, F2>(n, f2);
}


template <typename F1>
Multiply<F1, Cte> operator * (F1 f1, double n) {
    return Multiply<F1, Cte>(f1, n);
}

template <typename F1>
Multiply<F1, Cte> operator * (F1 f1, int n) {
    return Multiply<F1, Cte>(f1, n);
}

/****************************************************************/

template <typename F1, typename F2>
class Sum {
public:
    Sum(F1 f1, F2 f2) : f1(f1), f2(f2) {}

    double operator()(double v) {
        return f1(v) + f2(v);
    }

    double dx(double v) {
        return f1.dx(v) + f2.dx(v);
    }

    double e(double v) {
        return f1(v) + f2(v);
    }

private:
    F1 f1;
    F2 f2;

};

template <typename F1, typename F2>
Sum<F1, F2> operator + (F1 f1, F2 f2) {
    return Sum<F1, F2>(f1, f2);
}

template <typename F2>
Sum<Cte, F2> operator + (double n, F2 f2) {
    return Sum<Cte, F2>(n, f2);
}

template <typename F1>
Sum<F1, Cte> operator + (F1 f1, double n) {
    return Sum<F1, Cte>(f1, n);
}


template <typename F2>
Sum<Cte, F2> operator + (int n, F2 f2) {
    return Sum<Cte, F2>(n, f2);
}

template <typename F1>
Sum<F1, Cte> operator + (F1 f1, int n) {
    return Sum<F1, Cte>(f1, n);
}

/****************************************************************/

template <typename F1, typename F2>
class Sub {
public:
    Sub(F1 f1, F2 f2) : f1(f1), f2(f2) {}

    double operator()(double v) {
        return f1(v) - f2(v);
    }

    double dx(double v) {
        return f1.dx(v) - f2.dx(v);
    }

    double e(double v) {
        return f1(v) - f2(v);
    }

private:
    F1 f1;
    F2 f2;

};

template <typename F1, typename F2>
Sub<F1, F2> operator - (F1 f1, F2 f2) {
    return Sub<F1, F2>(f1, f2);
}

template <typename F2>
Sub<Cte, F2> operator - (double n, F2 f2) {
    return Sub<Cte, F2>(n, f2);
}

template <typename F1>
Sub<F1, Cte> operator - (F1 f1, double n) {
    return Sub<F1, Cte>(f1, n);
}

template <typename F2>
Sub<Cte, F2> operator - (int n, F2 f2) {
    return Sub<Cte, F2>(n, f2);
}

template <typename F1>
Sub<F1, Cte> operator - (F1 f1, int n) {
    return Sub<F1, Cte>(f1, n);
}


/****************************************************************/

template <typename F1, typename F2>
class Div {
public:
    Div(F1 f1, F2 f2) : f1(f1), f2(f2) {}

    double operator()(double v) {
        return f1(v) / f2(v);
    }

    double dx(double v) {
        return ((f1.dx(v) * f2(v)) - (f1(v) * f2.dx(v))) / (f2(v) * f2(v));
    }

    double e(double v) {
        return f1(v) / f2(v);
    }

private:
    F1 f1;
    F2 f2;

};

template <typename F1, typename F2>
Div<F1, F2> operator / (F1 f1, F2 f2) {
    return Div<F1, F2>(f1, f2);
}

template <typename F2>
Div<Cte, F2> operator / (double n, F2 f2) {
    return Div<Cte, F2>(n, f2);
}

template <typename F2>
Div<Cte, F2> operator / (int n, F2 f2) {
    return Div<Cte, F2>(n, f2);
}


template <typename F1>
Div<F1, Cte> operator / (F1 f1, double n) {
    return Div<F1, Cte>(f1, n);
}

template <typename F1>
Div<F1, Cte> operator / (F1 f1, int n) {
    return Div<F1, Cte>(f1, n);
}



/****************************************************************/

template <typename F1>
class Power {
public:
    Power(F1 f1, double n) : f1(f1), exp(n) {}

    double operator()(double v) {
        return std::pow(f1(v), exp);
    }

    double dx(double v) {
        return exp * f1.dx(v) * std::pow(f1(v), exp - 1);
    }

    double e(double v) {
        return std::pow(f1(v), exp);
    }

private:
    F1 f1;
    double exp;

};

template <typename F1>
Power<F1> operator ->* (F1 f1, int n) {
    return Power<F1>(f1, n);
}

template <typename F1>
Power<F1> operator ->* (F1 f1, double n) {
    static_assert(!std::is_same_v<double, decltype(n)>, "Operador de potenciação definido apenas para inteiros");
    return Power<F1>(f1, n);
}

/****************************************************************/


template <typename F1>
class Sin {
public:

    Sin(F1 f) : _f(f) {}

    double operator()(double v) {
        return std::sin(_f(v));
    }

    double dx(double v) {
        return (std::cos(_f(v)) * (_f.dx(v)));
    }

    double e(double v) {
        return std::sin(_f(v));
    }


private:
    F1 _f;

};

template <typename F1>
class Cos {
public:

    Cos(F1 f) : _f(f) {}

    double operator()(double v) {
        return std::cos(_f(v));
    }

    double dx(double v) {
        return (std::sin(_f(v)) * (_f.dx(v))) * -1;
    }

    double e(double v) {
        return std::cos(_f(v));
    }


private:
    F1 _f;

};

template <typename F1>
class Exp {
public:

    Exp(F1 f) : _f(f) {}

    double operator()(double v) {
        return std::exp(_f(v));
    }

    double dx(double v) {
        return (std::exp(_f(v)) * (_f.dx(v)));
    }

    double e(double v) {
        return std::exp(_f(v));
    }


private:
    F1 _f;

};

template <typename F1>
class Log {
public:

    Log(F1 f) : _f(f) {}

    double operator()(double v) {
        return std::log(_f(v));
    }

    double dx(double v) {
        return ((1 / (_f(v))) * (_f.dx(v)));
    }

    double e(double v) {
        return std::log(_f(v));
    }


private:
    F1 _f;

};

template<typename F>
Sin<F> sin(F f) {
    return Sin<F>(f);
}

template<typename F>
Cos<F> cos(F f) {
    return Cos<F>(f);
}

template<typename F>
Exp<F> exp(F f) {
    return Exp<F>(f);
}

template<typename F>
Log<F> log(F f) {
    return Log<F>(f);
}
