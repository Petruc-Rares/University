// Copyright 2019 Petruc Rares
#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <ctype.h>

#include "util/task_helper.h"

#define MAX 5000
#define MED 3000
#define MIN 1000

double atof(const char * str);

// functie ce calculeaza numarul de versuri
int lyrics_number(char * poem) {
  int i = 0;
  char * p;
  char * s1 = (char * ) malloc(MAX * sizeof(char));
  strcat(s1, poem);
  p = strtok(s1, "\n");
  i++;
  while (p != NULL) {
    p = strtok(NULL, "\n");
    i++;
  }
  free(s1);
  return (i - 1);
}

int alloc_matrix(char ** matrix, int no) {
  int j;
  for (j = 0; j < no; j++) {
    matrix[j] = calloc(MED, sizeof(char));
    if (matrix[j] == NULL) return 1;
  }
}

// functie ce imparte poemul in versuri
int split_poem(char * poem, char ** matrix, int nr) {
  int j = 0;
  char * s1 = (char * ) malloc(MAX * sizeof(char)), * p;
  strcpy(s1, poem);
  p = strtok(s1, "\n");
  while (p != NULL) {
    strcpy(matrix[j], p);
    p = strtok(NULL, "\n");
    j++;
  }
  free(s1);
  free(poem);
  return 0;
}

int check_character(char s) {
  if ((s >= 'a') && (s <= 'z')) {
    return 1;
  }
  if ((s >= 'A') && (s <= 'Z')) {
    return 2;
  }
  return 0;
}

int check_character_1(char s) {
  if (s == ' ') return 1;
  if (s == ',') return 2;
  if (s == '.') return 3;
  if (s == ';') return 4;
  if (s == ':') return 5;
  if (s == '?') return 6;
  if (s == '!') return 7;
  if (s == '\0') return 8;
  return 0;
}

void make_it_silly(char ** lyric, int no, double p) {
  int i, j;
  for (i = 0; i < no; i++) {
    for (j = 0; j < strlen(lyric[i]); j++)
      if (check_character(lyric[i][j]) != 0) {
        if (rand() % 100 / 99.0 < p) {
          if (check_character(lyric[i][j]) == 1) {
            lyric[i][j] = toupper(lyric[i][j]);
          } else {
              lyric[i][j] = tolower(lyric[i][j]);
          }
        }
      }
  }
  return;
}

void make_it_friendlier(char ** lyric, int no) {
  int i = 0, j, q;
  char * s1 = (char * ) calloc(MIN, sizeof(char));
  char * s2 = (char * ) calloc(MIN, sizeof(char));
  char * p, * dim;
  for (i = 0; i < no; i++) {
    strcpy(s1, lyric[i]);
    strcpy(s2, lyric[i]);
    strcpy(lyric[i], "");
    p = strtok(s1, " ,.;:");
    q = strlen(p);
    while (p) {
      get_friendly_word(p, &dim);
      if (dim) {
        strcat(lyric[i], dim);
      } else {
        strcat(lyric[i], p);
      }
      for (j = q; j < strlen(s2); j++) {
        if (check_character_1(s2[j]) != 0) {
          strncat(lyric[i], s2 + j, 1);
        } else {
          break;
        }
      }
      p = strtok(NULL, " ,.;:");
      if (p) q = j + strlen(p); // q nu va fi pe pozitia unei litere
                                // din cuvantul aflat in p
    }
  }
  free(s1);
  free(s2);
}

void get_last_word(char * lyric, char * word, int * size) {
  char * p;
  strcpy(word, "");
  p = strrchr(lyric, ' ');
  strcat(word, p + 1);
  * size = strlen(lyric) - strlen(p + 1);
}

// punctuation = pct
void remove_ortography(char * word_pct, char * word_no_pct) {
  int i;
  strcpy(word_no_pct, "");
  for (i = 0; i < strlen(word_pct); i++) {
    if (check_character_1(word_pct[i]) == 0) {
      strncat(word_no_pct, word_pct + i, 1);
    }
  }
}

