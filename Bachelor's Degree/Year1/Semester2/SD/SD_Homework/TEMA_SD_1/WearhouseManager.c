#include <stdio.h>
#include "WearhouseManager.h"


Package *create_package(long priority, const char* destination){
	Package* package = (Package*) malloc(1 * sizeof(Package));
	if (!package) {
		printf("Nu s-a putut aloca memorie pentru pachet");
		exit(1);
	}
	package->priority = priority;
	if (destination != NULL) {
		package->destination = strdup(destination);
	} else {
		destination = NULL;
	}
	return package;
}

void destroy_package(Package* package){
	if (!package) return;
	free(package->destination);
	free(package);
	return;
}

Manifest* create_manifest_node(void){
	Manifest* manifest = (Manifest*) malloc(1 * sizeof(Manifest));
	if (!manifest) {
		printf("Nu s-a putut aloca memorie pentru manifest");
		exit(1);
	}
	manifest->package = NULL;
	manifest->next = manifest->prev = NULL;
	return manifest;
}

void destroy_manifest_node(Manifest* manifest_node){
	if (!manifest_node) return;
	if (manifest_node->package) {
		free(manifest_node->package);
	}
	// daca este singurul nod
	if ((manifest_node->prev == NULL) && (manifest_node->next == NULL)) {
		free(manifest_node);
		return;
	} // daca este primul nod
	 else if (manifest_node->prev == NULL) {
	 	(manifest_node)->next->prev = NULL;
	 	free(manifest_node);
	 	return;
	 } // daca este ultimul nod
	 else if (manifest_node->next == NULL) {
	 	(manifest_node->prev)->next = NULL;
	 	free(manifest_node);
	 	return;
	 } // general case (la mijloc) 
	 else {
	 	(manifest_node->prev)->next = manifest_node->next;
	 	(manifest_node->next)->prev = manifest_node->prev;
	 	free(manifest_node);
	 	return;
	 }
	 // nu exista noduri;
	 free(manifest_node);
}

Wearhouse* create_wearhouse(long capacity){
	if (capacity <= 0)return NULL;
	Wearhouse* wearhouse = (Wearhouse*) malloc(1 * sizeof(Wearhouse));
	if (!wearhouse) {
		printf("Nu s-a putut aloca memorie pentru depozit");
		exit(1);
	}
	wearhouse->packages = (Package**)malloc(capacity * sizeof(Package*));
	if (!wearhouse->packages) {
		free(wearhouse);
		printf("Nu s-a putut aloca memorie pentru pachete");
		exit(1);
	}
	wearhouse->size = 0;
	wearhouse->capacity = capacity;
	return wearhouse;
}

Wearhouse *open_wearhouse(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Wearhouse *w = NULL;


	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	if((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		w = create_wearhouse(atol(token));

		free(line);
		line = NULL;
		len = 0;
	}

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		long priority = atol(token);
		token = strtok(NULL, ",\n ");
		Package *p = create_package(priority, token);
		w->packages[w->size++] = p;

		free(line);
		line = NULL;
		len = 0;
	}

	free(line);


	fclose(fp);
	return w;

	file_open_exception:
	return NULL;
}

long wearhouse_is_empty(Wearhouse *w){
	if (!w) {
		return 0;
	}
	if (w->size == 0) return 1;
	return 0;
}

long wearhouse_is_full(Wearhouse *w){
	if (!w) {
		return 0;
	}
	if (w->size == w->capacity) return 1;
	return 0;
}

long wearhouse_max_package_priority(Wearhouse *w){
	long size = w->size, i = 0, max = -1;
	for (i = 0; i < size; i++) {
		if (((w->packages[i])->priority) > max) {
			max = (w->packages[i])->priority;
		}
	}
	return max;
}

long wearhouse_min_package_priority(Wearhouse *w){
	long size = w->size, i, min = 100;
	for (i = 0; i < size; i++) {
		if (((w->packages[i])->priority) < min) {
			min = (w->packages[i])->priority;
		}
	}
	return min;
}


