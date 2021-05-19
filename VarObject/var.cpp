#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <map>
#include <memory>
#include <variant>
#include <functional>

using namespace std;

class Var {
public:
    static constexpr auto newErro = [](string st) { return [st]() { return st; }; };

    class Undefined {};

    class Object {
    public:
        virtual ~Object() {}

        virtual void print(ostream& o) const {
            o << "object";
        }

        virtual Var invk(const Var& arg) const {
            throw newErro("Essa variável não pode ser usada como função");
        }

        virtual Var& lvalue(const Var& st) { return atr[get<STRING>(st.v)]; }
        virtual Var rvalue(const Var& st) const {
            if (auto x = atr.find(get<STRING>(st.v)); x != atr.end())
                return x->second;
            else
                return Var();
        }
        virtual int sizeVec() const {
            throw newErro("Essa variável não pode ser usada como array");
        }

    private:
        map<string, Var> atr;
    };

    template <typename F>
    class Function : public Object {
    public:
        Function(F f) : f(f) {}

        virtual void print(ostream& o) const {
            o << "function";
        }

        virtual Var invk(const Var& arg) const {
            if constexpr (std::is_invocable<F, decltype(arg)>::value) {
                if constexpr (is_same_v< void, invoke_result_t< F, decltype(arg) > >) {
                    invoke(f, arg);
                    return Undefined();
                }
                else {
                    return invoke(f, arg);
                }
            }
            else {
                if constexpr (std::is_invocable<F, int>::value) {
                    return invoke(f, Var::asInt(arg));

                }
                return Undefined();
            }
        }

    private:
        F f;
    };
    class Array : public Object {
    public:
        virtual void print(ostream& o) const {
            o << "array";
        }

        virtual Var& lvalue(const Var& st) {
            switch (st.v.index())
            {
            case(INT):
                if ((unsigned int)get<INT>(st.v) <= vec.size() && get<INT>(st.v) >= 0) {
                    if (!vec.empty()) {
                        if ((unsigned int)get<INT>(st.v) != vec.size()) {
                            return vec[get<INT>(st.v)];
                        }
                        vec.push_back(get<INT>(st.v));
                        return vec[get<INT>(st.v)];
                    }
                    vec.push_back(get<INT>(st.v));
                    return vec[get<INT>(st.v)];
                }
                if (get<INT>(st.v) < 0) {
                    string s = to_string(get<INT>(st.v));
                    return mp[s];
                }
                else {
                    vec.resize((size_t)(get<INT>(st.v)) + 1);
                    return vec[get<INT>(st.v)];
                }
            case(DOUBLE):
                return mp[to_string(get<DOUBLE>(st.v))];

            case(STRING):
                return mp[get<STRING>(st.v)];

            default:
                throw newErro("undefined");
            }
        }

        virtual Var rvalue(const Var& st) const {
            if (auto x = mp.find(get<STRING>(st.v)); x != mp.end())
                return x->second;
            else
                return Var();
        }

        virtual int sizeVec() const {
            return vec.size();
        }


    private:
        vector<Var> vec;
        map<string, Var> mp;
    };

    enum { UNDEFINED, BOOL, CHAR, INT, DOUBLE, STRING, OBJECT };

    typedef variant<Undefined, bool, char, int, double, string, shared_ptr<Object>> Variant;

    typedef invoke_result< decltype(newErro), string >::type Erro;

    /*Constructors*/

    Var() : v() {}
    Var(const char* st) : v(string(st)) {}
    Var(bool b) : v(b) {}
    Var(char c) : v(c) {}
    Var(int i) : v(i) {}
    Var(double d) : v(d) {}
    Var(string s) : v(s) {}
    Var(Object* o) : v(shared_ptr<Object>(o)) {}

    template <typename F>
    Var(F func) : v(shared_ptr<Object>(new Function<F>(func))) {}

    // template <typename F>
     //Var(const enable_if_t< is_invocable_r<Var, F, Var>::value, F>&& f) : v(shared_ptr<Object>(new Function<F>(f))) {}


     /*Assignment Operator*/

