#include "Game.hpp"


int main() {

    // init rand
    std::srand(static_cast<unsigned>(time(NULL)));

    // game engine initalize
    Game game;

    // game loop
    while(game.running()&&!game.getEndGame()) { // outer loop starts
       
        // update
        game.update();

        // render
        game.render();

    } // outer loop ends

    // end of application

    return 0;
}