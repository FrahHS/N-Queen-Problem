#include <stdio.h>
#include <stdlib.h>

#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define G 7
#define H 8

#define MAX 8

#define bool int
#define true 1
#define false 0

FILE * fp;
FILE * fpM;

int actual = 0;
int actualM = 0;

typedef struct{
    int riga;
    int colonna;
    int disponibile;
} Posizione;

typedef struct {
    Posizione **griglia;
} Scacchiera;

typedef struct {
    Posizione posizione;
    Posizione * posizioniPossibili;
    int id;
} Regina;

Scacchiera * inizializzaScacchiera(Scacchiera * s);
void stampaScacchiera(Scacchiera * s);
void OccupaCaselle(Regina * r, Scacchiera *s);
void deOccupaCaselle(Regina * r, Scacchiera *s);
void inizializzaRegina(Regina * r, Scacchiera *s, int riga, int colonna);
void deInizializzaRegina(Regina * r, Scacchiera *s);
void scriviScacchiera(Scacchiera * s);
void scriviMathScacchiera(Scacchiera * s);
bool controllaScacchiera(Scacchiera *s);
void piazzaRegineRicorsivo(Regina **regine, Scacchiera *s, int nRegine);


int main() {
    int k = 0;

    printf("Starting...\n");

    fp = fopen("chessboard.txt", "w+");
    fpM = fopen("matrices.txt", "w+");

    Scacchiera * s = (Scacchiera *)malloc(sizeof(Scacchiera));
    s = inizializzaScacchiera(s);

    Regina ** regine = (Regina**)malloc(sizeof(Regina*)*MAX);

    for(k = 0; k < MAX; k++) {
        regine[k] = malloc(sizeof(Regina));
        regine[k]->id = k+1;
    }

    piazzaRegineRicorsivo(regine, s, MAX);

    printf("Casi totali trovati: %d\n", actual);
    printf("Closing...");

    return 0;
}

Scacchiera * inizializzaScacchiera(Scacchiera * s) {
    int colonna = A;
    int riga = 1;

    printf("Inizializzo scacchiera...\n");

    /*Alloco in memoria la scacchiera (8 puntatori a posizione e in ognuno 8 posizioni)*/
    s->griglia = (Posizione **)malloc(MAX*sizeof(Posizione*));
    for(colonna = A; colonna <= MAX; colonna++) {
        s->griglia[colonna-1] = (Posizione *)malloc(MAX*sizeof(Posizione));
    }

    /*inizializzo ogni posizione sulla scacchiera*/
    for(riga = 1; riga <= MAX; riga++) {
        for(colonna = A; colonna <= MAX; colonna++) {
            s->griglia[colonna-1][riga-1].riga = riga;
            s->griglia[colonna-1][riga-1].colonna = colonna;
            s->griglia[colonna-1][riga-1].disponibile = 0;
        }
    }

    printf("Scacchiera Inizializzata!\n");

    return s;
}

void stampaScacchiera(Scacchiera * s) {
    int colonna = A;
    int riga = MAX;
    int i = 1;

    printf("Stampo scacchiera numero: %d...\n", ++actual);

    for(riga = MAX; riga > 0; riga--) {
        printf("%d)\t", riga);

        for(colonna = A; colonna <= MAX && riga != 0; colonna++) {
            if(s->griglia[colonna-1][riga-1].disponibile == 0)
                printf(" \t");
            else if(s->griglia[colonna-1][riga-1].disponibile == 1) 
                printf("#\t");
            else
                printf("~\t");
        }
        printf("\n");
    }
    printf(" \t");

    for (i = 1; i <= MAX; i++) {
        printf("%c)\t", 'A' + i-1);
    }
    printf("\n");

    scriviScacchiera(s);
    scriviMathScacchiera(s);
}

