happy(X) :- parent(X, _).
hastwochildren(X) :- parent(X, Y), sister(_, Y).
grandchild(X, Y) :- parent(Y, Z), parent(Z, X).
aunt(X, Y) :- sister(X, Z), parent(Z, Y).