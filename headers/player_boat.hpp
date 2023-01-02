#ifndef player_boat_hpp
#define player_boat_hpp

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "direction.hpp"
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

  Direction playerDirection = DIR_RIGHT;
  int boatLevel = 1;

  float boatSize = 128;
  float boatTargetSize = 80;
  float boatScaleFactor = 1;

  float velocity = 5;
  float bulletVelocity = 10;

  Clock clockMultiplier{};
  float multiplier = 1;

  RectangleShape colliderBox{}, colliderBoxEnemy{}, colliderBoxFire{};
  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};
  RectangleShape colliderUpEnemy{}, colliderDownEnemy{}, colliderRightEnemy{},
      colliderLeftEnemy{};

  Clock clockFrameCannon{};
  float frameCannonDelay = 250;
  float frameCannonInterval = 0;
  int currFrameCannonIndex = 0;

  int currentBoatIndex = 0;

  Sound soundBullet{};

  int parseDirection(Direction dir) {
    int angle = 0;
    if (dir == DIR_UP_RIGHT) angle = 225;
    if (dir == DIR_UP_LEFT) angle = 135;
    if (dir == DIR_DOWN_RIGHT) angle = 315;
    if (dir == DIR_DOWN_LEFT) angle = 45;
    if (dir == DIR_UP) angle = 180;
    if (dir == DIR_DOWN) angle = 0;
    if (dir == DIR_RIGHT) angle = 270;
    if (dir == DIR_LEFT) angle = 90;

    return angle;
  }

 public:
  //  todo: getters
  RectangleShape *getColliderBoxFire() { return &this->colliderBoxFire; }
  RectangleShape *getColliderUpEnemy() { return &this->colliderUpEnemy; }
  RectangleShape *getColliderDownEnemy() { return &this->colliderDownEnemy; }
  RectangleShape *getColliderRightEnemy() { return &this->colliderRightEnemy; }
  RectangleShape *getColliderLeftEnemy() { return &this->colliderLeftEnemy; }

  RectangleShape *getColliderBox() { return &this->colliderBox; };
  RectangleShape *getColliderUp() { return &this->colliderUp; }
  RectangleShape *getColliderDown() { return &this->colliderDown; }
  RectangleShape *getColliderRight() { return &this->colliderRight; }
  RectangleShape *getColliderLeft() { return &this->colliderLeft; }

  Sprite *getSprite() { return &this->player; }
  vector<Fire> *getVectorFire() { return &this->vectorFire; }

  int getCurrentBoatIndex() { return this->currentBoatIndex; }
  float getVelocity() { return this->velocity; }
  float getBulletVelocity() { return this->bulletVelocity; }
  float getBoatTargetSize() { return this->boatTargetSize; }

  // todo: setters
  void setVelocity(float velocity) { this->velocity = velocity; }
  void setBulletVelocity(float bulletVelocity) {
    this->bulletVelocity = bulletVelocity;
  }
  void setMultiplier(float multiplier) { this->multiplier = multiplier; }
  void setLevel(int level) { this->boatLevel = level; }

  void moveUpRight() {
    this->playerDirection = DIR_UP_RIGHT;
    auto speed = this->velocity * sqrt(2) / 2 * this->multiplier;
    this->player.move(speed, -speed);
  }
  void moveUpLeft() {
    this->playerDirection = DIR_UP_LEFT;
    auto speed = this->velocity * sqrt(2) / 2 * this->multiplier;
    this->player.move(-speed, -speed);
  }
  void moveDownRight() {
    this->playerDirection = DIR_DOWN_RIGHT;
    auto speed = this->velocity * sqrt(2) / 2 * this->multiplier;
    this->player.move(speed, speed);
  }
  void moveDownLeft() {
    this->playerDirection = DIR_DOWN_LEFT;
    auto speed = this->velocity * sqrt(2) / 2 * this->multiplier;
    this->player.move(-speed, speed);
  }
  void moveUp() {
    this->playerDirection = DIR_UP;
    this->player.move(0, -this->velocity * this->multiplier);
  }
  void moveDown() {
    this->playerDirection = DIR_DOWN;
    this->player.move(0, this->velocity * this->multiplier);
  }
  void moveRight() {
    this->playerDirection = DIR_RIGHT;
    this->player.move(this->velocity * this->multiplier, 0);
  }
  void moveLeft() {
    this->playerDirection = DIR_LEFT;
    this->player.move(-this->velocity * this->multiplier, 0);
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

        this->soundBullet.play();
      }

      this->clockFrameCannon.restart();
    }
  }

  PlayerBoat(Asset *asset, TilemapWar *tilemap) {
    this->asset = asset;
    this->tilemap = tilemap;

    this->boatScaleFactor = this->boatTargetSize / this->boatSize;
    this->soundBullet.setBuffer(*this->asset->getSoundBullet());
  }

  void draw(RenderWindow &window) {
    // todo: init multiplier
    this->multiplier = this->clockMultiplier.restart().asSeconds() * 60;

    // todo: draw boat
    this->player.setTexture(this->asset->getVectorBoats()
                                ->at(this->boatLevel - 1)
                                .at(this->currFrameCannonIndex));
    this->player.setOrigin(this->boatSize / 2, this->boatSize / 2);
    this->player.setRotation(this->parseDirection(this->playerDirection));
    this->player.setScale(this->boatScaleFactor, this->boatScaleFactor);

    window.draw(this->player);

    // todo: draw rect
    auto playerPos = this->player.getPosition();
    this->colliderBox.setFillColor(Color::Transparent);
    this->colliderBox.setPosition(playerPos.x - this->boatTargetSize / 2,
                                  playerPos.y - this->boatTargetSize / 2);
    this->colliderBox.setOutlineThickness(-2);
    this->colliderBox.setOutlineColor(Color::Transparent);
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

    auto colliderColor = Color::Transparent;
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
    this->colliderBoxEnemy.setOutlineColor(Color::Transparent);
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

    auto colliderEnemyColor = Color::Transparent;
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
    this->colliderBoxFire.setOutlineColor(Color::Transparent);
    if (this->playerDirection == DIR_UP || this->playerDirection == DIR_DOWN) {
      this->colliderBoxFire.setSize(
          Vector2f(this->boatTargetSize / 2, this->boatTargetSize));
      this->colliderBoxFire.setPosition(playerPos.x - this->boatTargetSize / 4,
                                        playerPos.y - this->boatTargetSize / 2);
    } else if (this->playerDirection == DIR_RIGHT ||
               this->playerDirection == DIR_LEFT) {
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