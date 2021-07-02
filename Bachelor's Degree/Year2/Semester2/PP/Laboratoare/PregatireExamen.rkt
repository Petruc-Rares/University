#lang racket
(require (lib "trace.ss"))


; intersectie lista de liste
(define (intersectieListe L1 L2)
  (map (λ (crtList1 crtList2) (set-intersect crtList1 crtList2)) L1 L2))

;(intersectieListe '((1 2 3 1) (4 5 6)) '((2 5 6) (3 6 9)))



; intersectie liste
(define (intersectLists L1 L2)
  (foldr (λ (x acc) (if (and (member x L2) (not (member x acc))) (cons x acc) acc)) '() L1))

(intersectLists '(5 2 3 7 1) '(1 2 5 8))


(checkSmallerLists '(2 3 1) '(5 4 3))