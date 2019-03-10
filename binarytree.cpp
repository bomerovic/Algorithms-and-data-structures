#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename tip1, typename tip2>
class Mapa {
    public:
    Mapa() {}
    virtual ~ Mapa() {}
    virtual tip2 operator [](const tip1 &a) const=0;
    virtual tip2 &operator[](const tip1 &a)=0;
    virtual int brojElemenata()const=0;
    virtual void obrisi()=0;
    virtual void obrisi(const tip1 &kljuc)=0;
};

template<typename tip1, typename tip2>
struct Pair{
    tip1 kljuc;
    tip2 vrijednost;
};

template<typename tip1, typename tip2>
class NizMapa : public Mapa<tip1,tip2> {
    Pair<tip1,tip2>* niz=nullptr;
    int kapacitet=100;
    int br_el=0;
    public:
    NizMapa() {
        niz = new Pair<tip1,tip2>[kapacitet];
    }
    NizMapa(const NizMapa<tip1,tip2> &m) : kapacitet(m.kapacitet), br_el(m.br_el) {
        niz = new Pair<tip1,tip2>[m.kapacitet];
        for(int i(0); i<m.br_el; i++)
            niz[i] = m.niz[i];
    }
    NizMapa(NizMapa<tip1,tip2> &&m) : kapacitet(m.kapacitet), br_el(m.br_el) {
        niz=m.niz;
    }
    NizMapa &operator = (const NizMapa<tip1,tip2> &m) {
        if(this==&m) return *this;
        delete[] niz;
        kapacitet = m.kapacitet;
        br_el = m.br_el;
        niz = new Pair<tip1,tip2>[m.kapacitet];
        for(int i(0); i<m.br_el; i++)
            niz[i] = m.niz[i];
        return *this;
    }
    NizMapa &operator = (NizMapa<tip1,tip2> &&m) {
        if(this==&m) return *this;
        delete[] niz;
        kapacitet=m.kapacitet;
        br_el=m.br_el;
        niz=m.niz;
        return *this;
    }
    ~NizMapa() {
        delete[]niz;
    }
    int brojElemenata() const override {
        return br_el;
    }
    tip2 operator [](const tip1 &a) const override {
        for(int i(0); i<br_el; i++) {
            if(niz[i].kljuc == a) {
                return niz[i].vrijednost;
            }
        }
        return tip2();
        
    }
    tip2 &operator[](const tip1 &a) override {
        for(int i(0); i<br_el; i++) {
            if(niz[i].kljuc == a) {
                return niz[i].vrijednost;
            }
        }
        if(br_el==kapacitet) {
            Pair<tip1,tip2>* novi=new Pair<tip1,tip2>[kapacitet*2];
            for(int i(0); i<br_el; i++) {
                novi[i]=niz[i];
            }
            kapacitet*=2;
            delete[]niz;
            niz=novi;
            novi=nullptr;
        }
        br_el++;
        niz[br_el-1].kljuc =a;
        niz[br_el-1].vrijednost=tip2();
        return niz[br_el-1].vrijednost;
        
    }
    void obrisi() {
        delete [] niz;
        kapacitet=100; br_el=0;
        niz = new Pair<tip1,tip2>[kapacitet];
    }
    
    void obrisi(const tip1 &kljuc) {
        for(int i(0); i<br_el; i++) {
            if(kljuc==niz[i].kljuc) {
                for(int j(i); j<br_el-1; j++) {
                    niz[i]=niz[i+1];
                }
            }
        }
        br_el--;
    }
};

template<typename tip1, typename tip2>
struct BSCvor{
    tip1 kljuc;
    tip2 vrijednost;
    BSCvor *lijevo, *desno,*roditelj;
    BSCvor() : lijevo(nullptr), desno(nullptr), roditelj(nullptr) {}
    BSCvor(const tip1 &k, const tip2 &v, BSCvor* l, BSCvor* d, BSCvor* r) {
        kljuc=k;
        vrijednost=v;
        lijevo=l;
        desno=d;
        roditelj=r;
    }
};

template<typename tip1, typename tip2>
class BinStabloMapa : public Mapa<tip1,tip2> {
    BSCvor<tip1,tip2> *root;
    int br_el;
    public:
    BinStabloMapa() : root(nullptr), br_el(0) {}
    
    int brojElemenata() const { return br_el; }
    ~BinStabloMapa() {
        obrisi();
    }
    
    
    
