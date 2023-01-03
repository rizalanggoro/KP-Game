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
  // asset digunakan untuk memberikan texture sprite fire
  Asset *asset;
  // direction digunakan sebagai penentu arah gerak peluru
  Direction fireDirection;

  // objek yang akan digambar pada window dan diberikan texture
  Sprite fire{};
  // sebagai pendeteksi tabrakan antara pembatas map dan tabrakan antar kapal
  CircleShape fireCollider{};

  // kecepatan peluru
  float velocity = 0;

  // target size digunakan untuk mengatur ukuran peluru
  // ukuran gambar texture -> 128
  float targetFireSize = 32;
  float fireSize = 0;
  float fireScaleFactor = 1;

  // berguna agar peluru dapat bergerak dengan kecepatan yang sesuai
  // meskipun fps tidak stabil
  Clock clockMultiplier{};
  float multiplier = 1;

  void handleMovement() {
    // fungsi untuk mengatur arah dan jalannya peluru
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

    // perintah untuk memindahkan / menggerakkan peluru sesuai dengan kecepatan
    // yang telah ditentukan di atas
    this->fire.move(speedX * this->multiplier, speedY * this->multiplier);
  }

 public:
  //  todo: getters
  // untuk menggembalikan properti yang ada di kelas ini
  // yang nantinya akan digunakan pada kelas lain
  CircleShape *getFireCollider() { return &this->fireCollider; }
  Sprite *getSprite() { return &this->fire; }

  // todo: setters
  // untuk mengatur properti yang ada di kelas ini
  void setPosition(Vector2f position) { this->fire.setPosition(position); }
  void setVelocity(float velocity) { this->velocity = velocity; }

  Fire(Asset *asset, Direction fireDirection) {
    this->asset = asset;
    this->fireDirection = fireDirection;

    // todo: set fire rotation
    // untuk mengatur sudut peluru sesuai dengan arah kapal
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

    // menyetel rotasi
    this->fire.setRotation(angle);

    // menyetel texture dari object dan mendapatkan scala yang tepat
    this->fire.setTexture(*this->asset->getTextureFire());
    this->fireSize = this->fire.getGlobalBounds().width;
    this->fireScaleFactor = this->targetFireSize / this->fireSize;
  }

  void draw(RenderWindow &window) {
    // todo: handle multiplier
    this->multiplier = this->clockMultiplier.restart().asSeconds() * 60;

    // todo: draw fire
    // untuk mengatur titik awal / titik rotasi supaya berada di tengah
    this->fire.setOrigin(128 / 2, 128 / 2);
    // untuk mengatur skala
    this->fire.setScale(this->fireScaleFactor, this->fireScaleFactor);
    // untuk menggambar di layar
    window.draw(this->fire);

    // todo: draw fire collider
    // sebagai pendeteksi tabrakan -> map, kapal
    this->fireCollider.setFillColor(Color::Transparent);
    this->fireCollider.setRadius(1);
    this->fireCollider.setPosition(this->fire.getPosition());
    window.draw(this->fireCollider);

    // todo: move fire
    this->handleMovement();
  }
};

#endif