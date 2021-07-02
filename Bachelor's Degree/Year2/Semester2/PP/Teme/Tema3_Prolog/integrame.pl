:- ensure_loaded('checker.pl').

%test_mode(detailed).

% Considerăm următoarele reprezentări:
%
% O integramă este reprezentată prin structura (compusul)
% integ(H, W, Lista, Vocab), unde:
% H este înălțimea integramei
% W este lățimea integramei
% Lista este o listă de tupluri (Poz, Valoare), unde
%   Poz este un tuplu (R, C) conținând rândul și coloana (0-based)
%   Valoare este una dintre:
%     x - dacă celula este neagră (nu poate fi completată cu litere)
%     o literă, dacă celula este completată cu o literă
%     o listă de întrebări, reprezentate ca tupluri (Text, Dir, ID), cu
%       Text - un srting, textul întrebării
%       Dir - una dintre valorile j sau d, indicând direcția întrebării
%       ID - un identificator numeric al întrebării
% Vocab este o listă de stringuri reprezentând cuvinte disponibile
% pentru a rezolva întrebarea.
%
% În ieșirea predicatului intrebări, o întrebare este reprezentată ca
% ((R, C), Text, Dir, ID), unde
% R este rândul căsuței cu întrebarea (0-based)
% C este coloana căsuței cu întrebarea (0-based)
% Text este textul întrebării (un string)
% Dir este j sau d, reprezentând direcția în care trebuie să fie plasat
% răspunsul (jos sau dreapta)
% ID este un identificator numeric al întrebării.

% Puteți vizualiza integramele cu:
% integrama(0, W), print_integrama(W).
% integrama(1, W), print_integrama(W).
% integrama(2, W), print_integrama(W).
% integrama(3, W), print_integrama(W).
%
% Testați cu
% vmtest.
% Testați teste individuale (vedeți predicatul tt din checker.pl) cu
% vmtest(Test).
% de exemplu cu vmtest(intrebari).


% intrebari/2
% intrebari(integ(+H, +W, +Lista, +Vocab), -Lista_intrebari)
% Este adevărat atunci când Lista_intrebari este o lista de tupluri
% ((R, C), Text, Dir, ID), fiecare tuplu corespunzând unei întrebări din
% integramă (rândul, coloana, textul întrebării, direcția (j/d),
% identificatorul).
% BONUS: intrebari are o singură soluție (o singură listă) pentru o
% anumită integramă.
intrebari(integ(_, _, [], _), []).
% caz o intrebare
intrebari(integ(H, W, [((R, C), [(Text, Dir, Id)])|Rest], Vocab), [X|Res]) :- X = ((R, C), Text, Dir, Id), intrebari(integ(H, W, Rest, Vocab), Res).
% caz doua intrebari
intrebari(integ(H, W, [((R, C), [(Text, Dir, Id), (Text_, Dir_, Id_)])|Rest], Vocab), [X, Y|Res]) :- X = ((R, C), Text, Dir, Id), Y = ((R, C), Text_, Dir_, Id_), intrebari(integ(H, W, Rest, Vocab), Res).
% caz nu e intrebare
intrebari(integ(H, W, [((_, _), X)|Rest], Vocab), Res) :- not(is_list(X)), intrebari(integ(H, W, Rest, Vocab), Res).

% id_intrebare/2
% id_intrebare(+Integ, ?Intrebare, ?Q_ID)
% Este adevărat dacă în integrama reprezentată ca integ(...), Intrebare
% este un text iar Q_ID este un identificator care corespund aceleași
% întrebări.
id_intrebare(Integ, Text, Q_ID) :- intrebari(Integ, Questions), member((_, Text, _, Q_ID), Questions).

