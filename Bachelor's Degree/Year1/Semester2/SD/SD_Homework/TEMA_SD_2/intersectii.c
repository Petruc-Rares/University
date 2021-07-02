// Copyright 2020 Petruc Rares (rareson168@gmail.com)
#include "intersectii.h"


// functie ajutatoare
void afiseaza_lista_intervale(ListaIntervale2D *lista) {
    int i;
    for (i = 0; i < lista->dimensiune; i++) {
        Interval2D *interv = lista->intervale[i];
        printf("{punct stanga x: %d, punct stanga y: %d, punct dreapta x: %d, punct dreapta y: %d}\n",
                    interv->punct_stanga->x, interv->punct_stanga->y,
                    interv->punct_dreapta->x, interv->punct_dreapta->y);
    }
}

Interval2D** creeazaListaIntervale(ListaIntervale2D* lista, FILE* fp) {

    int dimensiune = lista->dimensiune;

    int i;
    int cx1, cy1, cx2, cy2;

    for (i = 0; i < dimensiune; i++) {

        lista->intervale[i] = (Interval2D*) malloc(1 * sizeof(Interval2D));

        if (!lista->intervale[i]) {

            printf("Nu s-a putut aloca memorie pentru");
            printf("intervalul %d", i);

            for(int j = 0; j <= i; j++) {

                free(lista->intervale[j]);

            }

            free(lista);
            fclose(fp);
            exit(1);

        }

        lista->intervale[i]->punct_stanga = (Punct2D*)malloc(1 * sizeof(Punct2D));
        lista->intervale[i]->punct_dreapta = (Punct2D*)malloc(1 * sizeof(Punct2D));

        fscanf(fp, "%d %d %d %d", &cx1, &cy1, &cx2, &cy2);
        lista->intervale[i]->punct_stanga->x =  cx1;
        lista->intervale[i]->punct_stanga->y =  cy1;
        lista->intervale[i]->punct_dreapta->x =  cx2;
        lista->intervale[i]->punct_dreapta->y =  cy2;


        if ((!lista->intervale[i]->punct_stanga) ||
            (!lista->intervale[i]->punct_dreapta)) {

            printf("Nu s-a putut aloca memorie pentru unul din punctele");
            printf(" corespunzatoare listei si intervalului %d\n", i);

            for (int j = 0; j <= i; j++) {

                free(lista->intervale[j]->punct_stanga);
                free(lista->intervale[j]->punct_dreapta);

            }

            for (int j = 0; j < dimensiune; j++) {

                free(lista->intervale[j]);

            }

            free(lista);
            fclose(fp);
            exit(1);

        }

    }
    fclose(fp);

    return lista->intervale;
}

ListaIntervale2D* citeste_intrare(char *nume_fisier_intrare) {

    int dimensiune;
    
    FILE* fp = fopen(nume_fisier_intrare, "r");

    if (fp == NULL) {
        printf("Nu s-a putut deschide fisierul\n");
        exit(1);
    }

    ListaIntervale2D* lista = (ListaIntervale2D*)malloc(sizeof(ListaIntervale2D));

    if (lista == NULL) {

        printf("Nu s-a putut aloca memorie pentru lista\n");
        fclose(fp);
        exit(1);

    }

    fscanf(fp, "%d", &dimensiune);

    lista->dimensiune = dimensiune;

    lista->intervale = (Interval2D**)malloc(dimensiune*sizeof(Interval2D*));

    if (!lista->intervale) {

        printf("Nu s-a putut aloca memorie pentru intervale\n");
        free(lista);
        fclose(fp);
        exit(1);

    }

    lista->intervale = creeazaListaIntervale(lista, fp);

    return lista;
}

// ! Functie pentru actualizarea in arbore
// ! O veti da ca parametru cand initializati arborele
void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

// ! Functie pentru combinarea raspunsurilor in arbore
// ! O veti da ca parametru cand initializati arborele
int suma_raspunsurilor(int r1, int r2) {
    return r1 + r2;
}

Interval* construct_interval(int st, int dr) {
    Interval *interval = malloc(sizeof(Interval));
    if (interval == NULL) {
        exit(1);
    }
    interval->capat_stanga = st;
    interval->capat_dreapta = dr;
    return interval;
}

typedef struct PointInterval{

    int x;
    int y;

    // 'used' ne specifica daca segmentul nostru s-a incheiat sau nu
    // are valoarea 1 daca s-a incheiat
    // are valoarea 0 daca nu s-a incheiat

    int used;
    Interval2D* interval;

}PointInterval;

int comparator(const void *p, const void *q) 
{ 
    // Get the values at given addresses 
    const int l = ((PointInterval*)p)->x; 
    const int r = ((PointInterval*)q)->x; 

    return l - r; 
}

int isvertical(Interval2D* interval) {

    return (interval->punct_stanga->x == interval->punct_dreapta->x);

}

int getMaxY(ListaIntervale2D* lista) {

    int y_max = lista->intervale[0]->punct_dreapta->y;

    for (int i = 0; i < lista->dimensiune; i++) {

        if (lista->intervale[i]->punct_dreapta->y > y_max)
            y_max = lista->intervale[i]->punct_dreapta->y;

    }

    return y_max;

}

