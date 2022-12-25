#ifndef player_boat_hpp
#define player_boat_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class PlayerBoat {
 private:
  Asset *asset;
  TilemapWar *tilemap;
  Sprite player{};

  float boatSize = 128;
  float boatTargetSize = 80;
  float boatScaleFactor = 1;

  float velocity = 3;

  RectangleShape colliderBox{};
  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};

 public:
  float getVelocity() { return this->velocity; }
  float getBoatTargetSize() { return this->boatTargetSize; }
  Sprite *getSprite() { return &this->player; }
  RectangleShape *getColliderBox() { return &this->colliderBox; };
  RectangleShape *getColliderUp() { return &this->colliderUp; }
  RectangleShape *getColliderDown() { return &this->colliderDown; }
  RectangleShape *getColliderRight() { return &this->colliderRight; }
  RectangleShape *getColliderLeft() { return &this->colliderLeft; }

  void moveUpRight() {
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(speed, -speed);
    this->player.setRotation(225);
  }
  void moveUpLeft() {
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(-speed, -speed);
    this->player.setRotation(135);
  }
  void moveDownRight() {
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(speed, speed);
    this->player.setRotation(315);
  }
  void moveDownLeft() {
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(-speed, speed);
    this->player.setRotation(45);
  }
  void moveUp() {
    this->player.move(0, -this->velocity);
    this->player.setRotation(180);
  }
  void moveDown() {
    this->player.move(0, this->velocity);
    this->player.setRotation(0);
  }
  void moveRight() {
    this->player.move(this->velocity, 0);
    this->player.setRotation(270);
  }
  void moveLeft() {
    this->player.move(-this->velocity, 0);
    this->player.setRotation(90);
  }

  PlayerBoat(Asset *asset, TilemapWar *tilemap) {
    this->asset = asset;
    this->tilemap = tilemap;

    this->boatScaleFactor = this->boatTargetSize / this->boatSize;

    this->player.setPosition(
        10 * this->tilemap->getTileTargetSize() - this->boatTargetSize / 2,
        this->tilemap->getHeight() * this->tilemap->getTileTargetSize() / 2 -
            this->boatTargetSize / 2);
  }

  void draw(RenderWindow &window) {
    this->player.setTexture(this->asset->getVectorBoatColor1()->at(0));
    this->player.setOrigin(this->boatSize / 2, this->boatSize / 2);
    this->player.setScale(this->boatScaleFactor, this->boatScaleFactor);

    window.draw(this->player);

    // todo: draw rect
    auto playerPos = this->player.getPosition();

    this->colliderBox.setFillColor(Color::Transparent);
    this->colliderBox.setPosition(playerPos.x - this->boatTargetSize / 2,
                                  playerPos.y - this->boatTargetSize / 2);
    this->colliderBox.setOutlineThickness(-2);
    this->colliderBox.setOutlineColor(Color(255, 0, 0, 50));
    this->colliderBox.setSize(
        Vector2f(this->boatTargetSize, this->boatTargetSize));

    window.draw(this->colliderBox);

    // todo: draw collider
    auto hColliderSize = Vector2f(this->boatTargetSize, this->velocity * 2);
    auto vColliderSize = Vector2f(this->velocity * 2, this->boatTargetSize);

    this->colliderUp.setSize(hColliderSize);
    this->colliderDown.setSize(hColliderSize);
    this->colliderRight.setSize(vColliderSize);
    this->colliderLeft.setSize(vColliderSize);

    auto colliderColor = Color(0, 0, 255, 50);
    this->colliderUp.setFillColor(colliderColor);
    this->colliderDown.setFillColor(colliderColor);
    this->colliderRight.setFillColor(colliderColor);
    this->colliderLeft.setFillColor(colliderColor);

    auto colliderBoxPos = this->colliderBox.getPosition();
    this->colliderUp.setPosition(
        Vector2f(colliderBoxPos.x, colliderBoxPos.y - this->velocity * 2));
    this->colliderDown.setPosition(
        Vector2f(colliderBoxPos.x, colliderBoxPos.y + this->boatTargetSize));
    this->colliderRight.setPosition(
        Vector2f(colliderBoxPos.x + this->boatTargetSize, colliderBoxPos.y));
    this->colliderLeft.setPosition(
        Vector2f(colliderBoxPos.x - this->velocity * 2, colliderBoxPos.y));

    window.draw(this->colliderUp);
    window.draw(this->colliderDown);
    window.draw(this->colliderRight);
    window.draw(this->colliderLeft);
  }
};

#endif