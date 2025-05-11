#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLAUZE 100
#define MAX_LITERALI 10
#define MAX_LUNGIME 50

typedef struct {
    int literali[MAX_LITERALI];
    int num_literali;
} Clauza;

int numar_clauze = 0;
Clauza clauze[MAX_CLAUZE];

int rezolutie(Clauza a, Clauza b, Clauza *rezultat) {
    int gasit = 0;
    rezultat->num_literali = 0;

    for (int i = 0; i < a.num_literali; i++) {
        for (int j = 0; j < b.num_literali; j++) {
            if (a.literali[i] == -b.literali[j]) {
                gasit++;
                if (gasit > 1) return 0;

                for (int k = 0; k < a.num_literali; k++) {
                    if (k != i) rezultat->literali[rezultat->num_literali++] = a.literali[k];
                }
                for (int k = 0; k < b.num_literali; k++) {
                    if (k != j) rezultat->literali[rezultat->num_literali++] = b.literali[k];
                }
            }
        }
    }

    return gasit == 1;
}

int clauze_egale(Clauza a, Clauza b) {
    if (a.num_literali != b.num_literali) return 0;
    for (int i = 0; i < a.num_literali; i++) {
        int gasit = 0;
        for (int j = 0; j < b.num_literali; j++) {
            if (a.literali[i] == b.literali[j]) {
                gasit = 1;
                break;
            }
        }
        if (!gasit) return 0;
    }
    return 1;
}

int main() {
    char linie[MAX_LUNGIME];
    printf("Introdu clauze (format: (A, -B, C)), una pe linie. Scrie STOP pentru a termina:\n");

    while (1) {
        printf("> ");
        fgets(linie, sizeof(linie), stdin);
        if (strncmp(linie, "STOP", 4) == 0) break;

        Clauza c;
        c.num_literali = 0;

        char *p = strtok(linie, "(,), \n");
        while (p != NULL) {
            if (p[0] == '-') {
                c.literali[c.num_literali++] = -(p[1] - 'A' + 1);
            } else {
                c.literali[c.num_literali++] = p[0] - 'A' + 1;
            }
            p = strtok(NULL, "(,), \n");
        }
        clauze[numar_clauze++] = c;
    }

    int schimbat = 1;
    while (schimbat) {
        schimbat = 0;
        for (int i = 0; i < numar_clauze; i++) {
            for (int j = i + 1; j < numar_clauze; j++) {
                Clauza rez;
                if (rezolutie(clauze[i], clauze[j], &rez)) {
                    int exista = 0;
                    for (int k = 0; k < numar_clauze; k++) {
                        if (clauze_egale(clauze[k], rez)) {
                            exista = 1;
                            break;
                        }
                    }
                    if (!exista) {
                        if (rez.num_literali == 0) {
                            printf("\nFormula este NESATISFIABILA (clauza vida gasita).\n");
                            return 0;
                        }
                        if (numar_clauze < MAX_CLAUZE)
                            clauze[numar_clauze++] = rez;
                        schimbat = 1;
                    }
                }
            }
        }
    }

    printf("\nFormula este SATISFIABILA (nu s-a gasit clauza vida).\n");
    return 0;
}