% completare/3
% completare(+Integ, +Sol, -Integrama)
% Predicatul produce Integrama, o structură de forma integ(...),
% pornind de la Integ, în care au fost completate celule conform cu
% soluția Sol.
% Soluția este reprezentată ca o listă de perechi (Întrebare, Răspuns),
% unde Întrebarea este textul unei întrebări, iar Răspuns este un cuvând
% de completat; ambele sunt stringuri.
% De exemplu, o soluție parțială pentru integrama 0 poate fi:
% [('Din care plouă', 'NOR'), ('Al doilea număr', 'DOI')]
% BONUS: lungime_spatiu are o singură soluție pentru o anumită
% întrebare.
% Puteți testa manual predicatul cu o interogare de forma:
% integrama(0, W), solutie(0, Sol), completare(W, Sol, W2),
%   print_integrama(W2).
completare(integ(H, W, Lista, Vocab), QAndAs, NewInteg) :- intrebari(integ(H, W, Lista, Vocab), QuestionsList), update(Lista, QuestionsList, QAndAs, UpdatedList), NewInteg = integ(H, W, UpdatedList, Vocab).

update(List, _ , [], List).
update(List, ListQuestions, [(Q, A)|RestQA], NewList) :- member(((R, C), Q, Dir, _), ListQuestions), atom_chars(A, Achars),
                                                         update(List, ListQuestions, RestQA, RestNewList), fill((R, C), Dir, RestNewList, Achars, CurrentAnswer),
                                                         append(CurrentAnswer, RestNewList, NewList).

fill(_, _, _, [], []).
% verificam daca e intrebare
fill((R, C), Dir, List, Achars, Answer) :- member(((R, C), X), List), not(Achars == []), is_list(X), Dir=='j', R_ is R + 1, fill((R_, C), Dir, List, Achars, Answer).
fill((R, C), Dir, List, Achars, Answer) :- member(((R, C), X), List), not(Achars == []), is_list(X), Dir=='d', C_ is C + 1, fill((R, C_), Dir, List, Achars, Answer).
% daca e loc de raspuns
% mai intai verificam daca deja a fost completat spatiul
% daca e completat deja, se sare peste spatiul asta
fill((R, C), Dir, List, [_|RestChars], RestAnswer) :- member(((R, C), Val), List), not(is_list(Val)), Dir == 'j', R_ is R + 1, fill((R_, C), Dir, List, RestChars, RestAnswer).
fill((R, C), Dir, List, [_|RestChars], RestAnswer) :- member(((R, C), Val), List), not(is_list(Val)), Dir == 'd', C_ is C + 1, fill((R, C_), Dir, List, RestChars, RestAnswer).
% verificam daca trebuie sa fie completat
fill((R, C), Dir, List, [Char|RestChars], [X|RestAnswer]) :- not(member(((R, C), Val), List)), not(is_list(Val)), Dir == 'j', R_ is R + 1, X=((R, C), Char), fill((R_, C), Dir, List, RestChars, RestAnswer).
fill((R, C), Dir, List, [Char|RestChars], [X|RestAnswer]) :- not(member(((R, C), Val), List)), not(is_list(Val)), Dir == 'd', C_ is C + 1, X=((R, C), Char), fill((R, C_), Dir, List, RestChars, RestAnswer).


% lungime_spatiu/3
% lungime_spatiu(integ(+H, +W, +Lista, +Vocab), +Intrebare, -Lungime)
% Returnează lungimea spațiului asociat întrebării date.
% Întrebarea este indicată prin textul ei. De exemplu:
% lungime_spatiu pentru integrama 0 și întrebarea 'Al doilea număr'
% trebuie să lege Lungime la 3.
% BONUS: lungime_spatiu are o singură soluție pentru o anumită
% întrebare.
% Puteți testa manual predicatul cu o interogare de forma:
% integrama(0, W), id_intrebare(W, Text, 3), lungime_spatiu(W, Text, X).
% vom scadea unu din lungimea listei obtinute, pentru ca ne oprim
% la primul element care violeaza regulile noastre

lies(lion, mon).
lies(lion, tue).
lies(lion, wed).

% cazul raspuns in jos
lungime_spatiu(integ(H, W, List, Vocab), Text, Lung_) :- intrebari(integ(H, W, List, Vocab), Questions), member(((R, C), Text, Dir, _), Questions),
                                                    Dir == 'j', R_ is R + 1, H_ is H - 1,
                                                    findall((Raux, C), (numlist(R_, H_, PosRows), member(Raux, PosRows),
                                                    (\+member(((Raux, C), _), List); (member(((Raux, C), _), List),!))),
                                                    RowsIndexes), length(RowsIndexes, Lung), Lung_ is Lung - 1.

