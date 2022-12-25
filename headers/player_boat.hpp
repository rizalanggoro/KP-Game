#ifndef player_boat_hpp
#define player_boat_hpp

#include <SFML/Graphics.hpp>
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
  float boatTargetSize = 160;
  float boatScaleFactor = 1;

  float velocity = 3;

  RectangleShape colliderUp{}, colliderDown{}, colliderRight{}, colliderLeft{};

 public:
  float getVelocity() { return this->velocity; }
  float getBoatTargetSize() { return this->boatTargetSize; }
  Sprite *getSprite() { return &this->player; }
  RectangleShape *getColliderUp() { return &this->colliderUp; }
  RectangleShape *getColliderDown() { return &this->colliderDown; }
  RectangleShape *getColliderRight() { return &this->colliderRight; }
  RectangleShape *getColliderLeft() { return &this->colliderLeft; }

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
    this->player.setTexture(this->asset->getVectorBoatColor1()->at(1));
    this->player.setScale(this->boatScaleFactor, this->boatScaleFactor);

    window.draw(this->player);

    // todo: draw rect
    RectangleShape rect{};
    rect.setFillColor(Color::Transparent);
    rect.setPosition(this->player.getPosition());
    rect.setOutlineThickness(-2);
    rect.setOutlineColor(Color(255, 0, 0, 50));
    rect.setSize(Vector2f(this->boatTargetSize, this->boatTargetSize));

    window.draw(rect);

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

    auto playerPos = this->player.getPosition();
    this->colliderUp.setPosition(
        Vector2f(playerPos.x, playerPos.y - this->velocity * 2));
    this->colliderDown.setPosition(
        Vector2f(playerPos.x, playerPos.y + this->boatTargetSize));
    this->colliderRight.setPosition(
        Vector2f(playerPos.x + this->boatTargetSize, playerPos.y));
    this->colliderLeft.setPosition(
        Vector2f(playerPos.x - this->velocity * 2, playerPos.y));

    window.draw(this->colliderUp);
    window.draw(this->colliderDown);
    window.draw(this->colliderRight);
    window.draw(this->colliderLeft);
  }
};

#endif