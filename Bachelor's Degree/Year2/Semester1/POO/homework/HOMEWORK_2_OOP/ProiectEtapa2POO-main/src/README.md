# Proiect Energy System Etapa 2

## Un proiect care a avut drept scop evidentierea importantei scrierii unui cod
usor de adaptat/modificat.

Proiectare Orientata pe Obiecte, Seriile CA, CD
2020-2021

<https://ocw.cs.pub.ro/courses/poo-ca-cd/teme/proiect/etapa2>

Student: Petruc Rares, 322CD
Adresa de mail institutionala: rares.petruc@stud.acs.upb.ro

## Rulare teste

Clasa Test#main
  * ruleaza solutia pe testele din checker/, comparand rezultatele cu cele de referinta
  * ruleaza checkstyle

Biblioteci necesare pentru implementare:
* Jackson Core 
* Jackson Databind 
* Jackson Annotations

Tutorial Jackson JSON: 
<https://ocw.cs.pub.ro/courses/poo-ca-cd/laboratoare/tutorial-json-jackson>

## Implementare

Am gandit un cod care sa difere foarte putin fata de cel din etapa 1 (cateva
clase noi adaugate si adaptari pentru:
- consumatori care inca sunt in datoriesi mai au 0 luni din contractul curent;
- distribuitori care acum cand dau faliment trebuie sa notifice si producatorii,
pentru a-si actualiza listele de distribuitori actuali).

### Entitati (noi fata de etapa 1)

- Producer din powergridentities, laolalta cu ProducerInputData si
ProducerOutputData din fileio;
- Green/Price/Quantity strategy alaturi cu celelalte clase folosite pentru
alegerea corecta a producatorului potrivit pentru fiecare distribuitor;
- PRDUpdates si ProducersDistributors (cu precizarea ca am tinut sa notific
distribuitorii producatorilor dupa ordinea producatorilor primita in input,
pentru ca poate actualizarile, desi se produc lunar, pentru unii producatori
poate vin mai rapid, iar distribuitorii racordati la ei, pot lua si alte decizii)


### Flow

Am ales o clasa Solver care sa modeleze simularea in care am avut neaparata
nevoie de input, de o clasa ce sa stie ce sa faca in a gestiona updateurile
(fac referire la PRDUpdates), cat si fisierele de input si output.

Luna 0 este putin diferita de celelalte luni, intrucat consumatorii nu au cum
sa fi dat faliment deja, acelasi lucru fiind valabil si pentru distribuitori,
motiv pentru care am folosit o metoda separata pentru a simula prima luna fata
de urmatoarele luni ale simularii.

### Elemente de design OOP

Am folosit incapsularea foarte des pentru a seta/reseta diverse campuri (spre
exemplu contractul unui consumator la null atunci cand acesta devine liber de
contract sau modificarea campurilor diverselor entitati, atunci cand updateurile
le priveau, etc.).

Am folosit abstractizare prin definirea unor interfete pe care le implementau
diverse clase (cum ar fi ProducersStrategy)

### Design patterns

Fata de etapa trecuta, am folosit si:
- Observer pentru distribuitori;
- Observable pentru fiecare producator ce are o lista cu toti distribuitorii
prezenti in cadrul jocului;
* am facut verificari prin intermediul carora sa ma asigur ca dintre toti
distribuitorii notificati, doar cei ce isi preluau energia de la un producator
care a suferit o modificare isi vor reaplica strategia de alegere a unui
nou producator.

- Strategy pentru ca fiecare distribuitor sa isi aleaga producatorii despre
care e direct interesat ca ii satisfac conditiile;

** am adaugat in factory-ul de data trecuta si un calculator ce calculeaza
costul de productie al unui distribuitor tinand cont de producatorii care ii
furnizeaza curent (in etapa anterioara costul de productie al unui distribuitor
nu trebuia calculat, ci preluat din input).

### Dificultati intalnite
Am stat foarte mult pe testul complex_5 intrucat, atunci cand aveam mai multi
producatori ce faceau schimbari, faceam updateuri pentru toti distribuitorii
sa se debranseze si de abia la urma, iteram prin ei in ordine crescatoare
a idurilor acestora. De fapt, ar fi trebuit sa schimb pe rand pentru fiecare
distribuitor producatorii de la care isi ia energie.

## Feedback
Am constientizat prin intermediul acest proiect importanta scrierii unui cod
frumos organizat, detaliat, comentat. Mi s-a parut foarte ok fata de etapa 1.
Am adaugat foarte putine lucruri si nu am modificat aproape nimic. Sunt
satisfacut de rezultatul final al muncii depuse, intrucat am stiut exact
unde trebuie schimbat ceva din cauza noii entitati introduse in aceasta
etapa si anume cea de producator.