    const Var& operator = (bool b) { v = b; return *this; }
    const Var& operator = (char c) { v = c; return *this; }
    const Var& operator = (int i) { v = i; return *this; }
    const Var& operator = (double d) { v = d; return *this; }
    const Var& operator = (string s) { v = s; return *this; }
    const Var& operator = (Object* o) { v = shared_ptr<Object>(o); return *this; }
    const Var& operator = (const char* st) { v = string(st); return *this; }
    template<typename F>
    const Var& operator = (F func) { v = shared_ptr<Object>(new Function<F>(func)); return *this; }
    /*
    template <typename F>
     auto operator = (const F& f) -> const enable_if_t< is_invocable_r<Var, F, Var>::value, Var>& {
         v = shared_ptr<Object>(new Function<F>(f));
         return *this;
     }*/


     /*Operators for Function, Object and Array types*/

    Var operator()(const Var& arg) const {
        if (v.index() != OBJECT)
            throw newErro("Essa variável não pode ser usada como função");

        if (arg.v.index() == INT) {

            return get<OBJECT>(v)->invk(arg);
        }

        return get<OBJECT>(v)->invk(arg);
    }

    Var& operator[](Var arg) {
        if (v.index() != OBJECT)
            throw newErro("Essa variável não é um objeto");

        return get<OBJECT>(v)->lvalue(arg);
    }

    Var operator[](const Var& arg) const {
        if (v.index() != OBJECT)
            throw newErro("Essa variável não é um objeto");

        return get<OBJECT>(v)->rvalue(arg);
    }

    static constexpr int tipo(int a, int b) { return (a << 4) | b; }
    static constexpr int tipoUni(int a) { return a; }

    /***********Arithmetic Operators Methods**************/

