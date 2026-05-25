#include <iostream>
#include <cmath>

class SimpleCalc
{
    protected:
    double num1,num2;
    char op;
    double res;

    public:
    double calc()
    {
        std::cout<<"enter prompt"<<std::endl;
        std::cin>>num1 >> op >> num2;
        if(op != '+' && op != '-' && op != '*' && op != '/')
        {
            std::cout<<"invlaid operator"<<std::endl;
            return 0;
        }

        if(op == '+'){ res = num1 + num2;}
        else if(op == '-'){ res = num1 - num2;}
        else if(op == '*'){ res = num1 * num2;}
        else if(op == '/'){ res = num1 / num2;}
        return res;
    }

    void show()
    {
        std::cout<<res<<std::endl;
    }

};

class ScientificCalc
{
    protected:
    double num;
    std::string op;
    double res;

    public:
    double SciCalc()
    {
        std::cout<<"enter prompt"<<std::endl;
        std::cin>>op >> num;
        if(op != "root" && op != "Clog" && op != "cubert" && op != "sq"){std::cout<<"invalid operator"<<std::endl;}

        if(op == "root"){ res = std::sqrt(num); }

        else if(op == "Clog"){ res = log10(num); }

        else if(op == "cubert"){    res = cbrt(num);}

        else if(op == "sq"){    res = pow(num, 2);}

        return res;
        
    }

    void showRes()
    {
        std::cout<<res<<std::endl;
    }

};

class HybridCalc : public SimpleCalc, public ScientificCalc
{
    protected:
    int mode;
    public:
    HybridCalc()
    {
        std::cout<<"choose mode: \n 1. S1mple                                 2. Scientific"<<std::endl;
        std::cin>>mode;
        if(mode != 1 && mode != 2){
            std::cout<<"invalid mode input"<<std::endl;}
        if(mode == 1){
            calc();
            show();}
        else if(mode == 2){
            std::cout<<"info: \n log10 = Clog \n square = sq \n square root = root \n cube root  = cubert"<<std::endl;
            std::cout<<"prompt instructions \n prompt: \n <operator> <value>"<<std::endl;
            SciCalc();
            showRes();}

    }
};

int main()
{
    HybridCalc eq1;
    return 0;
}