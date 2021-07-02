// Copyright 2019 Petruc Rares

#include <stdio.h>

#include <math.h>

// am declarat o structura pentru a evita lucrul cu foarte multe variabile
struct ADRESA {
  unsigned char o1, o2, o3, o4; // octet1, octet2, octet3, octet4
};

// functia  O1 returneaza primul octet din masca
int O1(unsigned char MASK_2) {
  int i = 31;
  unsigned char o = 0, ok = 1;
  while ((i > 31 - MASK_2) & (ok != 0)) {
    o += (1 << i) / (pow((pow(2, 8)), 3));
    i--;
    if (i % 8 == 7) ok = 0;
  }
  return o;
}

// functia O2 returneaza al doilea octet din masca
int O2(unsigned char MASK_2) {
  int i = 23;
  unsigned char o = 0, ok = 1;
  while ((i > 31 - MASK_2) & (ok != 0)) {
    o += (1 << i) / (pow((pow(2, 8)), 2));
    i--;
    if (i % 8 == 7) ok = 0;
  }
  return o;
}

// functia O3 returneaza al treilea octet din masca
int O3(unsigned char MASK_2) {
  int i = 15;
  unsigned char o = 0, ok = 1;
  while ((i > 31 - MASK_2) & (ok != 0)) {
    o += (1 << i) / (pow((pow(2, 8)), 1));
    i--;
    if (i % 8 == 7) ok = 0;
  }
  return o;
}

// functia O4 returneaza al patrulea octet din masca
int O4(unsigned char MASK_2) {
  int i = 7;
  unsigned char o = 0, ok = 1;
  while ((i > 31 - MASK_2) & (ok != 0)) {
    o += (1 << i) / (pow((pow(2, 8)), 0));
    i--;
    if (i % 8 == -1) ok = 0;
  }
  return o;
}

// adresa de retea pe octet
int a_d_r_o(unsigned char MASK_1, unsigned char IP_1) {
  return (MASK_1 & IP_1);
}

// adresa de broadcast pe octet
int a_d_b_o(unsigned char MASK_1, unsigned char IP_1) {
  return (~(MASK_1) | IP_1);
}

// functia masca verifica corectitudinea acesteia
// se cauta prima alternanta de tip 01 din ip
int masca(unsigned char MSK_1o1,
          unsigned char MSK_1o2,
          unsigned char MSK_1o3,
          unsigned char MSK_1o4) {
  int i, ok = 1;
  if ((MSK_1o1 && (1 << 7)) == 0) {
    return 0;
  } else {
    // verificare fiecare bit octet 1
    for (i = 7; i >= 1; i--) {
      if (((MSK_1o1 & (1 << i)) == 0) &&
          ((MSK_1o1 & (1 << (i - 1))) != 0)) {
        ok = 0;
        break;
      }
    }
    // verificare ultimul bit octet 1 cu primul bit octet 2
    if (((MSK_1o1 & (1 << 0)) == 0) &&
        ((MSK_1o2 & (1 << 7)) != 0)) {
          ok = 0;
        }
    if (ok != 0) {
      // verificare fiecare bit octet 2
      for (i = 7; i >= 1; i--) {
        if (((MSK_1o2 & (1 << i)) == 0) &&
            ((MSK_1o2 & (1 << (i - 1))) != 0)) {
          ok = 0;
          break;
        }
      }
      // verificare ultimul bit octet 2 cu primul bit octet 3
      if (((MSK_1o2 & (1 << 0)) == 0) &&
          ((MSK_1o3 & (1 << 7)) != 0)) {
        ok = 0;
        }
      }
    if (ok != 0) {
      // verificare fiecare bit octet 3
      for (i = 7; i >= 1; i--) {
        if (((MSK_1o3 & (1 << i)) == 0) &&
            ((MSK_1o3 & (1 << (i - 1))) != 0)) {
          ok = 0;
          break;
        }
      }
      // verificare ultimul bit octet 3 cu primul bit octet 4
      if (((MSK_1o3 & (1 << 0)) == 0) &&
          ((MSK_1o4 & (1 << 7)) != 0)) {
        ok = 0;
      }
    }
    if (ok != 0) {
      // verificare fiecare bit octet 4
      for (i = 7; i >= 1; i--) {
        if (((MSK_1o4 & (1 << i)) == 0) &&
            ((MSK_1o4 & (1 << (i - 1))) != 0)) {
          ok = 0;
          break;
        }
      }
    }
  }
  if (ok == 1) {
    return 1;
  } else {
    return 0;
  }
}

// functia MSK_1nou afiseaza masca dupa corectare
void MSK_1nou(unsigned char MSK_1o1,
              unsigned char MSK_1o2,
              unsigned char MSK_1o3,
              unsigned char MSK_1o4) {
  char i, ok = 1, auxi;
  for (i = 7; i >= 0; i--) {
    if ((MSK_1o1 & (1 << i)) == 0) {
      auxi = i + 24;
      ok = 0;
      break;
    }
  }
  if (ok) {
    for (i = 7; i >= 0; i--) {
      if ((MSK_1o2 & (1 << i)) == 0) {
        auxi = i + 16;
        ok = 0;
        break;
      }
    }
  }
  if (ok) {
    for (i = 7; i >= 0; i--) {
      if ((MSK_1o3 & (1 << i)) == 0) {
        auxi = i + 8;
        ok = 0;
        break;
      }
    }
  }
  if (ok) {
    for (i = 7; i >= 0; i--) {
      if ((MSK_1o4 & (1 << i)) == 0) {
        auxi = i;
        ok = 0;
        break;
      }
    }
  }
  printf("%hhu.%hhu.%hhu.%hhu", O1(31 - auxi)
                              , O2(31 - auxi)
                              , O3(31 - auxi)
                              , O4(31 - auxi));
}

