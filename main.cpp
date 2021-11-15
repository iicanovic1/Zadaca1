#include <iostream>
#include <new>
using std::cout;

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

template <typename Tip>
class Iterator; // Predefinisana klasa iterator

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
    explicit  DvostrukaLista(){ Iniciraj(); }; //testirano
    ~DvostrukaLista()override{ Unisti(); }; //testirano
    int brojElemenata() const override{ return  dduzina+lduzina;}; //testirano
    T &trenutni() override{return  tekuci->sljedeci->element;}; //testirano
    T trenutni()const override{return  tekuci->sljedeci->element; }; //testirano
    bool prethodni() override; //testirano
    bool sljedeci() override; //testirano
    void pocetak() override{ tekuci = pocetakListe; int privremena = this->brojElemenata();
    dduzina = privremena; lduzina = 0;}; //testirano
    void kraj() override{ tekuci = krajListe->prethodni->prethodni;
        int privremena = this->brojElemenata();
        dduzina = 0; lduzina = privremena;}; //testirano
    void obrisi() override; //testirano
    void dodajIspred(const T &el) override; //testirano
    void dodajIza(const T &el) override; //testirano
    T &operator[](int broj) override; //testirano
    T operator[](int broj) const override; //testirano
    DvostrukaLista<T> &operator = (const DvostrukaLista<T> &lista); //testirano
    DvostrukaLista<T> &operator = (DvostrukaLista<T> &&lista) noexcept; //testirano
    DvostrukaLista(const DvostrukaLista<T> &lista); //testirano
    DvostrukaLista(DvostrukaLista<T> &&lista); //testirano
    template<typename T1>
    friend void ispisiDvostruka ( DvostrukaLista<T1> lista); //testirano
    friend class Iterator<T>;



};
template <typename T>
class Iterator {
    Cvor<T> *trenutniLista;
    Cvor<T> *pocetakLista;
    const DvostrukaLista<T> *lista;

public:
    bool slijedeci() {
        if(trenutniLista->sljedeci->sljedeci->sljedeci != nullptr) {
            trenutniLista = trenutniLista->sljedeci;
            return true;
        }
        return false;
    }
    T element() {
        return trenutniLista->sljedeci->element;
    }
    void pocetak() {
        trenutniLista = pocetakLista;
    }

    Iterator(const Lista<T> *dvostrukaLista){
        lista = (const DvostrukaLista<T> *) dvostrukaLista;
        trenutniLista = lista->pocetakListe;
        pocetakLista = lista->pocetakListe;
    }
};

template <typename T>
T dajMaksimum(const Lista<T> &lista)
{
    Iterator<T> iterator = Iterator<T>(&lista);
    T maxEl;
    if(lista.brojElemenata() != 0)
        maxEl = iterator.element();
else throw std::domain_error("Prazna Lista!!!!");
    while (iterator.slijedeci()){
        if(iterator.element() > maxEl)
            maxEl = iterator.element();
    }
    return maxEl;
}

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
    while (privremeni->sljedeci != nullptr){
        std::cout << privremeni->element<<" ";
        privremeni = privremeni->sljedeci;
    }
    std::cout << "\n" << "Tekuci element : " << lista.tekuci->sljedeci->element << "\n";
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
        tekuci = nullptr;
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
    if(tekuci->sljedeci->sljedeci != krajListe){
        tekuci = tekuci->sljedeci;
        dduzina--;
        lduzina++;
        return true;
    }
    return false;
}

template<typename T>
void DvostrukaLista<T>::obrisi() {
    //std::cout << "Element koji se izbacuje : " << tekuci->sljedeci->element;
    if(dduzina == 0)
         throw std::domain_error("Ništa za obrisati!\n");
    Cvor<T>* privremeni  = tekuci->sljedeci;
    privremeni->sljedeci->prethodni = tekuci;
    tekuci->sljedeci = privremeni->sljedeci;
    delete privremeni;
    if(dduzina == 1 && brojElemenata()!= 1) {
        tekuci = tekuci->prethodni;
        lduzina--;
    }else {
        dduzina--;
    }
}

template<typename T>
void DvostrukaLista<T>::dodajIspred(const T &el) {
    Cvor<T>* novi = new Cvor<T>(el, tekuci, tekuci->sljedeci );
    tekuci->sljedeci = novi;
    novi->sljedeci->prethodni = novi;
    if(brojElemenata() == 0 ) dduzina++;
    else lduzina++;
    if(lduzina > 0)tekuci = tekuci->sljedeci;
}

