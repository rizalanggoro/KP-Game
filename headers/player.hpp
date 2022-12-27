#ifndef player_hpp
#define player_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"

using namespace std;
using namespace sf;

class Player {
 private:
  Asset *asset;
  float velocity = 2;

  Sprite spritePlayer{};
  Clock clockPlayer;
  float frameInterval;
  float frameDelay = 250;
  int frameIndex = 0;
  int frameDirIndex = 0;
  bool isPlayerIdle = true;

  float playerSize = 48;
  float playerTargetSize = 48 * 3;
  float playerScaleFactor = 1;

  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};

  void handleKeyboard() {
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
      this->isPlayerIdle = false;
      this->frameDirIndex = 0;
    } else if (Keyboard::isKeyPressed(Keyboard::Up)) {
      this->isPlayerIdle = false;
      this->frameDirIndex = 1;
    } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      this->isPlayerIdle = false;
      this->frameDirIndex = 2;
    } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
      this->isPlayerIdle = false;
      this->frameDirIndex = 3;
    }
  }

 public:
  // todo: getters
  Sprite *getSpritePlayer() { return &this->spritePlayer; };
  float getVelocity() { return this->velocity; };
  RectangleShape *getColliderUp() { return &this->colliderUp; };
  RectangleShape *getColliderDown() { return &this->colliderDown; };
  RectangleShape *getColliderRight() { return &this->colliderRight; };
  RectangleShape *getColliderLeft() { return &this->colliderLeft; };

  Player(Asset *asset) {
    this->asset = asset;
    this->playerScaleFactor = this->playerTargetSize / this->playerSize;
  }

  void moveUp() {}
  void moveDown() {}
  void moveRight() {}
  void moveLeft() {}
  void moveUpRight() {}
  void moveUpLeft() {}
  void moveDownRight() {}
  void moveDownLeft() {}
  void setPosition(float x, float y) { this->spritePlayer.setPosition(x, y); }

  void draw(RenderWindow &window) {
    this->handleKeyboard();

    // todo: handle player frame
    this->frameInterval = this->clockPlayer.getElapsedTime().asMilliseconds();
    if (this->frameInterval >= this->frameDelay) {
      if (this->frameIndex < 3)
        this->frameIndex++;
      else
        this->frameIndex = 0;
      this->clockPlayer.restart();
    }

    // todo: draw player
    spritePlayer.setTexture(this->asset->getVectorCharSpite()
                                ->at(this->frameDirIndex)
                                .at(this->isPlayerIdle ? 0 : this->frameIndex));
    this->spritePlayer.setScale(this->playerScaleFactor,
                                this->playerScaleFactor);
    window.draw(spritePlayer);

    // todo: draw collider box
    auto playerPos = spritePlayer.getPosition();
    auto playerGlobalBounds = spritePlayer.getGlobalBounds();

    RectangleShape rectColliderBox{};
    rectColliderBox.setOutlineColor(Color(255, 0, 0, 50));
    rectColliderBox.setOutlineThickness(-this->velocity);
    // rectColliderBox.setFillColor(Color::Red);
    rectColliderBox.setFillColor(Color::Transparent);
    rectColliderBox.setSize(Vector2f(this->playerSize, this->playerSize));
    rectColliderBox.setPosition(
        playerPos.x + this->playerSize,
        playerPos.y + this->playerSize + this->playerSize / 3);
    window.draw(rectColliderBox);

    // todo: draw collider
    auto colliderBoxPos = rectColliderBox.getPosition();
    auto colliderBoxGlobalBounds = rectColliderBox.getGlobalBounds();

    int colliderSize = this->playerSize / 3;

    colliderUp.setSize(Vector2f(colliderSize, this->velocity * 2));
    colliderDown.setSize(Vector2f(colliderSize, this->velocity * 2));
    colliderRight.setSize(Vector2f(this->velocity * 2, colliderSize));
    colliderLeft.setSize(Vector2f(this->velocity * 2, colliderSize));

    auto color = Color(0, 255, 0, 255);
    colliderUp.setFillColor(color);
    colliderDown.setFillColor(color);
    colliderRight.setFillColor(color);
    colliderLeft.setFillColor(color);

    colliderUp.setPosition(
        colliderBoxPos.x + (this->playerSize - colliderSize) / 2,
        colliderBoxPos.y - this->velocity * 2);
    colliderDown.setPosition(
        colliderBoxPos.x + (this->playerSize - colliderSize) / 2,
        colliderBoxPos.y + this->playerSize);
    colliderRight.setPosition(
        colliderBoxPos.x + this->playerSize,
        colliderBoxPos.y + +(this->playerSize - colliderSize) / 4);
    colliderLeft.setPosition(
        colliderBoxPos.x - this->velocity * 2,
        colliderBoxPos.y + (this->playerSize - colliderSize) / 4);

    // todo: -> collider up
    // todo: -> collider down
    // todo: -> collider right
    // todo: -> collider left
    window.draw(colliderUp);
    window.draw(colliderDown);
    window.draw(colliderRight);
    window.draw(colliderLeft);
  }

  void handleEvent(Event &event) {
    if (event.type == Event::KeyReleased) {
      auto code = event.key.code;
      if (code == Keyboard::Down) {
        this->isPlayerIdle = true;
        this->frameDirIndex = 0;
        this->frameIndex = 0;
      }

      else if (code == Keyboard::Up) {
        this->isPlayerIdle = true;
        this->frameDirIndex = 1;
        this->frameIndex = 0;
      }

      else if (code == Keyboard::Left) {
        this->isPlayerIdle = true;
        this->frameDirIndex = 2;
        this->frameIndex = 0;
      }

      else if (code == Keyboard::Right) {
        this->isPlayerIdle = true;
        this->frameDirIndex = 3;
        this->frameIndex = 0;
      }
    }
  }
};

#endif