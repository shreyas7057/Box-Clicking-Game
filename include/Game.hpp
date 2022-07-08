#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>

#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>

/*
class acts as game engine or wrapper class

*/

class Game {

private:

    // variables

    // window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    // resources
    sf::Font font;

    // text
    sf::Text uiText;


    // game logic
    bool endGame;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;


    // game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    // private functions
    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initEnemies();

public:

    // constructors and destructors
    Game();

    virtual ~Game();


    // accessors
    const bool running() const;
    const bool getEndGame() const;

    // functions
    void spawnEnemy();
    void pollEvents();
    void updateMousePositions();
    void updateEnemies();
    void renderEnemies(sf::RenderTarget& target);
    void update();
    void render();
    void renderText(sf::RenderTarget& target);
    void updateText();
};

