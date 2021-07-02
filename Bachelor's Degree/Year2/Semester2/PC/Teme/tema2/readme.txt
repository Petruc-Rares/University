322CD, PETRUC RARES, TEMA 2 PC  

    Am inceput tema prin a realiza structurile folosite pentru protocolul de
nivel aplicatie. Pentru structura trimisa de clientul TCP catre server, am 
ales sa incadrez un singur sir de caractere, ca in cele ce urmeaza, server-ul
sa se ocupe de parsarea continutului mesajului. In schimb, pentru server, am
folosit mai multe campuri cum ar fi: port, adresa IP, topic, content, etc, necesare
pentru afisarea din client la primirea mesajelor pe topicurile la care era
abonat. Foarte important mi se pare si campul de flag din structura pachetului
venit de la server, ce poate lua valori intre 0 si 3 (0 pentru NON-ACK,
1 pentru ACK, 2 pentru mesaje ce se leaga de abonari si 3 pentru mesajele
forwardate de server de la udp catre clientii tcp).
    Ulterior finalizarii structurilor, am ales sa folosesc o lista cu subscriberii
curenti si cu topicurile actuale, la care au existat sau inca sunt abonati
clienti TCP. Pentru simplitate, am folosit liste inlantuite, insa cu siguranta,
daca as fi avut mai multe date legate de numele topicurilor, as fi putut alege
un dictionar (vector de 26 de structuri) sau alte implementari gen HashMap,
HashSet in cpp (pentru a reduce timpul de cautare la O(1)).
    Legat de implementare, daca pe server primeam un pachet pe socketul UDP,
il parsam, am facut transformarile ce se cereau si am trimis mesajul daca era cineva
interesat de topicul mesajului primit.
    Daca se primea o cerere de connect pe socketul TCP, adaugam un nou subscriber,
cu un nou socket, insa aveam grija ca in partea a doua, cand trimiteam un nou mesaj
din subscriber cu client_id, sa verific unicitatea acestuia si sa scot din lista
subscriberul adaugat anterior in mod eronat, alaturi de socket-ul acestuia.
    Pentru cererile de subscribe si unsubscribe, respectiv exit, am cautat in listele
de topicuri si am adaugat informatiile necesare, legate de sf, subscriberii curenti la
un topic, respectiv am inchis socketii.
    Nu am intampinat dificultati in afara de debuggingul clasic. Mentionez ca testul de
quick_flow uneori a trecut, alteori ba, cand nu verificam trunchierile. De-a lungul
temei am facut verificari pentru toate apelurile de functii de sistem si nu numai, plus
alte verificari nementionate in enuntul temei (un client TCP sa dea subscribe de 2
sau mai multe ori la un acelasi topic sau unsubscribe in acelasi mod). In acelasi timp,
am verificat cand un mesaj vine trunchiat si in caz afirmativ, am format mesajul complet
prin apeluri de recv succesive. Tema a luat aproximativ 3 zile si mi s-a parut de
dificultate medie spre grea, foarte interesanta.