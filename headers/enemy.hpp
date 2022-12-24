#ifndef enemy_hpp
#define enemy_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <istream>

#include "player.hpp"

using namespace std;
using namespace sf;

class Enemy {
 private:
  Player *player;

  RectangleShape rect{};

 public:
  Enemy(Player *player) { this->player = player; }
  Enemy(Player *player, Vector2f pos) {
    this->player = player;
    this->rect.setPosition(pos);
  }

  void draw(RenderWindow &window) {
    auto playerSprite = this->player->getSpritePlayer();
    auto playerColliderUp = this->player->getColliderUp();
    auto playerColliderDown = this->player->getColliderDown();
    auto playerColliderRight = this->player->getColliderRight();
    auto playerColliderLeft = this->player->getColliderLeft();

    auto playerColliderUpGB = this->player->getColliderUp()->getGlobalBounds();
    auto playerColliderDownGB =
        this->player->getColliderDown()->getGlobalBounds();
    auto playerColliderRightGB =
        this->player->getColliderRight()->getGlobalBounds();
    auto playerColliderLeftGB =
        this->player->getColliderLeft()->getGlobalBounds();

    auto playerPos = this->player->getColliderUp()->getPosition();
    // cout << "player pos: " << playerPos.x << "," << playerPos.y << endl;

    rect.setFillColor(Color::Red);
    rect.setSize(Vector2f(16, 16));

    window.draw(rect);

    // todo: move enemy to player
    auto enemyPos = this->rect.getPosition();
    // cout << "enemy pos: " << enemyPos.x << "," << enemyPos.y << endl;
    auto enemyGB = this->rect.getGlobalBounds();

    bool intersect = enemyGB.intersects(playerColliderUpGB) ||
                     enemyGB.intersects(playerColliderDownGB) ||
                     enemyGB.intersects(playerColliderRightGB) ||
                     enemyGB.intersects(playerColliderLeftGB);
    if (!intersect) {
      auto dx = playerPos.x - enemyPos.x;
      auto dy = playerPos.y - enemyPos.y;

      // cout << "dx: " << dx << endl;
      // cout << "dy: " << dy << endl;

      if (dx > 0) {
        // todo: move right
        if (dx < this->player->getVelocity())
          this->rect.setPosition(playerPos.x, enemyPos.y);
        else
          this->rect.move(this->player->getVelocity(), 0);
      } else if (dx < 0) {
        // todo: move left
        if (dx > -this->player->getVelocity())
          this->rect.setPosition(playerPos.x, enemyPos.y);
        else
          this->rect.move(-this->player->getVelocity(), 0);
      } else if (dy > 0) {
        // todo: move down
        this->rect.move(0, this->player->getVelocity());
      } else if (dy < 0) {
        // todo: move up
        this->rect.move(0, -this->player->getVelocity());
      }

      // todo: move 2 directions
      // if (enemyPos.x != playerPos.x && enemyPos.y != playerPos.y) {
      //   auto deltaX = playerPos.x - enemyPos.x;
      //   auto deltaY = playerPos.y - enemyPos.y;
      //   // cout << delta << endl;

      //   if (fabs(deltaX) <= this->player->getVelocity()) {
      //     this->rect.setPosition(playerPos.x, enemyPos.y);
      //   } else if (fabs(deltaY) <= this->player->getVelocity()) {
      //     this->rect.setPosition(enemyPos.x, playerPos.y);
      //   } else {
      //     this->rect.move(
      //         this->player->getVelocity() * (deltaX > 0 ? 1 : -1) * sqrt(2) /
      //         2, this->player->getVelocity() * (deltaY > 0 ? 1 : -1) *
      //         sqrt(2) /
      //             2);
      //   }
      // }

      // todo: move horizontal

      // else if (enemyPos.x != playerPos.x) {
      //   auto delta = playerPos.x - enemyPos.x;
      //   // cout << delta << endl;
      //   if (fabs(delta) <= this->player->getVelocity()) {
      //     this->rect.setPosition(playerPos.x, enemyPos.y);
      //   } else {
      //     this->rect.move(this->player->getVelocity() * (delta > 0 ? 1 : -1),
      //                     0);
      //   }
      // }

      // // todo: move vertical
      // else if (enemyPos.y != playerPos.y) {
      //   auto delta = playerPos.y - enemyPos.y;
      //   // cout << delta << endl;
      //   if (fabs(delta) <= this->player->getVelocity()) {
      //     this->rect.setPosition(enemyPos.x, playerPos.y);
      //   } else {
      //     this->rect.move(0,
      //                     this->player->getVelocity() * (delta > 0 ? 1 :
      //                     -1));
      //   }
      // }
    }
  }
};

#endif