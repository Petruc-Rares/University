#lang racket

; generator pentru numere naturale
(define y 10)
(define pr (lambda (x) (delay (+ x y))))
(force (pr 2))
(define y 20)
(force (pr 2))
