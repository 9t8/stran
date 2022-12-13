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

; true/false
(define if (lambda (c t f) (c t f)))
(define t (lambda (l r) l))
(define f (lambda (l r) r))
(if t 1 0)
(if f 1 0)

; swapped names to check for mangling
(define if (lambda (c f t) (c f t)))
(define t (lambda (l r) l))
(define f (lambda (l r) r))
(if t 1 0)
(if f 1 0)
