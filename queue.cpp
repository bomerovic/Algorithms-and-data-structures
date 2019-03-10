#include <iostream>
#include <stdexcept>

template<typename tip>
struct Cvor{
    tip element;
    Cvor<tip> *sljedeci;
    Cvor() {}
    Cvor(const Cvor<tip> &c) : element(c->element), sljedeci(c->sljedeci) {}
    Cvor(const tip &element, Cvor<tip> *sljedeci) :element(element), sljedeci(sljedeci) {}
};

template<typename tip>
class Red {
    int broj_elemenata=0;
    Cvor<tip> *pocetak=nullptr;
    Cvor<tip> *kraj=nullptr;
    public:
    Red() {}
    Red(const Red<tip> &r) {
        broj_elemenata=r.broj_elemenata;
        Cvor<tip> *pom(r.pocetak);
        Cvor<tip> *p = new Cvor<tip>;
        pocetak=p;
        p->element = pom->element;
        broj_elemenata--;
        while (broj_elemenata!=0) {
            p->sljedeci=new Cvor<tip>;
            p=p->sljedeci;
            pom=pom->sljedeci;
            p->element=pom->element;
            broj_elemenata--;
        }
        broj_elemenata=r.broj_elemenata;
    }
    Red &operator =(const Red<tip> &r) {
        if(&r==this) return *this;
        brisi();
        broj_elemenata=r.broj_elemenata;
        Cvor<tip> *pom(r.pocetak);
        Cvor<tip> *p = new Cvor<tip>;
        pocetak=p;
        p->element = pom->element;
        broj_elemenata--;
        while (broj_elemenata!=0) {
            p->sljedeci=new Cvor<tip>;
            p=p->sljedeci;
            pom=pom->sljedeci;
            p->element=pom->element;
            broj_elemenata--;
        }
        broj_elemenata=r.broj_elemenata;
        return *this;
    }
    ~Red() {
        brisi();
    }
    void brisi() {
        while (broj_elemenata > 0) {
            Cvor<tip> *pom = pocetak;
            pocetak= pocetak->sljedeci;
            delete pom;
            pom=nullptr;
            broj_elemenata--;
        }
    }
    void stavi(const tip &el) {
        Cvor<tip> *p= new Cvor<tip>(el,nullptr);
        if(broj_elemenata==0) pocetak=kraj=p;
        else {
            kraj->sljedeci=p;
            kraj=kraj->sljedeci;
            /*kraj=p;
            kraj->prethodni=pom;*/
        }
        broj_elemenata++;
    }
    tip skini() {
        if(broj_elemenata==0) throw std::logic_error("Prazan red");
        tip prvi(pocetak->element);
        if(broj_elemenata==1) {
            delete pocetak;
            broj_elemenata=0;
            return prvi;
        }
        Cvor<tip> *pom=pocetak->sljedeci;
        delete pocetak;
        pocetak=pom;
        broj_elemenata--;
        return prvi;
    }
    tip &celo() {
        if(broj_elemenata==0) throw std::logic_error("Prazan red");
        return pocetak->element;
    }
    int brojElemenata() {
        return broj_elemenata;
    }
};
template<typename tip>
int Broj_Elemenata(Red<tip> &r) {
    return r.brojElemenata();
}
template<typename tip>
tip Skini(Red<tip> &r) {
    try {
        return r.skini();
    }
    catch(...) {
        throw;
    }
}
template<typename tip>
void Stavi(Red<tip> &r, const tip &el) {
    r.stavi(el);
}
template<typename tip>
void Brisi(Red<tip> &r) {
    r.brisi();
}
template<typename tip>
tip &Celo(Red<tip> &r) {
    try {
        return r.celo();
    }
    catch(...) {
        throw;
    }
}
int main() {
    try {
        Red<int> r;
        std::cout<<"Unesite elemente (-1 za prekid): "<<std::endl;
        while (1) {
            int broj;
            std::cin>>broj;
            if(broj==-1) break;
            Stavi(r,broj);
        }
        Red<int>r1(r);
        Red<int>r2;
        r2=r;
        std::cout<<Broj_Elemenata(r)<<std::endl;
        std::cout<<Celo(r)<<std::endl;
        while (Broj_Elemenata(r) != 0) {
            std::cout<<Skini(r);
        }
        std::cout<<std::endl;
        std::cout<<"*"<<Broj_Elemenata(r1)<<"*"<<Broj_Elemenata(r2)<<std::endl;
        Brisi(r1);
        Stavi(r2,1);
        Stavi(r2,2);
        std::cout<<Broj_Elemenata(r2)<<std::endl<<Celo(r2)<<std::endl;
        while (Broj_Elemenata(r2) != 0) {
            std::cout<<Skini(r2);
        }
    }
    catch(...) {
        std::cout<<"Something went wrong";
    }
    return 0;
}
