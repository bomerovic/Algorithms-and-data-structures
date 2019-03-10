#include <iostream>
#include <stdexcept>

template<typename tip>
class Iterator;

template <typename tip>
class Lista
{
public:
    Lista() {} 
    virtual ~Lista() {} 
    virtual int brojElemenata()const = 0;
    virtual void obrisi()= 0;
    virtual void dodajIspred(const tip& el)= 0; 
    virtual void dodajIza(const tip& el)= 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0; 
    virtual bool prethodni() = 0; 
    virtual bool sljedeci() = 0; 
    virtual tip& trenutni()= 0;
    virtual tip trenutni() const=0;
    virtual tip operator [](int indeks)const=0;
    virtual tip &operator [](int indeks)=0;
};

template<typename tip>
class DvostrukaLista : public Lista<tip> {
    struct Cvor {
    tip element=0;
    Cvor *s=nullptr;
    Cvor *p=nullptr;
    Cvor(){}
    Cvor(const Cvor &c) : element(c->element), s(c->s), p(c->p) {}
    Cvor(const tip &element, Cvor *s, Cvor *p) : element(element), s(s), p(p) {}
};
    int duzina=0;
    Cvor *prvi=nullptr;
    Cvor *zadnji=nullptr;
    Cvor *tekuci=nullptr;
    public:
    //template<class t> friend class Iterator;
    friend class Iterator<tip>;
    DvostrukaLista():Lista<tip>() {}
    DvostrukaLista(const DvostrukaLista<tip> &dlista) {
        Cvor *pom(dlista.prvi);
        Cvor *pom2(nullptr);
        while (pom!=nullptr) {
            Cvor *novi = new Cvor;
            novi->element=pom->element;
            if(pom2==nullptr) {
                prvi=novi;
            }
            else {
                pom2->s=novi;
                novi->p=pom2;
            }
            pom2=novi;
            
            if(pom == dlista.tekuci) tekuci=novi;
            pom = pom->s;
        }
        zadnji=pom2;
        duzina=dlista.duzina;
    }
    DvostrukaLista &operator =(const DvostrukaLista<tip> &dlista) {
        if(this==&dlista) return *this;
        while (duzina>0) {
            tekuci=prvi;
            prvi=prvi->s;
            delete tekuci;
            tekuci=nullptr;
            duzina--;
        }
        Cvor *pom(dlista.prvi);
        Cvor *pom2(nullptr);
        while (pom!=nullptr) {
            Cvor *novi(new Cvor);
            novi->element=pom->element;
            if(pom2==nullptr) {
                prvi=novi;
            }
            else {
                pom2->s=novi;
                novi->p=pom2;
            }
            pom2=novi;
            
            if(pom == dlista.tekuci) tekuci=novi;
            pom = pom->s;
        }
        zadnji=pom2;
        duzina=dlista.duzina;
        return *this;
    }
    ~DvostrukaLista() {
        while (duzina>0) {
            tekuci=prvi;
            prvi=prvi->s;
            delete tekuci;
            tekuci=nullptr;
            duzina--;
        }
        
    }
    int brojElemenata()const{
        return duzina;
    }
    void obrisi() {
        if(duzina==1) {
            delete tekuci;
            prvi=tekuci=zadnji=nullptr;
        }
        else if(prvi==tekuci) {
            tekuci=tekuci->s;
            delete prvi;
            prvi=tekuci;
        }
        else if(tekuci==zadnji) {
            tekuci=zadnji->p;
            zadnji->p=nullptr;
            tekuci->s=nullptr;
            delete zadnji;
            zadnji=tekuci;
            
        }
        else {
            Cvor *ispredtekuceg = tekuci->p;
            Cvor *izatekuceg = tekuci->s;
            ispredtekuceg->s=tekuci->s;
            izatekuceg->p=tekuci->p;
            delete tekuci;
            tekuci=ispredtekuceg->s;
        }
        duzina--;
    }
    void dodajIspred(const tip& el) {
        Cvor *novi=new Cvor(el,nullptr,nullptr);
        if(duzina==0) {
            prvi=novi;
            tekuci=prvi;
            zadnji=prvi;
            
        } else if(tekuci==prvi) {
            tekuci->p=novi;
            novi->s = tekuci;
            prvi=novi;
        } else {
            
            Cvor *ispredtekuceg=tekuci->p;
            ispredtekuceg->s=novi;
            novi->p=ispredtekuceg;
            novi->s = tekuci;
            tekuci->p=novi;
        }
        duzina++;
        
    } 
    void dodajIza(const tip& el) {
        Cvor *novi=new Cvor(el,nullptr,nullptr);
        if(duzina==0) {
            prvi=novi;
            tekuci=prvi;
            zadnji=prvi;
        }
        if(duzina==1) {
            tekuci->s=novi;
            novi->p=tekuci;
            zadnji=novi;
        }
        else if(tekuci==zadnji) {
            novi->p=tekuci;
            tekuci->s=novi;
            zadnji=novi;
            zadnji->s=nullptr;
        }
        else {
            /*Cvor<tip> *izatekuceg = zadnji;
            while (izatekuceg->p != tekuci) {
                izatekuceg = izatekuceg->p;
            }*/
            novi->p=tekuci;
            novi->s = tekuci->s; 
            tekuci->s=novi;
            novi->s->p=novi;
            //novi=nullptr;
        }
        duzina++;
    }
    void pocetak() {
        if(brojElemenata() == 0) throw std::logic_error("Prazna lista");
        tekuci=prvi;
    }
    void kraj() {
        if(brojElemenata() == 0) throw std::logic_error("Prazna lista");
        tekuci=zadnji;
    }
    bool prethodni() {
        if(brojElemenata() == 0) throw std::logic_error("Prazna lista");
        if(tekuci==prvi) return false;
        tekuci=tekuci->p;
        return true;
    } 
    bool sljedeci() {
        if(brojElemenata() == 0) throw std::logic_error("Prazna lista");
        if(tekuci==zadnji) return false;
        tekuci=tekuci->s;
        return true;
    }
    tip& trenutni() {
        if(brojElemenata() == 0) throw std::logic_error("Prazna lista");
        return tekuci->element;
    }
    tip trenutni() const {
        if(brojElemenata() == 0) throw std::logic_error("Prazna lista");
        return tekuci->element;
    }
    tip operator [](int indeks)const {
        if(indeks<0 || indeks>=brojElemenata()) throw std::logic_error("Invalid index");
        Cvor *temp=prvi;
        int k=0;
        while(k<indeks) {
            temp=temp->s;
            k++;
        }
        return temp->element;
    }
    tip &operator [](int indeks) {
        if(indeks<0 || indeks>=brojElemenata()) throw std::logic_error("Invalid index");
        Cvor *temp=prvi;
        int k=0;
        while(k<indeks) {
            temp=temp->s;
            k++;
        }
        return temp->element;
    }
};

