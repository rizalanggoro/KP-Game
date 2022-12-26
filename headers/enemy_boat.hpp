#ifndef enemy_boat_hpp
#define enemy_boat_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "player_boat.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class EnemyBoat {
 private:
  Asset *asset;
  TilemapWar *tilemap;
  PlayerBoat *playerBoat;

  float boatSize = 128;
  float boatTargetSize = 80;
  // float boatTargetSize = 256;
  float boatScaleFactor = 1;

  int hp = 100;

  Sprite enemy{};
  float enemyVelocity = 0;

  RectangleShape colliderBox{};
  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};

 public:
  EnemyBoat(Asset *asset, TilemapWar *tilemap, PlayerBoat *playerBoat) {
    this->asset = asset;
    this->tilemap = tilemap;
    this->playerBoat = playerBoat;

    this->enemyVelocity = this->playerBoat->getVelocity() * .6;
    this->boatScaleFactor = this->boatTargetSize / this->boatSize;
  }

  void setInitPos(int tileC, int tileR) {
    this->enemy.setPosition(
        tileC * this->tilemap->getTileTargetSize() - this->boatTargetSize / 2,
        tileR * this->tilemap->getTileTargetSize() - this->boatTargetSize / 2);
  }

  void draw(RenderWindow &window) {
    this->enemy.setTexture(this->asset->getVectorBoatColor3()->at(0));
    this->enemy.setOrigin(this->boatSize / 2, this->boatSize / 2);
    this->enemy.setScale(this->boatScaleFactor, this->boatScaleFactor);
    window.draw(this->enemy);

    auto playerPos = this->playerBoat->getSprite()->getPosition();
    auto enemyPos = this->enemy.getPosition();

    // todo: draw collider box
    this->colliderBox.setFillColor(Color::Transparent);
    this->colliderBox.setOutlineColor(Color::Red);
    this->colliderBox.setOutlineThickness(-2);
    this->colliderBox.setPosition(enemyPos.x - this->boatTargetSize / 2,
                                  enemyPos.y - this->boatTargetSize / 2);
    this->colliderBox.setSize(
        Vector2f(this->boatTargetSize, this->boatTargetSize));
    window.draw(this->colliderBox);

    auto colliderBoxPos = this->colliderBox.getPosition();

    // todo: draw hp bar
    RectangleShape rectHp{};
    rectHp.setFillColor(Color::Red);
    rectHp.setOutlineColor(Color::Black);
    rectHp.setOutlineThickness(2);
    rectHp.setSize(Vector2f(this->boatTargetSize, 4));
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

    auto colliderColor = Color(0, 0, 255, 50);
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

    if (enemyPos.x < playerPos.x &&
        this->tilemap->canMove(&this->colliderRight)) {
      // todo: move right
      this->enemy.setRotation(270);
      if (dx < this->enemyVelocity)
        this->enemy.setPosition(playerPos.x, enemyPos.y);
      else
        this->enemy.move(this->enemyVelocity, 0);

    } else if (enemyPos.x > playerPos.x &&
               this->tilemap->canMove(&this->colliderLeft)) {
      // todo: move left
      this->enemy.setRotation(90);
      if (dx > this->enemyVelocity)
        this->enemy.setPosition(playerPos.x, enemyPos.y);
      else
        this->enemy.move(-this->enemyVelocity, 0);

    }

    else if (enemyPos.y < playerPos.y &&
             this->tilemap->canMove(&this->colliderDown)) {
      // todo: move down
      this->enemy.setRotation(0);
      if (dy < this->enemyVelocity)
        this->enemy.setPosition(enemyPos.x, playerPos.y);
      else
        this->enemy.move(0, this->enemyVelocity);
    } else if (enemyPos.y > playerPos.y &&
               this->tilemap->canMove(&this->colliderUp)) {
      // todo: move up
      this->enemy.setRotation(180);
      if (dy > this->enemyVelocity)
        this->enemy.setPosition(enemyPos.x, playerPos.y);
      else
        this->enemy.move(0, -this->enemyVelocity);
    }
  }
};

#endif