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
    bool keyUp = Keyboard::isKeyPressed(Keyboard::Up);
    bool keyDown = Keyboard::isKeyPressed(Keyboard::Down);
    bool keyRight = Keyboard::isKeyPressed(Keyboard::Right);
    bool keyLeft = Keyboard::isKeyPressed(Keyboard::Left);

    bool canMoveUp = this->tilemapWar.canMove(this->playerBoat.getColliderUp());
    bool canMoveDown =
        this->tilemapWar.canMove(this->playerBoat.getColliderDown());
    bool canMoveRight =
        this->tilemapWar.canMove(this->playerBoat.getColliderRight());
    bool canMoveLeft =
        this->tilemapWar.canMove(this->playerBoat.getColliderLeft());

    auto mapMaxY =
        this->window->getSize().y - this->playerBoat.getBoatTargetSize() - 32;
    auto mapMaxX =
        this->window->getSize().x - this->playerBoat.getBoatTargetSize() - 32;
    auto mapMin = this->colliderMapSize;

    if (keyUp && keyRight) {
      if (canMoveUp && canMoveRight)
        this->playerBoat.moveUpRight();
      else if (canMoveUp)
        this->playerBoat.moveUp();
      else if (canMoveRight)
        this->playerBoat.moveRight();

    } else if (keyUp && keyLeft) {
      if (canMoveUp && canMoveLeft)
        this->playerBoat.moveUpLeft();
      else if (canMoveUp)
        this->playerBoat.moveUp();
      else if (canMoveLeft)
        this->playerBoat.moveLeft();

    } else if (keyDown && keyRight) {
      if (canMoveDown && canMoveRight)
        this->playerBoat.moveDownRight();
      else if (canMoveDown)
        this->playerBoat.moveDown();
      else if (canMoveRight)
        this->playerBoat.moveRight();

    } else if (keyDown && keyLeft) {
      if (canMoveDown && canMoveLeft)
        this->playerBoat.moveDownLeft();
      else if (canMoveDown)
        this->playerBoat.moveDown();
      else if (canMoveLeft)
        this->playerBoat.moveLeft();
    }

    else if (Keyboard::isKeyPressed(Keyboard::Up)) {
      if (canMoveUp) {
        if (this->playerRealPos.y <= mapMin)
          this->view.move(0, -this->playerBoat.getVelocity());
        this->playerBoat.moveUp();
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
      if (canMoveDown) {
        if (this->playerRealPos.y >= mapMaxY)
          this->view.move(0, this->playerBoat.getVelocity());
        this->playerBoat.moveDown();
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
      if (canMoveRight) {
        if (this->playerRealPos.x >= mapMaxX)
          this->view.move(this->playerBoat.getVelocity(), 0);
        this->playerBoat.moveRight();
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      if (canMoveLeft) {
        if (this->playerRealPos.x <= mapMin)
          this->view.move(-this->playerBoat.getVelocity(), 0);
        this->playerBoat.moveLeft();
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
    this->playerRealPos = window.mapCoordsToPixel(
        this->playerBoat.getColliderBox()->getPosition());
    this->window->setView(this->view);

    this->handleKeyboard();

    this->tilemapWar.draw(window);
    this->playerBoat.draw(window);
  }
};

#endif