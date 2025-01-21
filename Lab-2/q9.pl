add_key([],[]).
add_key([A|A1],[L-p(A)|B1]) :-!, length(A,L), add_key(A1,B1).

rem_key([],[]).
rem_key([_-p(A)|A1],[A|B1]) :- rem_key(A1,B1).

lsort(In,Out) :-
	add_key(In,K),
	keysort(K,SK),
	rem_key(SK,Out).