int calculeaza_numar_intersectii(ListaIntervale2D *lista) {

    Interval *in = NULL;

    int contor = 0;

    int y_max = getMaxY(lista);

    ArboreDeIntervale* arbore = construieste_arbore(0, y_max, 0,
                         actualizare_cu_delta, suma_raspunsurilor);

    PointInterval auxStruct[lista->dimensiune], helperStruct;

    for (int i = 0; i < lista->dimensiune; i++) {

        auxStruct[i].x = lista->intervale[i]->punct_stanga->x;
        auxStruct[i].y = lista->intervale[i]->punct_stanga->y;
        auxStruct[i].interval = lista->intervale[i];
        auxStruct[i].used = 0;

    }

    qsort(auxStruct, sizeof(auxStruct)/sizeof(auxStruct[0]),
                     sizeof(auxStruct[0]), comparator);

   	for (int i = 0; i < lista->dimensiune; i++){

   		// corner case (segment vertical urmat de segment orizontal cu o intersectie)

   		if (i + 1 < lista->dimensiune) {

   			if ((auxStruct[i].interval->punct_stanga->x == 
   				auxStruct[i+1].interval->punct_stanga->x) &&
   			 	(isvertical(auxStruct[i].interval) == 1) &&
   			  	(isvertical(auxStruct[i+1].interval) == 0)) {

   				// mai intai dorim sa actualizam
                // si apoi sa interogam

   				helperStruct = auxStruct[i];
   				auxStruct[i] = auxStruct[i+1];
   				auxStruct[i+1] = helperStruct;

   			}
   		}

   		if (isvertical(auxStruct[i].interval) == 1) {

   			for (int j = 0; j < i; j++) {

   				if ((auxStruct[j].used == 0) && (isvertical(auxStruct[j].interval) == 0)
                && (auxStruct[j].interval->punct_dreapta->x < auxStruct[i].interval->punct_dreapta->x)) {

   					auxStruct[j].used = 1;
            		in = construct_interval(auxStruct[j].y, auxStruct[j].y);

        	    	actualizare_interval_in_arbore(arbore, in, -1);

   		        	free(in);

   				}


   			}


            in = construct_interval(auxStruct[i].interval->punct_stanga->y,
                                   auxStruct[i].interval->punct_dreapta->y);
            contor += interogare_interval_in_arbore(arbore, in);
            free(in);

   		} else {

   			in = construct_interval(auxStruct[i].y, auxStruct[i].y);
            actualizare_interval_in_arbore(arbore, in, 1);
            free(in);
   		}

   	}

    return contor;

}

void interschimbare(int *x, int *y) {

    int aux = *x;
    *x = *y;
    *y = aux;

}


int calculeaza_numar_intersectii_trivial(ListaIntervale2D *lista) {

    int dimensiune, contor = 0;

    // coordonata (absicsa/ordonata)(stanga/dreapta)(nr.punct)
    int cxs1, cys1, cxd1, cyd1;
    int cxs2, cys2, cxd2, cyd2;

    dimensiune = lista->dimensiune;

    for (int i = 0; i < dimensiune; i++) {

        cxs1 = lista->intervale[i]->punct_stanga->x;
        cys1 = lista->intervale[i]->punct_stanga->y;
        cxd1 = lista->intervale[i]->punct_dreapta->x;
        cyd1 = lista->intervale[i]->punct_dreapta->y;

        for (int j = i + 1; j < dimensiune; j++) {

            int auxcxs1 = cxs1, auxcys1 = cys1;
            int auxcxd1 = cxd1, auxcyd1 = cyd1;

            cxs2 = lista->intervale[j]->punct_stanga->x;
            cys2 = lista->intervale[j]->punct_stanga->y;
            cxd2 = lista->intervale[j]->punct_dreapta->x;
            cyd2 = lista->intervale[j]->punct_dreapta->y;            

            // vreau sa compar coordonatele unui segment
            // vertical cu cele ale unui segment orizontal


            // daca ambele segmente sunt verticale
            // sau amele sunt orizontale trec la urmatorul segment

            if (((cxs1 == cxd1) && (cxs2 == cxd2)) ||
                 ((cys1 == cyd1) && (cys2 == cyd2))) continue;

            // daca segmentul i este vertical
            // nu fac nimic
            else if (cxs1 == cxd1);

            // daca segmentul j este vertical
            // interschimb coordonatele celor
            // doua segmente pentru a trata un caz unic

            else if (cxs2 == cxd2){

                interschimbare(&auxcxs1, &cxs2);
                interschimbare(&auxcys1, &cys2);
                interschimbare(&auxcxd1, &cxd2);
                interschimbare(&auxcyd1, &cyd2);
 
            }

            // ajuns aici am coordonatele punctului corespunzatoare
            // unui segment vertical si coordonatele punctului doi
            // unui segment orizontal
            // ramane de verificat daca cele doua segmente se si
            // intersecteaza

            if ((auxcxs1 >= cxs2) && (auxcxs1 <= cxd2)
             && (auxcys1 <= cys2) && (cys2 <= auxcyd1))contor++;


        }

    }

    return contor;
}
