#lang racket
(require racket/match)

(provide (all-defined-out))

(define ITEMS 5)

;; C1, C2, C3, C4 sunt case într-un supermarket.
;; C1 acceptă doar clienți care au cumparat maxim ITEMS produse (ITEMS este definit mai sus).
;; C2 - C4 nu au restricții.
;; Considerăm că procesarea fiecărui produs la casă durează un minut.
;; Casele pot suferi întarzieri (delay).
;; La un moment dat, la fiecare casă există 0 sau mai mulți clienți care stau la coadă.
;; Timpul total (tt) al unei case reprezintă timpul de procesare al celor aflați la coadă,
;; adică numărul de produse cumpărate de ei + întârzierile suferite de casa respectivă (dacă există).
;; Ex:
;; la C3 sunt Ana cu 3 produse și Geo cu 7 produse,
;; și C3 nu are întârzieri => tt pentru C3 este 10.


; Definim o structură care descrie o casă prin:
; - index (de la 1 la 4)
; - tt (timpul total descris mai sus)
; - queue (coada cu persoanele care așteaptă)
(define-struct counter (index tt queue) #:transparent)


; TODO
; Implementați o functie care intoarce o structură counter goală.
; tt este 0 si coada este vidă.
; Obs: la definirea structurii counter se creează automat o funcție make-counter pentru a construi date de acest tip
(define (empty-counter index)
  (make-counter index 0 '()))

(define C1 (empty-counter 1))
(define C2 (empty-counter 2))
(define C3 (empty-counter 3))
(define C4 (empty-counter 4))

; TODO
; Implementați o funcție care crește tt-ul unei case cu un număr dat de minute.
(define (tt+ C minutes)
  (match C
    [(counter index tt queue)
     (make-counter index (+ tt minutes) queue)]))

(define (min-tt-comparator C1 C2)
  (if (< (cdr C1) (cdr C2))
      C1
      ; in acest moment stim sigur ca C2.tt <= C1.tt
      ; deci daca e diferit, e sigur mai mic
      (if (not(= (cdr C1) (cdr C2)))
          C2
          ; la egalitate de timpi verificam si C2.index fata de C1.index
          (if (< (car C2) (car C1))
              C2
              C1))))

; TODO
; Implementați o funcție care primește o listă de case și intoarce o pereche dintre:
; - indexul casei (din listă) care are cel mai mic tt
; - tt-ul acesteia
; Obs: când mai multe case au același tt, este preferată casa cu indexul cel mai mic
(define (min-tt counters)
  (cond
    ; creez o pereche cu index invalid si timp mare de asteptare ca sa nu fie aleasa la comparatia cu o alta pereche
    [(empty? counters) (cons -inf.0 +inf.0)]
    [else (min-tt-comparator (cons (match (car counters) [(counter index tt queue) index]) (match (car counters) [(counter index tt queue) tt])) (min-tt (cdr counters)))]))

(define counter1 (list C2 C4))


; TODO
; Implementați o funcție care adaugă o persoană la o casă.
; C = casa, name = numele persoanei, n-items = numărul de produse cumpărate
; Veți întoarce o nouă structură obținută prin așezarea perechii (name . n-items)
; la sfârșitul cozii de așteptare.
(define (add-to-counter C name n-items)
  (match C
    [(counter index tt queue)
     (if (not (null? queue))
         (make-counter index (+ tt n-items) (append queue (list (cons name n-items))))
         (make-counter index (+ tt n-items) (cons (cons name n-items) queue)))]))

  (define (searchByIndex counters index)
    (cond
      ; if reach here, no counter found with @index = index
      [(null? counters) '(empty-counter -inf.0)]
      [(= (match (car counters) [(counter index tt queue) index]) index) (car counters)]
      [(searchByIndex (cdr counters) index)]
      ))

; TODO
; Implementați funcția care simulează fluxul clienților pe la case.
; requests = listă de cereri care pot fi de 2 tipuri:
; - (<name> <n-items>) - persoana <name> trebuie așezată la coadă la o casă
; - (delay <index> <minutes>) - casa <index> este întârziată cu <minutes> minute
; C1, C2, C3, C4 = structuri corespunzătoare celor 4 case
; Sistemul trebuie să proceseze aceste cereri în ordine, astfel:
; - persoanele vor fi distribuite la casele cu tt minim (dintre casele la care au voie)
; - când o casă suferă o întârziere, tt-ul ei crește
(define (serve requests C1 C2 C3 C4)

  ; puteți să vă definiți aici funcții ajutătoare (define în define)
  ; - avantaj: aveți acces la variabilele requests, C1, C2, C3, C4 fără a le retrimite ca parametri
  ; puteți de asemenea să vă definiți funcțiile ajutătoare în exteriorul lui "serve"
  ; - avantaj: puteți să vă testați fiecare funcție imediat ce ați implementat-o

  (define (modifyCounterAdd listGoodCounters name n-items)
    (define (foundCounter) (searchByIndex (list C1 C2 C3 C4) (car (min-tt listGoodCounters))))
    (cond
      [(= (match (foundCounter) [(counter index tt queue) index]) 1) (serve (cdr requests) (add-to-counter (foundCounter) name n-items) C2 C3 C4)]  
      [(= (match (foundCounter) [(counter index tt queue) index]) 2) (serve (cdr requests) C1 (add-to-counter (foundCounter) name n-items) C3 C4)]
      [(= (match (foundCounter) [(counter index tt queue) index]) 3) (serve (cdr requests) C1 C2 (add-to-counter (foundCounter) name n-items) C4)]
      [(= (match (foundCounter) [(counter index tt queue) index]) 4) (serve (cdr requests) C1 C2 C3 (add-to-counter (foundCounter) name n-items))]
      )
    )

  (define (modifyCounterDelay index minutes)
    (define (foundCounter) (searchByIndex (list C1 C2 C3 C4) index))
    (cond
      [(= (match (foundCounter) [(counter index tt queue) index]) 1) (serve (cdr requests) (tt+ (foundCounter) minutes) C2 C3 C4)]  
      [(= (match (foundCounter) [(counter index tt queue) index]) 2) (serve (cdr requests) C1 (tt+ (foundCounter) minutes) C3 C4)]
      [(= (match (foundCounter) [(counter index tt queue) index]) 3) (serve (cdr requests) C1 C2 (tt+ (foundCounter) minutes) C4)]
      [(= (match (foundCounter) [(counter index tt queue) index]) 4) (serve (cdr requests) C1 C2 C3 (tt+ (foundCounter) minutes))]
      )
    )    
  
  (if (null? requests)
      (list C1 C2 C3 C4)
      (match (car requests)
        [(list 'delay index minutes) (modifyCounterDelay index minutes)]
        [(list name n-items) (if(<= n-items ITEMS)
                               (modifyCounterAdd (list C1 C2 C3 C4) name n-items)
                               (modifyCounterAdd (list C2 C3 C4) name n-items))] ; final al doilea caz de match
        ); final else
      ) ; final if
  )