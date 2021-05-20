#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
FILE * matrix;

int actual = 0;
int actualFile = 0;



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
    printf("Starting...\n");

    fp = fopen("scacchiere.txt", "w+");
    matrix = fopen("mathScacchiera.txt", "w+");


    Scacchiera * s = (Scacchiera *)malloc(sizeof(Scacchiera));
    s = inizializzaScacchiera(s);

    Regina ** regine = (Regina**)malloc(sizeof(Regina*)*MAX);

    for(int k = 0; k < MAX; k++) {
        regine[k] = malloc(sizeof(Regina));
        regine[k]->id = k+1;
    }

    piazzaRegineRicorsivo(regine, s, MAX);

    printf("Closing...");

    return 0;
}

Scacchiera * inizializzaScacchiera(Scacchiera * s) {
    printf("Inizializzo scacchiera...\n");

    /*Alloco in memoria la scacchiera (8 puntatori a posizione e in ognuno 8 posizioni)*/
    s->griglia = (Posizione **)malloc(MAX*sizeof(Posizione*));
    for(int colonna = A; colonna <= MAX; colonna++) {
        s->griglia[colonna-1] = (Posizione *)malloc(MAX*sizeof(Posizione));
    }

    /*inizializzo ogni posizione sulla scacchiera*/
    for(int riga = 1; riga <= MAX; riga++) {
        for(int colonna = A; colonna <= MAX; colonna++) {
            s->griglia[colonna-1][riga-1].riga = riga;
            s->griglia[colonna-1][riga-1].colonna = colonna;
            s->griglia[colonna-1][riga-1].disponibile = 0;
        }
    }

    printf("Scacchiera Inizializzata!\n");

    return s;
}

void stampaScacchiera(Scacchiera * s) {
    printf("Stampo scacchiera numero: %d...\n", ++actual);

    for(int riga = MAX; riga > 0; riga--) {
        printf("%d)\t", riga);

        for(int colonna = A; colonna <= MAX && riga != 0; colonna++) {
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

    for (int i = 1; i <= MAX; i++) {
        printf("%c)\t", 'A' + i-1);
    }
    printf("\n");
    scriviScacchiera(s);
    scriviMathScacchiera(s);
}

void scriviScacchiera(Scacchiera * s) {

    fprintf(fp, "Stampo scacchiera numero: %d...\n", ++actualFile);

    for(int riga = MAX; riga > 0; riga--) {
        fprintf(fp, "%d)\t", riga);

        for(int colonna = A; colonna <= MAX && riga != 0; colonna++) {
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

    for (int i = 1; i <= MAX; i++) {
        fprintf(fp, "%c)\t", 'A'+i-1);
    }
    fprintf(fp, "\n");
}

void scriviMathScacchiera(Scacchiera * s) {

    for(int riga = MAX; riga > 0; riga--) {

        for(int colonna = A; colonna <= MAX && riga != 0; colonna++) {
            if(s->griglia[colonna-1][riga-1].disponibile == 0)
                fprintf(matrix, "0");
            else if(s->griglia[colonna-1][riga-1].disponibile == 1) 
                fprintf(matrix, "1");
            else
                fprintf(matrix, "0");
        }
        
        fprintf(matrix, "\n");
    }
    fprintf(matrix, "\n");
}

void inizializzaRegina(Regina * r, Scacchiera *s, int riga, int colonna) {
    //printf("Inizializzo regina...\n");

    r->posizione = s->griglia[riga-1][colonna-1];
    s->griglia[riga-1][colonna-1].disponibile += 1;

    OccupaCaselle(r, s);

}

void deInizializzaRegina(Regina * r, Scacchiera *s) {
    s->griglia[r->posizione.colonna-1][r->posizione.riga-1].disponibile -= 1;

    deOccupaCaselle(r, s);
}

void OccupaCaselle(Regina * r, Scacchiera *s) {
    //printf("Calcolo righe...\n");

    for(int i = 1; i <= MAX; i++) {
        if(i != r->posizione.colonna)
            s->griglia[i-1][r->posizione.riga-1].disponibile += 2;
    }
    //printf("Calcolo colonne...\n");

    for(int i = 1; i <= MAX; i++) {
        if(i != r->posizione.riga)
            s->griglia[r->posizione.colonna-1][i-1].disponibile += 2;
    }
    //printf("Calcolo K...\n");
    int k = r->posizione.riga - r->posizione.colonna ;

    //printf("k = %d\n", k);

    int x = 1;
    int y = 1;


    while(x <= MAX) {
        y = x + k;
        //printf("x = %d, y = %d\n", x, y);

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
        //printf("x = %d, y = %d\n", x, y);

        if(x != r->posizione.colonna && y != r->posizione.riga && x <= MAX && y<=MAX && x>0 && y>0) {
            s->griglia[x-1][y-1].disponibile += 2;
        }
        x++;
    }

}

void deOccupaCaselle(Regina * r, Scacchiera *s) {
    for(int i = 1; i <= MAX; i++) {
        if(i != r->posizione.colonna)
            s->griglia[i-1][r->posizione.riga-1].disponibile -= 2;
    }

    for(int i = 1; i <= MAX; i++) {
        if(i != r->posizione.riga)
            s->griglia[r->posizione.colonna-1][i-1].disponibile -= 2;
    }

    int k = r->posizione.riga - r->posizione.colonna ;

    //printf("k = %d\n", k);

    int x = 1;
    int y = 1;


    while(y <= MAX) {
        y = x + k;
        //printf("x = %d, y = %d\n", x, y);

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
        //printf("x = %d, y = %d\n", x, y);

        if(x != r->posizione.colonna && y != r->posizione.riga && x <= MAX && y<=MAX && x>0 && y>0) {
            s->griglia[x-1][y-1].disponibile -= 2;
        }
        x++;
    }

}



void piazzaRegineRicorsivo(Regina ** regine, Scacchiera *s, int nRegine) {
    for(int j = 0; j < MAX; j++) {
        if(s->griglia[(regine[MAX - nRegine]->id)-1][j].disponibile == 0) {
            inizializzaRegina(regine[MAX - nRegine],s,regine[MAX - nRegine]->id,j+1);
            if(nRegine > 1) {
                piazzaRegineRicorsivo(regine, s, nRegine-1);
            }
            if(controllaScacchiera(s) && nRegine == 1)
                stampaScacchiera(s);
            deInizializzaRegina(regine[MAX - nRegine],s);
        }
    }
}


bool controllaScacchiera(Scacchiera *s) {
    for(int j = 0; j < MAX; j++) {
        for(int i = 0; i < MAX; i++) {
            if((s->griglia[j][i].disponibile % 2) && s->griglia[j][i].disponibile != 1)
                return false;
        }
    }
    return true;
}
