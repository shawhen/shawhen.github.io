# what are we talking about?
we are going to compute Fibonacci numbers iteratively.

# show me the code
inspired by the code snippet of section **1.2.2 Tree Recursion**:

    (define (fib n)
      (fib-iter 1 0 n))
    (define (fib-iter a b count)
      (if (= count 0)
	      b
	      (fib-iter (+ a b) a (- count 1))))

my code is below:

    (define (fibonacci2 num)
      (define (iter pre_2 pre_1 cursor)
        (cond ((= num 0) 0)
	          ((= num 1) 1)
	          (else (if (= num cursor) (+ pre_1 pre_2)
		                (iter pre_1 (+ pre_1 pre_2) (+ cursor 1))))))
	  (iter 0 1 2))
