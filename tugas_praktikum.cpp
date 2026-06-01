#include <iostream>
#include <string>
#include <array>

using namespace std;

class TokoElektronik {
private:
    array<string, 3> etalase;
public:
    TokoElektronik() {
        etalase = {"Laptop ASUS ROG", "Smartphone Samsung S24", "Smart TV LG 43 Inch"};
    }
    string ambilProduk(size_t nomorRak) {
        if (nomorRak >= etalase.size()) {
            throw "Gagal Mengambil Barang : Rak nomor [" + to_string(nomorRak) + "] kosong atau tidak tersedia!";
        }
        return etalase[nomorRak];
    }
};

const char* FILE_NAME = "gudang.txt";

void bacaGudang() {
    FILE* file = fopen(FILE_NAME, "r");
    cout << "\n=== DAFTAR BARANG GUDANG ===\n";
    if (!file) { cout << "[Gudang Kosong]\n============================\n"; return; }
    
    char buffer[256];
    int i = 1;
    while (fgets(buffer, sizeof(buffer), file)) {
        string baris(buffer);
        if (!baris.empty() && baris != "\n") {
            cout << i++ << ". " << baris;
        }
    }
    fclose(file);
    cout << "============================\n";
}

void tambahBarang() {
    FILE* file = fopen(FILE_NAME, "a");
    if (!file) return;
    string nama;
    cout << "Masukkan nama barang: "; cin.ignore(); getline(cin, nama);
    if (!nama.empty()) {
        fprintf(file, "%s\n", nama.c_str());
    }
    fclose(file);
}

void ubahHapusBarang(bool isDelete) {
    FILE* fileIn = fopen(FILE_NAME, "r");
    if (!fileIn) return;

    string data[100];
    char buffer[256];
    int total = 0, target;

    while (fgets(buffer, sizeof(buffer), fileIn)) {
        string baris(buffer);
        if (!baris.empty() && baris != "\n") {
            if (baris.back() == '\n') baris.pop_back(); // Hapus newline di akhir
            data[total++] = baris;
        }
    }
    fclose(fileIn);
    if (total == 0) return;

    cout << "Pilih nomor barang: "; cin >> target;
    if (target < 1 || target > total) return;

    if (!isDelete) {
        cout << "Masukkan nama baru: "; cin.ignore(); getline(cin, data[target - 1]);
    }

    FILE* fileOut = fopen(FILE_NAME, "w");
    for (int i = 0; i < total; i++) {
        if (isDelete && i == (target - 1)) continue;
        fprintf(fileOut, "%s\n", data[i].c_str());
    }
    fclose(fileOut);
}

void simulasiEtalase(TokoElektronik& toko) {
    size_t tes[] = {1, 5};
    for (size_t rak : tes) {
        cout << "\nMencoba rak indeks ke-" << rak << "...\n";
        try {
            cout << "Sukses: " << toko.ambilProduk(rak) << endl;
        } catch (const string& e) {
            cout << "Error: " << e << endl;
        }
    }
}

int main() {
    TokoElektronik toko;
    int menu;
    do {
        bacaGudang();
        cout << "\n1. Tambah | 2. Ubah | 3. Hapus | 4. Simulasi | 5. Keluar\nPilih: "; cin >> menu;
        if (menu == 1) tambahBarang();
        else if (menu == 2) ubahHapusBarang(false);
        else if (menu == 3) ubahHapusBarang(true);
        else if (menu == 4) simulasiEtalase(toko);
        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); }
    } while (menu != 5);
    return 0;
}