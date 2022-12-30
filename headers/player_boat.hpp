#ifndef player_boat_hpp
#define player_boat_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "fire.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class PlayerBoat {
 private:
  Asset *asset;
  TilemapWar *tilemap;
  Sprite player{};
  // Sprite playerCannon{};
  vector<Fire> vectorFire{};

  string playerDirection = "d";
  int boatLevel = 1;

  float boatSize = 128;
  float boatTargetSize = 80;
  // float boatTargetSize = 256;
  float boatScaleFactor = 1;

  // float boatCannonSize = 0;
  // float boatCannonTargetSize = 1;
  // float boatCannonScaleFactor = 1;

  float velocity = 5;

  RectangleShape colliderBox{};
  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};

  Clock clockFrameCannon{};
  float frameCannonDelay = 250;
  float frameCannonInterval = 0;
  int currFrameCannonIndex = 0;

  int currentBoatIndex = 0;

  int parseDirection(string dir) {
    int angle = 0;
    if (dir == "ur") angle = 225;
    if (dir == "ul") angle = 135;
    if (dir == "dr") angle = 315;
    if (dir == "dl") angle = 45;
    if (dir == "u") angle = 180;
    if (dir == "d") angle = 0;
    if (dir == "r") angle = 270;
    if (dir == "l") angle = 90;

    return angle;
  }

 public:
  float getVelocity() { return this->velocity; }
  void setVelocity(float velocity) { this->velocity = velocity; }
  float getBoatTargetSize() { return this->boatTargetSize; }
  Sprite *getSprite() { return &this->player; }
  RectangleShape *getColliderBox() { return &this->colliderBox; };
  RectangleShape *getColliderUp() { return &this->colliderUp; }
  RectangleShape *getColliderDown() { return &this->colliderDown; }
  RectangleShape *getColliderRight() { return &this->colliderRight; }
  RectangleShape *getColliderLeft() { return &this->colliderLeft; }
  int getCurrentBoatIndex() { return this->currentBoatIndex; }
  vector<Fire> *getVectorFire() { return &this->vectorFire; }

  // void nextBoat() {
  //   if (this->currentBoatIndex < this->asset->getVectorBoatColor1()->size() -
  //   1)
  //     this->currentBoatIndex++;
  //   else
  //     this->currentBoatIndex = 0;
  // }
  // void previousBoat() {
  //   if (this->currentBoatIndex > 0)
  //     this->currentBoatIndex--;
  //   else
  //     this->currentBoatIndex = this->asset->getVectorBoatColor1()->size() -
  //     1;
  // }

  void moveUpRight() {
    this->playerDirection = "ur";
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(speed, -speed);
  }
  void moveUpLeft() {
    this->playerDirection = "ul";
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(-speed, -speed);
  }
  void moveDownRight() {
    this->playerDirection = "dr";
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(speed, speed);
  }
  void moveDownLeft() {
    this->playerDirection = "dl";
    auto speed = this->velocity * sqrt(2) / 2;
    this->player.move(-speed, speed);
  }
  void moveUp() {
    this->playerDirection = "u";
    this->player.move(0, -this->velocity);
  }
  void moveDown() {
    this->playerDirection = "d";
    this->player.move(0, this->velocity);
  }
  void moveRight() {
    this->playerDirection = "r";
    this->player.move(this->velocity, 0);
  }
  void moveLeft() {
    this->playerDirection = "l";
    this->player.move(-this->velocity, 0);
  }

  void unfire() { this->currFrameCannonIndex = 0; }

  void fire() {
    this->frameCannonInterval =
        this->clockFrameCannon.getElapsedTime().asMilliseconds();

    // if (this->frameCannonInterval >=
    //     this->frameCannonDelay / this->asset->getVectorCannon4()->size()) {
    //   if (this->currFrameCannonIndex <
    //       this->asset->getVectorCannon4()->size() - 1)
    //     this->currFrameCannonIndex++;
    //   else {
    //     this->currFrameCannonIndex = 0;

    //     auto playerPos = this->player.getPosition();
    //     Fire newFire{this->asset, this->playerDirection};
    //     newFire.setPosition(Vector2f(playerPos.x, playerPos.y));
    //     newFire.setVelocity(this->velocity * 2);

    //     this->vectorFire.push_back(newFire);
    //   }

    //   this->clockFrameCannon.restart();
    // }
  }

  PlayerBoat(Asset *asset, TilemapWar *tilemap) {
    this->asset = asset;
    this->tilemap = tilemap;

    this->boatScaleFactor = this->boatTargetSize / this->boatSize;

    this->player.setPosition(
        25 * this->tilemap->getTileTargetSize() - this->boatTargetSize / 2,
        this->tilemap->getHeight() * this->tilemap->getTileTargetSize() / 2 -
            this->boatTargetSize / 2);

    // this->boatCannonTargetSize = this->boatTargetSize / 2;
    // this->playerCannon.setTexture(this->asset->getVectorCannon4()->at(0));
    // this->boatCannonSize = this->playerCannon.getGlobalBounds().width;
    // this->boatCannonScaleFactor =
    //     this->boatCannonTargetSize / this->boatCannonSize;
  }

  void setLevel(int level) { this->boatLevel = level; }

  void draw(RenderWindow &window) {
    // todo: draw boat
    this->player.setTexture(
        this->asset->getVectorBoats()->at(this->boatLevel - 1).at(0));
    this->player.setOrigin(this->boatSize / 2, this->boatSize / 2);
    this->player.setRotation(this->parseDirection(this->playerDirection));
    this->player.setScale(this->boatScaleFactor, this->boatScaleFactor);

    window.draw(this->player);

    // todo: draw cannon
    auto playerPos = this->player.getPosition();
    // this->playerCannon.setTexture(
    //     this->asset->getVectorCannon4()->at(this->currFrameCannonIndex));
    // this->playerCannon.setPosition(playerPos.x, playerPos.y);
    // this->playerCannon.setOrigin(boatCannonSize / 2, boatCannonSize / 2);
    // this->playerCannon.setRotation(this->parseDirection(this->playerDirection));
    // this->playerCannon.setScale(this->boatCannonScaleFactor,
    //                             this->boatCannonScaleFactor);

    // window.draw(this->playerCannon);

    // todo: draw rect
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

    // todo: draw fire
    for (int a = 0; a < this->vectorFire.size(); a++) {
      Fire *fire = &this->vectorFire.at(a);
      fire->draw(window);

      if (this->tilemap->isFireCollided(fire->getFireCollider()))
        this->vectorFire.erase(this->vectorFire.begin() + a);
    }
  }
};

#endif