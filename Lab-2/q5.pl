gcd(N,0,N) :- N > 0, !.
gcd(N,M,GCD) :- M > 0, R is N mod M, gcd(M,R,GCD).

coprime(N,M) :- gcd(N,M,1).