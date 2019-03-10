#include <iostream>
#include <stdexcept>
template<typename tip>
struct Cvor{
    tip element;
    Cvor<tip> *prethodni;
    Cvor() {}
    Cvor(const Cvor<tip> &c) : element(c->element), prethodni(c->prethodni) {}
    Cvor(const tip &element, Cvor<tip> *prethodni) :element(element), prethodni(prethodni) {}
};
template<typename tip>
class Stek {
    int broj_elemenata=0;
    Cvor<tip> *kraj=nullptr;
    public:
    Stek() {}
    Stek(const Stek<tip> &s) {
        /*Cvor<tip> *p(s.kraj);
        while(p!=0) {
            DodajEl(p->element);
            p=p->prethodni;
        }*/
        broj_elemenata=s.broj_elemenata;
        Cvor<tip> *pom(s.kraj);
        Cvor<tip> *p = new Cvor<tip>;
        kraj=p;
        p->element = pom->element;
        broj_elemenata--;
        while (broj_elemenata!=0) {
            p->prethodni=new Cvor<tip>;
            p=p->prethodni;
            pom=pom->prethodni;
            p->element=pom->element;
            broj_elemenata--;
        }
        broj_elemenata=s.broj_elemenata;
    }
    Stek &operator=(const Stek<tip> &s) {
        if(&s==this) return *this;
        brisi();
        /*Cvor<tip> *p(s.kraj);
        while(p!=0) {
            DodajEl(p->element);
            p=p->prethodni;
        }*/
        broj_elemenata=s.broj_elemenata;
        Cvor<tip> *pom(s.kraj);
        Cvor<tip> *p = new Cvor<tip>;
        kraj=p;
        p->element = pom->element;
        broj_elemenata--;
        while (broj_elemenata!=0) {
            p->prethodni=new Cvor<tip>;
            p=p->prethodni;
            pom=pom->prethodni;
            p->element=pom->element;
            broj_elemenata--;
        }
        broj_elemenata=s.broj_elemenata;
        return *this;
    }
    ~Stek() {
        brisi();
    }
    void brisi() {
        while (broj_elemenata > 0) {
            Cvor<tip> *pom = kraj;
            kraj= kraj->prethodni;
            delete pom;
            pom=nullptr;
            broj_elemenata--;
        }
    }
    void stavi(const tip &el) {
        Cvor<tip> *p= new Cvor<tip>(el,nullptr);
        if(broj_elemenata==0) kraj=p;
        else {
            Cvor<tip>*pom=kraj;
            kraj=p;
            kraj->prethodni=pom;
        }
        broj_elemenata++;
    }
    tip skini() {
        if(broj_elemenata==0) throw std::logic_error("Prazan stek");
        tip zadnji(kraj->element);
        if(broj_elemenata==1) {
            delete kraj;
            broj_elemenata=0;
            return zadnji;
        }
        Cvor<tip> *pom=kraj->prethodni;
        delete kraj;
        kraj=pom;
        broj_elemenata--;
        return zadnji;
    }
    tip &vrh() {
        if(broj_elemenata==0) throw std::logic_error("Prazan stek");
        //tip zadnji(kraj->element);
        return kraj->element;
    }
    int brojElemenata() {
        return broj_elemenata;
    }
    
};

template<typename tip>
int Broj_Elemenata(Stek<tip> &s) {
    return s.brojElemenata();
}
template<typename tip>
tip Skini(Stek<tip> &s) {
    try {
        return s.skini();
    }
    catch(...) {
        throw;
    }
}
template<typename tip>
void Stavi(Stek<tip> &s, const tip &el) {
    s.stavi(el);
}
template<typename tip>
void Brisi(Stek<tip> &s) {
    s.brisi();
}
template<typename tip>
tip &Vrh(Stek<tip> &s) {
    try {
        return s.vrh();
    }
    catch(...) {
        throw;
    }
}
int main() {
    try {
        Stek<int> s;
        std::cout<<"Unesite elemente (-1 za prekid): "<<std::endl;
        while (1) {
            int broj;
            std::cin>>broj;
            if(broj==-1) break;
            Stavi(s,broj);
        }
        Stek<int>s1(s);
        Stek<int>s2;
        s2=s;
        std::cout<<Broj_Elemenata(s)<<std::endl;
        std::cout<<Vrh(s)<<std::endl;
        while (Broj_Elemenata(s) != 0) {
            std::cout<<Skini(s);
        }
        std::cout<<std::endl;
        std::cout<<"*"<<Broj_Elemenata(s1)<<"*"<<Broj_Elemenata(s2)<<std::endl;
        Brisi(s1);
        Stavi(s2,1);
        Stavi(s2,2);
        std::cout<<Broj_Elemenata(s2)<<std::endl<<Vrh(s2)<<std::endl;
        while (Broj_Elemenata(s2) != 0) {
            std::cout<<Skini(s2);
        }
    }
    catch(...) {
        std::cout<<"Something went wrong";
    }
    return 0;
}