    static Var sel_sum(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(INT, INT): return get<INT>(a.v) + get<INT>(b.v);
        case tipo(DOUBLE, DOUBLE): return get<DOUBLE>(a.v) + get<DOUBLE>(b.v);
        case tipo(CHAR, CHAR): return string("") + get<CHAR>(a.v) + get<CHAR>(b.v);
        case tipo(STRING, STRING): return get<STRING>(a.v) + get<STRING>(b.v);

        case tipo(INT, CHAR): return get<INT>(a.v) + get<CHAR>(b.v);
        case tipo(CHAR, INT): return get<CHAR>(a.v) + get<INT>(b.v);
        case tipo(DOUBLE, INT): return get<DOUBLE>(a.v) + get<INT>(b.v);
        case tipo(INT, DOUBLE): return get<INT>(a.v) + get<DOUBLE>(b.v);
        case tipo(STRING, CHAR): return get<STRING>(a.v) + get<CHAR>(b.v);
        case tipo(CHAR, STRING): return get<CHAR>(a.v) + get<STRING>(b.v);

        default:
            return Var();
        }
    }



    static Var sel_sub(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(INT, INT): return get<INT>(a.v) - get<INT>(b.v);
        case tipo(DOUBLE, DOUBLE): return get<DOUBLE>(a.v) - get<DOUBLE>(b.v);
        case tipo(CHAR, CHAR): return get<CHAR>(a.v) - get<CHAR>(b.v);

        case tipo(INT, CHAR): return get<INT>(a.v) - get<CHAR>(b.v);
        case tipo(CHAR, INT): return get<CHAR>(a.v) - get<INT>(b.v);
        case tipo(DOUBLE, INT): return get<DOUBLE>(a.v) - get<INT>(b.v);
        case tipo(INT, DOUBLE): return get<INT>(a.v) - get<DOUBLE>(b.v);

        default:
            return Var();
        }
    }

    static Var sel_mul(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(INT, INT): return get<INT>(a.v) * get<INT>(b.v);
        case tipo(DOUBLE, DOUBLE): return get<DOUBLE>(a.v) * get<DOUBLE>(b.v);

        case tipo(DOUBLE, INT): return get<DOUBLE>(a.v) * get<INT>(b.v);
        case tipo(INT, DOUBLE): return get<INT>(a.v) * get<DOUBLE>(b.v);

        default:
            return Var();
        }
    }

    static Var sel_div(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(INT, INT): return get<INT>(a.v) / get<INT>(b.v);
        case tipo(DOUBLE, DOUBLE): return get<DOUBLE>(a.v) / get<DOUBLE>(b.v);

        case tipo(DOUBLE, INT): return get<DOUBLE>(a.v) / get<INT>(b.v);
        case tipo(INT, DOUBLE): return get<INT>(a.v) / get<DOUBLE>(b.v);

        default:
            return Var();
        }
    }

    /***********Logic Operators Methods**************/



    static Var sel_less(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(BOOL, BOOL): 	return get<BOOL>(a.v) < get<BOOL>(b.v);
        case tipo(CHAR, CHAR): 	return get<CHAR>(a.v) < get<CHAR>(b.v);
        case tipo(INT, INT): 		return get<INT>(a.v) < get<INT>(b.v);
        case tipo(DOUBLE, DOUBLE): 	return get<DOUBLE>(a.v) < get<DOUBLE>(b.v);
        case tipo(STRING, STRING): 		return get<STRING>(a.v) < get<STRING>(b.v);

        case tipo(INT, CHAR): 	return get<INT>(a.v) < get<CHAR>(b.v);
        case tipo(CHAR, INT): 	return get<CHAR>(a.v) < get<INT>(b.v);
        case tipo(INT, STRING): return to_string(get<INT>(a.v)) < get<STRING>(b.v);
        case tipo(STRING, INT): return get<STRING>(a.v) < to_string(get<INT>(b.v));


        case tipo(INT, DOUBLE): 	return get<INT>(a.v) < get<DOUBLE>(b.v);
        case tipo(DOUBLE, INT): 	return get<DOUBLE>(a.v) < get<INT>(b.v);

        case tipo(STRING, CHAR): 	return get<STRING>(a.v) < string(get<CHAR>(b.v), 1);
        case tipo(CHAR, STRING): 	return string(get<CHAR>(a.v), 1) < get<STRING>(b.v);

        default:
            return Var();
        }
    }

    static Var sel_and(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(BOOL, BOOL): 	return get<BOOL>(a.v) && get<BOOL>(b.v);

        default:
            return Var();
        }
    }

    static Var sel_or(const Var& a, const Var& b) {
        switch (tipo(a.v.index(), b.v.index())) {
        case tipo(BOOL, BOOL): 	return get<BOOL>(a.v) || get<BOOL>(b.v);

        default:
            return Var();
        }
    }

    Var operator ! () { return sel_not(*this); }

    Var sel_not(const Var& a) {
        switch (a.v.index()) {
        case(UNDEFINED): return !0;
        case(BOOL): return !get<BOOL>(a.v);
        case(INT): return !(bool)get<INT>(a.v);
        case(CHAR): return !true;
        case(OBJECT): return !true;
        case(STRING): return !(get<STRING>(a.v).empty() ? false : true);


        default:
            return Var();
        }
    }

    /***********Class Methods**************/




    void print(ostream& o) const {
        switch (v.index()) {
        case UNDEFINED: o << "undefined"; break;
        case BOOL: o << (get<BOOL>(v) ? "true" : "false"); break;
        case CHAR: o << get<CHAR>(v); break;
        case INT: o << get<INT>(v); break;
        case DOUBLE: o << get<DOUBLE>(v); break;
        case STRING: o << get<STRING>(v); break;
        case OBJECT: get<OBJECT>(v)->print(o); break;

        default:
            throw newErro("Tipo indefinido (bug)");
        }
    }

    string asString() {
        switch (v.index()) {
        case UNDEFINED: return "undefined";
        case BOOL: return (get<BOOL>(v) ? "true" : "false");
        case CHAR: return string("") + (get<CHAR>(v));
        case INT: return to_string(get<INT>(v));
        case DOUBLE: return to_string(get<DOUBLE>(v));
        case STRING: return get<STRING>(v);
        case OBJECT: return "object";

        default:
            return "";
        }

    }

    static int asInt(const Var& a) {
        switch (tipoUni(a.v.index())) {
        case UNDEFINED: return 0;
        case BOOL: return (get<BOOL>(a.v) ? 1 : 0);
        case CHAR: return (get<CHAR>(a.v));
        case INT: return (get<INT>(a.v));
        case DOUBLE: return (int)(get<DOUBLE>(a.v));
        case STRING: return get<STRING>(a.v).length();
        case OBJECT: return 0;

        default:
            return 0;
        }

    }

    bool asBool() {
        switch (v.index()) {
        case UNDEFINED: return false;
        case BOOL: return (get<BOOL>(v));
        case CHAR: return (get<CHAR>(v));
        case INT: return (get<INT>(v));
        case DOUBLE: return (get<DOUBLE>(v));
        case STRING:
            if (get<STRING>(v).empty()) {
                return false;
            }
            else
                return true;

        case OBJECT: return true;

        default:
            return false;
        }

    }

    bool isNumber() {
        switch (v.index()) {
        case UNDEFINED: return false;
        case BOOL: return (get<BOOL>(v) ? true : false);
        case CHAR: return std::isdigit((string("") + (get<CHAR>(v)))[0]) ? true : false;
        case INT: return true;
        case DOUBLE: return true;
        case STRING: return (std::isdigit((get<STRING>(v)[0])) || std::isblank((get<STRING>(v)[0])) || (get<STRING>(v)).empty()) ? true : false;
        case OBJECT: return false;

        default:
            return false;
        }

    }

    Var forEach(Var a) const {
        for (int i = 0; i < get<OBJECT>(v)->sizeVec(); i++) {
            get<OBJECT>(a.v)->invk(get<OBJECT>(v)->lvalue(i));
        }
        return Var();
    }

    Var::Object* newArray() {
        return new Var::Array();
    }


    const Var filter(Var a) {
        Var b = newArray();

        for (int i = 0, j = 0; i < get<OBJECT>(v)->sizeVec(); i++) {

            if ((get<OBJECT>(a.v)->invk(get<OBJECT>(v)->lvalue(i))).asBool()) {
                b[j] = get<OBJECT>(v)->lvalue(i);
                j++;
            }
        }
        return b;
    }

    Var indexOf(Var a)const {
        if (a.v.index() == INT) {
            int aux = get<INT>(a.v);
            for (int i = 0; i < get<OBJECT>(v)->sizeVec(); i++) {
                int temp = Var::asInt(get<OBJECT>(v)->lvalue(i));
                if (temp == aux) {
                    return Var(i);
                }
            }
        }
        else {
            string saux = get<STRING>(a.v);
            for (int i = 0; i < get<OBJECT>(v)->sizeVec(); i++) {
                string stemp = get<OBJECT>(v)->lvalue(i).asString();
                if (stemp == saux) {
                    return  Var(i);
                }
            }
        }
        return Var(-1);

    }



