#ifndef enemy_boat_hpp
#define enemy_boat_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "fire.hpp"
#include "player_boat.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class EnemyBoat {
 private:
  Asset *asset;
  TilemapWar *tilemap;
  PlayerBoat *playerBoat;

  int boatLevel = 1;
  float boatSize = 128;
  float boatTargetSize = 80;
  float boatScaleFactor = 1;
  string boatDirection = "d";

  int hp = 100;
  int maxHp = 100;

  Sprite enemy{};
  float enemyVelocity = 0;

  RectangleShape colliderBox{}, colliderFire{};
  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};

  Clock clockFire;
  float fireInterval = 0;
  float fireDelay = 500;
  float fireFrameIndex = 0;

  vector<Fire> vectorFire{};

  void fire(string dir) {
    this->fireInterval = this->clockFire.getElapsedTime().asMilliseconds();

    if (this->fireInterval >=
        this->fireDelay /
            this->asset->getVectorBoats()->at(this->boatLevel).size()) {
      if (this->fireFrameIndex <
          this->asset->getVectorBoats()->at(this->boatLevel).size() - 1)
        this->fireFrameIndex++;
      else {
        this->fireFrameIndex = 0;

        auto playerPos = this->enemy.getPosition();
        Fire newFire{this->asset, dir};
        newFire.setPosition(Vector2f(playerPos.x, playerPos.y));
        newFire.setVelocity(this->playerBoat->getBulletVelocity() * .8);

        this->vectorFire.push_back(newFire);
      }

      this->clockFire.restart();
    }
  }

 public:
  // todo: getters
  RectangleShape *getColliderFire() { return &this->colliderFire; }
  int getHp() { return this->hp; }
  void decreaseHp(int num) { this->hp -= num; }
  vector<Fire> *getVectorFire() { return &this->vectorFire; }
  void setBoatLevel(int level) { this->boatLevel = level; }

  EnemyBoat(Asset *asset, TilemapWar *tilemap, PlayerBoat *playerBoat) {
    this->asset = asset;
    this->tilemap = tilemap;
    this->playerBoat = playerBoat;

    this->enemyVelocity = this->playerBoat->getVelocity() * .4;
    this->boatScaleFactor = this->boatTargetSize / this->boatSize;
  }

  void setInitPos(int tileC, int tileR) {
    this->enemy.setPosition(
        tileC * this->tilemap->getTileTargetSize() - this->boatTargetSize / 2,
        tileR * this->tilemap->getTileTargetSize() - this->boatTargetSize / 2);
  }

  void draw(RenderWindow &window) {
    this->enemy.setTexture(this->asset->getVectorBoats()
                               ->at(this->boatLevel)
                               .at(this->fireFrameIndex));
    this->enemy.setColor(Color(163, 91, 112, 255));
    this->enemy.setOrigin(this->boatSize / 2, this->boatSize / 2);
    this->enemy.setScale(this->boatScaleFactor, this->boatScaleFactor);
    window.draw(this->enemy);

    auto playerPos = this->playerBoat->getSprite()->getPosition();
    auto enemyPos = this->enemy.getPosition();

    // todo: draw collider box
    this->colliderBox.setFillColor(Color::Transparent);
    this->colliderBox.setOutlineColor(Color::Transparent);
    this->colliderBox.setOutlineThickness(-2);
    this->colliderBox.setPosition(enemyPos.x - this->boatTargetSize / 2,
                                  enemyPos.y - this->boatTargetSize / 2);
    this->colliderBox.setSize(
        Vector2f(this->boatTargetSize, this->boatTargetSize));
    window.draw(this->colliderBox);

    auto colliderBoxPos = this->colliderBox.getPosition();

    // todo: draw hp bar
    float hpPercentage = float(this->hp) / float(this->maxHp);
    RectangleShape rectHp{};
    rectHp.setFillColor(Color::Red);
    rectHp.setOutlineColor(Color::Black);
    rectHp.setOutlineThickness(2);
    rectHp.setSize(Vector2f(hpPercentage * this->boatTargetSize, 4));
    rectHp.setPosition(colliderBoxPos.x, colliderBoxPos.y - 6);
    window.draw(rectHp);

    // todo: draw collider
    auto hColliderSize =
        Vector2f(this->boatTargetSize, this->enemyVelocity * 2);
    auto vColliderSize =
        Vector2f(this->enemyVelocity * 2, this->boatTargetSize);

    this->colliderUp.setSize(hColliderSize);
    this->colliderDown.setSize(hColliderSize);
    this->colliderRight.setSize(vColliderSize);
    this->colliderLeft.setSize(vColliderSize);

    auto colliderColor = Color::Transparent;
    this->colliderUp.setFillColor(colliderColor);
    this->colliderDown.setFillColor(colliderColor);
    this->colliderRight.setFillColor(colliderColor);
    this->colliderLeft.setFillColor(colliderColor);

    this->colliderUp.setPosition(
        Vector2f(colliderBoxPos.x, colliderBoxPos.y - this->enemyVelocity * 2));
    this->colliderDown.setPosition(
        Vector2f(colliderBoxPos.x, colliderBoxPos.y + this->boatTargetSize));
    this->colliderRight.setPosition(
        Vector2f(colliderBoxPos.x + this->boatTargetSize, colliderBoxPos.y));
    this->colliderLeft.setPosition(
        Vector2f(colliderBoxPos.x - this->enemyVelocity * 2, colliderBoxPos.y));

    window.draw(this->colliderUp);
    window.draw(this->colliderDown);
    window.draw(this->colliderRight);
    window.draw(this->colliderLeft);

    auto dx = playerPos.x - enemyPos.x;
    auto dy = playerPos.y - enemyPos.y;

    // todo: enemy move
    if (enemyPos.x < playerPos.x &&
        this->tilemap->canMove(&this->colliderRight)) {
      // todo: move right
      if (this->playerBoat->getColliderLeftEnemy()
              ->getGlobalBounds()
              .intersects(this->colliderRight.getGlobalBounds())) {
        this->fire("r");
        this->boatDirection = "r";
        this->enemy.setRotation(270);
      } else {
        this->boatDirection = "r";
        this->enemy.setRotation(270);
        if (dx < this->enemyVelocity)
          this->enemy.setPosition(playerPos.x, enemyPos.y);
        else
          this->enemy.move(this->enemyVelocity, 0);
      }
    } else if (enemyPos.x > playerPos.x &&
               this->tilemap->canMove(&this->colliderLeft)) {
      // todo: move left
      if (this->playerBoat->getColliderRightEnemy()
              ->getGlobalBounds()
              .intersects(this->colliderLeft.getGlobalBounds())) {
        this->fire("l");
        this->boatDirection = "l";
        this->enemy.setRotation(90);
      } else {
        this->boatDirection = "l";
        this->enemy.setRotation(90);
        if (dx > this->enemyVelocity)
          this->enemy.setPosition(playerPos.x, enemyPos.y);
        else
          this->enemy.move(-this->enemyVelocity, 0);
      }
    } else if (enemyPos.y < playerPos.y &&
               this->tilemap->canMove(&this->colliderDown)) {
      // todo: move down
      if (this->playerBoat->getColliderUpEnemy()->getGlobalBounds().intersects(
              this->colliderDown.getGlobalBounds())) {
        this->fire("d");
        this->boatDirection = "d";
        this->enemy.setRotation(0);
      } else {
        this->boatDirection = "d";
        this->enemy.setRotation(0);
        if (dy < this->enemyVelocity)
          this->enemy.setPosition(enemyPos.x, playerPos.y);
        else
          this->enemy.move(0, this->enemyVelocity);
      }
    } else if (enemyPos.y > playerPos.y &&
               this->tilemap->canMove(&this->colliderUp)) {
      // todo: move up
      if (this->playerBoat->getColliderDownEnemy()
              ->getGlobalBounds()
              .intersects(this->colliderUp.getGlobalBounds())) {
        this->fire("u");
        this->boatDirection = "u";
        this->enemy.setRotation(180);
      } else {
        this->boatDirection = "u";
        this->enemy.setRotation(180);
        if (dy > this->enemyVelocity)
          this->enemy.setPosition(enemyPos.x, playerPos.y);
        else
          this->enemy.move(0, -this->enemyVelocity);
      }
    }

    // todo: draw collider fire
    auto dir = this->boatDirection;
    float colliderFireX = (dir == "u" || dir == "d") ? this->boatTargetSize / 2
                                                     : this->boatTargetSize;
    float colliderFireY = (dir == "u" || dir == "d") ? this->boatTargetSize
                                                     : this->boatTargetSize / 2;
    float colliderFirePosX =
        colliderBoxPos.x +
        ((dir == "u" || dir == "d") ? this->boatTargetSize / 4 : 0);
    float colliderFirePoxY =
        colliderBoxPos.y +
        ((dir == "u" || dir == "d") ? 0 : this->boatTargetSize / 4);

    this->colliderFire.setFillColor(Color::Transparent);
    this->colliderFire.setOutlineColor(Color::Transparent);
    this->colliderFire.setOutlineThickness(2);
    this->colliderFire.setSize(Vector2f(colliderFireX, colliderFireY));
    this->colliderFire.setPosition(colliderFirePosX, colliderFirePoxY);
    window.draw(this->colliderFire);

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