template<typename tip>
int Broj_Elemenata_liste(const DvostrukaLista<tip> &lista) {
    return lista.brojElemenata();
}
template<typename tip>
void Sljedeci_list(DvostrukaLista<tip> &lista) {
    lista.sljedeci();
}
template<typename tip>
void Prethodni_list(DvostrukaLista<tip> &lista) {
    lista.prethodni();
}
template<typename tip>
void Pocetak_list(DvostrukaLista<tip> &lista) {
    lista.pocetak();
}
template<typename tip>
void Kraj_list(DvostrukaLista<tip> &lista) {
    lista.kraj();
}
template<typename tip>
void Dodaj_Ispred_list(DvostrukaLista<tip> &lista, tip element) {
    lista.dodajIspred(element);
}
template<typename tip>
void Dodaj_Iza_list(DvostrukaLista<tip> &lista, tip element) {
    lista.dodajIza(element);
}
template<typename tip>
void Obrisi_list(DvostrukaLista<tip> &lista) {
    lista.obrisi();
}
template<typename tip>
tip Trenutni_list(DvostrukaLista<tip> &lista) {
    return lista.trenutni();
}

template<typename tip>
class Iterator {
    //const Lista<tip> *lista;
    const DvostrukaLista<tip> *dlista=nullptr;
    typename DvostrukaLista<tip>::Cvor* pok;
    public:
    Iterator(const Lista<tip> &lista) {
        dlista=(const DvostrukaLista<tip>*) &lista;
        pok=dlista->tekuci;
    }
    int brojElemenata() {
        return dlista->duzina;
    }
    void pocetak() {
        pok=dlista->prvi;
    }
    void kraj() {
        pok=dlista->zadnji;
    }
    bool prethodni() {
        if(pok== dlista->prvi) return false;
        pok = pok->p;
        return true;
    }
    bool sljedeci() {
        if(pok== dlista->zadnji) return false;
        pok = pok->s;
        return true;
    }
    tip &trenutni() {
        return pok->element;
    }
    
    
};

