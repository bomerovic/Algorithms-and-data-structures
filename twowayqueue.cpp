#include <iostream>
#include <stdexcept>

template<typename tip>
struct Cvor{
    tip element;
    Cvor<tip> *sljedeci;
    Cvor<tip> *prethodni;
    Cvor() {}
    Cvor(const Cvor<tip> &c) : element(c->element), sljedeci(c->sljedeci), prethodni(c->prethodni) {}
    Cvor(const tip &element, Cvor<tip> *sljedeci, Cvor<tip> *prethodni) :element(element), sljedeci(sljedeci), prethodni(prethodni) {}
};

template<typename tip>
class DvostraniRed {
    int broj_elemenata=0;
    Cvor<tip> *pocetak=nullptr, *kraj=nullptr;
    public:
    DvostraniRed() {}
    DvostraniRed(const DvostraniRed<tip> &dred) {
        broj_elemenata=dred.broj_elemenata;
        Cvor<tip> *pom(dred.pocetak);
        Cvor<tip> *p = new Cvor<tip>;
        p->element = pom->element;
        pocetak=p;
        pom=pom->sljedeci;
        while (pom!=nullptr) {
            //Cvor<tip> *novi=new Cvor<tip>;
            p->sljedeci=new Cvor<tip>;
            p->sljedeci->element=pom->element;
            p->sljedeci->prethodni=p;
            //novi->prethodni=p;
            //p=novi;
            pom=pom->sljedeci;
            p=p->sljedeci;
        }
        kraj=p;
    }
    DvostraniRed &operator =(const DvostraniRed<tip> &dred) {
        if(this==&dred) return *this;
        brisi();
        broj_elemenata=dred.broj_elemenata;
        Cvor<tip> *pom(dred.pocetak);
        Cvor<tip> *p = new Cvor<tip>;
        p->element = pom->element;
        pocetak=p;
        pom=pom->sljedeci;
        while (pom!=nullptr) {
            //Cvor<tip> *novi=new Cvor<tip>;
            p->sljedeci=new Cvor<tip>;
            p->sljedeci->element=pom->element;
            p->sljedeci->prethodni=p;
            //novi->prethodni=p;
            //p=novi;
            pom=pom->sljedeci;
            p=p->sljedeci;
        }
        kraj=p;
        return *this;
    }
    ~DvostraniRed() {
        brisi();
    }
    int brojElemenata() {
        return broj_elemenata;
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
    void staviNaVrh(const tip &el) {
        Cvor<tip>*p=new Cvor<tip>(el,nullptr,nullptr);
        if(broj_elemenata==0) {
            kraj=pocetak=p;
        }
        else {
            Cvor<tip>*ispredkraja=kraj;
            kraj=p;
            kraj->prethodni=ispredkraja;
            ispredkraja->sljedeci=kraj;
        }
        broj_elemenata++;
    }
    void staviNaCelo(const tip &el) {
        Cvor<tip> *p= new Cvor<tip>(el,nullptr,nullptr);
        if(broj_elemenata==0) pocetak=kraj=p;
        else {
            Cvor<tip>*ispredpocetka=pocetak;
            pocetak=p;
            pocetak->sljedeci=ispredpocetka;
            ispredpocetka->prethodni=pocetak;
            /*kraj->sljedeci=p;
            kraj=kraj->sljedeci;*/
            /*kraj=p;
            kraj->prethodni=pom;*/
        }
        broj_elemenata++;
    }
    tip skiniSaVrha() {
        if(broj_elemenata==0) throw std::logic_error("Prazan dvostrani red");
        tip zadnji(kraj->element);
        if(broj_elemenata==1) {
            delete kraj;
            pocetak=nullptr;
            broj_elemenata=0;
            return zadnji;
        }
        Cvor<tip> *pom=kraj;
        kraj=kraj->prethodni;
        delete pom;
        broj_elemenata--;
        return zadnji;
    }
    tip skiniSaCela() {
        if(broj_elemenata==0) throw std::logic_error("Prazan dvostrani red");
        tip prvi(pocetak->element);
        if(broj_elemenata==1) {
            delete pocetak;
            kraj=nullptr;
            broj_elemenata=0;
            return prvi;
        }
        /*Cvor<tip> *pom=pocetak->sljedeci;
        delete pocetak;
        pom->prethodni=nullptr;
        pocetak=pom;
        broj_elemenata--;*/
        Cvor<tip> *pom=pocetak;
        pocetak=pocetak->sljedeci;
        delete pom;
        broj_elemenata--;
        return prvi;
    }
    
    tip &vrh() {
        if(broj_elemenata==0) throw std::logic_error("Prazan dvostrani red");
        return kraj->element;
    }
    tip &celo() {
        if(broj_elemenata==0) throw std::logic_error("Prazan dvostrani red");
        return pocetak->element;
    }
};

bool Stavi_na_vrh() {
    DvostraniRed<int> dr;
    for (int i(1); i<=5; i++)
	    dr.staviNaVrh(i);
        if(dr.brojElemenata()==5) return true;
    return false;
}

bool Stavi_na_celo() {
    DvostraniRed<int> dr;
    for (int i(1); i<=5; i++)
	    dr.staviNaCelo(i);
        if(dr.brojElemenata()==5) return true;
    return false;
}
bool Skini_sa_vrha() {
    DvostraniRed<int> dr;
    for (int i(1); i<=5; i++)
	    dr.staviNaVrh(i);
	int broj = dr.skiniSaVrha();
        if(dr.brojElemenata()==4) return true;
    return false;
}
bool Skini_sa_cela() {
    DvostraniRed<int> dr;
    for (int i(1); i<=5; i++)
	    dr.staviNaCelo(i);
	int broj = dr.skiniSaCela();
        if(dr.brojElemenata()==4) return true;
    return false;
}
bool Brisi() {
    DvostraniRed<int> dr;
    for (int i(1); i<=20; i++)
	    dr.staviNaCelo(i);
	dr.brisi();
	if(dr.brojElemenata()==0) return true;
	return false;
}
bool Konst() {
    DvostraniRed<int> dr;
    for (int i(1); i<=20; i++)
	    dr.staviNaCelo(i);
	DvostraniRed<int> dr1(dr);
	DvostraniRed<int> dr2 = dr;
	if(dr.brojElemenata()==dr1.brojElemenata()==dr2.brojElemenata()) return true;
	return false;
}
bool Vrh() {
    DvostraniRed<int> dr;
    for (int i(1); i<=20; i++)
	    dr.staviNaVrh(i);
	if(dr.vrh()==20) return true;
	return false;
}
bool Celo() {
    DvostraniRed<int> dr;
    for (int i(1); i<=20; i++)
	    dr.staviNaCelo(i);
	if(dr.celo()==20) return true;
	return false;
}
int main() {
    if(Stavi_na_celo()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    if(Stavi_na_vrh()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    if(Skini_sa_cela()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    if(Skini_sa_vrha()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    if(Brisi()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    if(Vrh()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    if(Celo()) std::cout<<"OK";
    else std::cout<<"NOT OK";
    std::cout<<std::endl;
    return 0;
}
