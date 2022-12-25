#ifndef enemy_boat_hpp
#define enemy_boat_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "player_boat.hpp"

using namespace std;
using namespace sf;

class EnemyBoat {
 private:
  Asset *asset;
  PlayerBoat *playerBoat;

  float boatSize = 128;
  float boatTargetSize = 80;
  // float boatTargetSize = 256;
  float boatScaleFactor = 1;

  Sprite enemy{};
  float enemyVelocity = 0;

 public:
  EnemyBoat(Asset *asset, PlayerBoat *playerBoat) {
    this->asset = asset;
    this->playerBoat = playerBoat;

    this->enemyVelocity = this->playerBoat->getVelocity() * .6;
    this->boatScaleFactor = this->boatTargetSize / this->boatSize;
  }

  void draw(RenderWindow &window) {
    this->enemy.setTexture(this->asset->getVectorBoatColor1()->at(0));
    this->enemy.setScale(this->boatScaleFactor, this->boatScaleFactor);
    window.draw(this->enemy);

    auto playerPos = this->playerBoat->getSprite()->getPosition();
    auto enemyPos = this->enemy.getPosition();

    auto dx = playerPos.x - enemyPos.x;
    auto dy = playerPos.y - enemyPos.y;

    if (enemyPos.x < playerPos.x) {
      if (dx < this->enemyVelocity)
        this->enemy.setPosition(playerPos.x, enemyPos.y);
      else
        this->enemy.move(this->enemyVelocity, 0);
    } else if (enemyPos.x > playerPos.x) {
      if (dx > this->enemyVelocity)
        this->enemy.setPosition(playerPos.x, enemyPos.y);
      else
        this->enemy.move(-this->enemyVelocity, 0);
    }

    if (enemyPos.y < playerPos.y) {
      if (dy < this->enemyVelocity)
        this->enemy.setPosition(enemyPos.x, playerPos.y);
      else
        this->enemy.move(0, this->enemyVelocity);
    } else if (enemyPos.y > playerPos.y) {
      if (dy > this->enemyVelocity)
        this->enemy.setPosition(enemyPos.x, playerPos.y);
      else
        this->enemy.move(0, -this->enemyVelocity);
    }
  }
};

#endif