    void Kopiraj(BSCvor<tip1,tip2> *c1, BSCvor<tip1,tip2> *&c2, BSCvor<tip1,tip2> *c3=nullptr) {
        if(c1!=nullptr) {
            c2=new BSCvor<tip1,tip2>();
            c2->kljuc=c1->kljuc;
            c2->vrijednost=c1->vrijednost;
            c2->roditelj=c3;
            Kopiraj(c1->lijevo,c2->lijevo, c2);
            Kopiraj(c1->desno,c2->desno, c2);
        }
    }
    BinStabloMapa(const BinStabloMapa<tip1,tip2> &b) : br_el(b.br_el) {
        Kopiraj(b.root, root);
    }
    BinStabloMapa(BinStabloMapa<tip1,tip2> &&b) : root(b.root), br_el(b.br_el) {}
    BinStabloMapa &operator=(BinStabloMapa<tip1,tip2> &&b) {
        if(this==&b) return *this;
        this->obrisi();
        root=b.root;
        br_el=b.br_el;
        return *this;
    }
    BinStabloMapa &operator=(const BinStabloMapa<tip1,tip2> &b) {
        if(this==&b) return *this;
        this->obrisi();
        Kopiraj(b.root,root);
        br_el=b.br_el;
        return *this;
    }
    
    
    void obrisi() {
       while (root!=nullptr) {
           obrisi(root->kljuc);
       }
    }
    void obrisi(const tip1 &kljuc) {
        BSCvor<tip1,tip2> *pom(root);
        BSCvor<tip1,tip2> *s=nullptr;
        while (pom!=nullptr && pom->kljuc!=kljuc) {
            s=pom;
            if(pom->kljuc>kljuc) pom=pom->lijevo;
            else pom=pom->desno;
        }
        if(pom==nullptr) return;
        BSCvor<tip1,tip2> *k;
        if(pom->lijevo==nullptr) k=pom->desno;
        else {
            if(pom->desno==nullptr) k=pom->lijevo;
            else {
            BSCvor<tip1,tip2> *pm, *tmp;
            pm=pom;
            k=pom->lijevo;
            tmp=k->desno;
            while (tmp!=nullptr) {
                pm=k;
                k=tmp;
                tmp=k->desno;
            }
            if(pm!=pom) {
                pm->desno=k->lijevo;
                k->lijevo=pom->lijevo;
            }
            k->desno=pom->desno;
            }
        }
        if(s==nullptr) root=k;
        else {
            if(pom==s->lijevo) s->lijevo=k;
            else s->desno=k;
        }
        delete pom;
        br_el--;
    }
    tip2 operator [](const tip1 &kljuc) const {
        BSCvor<tip1,tip2> *p=root;
        while (p!=nullptr && kljuc!=p->kljuc) {
            if(kljuc<p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p==nullptr) return tip2();
        return p->vrijednost;
    }
    tip2 &operator[](const tip1 &kljuc) {
        BSCvor<tip1,tip2> *p=root;
        while (p!=nullptr && kljuc!=p->kljuc) {
            if(kljuc<p->kljuc) p=p->lijevo;
            else p=p->desno;
        }
        if(p==nullptr) {
            p=new BSCvor<tip1,tip2>();
            br_el++;
            p->kljuc=kljuc;
            p->vrijednost=tip2();
            BSCvor<tip1,tip2> *x=root, *y=nullptr;
            while (x!=nullptr) {
                y=x;
                if(p->kljuc<x->kljuc) x=x->lijevo;
                else x=x->desno;
            }
            if(y==nullptr) root=p;
            else {
                if(p->kljuc<y->kljuc) y->lijevo=p;
                else y->desno=p;
                p->roditelj=y;
            }
        }
        return p->vrijednost;
    }

};

void generisi(Mapa<int,int> &m, int vel) {
    srand(time(NULL));
    for(int i(0); i<vel; i++) {
        m[i]=rand();
    }
}

void Test() {
    BinStabloMapa<int,int> b;
    NizMapa<int,int> n;
    clock_t v1=clock();
    generisi(b,4000);
    clock_t v2=clock();
    generisi(n,4000);
    clock_t v3=clock();
    double uk1=(v2-v1)/(CLOCKS_PER_SEC/1000);
    double uk2=(v3-v2)/(CLOCKS_PER_SEC/1000);
    std::cout<<"Binarno stablo "<<uk1/4000 << std::endl << "Niz mapa " << uk2/4000 << std::endl;
    
    clock_t v4=clock();
    //generisi(b,2000);
    for(int i(0); i<4000; i++) b[i];
    clock_t v5=clock();
    for(int i(0); i<4000; i++) n[i];
    clock_t v6=clock();
    double uk3=(v5-v4)/(CLOCKS_PER_SEC/1000);
    double uk4=(v6-v5)/(CLOCKS_PER_SEC/1000);
    std::cout<<"Binarno stablo "<<uk3/4000 << std::endl << "Niz mapa " << uk4/4000 << std::endl;
}
int main() {
    Test();
    return 0;
}
