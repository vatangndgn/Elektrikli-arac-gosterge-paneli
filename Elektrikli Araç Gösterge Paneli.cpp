#include <iostream>
#include <vector>
using namespace std;

class AnaEkran {
public:
    virtual void update(int hiz) = 0;
    virtual ~AnaEkran() = default;
};

class HizSensoru {
protected:
    int hiz = 0;
    vector<AnaEkran*> anaEkran;

public:
    void anaEkranaEkle(AnaEkran* aek) {
        anaEkran.push_back(aek);
    }

    void hizAyarla(int h) {
        hiz = h;

        for (auto aek : anaEkran) {
            aek->update(hiz);
        }
    }
};

class HizGostegesi : public AnaEkran {
private:
    int hiz = 0;

public:
    void hiziGoster() {
        cout << "Hiz: " << hiz << " km/h" << endl;
    }

    void update(int h) override {
        hiz = h;
        hiziGoster();
    }
};

class YakitTuketimi : public AnaEkran {
protected:
    int hiz = 0;
    double yakitMiktari = 88.0;
    double anlikTuketim = 0.0;
    double anlikEksilen = 0.0;
    int eskiHiz = 0;
    double aracKutlesi = 1800.0;

public:
    void anlikTuketimiGoster(int guncelHiz) {
        double sabitYuk = guncelHiz * 0.15;
        double ivmeYuku = 0.0;
        int ivme = guncelHiz - eskiHiz;

        if (ivme > 0) {
            double hizlanmaKuvveti = aracKutlesi * (ivme / 3.6);
            ivmeYuku = (hizlanmaKuvveti * (guncelHiz / 3.6) / 1000.0) / 0.90;
        }

        anlikTuketim = sabitYuk + ivmeYuku;

        cout << "Anlik Tuketim: "
            << anlikTuketim
            << " kW" << endl;

        eskiHiz = guncelHiz;
    }

    void kalanYakit() {
        anlikEksilen = anlikTuketim / 3600.0;
        yakitMiktari -= anlikEksilen;

        cout << "Kalan Yakit: "
            << yakitMiktari
            << " kWh" << endl;
    }

    void update(int h) override {
        hiz = h;
        anlikTuketimiGoster(h);
        kalanYakit();
    }
};

int main() {
    HizSensoru arabaSensoru;

    HizGostegesi gostergeEkrani;
    YakitTuketimi yakitEkrani;
  

    arabaSensoru.anaEkranaEkle(&gostergeEkrani);
    arabaSensoru.anaEkranaEkle(&yakitEkrani);
    

    cout << "1. DURUM: 0 km/h -> 10 km/h" << endl;
    arabaSensoru.hizAyarla(10);

    cout << "\n2. DURUM: 10 km/h -> 40 km/h" << endl;
    arabaSensoru.hizAyarla(40);

    return 0;
}