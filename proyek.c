#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MAPEL 5

struct Siswa {
    char nim[20];
    char nama[50];
    int nilai[MAX_MAPEL];
    float rata;
    char grade;
};

int loginDosen();
void BuatData();
float hitungRata(int nilai[]);
char tentukanGrade(float rata);
void simpanNilai(struct Siswa *s);
void tampilNilai();
void updateNilai();
void hapusNilai();

int main() {
    int pilihan;

    do {
        printf("\n===== SISTEM PENGELOLAAN NILAI =====\n");
        printf("1. Login Dosen\n");
        printf("2. Mahasiswa (Lihat Nilai)\n");
        printf("3. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        if (pilihan == 1 && loginDosen()) {
            int menu;
            do {
                printf("\n===== MENU DOSEN =====\n");
                printf("1. Input Nilai Mahasiswa\n");
                printf("2. Tampilkan Nilai Mahasiswa\n");
                printf("3. Update Data Mahasiswa\n");
                printf("4. Hapus Data Mahasiswa\n");
                printf("5. Logout\n");
                printf("Pilih menu: ");
                scanf("%d", &menu);

                if (menu == 1) BuatData();
                else if (menu == 2) tampilNilai();
                else if (menu == 3) {
                    tampilNilai();
                    updateNilai();
                    tampilNilai();
                }
                else if (menu == 4) {
                    tampilNilai();
                    hapusNilai();
                    tampilNilai();
                }
            } while (menu != 5);
        }

    } while (pilihan != 3);

    return 0;
}

int loginDosen() {
    FILE *file = fopen("dosen.txt", "r");
    char u[50], p[50], fu[50], fp[50];

    if (!file) {
        printf("File dosen.txt tidak ditemukan!\n");
        return 0;
    }

    printf("Username: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);

    while (fscanf(file, "%s %s", fu, fp) != EOF) {
        if (strcmp(u, fu) == 0 && strcmp(p, fp) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    printf("Login gagal.\n");
    return 0;
}

void BuatData() {
    struct Siswa s;
    getchar();

    printf("NIM  : ");
    fgets(s.nim, sizeof(s.nim), stdin);
    s.nim[strcspn(s.nim, "\n")] = 0;

    printf("Nama : ");
    fgets(s.nama, sizeof(s.nama), stdin);
    s.nama[strcspn(s.nama, "\n")] = 0;

    for (int i = 0; i < MAX_MAPEL; i++) {
        printf("Nilai mapel %d: ", i + 1);
        scanf("%d", &s.nilai[i]);
    }

    s.rata = hitungRata(s.nilai);
    s.grade = tentukanGrade(s.rata);
    simpanNilai(&s);

    printf("Data berhasil disimpan.\n");
}

float hitungRata(int nilai[]) {
    int total = 0;
    for (int i = 0; i < MAX_MAPEL; i++)
        total += nilai[i];
    return (float) total / MAX_MAPEL;
}

char tentukanGrade(float r) {
    if (r >= 85) return 'A';
    if (r >= 75) return 'B';
    if (r >= 65) return 'C';
    if (r >= 50) return 'D';
    return 'E';
}

void simpanNilai(struct Siswa *s) {
    FILE *file = fopen("nilai.txt", "a");

    if (!file) {
        printf("Gagal membuka file.\n");
        return;
    }

    fprintf(file, "%s %s ", s->nim, s->nama);
    for (int i = 0; i < MAX_MAPEL; i++)
        fprintf(file, "%d ", s->nilai[i]);
    fprintf(file, "%.2f %c\n", s->rata, s->grade);

    fclose(file);
}

void tampilNilai() {
    FILE *file = fopen("nilai.txt", "r");
    struct Siswa s;

    if (!file) {
        printf("Belum ada data.\n");
        return;
    }

    printf("\n===== DATA NILAI MAHASISWA =====\n");
    while (fscanf(file, "%s %s %d %d %d %d %d %f %c",
                  s.nim, s.nama,
                  &s.nilai[0], &s.nilai[1], &s.nilai[2],
                  &s.nilai[3], &s.nilai[4],
                  &s.rata, &s.grade) != EOF) {

        printf("NIM: %s | Nama: %s | Rata: %.2f | Grade: %c\n",
               s.nim, s.nama, s.rata, s.grade);
    }

    fclose(file);
}

void updateNilai() {
    FILE *file = fopen("nilai.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Siswa s;
    char nimCari[20], konfirmasi;
    int found = 0;

    if (!file || !temp) return;

    getchar();
    printf("Masukkan NIM yang akan diupdate: ");
    fgets(nimCari, sizeof(nimCari), stdin);
    nimCari[strcspn(nimCari, "\n")] = 0;

    while (fscanf(file, "%s %s %d %d %d %d %d %f %c",
                  s.nim, s.nama,
                  &s.nilai[0], &s.nilai[1], &s.nilai[2],
                  &s.nilai[3], &s.nilai[4],
                  &s.rata, &s.grade) != EOF) {

        if (strcmp(s.nim, nimCari) == 0) {
            found = 1;
            printf("Yakin update? (y/n): ");
            scanf(" %c", &konfirmasi);

            if (tolower(konfirmasi) == 'y') {
                for (int i = 0; i < MAX_MAPEL; i++) {
                    printf("Nilai baru mapel %d: ", i + 1);
                    scanf("%d", &s.nilai[i]);
                }
                s.rata = hitungRata(s.nilai);
                s.grade = tentukanGrade(s.rata);
            }
        }

        fprintf(temp, "%s %s ", s.nim, s.nama);
        for (int i = 0; i < MAX_MAPEL; i++)
            fprintf(temp, "%d ", s.nilai[i]);
        fprintf(temp, "%.2f %c\n", s.rata, s.grade);
    }

    fclose(file);
    fclose(temp);
    remove("nilai.txt");
    rename("temp.txt", "nilai.txt");

    if (!found)
        printf("NIM tidak ditemukan.\n");
}

void hapusNilai() {
    FILE *file = fopen("nilai.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Siswa s;
    char nimCari[20];
    int found = 0;

    if (!file || !temp) return;

    getchar();
    printf("Masukkan NIM yang akan dihapus: ");
    fgets(nimCari, sizeof(nimCari), stdin);
    nimCari[strcspn(nimCari, "\n")] = 0;

    while (fscanf(file, "%s %s %d %d %d %d %d %f %c",
                  s.nim, s.nama,
                  &s.nilai[0], &s.nilai[1], &s.nilai[2],
                  &s.nilai[3], &s.nilai[4],
                  &s.rata, &s.grade) != EOF) {

        if (strcmp(s.nim, nimCari) != 0) {
            fprintf(temp, "%s %s ", s.nim, s.nama);
            for (int i = 0; i < MAX_MAPEL; i++)
                fprintf(temp, "%d ", s.nilai[i]);
            fprintf(temp, "%.2f %c\n", s.rata, s.grade);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);
    remove("nilai.txt");
    rename("temp.txt", "nilai.txt");

    if (found)
        printf("Data berhasil dihapus.\n");
    else
        printf("NIM tidak ditemukan.\n");
}
