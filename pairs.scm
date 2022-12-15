; pairs test - doesn't work for some reason
(define (cons a b)
  (lambda (cond) (cond a b)))

(define (car p)
  (p (lambda (a b) a)))

(define (cdr p)
  (p (lambda (a b) b)))

(define test (cons 1 2))
(car test)
(cdr test)
