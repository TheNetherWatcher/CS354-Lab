lives(british, red_house).
lives(X, yellow_house) :- profession(X, diplomat).
lives(Norwegian, X) :- right(Y, X), right(Z, Y).
lives(X, Y) :- near(Y, Z), lives(A, Z), owns(A, fox).
owns(spanish, dog).
owns(sculptor, snails).
drinks(X, coffee) :- lives(X, green_house).
drinks(ukrainian, tea).
drinks(X, milk) :- right(Z, X), right(X, Y).
drinks(X, orange_juice) :- profession(X, violinist).
right(green_house, white_house).
profession(japanese, acrobat).
horse(X) :- lives(Y, X), profession(Y, diplomat), near(X, Z), lives(A, Z), owns(A, horse).
near(X, blue_house) :- lives(norwegian, X).