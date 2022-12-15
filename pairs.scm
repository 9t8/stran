; pairs test - doesn't work for some reason
(define (cons a b)
  (lambda (cond) (cond a b)))

(define (car p)
  (p (lambda (a b) a)))

(define test (cons 1 2))
(car test)