void wearhouse_print_packages_info(Wearhouse *w){
	for(long i = 0; i < w->size; i++){
		printf("P: %ld %s\n",
				w->packages[i]->priority,
				w->packages[i]->destination);
	}
	printf("\n");
}

void destroy_wearhouse(Wearhouse* wearhouse){
	long size = wearhouse->size, i;
	for (i = 0; i < size; i++) {
		destroy_package(wearhouse->packages[i]);
	}
	free(wearhouse);
}


Robot* create_robot(long capacity){
	Robot* robot = (Robot*) malloc(1 * sizeof(Robot));
	if (!robot) {
		printf("Nu s-a putut aloca memorie pentru robot");
		exit(1);
	}
	robot->manifest = NULL;
	robot->size = 0;
	robot->capacity = capacity;
	robot->next = NULL;
	return robot;
}

int robot_is_full(Robot* robot){
	if (!robot) {
		return 0;
	}
	if (robot->size == robot->capacity) {
		return 1;
	}
	return 0;
}

int robot_is_empty(Robot* robot){
	if (!robot) {
		return 0;
	}
	if (robot->size == 0) return 1;
	return 0;
}

Package* robot_get_wearhouse_priority_package(Wearhouse *w, long priority){
	if (priority < 0) return NULL;
	long i = 0, size = w->size;
	for (i = 0; i < size; i++) {
		if ((w->packages[i])->priority == priority) {
			return w->packages[i];
		}
	}
	// se ajunge aici daca nu s-a gasit vreun pachet cu prioritatea dorita
	return NULL;
}

void robot_remove_wearhouse_package(Wearhouse *w, Package* package){
	long contor = 0;
	while(w->packages[contor] != package) {
		contor++;
	}
	// consideram ca prioritatea este -1 pentru un pachet sters
	w->packages[contor]->priority = -1;
	for (long j = contor; j < w->size - 1; j++) {
		w->packages[j] = w->packages[j+1];
	}
	w->size--;
	//Package* aux = w->packages[0];
	//for (i = 0; i < size; i++) {
	//	if (w->packages[i] == package) {
	//		break;
	//	}
	//}
	// daca nu s-a gasit pachetul i-ul devine size
	//if (i == size) return;
	//for (long j = i; j < size - 1; j++) {
	//	w->packages[j] =  w->packages[j + 1];
	//}
	//w->packages[size - 1] = aux;
	//w->size--;
	return;
}

void robot_load_one_package(Robot* robot, Package* package){
	if(robot_is_full(robot)) {
		return ;
	}
	if (robot_is_empty(robot)) {
		robot->manifest = malloc(1 * sizeof(Manifest));
		if (!robot->manifest) {
			printf("Nu s-a putut aloca memorie pentru itinerariul robotului");
			exit(1);
		}	
		robot->manifest->package = package;
		robot->manifest->prev = NULL;
		robot->manifest->next = NULL;
		robot->size = 1;
		return;
	}
	// aici se ajunge daca robotul mai are capacitate
	robot->size++;
	// verificam daca mai exista vreun pachet cu aceeasi prioritate
	// ca a acelui trimis ca parametru
	Manifest* man = robot->manifest, *prev = NULL;
	Manifest *newManifest = create_manifest_node();
	newManifest->package = package;
	long priority = package->priority;
	while ((man != NULL) && (priority < man->package->priority)) {
		prev = man;
		man = man->next;
	}
	// in bucla se ajunge daca prioritatea de la momentul man
	// e mai amre sau egala cu cea cautata
	/*if (man != NULL) { 
		while ((priority == man->package->priority) &&
		 	   (strcmp(package->destination,
		 	    man->package->destination) > 0)) {
			man = man->next;
		}
	}*/
	if (man != NULL) {
			while ((man != NULL) && 
			   (priority == man->package->priority) &&
		 	   (strcmp(package->destination,
		 	    man->package->destination) > 0)) {
		 	prev = man;
			man = man->next;
		}
	}
	// daca se ajunge la capatul itinerariului
	if (man == NULL) {
		prev->next = newManifest;
		newManifest->prev = prev;
	}  // daca pachetul trebuie introdus la inceputul listei
	else if (prev == NULL) {
		man->prev = newManifest;
		newManifest->next = man;
		robot->manifest = newManifest;
	}  // default case 
	else {
		prev->next = newManifest;
		man->prev = newManifest;
		newManifest->prev = prev;
		newManifest->next = man;
	}
	return ;
}

