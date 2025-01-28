% is_at(monkey, door).
% is_at(box, window).
% is_at(banana, middle).
% hungry(monkey).

% grasp(monkey, banana):- hungry(monkey), climb(monkey, box), is_at(banana, middle).
% climb(monkey, box) :- is_at(monkey, box, middle).
% is_at(monkey, box, middle) :- push(monkey, box, middle).
% push(monkey, box, middle) :- is_at(box, window), is_at(monkey, window).
% is_at(monkey, window) :- walk_to(monkey, window).
% walk_to(monkey, window) :- is_at(monkey, L), L \= window.

is_at(monkey, door).
is_at(box, window).
is_at(banana, middle).
hungry(monkey).

grasp(monkey, banana) :-
    hungry(monkey),
    writeln("Monkey attempts to grasp the banana."),
    climb(monkey, box),
    is_at(banana, middle).

climb(monkey, box) :-
    writeln("Monkey climbs the box."),
    is_at(monkey, box, middle).

is_at(monkey, box, middle) :-
    writeln("Monkey is now on the box in the middle."),
    push(monkey, box, middle).

push(monkey, box, middle) :-
    writeln("Monkey pushes the box from the window to the middle."),
    is_at(box, window),
    is_at(monkey, window).

is_at(monkey, window) :-
    writeln("Monkey goes to the window."),
    walk_to(monkey, window).

walk_to(monkey, window) :-
    writeln("Monkey walks from L to the window."),
    is_at(monkey, L),
    L \= window.