#include<iostream>
#include<vector>

using namespace std;



class StackInt {
    public:
        StackInt(){}
         
        StackInt( int capacity) {
            vector<int> aux(capacity);
            stack = aux;
            stack.clear();
            
        }
    
        ~StackInt(){
           
        }
        
        StackInt(const StackInt& p){
            stack = p.stack;
            for(unsigned int i = 0; i < p.stack.size(); i++){
                stack[i] = p.stack[i];
        }
    }
    
        const StackInt& operator = (const StackInt& p) {
             if (p.stack.capacity() < stack.capacity()) {
            stack.resize(p.stack.capacity());
            stack.shrink_to_fit();
            stack = p.stack;
            return *this;
        }
           stack.reserve(p.stack.capacity());
           stack = p.stack;
            return *this;
        }
        
        
        StackInt& operator << (int valor){
            push(valor);
            return *this;
        }
        
        void push (int valor){
             stack.push_back(valor);
            
        }
        int pop() {
            int pop = stack.back();
            stack.pop_back();
            return pop;
            
        }
        
        void print( ostream& o) {
            o<<"[ ";
           for(unsigned int i = 0; i<stack.size(); i++){
               if( (i + 1) == stack.size()){
                   o<< stack[i];
               }
               else{
               o << stack[i]<< ", ";
               }
           }
           o << " ]";
         }
    
        int capacity(){
            return stack.capacity();
        }
        
        void resize(int n){
            stack.resize(n);
            stack.shrink_to_fit();

            
        }
    
    
    private:
        vector<int> stack;
     
};