long robot_load_packages(Wearhouse* wearhouse, Robot* robot){
	// se cauta pachetul cu cea mai mare prioritate
	// cat timp robotul nu e plin
	if (!robot) return 0;
	Package* packet = NULL, *copyPacket = NULL;
	long index;
	while (!robot_is_full(robot)) {
		index = wearhouse_max_package_priority(wearhouse);
		packet = robot_get_wearhouse_priority_package(wearhouse, index);
		copyPacket = create_package(packet->priority, packet->destination);
		robot_load_one_package(robot, copyPacket);
		robot_remove_wearhouse_package(wearhouse, packet);
	}
	return robot->size;
}

Package* robot_get_destination_highest_priority_package(Robot* robot, const char* destination){
	if (!robot) return NULL;
	Package* package = NULL;
	long maxpriority = -1;
	while (robot->manifest != NULL) {
		if (robot->manifest->package->priority > maxpriority) {
			maxpriority = robot->manifest->package->priority;
			package = robot->manifest->package;
		}
		robot->manifest = robot->manifest->next;
	}
	return package;
}

void destroy_robot(Robot* robot){
	Manifest* man = robot->manifest, *aux;
	while (man) {
		aux = man;
		man = man->next;
		free(aux->package->destination);
		free(aux->package);
		free(aux);
	}
	free(robot);
}

void robot_unload_manifest(Robot* robot, Manifest* man) {
	// nu e gata asta
	robot->manifest = robot->manifest->next;
	robot->manifest->prev = NULL;
	//free(man);
	return;
}

void robot_unload_packages(Truck* truck, Robot* robot){
	if ((!truck) || (!robot) || (!robot->manifest)) {
		return;
	}
	Manifest* man = NULL, *aux = NULL, *prev = NULL, *newManifestNode = create_manifest_node();
	man = robot->manifest;
	while(man) {
		// se verifica conditia doar daca pachetul robotului
		// are aceeasi destinatie cu a tirului
		if (strcmp(man->package->destination,
		 	truck->destination) == 0) {
			// daca tirul e plin se iese din bucla
			if (truck_is_full(truck) == 0) {
				// + verificare prioritate
				newManifestNode = create_manifest_node();
				newManifestNode->package = man->package;
				robot_unload_manifest(robot, man);
				//free(man);
				truck->size++;
				robot->size--;
				if (truck->size == 1) {
					truck->manifest = (Manifest*) malloc(1 * sizeof(Manifest));
					if (!truck->manifest) {
						printf("Nu s-a putut aloca memorie pentru itinerariul tirului");
						exit(1);
					}
					truck->manifest = newManifestNode;
				} else {
					aux = truck->manifest;
					while (aux != NULL) {
						prev = aux;
						aux = aux->next;
					}
					prev->next = newManifestNode;
					newManifestNode->prev = prev;
				}
			} else {
				break;
			}
		}
		man = man->next;
	}
	return;
}




// Attach to specific truck
int robot_attach_find_truck(Robot* robot, Parkinglot *parkinglot){
	int found_truck = 0;
	long size = 0;
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	Manifest* m_iterator = robot->manifest;


	while(m_iterator != NULL){
		while(arrived_iterator != parkinglot->arrived_trucks){
			size  = truck_destination_robots_unloading_size(arrived_iterator);
			if(strncmp(m_iterator->package->destination, arrived_iterator->destination, MAX_DESTINATION_NAME_LEN) == 0 &&
					size < (arrived_iterator->capacity-arrived_iterator->size)){
				found_truck = 1;
				break;
			}

			arrived_iterator = arrived_iterator->next;
		}

		if(found_truck)
			break;
		m_iterator = m_iterator->next;
	}

	if(found_truck == 0)
		return 0;


	Robot* prevr_iterator = NULL;
	Robot* r_iterator = arrived_iterator->unloading_robots;
	while(r_iterator != NULL){
		Package *pkg = robot_get_destination_highest_priority_package(r_iterator, m_iterator->package->destination);
		if(m_iterator->package->priority >= pkg->priority)
			break;
		prevr_iterator = r_iterator;
		r_iterator = r_iterator->next;
	}

	robot->next = r_iterator;
	if(prevr_iterator == NULL)
		arrived_iterator->unloading_robots = robot;
	else
		prevr_iterator->next = robot;

	return 1;
}

