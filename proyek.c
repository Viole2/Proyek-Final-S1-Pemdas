#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MAPEL 5

struct Siswa {
    char nama[50];
    int nilai[MAX_MAPEL];
    float rata;
    char grade;
};

struct Dosen {
    char username[20];
    char password[20];
};

float hitungRata(int nilai[]);
char tentukanGrade(float rata);
void simpanNilai(struct Siswa s);
void tampilNilai();
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
                    printf("3. Logout\n");
                    printf("Pilih menu: ");
                    scanf("%d", &menu);
                    if (menu == 1) {
                        struct Siswa s;
                        getchar();

                        printf("\nNama Mahasiswa: ");
                        fgets(s.nama, sizeof(s.nama), stdin);
                        s.nama[strcspn(s.nama, "\n")] = 0;

                        for (int i = 0; i < MAX_MAPEL; i++) {
                            printf("Nilai Mata Pelajaran %d: ", i + 1);
                            scanf("%d", &s.nilai[i]);
                        }

                        s.rata = hitungRata(s.nilai);
                        s.grade = tentukanGrade(s.rata);

                        simpanNilai(s);
                        printf("\nData berhasil disimpan!\n");
                    }
                    else if (menu == 2) {
                        tampilNilai();
                    }

                } while (menu != 3);
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

float hitungRata(int nilai[]) {
    int total = 0;
    for (int i = 0; i < MAX_MAPEL; i++)
        total += nilai[i];
    return (float) total / MAX_MAPEL;
}

char tentukanGrade(float rata) {
    if (rata >= 85) return 'A';
    else if (rata >= 75) return 'B';
    else if (rata >= 65) return 'C';
    else if (rata >= 50) return 'D';
    else return 'E';
}

void simpanNilai(struct Siswa s) {
    FILE *file = fopen("nilai.txt", "a");

    fprintf(file, "%s ", s.nama);
    for (int i = 0; i < MAX_MAPEL; i++)
        fprintf(file, "%d ", s.nilai[i]);

    fprintf(file, "%.2f %c\n", s.rata, s.grade);
    fclose(file);
}

void tampilNilai() {
    struct Siswa s;
    FILE *file = fopen("nilai.txt", "r");

    if (file == NULL) {
        printf("\nBelum ada data nilai!\n");
        return;
    }

    printf("\n===== DATA NILAI MAHASISWA =====\n");
    while (fscanf(file, "%s %d %d %d %d %d %f %c",
           s.nama,
           &s.nilai[0], &s.nilai[1], &s.nilai[2],
           &s.nilai[3], &s.nilai[4],
           &s.rata, &s.grade) != EOF) {

        printf("\nNama : %s", s.nama);
        printf("\nRata-rata : %.2f", s.rata);
        printf("\nGrade : %c\n", s.grade);
    }

    fclose(file);
}