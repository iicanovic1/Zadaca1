#include <iostream>
#include <new>

template <typename T> class Lista {
private:
    void operator=(const Lista &);
    Lista(const Lista &);

public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual T trenutni() const = 0;
    virtual T &trenutni()  = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const T &el) = 0;
    virtual void dodajIza(const T &el) = 0;
    virtual T &operator[](int broj) = 0;
    virtual T operator[](int broj) const = 0;
};

template <typename T>
struct Cvor{
    T element;
    Cvor *sljedeci;
    Cvor *prethodni;
    Cvor (const T &element1, Cvor *prethodni1 ,Cvor *sljedeci1 = nullptr):
        element(element1),sljedeci(sljedeci1),prethodni(prethodni1){
        if(prethodni != nullptr) prethodni->sljedeci = this;
        if(sljedeci != nullptr) sljedeci->sljedeci = this;
    }
    Cvor(Cvor *prethodni1 = nullptr, Cvor *sljedeci1 = nullptr) :
    sljedeci(sljedeci1),prethodni(prethodni1){
        if(prethodni != nullptr) prethodni->sljedeci = this;
        if(sljedeci != nullptr) sljedeci->sljedeci = this;
    }
    Cvor(const Cvor &cvor){
        element=cvor.element;
        sljedeci=cvor.sljedeci;
        prethodni = cvor.prethodni;
    }
};

template <typename T> class DvostrukaLista : public Lista<T> {
private:
    Cvor<T> *pocetakListe = nullptr;
    Cvor<T> *krajListe = nullptr;
    Cvor<T> *tekuci = nullptr;
    int lduzina{};
    int dduzina{};
    void Iniciraj();
    void Unisti();
public:
    explicit  DvostrukaLista(){ Iniciraj(); };
    ~DvostrukaLista()override{ Unisti(); };
    int brojElemenata() const override{ return  dduzina+lduzina;};
    T &trenutni() override{return  tekuci->sljedeci->element;};
    T trenutni()const override{return  tekuci->sljedeci->element; };
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override{ tekuci = pocetakListe;};
    void kraj() override{ tekuci = krajListe; };
    void obrisi() override;
    void dodajIspred(const T &el) override;
    void dodajIza(const T &el) override;
    T &operator[](int broj) override;
    T operator[](int broj) const override;
    DvostrukaLista<T> &operator = (const DvostrukaLista<T> &lista);
    DvostrukaLista<T> &operator = (DvostrukaLista<T> &&lista) noexcept;
    DvostrukaLista(const DvostrukaLista<T> &lista);
    DvostrukaLista(DvostrukaLista<T> &&lista);
    template<typename T1>
    friend void ispisiDvostruka ( DvostrukaLista<T1> lista);

};

template<typename T>
void ispisiDvostruka(DvostrukaLista<T> lista) {
    std::cout << "Podaci liste ispod " << std::endl <<
              "duzina : " << lista.brojElemenata() <<std::endl ;
    if(lista.brojElemenata() == 0) {
        std::cout << "Prazna lista nista za prikazati" << std::endl;
        throw std::domain_error("Prazna lista nista za prikazati");
    }
    std::cout <<  "Elementi :" << std::endl;
    Cvor<T> * privremeni = lista.pocetakListe->sljedeci;
    while (privremeni != nullptr){
        std::cout << privremeni->element<<" ";
        privremeni = privremeni->sljedeci;
    }
}

template<typename T>
void DvostrukaLista<T>::Iniciraj() {
    tekuci = pocetakListe = new Cvor<T>;
    pocetakListe->sljedeci = krajListe = new Cvor<T>(pocetakListe, nullptr);
    lduzina = dduzina = 0;
}

template<typename T>
void DvostrukaLista<T>::Unisti() {
    while (pocetakListe != nullptr){
        tekuci = pocetakListe;
        pocetakListe = pocetakListe->sljedeci;
        delete tekuci;
    }
    krajListe = tekuci = nullptr;
}

template<typename T>
bool DvostrukaLista<T>::prethodni() {
    if(tekuci != pocetakListe){
        tekuci = tekuci->prethodni;
        lduzina--;
        dduzina++;
        return true;
    }
    return false;
}

template<typename T>
bool DvostrukaLista<T>::sljedeci() {
    if(tekuci != krajListe){
        tekuci = tekuci->sljedeci;
        dduzina--;
        lduzina++;
        return true;
    }
    return false;
}

template<typename T>
void DvostrukaLista<T>::obrisi() {
    if(dduzina == 0)
        throw "Ništa za obrisati!\n";
    std::cout << "Element koji se izbacuje : " << tekuci->sljedeci->element;
    Cvor<T>* privremeni  = tekuci->sljedeci;
    privremeni->sljedeci->prethodni = tekuci;
    tekuci->sljedeci = privremeni->sljedeci;
    delete privremeni;
    dduzina--;
}

