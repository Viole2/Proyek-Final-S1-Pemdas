#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MAPEL 5

struct Siswa {
    char nama[50];
    int nilai[MAX_MAPEL];
    float rata;
    char grade;
};

float hitungRata(int *nilai);
char tentukanGrade(float rata);
void simpanNilai(struct Siswa *s);
void tampilNilai();
void hapusNilai();
int loginDosen();

int main() {
    int pilihan;

    do {
        printf("\n===== SISTEM PENGELOLAAN NILAI =====\n");
        printf("1. Login Dosen\n");
        printf("2. Mahasiswa (Lihat Nilai)\n");
        printf("3. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
        case 1:
            if (loginDosen()) {
                int menu;
                do {
                    printf("\n===== MENU DOSEN =====\n");
                    printf("1. Input Nilai Mahasiswa\n");
                    printf("2. Tampilkan Nilai Mahasiswa\n");
                    printf("3. Hapus Data Mahasiswa\n");
                    printf("4. Logout\n");
                    printf("Pilih menu: ");
                    scanf("%d", &menu);

                    if (menu == 1) {
                        struct Siswa s;
                        struct Siswa *ps = &s;

                        getchar();
                        printf("\nNama Mahasiswa: ");
                        fgets(ps->nama, sizeof(ps->nama), stdin);
                        ps->nama[strcspn(ps->nama, "\n")] = 0;

                        for (int i = 0; i < MAX_MAPEL; i++) {
                            printf("Nilai Mata Pelajaran %d: ", i + 1);
                            scanf("%d", &ps->nilai[i]);
                        }

                        ps->rata = hitungRata(ps->nilai);
                        ps->grade = tentukanGrade(ps->rata);

                        simpanNilai(ps);
                        printf("\nData berhasil disimpan!\n");
                    }
                    else if (menu == 2) {
                        tampilNilai();
                    }
                    else if (menu == 3) {
                        hapusNilai();
                    }

                } while (menu != 4);
            }
            break;

        case 2:
            tampilNilai();
            break;

        case 3:
            printf("\nTerima kasih!\n");
            break;

        default:
            printf("\nPilihan tidak valid!\n");
        }

    } while (pilihan != 3);

    return 0;
}

int loginDosen() {
    FILE *file;
    char inputUser[50], inputPass[50];
    char fileUser[50], filePass[50];
    int found = 0;

    file = fopen("dosen.txt", "r");
    if (file == NULL) {
        printf("File dosen.txt tidak ditemukan!\n");
        return 0;
    }

    printf("Username: ");
    scanf("%s", inputUser);
    printf("Password: ");
    scanf("%s", inputPass);

    while (fscanf(file, "%s %s", fileUser, filePass) != EOF) {
        if (strcmp(fileUser, inputUser) == 0 &&
            strcmp(filePass, inputPass) == 0) {
            found = 1;
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Login berhasil sebagai dosen\n");
        return 1;
    } else {
        printf("Login gagal!\n");
        return 0;
    }
}

float hitungRata(int *nilai) {
    int total = 0;
    for (int i = 0; i < MAX_MAPEL; i++) {
        total += *(nilai + i);
    }
    return (float) total / MAX_MAPEL;
}

char tentukanGrade(float rata) {
    if (rata >= 85) return 'A';
    else if (rata >= 75) return 'B';
    else if (rata >= 65) return 'C';
    else if (rata >= 50) return 'D';
    else return 'E';
}

void simpanNilai(struct Siswa *s) {
    FILE *file = fopen("nilai.txt", "a");

    fprintf(file, "%s ", s->nama);
    for (int i = 0; i < MAX_MAPEL; i++)
        fprintf(file, "%d ", s->nilai[i]);

    fprintf(file, "%.2f %c\n", s->rata, s->grade);
    fclose(file);
}

void tampilNilai() {
    struct Siswa s;
    struct Siswa *ps = &s;
    FILE *file = fopen("nilai.txt", "r");

    if (file == NULL) {
        printf("\nBelum ada data nilai!\n");
        return;
    }

    printf("\n===== DATA NILAI MAHASISWA =====\n");
    while (fscanf(file, "%s %d %d %d %d %d %f %c",
           ps->nama,
           &ps->nilai[0], &ps->nilai[1], &ps->nilai[2],
           &ps->nilai[3], &ps->nilai[4],
           &ps->rata, &ps->grade) != EOF) {

        printf("\nNama       : %s", ps->nama);
        printf("\nRata-rata  : %.2f", ps->rata);
        printf("\nGrade      : %c\n", ps->grade);
    }

    fclose(file);
}

void hapusNilai() {
    FILE *file, *temp;
    struct Siswa s;
    struct Siswa *ps = &s;
    char namaHapus[50];
    char konfirmasi;
    int found = 0;

    file = fopen("nilai.txt", "r");
    if (file == NULL) {
        printf("\nBelum ada data nilai!\n");
        return;
    }

    temp = fopen("temp.txt", "w");

    getchar();
    printf("\nMasukkan nama mahasiswa yang akan dihapus: ");
    fgets(namaHapus, sizeof(namaHapus), stdin);
    namaHapus[strcspn(namaHapus, "\n")] = 0;

    printf("Yakin ingin menghapus data ini? (y/n): ");
    scanf(" %c", &konfirmasi);
    konfirmasi = tolower(konfirmasi);

    if (konfirmasi != 'y') {
        fclose(file);
        fclose(temp);
        remove("temp.txt");
        printf("\nPenghapusan dibatalkan.\n");
        return;
    }

    while (fscanf(file, "%s %d %d %d %d %d %f %c",
           ps->nama,
           &ps->nilai[0], &ps->nilai[1], &ps->nilai[2],
           &ps->nilai[3], &ps->nilai[4],
           &ps->rata, &ps->grade) != EOF) {

        if (strcmp(ps->nama, namaHapus) != 0) {
            fprintf(temp, "%s ", ps->nama);
            for (int i = 0; i < MAX_MAPEL; i++)
                fprintf(temp, "%d ", ps->nilai[i]);
            fprintf(temp, "%.2f %c\n", ps->rata, ps->grade);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove("nilai.txt");
    rename("temp.txt", "nilai.txt");

    if (found)
        printf("\nData mahasiswa berhasil dihapus.\n");
    else
        printf("\nData tidak ditemukan.\n");
}
