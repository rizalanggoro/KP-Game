#ifndef fire_hpp
#define fire_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"

using namespace std;
using namespace sf;

class Fire {
 private:
  Asset *asset;
  string fireDirection;

  Sprite fire{};
  CircleShape fireCollider{};

  float velocity = 0;

 public:
  Fire(Asset *asset, string fireDirection) {
    this->asset = asset;
    this->fireDirection = fireDirection;

    // todo: set fire rotation
    auto dir = this->fireDirection;
    int angle = 0;
    if (dir == "u")
      angle = 180;
    else if (dir == "d")
      angle = 0;
    else if (dir == "r")
      angle = 270;
    else if (dir == "l")
      angle = 90;
    else if (dir == "ur")
      angle = 180 + 45;
    else if (dir == "ul")
      angle = 180 - 45;
    else if (dir == "dr")
      angle = 270 + 45;
    else if (dir == "dl")
      angle = 45;

    this->fire.setRotation(angle);
  }

  Sprite *getSprite() { return &this->fire; }
  void setPosition(Vector2f position) { this->fire.setPosition(position); }
  // void move(float x, float y) { this->fire.move(x, y); }
  void setVelocity(float velocity) { this->velocity = velocity; }
  void draw(RenderWindow &window) {
    // todo: draw fire
    this->fire.setTexture(*this->asset->getTextureFire());
    this->fire.setOrigin(128 / 2, 128 / 2);
    window.draw(this->fire);

    // todo: draw fire collider
    this->fireCollider.setFillColor(Color::Red);
    this->fireCollider.setRadius(4);
    this->fireCollider.setPosition(this->fire.getPosition());
    window.draw(this->fireCollider);

    // todo: move fire
    auto dir = this->fireDirection;
    float speedX = 0, speedY = 0;
    if (dir == "u") {
      speedY = -this->velocity;
    } else if (dir == "d") {
      speedY = this->velocity;
    } else if (dir == "r") {
      speedX = this->velocity;
    } else if (dir == "l") {
      speedX = -this->velocity;
    } else if (dir == "ur") {
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = speed;
      speedY = -speed;
    } else if (dir == "ul") {
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = -speed;
      speedY = -speed;
    } else if (dir == "dr") {
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = speed;
      speedY = speed;
    } else if (dir == "dl") {
      auto speed = this->velocity * sqrt(2) / 2;
      speedX = -speed;
      speedY = speed;
    }
    this->fire.move(speedX, speedY);
  }

  CircleShape *getFireCollider() { return &this->fireCollider; }
};

#endif