void robot_print_manifest_info(Robot* robot){
	Manifest *iterator = robot->manifest;
	while(iterator != NULL){
		printf(" R->P: %s %ld\n", iterator->package->destination, iterator->package->priority);
		iterator = iterator->next;
	}

	printf("\n");
}



Truck* create_truck(const char* destination, long capacity, long transit_time, long departure_time){
	if (capacity < 0) return NULL;
	Truck* truck = (Truck*) malloc(1 * sizeof(Truck));
	if (!truck) {
		printf("Nu s-a putut aloca memorie pentru tir");
		return NULL;
	}
	truck->manifest = NULL;
	truck->unloading_robots = NULL;
	if (destination != NULL) {
		truck->destination = strdup(destination);
	} else {
		truck->destination = NULL;
	}
	truck->size = 0;
	truck->capacity = capacity;
	truck->in_transit_time = 0;
	truck->transit_end_time = transit_time;
	truck->departure_time = departure_time;
	truck->next = NULL;
	return truck;
}

int truck_is_full(Truck *truck){
	if (!truck) return 0;
	if (truck->capacity == truck->size) {
		return 1;
	}
	return 0;
}

int truck_is_empty(Truck *truck){
	if (!truck) return 0;
	if (truck->size == 0) {
		return 1;
	}
	return 0;
}

long truck_destination_robots_unloading_size(Truck* truck){
	long noPackets = 0, i = 0;
	Robot *robot = truck->unloading_robots;
	Manifest* manifest = NULL;
	while (robot) {
		manifest = robot->manifest;
		while (manifest) {
			if (strcmp(manifest->package->destination, truck->destination)==0) {
				noPackets = robot->size;
				break;
			}
			manifest = manifest->next;
		i++;
		}
		robot = robot->next;
	}
	return noPackets;
}


void truck_print_info(Truck* truck){
	printf("T: %s %ld %ld %ld %ld %ld\n", truck->destination, truck->size, truck->capacity,
			truck->in_transit_time, truck->transit_end_time, truck->departure_time);

	Manifest* m_iterator = truck->manifest;
	while(m_iterator != NULL){
		printf(" T->P: %s %ld\n", m_iterator->package->destination, m_iterator->package->priority);
		m_iterator = m_iterator->next;
	}

	Robot* r_iterator = truck->unloading_robots;
	while(r_iterator != NULL){
		printf(" T->R: %ld %ld\n", r_iterator->size, r_iterator->capacity);
		robot_print_manifest_info(r_iterator);
		r_iterator = r_iterator->next;
	}
}


void destroy_truck(Truck* truck){
	if (!truck) return;
	Manifest *man = truck->manifest, *aux = NULL;
	while (man) {
		aux = man;
		man = man->next;
		free(aux->package);
		free(aux);
	}
	Robot* robot = truck->unloading_robots, *aux1 = NULL;
	while(robot) {
		aux1 = robot;
		robot = robot->next;
		free(aux1->manifest);
		free(aux1);
	}
	free(truck);


}


Parkinglot* create_parkinglot(void){
	Parkinglot* parkinglot = (Parkinglot*) malloc(sizeof(Parkinglot));
	if (!parkinglot) {
		printf("Nu s-a putut aloca memorie pentru parcare");
		exit(1);
	}

	parkinglot->arrived_trucks = (Truck*)malloc(sizeof(Truck));
	parkinglot->arrived_trucks->destination = NULL;
	parkinglot->arrived_trucks->next = parkinglot->arrived_trucks;

	parkinglot->departed_trucks = (Truck*)malloc(sizeof(Truck));
	parkinglot->departed_trucks->next = parkinglot->departed_trucks;

	parkinglot->pending_robots = (Robot*)malloc(sizeof(Robot));
	parkinglot->pending_robots->next = parkinglot->pending_robots;
	parkinglot->pending_robots->size = -1;
	parkinglot->pending_robots->capacity = -1;

	parkinglot->standby_robots = (Robot*)malloc(sizeof(Robot));
	parkinglot->standby_robots->next = parkinglot->standby_robots;
	parkinglot->standby_robots->size = -1;
	parkinglot->standby_robots->capacity = -1;

	return parkinglot;
}

