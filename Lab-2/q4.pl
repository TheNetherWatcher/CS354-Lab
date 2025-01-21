div(N, F, M, R, K) :- S is N // F, N =:= S * F, !,   
   K1 is K + 1, div(S, F, M, R, K1).
div(N, _, M, N, M).

division(N, F, M, R) :- div(N, F, M, R, 0), M > 0.

next_factor(_, 2, 3) :- !.
next_factor(N, F, NF) :- F * F < N, !, NF is F + 2.
next_factor(N, _, N).

prime_factors(N, L) :- N > 0, prime_factors(N, L, 2).

prime_factors(1, [], _) :- !.
prime_factors(N, [[F, M]|L], F) :- division(N, F, M, R), !, 
   next_factor(R, F, NF), prime_factors(R, L, NF).
prime_factors(N, L, F) :- !,                          
   next_factor(N, F, NF), prime_factors(N, L, NF).

prime_factors_mult(N, L) :- prime_factors(N, L).