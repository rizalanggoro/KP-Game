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
  vector<Fire> vectorFire{};

  string playerDirection = "r";
  int boatLevel = 1;

  float boatSize = 128;
  float boatTargetSize = 80;
  float boatScaleFactor = 1;

  float velocity = 5;
  float bulletVelocity = 10;

  RectangleShape colliderBox{}, colliderBoxEnemy{}, colliderBoxFire{};
  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};
  RectangleShape colliderUpEnemy{}, colliderDownEnemy{}, colliderRightEnemy{},
      colliderLeftEnemy{};

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
  RectangleShape *getColliderBoxFire() { return &this->colliderBoxFire; }
  RectangleShape *getColliderUpEnemy() { return &this->colliderUpEnemy; }
  RectangleShape *getColliderDownEnemy() { return &this->colliderDownEnemy; }
  RectangleShape *getColliderRightEnemy() { return &this->colliderRightEnemy; }
  RectangleShape *getColliderLeftEnemy() { return &this->colliderLeftEnemy; }

  float getVelocity() { return this->velocity; }
  void setVelocity(float velocity) { this->velocity = velocity; }
  void setBulletVelocity(float bulletVelocity) {
    this->bulletVelocity = bulletVelocity;
  }
  float getBulletVelocity() { return this->bulletVelocity; }
  float getBoatTargetSize() { return this->boatTargetSize; }
  Sprite *getSprite() { return &this->player; }

  RectangleShape *getColliderBox() { return &this->colliderBox; };
  RectangleShape *getColliderUp() { return &this->colliderUp; }
  RectangleShape *getColliderDown() { return &this->colliderDown; }
  RectangleShape *getColliderRight() { return &this->colliderRight; }
  RectangleShape *getColliderLeft() { return &this->colliderLeft; }

  int getCurrentBoatIndex() { return this->currentBoatIndex; }
  vector<Fire> *getVectorFire() { return &this->vectorFire; }

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

    if (this->frameCannonInterval >=
        this->frameCannonDelay /
            this->asset->getVectorBoats()->at(this->boatLevel - 1).size()) {
      if (this->currFrameCannonIndex <
          this->asset->getVectorBoats()->at(this->boatLevel - 1).size() - 1)
        this->currFrameCannonIndex++;
      else {
        this->currFrameCannonIndex = 0;

        auto playerPos = this->player.getPosition();
        Fire newFire{this->asset, this->playerDirection};
        newFire.setPosition(Vector2f(playerPos.x, playerPos.y));
        newFire.setVelocity(this->bulletVelocity);

        this->vectorFire.push_back(newFire);
      }

      this->clockFrameCannon.restart();
    }
  }

  PlayerBoat(Asset *asset, TilemapWar *tilemap) {
    this->asset = asset;
    this->tilemap = tilemap;

    this->boatScaleFactor = this->boatTargetSize / this->boatSize;

    // this->boatCannonTargetSize = this->boatTargetSize / 2;
    // this->playerCannon.setTexture(this->asset->getVectorCannon4()->at(0));
    // this->boatCannonSize = this->playerCannon.getGlobalBounds().width;
    // this->boatCannonScaleFactor =
    //     this->boatCannonTargetSize / this->boatCannonSize;
  }

  void setLevel(int level) { this->boatLevel = level; }

  void draw(RenderWindow &window) {
    // todo: draw boat
    this->player.setTexture(this->asset->getVectorBoats()
                                ->at(this->boatLevel - 1)
                                .at(this->currFrameCannonIndex));
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

    // todo: draw collider box enemy
    this->colliderBoxEnemy.setFillColor(Color::Transparent);
    this->colliderBoxEnemy.setPosition(playerPos.x - this->boatTargetSize * 2,
                                       playerPos.y - this->boatTargetSize * 2);
    this->colliderBoxEnemy.setOutlineThickness(-2);
    this->colliderBoxEnemy.setOutlineColor(Color(255, 0, 0, 50));
    this->colliderBoxEnemy.setSize(
        Vector2f(this->boatTargetSize * 4, this->boatTargetSize * 4));

    window.draw(this->colliderBoxEnemy);

    // todo: draw collider enemy
    auto hColliderEnemySize =
        Vector2f(this->boatTargetSize * 4, this->boatTargetSize * 2);
    auto vColliderEnemySize =
        Vector2f(this->boatTargetSize * 2, this->boatTargetSize * 4);

    this->colliderUpEnemy.setSize(hColliderEnemySize);
    this->colliderDownEnemy.setSize(hColliderEnemySize);
    this->colliderRightEnemy.setSize(vColliderEnemySize);
    this->colliderLeftEnemy.setSize(vColliderEnemySize);

    auto colliderEnemyColor = Color(0, 0, 255, 50);
    this->colliderUpEnemy.setFillColor(colliderEnemyColor);
    this->colliderDownEnemy.setFillColor(colliderEnemyColor);
    this->colliderRightEnemy.setFillColor(colliderEnemyColor);
    this->colliderLeftEnemy.setFillColor(colliderEnemyColor);

    auto colliderBoxEnemyPos = this->colliderBoxEnemy.getPosition();
    this->colliderUpEnemy.setPosition(Vector2f(
        colliderBoxEnemyPos.x, colliderBoxEnemyPos.y - this->velocity * 2));
    this->colliderDownEnemy.setPosition(Vector2f(
        colliderBoxEnemyPos.x,
        colliderBoxEnemyPos.y + this->boatTargetSize * 2 + this->velocity * 2));
    this->colliderRightEnemy.setPosition(Vector2f(
        colliderBoxEnemyPos.x + this->boatTargetSize * 2 + this->velocity * 2,
        colliderBoxEnemyPos.y));
    this->colliderLeftEnemy.setPosition(Vector2f(
        colliderBoxEnemyPos.x - this->velocity * 2, colliderBoxEnemyPos.y));

    window.draw(this->colliderUpEnemy);
    window.draw(this->colliderDownEnemy);
    window.draw(this->colliderRightEnemy);
    window.draw(this->colliderLeftEnemy);

    // todo: draw collider box fire
    this->colliderBoxFire.setFillColor(Color::Transparent);
    this->colliderBoxFire.setOutlineThickness(-2);
    this->colliderBoxFire.setOutlineColor(Color(0, 255, 0, 255));
    if (this->playerDirection == "u" || this->playerDirection == "d") {
      this->colliderBoxFire.setSize(
          Vector2f(this->boatTargetSize / 2, this->boatTargetSize));
      this->colliderBoxFire.setPosition(playerPos.x - this->boatTargetSize / 4,
                                        playerPos.y - this->boatTargetSize / 2);
    } else if (this->playerDirection == "r" || this->playerDirection == "l") {
      this->colliderBoxFire.setSize(
          Vector2f(this->boatTargetSize, this->boatTargetSize / 2));
      this->colliderBoxFire.setPosition(playerPos.x - this->boatTargetSize / 2,
                                        playerPos.y - this->boatTargetSize / 4);
    } else {
      this->colliderBoxFire.setSize(
          Vector2f(this->boatTargetSize, this->boatTargetSize));
      this->colliderBoxFire.setPosition(playerPos.x - this->boatTargetSize / 2,
                                        playerPos.y - this->boatTargetSize / 2);
    }

    window.draw(this->colliderBoxFire);

    // todo: draw fire
    for (int a = 0; a < this->vectorFire.size(); a++) {
      Fire *fire = &this->vectorFire.at(a);
      fire->draw(window);

      if (this->tilemap->isFireCollided(fire->getFireCollider()))
        this->vectorFire.erase(this->vectorFire.begin() + a);

      if (this->tilemap->isFireCollidedMap(fire->getFireCollider()))
        this->vectorFire.erase(this->vectorFire.begin() + a);
    }
  }
};

#endif