Parkinglot* open_parckinglot(const char* file_path){
	ssize_t read_size;
	char* line = NULL;
	size_t len = 0;
	char* token = NULL;
	Parkinglot *parkinglot = create_parkinglot();

	FILE *fp = fopen(file_path, "r");
	if(fp == NULL)
		goto file_open_exception;

	while((read_size = getline(&line, &len, fp)) != -1){
		token = strtok(line, ",\n ");
		// destination, capacitym transit_time, departure_time, arrived
		if(token[0] == 'T'){
			token = strtok(NULL, ",\n ");
			char *destination = token;

			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			token = strtok(NULL, ",\n ");
			long transit_time = atol(token);

			token = strtok(NULL, ",\n ");
			long departure_time = atol(token);

			token = strtok(NULL, ",\n ");
			int arrived = atoi(token);

			Truck *truck = create_truck(destination, capacity, transit_time, departure_time);

			if(arrived)
				truck_arrived(parkinglot, truck);
			else
				truck_departed(parkinglot, truck);

		}else if(token[0] == 'R'){
			token = strtok(NULL, ",\n ");
			long capacity = atol(token);

			Robot *robot = create_robot(capacity);
			parkinglot_add_robot(parkinglot, robot);

		}

		free(line);
		line = NULL;
		len = 0;
	}
	free(line);

	fclose(fp);
	return parkinglot;

	file_open_exception:
	return NULL;
}

void parkinglot_add_robot(Parkinglot* parkinglot, Robot *robot){
	if (robot_is_empty(robot) == 1) {
		Robot* standby = parkinglot->standby_robots->next, *prev = parkinglot->standby_robots;
		// caz in care lista contine doar santinela
		if (standby == standby->next) {
			prev->next = robot;
			robot->next = prev;
			return;
		}
		while ((standby != parkinglot->standby_robots) && (standby->capacity > robot->capacity)) {
			prev = standby;
			standby = standby->next;
		}
		prev->next = robot;
		robot->next = standby;
		return;
	} else {
		Robot* pending = parkinglot->pending_robots->next, *prev = parkinglot->pending_robots;
		// caz in care lista contine doar santinela
		if (pending == pending->next) {
			pending->next = robot;
			robot->next = pending;
			return;
		}
		while ((pending != parkinglot->pending_robots) && (pending->size > robot->size)) {
			prev = pending;
			pending = pending->next;
		}
		prev->next = robot;
		robot->next = pending;
		return;
	}
}

void parkinglot_remove_robot(Parkinglot *parkinglot, Robot* robot){
	if (robot_is_empty(robot) == 1) {
		Robot* standby = parkinglot->standby_robots->next, *prev = parkinglot->standby_robots;
		// caz in care lista contine doar santinela
		if (standby == standby->next) {
			standby->next = robot;
			robot->next = standby;
			return;
		}
		while ((standby != parkinglot->standby_robots) && (standby != robot)) {
			prev = standby;
			standby = standby->next;
		}
		prev->next = standby->next;
		free(standby);
		return;
	} else {
		Robot* pending = parkinglot->pending_robots->next, *prev = parkinglot->pending_robots;
		// caz in care lista contine doar santinela
		if (pending == pending->next) {
			pending->next = robot;
			robot->next = pending;
			return;
		}
		while ((pending != parkinglot->pending_robots) && (pending->size > robot->size)) {
			prev = pending;
			pending = pending->next;
		}
		prev->next = pending->next;
		free(pending);
		return;
	}
}

int parckinglot_are_robots_peding(Parkinglot* parkinglot){
	if(parkinglot->pending_robots == parkinglot->pending_robots->next) return 0;
	return 1;
}

