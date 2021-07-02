#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* Key;
typedef int Value;
typedef long(*HashFunction)(Key, long);

typedef struct Element {
  Key key;
  Value value;
  struct Element *next;
} Element;

typedef struct HashTable {
  Element** elements;
  long size;
  HashFunction hashFunction;
} HashTable;

void initHashTable(HashTable **h, long size, HashFunction f) {
	(*h) = (HashTable*) malloc(sizeof(HashTable));
	(*h)->elements = (Element**)malloc(size*sizeof(Element*));
	(*h)->size = size;
	for (long i = 0l; i < size; i++) {
		(*h)->elements[i] = NULL;
	}
	(*h)->hashFunction = f;
	return;
}

int exists(HashTable *hashTable, Key key) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e = hashTable->elements[idx];
  while (e != NULL) {
  	if (strcmp(e->key, key) == 0) return 1;
  	e = e->next;
  }
  return 0;
}

Value get(HashTable *hashTable, Key key) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e = hashTable->elements[idx];
  while (e != NULL) {
  	if (strcmp(e->key, key) == 0) return e->value;
  	e = e->next;
  }
  // return default value (in lab nu era returnat nimic)
  return 0;
}

void put(HashTable *hashTable, Key key, Value value) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e = hashTable->elements[idx];
  while (e != NULL) {
  	// daca numele animalului exista deja
  	if (strcmp(e->key, key) == 0) {
  		e->value = value;
  		return;
  	}
  	e = e->next;
  }
  // daca nu exista numele animalului
  e = (Element*) malloc(sizeof(Element));
  e->key = (Key) strdup(key);
  e->value = value;
  e->next = hashTable->elements[idx];
  hashTable->elements[idx] = e;
  return;
}

void deleteKey(HashTable *hashTable, Key key) {
  long idx = hashTable->hashFunction(key, hashTable->size);
  Element* e1 = hashTable->elements[idx];
  Element* e2 = e1->next;
  if (strcmp(e1->key, key) == 0) {
  	hashTable->elements[idx] = e1->next;
  	free(e1->key);
  	free(e1);
  	return;
  }
  while (e2->next != NULL) {
  	if (strcmp(e2->key, key) == 0) {
  		e1->next = e2->next;
  		free(e2->key);
  		free(e2);
  		return;
  	}
  	e1 = e1->next;
  	e2 = e2->next;
  }
}

void print(HashTable *hashTable) {
  long i = 0;
  Element* it;
  for (i = 0l; i < hashTable->size; i++) {
  	it = hashTable->elements[i];
  	if (it != NULL) {
  		printf("%ld\n", i);
  		while(it) {
  			printf("\t%s: %d\n", it->key, it->value);
  			it = it->next;
  		}
  	}
  }
  return;
}

void freeHashTable(HashTable *hashTable) {
  long i = 0;
  Element* it, *aux;
  for (i = 0l; i < hashTable->size; i++) {
  	it = hashTable->elements[i];
  	while(it) {
  		aux = it;
  		it = it->next;
  		free(aux->key);
  		free(aux);
  	}
  }
  free(hashTable->elements);
  free(hashTable);
  return;
}


long hash1(Key word, long size) {
  long h = 0;
  for (long i = 0; i < strlen(word); i++) {
  	h =  h*17 + word[i];
  }
  return h%size;
}

int main(int argc, char* argv[]) {
  HashTable *hashTable;
  FILE *f1, *f2;
  char word[256];
  long hashSize, common = 0;

  hashSize = atoi(argv[1]);
  f1 = fopen(argv[2], "r");
  f2 = fopen(argv[3], "r");

  initHashTable(&hashTable, hashSize, &hash1);

  // Cerinta 3

  while (fscanf(f1, "%s", word) != EOF) {
  	if (exists(hashTable, word))
  		put(hashTable, word, get(hashTable, word) + 1);
    else 
  		put(hashTable, word, 1);
  }

  print(hashTable);
  // ...

	long count;
	while(fscanf(f2, "%s", word) != EOF) {
		if(exists(hashTable, word)) {
			common = common+1;
			count = get(hashTable, word);
			if (count == 1) {
				deleteKey(hashTable, word);
			} else {
				put(hashTable, word, count-1);
			}
		}
	}
  // Cerinta 4

  // ...

  printf("Cele doua fisiere au %ld cuvinte in comun\n", common);

  freeHashTable(hashTable);
  fclose(f1);
  fclose(f2);
  return 0;
}
