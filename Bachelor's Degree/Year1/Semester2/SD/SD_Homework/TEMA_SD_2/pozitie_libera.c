#include "pozitie_libera.h"

void actualizare_cu_delta(Nod *nod, int v2) {
    nod->info += v2;
}

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

DateIntrare* citeste_fisier_in(char *nume_fisier_intrare) {
    int nrMaximIn, nrIn, i, aux;
    
    FILE* fp = fopen(nume_fisier_intrare, "r");

    if (fp == NULL) {
        printf("Nu s-a putut deschide fisierul\n");
        exit(1);
    }

    DateIntrare* date = (DateIntrare*)malloc(sizeof(DateIntrare));

    if (!date) {
    	printf("Nu s-a putut aloca memorie pentru datele de intrare\n");
    	fclose(fp);
    	exit(1);
    }

    fscanf(fp, "%d", &nrMaximIn);
    fscanf(fp, "%d", &nrIn);

    date->numarul_maxim_din_intrari = nrMaximIn;
    date->numar_intrari = nrIn;
    date->intrari = (int*)malloc(nrIn*sizeof(int));


    if(!date->intrari) {
    	printf("Nu s-a putut aloca memorie pentru intrari\n");
    	free(date);
    	fclose(fp);
    	exit(1);
    }

    for (i = 0; i < nrIn; i++) {
    	fscanf(fp, "%d", &aux);
    	date->intrari[i] = aux;
    }

    return date;
}

int conditieSmechera(Nod* nod) {
	if (nod->info < (nod->interval->capat_dreapta - nod->interval->capat_stanga + 1)) return 1;
	return 0;
}

void pseudo_actualizare_interval_pentru_nod(ArboreDeIntervale *arbore, Nod *nod,
                                        Interval *interval, int v_actualizare) {
    // TODO implementati functia de actualizare pentru un nod

    if (nod == NULL)return;
    // punem in informatia din radacina arborelui pozitia din vectorul
    // T unde vom introduce valoarea coresp
    if ((nod->copil_stanga == NULL) && (nod->copil_dreapta == NULL)) {
    	nod->info++;
    	arbore->radacina->info = nod->interval->capat_stanga-1;
    	return;
    } else arbore->radacina->info = -1;

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
        	if (conditieSmechera(nod->copil_stanga))
            pseudo_actualizare_interval_pentru_nod(arbore, nod->copil_stanga,
            								interval, v_actualizare);
        // daca nu mai e loc nicaieri in stanga sterg ultimele
        // actualizari
        	else pseudo_actualizare_interval_pentru_nod(arbore, nod->copil_stanga,
        	 interval, -v_actualizare);
        }
        
        if (mijloc < valDreapta) {
        	if (conditieSmechera(nod->copil_dreapta))
            {pseudo_actualizare_interval_pentru_nod(arbore, nod->copil_dreapta,
            							    interval, v_actualizare);}//printf("HAHA\n");}
        	else {pseudo_actualizare_interval_pentru_nod(arbore, nod->copil_stanga,
            							    interval, v_actualizare); }//printf("HAHA2\n");}
        }

        arbore->f_actualizare(nod, v_actualizare);

    }

    return;
}

void pseudo_actualizare_interval_in_arbore(ArboreDeIntervale *arbore, 
                                        Interval *interval, int v_actualizare) {
    pseudo_actualizare_interval_pentru_nod(arbore, arbore->radacina, interval,
    							   v_actualizare);
}

int* calculeaza_vector_raspuns(DateIntrare *date_intrare) {
    if (!date_intrare) return NULL;
    int i, aux1, aux2;

    int*T = (int*)malloc((date_intrare->numarul_maxim_din_intrari+1)*sizeof(int));
        if (!T) {
    	printf("Alocare esuata in functia calculeaza_vector_raspuns\n");
    	exit(1);
    }

    ArboreDeIntervale *arbore = construieste_arbore(0, date_intrare->numarul_maxim_din_intrari, 0, actualizare_cu_delta, suma_raspunsurilor);


    	for (i = 0; i < date_intrare->numar_intrari; i++) {
    		if (conditieSmechera(arbore->radacina) == 0) break;
    		Interval *interval = construct_interval(date_intrare->intrari[i], date_intrare->intrari[i]);
    		pseudo_actualizare_interval_in_arbore(arbore, interval, 1);
    		if (arbore->radacina->info != -1) T[arbore->radacina->info] = date_intrare->intrari[i];

    		Interval* iaux1 = construct_interval(0, (arbore->radacina->interval->capat_dreapta)/2);
    		Interval* iaux2 = construct_interval((arbore->radacina->interval->capat_dreapta)/2 + 1, (arbore->radacina->interval->capat_dreapta));
    		
    		aux1 = interogare_interval_in_arbore(arbore, iaux1);
    		aux2 = interogare_interval_in_arbore(arbore, iaux2);
    		arbore->radacina->info = aux1 + aux2;

    		free(iaux1);
    		free(iaux2);
    		free(interval);
    	}

    return T;

}


int* calculeaza_vector_raspuns_trivial(DateIntrare *date_intrare) {
    if (!date_intrare) return NULL;

    int*T = (int*)malloc((date_intrare->numarul_maxim_din_intrari+1)* sizeof(int));
    if (!T) {
    	printf("Alocare esuata in functia calculeaza_vector_raspuns_trivial\n");
    	exit(1);
    }
    int i, j;

    // consideram -1 ca insemnand o pozitie care nu a fost ocupata
    for (i = 0; i < date_intrare->numarul_maxim_din_intrari+1; i++) {
    	T[i] = -1;
    }

    for (i = 0; i < date_intrare->numar_intrari; i++) {

    	if (T[date_intrare->intrari[i]] == -1) T[date_intrare->intrari[i]] = date_intrare->intrari[i];
    	else {
    		for (j = date_intrare->intrari[i]-1; j >=0; j--) {
    			if (T[j] == -1) {T[j] = date_intrare->intrari[i]; break;}
    		}
    	}
    }

    return T;
}