1) Am observat ca in urma mutarii celor 2 fisiere, acestea s-au schimbat
   prin adaugarea unei linii de cod pe primul rand al fiecarui fisier:
  "package task 1";

3) Rezultatul este false pentru ca avem de a face cu doua obiecte diferite.
  In cazul in care student2 ar fi fost egal cu student1 (deja initializat),
  atunci amandoua referentiau acelasi obiect iar operatia student1.equals(student2)
  ar fi avut rezultat TRUE;

4) Pentru taskul 4, nu am mai respectat punctul de la taskul 2 ce specifica sa 
  nu folosim modificatori de acces; daca este nevoie, pot reface task2 separat;