template<typename tip>
tip dajMaksimum(const Lista<tip> &n) {
    Iterator<tip> pom(n);
    pom.pocetak();
    tip maxi=pom.trenutni();
    for(int i(0); i<pom.brojElemenata(); i++) {
        if(pom.trenutni()>maxi) maxi=pom.trenutni();
        pom.sljedeci();
    }
    return maxi;
}

int main() {
    {
        Lista<int>* l;
        l = new DvostrukaLista<int>;
        for(int i(1); i<=10; i++) {
            l->dodajIspred(i);
        }
        for(int i(1); i<=10; i++) {
            l->dodajIza(i);
        }
        l->pocetak();
        for(int i(0); i<l->brojElemenata(); i++) {
            std::cout<<l->trenutni()<<" ";
            l->sljedeci();
        }
        std::cout<<std::endl;
        delete l;
    }
    {
        Lista<int>* l;
        l = new DvostrukaLista<int>;
        for(int i(1); i<=10; i++) {
            l->dodajIspred(i);
        }
        l->pocetak();
        for(int i(0); i<l->brojElemenata(); i++) {
            std::cout<<l->trenutni()<<" ";
            l->sljedeci();
        }
        std::cout<<std::endl;
        l->obrisi();
        l->pocetak();
        for(int i(0); i<l->brojElemenata(); i++) {
            std::cout<<l->trenutni()<<" ";
            l->sljedeci();
        }
        std::cout<<std::endl;
        delete l;
    }
    {
        Lista<int>* l;
        l = new DvostrukaLista<int>;
        for(int i(1); i<=5; i++) {
            l->dodajIza(10-i);
        }
        for(int i(0); i<5; i++) {
            std::cout<<(*l)[i]<<" ";
        }
        l->kraj();
        std::cout << std::endl;
        l->prethodni();
        std::cout<<l->trenutni()<<' ';
        l->prethodni();
        std::cout<<l->trenutni();
        std::cout << std::endl;
        delete l;
    }
    {
        DvostrukaLista<int> l;
        for (int i(1); i<=5; i++) {
            l.dodajIza(i);
        }
        DvostrukaLista<int> l2(l);
        DvostrukaLista<int> l3;
        l3=l;
        std::cout << l2.brojElemenata() << " ";
        std::cout << l3.brojElemenata();
        std::cout<<std::endl;
    }
    {
        try {
        DvostrukaLista<int> l;
            l.sljedeci();
        }
        catch(std::logic_error izuzetak) {
            std::cout<<izuzetak.what()<<std::endl;
        }
    }
    {
       try {
        DvostrukaLista<int> l;
            l.prethodni();
        }
        catch(std::logic_error izuzetak) {
            std::cout<<izuzetak.what()<<std::endl;
        } 
    }
    {
        DvostrukaLista<int> l;
        for (int i(1); i<=10; i++) {
            l.dodajIspred(i);
        }
        for (int i(11); i<=20; i++) {
            l.dodajIza(i);
        }    
        std::cout <<dajMaksimum(l);
    }
    return 0;
}
