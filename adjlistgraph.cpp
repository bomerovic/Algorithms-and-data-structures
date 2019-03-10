#include <iostream>
#include <string>
#include <vector>
#include <list>
#include<queue>

template<typename tip>
class Grana;

template<typename tip>
class Cvor;

template<typename tip>
class GranaIterator;

template<typename tip1, typename tip2>
class Mapa
{
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
struct Pair {
    tip1 kljuc;
    tip2 vrijednost;
};

template<typename tip1, typename tip2>
class HashMapaLan : public Mapa<tip1,tip2>
{
    std::vector<std::list<Pair<tip1,tip2>>> m;
    int br_el=0;
    static const int kapacitet=10000;
    unsigned int (*hash)(tip1, unsigned int)=0;
    typename std::list<Pair<tip1,tip2>>::iterator trazi(tip1 kljuc) {
        if(hash==0) throw "Error";
        int indeks=hash(kljuc,m.size());
        auto iter(m[indeks].begin());
        while(iter!=m[indeks].end() && kljuc>iter->kljuc) iter++;
        return iter;
    }
    typename std::list<Pair<tip1,tip2>>::const_iterator trazi(tip1 kljuc) const {
        if(hash==0) throw "Error";
        int indeks=hash(kljuc,m.size());
        auto iter(m[indeks].begin());
        while(iter!=m[indeks].end() && kljuc>iter->kljuc) iter++;
        return iter;
    }
public:
    HashMapaLan() : m(kapacitet, std::list<Pair<tip1,tip2>>()) {}
    int brojElemenata() const {
        return br_el;
    }
    tip2 operator [](const tip1 &a) const {
        auto iter(trazi(a));
        int indeks=hash(a, m.size());
        if(iter==m[indeks].end() || iter->kljuc!=a) return tip2();
        return iter->vrijednost;
    }
    tip2 &operator[](const tip1 &a) {
        auto iter(trazi(a));
        int indeks=hash(a, m.size());
        if(iter==m[indeks].end() || iter->kljuc!=a) {
            Pair<tip1,tip2> temp;
            temp.kljuc=a;
            temp.vrijednost=tip2();
            iter=m[indeks].insert(iter,temp);
            br_el++;
        }
        return iter->vrijednost;
    }
    void obrisi() {
        for(auto &l: m) l.clear();
        br_el=0;
    }
    void obrisi(const tip1 &kljuc) {
        auto iter(trazi(kljuc));
        int indeks=hash(kljuc, m.size());
        if(iter==m[indeks].end() || iter->kljuc!=kljuc) throw "Error";
        auto iter2(iter++);
        m[indeks].erase(iter2,iter);
        br_el--;
    }
    void definisiHashFunkciju(unsigned int (*fun)(tip1, unsigned int)) {
        hash=fun;
    }
};

template<typename tip>
class UsmjereniGraf
{
protected:
    UsmjereniGraf() {}
public:
    UsmjereniGraf(int n) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const=0;
    virtual void postaviBrojCvorova(int n)=0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina=0)=0;
    virtual void obrisiGranu(int polazni,int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni,float tezina=0)=0;
    virtual float dajTezinuGrane(int polazni, int dolazni)const=0;
    virtual bool postojiGrana(int polazni, int dolazni)const=0;
    virtual void postaviOznakuCvora(int cvor, tip oznaka)=0;
    virtual tip dajOznakuCvora(int cvor)const=0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, tip oznaka)=0;
    virtual tip dajOznakuGrane(int polazni, int dolazni)const=0;
    virtual Grana<tip> dajGranu(int polazni, int dolazni)=0;
    virtual Cvor<tip> dajCvor(int cvor)=0;
    GranaIterator<tip> dajGranePocetak() {
        return ++GranaIterator<tip>(this,0,-1);
    }
    GranaIterator<tip> dajGraneKraj() {
        return GranaIterator<tip>(this,dajBrojCvorova(),0);
    }
};

