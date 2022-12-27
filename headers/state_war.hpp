#ifndef state_war_hpp
#define state_war_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "enemy_boat.hpp"
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
  // EnemyBoat enemyBoat{&asset, &tilemapWar, &playerBoat};

  vector<EnemyBoat> vectorEnemyBoat{};
  Clock clockEnemyBoat;
  float spawnEnemyBoatInterval = 0;
  float spawnEnemyBoatDelay = 2000;

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

    else if (keyUp) {
      if (canMoveUp) {
        if (this->playerRealPos.y <= mapMin)
          this->view.move(0, -this->playerBoat.getVelocity());
        this->playerBoat.moveUp();
      }
    } else if (keyDown) {
      if (canMoveDown) {
        if (this->playerRealPos.y >= mapMaxY)
          this->view.move(0, this->playerBoat.getVelocity());
        this->playerBoat.moveDown();
      }
    } else if (keyRight) {
      if (canMoveRight) {
        if (this->playerRealPos.x >= mapMaxX)
          this->view.move(this->playerBoat.getVelocity(), 0);
        this->playerBoat.moveRight();
      }
    } else if (keyLeft) {
      if (canMoveLeft) {
        if (this->playerRealPos.x <= mapMin)
          this->view.move(-this->playerBoat.getVelocity(), 0);
        this->playerBoat.moveLeft();
      }
    }

    if (Keyboard::isKeyPressed(Keyboard::Space)) {
      this->playerBoat.fire();
    }
  }

  void drawGuiProfile(RenderWindow &window) {
    // todo: gui selected boat
    {
      for (int a = 0; a < 4; a++) {
        auto buttonSize = 136;
        auto buttonScale = buttonSize / 48;
        auto isSelected = this->playerBoat.getCurrentBoatIndex() == a;

        Sprite spriteButton{};
        spriteButton.setTexture(
            this->asset.getVectorSquareButtons()->at(isSelected ? 0 : 4));
        spriteButton.setPosition(buttonSize / 2 * a, 0);
        spriteButton.setScale(buttonScale, buttonScale);

        window.draw(spriteButton);

        float boatSize = 56;
        float boatScale = boatSize / 128;
        Sprite spriteBoat{};
        spriteBoat.setTexture(this->asset.getVectorBoatColor1()->at(a));
        spriteBoat.setScale(boatScale, boatScale);
        spriteBoat.setPosition(a * buttonSize / 2 + 20, 20);

        window.draw(spriteBoat);
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

  void handleEvent(Event &event) {
    if (event.type == Event::Resized) {
      this->view.setSize(event.size.width, event.size.height);
      this->window->setView(this->view);
    } else if (event.type == Event::KeyReleased) {
      auto code = event.key.code;
      if (code == Keyboard::Space) {
        this->playerBoat.unfire();
      }
    } else if (event.type == Event::KeyPressed) {
      auto code = event.key.code;
      if (code == Keyboard::Q) {
        this->playerBoat.previousBoat();
      } else if (code == Keyboard::E) {
        this->playerBoat.nextBoat();
      }
    }
  }

  void run(RenderWindow &window) {
    // todo: spawn enemy boat
    this->spawnEnemyBoatInterval =
        this->clockEnemyBoat.getElapsedTime().asMilliseconds();
    if (this->spawnEnemyBoatInterval >= this->spawnEnemyBoatDelay) {
      if (this->vectorEnemyBoat.size() <= 2) {
        EnemyBoat newEnemyBoat{&this->asset, &this->tilemapWar,
                               &this->playerBoat};
        newEnemyBoat.setInitPos(0, 0);
        this->vectorEnemyBoat.push_back(newEnemyBoat);
      }

      this->clockEnemyBoat.restart();
    }

    this->playerRealPos = window.mapCoordsToPixel(
        this->playerBoat.getColliderBox()->getPosition());
    this->window->setView(this->view);

    this->handleKeyboard();

    this->tilemapWar.draw(window);
    this->playerBoat.draw(window);

    // todo: draw enemy
    for (int a = 0; a < this->vectorEnemyBoat.size(); a++) {
      EnemyBoat *enemyBoat = &this->vectorEnemyBoat.at(a);
      enemyBoat->draw(window);

      // todo: check collision
      for (int b = 0; b < this->playerBoat.getVectorFire()->size(); b++) {
        Fire *fire = &this->playerBoat.getVectorFire()->at(b);
        if (enemyBoat->getColliderFire()->getGlobalBounds().intersects(
                fire->getSprite()->getGlobalBounds())) {
          this->playerBoat.getVectorFire()->erase(
              this->playerBoat.getVectorFire()->begin() + b);
          enemyBoat->decreaseHp(10);
        }
      }

      // todo: check hp
      if (enemyBoat->getHp() <= 0) {
        this->vectorEnemyBoat.erase(this->vectorEnemyBoat.begin() + a);
      }
    }

    // todo: reset view
    this->window->setView(this->window->getDefaultView());

    this->drawGuiProfile(window);

    // todo: set view
    this->window->setView(this->view);
  }
};

#endif