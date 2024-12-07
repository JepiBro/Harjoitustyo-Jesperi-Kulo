// Jesperi Kulo
// Lopputyo

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// Arvot
const int MIN_HUONEET = 40;
const int MAX_HUONEET = 300;
const int YKSI_HH_HINTA = 100;
const int KAKSI_HH_HINTA = 150;

struct Huone 
{
    int numero;
    bool varattu = false;
    string varaajanNimi;
    int varausNumero;
};

int Luku(int min, int max);
void tulostaMenu();
int varaaHuone(vector<Huone>& huoneet, int huonetyyppi, bool arvonta);
bool tarkistaSaatavuus(const vector<Huone>& huoneet, int huonetyyppi);
void haeVaraus(const vector<Huone>& huoneet);
double Alennus(double summa);
int varatutHuoneet = 0;

// Paaohjelma
int main() 
{
    srand(time(0));
    int huonemaara = Luku(MIN_HUONEET, MAX_HUONEET);
    vector<Huone> huoneet(huonemaara);

    for (int i = 0; i < huonemaara; ++i) 
    {
        huoneet[i].numero = i + 1;
    }

    bool jatka = true;
    while (jatka) 
    {
        tulostaMenu();
        int valinta;
        cout << "Valintasi: ";
        cin >> valinta;

        switch (valinta)
        {
            case 1: 
            {
                cout << "Valitse huonetyyppi\n1 = Yhden hengen\n2 = Kahden hengen\nValintasi: ";
                int huonetyyppi;
                cin >> huonetyyppi;

                if (huonetyyppi != 1 && huonetyyppi != 2) 
                {
                    cout << "Virheellinen syote." << endl;
                    break;
                }

                if (!tarkistaSaatavuus(huoneet, huonetyyppi)) 
                {
                    cout << "Valitsemasi huonetyyppi on loppuun varattu." << endl;
                    break;
                }

                cout << "Haluatko valita huoneen itse vai arvotaanko se?\n1 = Valitse itse\n2 = Arvonta\nValintasi: ";
                int valintaTapa;
                cin >> valintaTapa;

                if (valintaTapa != 1)
                {
                    if ( valintaTapa != 2)
                    {
                    cout << "Virheellinen syote." << endl;
                    break;
                    }
                }

                bool arvonta = (valintaTapa == 2);
                int varausTila = varaaHuone(huoneet, huonetyyppi, arvonta);

                if (varausTila != -1)
                    cout << "Varausnumero: " << varausTila << endl;
                break;
            }
            case 2:
                haeVaraus(huoneet);
                break;
            case 3:
                for (const auto& huone : huoneet) 
                {
                    if (huone.varattu) 
                    {
                        varatutHuoneet++;
                    }
                }
                cout << "Hotellissa on " << huonemaara << " huonetta, joista " << varatutHuoneet << " on varattu." << endl;
                cout << "1 hengen huoneet: 1 - " << huonemaara / 2 << endl;
                cout << "2 hengen huoneet: " << huonemaara / 2 + 1 << " - " << huonemaara << endl;
                break;
            case 4:
                jatka = false;
                cout << "Ohjelma suljetaan. Kiitos kaynnista!" << endl;
                break;
            default:
                cout << "Virheellinen syote." << endl;
        }
    }

    return 0;
}

// Aliohjelmat
int Luku(int min, int max) 
{
    int i = rand() % (max - min + 1) + min;
    if (i % 2 != 0) 
    {
        i++;
    }
    return i;
}

void tulostaMenu() 
{
    cout << "\n-- Eskon hotelli --\n";
    cout << "1. Tee uusi varaus\n";
    cout << "2. Hae varaus\n";
    cout << "3. Huone info\n";
    cout << "4. Lopeta\n";
}

bool tarkistaSaatavuus(const vector<Huone>& huoneet, int huonetyyppi) 
{
    int alku = (huonetyyppi == 1) ? 0 : huoneet.size() / 2;
    int loppu = (huonetyyppi == 1) ? huoneet.size() / 2 : huoneet.size();

    for (int i = alku; i < loppu; ++i) 
    {
        if (!huoneet[i].varattu) 
        {
            return true;
        }
    }
    return false;
}

