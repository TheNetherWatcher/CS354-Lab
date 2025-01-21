reverse([],A,A).
reverse([H|T],A,Acc) :- reverse(T,A,[H|Acc]).