int parkinglot_are_arrived_trucks_empty(Parkinglot* parkinglot){
	Truck* truck = parkinglot->arrived_trucks, *p = truck->next;
	while (p != truck) {
		if (p->size != 0) return 0;
		p = p->next;
	}
	return 1;
}


int parkinglot_are_trucks_in_transit(Parkinglot* parkinglot){
	if (parkinglot->departed_trucks == parkinglot->departed_trucks->next) return 0;
	return 1;
}


void destroy_parkinglot(Parkinglot* parkinglot){
	Truck* p = parkinglot->arrived_trucks->next, *p1 = parkinglot->departed_trucks->next, *aux = NULL;
	Robot* p2 = parkinglot->pending_robots, *p3 = parkinglot->standby_robots, *aux1 = NULL;
	while (p != parkinglot->arrived_trucks) {
		aux = p;
		p = p->next;
		free(aux);
	}
	while (p1 != parkinglot->departed_trucks) {
		aux = p1;
		p1 = p1->next;
		free(aux);
	}
	while (p2 != parkinglot->pending_robots) {
		aux1 = p2;
		p2 = p2->next;
		free(aux1);
	}
	while (p3 != parkinglot->standby_robots) {
		aux1 = p3;
		p3 = p3->next;
		free(aux1);
	}
		// free la santinele

	free(parkinglot->arrived_trucks);
	free(parkinglot->departed_trucks);
	free(parkinglot->pending_robots);
	free(parkinglot->standby_robots);

	free(parkinglot);
	return ;
}

void parkinglot_print_arrived_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->arrived_trucks->next;
	while(iterator != parkinglot->arrived_trucks){

		truck_print_info(iterator);
		iterator = iterator->next;
	}

	printf("\n");

}

