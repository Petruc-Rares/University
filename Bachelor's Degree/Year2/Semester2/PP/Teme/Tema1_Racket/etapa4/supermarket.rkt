#lang racket
(require racket/match)
(require "queue.rkt")

(provide (all-defined-out))

(define ITEMS 5)

(define (ins-sort op)
  (λ (L)
    (if (null? L)
        L
        (insert (car L) ((ins-sort op) (cdr L)) op))))  ; wishful thinking

(define (insert x L op)
  (cond
    ((or (null? L) (op (second x) (second (car L))))  (cons x L))
    ((and (= (second x) (second (car L))) (< (first x) (first (car L)))) (cons x L))
    (else (cons (car L) (insert x (cdr L) op)))))


; TODO
; Aveți libertatea să vă structurați programul cum doriți (dar cu restricțiile
; de mai jos), astfel încât funcția serve să funcționeze conform specificației.
; 
; Restricții (impuse de checker):
; - trebuie să existe în continuare funcția (empty-counter index)
; - cozile de la case trebuie implementate folosind noul TDA queue

(define-struct counter (index tt et queue open) #:transparent)

(define (empty-counter index)           ; testată de checker
  (make-counter index 0 0 empty-queue #t))

(define C1 (empty-counter 1))
(define C2 (empty-counter 2))
(define C3 (empty-counter 3))
(define C4 (empty-counter 4))

(define (update f counters index)
  (foldr (λ (crt acc) (if (= index (match crt [(counter idxCrt tt et queue open) idxCrt])) (cons (f crt) acc) (cons crt acc))) '() counters))

(define closeCounter
  (λ (counterName)
    (match counterName [(counter index tt et queue open)
                        (make-counter index tt et queue #f)])))

(define tt+
  (λ (counterName)
    (match counterName [(counter index tt et queue open)
                        (λ (minutes)
                          (make-counter index (+ tt minutes) et queue open))])))

(define et+
  (λ (counterName)
    (match counterName [(counter index tt et queue open)
                        (λ (minutes)
                          (make-counter index (+ tt minutes) (+ et minutes) queue open))])))

(define (add-to-counter name items)     ; testată de checker
  (λ (C)                                ; nu modificați felul în care funcția își primește argumentele
    (match C [(counter index tt et queue open)
              (if (queue-empty? queue)
                  (make-counter index (+ tt items) (+ et items) (enqueue (cons name items) queue) open)
                  (make-counter index (+ tt items) et (enqueue (cons name items) queue) open) )])))


(define minTime
  (λ (field)
    (λ (L)
      (foldl (λ (crtCounter acc) (if (and (< (field crtCounter) (cdr acc)) (counter-open crtCounter)) (cons (counter-index crtCounter) (field crtCounter)) acc )) '(-inf.0 . +inf.0) L))))

(define min-tt (minTime counter-tt)) ; folosind funcția de mai sus
(define min-et (minTime counter-et)) ; folosind funcția de mai sus

(define (remove-first-from-counter C)   ; testată de checker
  (if (queue-empty? (counter-queue C))
      ; daca nu exista cineva se va face o casa goala
      (make-counter (counter-index C) 0 0 empty-queue (counter-open C))
      ; altfel se scade din tt-ul curent et-ul curent
      ; si se updateaza et-ul cu numarul de produse al urmatorului client
      (if (= (+ (queue-size-l (counter-queue C))(queue-size-r (counter-queue C))) 1)
          ; cazul cand nu mai ramane nimeni la coada
          (make-counter (counter-index C) (- (counter-tt C) (counter-et C)) 0 (dequeue (counter-queue C)) (counter-open C))
          (make-counter (counter-index C) (- (counter-tt C) (counter-et C)) (cdr (top (dequeue (counter-queue C)))) (dequeue (counter-queue C)) (counter-open C)))))


; TODO
; Implementați o funcție care calculează starea unei case după un număr dat de minute.
; Funcția presupune, fără să verifice, că în acest timp nu a ieșit nimeni din coadă, 
; deci va avea efect doar asupra câmpurilor tt și et.
; (cu alte cuvinte, este responsabilitatea utilizatorului să nu apeleze această funcție
; cu minutes > timpul până la ieșirea primului client din coadă)
; Atenție: casele fără clienți nu trebuie să ajungă la timpi negativi!
(define (pass-time-through-counter minutes)
  (λ (C)
    (match C [(counter index tt et queue open)
              (if (queue-empty? queue)
                  (if (<= et minutes) (make-counter (counter-index C) 0 0 empty-queue (counter-open C))
                      ; exista posibilitatea ca aceasta casa sa fie intarziata desi nu
                      ; are pe nimeni la coada
                      (make-counter index (- tt minutes) (- et minutes) queue open))
                  (make-counter index (- tt minutes) (- et minutes) queue open))])))

; create a pass-time function that can solve more than
; assuming that the first person at the queue won't be affected
(define (pass-time-through-counter-anyNoMinutes minutes)
  (λ (C)
    (match C [(counter index tt et queue open)
              (cond
                ((queue-empty? queue)
                 (cond
                   ((<= et minutes) (make-counter (counter-index C) 0 0 empty-queue (counter-open C)))
                   ; exista posibilitatea ca aceasta casa sa fie intarziata desi nu
                   ; are pe nimeni la coada
                   (else (make-counter index (- tt minutes) (- et minutes) queue open))))
                ((> et minutes) ((pass-time-through-counter minutes) C))
                ((<= et minutes) ((pass-time-through-counter-anyNoMinutes (- minutes et)) (remove-first-from-counter C))))])))

(define (addCounters counters noNeededCounters nextIndex)
  (if (zero? noNeededCounters)
      counters
      (addCounters (flatten (append counters (empty-counter nextIndex))) (- noNeededCounters 1) (+ 1 nextIndex))
      )
  )


(define (getClientsLeaving crtCounter minutes acc minutesGone)
  (if (queue-empty? (counter-queue crtCounter))
      (reverse acc)
      (if (> (counter-et crtCounter) minutes)
          (reverse acc)
          (getClientsLeaving (remove-first-from-counter crtCounter) (- minutes (counter-et crtCounter)) (append (list (list (counter-index crtCounter) (+ (counter-et crtCounter) minutesGone) (car (top (counter-queue crtCounter))))) acc) (+ (counter-et crtCounter) minutesGone))
          ))
  )

(define (getNeededCountersNo waitingTime noNeededCounters targetTime)
  (if (> (/ waitingTime noNeededCounters) targetTime)
      (getNeededCountersNo waitingTime (+ 1 noNeededCounters) targetTime)
      noNeededCounters
      )
  )
  
; TODO
; Implementați funcția care simulează fluxul clienților pe la case.
; ATENȚIE: Față de etapa 3, apare un nou tip de cerere, așadar
; requests conține 5 tipuri de cereri (cele moștenite din etapa 3 plus una nouă):
;   - (<name> <n-items>) - persoana <name> trebuie așezată la coadă la o casă              (ca înainte)
;   - (delay <index> <minutes>) - casa <index> este întârziată cu <minutes> minute         (ca înainte)
;   - (ensure <average>) - cât timp tt-ul mediu al caselor este mai mare decât
;                          <average>, se adaugă case fără restricții (case slow)           (ca înainte)
;   - <x> - trec <x> minute de la ultima cerere, iar starea caselor se actualizează
;           corespunzător (cu efect asupra câmpurilor tt, et, queue)                       (ca înainte)
;   - (close <index>) - casa index este închisă                                            (   NOU!   )
; Sistemul trebuie să proceseze cele 5 tipuri de cereri în ordine, astfel:
; - persoanele vor fi distribuite la casele DESCHISE cu tt minim; nu se va întâmpla
;   niciodată ca o persoană să nu poată fi distribuită la nicio casă                       (mică modificare)
; - când o casă suferă o întârziere, tt-ul și et-ul ei cresc (chiar dacă nu are clienți);
;   nu aplicați vreun tratament special caselor închise                                    (ca înainte)
; - tt-ul mediu (ttmed) se calculează pentru toate casele DESCHISE, 
;   iar la nevoie veți adăuga case slow una câte una, până când ttmed <= <average>         (mică modificare)
; - când timpul prin sistem avansează cu <x> minute, tt-ul, et-ul și queue-ul tuturor 
;   caselor se actualizează pentru a reflecta trecerea timpului; dacă unul sau mai mulți 
;   clienți termină de stat la coadă, ieșirile lor sunt contorizate în ordine cronologică. (ca înainte)
; - când o casă se închide, ea nu mai primește clienți noi; clienții care erau deja acolo
;   avansează normal, până la ieșirea din supermarket                                    
; Rezultatul funcției serve va fi o pereche cu punct între:
; - lista sortată cronologic a clienților care au părăsit supermarketul:
;   - fiecare element din listă va avea forma (index_casă . nume)
;   - dacă mai mulți clienți ies simultan, sortați-i crescător după indexul casei
; - lista cozilor (de la case DESCHISE sau ÎNCHISE) care încă au clienți:
;   - fiecare element va avea forma (index_casă . coadă) (coada este de tip queue)
;   - lista este sortată după indexul casei
(define (serve_helper requests fast-counters slow-counters history)
  (define (selectCheckerFields lista)
    (foldr (λ (x acc) (append (list (cons (first x) (third x))) acc)) null lista)
    )
  (if (null? requests)
      (cons (selectCheckerFields (reverse history)) (foldr (λ(counter acc) (if (not (queue-empty? (counter-queue counter))) (cons (cons (counter-index counter) (counter-queue counter)) acc) acc)) null (append fast-counters slow-counters )))
      ;(cons (selectCheckerFields (reverse history)) (append fast-counters slow-counters ))
      (match (car requests)
        [(list 'ensure targetTime)
         
         (define (getNeededCountersNo waitingTime noActualCounters noNeededCounters targetTime)
           (if (> (/ waitingTime noActualCounters) targetTime)
               (getNeededCountersNo waitingTime (+ 1 noActualCounters) (+ 1 noNeededCounters) targetTime)
               noNeededCounters))

         (define (nextCounterIndex)
           (if (null? slow-counters)
               (if (null? fast-counters)
                   ; it means now we open the first counter
                   1
                   (+ 1 (counter-index (last fast-counters)))
                   )
               (+ 1 (counter-index (last slow-counters)))
               )
           )

         (serve_helper (cdr requests) fast-counters (addCounters slow-counters (getNeededCountersNo (foldl (λ (x acc) (if (counter-open x) (+ (counter-tt x) acc) acc)) 0 (append fast-counters slow-counters)) (length (foldr (λ (x acc) (if (counter-open x) (cons x acc) acc)) '() (append fast-counters slow-counters))) 0 targetTime) (nextCounterIndex)) history) 
         ]
        [(list 'close index) (serve_helper (cdr requests) (update closeCounter fast-counters index) (update closeCounter slow-counters index) history)]
        [(list 'delay index minutes) (serve_helper (cdr requests) (update (λ (x) ((et+ x) minutes)) fast-counters index) (update (λ (x) ((et+ x) minutes)) slow-counters index) history)]
        [(list name n-items) (if (<= n-items ITEMS)
                                 ; even if the customers has lest than ITEMS items, he can
                                 ; place himself at the slow-counters
                                 (if (<= (cdr (min-tt fast-counters)) (cdr (min-tt slow-counters)))
                                     (serve_helper (cdr requests) (update (λ (x) ((add-to-counter name n-items) x)) fast-counters (car (min-tt fast-counters))) slow-counters history)
                                     (serve_helper (cdr requests) fast-counters (update (λ (x) ((add-to-counter name n-items) x)) slow-counters (car (min-tt slow-counters))) history)
                                     )
                                 (serve_helper (cdr requests) fast-counters (update (λ (x) ((add-to-counter name n-items) x)) slow-counters (car (min-tt slow-counters))) history)
                                 )]; final else
        [minutes (serve_helper (cdr requests) (foldr (λ (x acc) (append (list ((pass-time-through-counter-anyNoMinutes minutes) x)) acc)) null fast-counters) (foldr (λ (x acc) (append (list ((pass-time-through-counter-anyNoMinutes minutes) x)) acc)) null slow-counters) (append (reverse ((ins-sort  <) (foldr (λ (x acc) (append (getClientsLeaving x minutes null 0) acc)) null (append fast-counters slow-counters)))) history)) ]
          
        [(list 'remove-first)
         (define firstToRemove (car (foldl (λ (x acc) (if (and (not (queue-empty? (counter-queue x))) (< (counter-et x) (cdr acc))) (cons (counter-index x) (counter-et x)) acc)) '(-inf.0 . +inf.0)  (append fast-counters slow-counters))))           
         (serve_helper (cdr requests) (update (λ (x) (remove-first-from-counter x)) fast-counters firstToRemove) (update (λ (x) (remove-first-from-counter x)) slow-counters firstToRemove) history)]
              
        ) ; final match
      ) ; final if
  )
  

(define (serve requests fast-counters slow-counters)
  (serve_helper requests fast-counters slow-counters null))

(define (list->stream l)
  (if (null? l)
      empty-stream
      (stream-cons (car l) (list->stream (cdr l)))))

(define Q1 (make-queue empty-stream '() 0 0))
(define Q2 (make-queue (list->stream '(10 2 3)) '(60 5 40) 3 3))
(define Q3 (make-queue (list->stream '(1)) '(10) 1 1))
(define Q4 (make-queue (list->stream '(1 7)) '(12) 2 1))
(define Q5 (make-queue (list->stream '(2 5 8)) '() 3 0))
