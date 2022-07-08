#include<iostream>
#include "Game.hpp"


// private functions
void Game::initVariables() { 
    this->window = nullptr;

    // game logic
    this->endGame = false;
    this->points = 0;
    this->health = 20;
    this->enemySpawnTimerMax = 20.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;
    this->mouseHeld = false;
}

void Game::initWindow() { 
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode,"First SFML Game",sf::Style::Titlebar|sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::initFonts() {
    if(this->font.loadFromFile("Fonts/Dosis-Light.ttf")) {
        std::cout<<"Failed to load font";
    }
}

void Game::initText() {
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

void Game::initEnemies() {
    this->enemy.setPosition(10.f,10.f);
    this->enemy.setSize(sf::Vector2f(100.f,100.f));
    // this->enemy.setScale(sf::Vector2f(0.5f,0.5f));
    this->enemy.setFillColor(sf::Color::Cyan);
    // this->enemy.setOutlineColor(sf::Color::Green);
    // this->enemy.setOutlineThickness(1.f);


}

// constructor and desctructor
Game::Game() {

    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
}

Game::~Game() {
    delete this->window;
}

// accessors
const bool Game::running() const {
    return this->window->isOpen();
}


// functions


const bool Game::getEndGame() const {
    return this->endGame;
}

void Game::spawnEnemy() {
    /*
        Spawn enemies and sets thier color and position
        sets random position
        sets random color
        adds enemy to vector
    */

   this->enemy.setPosition(
        static_cast<float>(rand()%static_cast<int>(this->window->getSize().x-this->enemy.getSize().x)),0.f
        
   );

    // randomly get enemy type 
    int type = rand()%5;

    switch(type) {
        case 0:
            this->enemy.setSize(sf::Vector2f(10.f,10.f));
            this->enemy.setFillColor(sf::Color::Magenta);
            break;

        case 1:
            this->enemy.setSize(sf::Vector2f(30.f,30.f));
            this->enemy.setFillColor(sf::Color::Blue);
            break;

        case 2:
            this->enemy.setSize(sf::Vector2f(50.f,50.f));
            this->enemy.setFillColor(sf::Color::Cyan);
            break;
        
        case 3:
            this->enemy.setSize(sf::Vector2f(70.f,70.f));
            this->enemy.setFillColor(sf::Color::Red);
            break;

        case 4:
            this->enemy.setSize(sf::Vector2f(10.f,10.f));
            this->enemy.setFillColor(sf::Color::Green);
            break;

        default:
            this->enemy.setSize(sf::Vector2f(10.f,10.f));
            this->enemy.setFillColor(sf::Color::Yellow);
            break;
    }

   this->enemies.push_back(this->enemy);

    // remove enemies at the end of screen
}

void Game::pollEvents() {
    while(this->window->pollEvent(this->ev)) { // inner loop starts

            switch(this->ev.type) { // switch start
                
                case sf::Event::Closed:
                    this->window->close(); //closing window
                    break;

                case sf::Event::KeyPressed:
                    if(this->ev.key.code==sf::Keyboard::Escape) {
                        this->window->close();
                    }
                    break;

            } // switch end
        } // inner loop ends
}

void Game::updateMousePositions() {
    // return void
    // updates the mouse positions
    // relative to window

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText() {
    std::stringstream ss;
    ss <<"Points: "<<this->points<<"\n"<<"Health: "<<this->health<<"\n";
    this->uiText.setString(ss.str());
}

void Game::updateEnemies() {

    // update timer  
    if(this->enemies.size()<this->maxEnemies) {
        if(this->enemySpawnTimer>=this->enemySpawnTimerMax) {
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else {
            this->enemySpawnTimer +=1.f;
        }
    }

    // move and update enemies
    for(int i=0;i<this->enemies.size();i++) {
        
        bool deleted = false;

        this->enemies[i].move(0.f,5.f); //e.move(x,y);

        // if enemy at bottom of screen
        if(this->enemies[i].getPosition().y>this->window->getSize().y) {

            this->enemies.erase(this->enemies.begin()+i);
            this->health -=1;
            std::cout<<"Health: "<<this->health<<"\n";
        }
    }

    // check if cliked upon
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        // if mouse is held
        if(this->mouseHeld==false) {
            this->mouseHeld = true;
            bool deleted = false;
            for(size_t i=0;i<this->enemies.size()&&deleted==false;i++) {
                if(this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
                    

                    // gain points
                    if(this->enemies[i].getFillColor()==sf::Color::Magenta)
                        this->points += 10;

                    else if(this->enemies[i].getFillColor()==sf::Color::Blue)
                        this->points += 7;

                    else if(this->enemies[i].getFillColor()==sf::Color::Cyan)
                        this->points += 5;

                    else if(this->enemies[i].getFillColor()==sf::Color::Red)
                        this->points += 3;

                    else if(this->enemies[i].getFillColor()==sf::Color::Green)
                        this->points += 1;

                    std::cout<<"Points: "<<this->points<<"\n";

                    // delete enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin()+i);

                }
            }
        }
    }
    else {
        this->mouseHeld = false;
    }
}

void Game::update() { //update game logic when mouse is moved

    this->pollEvents();

    if(this->endGame==false) {
        this->updateMousePositions();
        this->updateText();
        this->updateEnemies();
    }

    // end game condition
    if(this->health<=0) {
        this->endGame = true;
    }
    

    // update mouse position

    // relative to screen
    // std::cout<<"Mouse pos: "<<sf::Mouse::getPosition().x<<" "<<sf::Mouse::getPosition().y<<"\n";

    // relative to window
    // std::cout<<"Mouse pos: "<<sf::Mouse::getPosition(*this->window).x<<" "<<sf::Mouse::getPosition(*this->window).y<<"\n";
}

void Game::renderText(sf::RenderTarget& target) {
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target) {
    // render all enemies
    for(auto &e: this->enemies) {
        target.draw(e);
    }
}

void Game::render() { // visual representation of whats going

    /*  
        clear old frame
        render objects
        display in window
    */

    this->window->clear();
    
    // draw game here
    this->renderEnemies(*this->window);

    this->renderText(*this->window);
    this->window->display();
}