private:
    Variant v;
};

ostream& operator << (ostream& o, const Var& v) {
    v.print(o);
    return o;
}

Var operator + (const Var& a, const Var& b) { return Var::sel_sum(a, b); }
Var operator - (const Var& a, const Var& b) { return Var::sel_sub(a, b); }
Var operator * (const Var& a, const Var& b) { return Var::sel_mul(a, b); }
Var operator / (const Var& a, const Var& b) { return Var::sel_div(a, b); }
Var operator < (const Var& a, const Var& b) { return Var::sel_less(a, b); }
Var operator || (const Var& a, const Var& b) { return Var::sel_or(a, b); }
Var operator && (const Var& a, const Var& b) { return Var::sel_and(a, b); }
Var operator %(const Var& a, int n) { return std::div(Var::asInt(a), n).rem; }


Var operator > (const Var& a, const Var& b) { return b < a; }
Var operator != (const Var& a, const Var& b) { return (a < b) || (b < a); }
Var operator == (const Var& a, const Var& b) { return !(a != b); }
Var operator <= (const Var& a, const Var& b) { return !(b < a); }
Var operator >= (const Var& a, const Var& b) { return !(a < b); }

Var::Object* newObject() {
    return new Var::Object();
}

Var::Object* newArray() {
    return new Var::Array();
}
