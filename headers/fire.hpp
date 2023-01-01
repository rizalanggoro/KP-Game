#ifndef fire_hpp
#define fire_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "direction.hpp"

using namespace std;
using namespace sf;

class Fire {
 private:
  Asset *asset;
  Direction fireDirection;

  Sprite fire{};
  CircleShape fireCollider{};

  float velocity = 0;

  float targetFireSize = 32;
  float fireSize = 0;
  float fireScaleFactor = 1;

  Clock clockMultiplier{};
  float multiplier = 1;

  void handleMovement() {
    auto dir = this->fireDirection;
    float speedX = 0, speedY = 0;
    if (dir == DIR_UP) {
      // todo: move up
      speedY = -this->velocity;
    } else if (dir == DIR_DOWN) {
      // todo: move down
      speedY = this->velocity;
    } else if (dir == DIR_RIGHT) {
      // todo: move right
      speedX = this->velocity;
    } else if (dir == DIR_LEFT) {
      // todo: move left
      speedX = -this->velocity;
    }

    else if (dir == DIR_UP_RIGHT) {
      // todo: move up right
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = speed;
      speedY = -speed;
    } else if (dir == DIR_UP_LEFT) {
      // todo: move up left
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = -speed;
      speedY = -speed;
    } else if (dir == DIR_DOWN_RIGHT) {
      // todo: move down right
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = speed;
      speedY = speed;
    } else if (dir == DIR_DOWN_LEFT) {
      // todo: move down left
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = -speed;
      speedY = speed;
    }
    this->fire.move(speedX * this->multiplier, speedY * this->multiplier);
  }

 public:
  //  todo: getters
  CircleShape *getFireCollider() { return &this->fireCollider; }
  Sprite *getSprite() { return &this->fire; }

  // todo: setters
  void setPosition(Vector2f position) { this->fire.setPosition(position); }
  void setVelocity(float velocity) { this->velocity = velocity; }

  Fire(Asset *asset, Direction fireDirection) {
    this->asset = asset;
    this->fireDirection = fireDirection;

    // todo: set fire rotation
    auto dir = this->fireDirection;
    int angle = 0;
    if (dir == DIR_UP)
      angle = 180;
    else if (dir == DIR_DOWN)
      angle = 0;
    else if (dir == DIR_RIGHT)
      angle = 270;
    else if (dir == DIR_LEFT)
      angle = 90;
    else if (dir == DIR_UP_RIGHT)
      angle = 180 + 45;
    else if (dir == DIR_UP_LEFT)
      angle = 180 - 45;
    else if (dir == DIR_DOWN_RIGHT)
      angle = 270 + 45;
    else if (dir == DIR_DOWN_LEFT)
      angle = 45;

    this->fire.setRotation(angle);

    this->fire.setTexture(*this->asset->getTextureFire());
    this->fireSize = this->fire.getGlobalBounds().width;
    this->fireScaleFactor = this->targetFireSize / this->fireSize;
  }

  void draw(RenderWindow &window) {
    // todo: handle multiplier
    this->multiplier = this->clockMultiplier.restart().asSeconds() * 60;

    // todo: draw fire
    this->fire.setOrigin(128 / 2, 128 / 2);
    this->fire.setScale(this->fireScaleFactor, this->fireScaleFactor);
    window.draw(this->fire);

    // todo: draw fire collider
    this->fireCollider.setFillColor(Color::Transparent);
    this->fireCollider.setRadius(1);
    this->fireCollider.setPosition(this->fire.getPosition());
    window.draw(this->fireCollider);

    // todo: move fire
    this->handleMovement();
  }
};

#endif