// functia binar afiseaza octetele ip-ului din format zecimal in binar
void binar(unsigned char IP_1o) {
  int i;
  for (i = 7; i >= 0; i--) {
    printf("%hhu", (IP_1o & (1 << i)) != 0);
  }
}

int main() {
  struct ADRESA MSK_1, IP_1, IP_2, NET_[375];
  unsigned char MSK_2, dim[375];
  unsigned n, N, i, j;
  scanf("%u\n", & n);
  for (i = 1; i <= n; i++) {
    scanf("%hhu.%hhu.%hhu.%hhu",
        & MSK_1.o1, & MSK_1.o2, & MSK_1.o3, & MSK_1.o4);
    scanf("%hhu", & MSK_2);
    scanf("%hhu.%hhu.%hhu.%hhu", & IP_1.o1, & IP_1.o2, & IP_1.o3, & IP_1.o4);
    scanf("%hhu.%hhu.%hhu.%hhu", & IP_2.o1, & IP_2.o2, & IP_2.o3, & IP_2.o4);
    scanf("%u", & N);
    j = 1;
    while (j <= N) {
      scanf("%hhu.%hhu.%hhu.%hhu/%hhu",
        & NET_[j].o1, & NET_[j].o2, & NET_[j].o3, & NET_[j].o4, & dim[j]);
      j++;
    }
    printf("%u\n", i);
    j = 1;
    printf("-0 %hhu.%hhu.%hhu.%hhu/%hhu\n",
        IP_1.o1, IP_1.o2, IP_1.o3, IP_1.o4, MSK_2);
    printf("-1 %hhu.%hhu.%hhu.%hhu", O1(MSK_2), O2(MSK_2),
                                     O3(MSK_2), O4(MSK_2));
    printf("\n");
    printf("-2 %o.%o.%o.%o %X.%X.%X.%X",
        MSK_1.o1, MSK_1.o2, MSK_1.o3, MSK_1.o4,
        MSK_1.o1, MSK_1.o2, MSK_1.o3, MSK_1.o4);
    printf("\n");
    printf("-3 %hhu.%hhu.%hhu.%hhu\n", a_d_r_o(O1(MSK_2), IP_1.o1)
                                     , a_d_r_o(O2(MSK_2), IP_1.o2)
                                     , a_d_r_o(O3(MSK_2), IP_1.o3)
                                     , a_d_r_o(O4(MSK_2), IP_1.o4));
    printf("-4 %hhu.%hhu.%hhu.%hhu\n-5 ", a_d_b_o(O1(MSK_2), IP_1.o1)
                                        , a_d_b_o(O2(MSK_2), IP_1.o2)
                                        , a_d_b_o(O3(MSK_2), IP_1.o3)
                                        , a_d_b_o(O4(MSK_2), IP_1.o4));
    if ((a_d_r_o(O1(MSK_2), IP_1.o1) == a_d_r_o(O1(MSK_2), IP_2.o1)) &&
        (a_d_r_o(O2(MSK_2), IP_1.o2) == a_d_r_o(O2(MSK_2), IP_2.o2)) &&
        (a_d_r_o(O3(MSK_2), IP_1.o3) == a_d_r_o(O3(MSK_2), IP_2.o3)) &&
        (a_d_r_o(O4(MSK_2), IP_1.o4) == a_d_r_o(O4(MSK_2), IP_2.o4))) {
      printf("DA");
    } else {
      printf("NU");
    }
    printf("\n-6 ");
    if (masca(MSK_1.o1, MSK_1.o2, MSK_1.o3, MSK_1.o4)) {
      printf("DA\n-7 ");
      printf("%hhu.%hhu.%hhu.%hhu", MSK_1.o1, MSK_1.o2, MSK_1.o3, MSK_1.o4);
    } else {
      printf("NU\n-7 ");
      MSK_1nou(MSK_1.o1, MSK_1.o2, MSK_1.o3, MSK_1.o4);
    }
    printf("\n-8 ");  binar(IP_1.o1);  printf(".");
                      binar(IP_1.o2);  printf(".");
                      binar(IP_1.o3);  printf(".");
                      binar(IP_1.o4);  printf("\n-9 ");
    for (j = 1; j <= N; j++) {
      if ((a_d_r_o(O1(dim[j]), IP_2.o1) ==
           a_d_r_o(O1(dim[j]), NET_[j].o1)) &&
          (a_d_r_o(O2(dim[j]), IP_2.o2) ==
           a_d_r_o(O2(dim[j]), NET_[j].o2)) &&
          (a_d_r_o(O3(dim[j]), IP_2.o3) ==
           a_d_r_o(O3(dim[j]), NET_[j].o3)) &&
          (a_d_r_o(O4(dim[j]), IP_2.o4) ==
           a_d_r_o(O4(dim[j]), NET_[j].o4))) {
             printf("%u ", (j - 1));
      }
    }
    printf("\n");
  }
}