void sort_string(char ** synonim_list, int * n) {
  int i, j;
  char * temp = (char * ) calloc(MAX, sizeof(char));
  for (i = 0; i < * n; i++)
    for (j = i + 1; j < * n; j++) {
      if (strcmp(synonim_list[i], synonim_list[j]) > 0) {
        strcpy(temp, synonim_list[i]);
        strcpy(synonim_list[i], synonim_list[j]);
        strcpy(synonim_list[j], temp);
      }
    }
  free(temp);
  return;
}

void make_substitute(char * word1, char * word2) {
  int j = 0, n, ok = 0;
  char ** synonim_list;
  if (tolower(word1[strlen(word1) - 1]) == tolower(word2[strlen(word2) - 1])) {
  } else {
    get_synonym(word1, & n, & synonim_list);
    if (n) {
      sort_string(synonim_list, & n);
    }
    for (j = 0; j < n; j++) {
      if (tolower(synonim_list[j][strlen(synonim_list[j]) - 1]) ==
          tolower(word2[strlen(word2) - 1])) {
        strcpy(word1, synonim_list[j]);
        ok = 1;
        break;
      }
    }
    if (ok == 0) {
      get_synonym(word2, & n, & synonim_list);
      if (n) {
        sort_string(synonim_list, & n);
      }
      for (j = 0; j < n; j++) {
        if (tolower(synonim_list[j][strlen(synonim_list[j]) - 1]) ==
            tolower(word1[strlen(word1) - 1])) {
          strcpy(word2, synonim_list[j]);
          break;
        }
      }
    }
  }
}

void verse_creation(int k, char * word0, char * word00, int * s_b0,
                    char * sir1, char ** versuri, int j) {
  int i;
  strncpy(sir1, versuri[k * 4 + j], * s_b0);
  for (i = * s_b0; i < strlen(versuri[k * 4 + j]); i++) {
    if (check_character(versuri[k * 4 + j][i]) == 0) {
      strncat(sir1, versuri[k * 4 + j] + i, 1);
    } else {
      strcat(sir1, word00);
      i += strlen(word0) - 1;
    }
  }
  strcpy(versuri[k * 4 + j], sir1);
}

void recreate_poem(int k, char * word0, char * word00, char * word1,
                 char * word01, char * word2, char * word02, char * word3,
                 char * word03, int * s_b1, int * s_b2, int * s_b3, int * s_b4,
                 char * sir1, char * sir2, char * sir3, char * sir4,
                 char ** versuri) {
  int i, j = 0;
  verse_creation(k, word0, word00, s_b1, sir1, versuri, j);
  j++;
  verse_creation(k, word1, word01, s_b2, sir2, versuri, j);
  j++;
  verse_creation(k, word2, word02, s_b3, sir3, versuri, j);
  j++;
  verse_creation(k, word3, word03, s_b4, sir4, versuri, j);
}