% cazul raspuns la dreapta
lungime_spatiu(integ(H, W, List, Vocab), Text, Lung_) :- intrebari(integ(H, W, List, Vocab), Questions), member(((R, C), Text, Dir, _), Questions),
                                                    Dir == 'd', C_ is C + 1, W_ is W - 1,
                                                    findall((R, Caux), (numlist(C_, W_, PosCols), member(Caux, PosCols),
                                                    (\+member(((R, Caux), _), List); (member(((R, Caux), _), List),!))),
                                                    ColsIndexes), length(ColsIndexes, Lung), Lung_ is Lung - 1.                                                    


% intersectie/5
% intersectie(integ(+H, +W, +Lista, +Voc), +I1, -Poz1, +I2, -Poz2)
% Pentru o integramă și două întrebări date prin textul lor (I1 și I2),
% al căror răspunsuri se intersectează, întoarce în Poz1 indicele din
% răspunsul la I1 la care este intersecția, și în Poz2 indicele din
% răspunsul la I2 la care este intersecția. Indecșii incep de la 0.
%
% De exemplu, în integrama 0:
%  █       █       2↓      3↓      █
%  █       0↓,1→   -       -       █
%  4→      -       -       -       █
%  5→      -       -       -       █
%  █       █       █       █       █
%
%  Întrebările 'Primii 3 din artă' și 'Afirmativ' (3, respectiv 1) se
%  intersectează la pozițiile 0, respectiv 2 (va fi litera A, de la
%  ART, respectiv DA).
intersectie(integ(H, W, Lista, Vocab), I1, Poz1, I2, Poz2) :- intrebari(integ(H, W, Lista, Vocab), Questions),
                                                            member(((R1, C1), I1, Dir1, _), Questions),
                                                            member(((R2, C2), I2, Dir2, _), Questions),
                                                            Dir1=='j', Dir2=='d',
                                                            lungime_spatiu(integ(H, W, Lista, Vocab), I1, Lung1),
                                                            lungime_spatiu(integ(H, W, Lista, Vocab), I2, Lung2),
                                                            R1_ is R1+1, C2_ is C2+1, R1__ is R1 + Lung1, C2__ is C2+Lung2,
                                                            findall((R1aux, C1), (numlist(R1_, R1__, Rows), 
                                                            member(R1aux, Rows)), I1Spaces),
                                                            findall((R2, C2aux), (numlist(C2_, C2__, Cols), 
                                                            member(C2aux, Cols)), I2Spaces),
                                                            intersection(I1Spaces, I2Spaces, CommonSpace),
                                                            [(Index1, Index2)]=CommonSpace,
                                                            Poz1 is Index1 - R1_, Poz2 is Index2 - C2_.

intersectie(integ(H, W, Lista, Vocab), I1, Poz1, I2, Poz2) :- intrebari(integ(H, W, Lista, Vocab), Questions),
                                                            member(((R1, C1), I1, Dir1, _), Questions),
                                                            member(((R2, C2), I2, Dir2, _), Questions),
                                                            Dir1=='d', Dir2=='j',
                                                            lungime_spatiu(integ(H, W, Lista, Vocab), I1, Lung1),
                                                            lungime_spatiu(integ(H, W, Lista, Vocab), I2, Lung2),
                                                            C1_ is C1+1, R2_ is R2+1, C1__ is C1 + Lung1, R2__ is R2+Lung2,
                                                            findall((R1, C1aux), (numlist(C1_, C1__, Cols), 
                                                            member(C1aux, Cols)), I1Spaces),
                                                            findall((R2aux, C2), (numlist(R2_, R2__, Rows), 
                                                            member(R2aux, Rows)), I2Spaces),
                                                            intersection(I1Spaces, I2Spaces, CommonSpace),
                                                            [(Index1, Index2)]=CommonSpace,
                                                            Poz1 is Index2 - C1_, Poz2 is Index1 - R2_.

