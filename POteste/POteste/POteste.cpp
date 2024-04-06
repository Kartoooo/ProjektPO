#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Klasa abstrakcyjna Kontakt jest też przykładem polimorfizmu
class Kontakt {
protected:
    string imie;
    string nazwisko;
public:
    Kontakt(string i, string n) : imie(i), nazwisko(n) {}
    //destruktor
    virtual ~Kontakt() {}
    virtual void wypisz() const = 0;
    virtual string zapisz() const = 0;

    // Przeciążenie operatora <<
    friend ostream& operator<<(ostream& os, const Kontakt& kontakt) {
        os << "Imię: " << kontakt.imie << ", Nazwisko: " << kontakt.nazwisko;
        return os;
    }

    // Przeciążenie operatora >>
    friend istream& operator>>(istream& is, Kontakt& kontakt) {
        cout << "Podaj imię: ";
        is >> kontakt.imie;
        cout << "Podaj nazwisko: ";
        is >> kontakt.nazwisko;
        return is;
    }
};

// Klasa Osoba dziedzicząca po klasie Kontakt
class Osoba : public Kontakt {
private:
    string telefon;
public:
    Osoba(string i, string n, string t) : Kontakt(i, n), telefon(t) {}
    void wypisz() const override {
        cout << "Imię: " << imie << ", Nazwisko: " << nazwisko << ", Telefon: " << telefon << endl;
    }
    string zapisz() const override {
        return "BEGIN:VCARD\nVERSION:3.0\nFN:" + imie + " " + nazwisko + "\nTEL:" + telefon + "\nEND:VCARD";
    }

    // Przeciążenie operatora <<
    friend ostream& operator<<(ostream& os, const Osoba& osoba) {
        os << "Imię: " << osoba.imie << ", Nazwisko: " << osoba.nazwisko << ", Telefon: " << osoba.telefon;
        return os;
    }

    // Przeciążenie operatora >>
    friend istream& operator>>(istream& is, Osoba& osoba) {
        is >> osoba.imie;
        is >> osoba.nazwisko;
        is >> osoba.telefon;
        return is;
    }
};

// Klasa ZarzadcaKontaktow, agregacja obiektów typu Kontakt
class ZarzadcaKontaktow {
private:
    //agregacja Kontakt
    vector<Kontakt*> kontakty;
    string nazwaPliku;

    void odczytajZPliku() {
        ifstream plik(nazwaPliku);
        if (plik.is_open()) {
            string linia;
            while (getline(plik, linia)) {
            }
            plik.close();
        }
    }

    void zapiszDoPliku() const {
        ofstream plik(nazwaPliku);
        if (plik.is_open()) {
            for (const auto& kontakt : kontakty) {
                plik << kontakt->zapisz() << endl;
            }
            plik.close();
        }
        else {
            cerr << "Nie można otworzyć pliku do zapisu!" << endl;
        }
    }

public:
    ZarzadcaKontaktow(const string& plik) : nazwaPliku(plik) {
        odczytajZPliku();
    }

    ~ZarzadcaKontaktow() {
        for (auto& kontakt : kontakty) {
            delete kontakt;
        }
    }

    void dodajKontakt(Kontakt* kontakt) {
        kontakty.push_back(kontakt);
        zapiszDoPliku();
    }

    void usunKontakt(const string& imie, const string& nazwisko) {
        for (auto it = kontakty.begin(); it != kontakty.end(); ++it) {
            if ((*it)->zapisz().find("FN:" + imie + " " + nazwisko) != string::npos) {
                delete* it;
                kontakty.erase(it);
                break; // Przerywamy pętlę, gdy usunięto kontakt
            }
        }
        zapiszDoPliku();
    }

    void wypiszWszystkie() const {
        for (const auto& kontakt : kontakty) {
            kontakt->wypisz();
        }
    }
};

int main() {
    ZarzadcaKontaktow zarzadca("kontakty.vcf");

    int wybor;
    do {
        cout << "1. Dodaj kontakt" << endl;
        cout << "2. Wyswietl kontakty" << endl;
        cout << "3. Usun kontakt" << endl;
        cout << "0. Wyjdz" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;

        switch (wybor) {
        case 1: {
            string imie, nazwisko, numer;

            
            cout << "Podaj imie : " << endl;
            cin >> imie;
                cout << "Podaj nazwisko : " << endl;
            cin >> nazwisko;
                cout << "Podaj numer : " << endl;
               cin >>numer;
               Osoba nowyKontakt(imie, nazwisko, numer); // Tworzymy pusty kontakt
            
            zarzadca.dodajKontakt(new Osoba(nowyKontakt)); // Dodajemy kontakt do zarządcy
            break;
        }
        case 2:
            cout << "Wszystkie kontakty:" << endl;
            zarzadca.wypiszWszystkie();
            break;
        case 3: {
            string imie, nazwisko;
            cout << "Podaj imie kontaktu do usuniecia: ";
            cin >> imie;
            cout << "Podaj nazwisko kontaktu do usuniecia: ";
            cin >> nazwisko;
            zarzadca.usunKontakt(imie, nazwisko);
            break;
        }
        case 0:
            cout << "Koniec programu." << endl;
            break;
        default:
            cout << "Niepoprawna opcja. Wybierz jeszcze raz." << endl;
            break;
        }
    } while (wybor != 0);

    return 0;
}