template<typename tip>
class Grana
{
    UsmjereniGraf<tip>* gr;
    int polazni, dolazni;
public:
    Grana(UsmjereniGraf<tip>* graf, int polazni, int dolazni) :gr(graf), polazni(polazni), dolazni(dolazni) {}
    float dajTezinu() const {
        return gr->dajTezinuGrane(polazni,dolazni);
    }
    void postaviTezinu(float tezina) {
        gr->postaviTezinuGrane(polazni,dolazni,tezina);
    }
    tip dajOznaku() const {
        return gr->dajOznakuGrane(polazni,dolazni);
    }
    void postaviOznaku(tip oznaka) {
        gr->postaviOznakuGrane(polazni,dolazni,oznaka);
    }
    Cvor<tip> dajPolazniCvor() const {
        return gr->dajCvor(polazni);
    }
    Cvor<tip> dajDolazniCvor() const {
        return gr->dajCvor(dolazni);
    }
};

template<typename tip>
class Cvor
{
    UsmjereniGraf<tip> *gr;
    int r;
public:
    Cvor(UsmjereniGraf<tip> *graf, int redni) : gr(graf), r(redni) {}
    tip dajOznaku() const {
        return gr->dajOznakuCvora(r);
    }
    void postaviOznaku(tip oznaka) {
        gr->postaviOznakuCvora(r,oznaka);
    }
    int dajRedniBroj() const {
        return r;
    }
};

template <typename tip>
class GranaIterator
{
    UsmjereniGraf<tip>*gr;
    int i,j;
public:
    GranaIterator(UsmjereniGraf<tip>*graf, int i, int j):gr(graf), i(i), j(j) {}
    Grana<tip> operator*() {
        return gr->dajGranu(i,j);
    }
    bool operator==(const GranaIterator &iter) const {
        return (i==iter.i && j==iter.j && gr==iter.gr);
    }
    bool operator!=(const GranaIterator &iter) const {
        return !(iter==*this);
    }
    GranaIterator& operator++() {
        do {
            if(j+1>=gr->dajBrojCvorova()) {
                j=0;
                i++;
            } else j++;
        } while (i<gr->dajBrojCvorova() && !gr->postojiGrana(i,j));
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator<tip> temp(*this);
        ++(*this);
        return temp;
    }
};

template<typename tip>
struct Element {
    int cvor;
    float tezina;
    tip oznaka;
};

template<typename tip>
class ListaGraf : public UsmjereniGraf<tip>
{
    std::vector<std::list<Element<tip>>> l;
    std::vector<tip> oznakeCvorova;
    void ispravanCvor(int redniBroj) const {
        if(redniBroj<0 || redniBroj>=dajBrojCvorova()) throw "Error";
    }
    void ispravnaGrana(int polazni, int dolazni) const {
        ispravanCvor(polazni);
        ispravanCvor(dolazni);
    }
    void postojiLiGrana(int polazni, int dolazni) const {
        ispravnaGrana(polazni,dolazni);
        if(!postojiGrana(polazni,dolazni)) throw "Error";
    }
    typename std::list<Element<tip>>::iterator traziElement(int i, int j) {
        auto it=l[i].begin();
        while (it!=l[i].end()) {
            if(j==it->cvor) return it;
            else if(j<it->cvor) return l[i].end();
            it++;
        }
        return it;
    }
    typename std::list<Element<tip>>::const_iterator traziElement(int i, int j) const {
        auto it=l[i].begin();
        while (it!=l[i].end()) {
            if(j==it->cvor) return it;
            else if(j<it->cvor) return l[i].end();
            it++;
        }
        return it;
    }
public:
    ListaGraf(int n) {
        if(n<0) throw "Error";
        l=std::vector<std::list<Element<tip>>>(n,std::list<Element<tip>>());
        oznakeCvorova=std::vector<tip>(n);
    }
    int dajBrojCvorova() const {
        return l.size();
    }
    void postaviBrojCvorova(int n) {
        if(n<dajBrojCvorova()) throw "Error";
        l.resize(n);
        oznakeCvorova.resize(n);
    }
    void dodajGranu(int polazni, int dolazni, float tezina=0) {
        ispravnaGrana(polazni,dolazni);
        auto it=l[polazni].begin();
        while (it!=l[polazni].end() && dolazni>it->cvor) {
            it++;
        }
        Element<tip> el;
        el.cvor=dolazni;
        el.tezina=tezina;
        el.oznaka=tip();
        l[polazni].insert(it,el);
    }
    void obrisiGranu(int polazni,int dolazni) {
        postojiLiGrana(polazni,dolazni);
        l[polazni].erase(traziElement(polazni,dolazni));
    }
    void postaviTezinuGrane(int polazni, int dolazni,float tezina=0) {
        postojiLiGrana(polazni,dolazni);
        traziElement(polazni,dolazni)->tezina=tezina;
    }
    float dajTezinuGrane(int polazni, int dolazni)const {
        postojiLiGrana(polazni,dolazni);
        return traziElement(polazni,dolazni)->tezina;
    }
    bool postojiGrana(int polazni, int dolazni)const {
        ispravnaGrana(polazni,dolazni);
        return traziElement(polazni,dolazni)!=l[polazni].end();
    }
    void postaviOznakuCvora(int cvor, tip oznaka) {
        ispravanCvor(cvor);
        oznakeCvorova[cvor]=oznaka;
    }
    tip dajOznakuCvora(int cvor)const {
        ispravanCvor(cvor);
        return oznakeCvorova[cvor];
    }
    void postaviOznakuGrane(int polazni, int dolazni, tip oznaka) {
        postojiLiGrana(polazni,dolazni);
        traziElement(polazni,dolazni)->oznaka=oznaka;
    }
    tip dajOznakuGrane(int polazni, int dolazni)const {
        postojiLiGrana(polazni,dolazni);
        return traziElement(polazni,dolazni)->oznaka;
    }
    Grana<tip> dajGranu(int polazni, int dolazni) {
        postojiLiGrana(polazni,dolazni);
        return Grana<tip>(this,polazni,dolazni);
    }
    Cvor<tip> dajCvor(int cvor) {
        ispravanCvor(cvor);
        return Cvor<tip>(this,cvor);
    }
};

