com(A, [A|L], L).
com(A, [_|L], R) :- com(A, L, R).

combination(0, _, []):- !.
combination(N, L, [H|T]) :- com(H, L, R), N1 is N-1,  combination(N1, R, T).