template<typename T>
void DvostrukaLista<T>::dodajIza(const T &el) {
    if(brojElemenata() == 0) dodajIspred(el);
    else {
        Cvor<T> *pomocniTekuci = tekuci->sljedeci;
        Cvor<T> *novi = new Cvor<T>(el, pomocniTekuci, pomocniTekuci->sljedeci);
        pomocniTekuci->sljedeci = novi;
        novi->sljedeci->prethodni = novi;
        dduzina++;
    }
}

template<typename T>
T &DvostrukaLista<T>::operator[](int broj) {
    if( broj < 0 || broj >= dduzina+lduzina)
        throw std::domain_error("Neprihvatljiv broj!");
    int  k = 0;
    if(broj < lduzina)
        k = lduzina-broj;
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
    if( broj < 0 || broj >= dduzina+lduzina)
        throw std::domain_error("Neprihvatljiv broj!");
    int  k = 0;
    if(broj < lduzina)
        k = lduzina-broj;
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
    if(this->pocetakListe == lista.pocetakListe &&
            this->krajListe == lista.krajListe && this->tekuci == lista.tekuci
                && this->brojElemenata() == lista.brojElemenata()) return *this;
    this->Unisti();
    pocetakListe=new Cvor<T>(*lista.pocetakListe);
    if(pocetakListe->sljedeci == krajListe) return *this;
    Cvor<T> *tempNova=pocetakListe;
    Cvor<T> *tempStara=lista.pocetakListe;
    while(tempStara->sljedeci != nullptr) {
        tempNova->sljedeci = new Cvor <T>(*tempStara->sljedeci);
        tempNova=tempNova->sljedeci;
        tempStara=tempStara->sljedeci;
    }
    tekuci = lista.tekuci;
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
    if(pocetakListe->sljedeci == krajListe) return;
    Cvor<T> *tempNova=pocetakListe;
    Cvor<T> *tempStara=lista.pocetakListe;
    while(tempStara->sljedeci != nullptr) {
        tempNova->sljedeci = new Cvor <T>(*tempStara->sljedeci);
        tempNova=tempNova->sljedeci;
        tempStara=tempStara->sljedeci;
    }
    tekuci = lista.tekuci;
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

//testtne FUNKCIJE

template <typename T>
bool testKonstruktoraDvostruka (){
    DvostrukaLista<int> testnaLista ;
    //ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testDodajIspredDvostruka(){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.dodajIspred(2);
    testnaLista.dodajIspred(3);
    testnaLista.dodajIspred(4);
    testnaLista.dodajIspred(5);
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testDodajIzaDvostruka (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testObrisi (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIspred(1);
    testnaLista.obrisi();
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testPocetakKraj (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    testnaLista.pocetak();
    ispisiDvostruka<int>(testnaLista);
    testnaLista.kraj();
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testTrenutni (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    testnaLista.pocetak();
    testnaLista.trenutni() = 100;
    ispisiDvostruka<int>(testnaLista);
    testnaLista.kraj();
    testnaLista.trenutni() = 100;
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testPrethodniSlijedeci (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    testnaLista.pocetak();
    testnaLista.sljedeci();
    testnaLista.trenutni() = 100;
    ispisiDvostruka<int>(testnaLista);
    testnaLista.kraj();
    testnaLista.prethodni();
    testnaLista.trenutni() = 100;
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testOperatorUglaste (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    testnaLista[4] = 100;
    ispisiDvostruka<int>(testnaLista);
    return true;
}

template<typename T>
bool testDajMaximum (){
    DvostrukaLista<int> testnaLista ;
    testnaLista.dodajIza(1);
    testnaLista.dodajIza(2);
    testnaLista.dodajIza(3);
    testnaLista.dodajIza(4);
    testnaLista.dodajIza(5);
    std::cout <<dajMaksimum(testnaLista)<<" ";
    return true;
}

int main() {
    testKonstruktoraDvostruka<int>();
    testDodajIspredDvostruka<int>();
    testDodajIzaDvostruka<int>();
    testObrisi<int>();
    testPocetakKraj<int>();
    testTrenutni<int>();
    testPrethodniSlijedeci<int>();
    testOperatorUglaste<int>();
    testDajMaximum<int>();
    return 0;
}
