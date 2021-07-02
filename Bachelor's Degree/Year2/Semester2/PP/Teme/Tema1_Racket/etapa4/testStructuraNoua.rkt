#lang racket

(require "queue.rkt")
(define-struct counter (index tt et queue open) #:transparent)
(define C5 (make-counter 5 12 8 (queue '((remus . 6) (vivi . 4)) '() 2 0)))