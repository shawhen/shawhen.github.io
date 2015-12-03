# convert recursive to iterative
The key is find the recursive factor(s), then use variable(s) or register(s) instead of. And find the start point of iterative. And how the procedure proceed step by step.

## fibonacci
The numbers below are so called **fibonacci numbers**:

    0 1 1 2 3 5 8 13 21...

represented by mathematic formula:

            0                 n = 0
    f(n) =  1                 n = 1
            f(n-1) + f(n-2)   n >= 2
			
we can express it recursively:

    (define (fib-recursive n)
      (cond ((= n 0) 0)
            ((= n 1) 1)
            (else (+ (fib-recursive (- n 1)) (fib-recursive (- n 2))))))

we can see, the recursive factors are: `f(n-1)` and `f(n-2)`, we make **a** is **f(n-1)**, **b** is **f(n-2)**, when we step forward one time, so **n-1** will be **n**, **f(n-1)** will be **f(n)**, **f(n-2)** will be **f(n-1)**. This means that, with one step forward, these changes will be made:

    a = a + b
	b = a

and the start point is:

    n = 2
    f(1) = 1
	f(0) = 0

now, we can construct the iterative code snippet:

    (define (fib-iterative n)
      (define (iter a b cursor)
        (cond ((= num 0) 0)
              ((= num 1) 1)
              (else (if (= n cursor) (+ a b)
		            (iter (+ a b) a (+ cursor 1))))))
      (iter 1 0 2)) ;; this is the start point: f(1) f(0) 2

## sicp exercise 1.11

# Reference
- [clew of solve sicp exercise 1.11](http://stackoverflow.com/questions/2365993/no-idea-how-to-solve-sicp-exercise-1-11)
