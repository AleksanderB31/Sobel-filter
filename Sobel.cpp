#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct fileheader {
    unsigned short int sygnatura_pliku;
    unsigned int dlugosc_pliku;
    unsigned short int pole_zarezerwowane1;
    unsigned short int pole_zarezerwowane2;
    unsigned int pozycja_danych_obrazowych;
};
struct imageheader {
    unsigned int rozmiar_naglowka;
    unsigned  int szerokosc_obrazu;
    unsigned int wysokosc_obrazu;
    unsigned short int liczba_platow;
    unsigned short int liczba_bitow_na_pixel;
    unsigned  int algorytm_kompresji;
    unsigned  int rozmiar_rysunku;
    unsigned int rozdzielczosc_pozioma;
    unsigned int rozdzielczosc_pionowa;
    unsigned  int liczba_kolorow_w_palecie;
    unsigned  int liczba_waznych_kolorow_w_palecie;
};
struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
struct bitmapa {
    imageheader ih;
    fileheader fh;
    pixel** piksele;
    int padding;
    pixel** nowa;
};
int odczytajFH(ifstream& ifs, bitmapa& bmp) {
    ifs.read(reinterpret_cast<char*>(&bmp.fh.sygnatura_pliku), 2);
    ifs.read(reinterpret_cast<char*>(&bmp.fh.dlugosc_pliku), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.fh.pole_zarezerwowane1), 2);
    ifs.read(reinterpret_cast<char*>(&bmp.fh.pole_zarezerwowane2), 2);
    ifs.read(reinterpret_cast<char*>(&bmp.fh.pozycja_danych_obrazowych), 4);

    return ifs.tellg();
}
int odczytajIH(ifstream& ifs, bitmapa& bmp) {
    ifs.read(reinterpret_cast<char*>(&bmp.ih.rozmiar_naglowka), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.szerokosc_obrazu), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.wysokosc_obrazu), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.liczba_platow), 2);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.liczba_bitow_na_pixel), 2);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.algorytm_kompresji), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.rozmiar_rysunku), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.rozdzielczosc_pozioma), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.rozdzielczosc_pionowa), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.liczba_kolorow_w_palecie), 4);
    ifs.read(reinterpret_cast<char*>(&bmp.ih.liczba_waznych_kolorow_w_palecie), 4);
    bmp.padding = (4 - ((bmp.ih.szerokosc_obrazu * 3) % 4)) % 4;
    return ifs.tellg();
}
int zapiszFH(ofstream& ofs, bitmapa& bmp) {
    ofs.write(reinterpret_cast<char*>(&bmp.fh.sygnatura_pliku), 2);
    ofs.write(reinterpret_cast<char*>(&bmp.fh.dlugosc_pliku), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.fh.pole_zarezerwowane1), 2);
    ofs.write(reinterpret_cast<char*>(&bmp.fh.pole_zarezerwowane2), 2);
    ofs.write(reinterpret_cast<char*>(&bmp.fh.pozycja_danych_obrazowych), 4);
    return ofs.tellp();
}
int zapiszIH(ofstream& ofs, bitmapa& bmp) {

    ofs.write(reinterpret_cast<char*>(&bmp.ih.rozmiar_naglowka), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.szerokosc_obrazu), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.wysokosc_obrazu), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.liczba_platow), 2);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.liczba_bitow_na_pixel), 2);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.algorytm_kompresji), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.rozmiar_rysunku), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.rozdzielczosc_pozioma), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.rozdzielczosc_pionowa), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.liczba_kolorow_w_palecie), 4);
    ofs.write(reinterpret_cast<char*>(&bmp.ih.liczba_waznych_kolorow_w_palecie), 4);
    return ofs.tellp();
}
void zapisz_bitmape(ofstream& ofs, bitmapa& bmp) {
    for (int i = 0; i < bmp.ih.wysokosc_obrazu; i++) {
        for (int j = 0; j < bmp.ih.szerokosc_obrazu; j++) {
            ofs.write(reinterpret_cast<char*>(&bmp.nowa[i][j].b), 1);
            ofs.write(reinterpret_cast<char*>(&bmp.nowa[i][j].g), 1);
            ofs.write(reinterpret_cast<char*>(&bmp.nowa[i][j].r), 1);
        }
        if (bmp.padding != 0) {
            char zero = 0;
            for (int i = 0; i < bmp.padding; i++)
                ofs.write((&zero), 1);
        }
    }
}
void wyswietlanie_paramatrow(bitmapa& bmp) {
    cout << "Informacje fh" << endl;
    cout << "Sygnatura pliku:" << endl << bmp.fh.sygnatura_pliku << endl;
    cout << "Dlugosc pliku:" << endl << bmp.fh.dlugosc_pliku << endl;
    cout << "Pole zarezerwowane 1:" << endl << bmp.fh.pole_zarezerwowane1 << endl;
    cout << "Pole zarezerwowane 2:" << endl << bmp.fh.pole_zarezerwowane2 << endl;
    cout << "Pozycja danych obrazowych:" << endl << bmp.fh.pozycja_danych_obrazowych << endl << endl << endl;
    cout << "Informacje ih" << endl;
    cout << "Rozmiar naglowka:" << endl << bmp.ih.rozmiar_naglowka << endl;
    cout << "Szerokosc obrazu:" << endl << bmp.ih.szerokosc_obrazu << endl;
    cout << "Wysokosc obrazu:" << endl << bmp.ih.wysokosc_obrazu << endl;
    cout << "Liczba platow:" << endl << bmp.ih.liczba_platow << endl;
    cout << "Liczba bitow na pixel:" << endl << bmp.ih.liczba_bitow_na_pixel << endl;
    cout << "Algorytm Kompresji:" << endl << bmp.ih.algorytm_kompresji << endl;
    cout << "Rozmiar rysunku:" << endl << bmp.ih.rozmiar_rysunku << endl;
    cout << "Rozdzielczosc pozioma:" << endl << bmp.ih.rozdzielczosc_pozioma << endl;
    cout << "Rozdzielczosc pionowa:" << endl << bmp.ih.rozdzielczosc_pionowa << endl;
    cout << "Liczba kolorow w palecie:" << endl << bmp.ih.liczba_kolorow_w_palecie << endl;
    cout << "Liczba waznych kolorow w palecie:" << endl << bmp.ih.liczba_waznych_kolorow_w_palecie << endl;
}
void tworzenie_pierwszej_tablicy(bitmapa& bmp) {
    bmp.piksele = new pixel * [bmp.ih.wysokosc_obrazu];
    for (int i = 0; i < bmp.ih.wysokosc_obrazu; i++)
        bmp.piksele[i] = new pixel[bmp.ih.szerokosc_obrazu];
}
void wypelnianie_tablicy(bitmapa& bmp, ifstream& ifs) {
    for (int i = 0; i < bmp.ih.wysokosc_obrazu; i++) {
        for (int j = 0; j < bmp.ih.szerokosc_obrazu; j++) {
            ifs.read(reinterpret_cast<char*>(&bmp.piksele[i][j].b), 1);
            ifs.read(reinterpret_cast<char*>(&bmp.piksele[i][j].g), 1);
            ifs.read(reinterpret_cast<char*>(&bmp.piksele[i][j].r), 1);
        }
        if (bmp.padding != 0) {
            for (int k = 0; k < bmp.padding; k++) {
                char zero;
                ifs.read((&zero), 1);
                if (zero != 0) {
                    cout << "error";
                    exit(0);
                }
            }
        }
    }
}
void tworzenie_drugiej_tablicy(bitmapa& bmp) {
    bmp.nowa = new pixel * [bmp.ih.wysokosc_obrazu];
    for (int i = 0; i < bmp.ih.wysokosc_obrazu; i++)
        bmp.nowa[i] = new pixel[bmp.ih.szerokosc_obrazu];
}
void greyscale(bitmapa& bmp) {
    for (int i = 0; i < bmp.ih.wysokosc_obrazu; i++) {
        for (int j = 0; j < bmp.ih.szerokosc_obrazu; j++) {
            int grey = 0;
            grey = ((int)bmp.piksele[i][j].r * 0.3) + ((int)bmp.piksele[i][j].g * 0.59) + ((int)bmp.piksele[i][j].b * 0.11);
            reinterpret_cast<char*>(grey);
            bmp.piksele[i][j].g = grey;
            bmp.piksele[i][j].r = grey;
            bmp.piksele[i][j].b = grey;
        }
    }
}
void operator_sobela(bitmapa& bmp) {

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int h = 0;

    int ar = 0;
    int br = 0;
    int cr = 0;
    int dr = 0;
    int er = 0;
    int fr = 0;
    int gr = 0;
    int hr = 0;

    int ab = 0;
    int bb = 0;
    int cb = 0;
    int db = 0;
    int eb = 0;
    int fb = 0;
    int gb = 0;
    int hb = 0;

    for (int i = 1; i < bmp.ih.wysokosc_obrazu - 1; i++) {
        for (int j = 1; j < bmp.ih.szerokosc_obrazu - 1; j++) {

            a = ((bmp.piksele[i - 1][j - 1].g) * (-1)) + ((bmp.piksele[i - 1][j].g) * (0)) + ((bmp.piksele[i - 1][j + 1].g) * (1)) + ((bmp.piksele[i][j - 1].g) * (-2)) + ((bmp.piksele[i][j + 1].g) * (2)) + ((bmp.piksele[i + 1][j - 1].g) * (-1)) + ((bmp.piksele[i + 1][j].g) * (0)) + ((bmp.piksele[i + 1][j + 1].g) * (1));
            b = ((bmp.piksele[i - 1][j - 1].g) * (0)) + ((bmp.piksele[i - 1][j].g) * (1)) + ((bmp.piksele[i - 1][j + 1].g) * (2)) + ((bmp.piksele[i][j - 1].g) * (-1)) + ((bmp.piksele[i][j + 1].g) * (1)) + ((bmp.piksele[i + 1][j - 1].g) * (-2)) + ((bmp.piksele[i + 1][j].g) * (-1)) + ((bmp.piksele[i + 1][j + 1].g) * (0));
            c = ((bmp.piksele[i - 1][j - 1].g) * (1)) + ((bmp.piksele[i - 1][j].g) * (2)) + ((bmp.piksele[i - 1][j + 1].g) * (1)) + ((bmp.piksele[i][j - 1].g) * (0)) + ((bmp.piksele[i][j + 1].g) * (0)) + ((bmp.piksele[i + 1][j - 1].g) * (-1)) + ((bmp.piksele[i + 1][j].g) * (-2)) + ((bmp.piksele[i + 1][j + 1].g) * (-1));
            d = ((bmp.piksele[i - 1][j - 1].g) * (2)) + ((bmp.piksele[i - 1][j].g) * (1)) + ((bmp.piksele[i - 1][j + 1].g) * (0)) + ((bmp.piksele[i][j - 1].g) * (1)) + ((bmp.piksele[i][j + 1].g) * (-1)) + ((bmp.piksele[i + 1][j - 1].g) * (0)) + ((bmp.piksele[i + 1][j].g) * (-1)) + ((bmp.piksele[i + 1][j + 1].g) * (-2));
            e = ((bmp.piksele[i - 1][j - 1].g) * (1)) + ((bmp.piksele[i - 1][j].g) * (0)) + ((bmp.piksele[i - 1][j + 1].g) * (-1)) + ((bmp.piksele[i][j - 1].g) * (2)) + ((bmp.piksele[i][j + 1].g) * (-2)) + ((bmp.piksele[i + 1][j - 1].g) * (1)) + ((bmp.piksele[i + 1][j].g) * (0)) + ((bmp.piksele[i + 1][j + 1].g) * (-1));
            f = ((bmp.piksele[i - 1][j - 1].g) * (0)) + ((bmp.piksele[i - 1][j].g) * (-1)) + ((bmp.piksele[i - 1][j + 1].g) * (-2)) + ((bmp.piksele[i][j - 1].g) * (1)) + ((bmp.piksele[i][j + 1].g) * (-1)) + ((bmp.piksele[i + 1][j - 1].g) * (2)) + ((bmp.piksele[i + 1][j].g) * (1)) + ((bmp.piksele[i + 1][j + 1].g) * (0));
            g = ((bmp.piksele[i - 1][j - 1].g) * (-1)) + ((bmp.piksele[i - 1][j].g) * (-2)) + ((bmp.piksele[i - 1][j + 1].g) * (-1)) + ((bmp.piksele[i][j - 1].g) * (0)) + ((bmp.piksele[i][j + 1].g) * (0)) + ((bmp.piksele[i + 1][j - 1].g) * (1)) + ((bmp.piksele[i + 1][j].g) * (2)) + ((bmp.piksele[i + 1][j + 1].g) * (1));
            h = ((bmp.piksele[i - 1][j - 1].g) * (-2)) + ((bmp.piksele[i - 1][j].g) * (-1)) + ((bmp.piksele[i - 1][j + 1].g) * (0)) + ((bmp.piksele[i][j - 1].g) * (-1)) + ((bmp.piksele[i][j + 1].g) * (1)) + ((bmp.piksele[i + 1][j - 1].g) * (0)) + ((bmp.piksele[i + 1][j].g) * (1)) + ((bmp.piksele[i + 1][j + 1].g) * (2));
            if (a > 255)
                a = 255;
            if (a < 0)
                a = 0;
            if (b > 255)
                b = 255;
            if (b < 0)
                b = 0;
            if (c > 255)
                c = 255;
            if (c > 255)
                c = 255;
            if (d < 0)
                d = 0;
            if (d > 255)
                d = 255;
            if (e > 255)
                e = 255;
            if (e < 0)
                e = 0;
            if (f > 255)
                f = 255;
            if (f < 0)
                f = 0;
            if (g > 255)
                g = 255;
            if (g > 255)
                g = 255;
            if (h < 0)
                h = 0;
            if (h > 255)
                h = 255;

            ar = ((bmp.piksele[i - 1][j - 1].r) * (-1)) + ((bmp.piksele[i - 1][j].r) * (0)) + ((bmp.piksele[i - 1][j + 1].r) * (1)) + ((bmp.piksele[i][j - 1].r) * (-2)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (2)) + ((bmp.piksele[i + 1][j - 1].r) * (-1)) + ((bmp.piksele[i + 1][j].r) * (0)) + ((bmp.piksele[i + 1][j + 1].r) * (1));
            br = ((bmp.piksele[i - 1][j - 1].r) * (0)) + ((bmp.piksele[i - 1][j].r) * (1)) + ((bmp.piksele[i - 1][j + 1].r) * (2)) + ((bmp.piksele[i][j - 1].r) * (-1)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (1)) + ((bmp.piksele[i + 1][j - 1].r) * (-2)) + ((bmp.piksele[i + 1][j].r) * (-1)) + ((bmp.piksele[i + 1][j + 1].r) * (0));
            cr = ((bmp.piksele[i - 1][j - 1].r) * (1)) + ((bmp.piksele[i - 1][j].r) * (2)) + ((bmp.piksele[i - 1][j + 1].r) * (1)) + ((bmp.piksele[i][j - 1].r) * (0)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (0)) + ((bmp.piksele[i + 1][j - 1].r) * (-1)) + ((bmp.piksele[i + 1][j].r) * (-2)) + ((bmp.piksele[i + 1][j + 1].r) * (-1));
            dr = ((bmp.piksele[i - 1][j - 1].r) * (2)) + ((bmp.piksele[i - 1][j].r) * (1)) + ((bmp.piksele[i - 1][j + 1].r) * (0)) + ((bmp.piksele[i][j - 1].r) * (1)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (-1)) + ((bmp.piksele[i + 1][j - 1].r) * (0)) + ((bmp.piksele[i + 1][j].r) * (-1)) + ((bmp.piksele[i + 1][j + 1].r) * (-2));
            er = ((bmp.piksele[i - 1][j - 1].r) * (1)) + ((bmp.piksele[i - 1][j].r) * (0)) + ((bmp.piksele[i - 1][j + 1].r) * (-1)) + ((bmp.piksele[i][j - 1].r) * (2)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (-2)) + ((bmp.piksele[i + 1][j - 1].r) * (1)) + ((bmp.piksele[i + 1][j].r) * (0)) + ((bmp.piksele[i + 1][j + 1].r) * (-1));
            fr = ((bmp.piksele[i - 1][j - 1].r) * (0)) + ((bmp.piksele[i - 1][j].r) * (-1)) + ((bmp.piksele[i - 1][j + 1].r) * (-2)) + ((bmp.piksele[i][j - 1].r) * (1)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (-1)) + ((bmp.piksele[i + 1][j - 1].r) * (2)) + ((bmp.piksele[i + 1][j].r) * (1)) + ((bmp.piksele[i + 1][j + 1].r) * (0));
            gr = ((bmp.piksele[i - 1][j - 1].r) * (-1)) + ((bmp.piksele[i - 1][j].r) * (-2)) + ((bmp.piksele[i - 1][j + 1].r) * (-1)) + ((bmp.piksele[i][j - 1].r) * (0)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (0)) + ((bmp.piksele[i + 1][j - 1].r) * (1)) + ((bmp.piksele[i + 1][j].r) * (2)) + ((bmp.piksele[i + 1][j + 1].r) * (1));
            hr = ((bmp.piksele[i - 1][j - 1].r) * (-2)) + ((bmp.piksele[i - 1][j].r) * (-1)) + ((bmp.piksele[i - 1][j + 1].r) * (0)) + ((bmp.piksele[i][j - 1].r) * (-1)) + ((bmp.piksele[i][j].r) * (0)) + ((bmp.piksele[i][j + 1].r) * (1)) + ((bmp.piksele[i + 1][j - 1].r) * (0)) + ((bmp.piksele[i + 1][j].r) * (1)) + ((bmp.piksele[i + 1][j + 1].r) * (2));
            if (ar > 255)
                ar = 255;
            if (ar < 0)
                ar = 0;
            if (br > 255)
                br = 255;
            if (br < 0)
                br = 0;
            if (cr > 255)
                cr = 255;
            if (cr > 255)
                cr = 255;
            if (dr < 0)
                dr = 0;
            if (dr > 255)
                dr = 255;
            if (er > 255)
                er = 255;
            if (er < 0)
                er = 0;
            if (fr > 255)
                fr = 255;
            if (fr < 0)
                fr = 0;
            if (gr > 255)
                gr = 255;
            if (gr > 255)
                gr = 255;
            if (hr < 0)
                hr = 0;
            if (hr > 255)
                hr = 255;


            ab = ((bmp.piksele[i - 1][j - 1].b) * (-1)) + ((bmp.piksele[i - 1][j].b) * (0)) + ((bmp.piksele[i - 1][j + 1].b) * (1)) + ((bmp.piksele[i][j - 1].b) * (-2)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (2)) + ((bmp.piksele[i + 1][j - 1].b) * (-1)) + ((bmp.piksele[i + 1][j].b) * (0)) + ((bmp.piksele[i + 1][j + 1].b) * (1));
            bb = ((bmp.piksele[i - 1][j - 1].b) * (0)) + ((bmp.piksele[i - 1][j].b) * (1)) + ((bmp.piksele[i - 1][j + 1].b) * (2)) + ((bmp.piksele[i][j - 1].b) * (-1)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (1)) + ((bmp.piksele[i + 1][j - 1].b) * (-2)) + ((bmp.piksele[i + 1][j].b) * (-1)) + ((bmp.piksele[i + 1][j + 1].b) * (0));
            cb = ((bmp.piksele[i - 1][j - 1].b) * (1)) + ((bmp.piksele[i - 1][j].b) * (2)) + ((bmp.piksele[i - 1][j + 1].b) * (1)) + ((bmp.piksele[i][j - 1].b) * (0)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (0)) + ((bmp.piksele[i + 1][j - 1].b) * (-1)) + ((bmp.piksele[i + 1][j].b) * (-2)) + ((bmp.piksele[i + 1][j + 1].b) * (-1));
            db = ((bmp.piksele[i - 1][j - 1].b) * (2)) + ((bmp.piksele[i - 1][j].b) * (1)) + ((bmp.piksele[i - 1][j + 1].b) * (0)) + ((bmp.piksele[i][j - 1].b) * (1)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (-1)) + ((bmp.piksele[i + 1][j - 1].b) * (0)) + ((bmp.piksele[i + 1][j].b) * (-1)) + ((bmp.piksele[i + 1][j + 1].b) * (-2));
            eb = ((bmp.piksele[i - 1][j - 1].b) * (1)) + ((bmp.piksele[i - 1][j].b) * (0)) + ((bmp.piksele[i - 1][j + 1].b) * (-1)) + ((bmp.piksele[i][j - 1].b) * (2)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (-2)) + ((bmp.piksele[i + 1][j - 1].b) * (1)) + ((bmp.piksele[i + 1][j].b) * (0)) + ((bmp.piksele[i + 1][j + 1].b) * (-1));
            fb = ((bmp.piksele[i - 1][j - 1].b) * (0)) + ((bmp.piksele[i - 1][j].b) * (-1)) + ((bmp.piksele[i - 1][j + 1].b) * (-2)) + ((bmp.piksele[i][j - 1].b) * (1)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (-1)) + ((bmp.piksele[i + 1][j - 1].b) * (2)) + ((bmp.piksele[i + 1][j].b) * (1)) + ((bmp.piksele[i + 1][j + 1].b) * (0));
            gb = ((bmp.piksele[i - 1][j - 1].b) * (-1)) + ((bmp.piksele[i - 1][j].b) * (-2)) + ((bmp.piksele[i - 1][j + 1].b) * (-1)) + ((bmp.piksele[i][j - 1].b) * (0)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (0)) + ((bmp.piksele[i + 1][j - 1].b) * (1)) + ((bmp.piksele[i + 1][j].b) * (2)) + ((bmp.piksele[i + 1][j + 1].b) * (1));
            hb = ((bmp.piksele[i - 1][j - 1].b) * (-2)) + ((bmp.piksele[i - 1][j].b) * (-1)) + ((bmp.piksele[i - 1][j + 1].b) * (0)) + ((bmp.piksele[i][j - 1].b) * (-1)) + ((bmp.piksele[i][j].b) * (0)) + ((bmp.piksele[i][j + 1].b) * (1)) + ((bmp.piksele[i + 1][j - 1].b) * (0)) + ((bmp.piksele[i + 1][j].b) * (1)) + ((bmp.piksele[i + 1][j + 1].b) * (2));
            if (ab > 255)
                ab = 255;
            if (ab < 0)
                ab = 0;
            if (bb > 255)
                bb = 255;
            if (bb < 0)
                bb = 0;
            if (cb > 255)
                cb = 255;
            if (cb > 255)
                cb = 255;
            if (db < 0)
                db = 0;
            if (db > 255)
                db = 255;
            if (eb > 255)
                eb = 255;
            if (eb < 0)
                eb = 0;
            if (fb > 255)
                fb = 255;
            if (fb < 0)
                fb = 0;
            if (gb > 255)
                gb = 255;
            if (gb > 255)
                gb = 255;
            if (hb < 0)
                hb = 0;
            if (hb > 255)
                hb = 255;
            bmp.nowa[i][j].g = ((a)+(b)+(c)+(d)+(e)+(f)+(g)+(h)) / 8;
            bmp.nowa[i][j].r = ((ar)+(br)+(cr)+(dr)+(er)+(fr)+(gr)+(hr)) / 8;
            bmp.nowa[i][j].b = ((ab)+(bb)+(cb)+(db)+(eb)+(fb)+(gb)+(hb)) / 8;

        }
    }
    char t = 0;
    for (int i = 0; i < bmp.ih.wysokosc_obrazu; i++) {
        for (int j = 0; j < bmp.ih.szerokosc_obrazu; j++) {
            bmp.nowa[0][j].g = t;
            bmp.nowa[bmp.ih.wysokosc_obrazu - 1][j].g = t;
            bmp.nowa[i][0].g = t;
            bmp.nowa[i][bmp.ih.szerokosc_obrazu - 1].g = t;

            bmp.nowa[0][j].b = t;
            bmp.nowa[bmp.ih.wysokosc_obrazu - 1][j].b = t;
            bmp.nowa[i][0].b = t;
            bmp.nowa[i][bmp.ih.szerokosc_obrazu - 1].b = t;

            bmp.nowa[0][j].r = t;
            bmp.nowa[bmp.ih.wysokosc_obrazu - 1][j].r = t;
            bmp.nowa[i][0].r = t;
            bmp.nowa[i][bmp.ih.szerokosc_obrazu - 1].r = t;
        }
    }

}
int main() {
    bitmapa bmp;
    ifstream ifs;
    ofstream ofs;
    string nazwa_pliku;
    string druga_nazwa;
    cout << "Podaj nazwe pliku, ktory chcesz otworzyc" << endl;
    cin >> nazwa_pliku;
    cout << "Podaj nazwe pliku, ktory chcesz stworzyc" << endl;
    cin >> druga_nazwa;
    ifs.open(nazwa_pliku, ios::in | ios::binary);
    if (!ifs) {
        cout << "Nie udalo sie otworzyc pliku";
    }
    else {
        odczytajFH(ifs, bmp);
        odczytajIH(ifs, bmp);
        tworzenie_pierwszej_tablicy(bmp);
        wyswietlanie_paramatrow(bmp);
        wypelnianie_tablicy(bmp, ifs);
        //greyscale(bmp);
        tworzenie_drugiej_tablicy(bmp);
        ofs.open(druga_nazwa.append(".bmp"), ios::out | ios::binary);
        zapiszFH(ofs, bmp);
        zapiszIH(ofs, bmp);
        operator_sobela(bmp);
        zapisz_bitmape(ofs, bmp);
        ifs.close();
        ofs.close();
    }
    return 0;

}