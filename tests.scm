; defines
(define a 3)
(define b a)
1
2
a
b

; lambdas
(lambda (x) x)
((lambda (x) x) 4)
(define a (lambda (x) x))
(a 1)
(a 2)
((lambda () 2))
(define (b) 2)
b
(b)
(define (f a) (lambda () a))
((f 0))

; higher-order functions
((lambda (f a) (f a)) (lambda (b) b) 420)
(define (f a) (lambda (m) (m a)))
(define y (f 1))
(y (lambda (x) x))
(define (g b) (b (lambda (x) x)))
(g y)

; true/false
(define if (lambda (c t f) (c t f)))
(define t (lambda (l r) l))
(define f (lambda (l r) r))
(if t 1 0)
(if f 1 0)

; swapped names
(define (if c f t) (c f t))
(define (t l r) l)
(define (f l r) r)
(if t 1 0)
(if f 1 0)

; pairs
(define (cons car cdr)
  (lambda (m) (m car cdr)))
(define (car p)
  (p (lambda (car cdr) car)))
(define (cdr p)
  (p (lambda (car cdr) cdr)))
(define p (cons 1 2))
(car p)
(cdr p)

; multi-statement procedures
(lambda () 0 1)
((lambda () 0 1))
(define (f a b) a b)
(f 0 1)

; standard procedures
(define (list . l) l)

; variadics
(lambda l l)
((lambda l l))
((lambda l l) 0)
((lambda l l) 0 1)
((lambda l l) 0 1 2)
(list)
(list 0)
(list 0 1)
(list 0 1 2)
(define (first first . rest) first)
(first 0)
(first 0 1)
(first 0 1 2)
(define (rest first . rest) rest)
(rest 0)
(rest 0 1)
(rest 0 1 2)

; quotes
1
'1
'a
'(1 2 3)
(list 1 2 3)
((lambda (a) a) '(1 2 3))
((lambda (a) a) (list 1 2 3))
(define x '(1 . 2))
x
