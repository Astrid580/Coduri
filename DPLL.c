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

void elimina_literal_din_formula(Clauza *formula, int *num_clauze, int literal) {
    for (int i = 0; i < *num_clauze;) {
        int eliminat = 0;
        Clauza *c = &formula[i];
        for (int j = 0; j < c->num_literali; j++) {
            if (c->literali[j] == literal) {
                eliminat = 1;
                break;
            }
        }

        if (eliminat) {
            for (int k = i; k < *num_clauze - 1; k++) {
                formula[k] = formula[k + 1];
            }
            (*num_clauze)--;
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

int gaseste_literal_pur(Clauza *formula, int num_clauze) {
    int aparitii[NUM_VAR + 1] = {0};
    for (int i = 0; i < num_clauze; i++) {
        for (int j = 0; j < formula[i].num_literali; j++) {
            int lit = formula[i].literali[j];
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

int gaseste_unit_clause(Clauza *formula, int num_clauze) {
    for (int i = 0; i < num_clauze; i++) {
        if (formula[i].num_literali == 1) {
            return formula[i].literali[0];
        }
    }
    return 0;
}

int dpll(Clauza *formula, int num_clauze) {
    if (num_clauze == 0) return 1;

    for (int i = 0; i < num_clauze; i++) {
        if (formula[i].num_literali == 0) return 0;
    }

    int literal = gaseste_literal_pur(formula, num_clauze);
    if (literal) {
        elimina_literal_din_formula(formula, &num_clauze, literal);
        if (dpll(formula, num_clauze)) return 1;
        return 0;
    }

    literal = gaseste_unit_clause(formula, num_clauze);
    if (literal) {
        elimina_literal_din_formula(formula, &num_clauze, literal);
        if (dpll(formula, num_clauze)) return 1;
        return 0;
    }

    literal = formula[0].literali[0];
    int copia_clauze[MAX_CLAUZE];
    memcpy(copia_clauze, formula, sizeof(Clauza) * num_clauze);

    elimina_literal_din_formula(copia_clauze, &num_clauze, literal);
    if (dpll(copia_clauze, num_clauze)) return 1;

    elimina_literal_din_formula(copia_clauze, &num_clauze, -literal);
    return dpll(copia_clauze, num_clauze);
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

    int rezultat = dpll(clauze, numar_clauze);

    if (rezultat)
        printf("Formula este SATISFIABILA (SAT).\n");
    else
        printf("Formula este NESATISFIABILA (UNSAT).\n");

    return 0;
}
