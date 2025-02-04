jump(N, X/Y, U/V) :-
    member((DX,DY),[(2,1),(1,2),(-1,2),(-2,1),(-2,-1),(-1,-2),(1,-2),(2,-1)]),
    U is X + DX,
    V is Y + DY,

    U > 0, U =< N,
    V > 0, V =< N.

knight_tour(N, Path, Path) :-
    length(Path, L),
    L =:= N * N.

knight_tour(N, Visited, Path) :-
    Visited = [X/Y | _],                    
    jump(N, X/Y, U/V),                     
    \+ member(U/V, Visited),               
    knight_tour(N, [U/V | Visited], Path). 

solve_knight_tour(N, Path) :-
    StartX = 1, StartY = 1,               
    knight_tour(N, [StartX/StartY], Path).