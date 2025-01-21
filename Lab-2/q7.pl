compress([],[]).
compress([H], [H]).
compress([H,H|T], X) :- compress([H|T],X).
compress([H|T], [H|X]) :- compress(T, X).