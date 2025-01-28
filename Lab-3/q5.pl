% Possible knight moves
jump(N, X/Y, U/V) :-
    member((DX, DY), [(2, 1), (1, 2), (-1, 2), (-2, 1), (-2, -1), (-1, -2), (1, -2), (2, -1)]),
    U is X + DX, U > 0, U =< N,
    V is Y + DY, V > 0, V =< N.

% Solve knight's tour
knights_tour(N, Path) :-
    N2 is N * N,
    Path = [1/1 | Rest], % Start from top-left corner
    knights_tour(N, [1/1], Rest, N2).

knights_tour(_, Visited, [], 1) :-
    length(Visited, 1). % Base case: all squares are visited.

knights_tour(N, Visited, [Next | Rest], SquaresLeft) :-
    Visited = [Current | _],
    jump(N, Current, Next),
    \+ member(Next, Visited),
    NewSquaresLeft is SquaresLeft - 1,
    knights_tour(N, [Next | Visited], Rest, NewSquaresLeft).