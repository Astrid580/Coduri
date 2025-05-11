#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLAUZE 100
#define MAX_LITERALI 10
#define MAX_LUNGIME 50
#define NUM_VAR 5

typedef struct {
    int literali[MAX_LITERALI];
    int num_literali;
} Clauza;

int numar_clauze = 0;
Clauza clauze[MAX_CLAUZE];

void elimina_literal(int literal) {
    for (int i = 0; i < numar_clauze; ) {
        int eliminat = 0;
        Clauza *c = &clauze[i];
        for (int j = 0; j < c->num_literali; j++) {
            if (c->literali[j] == literal) {
                eliminat = 1;
                break;
            }
        }
        if (eliminat) {
            for (int k = i; k < numar_clauze - 1; k++) {
                clauze[k] = clauze[k+1];
            }
            numar_clauze--;
        } else {
            int nou_nl = 0;
            for (int j = 0; j < c->num_literali; j++) {
                if (c->literali[j] != -literal) {
                    c->literali[nou_nl++] = c->literali[j];
                }
            }
            c->num_literali = nou_nl;
            i++;
        }
    }
}

int gaseste_literal_pur() {
    int aparitii[NUM_VAR + 1] = {0};

    for (int i = 0; i < numar_clauze; i++) {
        for (int j = 0; j < clauze[i].num_literali; j++) {
            int lit = clauze[i].literali[j];
            int var = abs(lit);
            if (lit > 0) {
                if (aparitii[var] == -1) aparitii[var] = -2;
                else if (aparitii[var] == 0) aparitii[var] = 1;
            } else {
                if (aparitii[var] == 1) aparitii[var] = -2;
                else if (aparitii[var] == 0) aparitii[var] = -1;
            }
        }
    }

    for (int i = 1; i <= NUM_VAR; i++) {
        if (aparitii[i] == 1) return i;
        if (aparitii[i] == -1) return -i;
    }
    return 0;
}

int gaseste_unit_clause() {
    for (int i = 0; i < numar_clauze; i++) {
        if (clauze[i].num_literali == 1) {
            return clauze[i].literali[0];
        }
    }
    return 0;
}

int main() {
    int i;
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

    while (1) {
        if (numar_clauze == 0) {
            printf("\nFormula este SATISFIABILA!\n");
            return 0;
        }

        for (i = 0; i < numar_clauze; i++) {
            if (clauze[i].num_literali == 0) {
                printf("\nFormula este NESATISFIABILA!\n");
                return 0;
            }
        }

        int literal = gaseste_literal_pur();
        if (literal) {
            elimina_literal(literal);
            continue;
        }

        literal = gaseste_unit_clause();
        if (literal) {
            elimina_literal(literal);
            continue;
        }
        literal = clauze[0].literali[0];
        elimina_literal(literal);
    }

    return 0;
}