% solutii_posibile/2
% solutii_posibile(integ(+H, +W, +Lista, +Vocabular), -Solutii)
% Formează o listă Solutii, conținând perechi de forma
% (Întrebare, Cuvinte), unde
% Întrebare este textul unei întrebări din integramă, iar Cuvinte este o
% listă de cuvinte sunt din Vocabular și au lungimea corectă pentru a fi
% răspuns la întrebare. Solutii conține câte o pereche pentru fiecare
% întrebare din integramă.
% Cuvintele sunt reprezentate ca liste de atomi, fiecare atom
% având lungime 1 (o singură literă).
% De exemplu, pentru integrama 0, Solutii conține 6 perechi, două dintre
% ele fiind:
% ('Afirmativ', [['D', 'A'], ['N', 'U']])
% ('Din care plouă',
% [['N','O','R'],['A','R','T'],['U','I','T'],['D','O','I']])
solutii_posibile_helper(_, [], []).
solutii_posibile_helper(integ(H, W, ListaValori, Vocab), [((_, _), Quest, _, _) |RestQs], [(Quest, L)|RestPairs]):-
    lungime_spatiu(integ(H, W, ListaValori, Vocab), Quest, Lq),
    findall(X_, (member(X, Vocab), atom_length(X, Lq), atom_chars(X, X_)), L),
    solutii_posibile_helper(integ(H, W, ListaValori, Vocab), RestQs, RestPairs).


solutii_posibile(integ(H, W, ListaValori, Vocab), Solutii) :- intrebari(integ(H, W, ListaValori, Vocab), Qs),
                                                            solutii_posibile_helper(integ(H, W, ListaValori, Vocab), Qs, Solutii).

% rezolvare/2
% rezolvare(+Integ, -Solutie)
% Rezolvare produce în Solutie soluția integramei Integ. Soluția este
% reprezentată ca o listă de perechi de literali, fiecare pereche
% conținând textul unei întrebări și cuvântul (ca literal) care este
% răspunsul la întrebare.
%
% BONUS: rezolvare nu oferă soluții duplicate - numărul de soluții ale 
% predicatului este chiar numărul de completări posibile ale integramei.

rezolvare_helper(_, [], _, FinalResult, FinalResult).
rezolvare_helper(W, [((_, _), Q1, _, _)|RestQs], PosSols, CrtSols, FinalResult):-
    findall(Q2, (intersectie(W, Q1, _, Q2, _)), ListIntQuests),
    member((Q1, ListAnsws1), PosSols),
    member(Answ1, ListAnsws1),
    \+member((_, Answ1), CrtSols),

    forall(member(Quest2, ListIntQuests),
    
    (
    (\+member((Quest2, _), CrtSols),
    member((Quest2, ListAnsws2), PosSols),
    member(Answ2, ListAnsws2),
    \+member((_, Answ2), CrtSols),
    intersectie(W, Q1, Pos1, Quest2, Pos2),
    nth0(Pos1, Answ1, I1),
    nth0(Pos2, Answ2, I1));

    
    (member((Quest2, Answ2), CrtSols),
    intersectie(W, Q1, Pos1, Quest2, Pos2),
    nth0(Pos1, Answ1, I1),
    nth0(Pos2, Answ2, I1)
    ))),
    rezolvare_helper(W, RestQs, PosSols, [(Q1, Answ1)|CrtSols], FinalResult).

formatare_raspuns([], []).
formatare_raspuns([(Q, Ans)|Rest], [(Q, GoodA)|RestGood]):-
    atom_chars(GoodA, Ans),
    formatare_raspuns(Rest, RestGood).


rezolvare(Integ, Solutie):-
    intrebari(Integ, Qs),
    solutii_posibile(Integ, PosSols),
    rezolvare_helper(Integ, Qs, PosSols, [], Solutie_),
    formatare_raspuns(Solutie_, Solutie).

once(X, L) :-           % X occurs once in L if...
    select(X, L, R),    % I can remove X from L giving R, and
    \+ member(X, R).    % X is not a member of R
solve(L1, L2, Res):- findall(X, (member(X, L1), member(Y, L2), 0 =:= mod(X, Y)), ResAux),
findall(Z, once(Z, ResAux), Res).