void parkinglot_print_departed_trucks(Parkinglot* parkinglot){
	Truck *iterator = parkinglot->departed_trucks->next;
	while(iterator != parkinglot->departed_trucks){
		truck_print_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_pending_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->pending_robots->next;
	while(iterator != parkinglot->pending_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}

void parkinglot_print_standby_robots(Parkinglot* parkinglot){
	Robot *iterator = parkinglot->standby_robots->next;
	while(iterator != parkinglot->standby_robots){
		printf("R: %ld %ld\n", iterator->size, iterator->capacity);
		robot_print_manifest_info(iterator);
		iterator = iterator->next;
	}
	printf("\n");

}


void truck_departed(Parkinglot *parkinglot, Truck* truck){
	if (parkinglot_are_trucks_in_transit(parkinglot) == 1 ||
	 (parkinglot_are_arrived_trucks_empty(parkinglot) == 1)) {
		Truck* aux = parkinglot->arrived_trucks->next,* prev = parkinglot->arrived_trucks;
		//prev->next = aux->next; return;
		// se verifica daca tirul aflat in departure
		// este si in arrived
		while (aux != parkinglot->arrived_trucks) {
			if (aux == truck) {
				prev->next = aux->next;
				//aux->next->next = parkinglot->arrived_trucks;
				break;
			}
			prev = aux;
			aux = aux->next;
		}
	}
	Truck* aux = parkinglot->departed_trucks->next, *prev = parkinglot->departed_trucks;
	while ((aux != parkinglot->departed_trucks) &&
		   (aux->departure_time < truck->departure_time)) {
		/*if (aux == truck) {
			break;
		}*/
		prev = aux;
		aux = aux->next;
	}
	prev->next = truck;
	truck->next = aux;
}


void truck_arrived(Parkinglot *parkinglot, Truck* truck){
	if(parkinglot == NULL || truck == NULL) return;
	if (parkinglot_are_trucks_in_transit(parkinglot) == 1) {
		Truck* aux = parkinglot->departed_trucks->next,* prev = parkinglot->departed_trucks;
		// se verifica daca tirul aflat in departure
		// este si in arrived
		while (aux != parkinglot->departed_trucks) {
			if (aux == truck) {
				prev->next = aux->next;
				(aux->next)->next = parkinglot->departed_trucks;
				break;
			}
			prev = aux;
			aux = aux->next;
		}
	}
	truck->size = 0;
	truck->manifest = NULL;
	Truck* aux = parkinglot->arrived_trucks->next, *prev = parkinglot->arrived_trucks;
	while ((aux != parkinglot->arrived_trucks) &&
		   (strcmp(aux->destination, truck->destination) <= 0)) {
		if (strcmp(aux->destination, truck->destination) == 0) {
			while ((aux->destination != NULL) && (strcmp(aux->destination, truck->destination) == 0)) {
					if (aux->departure_time > truck->departure_time) {
						prev->next = truck;
						truck->next = aux;
						return;
					}
				prev = aux;
				aux = aux->next;
			}
			truck->next = prev->next;
			prev->next = truck;
			return;
		}
		prev = aux;
		aux = aux->next;
	}
	prev->next = truck;
	truck->next = aux;
	return;
}



void truck_transfer_unloading_robots(Parkinglot* parkinglot, Truck* truck){
	Robot* robot = truck->unloading_robots, *aux = NULL;
		while(robot) {
			aux = robot;
			Robot* auxRobot = create_robot(robot->capacity);
			parkinglot_add_robot(parkinglot, auxRobot);
			robot = robot->next;
			destroy_robot(aux);
		}
	truck->unloading_robots = NULL;
	return;

}


// Depends on parking_turck_departed
void truck_update_depatures(Parkinglot* parkinglot, long day_hour){
	Truck* truck = parkinglot->arrived_trucks->next;
	while ((truck != parkinglot->arrived_trucks) && (truck->departure_time != day_hour)) {
		truck = truck->next;
	}
	//if (truck == NULL) return;
	//Truck* auxTruck = create_truck(truck->destination, truck->capacity,
	//					truck->in_transit_time, truck->departure_time);
	truck_departed(parkinglot, truck);
	//destroy_truck(truck);
	return;
}

// Depends on parking_turck_arrived
void truck_update_transit_times(Parkinglot* parkinglot){
	Truck* truck = parkinglot->departed_trucks->next;
	(((parkinglot->departed_trucks)->next)->in_transit_time)++;
	if (truck->in_transit_time == truck->transit_end_time) {
		truck_arrived(parkinglot, truck);
		(((parkinglot->arrived_trucks)->next)->in_transit_time) = 0;
		(((parkinglot->arrived_trucks)->next)->transit_end_time) = 0;
		return;
	}
	return;
}

void robot_swarm_collect(Wearhouse *wearhouse, Parkinglot *parkinglot){
	Robot *head_robot = parkinglot->standby_robots;
	Robot *current_robot = parkinglot->standby_robots->next;
	while(current_robot != parkinglot->standby_robots){

		// Load packages from wearhouse if possible
		if(!robot_load_packages(wearhouse, current_robot)){
			break;
		}

		// Remove robot from standby list
		Robot *aux = current_robot;
		head_robot->next = current_robot->next;
		current_robot = current_robot->next;

		// Add robot to the
		parkinglot_add_robot(parkinglot, aux);
	}
}


void robot_swarm_assign_to_trucks(Parkinglot *parkinglot){

	Robot *current_robot = parkinglot->pending_robots->next;

	while(current_robot != parkinglot->pending_robots){
		Robot* aux = current_robot;
		current_robot = current_robot->next;
		parkinglot_remove_robot(parkinglot, aux);
		int attach_succeded = robot_attach_find_truck(aux, parkinglot);
		if(!attach_succeded)
			parkinglot_add_robot(parkinglot, aux);
	}
}

void robot_swarm_deposit(Parkinglot* parkinglot){
	Truck *arrived_iterator = parkinglot->arrived_trucks->next;
	while(arrived_iterator != parkinglot->arrived_trucks){
		Robot *current_robot = arrived_iterator->unloading_robots;
		while(current_robot != NULL){
			robot_unload_packages(arrived_iterator, current_robot);
			Robot *aux = current_robot;
			current_robot = current_robot->next;
			arrived_iterator->unloading_robots = current_robot;
			parkinglot_add_robot(parkinglot, aux);
		}
		arrived_iterator = arrived_iterator->next;
	}
}

