#include <iostream>

int fib_petlja(int n) {
    if(n<=1) return n;
    int pretprosli(0), prosli(1), rezultat;
    for(int i(2); i<=n; i++) {
        rezultat = pretprosli+prosli;
        pretprosli=prosli;
        prosli=rezultat;
    }
    return rezultat;
}

int fib2_0(int n, int prosli=1, int pretprosli=0, int rez=0) {
    if(n<=1) return rez;
    rez=prosli+pretprosli;
    pretprosli=prosli;
    prosli=rez;
    return fib2_0(n-1, prosli, pretprosli, rez);
}

int nzd(int x, int y) {
    if(y==0) return x;
    return nzd(y, x%y);
}

int main() {
    {
        std::cout<<fib2_0(35)<<std::endl;
        std::cout<<fib2_0(40)<<std::endl;
        std::cout<<fib2_0(1000)<<std::endl;
    }    
    
    {
        std::cout<<nzd(40,5)<<std::endl;
        std::cout<<nzd(255,66)<<std::endl;
    }
    return 0;
}
