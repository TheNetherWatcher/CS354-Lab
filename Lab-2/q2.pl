element_at(H,[H|_],1):- !.
element_at(A,[_|T],N) :- N1 is N - 1, element_at(A,T,N1).