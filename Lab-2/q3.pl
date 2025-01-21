reverse([],A,A).
reverse([H|T],A,Acc) :- reverse(T,A,[H|Acc]).
palindrome(L):- reverse(L, L, []).