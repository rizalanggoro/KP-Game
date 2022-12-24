#ifndef state_world_hpp
#define state_world_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "player.hpp"
#include "tilemap.hpp"

using namespace std;
using namespace sf;

class StateWorld {
 private:
  string *state;
  View view;
  RenderWindow *window;

  Asset asset{};
  Player player{&asset};
  Tilemap tilemap{&asset};
  Enemy enemy{&player, Vector2f(27 * 16, 15 * 16)};

  Vector2i playerRealPos;

  // RectangleShape rectColliderUp{}, rectColliderDown{}, rectColliderRight{},
  //     rectColliderLeft{};

  void handleKeyboard() {
    // todo: handle two direction move
    if (Keyboard::isKeyPressed(Keyboard::Down) &&
        Keyboard::isKeyPressed(Keyboard::Left)) {
      if (this->tilemap.canMove(this->player, 0) &&
          this->tilemap.canMove(this->player, 2)) {
        auto speed = sqrt(2) * this->player.getVelocity() / 2;
        this->view.move(-speed, speed);
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Down) &&
               Keyboard::isKeyPressed(Keyboard::Right)) {
      if (this->tilemap.canMove(this->player, 0) &&
          this->tilemap.canMove(this->player, 3)) {
        auto speed = sqrt(2) * this->player.getVelocity() / 2;
        this->view.move(speed, speed);
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Up) &&
               Keyboard::isKeyPressed(Keyboard::Left)) {
      if (this->tilemap.canMove(this->player, 1) &&
          this->tilemap.canMove(this->player, 2)) {
        auto speed = sqrt(2) * this->player.getVelocity() / 2;
        this->view.move(-speed, -speed);
      }
    } else if (Keyboard::isKeyPressed(Keyboard::Up) &&
               Keyboard::isKeyPressed(Keyboard::Right)) {
      if (this->tilemap.canMove(this->player, 1) &&
          this->tilemap.canMove(this->player, 3)) {
        auto speed = sqrt(2) * this->player.getVelocity() / 2;
        this->view.move(speed, -speed);
      }
    }

    // todo: handle one direction move
    else if (Keyboard::isKeyPressed(Keyboard::Down)) {
      if (this->tilemap.canMove(player, 0))
        this->view.move(0, this->player.getVelocity());
    } else if (Keyboard::isKeyPressed(Keyboard::Up)) {
      if (this->tilemap.canMove(player, 1))
        this->view.move(0, -this->player.getVelocity());
    } else if (Keyboard::isKeyPressed(Keyboard::Left)) {
      if (this->tilemap.canMove(player, 2))
        this->view.move(-this->player.getVelocity(), 0);
    } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
      if (this->tilemap.canMove(player, 3))
        this->view.move(this->player.getVelocity(), 0);
    }
  }

 public:
  StateWorld(string *state, RenderWindow *window) {
    this->state = state;
    this->window = window;

    view = window->getDefaultView();
    view.setCenter(0, 0);
    view.zoom(.3);

    // this->player.getSpritePlayer()->setPosition((30 * 16) / 2, (20 * 16) /
    // 2);

    view.move((5 * 16), (15 * 16));

    // this->player.getSpritePlayer()->setPosition(0, 0);
  }

  void handleEvent(Event &event) {
    this->player.handleEvent(event);
    if (event.type == Event::KeyPressed) {
      auto code = event.key.code;
      if (code == Keyboard::Equal) {
        this->view.zoom(.9);
      }

      if (code == Keyboard::Dash) {
        this->view.zoom(1.1);
      }
    }
  }

  void run(RenderWindow &window) {
    window.setView(view);

    this->handleKeyboard();
    this->playerRealPos =
        window.mapCoordsToPixel(this->player.getSpritePlayer()->getPosition());

    RectangleShape rect{};
    rect.setOutlineColor(Color::Red);
    rect.setOutlineThickness(2);

    auto wSize = window.getSize();
    rect.setSize(Vector2f(tilemap.getWidth() * 16, tilemap.getHeight() * 16));

    window.draw(rect);

    // todo: draw ground
    tilemap.draw(window);

    // todo: draw sprite player
    // window.setView(window.getDefaultView());
    auto winCenter = window.getView().getCenter();
    auto playerGlobalBounds = this->player.getSpritePlayer()->getGlobalBounds();
    this->player.getSpritePlayer()->setPosition(
        winCenter.x - (playerGlobalBounds.width / 2),
        winCenter.y - (playerGlobalBounds.height / 2));
    this->player.draw(window);

    // todo: draw enemy
    this->enemy.draw(window);

    // todo: draw tree

    // // todo: draw tilemap collider
    // View viewCollider = window.getDefaultView();
    // window.setView(viewCollider);

    // auto colliderWSize = window.getSize();

    // rectColliderUp.setSize(Vector2f(colliderWSize.x, 32));
    // rectColliderDown.setSize(Vector2f(colliderWSize.x, 32));
    // rectColliderRight.setSize(Vector2f(32, colliderWSize.x));
    // rectColliderLeft.setSize(Vector2f(32, colliderWSize.x));

    // rectColliderUp.setFillColor(Color(255, 0, 0, 50));
    // rectColliderDown.setFillColor(Color(255, 0, 0, 50));
    // rectColliderRight.setFillColor(Color(255, 0, 0, 50));
    // rectColliderLeft.setFillColor(Color(255, 0, 0, 50));

    // rectColliderDown.setPosition(0, colliderWSize.y - 32);
    // rectColliderRight.setPosition(colliderWSize.x - 32, 0);

    // window.draw(rectColliderUp);
    // window.draw(rectColliderDown);
    // window.draw(rectColliderRight);
    // window.draw(rectColliderLeft);
  }
};

#endif