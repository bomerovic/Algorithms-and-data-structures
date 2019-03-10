#include <iostream>

template <typename tip>
class Lista
{
public:
    Lista() {} // Podrazumijevani konstruktor
    virtual ~Lista() {} // Bazni destruktor
    virtual int brojElemenata()const = 0;
    virtual void obrisi()= 0;
    virtual bool dodajIspred(const tip& el)= 0; // Umetanje elementa
    virtual bool dodajIza(const tip& el)= 0; // Dodavanje na kraj
    virtual void pocetak() = 0; // Pozicioniranje na početak
    virtual void kraj() = 0; // Pozicioniranje na kraj
    virtual bool prethodni() = 0; // Pozicioniranje na prethodni
    virtual bool sljedeci() = 0; // Pozicioniranje na sljedeći
    //virtual void IdiNaPoziciju(int pos) =0; // Pozicioniranje na i-tu poz.
    virtual const tip& trenutni() const = 0; // Vraća tekući el.
    virtual int LDuzina() const=0; // Dužina lijeve particije
    virtual int DDuzina() const=0; // Dužina desne particije
    virtual tip operator [](int indeks)const=0;
    virtual tip &operator [](int indeks)=0;
};

template <typename tip>
class NizLista : public Lista<tip>
{
protected:
    int kapacitet; // Kapacitet liste
    int duzina; // Dužina liste
    int tekuci; // Pozicija tekućeg elementa
    tip* niz; // Niz za elemente liste
public:
    NizLista(int size=10); // Konstruktor

    NizLista(const NizLista &l) : kapacitet(l.kapacitet), duzina(l.duzina), tekuci(l.tekuci) {
        niz = new tip[l.kapacitet];
        for(int i(0); i<l.duzina; i++)
            niz[i] = l.niz[i];
    }

    NizLista(NizLista &&l) : kapacitet(l.kapacitet), duzina(l.duzina), tekuci(l.tekuci), niz(l.niz) {
        l.niz = nullptr;
        l.kapacitet=0;
        l.tekuci=0;
        l.duzina=0;
    }
    NizLista &operator = (const NizLista &l) {
        if(this==&l) return *this;
        delete[] niz;
        kapacitet = l.kapacitet;
        duzina = l.duzina;
        tekuci = l.tekuci;
        niz = new tip[l.kapacitet];
        for(int i(0); i<l.duzina; i++)
            niz[i] = l.niz[i];
        return *this;
    }
    NizLista &operator = (const NizLista &&l) {
        if(this == &l) return *this;
        delete[] niz;
        kapacitet = l.kapacitet;
        duzina = l.duzina;
        tekuci = l.tekuci;
        niz = l.niz;
        l.kapacitet=0;
        l.duzina=0;
        l.tekuci=0;
        l.niz=nullptr;
        return *this;
    }
    ~NizLista() {
        delete [] niz;    // Destruktor
    }
    int brojElemenata() const {
        return duzina;
    }
    void obrisi(); // Brisanje liste
    bool dodajIspred(const tip& x); // Umetanje
    bool dodajIza(const tip& x); // Dodavanje
    void pocetak() {
        tekuci = 0;   // Poziciniranje na početak
    }
    void kraj() {
        tekuci = duzina;   // Pozicioniranje na kraj
    }
    bool prethodni() { // Pozicioniranje na prethodni
        if(tekuci == 0)  return false;
        if (tekuci != 0)
            tekuci--;
        return true;
    }
    bool sljedeci() { // Pozicioniranje na sljedeći
        if(tekuci == duzina-1) return false;
            tekuci++;
        return true;
    }
    int LDuzina() const override {
        return tekuci;   // Dužina lijeve particije
    }
    int DDuzina() const override {
        return duzina - tekuci;   // Dužina desne particije
    }
    const tip& trenutni() const { // Vraća tekući el.
        if (DDuzina() <= 0) throw "Nema nista za vratiti!\n";
        return niz[tekuci];
    }
    void Prikazi() const { // Prikazuje sadržaj liste
        for(int i=0; i < duzina; i++)
            std::cout << "Element " << i << ":" << niz[i] << std::endl;
    }
    tip operator [](int indeks) const {
        return niz[indeks-1];
    }
    tip &operator [](int indeks) {
        return niz[indeks-1];
    }
};

