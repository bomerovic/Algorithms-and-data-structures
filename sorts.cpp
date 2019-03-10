#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cmath>

template<typename tip>
void bubble_sort(tip* niz, int vel)
{
    for(int i(0); i<vel-1; i++)
        for(int j(0); j<vel-i-1; j++)
            if(niz[j]>niz[j+1]) {
                tip temp = niz[j];
                niz[j]=niz[j+1];
                niz[j+1]=temp;
            }
}
template<typename tip>
void selection_sort(tip* niz, int vel)
{
    for(int i(0); i<vel-1; i++) {
        int min=i;
        for(int j(i+1); j<vel; j++)
            if(niz[j]<niz[min]) min=j;
        tip temp = niz[i];
        niz[i]=niz[min];
        niz[min]=temp;
    }
}

template<typename tip>
int pomocna(tip* niz, int pocetak, int kraj)
{
    tip b=niz[kraj];
    int i=pocetak-1;
    for(int j(pocetak); j<=kraj-1; j++) {
        if(niz[j]<=b) {
            i++;
            tip temp = niz[i];
            niz[i]=niz[j];
            niz[j]=temp;
        }
    }
    tip temp = niz[i+1];
    niz[i+1]=niz[kraj];
    niz[kraj]=temp;
    return i+1;
}

template<typename tip>
void quick_sort(tip* niz, int vel, int pocetak=-1, int kraj=-1)
{
    if(pocetak==-1 && kraj==-1) {
        pocetak=0;
        kraj=vel-1;
    }
    if(pocetak<kraj) {
        int p = pomocna(niz,pocetak,kraj);
        quick_sort(niz,vel,pocetak,p-1);
        quick_sort(niz,vel,p+1,kraj);
    }
}
template<typename tip>
void mrdz(tip* niz, int pocetak, int m, int kraj)
{
    tip* pom=new tip[kraj-pocetak+1];
    int i=pocetak, j=m+1;
    int k=0;
    while(i<=m && j<=kraj) {
        if(niz[i]<=niz[j]) pom[k++] = niz[i++];
        else pom[k++] = niz[j++];
    }
    while(i<=m) pom[k++] =niz[i++];
    while(j<=kraj) pom[k++] = niz[j++];
    for(k=0, i=pocetak; i<=kraj; ++i, ++k) niz[i] = pom[k];
    delete [] pom;
}

template<typename tip>
void merge_sort(tip* niz, int vel, int pocetak=-1, int kraj=-1)
{
    if(pocetak==-1 && kraj==-1) {
        pocetak=0;
        kraj=vel-1;
    }
    if(pocetak<kraj) {
        int m = pocetak +(kraj-pocetak)/2;
        merge_sort(niz,vel,pocetak,m);
        merge_sort(niz,vel,m+1,kraj);
        mrdz(niz,pocetak,m,kraj);
    }
}

void generisi(std::string filename, int vel)
{
    std::ofstream dat(filename);
    if(!dat) std::cout << "Greska!";
    else {
        srand(time(0));
        for(int i(0); i<vel; i++) {
            dat << rand() << " ";
        }
    }
}

void ucitaj(std::string filename, int*& niz, int &vel) {
    vel=0;
    std::ifstream dat(filename);
    if(!dat) std::cout << "Greska!";
    else {
        int broj1;
        char znak;
        while (dat>> broj1>>znak) {
            vel++;
        }
        dat.close();
        dat.open(filename);
        niz = new int[vel];
        int i=0;
        while (dat>> broj1>>znak) {
            niz[i] = broj1;
            i++;
        }
    }
}

template<typename tip>
void modifikovani_bubble_sort(tip* niz, int vel)
{
    int p = vel;
    do {
        int i = p-1;
        p=0;
        for(int j(1); j<=i; j++) {
            if(niz[j-1]>niz[j]) {
                tip temp = niz[j-1];
                niz[j-1]=niz[j];
                niz[j]=temp;
                p=j;
            }
        }
        
    }while (p==0);
}

void generisiZaBubble(std::string filename, int vel)
{
    std::ofstream dat(filename);
    if(!dat) std::cout << "Greska!";
    else {
        srand(time(0));
        for(int i(0); i<vel; i++) {
            dat << rand() %10 + 1<< " ";
        }
    }
    dat.close();
    int *niz=nullptr;
    ucitaj(filename,niz,vel);
    modifikovani_bubble_sort(niz,vel);
    for(int i(0); i<vel; i++) std::cout << niz[i] << " ";
    delete []niz;
}
template<typename tip>
void insertionSort(tip* niz, int vel) {
    for(int i(1); i<vel; i++) {
        tip key = niz[i];
        int j = i-1;
        while (j>=0 && niz[j]>key) {
            niz[j+1] = niz[j];
            j-=1;
        }
        niz[j+1] = key;
    }
}
template<typename tip>
void shellSort(tip* niz, int vel) {
    for(int i = vel/2; i>0; i/=2) {
        for(int j=i; j<vel; j+=1) {
            tip temp = niz[j];
            int k;
            for(k=j; k>=i && niz[k-i]>temp; k-=i) niz[k]=niz[k-i];
            niz[k]=temp;
        }
    }
}
template<typename tip>
void countingSort(tip* a, int vel, int* bNiz, int k) {
    int i, c[10]={0};
    for(i=0; a[i]; ++i) ++c[a[i]];
    for(i=1; i<=10; i++) c[i]+=c[i-1];
    for(i=0; a[i]; ++i) {
        bNiz[c[a[i]-1]] = a[i];
        --c[a[i]];
    }
    
}
/*void Testna() {
    int* niz;
    int vel = 0;
    generisi("berina.txt", 10);
    ucitaj("berina.txt", niz, vel);
    std::cout<<"Izaberite koji algoritam zelite koristiti za sortiranje: "<<std::endl;
    std::cout<<"1 - Bubble sort"<<std::endl;
    std::cout<<"2 - Selection sort"<<std::endl;
    std::cout<<"3 - Quick sort"<<std::endl;
    std::cout<<"4 - Merge sort"<<std::endl;
    int broj;
    std::cin>>broj;
    clock_t vrijeme1 = clock();
    if(broj==1) {
        bubble_sort(niz,vel);
    }
    else if(broj==2) {
        selection_sort(niz,vel);
    }
    else if(broj==3) {
        quick_sort(niz,vel);
    }
    else if(broj==4) {
        merge_sort(niz,vel);
    }
    else {
        std::cout<<"Unijeli ste pogresan broj";
    }
    if(broj==1 || broj==2 || broj==3 || broj==4) {
        clock_t vrijeme2 = clock();
        int ukupno = (vrijeme2-vrijeme1) / (CLOCKS_PER_SEC/1000);
        std::cout<<"Vrijeme izvrsavanja funkcije: "<<ukupno<<" ms."<<std::endl;
    }
    bool ispravno(true);
    for(int i(1); i<vel; i++) {
        if(niz[i]<niz[i-1]) ispravno=false;
    }
    if(ispravno) {
        std::cout<<"Niz je ispravno sortiran!"<<std::endl;
        
        std::ofstream dat("Sortiranniz.txt");
        if(!dat) std::cout << "Greska!";
        else {
            for(int i(0); i<vel; i++) {
                dat<<niz[i]<<" ";
            }
        }
    }
    else std::cout<<"Niz nije ispravno sortiran!"<<std::endl;
    
    delete []niz;
}*/



int main()
{
    //Testna();
    return 0;
}
