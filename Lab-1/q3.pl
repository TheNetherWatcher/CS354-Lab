female(mary).
female(sandra). 
female(juliet). 
female(lisa). 
male(peter). 
male(paul).
male(john). 
male(bob). 
male(harry). 
parent(bob, lisa). 
parent(bob, paul). 
parent(bob,mary). 
parent(juliet, lisa). 
parent(juliet, paul). 
parent(juliet, mary). 
parent(peter, harry).
parent(lisa, harry). 
parent(mary, john). 
parent(mary, sandra).
father(X,Y):- parent(X,Y), male(X). 
sister(X,Y):- female(X), parent(Z, X), parent(Z, Y), X \= Y.
grandmother(X, Y):- female(X), parent(X, Z), parent(Z,Y).
grandparent(X, Y):- parent(X, Z), parent(Z, Y).
cousin(X,Y):- grandparent(Z,X),grandparent(Z,Y).