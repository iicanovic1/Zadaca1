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
    //Cvor(const Cvor &cvor){ element=cvor.element; sljedeci=cvor.sljedeci;}
};

template <typename T> class DvostrukaLista : public Lista<T> {
private:
    Cvor<T> *pocetakListe = nullptr;
    Cvor<T> *krajListe = nullptr;
    Cvor<T> *tekuci = nullptr;
    Cvor<T> *prije_tekuci = nullptr;
    int lduzina{};
    int dduzina{};
    void Iniciraj();
    void Unisti();
public:
    explicit  DvostrukaLista();
    ~DvostrukaLista()override ;
    int brojElemenata() const override ;
    T &trenutni() override;
    T trenutni()const override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const T &el) override;
    void dodajIza(const T &el) override;
    T &operator[](int broj) override;
    T operator[](int broj) const override;
    DvostrukaLista<T> &operator = (const DvostrukaLista<T> &lista);
    DvostrukaLista<T> &operator = (DvostrukaLista<T> &&lista);
    DvostrukaLista(const DvostrukaLista<T> &lista);
    DvostrukaLista(DvostrukaLista<T> &&lista);
    template<typename T1>
    friend void ispisiDvostruka ( DvostrukaLista<T1> lista);

};



int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