template<typename tip> // Konstruktor
NizLista<tip>::NizLista(int size)
{
    kapacitet = size;
    duzina = tekuci = 0;
    niz = new tip[kapacitet];
    for(int i=0; i<kapacitet; i++) niz[i] = tip();
}
template<typename tip> // Uništavanje
void NizLista<tip>::obrisi()
{
    if (DDuzina() == 0) {
        tekuci = duzina-2;
        duzina--;
    }
    for (int i=tekuci; i<duzina-1; i++)
        niz[i] = niz[i+1];
    duzina--;
}
template<typename tip> // Umetanje
bool NizLista<tip>::dodajIza(const tip& x)
{
    if (duzina<kapacitet) {
        for(int i = duzina; i > tekuci; i--)
            niz[i] = niz[i-1];
            niz[tekuci] = x;
            duzina++;
            return true;
    } else return false;
}
template<typename tip> // Dodavanje na kraj
bool NizLista<tip>::dodajIspred(const tip& x)
{
    if (duzina < kapacitet) {
        niz[duzina++] = x;
        return true;
    } else return false;
}
/*
template <typename tip>
class JednostrukaLista : public Lista<tip>
{
private:
    struct Cvor {
        tip element;
        Cvor *sljedec;
        Cvor(const tip &element, Cvor *sljedec) : element(element), sljedec(sljedec) {}
    };
    int lduzina,dduzina;
    Cvor *poceta; // Pokazivač na početak liste
    Cvor *kr; // Pokazivač na kraj liste
    Cvor *tekuc; // Pokazivač na prethodnika tekućeg el.
    void Unisti(); // Uništavanje liste
    void Iniciraj(); // Inicijalizacija liste
public:
    JednostrukaLista() : poceta(0),kr(0), // Konstruktor
        tekuc(0), lduzina(0),dduzina(0) {}
    JednostrukaLista(const JednostrukaLista &jplista); // Konstruktor kopije
    JednostrukaLista &operator =(const JednostrukaLista &jplista); // Preklopljeni op. dodjele
    ~JednostrukaLista(); // Destruktor
    void obrisi() {
        Unisti();    // Brisnaje liste
        Iniciraj();
    }
    bool dodajIspred(const tip &element); // Umetanje
    bool dodajIza(const tip &element); // Dodavanje
    void pocetak() { // Pozicioniranje na početak
        tekuc = 0;
        dduzina += lduzina;
        lduzina = 0;
    }
    void kraj() { // Pozicioniranje na kraj
        tekuc = kr;
        lduzina += dduzina;
        dduzina = 0;
    }
    void prethodni(); // Pozicioniranje na prethodnika
    void sljedeci(); // Pozicioniranje na sljedbenika
    int LDuzina() const {
        return lduzina;    // Dužina lijeve particije
    }
    int DDuzina() const {
        return dduzina;    // Dužina desne particije
    }

    bool JeLiPrazna() const { // Provjera je li lista prazna
        return lduzina + dduzina == 0;
    }
    int Duzina() const { // Vraća dužinu liste
        return lduzina + dduzina;
    }
    void IdiNaPoziciju(int i);
    const tip& trenutni() const; // Vraća tekući el.
    tip &operator [](int i); // Preklopljeni op. indeksiranja
    tip operator [](int i) const; // Preklopljeni op. indeksirabnja
    JednostrukaLista &operator +=(const JednostrukaLista &jplista); // Preklopljeni operatori
    JednostrukaLista operator +(const JednostrukaLista &jplista) const;
    void Prikazi() const; // Prikaz sadržaja liste
    tip Izbaci();
};

template <typename tip> // Konstruktor kopije
JednostrukaLista<tip>::JednostrukaLista(const JednostrukaLista &jplista) :
    poceta(0), kr(0),dduzina(0),lduzina(0)
{
    Cvor *p(jplista.poceta);
    while (p != 0) {
        dodajIza(p->element);
        p=p->sljedec;
    }
}
template <typename tip> // Destruktor
JednostrukaLista<tip>::~JednostrukaLista()
{
    while(!JeLiPrazna())
        Izbaci();
    poceta = kr = tekuc = 0;
}

template <typename tip> // Preklopljeni op. dodjele
JednostrukaLista<tip> &JednostrukaLista<tip>::operator=(const JednostrukaLista &jplista)
{
    if (&jplista==this) return *this;
    while(!JeLiPrazna())
        Izbaci();
    Cvor *p(jplista.poceta);
    while(p != 0) {
        Dodaj(p->element);
        p=p->sljedec;
    }
    return *this;
}

template<typename tip> // Inicijalizacija liste
void JednostrukaLista<tip>::Iniciraj()
{
    tekuc = kr = poceta = 0;
    lduzina = dduzina = 0;
}
template<typename tip> // Uništavanje liste
void JednostrukaLista<tip>::Unisti()
{
    while(poceta != 0) {
        tekuc = poceta;
        poceta = poceta->sljedec;
        delete tekuc;
    }
    poceta = tekuc = kr= 0;
}

template <typename tip> // Umetanje
bool JednostrukaLista<tip>::dodajIspred(const tip &element)
{
    Cvor* p=new Cvor(element,0);
    if (lduzina == 0) {
        p->sljedec = poceta;
        poceta = p;
        if (lduzina + dduzina == 0)
            kr = p;
    } else {
        p->sljedec = tekuc->sljedec;
        tekuc->sljedec = p;
        if (dduzina == 0)
            kr = tekuc->sljedec;
    }
    dduzina++;
    return true;
}

template <typename tip> // Dodavanje elementa
bool JednostrukaLista<tip>::dodajIza(const tip &element)
{
    Cvor *p = new Cvor(element,0);
    if (lduzina+dduzina == 0)
        poceta = kr = p;
    else
        kr = kr->sljedec = p;
    dduzina++;
    return true;
}

template <typename tip> // Izbacivanje
tip JednostrukaLista<tip>::Izbaci()
{
    if (dduzina <= 0) throw "Nista za izbaciti!\n";
    tip x;
    Cvor* priv;
    if (lduzina == 0) {
        x = poceta->element;
        priv = poceta;
        poceta = priv->sljedec;
    } else {
        priv = tekuc->sljedec;
        x = tekuc->sljedec->element;
        tekuc->sljedec=priv->sljedec;
    }
    if (dduzina == 1)
        kr = tekuc;
    delete priv;
    dduzina--;
    return x;
}

template<typename tip> // Pozicioniranje na prethodnika
void JednostrukaLista<tip>::prethodni()
{
    if(lduzina != 0) {
        if (lduzina == 1)
            tekuc = 0;
        else {
            Cvor* p(poceta);
            while (p->sljedec != tekuc)
                p=p->sljedec;
            tekuc= p;
        }
        lduzina--;
        dduzina++;
    }
}
template<typename tip> // Pozicioniranje na sljedbenika
void JednostrukaLista<tip>::sljedeci()
{
    if (dduzina != 0) {
        if (lduzina == 0)
            tekuc = poceta;
        else
            tekuc = tekuc->sljedec;
        lduzina++;
        dduzina--;
    }
}

template <typename tip> // Vraća tekući element
const tip& JednostrukaLista<tip>::trenutni() const
{
    if (dduzina == 0) throw "Nista za vratiti!\n";
    if (tekuc == 0) return poceta->element;
    else return tekuc->sljedec->element;
}

template <typename tip> // Pozicioniranje na i-tu poz.
void JednostrukaLista<tip>::IdiNaPoziciju(int i)
{
    if ((i<0) || (i>dduzina+lduzina)) throw "Indeks izvan raspona!\n";
    int p(0);
    int ld(lduzina);
    dduzina = dduzina+lduzina-i;
    lduzina = i;
    if (i == 0) {
        tekuc = 0;
        return;
    }
    if (i >= ld) {
        if (ld == 0) tekuc = poceta;
        else p = ld-1;
    } else tekuc = poceta;
    for (int j = p; j <i-1; j++)
        tekuc = tekuc->sljedec;
}


template <typename tip> // Operator pristupa
tip &JednostrukaLista<tip>::operator [](int i)
{
    IdiNaPoziciju(i);
    if (i==0 || tekuc == 0)
        return poceta->element;
    else
        return tekuc->sljedec->element;
}
template <typename tip> // Konstantni operator pristupa
tip JednostrukaLista<tip>::operator [](int i) const
{
    IdiNaPoziciju(i);
    if (i == 0 || tekuc == 0) return poceta->element;
    else return tekuc->sljedec->element;
}

template <typename tip> // Operator dodavanja
JednostrukaLista<tip> &JednostrukaLista<tip>::operator +=(const JednostrukaLista& jplista)
{
    JednostrukaLista l(jplista);
    if (poceta == 0)
        poceta=l.poceta;
    else
        kr->sljedec = l.poceta;
    kr = l.kr;
    dduzina += l.dduzina;
    lduzina += l.lduzina;
    l.poceta = l.kr = l.tekuc=0;
    l.dduzina = l.lduzina = 0;
    return *this;
}

template <typename tip> // Operator spajanja
JednostrukaLista<tip> JednostrukaLista<tip>::operator +(const JednostrukaLista& jplista) const
{
    JednostrukaLista l(*this);
    l += jplista;
    return l;
}
template <typename tip> // Prikaz sadržaja liste
void JednostrukaLista<tip>::Prikazi() const
{
    Cvor *p(poceta);
    int i(0);
    std::cout << "\nSadrzaj liste:\n";
    while (p != 0) {
        std::cout << i << ":" << p->element;
        if (p == poceta)
            std::cout << "<- pocetak";
        if (p == tekuc)
            std::cout << "<- tekuci,lduzina:" << lduzina;
        if (p == kr)
            std::cout << "<- kraj";
        std::cout << std::endl;
        i++;
        p=p->sljedec;
    }
    std::cout << "lduzina:" << lduzina << std::endl;
    std::cout << "dduzina:" << dduzina << std::endl;
}
*/
int main()
{
    Lista<int>* l;
    l = new NizLista<int>;
    for (int i(1); i<=5; i++)
	    l->dodajIza(i);
    do {
	    std::cout << l->trenutni() << " ";
    } while(l->sljedeci());
    delete l;
    return 0;
}
