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
  // Enemy enemy{&player, Vector2f(27 * 16, 15 * 16)};

  Vector2i playerRealPos;

  void handleKeyboard() {
    // todo: handle two direction move
    if (Keyboard::isKeyPressed(Keyboard::Down) &&
        Keyboard::isKeyPressed(Keyboard::Left)) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, 0);
      auto canMoveLeft = this->tilemap.canMove(this->player, 2);

      if (canMoveDown && canMoveLeft)
        this->view.move(-speed, speed);
      else if (canMoveDown)
        this->view.move(0, speed);
      else if (canMoveLeft)
        this->view.move(-speed, 0);

    } else if (Keyboard::isKeyPressed(Keyboard::Down) &&
               Keyboard::isKeyPressed(Keyboard::Right)) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, 0);
      auto canMoveRight = this->tilemap.canMove(this->player, 3);

      if (canMoveDown && canMoveRight)
        this->view.move(speed, speed);
      else if (canMoveDown)
        this->view.move(0, speed);
      else if (canMoveRight)
        this->view.move(speed, 0);

    } else if (Keyboard::isKeyPressed(Keyboard::Up) &&
               Keyboard::isKeyPressed(Keyboard::Left)) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, 1);
      auto canMoveLeft = this->tilemap.canMove(this->player, 2);

      if (canMoveUp && canMoveLeft)
        this->view.move(-speed, -speed);
      else if (canMoveUp)
        this->view.move(0, -speed);
      else if (canMoveLeft)
        this->view.move(-speed, 0);

    } else if (Keyboard::isKeyPressed(Keyboard::Up) &&
               Keyboard::isKeyPressed(Keyboard::Right)) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, 1);
      auto canMoveRight = this->tilemap.canMove(this->player, 3);

      if (canMoveUp && canMoveRight)
        this->view.move(speed, -speed);
      else if (canMoveUp)
        this->view.move(0, -speed);
      else if (canMoveRight)
        this->view.move(speed, 0);

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
    // view.zoom(.3);

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
    auto winCenter = window.getView().getCenter();
    auto playerGlobalBounds = this->player.getSpritePlayer()->getGlobalBounds();
    this->player.getSpritePlayer()->setPosition(
        winCenter.x - (playerGlobalBounds.width / 2),
        winCenter.y - (playerGlobalBounds.height / 2));
    this->player.draw(window);

    // todo: draw enemy
    // this->enemy.draw(window);

    // todo: draw tree
  }
};

#endif