void scriviScacchiera(Scacchiera * s) {
    int riga = MAX;
    int colonna = A;
    int i = 1;

    fprintf(fp, "Stampo scacchiera numero: %d...\n", ++actualM);

    for(riga = MAX; riga > 0; riga--) {
        fprintf(fp, "%d)\t", riga);

        for(colonna = A; colonna <= MAX && riga != 0; colonna++) {
            if(s->griglia[colonna-1][riga-1].disponibile == 0)
                fprintf(fp, " \t");
            else if(s->griglia[colonna-1][riga-1].disponibile == 1) 
                fprintf(fp, "#\t");
            else
                fprintf(fp, "~\t");
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, " \t");

    for (i = 1; i <= MAX; i++) {
        fprintf(fp, "%c)\t", 'A'+i-1);
    }
    fprintf(fp, "\n");
}

void scriviMathScacchiera(Scacchiera * s) {
    int riga = MAX;
    int colonna = A;

    for(riga = MAX; riga > 0; riga--) {
        for(colonna = A; colonna <= MAX && riga != 0; colonna++) {
            if(s->griglia[colonna-1][riga-1].disponibile == 0)
                fprintf(fpM, "0");
            else if(s->griglia[colonna-1][riga-1].disponibile == 1) 
                fprintf(fpM, "1");
            else
                fprintf(fpM, "0");
        }
        fprintf(fpM, "\n");
    }
    fprintf(fpM, "\n");
}

void inizializzaRegina(Regina * r, Scacchiera *s, int riga, int colonna) {
    r->posizione = s->griglia[riga-1][colonna-1];
    s->griglia[riga-1][colonna-1].disponibile += 1;

    OccupaCaselle(r, s);
}

void deInizializzaRegina(Regina * r, Scacchiera *s) {
    s->griglia[r->posizione.colonna-1][r->posizione.riga-1].disponibile -= 1;

    deOccupaCaselle(r, s);
}

void OccupaCaselle(Regina * r, Scacchiera *s) {
    int k = r->posizione.riga - r->posizione.colonna;
    int i = 1;
    int x = 1;
    int y = 1;

    /* RIGHE */
    for(i = 1; i <= MAX; i++) {
        if(i != r->posizione.colonna)
            s->griglia[i-1][r->posizione.riga-1].disponibile += 2;
    }

    /* COLONNE */
    for(i = 1; i <= MAX; i++) {
        if(i != r->posizione.riga)
            s->griglia[r->posizione.colonna-1][i-1].disponibile += 2;
    }

    while(x <= MAX) {
        y = x + k;

        if(x != r->posizione.colonna && y != r->posizione.riga && x <= MAX && y<=MAX && x>0 && y>0) {
            s->griglia[x-1][y-1].disponibile += 2;
        }
        x++;
    }

    x = 1;
    y = 1;

    k = r->posizione.colonna + r->posizione.riga;

    while(x <= MAX) {
        y = -x + k;

        if(x != r->posizione.colonna && y != r->posizione.riga && x <= MAX && y<=MAX && x>0 && y>0) {
            s->griglia[x-1][y-1].disponibile += 2;
        }
        x++;
    }
}

void deOccupaCaselle(Regina * r, Scacchiera *s) {
    int k = r->posizione.riga - r->posizione.colonna;
    int i = 1;
    int x = 1;
    int y = 1;

    for(i = 1; i <= MAX; i++) {
        if(i != r->posizione.colonna)
            s->griglia[i-1][r->posizione.riga-1].disponibile -= 2;
    }

    for(i = 1; i <= MAX; i++) {
        if(i != r->posizione.riga)
            s->griglia[r->posizione.colonna-1][i-1].disponibile -= 2;
    }

    while(y <= MAX) {
        y = x + k;

        if(x != r->posizione.colonna && y != r->posizione.riga && x <= MAX && y<=MAX && x>0 && y>0) {
            s->griglia[x-1][y-1].disponibile -= 2;
        }
        x++;
    }

    x = 1;
    y = 1;

    k = r->posizione.colonna + r->posizione.riga;

    while(x <= MAX) {
        y = -x + k;

        if(x != r->posizione.colonna && y != r->posizione.riga && x <= MAX && y<=MAX && x>0 && y>0) {
            s->griglia[x-1][y-1].disponibile -= 2;
        }
        x++;
    }

}

void piazzaRegineRicorsivo(Regina ** regine, Scacchiera *s, int nRegine) {
    int i = 0;

    for(i = 0; i < MAX; i++) {
        if(s->griglia[(regine[MAX - nRegine]->id)-1][i].disponibile == 0) {
            inizializzaRegina(regine[MAX - nRegine],s,regine[MAX - nRegine]->id,i+1);
            if(nRegine > 1)
                piazzaRegineRicorsivo(regine, s, nRegine-1);
            if(controllaScacchiera(s) && nRegine == 1)
                stampaScacchiera(s);
            deInizializzaRegina(regine[MAX - nRegine],s);
        }
    }
}

bool controllaScacchiera(Scacchiera *s) {
    int i = 0;
    int j = 0;

    for(j = 0; j < MAX; j++) {
        for(i = 0; i < MAX; i++) {
            if((s->griglia[j][i].disponibile % 2) && s->griglia[j][i].disponibile != 1)
                return false;
        }
    }
    return true;
}