int varaaHuone(vector<Huone>& huoneet, int huonetyyppi, bool arvonta) 
{
    int alku = (huonetyyppi == 1) ? 0 : huoneet.size() / 2;
    int loppu = (huonetyyppi == 1) ? huoneet.size() / 2 : huoneet.size();

    int valittuHuone = -1;
    if (arvonta) 
    {
        vector<int> vapaatHuoneet;
        for (int i = alku; i < loppu; ++i) 
        {
            if (!huoneet[i].varattu) {
                vapaatHuoneet.push_back(i);
            }
        }

        if (!vapaatHuoneet.empty()) 
        {
            valittuHuone = vapaatHuoneet[Luku(0, vapaatHuoneet.size() - 1)];
        }
    } else 
    {
        cout << "Valitse huonenumero (" << alku + 1 << " - " << loppu << "): ";
        int huonenumero;
        cin >> huonenumero;

        if (huonenumero < alku + 1 || huonenumero > loppu || huoneet[huonenumero - 1].varattu) 
        {
            cout << "Virheellinen tai varattu huone. Yrita uudelleen." << endl;
            return -1;
        }
        valittuHuone = huonenumero - 1;
    }

    if (valittuHuone == -1) 
    {
        cout << "Ei vapaita huoneita jaljella." << endl;
        return -1;
    }

    Huone& huone = huoneet[valittuHuone];
    cout << "Syota varaajan nimi: ";
    cin.ignore();
    getline(cin, huone.varaajanNimi);
    huone.varausNumero = Luku(10000, 99999);
    huone.varattu = true;

    int hinta = (huonetyyppi == 1) ? YKSI_HH_HINTA : KAKSI_HH_HINTA;

    int yot;

    for(int i = 0; i < 1;)
    {
    cout << "Syota oiden maara (max 14): ";
    cin >> yot;

    if (yot < 0 || yot > 14) 
    {
        cout << "Virheellinen syote." << endl;
    }
    else
    {
        i++;
    }
    }

    cout << "\nKuitti:\n"; 

    double summa = hinta * yot;
    summa = Alennus(summa);

    cout << "Loppusumma: " << fixed << setprecision(2) << summa << " Euroa" << endl;

    if (arvonta) 
    {
        cout << "Huone numero " << huone.numero << endl;
    }

    return huone.varausNumero;
}

void haeVaraus(const vector<Huone>& huoneet) 
{
    cout << "Etsi varaus\n1 = Varausnumerolla\n2 = Varaajan nimella\nValintasi: ";
    int tyyppi;
    cin >> tyyppi;

    if (tyyppi == 1) 
    {
        cout << "Syota varausnumero: ";
        int numero;
        cin >> numero;
        for (const auto& huone : huoneet) 
        {
            if (huone.varausNumero == numero) 
            {
                cout << "Varaus loytyi: Varaaja: " << huone.varaajanNimi << ", Huone: " << huone.numero << endl;
                return;
            }
        }
        cout << "Varausta ei loytynyt." << endl;
    } 
    else if (tyyppi == 2) 
    {
        cout << "Syota varaajan nimi: ";
        cin.ignore();
        string nimi;
        getline(cin, nimi);

        vector<Huone> loydetytVaraukset;

        for (const auto& huone : huoneet) 
        {
            if (huone.varaajanNimi == nimi) 
            {
                loydetytVaraukset.push_back(huone);
            }
        }

        if (!loydetytVaraukset.empty()) 
        {
            cout << "Loytyi " << loydetytVaraukset.size() << " varausta:"<< endl;
            for (const auto& huone : loydetytVaraukset) {
                cout << "  - Varausnumero: " << huone.varausNumero 
                     << ", Huone: " << huone.numero << endl;
            }
        } 
        else 
        {
            cout << "Varausta ei loytynyt." << endl;
        }
    } 
    else 
    {
        cout << "Virheellinen syote." << endl;
    }
}

double Alennus(double summa) 
{
    int alennusProsentti = Luku(0, 2) * 10;
    cout << "Alennus: " << alennusProsentti << "%" << endl;
    return summa * (1 - alennusProsentti / 100.0);
}