// wordx reprezinta ultimul cuvant din fiecare vers
// word0x este ultimul cuvant care este prelucrat (poate
// fi inlocuit cu sinonimele lui
void make_it_rhyme(char ** versuri, int nr, char * rima) {
  int k = 0, n, j, u, i, ok = 0;
  int * s_b1 = (int * ) calloc(MIN, sizeof(int));
  int * s_b2 = (int * ) calloc(MIN, sizeof(int));
  int * s_b3 = (int * ) calloc(MIN, sizeof(int));
  int * s_b4 = (int * ) calloc(MIN, sizeof(int));
  char * word = (char * ) calloc(MIN, sizeof(char));
  char * word0 = (char * ) calloc(MIN, sizeof(char));
  char * word1 = (char * ) calloc(MIN, sizeof(char));
  char * word2 = (char * ) calloc(MIN, sizeof(char));
  char * word3 = (char * ) calloc(MIN, sizeof(char));
  char * word00 = (char * ) calloc(MIN, sizeof(char));
  char * word01 = (char * ) calloc(MIN, sizeof(char));
  char * word02 = (char * ) calloc(MIN, sizeof(char));
  char * word03 = (char * ) calloc(MIN, sizeof(char));
  while (k < nr / 4) {
    char * sir4 = (char * ) calloc(MIN, sizeof(char));
    char * sir3 = (char * ) calloc(MIN, sizeof(char));
    char * sir2 = (char * ) calloc(MIN, sizeof(char));
    char * sir1 = (char * ) calloc(MIN, sizeof(char));
    get_last_word(versuri[k * 4 + 0], word, s_b1);
    remove_ortography(word, word00);
    get_last_word(versuri[k * 4 + 1], word, s_b2);
    remove_ortography(word, word01);
    get_last_word(versuri[k * 4 + 2], word, s_b3);
    remove_ortography(word, word02);
    get_last_word(versuri[k * 4 + 3], word, s_b4);
    remove_ortography(word, word03);
    strcpy(word0, word00);
    strcpy(word1, word01);
    strcpy(word2, word02);
    strcpy(word3, word03);
    if (strcmp(rima, "imperecheata") == 0) {
      make_substitute(word00, word01);
      make_substitute(word02, word03);
    } else if (strcmp(rima, "incrucisata") == 0) {
      make_substitute(word00, word02);
      make_substitute(word01, word03);
    } else if (strcmp(rima, "imbratisata") == 0) {
      make_substitute(word00, word03);
      make_substitute(word01, word02);
    }
    recreate_poem(k, word0, word00, word1, word01,
                          word2, word02, word3, word03,
                          s_b1, s_b2, s_b3, s_b4,
                       sir1, sir2, sir3, sir4, versuri);
    k++;
    free(sir1);
    free(sir2);
    free(sir3);
    free(sir4);
  }
  free(word);
  free(word0);
  free(word1);
  free(word2);
  free(word3);
  free(word00);
  free(word01);
  free(word02);
  free(word03);
  free(s_b1);
  free(s_b2);
  free(s_b3);
  free(s_b4);
}

void uppercase(char ** lyric, int no) {
  int i, j;
  i = 0;
  for (i = 0; i < no; i++)
    lyric[i][0] = toupper(lyric[i][0]);
  for (i = 0; i < no; i++) {
    for (j = 1; j < strlen(lyric[i]); j++) {
      lyric[i][j] = tolower(lyric[i][j]);
    }
  }
}

void trimming(char ** lyric, int no) {
  int i = 0;
  char * p;
  char * s1 = (char * ) malloc(MAX * sizeof(char));
  for (i = 0; i < no; i++) {
    strcpy(s1, lyric[i]);
    strcpy(lyric[i], "");
    p = strtok(s1, " .,;!?:");
    while (p) {
      strcat(lyric[i], p);
      strcat(lyric[i], " ");
      p = strtok(NULL, " .,;!?:");
    }
    lyric[i][strlen(lyric[i]) - 1] = '\0';
  }
  free(s1);
}

void print(char ** versuri, int nr) {
  int j = 0;
  for (j = 0; j < nr; j++) {
    printf("%s\n", versuri[j]);
    if (j % 4 == 3) printf("\n");
  }
}

int main(void) {
  srand(42);
  char input[50], path[50], prob[50], rhyme_type[50];
  char * poem;
  char ** lyric = (char ** ) calloc(MED, sizeof(char * ));
  if (lyric == NULL) return 1;
  int no, j;
  while (1) {
    scanf("%s", input);
    if (strcmp(input, "load") == 0) {
      scanf("%s", path);
      poem = (char * ) malloc(MED * sizeof(char));
      if (poem == NULL) return 1;
      load_file(path, poem);
      no = lyrics_number(poem);
      alloc_matrix(lyric, no);
      split_poem(poem, lyric, no);
    } else if (strcmp(input, "uppercase") == 0) {
      uppercase(lyric, no);
    } else if (strcmp(input, "trimming") == 0) {
      trimming(lyric, no);
    } else if (strcmp(input, "make_it_silly") == 0) {
      scanf("%s", prob);
      make_it_silly(lyric, no, atof(prob));
    } else if (strcmp(input, "make_it_friendlier") == 0) {
      make_it_friendlier(lyric, no);
    } else if (strcmp(input, "make_it_rhyme") == 0) {
      scanf("%s", rhyme_type);
      make_it_rhyme(lyric, no, rhyme_type);
    } else if (strcmp(input, "print") == 0) {
      print(lyric, no);
    } else if (strcmp(input, "quit") == 0) {
      for (j = 0; j < no; j++) {
        free(lyric[j]);
      }
      free(lyric);
      return 0;
    }
  }
}
