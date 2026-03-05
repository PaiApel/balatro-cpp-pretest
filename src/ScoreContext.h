#ifndef SCORECONTEXT_H
#define SCORECONTEXT_H

class ScoreContext {
public:
    int chips;
    int mult;

    ScoreContext(int chips, int mult) : chips(chips), mult(mult) {}

    int finalScore() const {
        return chips * mult;
    }
};

#endif