template<typename tip>
void dfs(UsmjereniGraf<tip>* graf, std::vector<Cvor<tip>> &dfsob, Cvor<tip> cvor)
{
    cvor.postaviOznaku(1);
    dfsob.push_back(cvor);
    for(GranaIterator<tip> it= graf->dajGranePocetak(); it!=graf->dajGraneKraj(); it++) {
        Cvor<tip> komsija=(*it).dajDolazniCvor();
        if((*it).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() && komsija.dajOznaku()!=1) {
            dfs(graf,dfsob,komsija);
        }
    }
}

template<typename tip>
void bfs(UsmjereniGraf<tip>* graf, std::vector<Cvor<tip>> &bfsob, Cvor<tip> cvor)
{
    cvor.postaviOznaku(1);
    bfsob.push_back(cvor);
    std::queue<Cvor<tip>> red;
    red.push(cvor);
    while (!red.empty()) {
        Cvor<tip> cv=red.front();
        red.pop();
        for(GranaIterator<tip> it= graf->dajGranePocetak(); it!=graf->dajGraneKraj(); it++) {
            Cvor<tip> komsija=(*it).dajDolazniCvor();
            if((*it).dajPolazniCvor().dajRedniBroj() == cv.dajRedniBroj() && komsija.dajOznaku()!=1) {
                komsija.postaviOznaku(1);
                bfsob.push_back(komsija);
                red.push(komsija);
            }
        }
    }
}

int main()
{
    UsmjereniGraf<bool> *g = new ListaGraf<bool>(6);
    g->dodajGranu(0, 1, 2.5);
    g->dodajGranu(1, 2, 1.2);
    g->dodajGranu(1, 3, 0.1);
    g->dodajGranu(2, 4, 3.14);
    g->dodajGranu(2, 3, 2.73);
    g->dodajGranu(3, 5, 1);
    g->dodajGranu(5, 2, -5.0);
    std::vector<Cvor<bool> > dfs_obilazak;
    dfs(g, dfs_obilazak, g->dajCvor(0));
    for (int i = 0; i < dfs_obilazak.size(); i++)
        std::cout << dfs_obilazak[i].dajRedniBroj() << ",";
    delete g;
    return 0;
}
