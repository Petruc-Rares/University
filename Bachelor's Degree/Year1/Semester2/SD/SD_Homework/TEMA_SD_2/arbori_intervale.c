// Copyright 2020 Petruc Rares (rareson168@gmail.com)
#include "arbori_intervale.h"

Nod* creare_nod(int valStanga, int valDreapta) {

    if (valStanga > valDreapta) return NULL;

    Nod* nod = (Nod*)malloc(1 * sizeof(Nod));
    int mijloc;

    // in cazul in care alocarea nu s-a putut realiza

    if (!nod) {
        printf("Nu s-a putut aloca memorie pentru nodul"); 
        printf("cu capetele intervalului: %d %d\n", valStanga, valDreapta);
        exit(1);
    }

    nod->interval = (Interval*) malloc(1 * sizeof(nod->interval));

    if (!nod->interval) {
        // mai intai dezalocam memoria alocata nodului

        free(nod); printf("Nu s-a putut aloca memorie intervalului nodului");
        printf("cu captele: %d %d\n", valStanga, valDreapta); exit(1);

    }

    nod->interval->capat_stanga = valStanga;
    nod->interval->capat_dreapta = valDreapta;
    nod->info = 0;

    if (valStanga < valDreapta) {

        mijloc = (valStanga + valDreapta)/2;
        nod->copil_stanga = creare_nod(valStanga, mijloc);
        nod->copil_dreapta = creare_nod(mijloc + 1, valDreapta);
        
        if ((!nod->copil_stanga) || (!nod->copil_dreapta)) {

        	printf("Nu s-a putut aloca memorie pentru unul din copii\n");
        	free(nod->copil_dreapta);
        	free(nod->copil_stanga);
        	free(nod->interval);
        	free(nod);
        	exit(1);

        }

    }

    return nod;
}

ArboreDeIntervale *construieste_arbore(int capat_stanga, int capat_dreapta,
        int valoare_predifinita_raspuns_copil,
        void (*f_actualizare)(Nod *nod, int v_actualizare),
        int (*f_combinare_raspunsuri_copii)(int raspuns_stanga,
        								    int raspuns_dreapta)) {
    // TODO implementati functia de constructie a arborelui prezentata in enunt

    ArboreDeIntervale* arbore = (ArboreDeIntervale*) malloc(1 *
    sizeof(ArboreDeIntervale));

    if (!arbore) {printf("Nu s-a putut aloca memorie pentru arbore\n");
    			 exit(1);}
    // intializez radacina arborelui

    arbore->radacina = creare_nod(capat_stanga, capat_dreapta);

    // TODO initializati campurile unui ArboreDeIntervale*

    arbore->valoare_predifinita_raspuns_copil = 
  	valoare_predifinita_raspuns_copil;
    arbore->f_actualizare = f_actualizare;
    arbore->f_combinare_raspunsuri_copii = f_combinare_raspunsuri_copii;

    return arbore;
}


void actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    // TODO implementati functia de actualizare pentru un nod

    if (nod == NULL)return;

    int valStanga, valDreapta, mijloc, capatStanga, capatDreapta;

    valStanga = interval->capat_stanga;
    valDreapta = interval->capat_dreapta;
    capatStanga = nod->interval->capat_stanga;
    capatDreapta = nod->interval->capat_dreapta;

    if ((valStanga <= capatStanga) && (capatDreapta <= valDreapta)) {

        arbore->f_actualizare(nod, v_actualizare);

    } else {
        
        mijloc = (capatStanga + capatDreapta)/2;
        if (valStanga <= mijloc) {
            actualizare_interval_pentru_nod(arbore, nod->copil_stanga,
            								interval, v_actualizare);
        }
        
        if (mijloc < valDreapta) {
            actualizare_interval_pentru_nod(arbore, nod->copil_dreapta,
            							    interval, v_actualizare);
        }

        arbore->f_actualizare(nod, v_actualizare);

    }

    return;
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
void actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    actualizare_interval_pentru_nod(arbore, arbore->radacina, interval,
    							   v_actualizare);
}


int interogare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
								 Interval *interval){

    if (nod == NULL)return arbore->valoare_predifinita_raspuns_copil;

    int valStanga, valDreapta, mijloc, capatStanga, capatDreapta;
    int valInterStanga, valInterDreapta;

    valInterStanga = valInterDreapta = 
    arbore->valoare_predifinita_raspuns_copil;

    valStanga = interval->capat_stanga;
    valDreapta = interval->capat_dreapta;
    capatStanga = nod->interval->capat_stanga;
    capatDreapta = nod->interval->capat_dreapta;

    if ((valStanga <= capatStanga) && (capatDreapta <= valDreapta)) {
        return nod->info;
    } else {
        mijloc = (capatStanga + capatDreapta)/2;

        if (valStanga <= mijloc)
            valInterStanga =  interogare_interval_pentru_nod(arbore,
            							 nod->copil_stanga, interval);

        if (mijloc < valDreapta)
            valInterDreapta = interogare_interval_pentru_nod(arbore,
            							 nod->copil_dreapta, interval);

        return arbore->f_combinare_raspunsuri_copii(valInterStanga,
        										    valInterDreapta);
    }
}


// Functia este deja implementata, se cheama functia de mai sus cu radacina arborelui
int interogare_interval_in_arbore(ArboreDeIntervale *arbore,
								 Interval *interval) {
    return interogare_interval_pentru_nod(arbore, arbore->radacina, interval);
}


// ----- DOAR pentru bonus si DOAR daca considerati ca e necesara ----- //
void seteaza_info_in_nod_la_valoare_capat_dreapta(Nod* nod) {
    // TODO cred ca e destul de clar ce vrea sa faca functia asta
}


void seteaza_info_in_arbore_la_valoare_capat_dreapta(ArboreDeIntervale* arbore) {
    seteaza_info_in_nod_la_valoare_capat_dreapta(arbore->radacina);
}
