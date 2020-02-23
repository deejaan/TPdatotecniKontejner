#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <functional>

template <typename TipElemenata>
class DatotecniKontejner {
    std::fstream tok;
    public:
    DatotecniKontejner(const std::string &ime_datoteke);
    void DodajNoviElement(const TipElemenata &element);
    int DajBrojElemenata();
    TipElemenata DajElement(int pozicija);
    void IzmijeniElement(int pozicija, const TipElemenata &element);
    void Sortiraj(std::function<bool(const TipElemenata &, const TipElemenata &)> kriterij = std::less<TipElemenata>());
};

template <typename TipElemenata>
DatotecniKontejner<TipElemenata>::DatotecniKontejner(const std::string &ime_datoteke) {
    tok.open(ime_datoteke, std::ios::app | std::ios::binary); tok.close();
    tok.open(ime_datoteke, std::ios::in | std::ios::out | std::ios::binary);
    if (!tok) throw std::logic_error("Problemi prilikom otvaranja ili kreiranja datoteke");
}

template <typename TipElemenata>
void DatotecniKontejner<TipElemenata>::DodajNoviElement(const TipElemenata &element) {
    tok.seekp(0, std::ios::end);
    tok.write(reinterpret_cast<const char*>(&element), sizeof element);
    tok.flush();
    if (!tok) throw std::logic_error("Problemi prilikom pristupa datoteci");
}

template <typename TipElemenata>
int DatotecniKontejner<TipElemenata>::DajBrojElemenata() {
    tok.seekg(0, std::ios::end);
    return tok.tellg() / sizeof(TipElemenata);
}

template <typename TipElemenata>
TipElemenata DatotecniKontejner<TipElemenata>::DajElement(int pozicija) {
    if (pozicija<0 || pozicija>=DajBrojElemenata()) throw std::range_error("Neispravna pozicija");
    TipElemenata element;
    tok.seekg(pozicija * sizeof(TipElemenata), std::ios::beg);
    tok.read(reinterpret_cast<char*>(&element), sizeof element);
    if (!tok) throw std::logic_error("Problemi prilikom pristupa datoteci");
    return element;
}

template <typename TipElemenata>
void DatotecniKontejner<TipElemenata>::IzmijeniElement(int pozicija, const TipElemenata &element) {
    if (pozicija<0 || pozicija>=DajBrojElemenata()) throw std::range_error("Neispravna pozicija");
    tok.seekp(pozicija * sizeof(TipElemenata), std::ios::beg);
    tok.write(reinterpret_cast<const char*>(&element), sizeof element);
    tok.flush();
    if (!tok) throw std::logic_error("Problemi prilikom pristupa datoteci");
}

template <typename TipElemenata>
void DatotecniKontejner<TipElemenata>::Sortiraj(std::function<bool(const TipElemenata &, const TipElemenata &)> kriterij) {
    for (int i=0; i<DajBrojElemenata(); i++) {
        for (int j=i+1; j<DajBrojElemenata(); j++) {
            TipElemenata element1, element2;
            tok.seekg(i*sizeof(TipElemenata));
            tok.read(reinterpret_cast<char*>(&element1), sizeof element1);
            tok.seekg(j*sizeof(TipElemenata));
            tok.read(reinterpret_cast<char*>(&element2), sizeof element2);
            if (!kriterij(element1, element2)) {
                tok.seekp(i*sizeof(TipElemenata));
                tok.write(reinterpret_cast<const char*>(&element2), sizeof element2);
                tok.seekp(j*sizeof(TipElemenata));
                tok.write(reinterpret_cast<const char*>(&element1), sizeof element1);
                tok.flush();
            }
        }
    }
    if (!tok) throw std::logic_error("Problemi prilikom pristupa datoteci");
}

int main ()
{
	return 0;
}
