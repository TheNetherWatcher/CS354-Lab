add_key([],[]).
add_key([A|A1],[L-p(A)|B1]) :-!, length(A,L), add_key(A1,B1).

rem_key([],[]).
rem_key([_-p(A)|A1],[A|B1]) :- rem_key(A1,B1).

lsort(In,Out) :-
	add_key(In,K),
	keysort(K,SK),
	rem_key(SK,Out).

pack([],[]).
pack([L-A|A1],[[L-A|C]|C1]) :- transf(L-A,A1,B1,C), pack(B1,C1).

transf(_,[],[],[]).
transf(L-_,[K-B|B1],[K-B|B1],[]) :- L \= K.
transf(L-_,[L-A|A1],B1,[L-A|C1]) :- transf(L-A,A1,B1,C1).

lfsort(In,Out) :-
	add_key(In,K),
	keysort(K,SK),
	pack(SK,PK),
	lsort(PK,SPK),
	flatten(SPK,FK),
	rem_key(FK,Out).