template<typename T>
void DvostrukaLista<T>::dodajIspred(const T &el) {
    Cvor<T>* novi = new Cvor<T>(el, tekuci, tekuci->sljedeci );
    tekuci->sljedeci = novi;
    novi->sljedeci->prethodni = novi;
    lduzina++;
    tekuci = tekuci->sljedeci;
}

template<typename T>
void DvostrukaLista<T>::dodajIza(const T &el) {
    Cvor<T> * pomocniTekuci = tekuci->sljedeci;
    Cvor<T>* novi = new Cvor<T>(el, pomocniTekuci, pomocniTekuci->sljedeci );
    pomocniTekuci->sljedeci = novi;
    novi->sljedeci->prethodni = novi;
    dduzina++;
}

template<typename T>
T &DvostrukaLista<T>::operator[](int broj) {
    if( broj < 0 || broj < dduzina+lduzina)
        throw "Neprihvatljiv broj!";
    int  k = 0;
    if(broj < lduzina)
        k = lduzina-broj+1;
    else if(broj == lduzina)
        return tekuci->sljedeci->element;
    else {
        k =  lduzina - broj;
    }
    Cvor<T> * p = tekuci->sljedeci;
    if(k < 0){
        for (int i = 0; i<abs(k); i++)
            p = p->sljedeci;
    }else{
        for (int i = 0; i<abs(k); i++)
            p = p->prethodni;
    }
     return  p->element;
}

template<typename T>
T DvostrukaLista<T>::operator[](int broj) const {
    if( broj < 0 || broj < dduzina+lduzina)
        throw "Neprihvatljiv broj!";
    int  k = 0;
    if(broj < lduzina)
        k = lduzina-broj+1;
    else if(broj == lduzina)
        return tekuci->sljedeci->element;
    else {
        k =  lduzina - broj;
    }
    Cvor<T> * p = tekuci->sljedeci;
    if(k < 0){
        for (int i = 0; i<abs(k); i++)
            p = p->sljedeci;
    }else{
        for (int i = 0; i<abs(k); i++)
            p = p->prethodni;
    }
    return  p->element;
}

template<typename T>
DvostrukaLista<T> &DvostrukaLista<T>::operator=(const DvostrukaLista<T> &lista) {
    this->Unisti();
    pocetakListe=new Cvor<T>(*lista.pocetakListe);
    krajListe =new Cvor<T>(*lista.krajListe);
    if(pocetakListe->sljedeci == krajListe) return *this;
    Cvor<T> *tempNova=pocetakListe;
    Cvor<T> *tempStara=lista.pocetakListe;
    while(tempStara->sljedeci != nullptr) {
        tempNova->sljedeci = new Cvor <T>(*tempStara->sljedeci);
        tempNova=tempNova->sljedeci;
        tempStara=tempStara->sljedeci;
    }
    dduzina=lista.dduzina;
    lduzina = lista.lduzina;
    return *this;
}

template<typename T>
DvostrukaLista<T> &DvostrukaLista<T>::operator=(DvostrukaLista<T> &&lista)  noexcept {
    if(&lista==this) return *this;
    this->Unisti();
    pocetakListe = lista.pocetakListe;
    tekuci = lista.tekuci;
    krajListe = lista.krajListe;
    dduzina = lista.dduzina;
    lduzina = lista.lduzina;
    pocetakListe = nullptr;
    krajListe= nullptr;
    tekuci = nullptr;
    return *this;
}

template<typename T>
DvostrukaLista<T>::DvostrukaLista(const DvostrukaLista<T> &lista) {
    pocetakListe=new Cvor<T>(*lista.pocetakListe);
    krajListe =new Cvor<T>(*lista.krajListe);
    if(pocetakListe->sljedeci == krajListe) return;
    Cvor<T> *tempNova=pocetakListe;
    Cvor<T> *tempStara=lista.pocetakListe;
    while(tempStara->sljedeci != nullptr) {
        tempNova->sljedeci = new Cvor <T>(*tempStara->sljedeci);
        tempNova=tempNova->sljedeci;
        tempStara=tempStara->sljedeci;
    }
    dduzina=lista.dduzina;
    lduzina = lista.lduzina;
}

template<typename T>
DvostrukaLista<T>::DvostrukaLista(DvostrukaLista<T> &&lista) {
    pocetakListe = lista.pocetakListe;
    tekuci = lista.tekuci;
    krajListe = lista.krajListe;
    dduzina = lista.dduzina;
    lduzina = lista.lduzina;
    pocetakListe = nullptr;
    krajListe= nullptr;
    tekuci = nullptr;
}


int main() {
    DvostrukaLista<int> test = DvostrukaLista<int>();
    ispisiDvostruka(test);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
