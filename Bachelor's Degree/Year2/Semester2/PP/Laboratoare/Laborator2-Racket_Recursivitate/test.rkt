#lang racket

(define f4 (lamdda x y)
  (if (= y 0)
      1
      (* (f4 x (- y 1)) x)))