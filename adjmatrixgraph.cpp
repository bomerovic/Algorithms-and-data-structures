#include <iostream>
#include <vector>
#include <string>
#include <cmath>
bool JesuLiJednaki(double x, double y, double Eps = 1e-10) {
    return std::fabs(x-y)<=Eps*(std::fabs(x)+std::fabs(y));
}
template<typename tip>
class Grana;

template<typename tip>
class Cvor;

template<typename tip>
class GranaIterator;

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
class MatricaGraf : public UsmjereniGraf<tip>
{
    std::vector<std::vector<float>>m;
    std::vector<tip> oznakeCvorova;
    std::vector<std::vector<tip>> oznakeGrana;
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
public:
    MatricaGraf(int n) {
        if(n<0) throw "Error";
        m=std::vector<std::vector<float>>(n,std::vector<float>(n,-1));
        oznakeCvorova=std::vector<tip>(n);
        oznakeGrana=std::vector<std::vector<tip>>(n,std::vector<tip>(n));
    }
    ~MatricaGraf() {}
    int dajBrojCvorova() const {
        return m.size();
    }
    void postaviBrojCvorova(int n) {
        if(n<dajBrojCvorova()) throw "Error";
        m.resize(n);
        for(int i(0); i<m.size(); i++) m[i].resize(n,-1);
        oznakeCvorova.resize(n);
        oznakeGrana.resize(n);
        for(int i(0); i<oznakeGrana.size(); i++)
            oznakeGrana[i].resize(n);
    }
    void dodajGranu(int polazni, int dolazni, float tezina=0) {
        ispravnaGrana(polazni,dolazni);
        m[polazni][dolazni]=tezina;
    }
    void obrisiGranu(int polazni,int dolazni) {
        postojiLiGrana(polazni,dolazni);
        m[polazni][dolazni]=-1;
    }
    void postaviTezinuGrane(int polazni, int dolazni,float tezina=0) {
        postojiLiGrana(polazni,dolazni);
        m[polazni][dolazni]=tezina;
    }
    float dajTezinuGrane(int polazni, int dolazni)const {
        postojiLiGrana(polazni,dolazni);
        return m[polazni][dolazni];
    }
    bool postojiGrana(int polazni, int dolazni)const {
        ispravnaGrana(polazni,dolazni);
        return m[polazni][dolazni]!=-1;
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
        oznakeGrana[polazni][dolazni]=oznaka;
    }
    tip dajOznakuGrane(int polazni, int dolazni)const {
        postojiLiGrana(polazni,dolazni);
        return oznakeGrana[polazni][dolazni];
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

int main()
{
    try {
        UsmjereniGraf<bool> *g = new MatricaGraf<bool>(3);
        g->dodajGranu(0, 1, 2.5);
        g->dodajGranu(1, 0, 1.2);
        g->dodajGranu(1, 2, 0.1);
        g->dodajGranu(0, 0, 3.14);
        for (GranaIterator<bool> iter = g->dajGranePocetak();
                iter != g->dajGraneKraj(); ++iter)
            std::cout << (*iter).dajPolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajDolazniCvor().dajRedniBroj() << " "
                      << (*iter).dajTezinu() << std::endl;
        delete g;
    } catch (const char izuzetak[]) {
        std::cout << izuzetak << std::endl;
    }

    return 0;
}