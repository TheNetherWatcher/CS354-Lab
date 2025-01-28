move(state(X, Y), fill4, state(4, Y)) :- X < 4.
move(state(X, Y), fill3, state(X, 3)) :- Y < 3.
move(state(X, Y), empty4, state(0, Y)) :- X > 0.
move(state(X, Y), empty3, state(X, 0)) :- Y > 0.
move(state(X, Y), pour4to3, state(NewX, NewY)) :-
    X > 0, Y < 3,
    Transfer is min(X, 3 - Y),
    NewX is X - Transfer,
    NewY is Y + Transfer.
move(state(X, Y), pour3to4, state(NewX, NewY)) :-
    Y > 0, X < 4,
    Transfer is min(Y, 4 - X),
    NewX is X + Transfer,
    NewY is Y - Transfer.

solve(JugStates) :- solve(state(0, 0), [], JugStates).

solve(state(2, _), _, []) :- !. 
solve(CurrentState, Visited, [Action|Actions]) :-
    move(CurrentState, Action, NextState),
    \+ member(NextState, Visited),
    solve(NextState, [NextState|Visited], Actions).