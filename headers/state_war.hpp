#ifndef state_war_hpp
#define state_war_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "player_boat.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class StateWar {
 private:
  string *state;
  RenderWindow *window;

  Asset asset{};
  View view{};

  TilemapWar tilemapWar{&asset};
  PlayerBoat playerBoat{&asset, &tilemapWar};

  float colliderMapSize = 32;
  Vector2i playerRealPos;

  void handleKeyboard() {
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
      if (this->tilemapWar.canMove(this->playerBoat.getColliderUp(), "w")) {
        if (this->playerRealPos.y <= this->colliderMapSize) {
          this->view.move(0, -5);
        }
        this->playerBoat.getSprite()->move(0, -5);
      }
    }

    else if (Keyboard::isKeyPressed(Keyboard::Down)) {
      if (this->tilemapWar.canMove(this->playerBoat.getColliderDown(), "s")) {
        if (this->playerRealPos.y >= this->window->getSize().y -
                                         this->playerBoat.getBoatTargetSize() -
                                         32) {
          this->view.move(0, 5);
        }
        this->playerBoat.getSprite()->move(0, 5);
      }
    }

    else if (Keyboard::isKeyPressed(Keyboard::Right)) {
      if (this->tilemapWar.canMove(this->playerBoat.getColliderRight(), "d")) {
        this->playerBoat.getSprite()->move(5, 0);
      }
    }

    else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      if (this->tilemapWar.canMove(this->playerBoat.getColliderLeft(), "a")) {
        this->playerBoat.getSprite()->move(-5, 0);
      }
    }
  }

 public:
  StateWar(string *state, RenderWindow *window) {
    this->state = state;
    this->window = window;

    this->view = this->window->getDefaultView();
    this->view.setCenter(0, 0);
    auto tileTargetSize = this->tilemapWar.getTileTargetSize();
    this->view.move(this->tilemapWar.getWidth() * tileTargetSize / 2,
                    this->tilemapWar.getHeight() * tileTargetSize / 2);

    this->window->setView(this->view);
  }

  void handleEvent(Event &event) {}

  void run(RenderWindow &window) {
    this->playerRealPos =
        window.mapCoordsToPixel(this->playerBoat.getSprite()->getPosition());
    this->window->setView(this->view);

    this->handleKeyboard();

    this->tilemapWar.draw(window);
    this->playerBoat.draw(window);
  }
};

#endif