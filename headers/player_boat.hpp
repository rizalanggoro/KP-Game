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
  float boatTargetSize = 192;
  float boatScaleFactor = 1;

 public:
  Sprite *getSprite() { return &this->player; }

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
    this->player.setScale(this->boatScaleFactor, this->boatScaleFactor);

    window.draw(this->player);

    // todo: draw rect
    RectangleShape rect{};
    rect.setFillColor(Color::Transparent);
    rect.setPosition(this->player.getPosition());
    rect.setOutlineThickness(-2);
    rect.setOutlineColor(Color::Red);
    rect.setSize(Vector2f(this->boatTargetSize, this->boatTargetSize));

    window.draw(rect);
  }
};

#endif