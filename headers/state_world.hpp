#ifndef state_world_hpp
#define state_world_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "player.hpp"
#include "tilemap_world.hpp"

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

  Vector2i playerRealPos;

  void handleKeyboard() {
    bool keyUp = Keyboard::isKeyPressed(Keyboard::Up);
    bool keyDown = Keyboard::isKeyPressed(Keyboard::Down);
    bool keyRight = Keyboard::isKeyPressed(Keyboard::Right);
    bool keyLeft = Keyboard::isKeyPressed(Keyboard::Left);

    // todo: handle two direction move
    if (keyDown && keyLeft) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, "d");
      auto canMoveLeft = this->tilemap.canMove(this->player, "l");

      if (canMoveDown && canMoveLeft)
        this->view.move(-speed, speed);
      else if (canMoveDown)
        this->view.move(0, speed);
      else if (canMoveLeft)
        this->view.move(-speed, 0);

    } else if (keyDown && keyRight) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, "d");
      auto canMoveRight = this->tilemap.canMove(this->player, "r");

      if (canMoveDown && canMoveRight)
        this->view.move(speed, speed);
      else if (canMoveDown)
        this->view.move(0, speed);
      else if (canMoveRight)
        this->view.move(speed, 0);

    } else if (keyUp && keyLeft) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, "u");
      auto canMoveLeft = this->tilemap.canMove(this->player, "l");

      if (canMoveUp && canMoveLeft)
        this->view.move(-speed, -speed);
      else if (canMoveUp)
        this->view.move(0, -speed);
      else if (canMoveLeft)
        this->view.move(-speed, 0);

    } else if (keyUp && keyRight) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, "u");
      auto canMoveRight = this->tilemap.canMove(this->player, "r");

      if (canMoveUp && canMoveRight)
        this->view.move(speed, -speed);
      else if (canMoveUp)
        this->view.move(0, -speed);
      else if (canMoveRight)
        this->view.move(speed, 0);

    }

    // todo: handle one direction move
    else if (keyDown) {
      if (this->tilemap.canMove(player, "d"))
        this->view.move(0, this->player.getVelocity());
    } else if (keyUp) {
      if (this->tilemap.canMove(player, "u"))
        this->view.move(0, -this->player.getVelocity());
    } else if (keyLeft) {
      if (this->tilemap.canMove(player, "l"))
        this->view.move(-this->player.getVelocity(), 0);
    } else if (keyRight) {
      if (this->tilemap.canMove(player, "r"))
        this->view.move(this->player.getVelocity(), 0);
    }
  }

 public:
  StateWorld(string *state, RenderWindow *window) {
    this->state = state;
    this->window = window;

    view = window->getDefaultView();
    view.setCenter(0, 0);

    auto tileTargetSize = this->tilemap.getTileTargetSize();
    auto width = this->tilemap.getWidth();
    auto height = this->tilemap.getHeight();

    this->player.setPosition(6 * this->tilemap.getTileTargetSize(),
                             16 * this->tilemap.getTileTargetSize());
    this->view.move(
        this->tilemap.getWidth() / 2 * this->tilemap.getTileTargetSize(),
        this->tilemap.getHeight() / 1.5 * this->tilemap.getTileTargetSize());
  }

  void handleEvent(Event &event) {
    this->player.handleEvent(event);
    if (event.type == Event::Resized) {
      auto size = event.size;
      this->view.setSize(size.width, size.height);
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
    // auto winCenter = window.getView().getCenter();
    // auto playerGlobalBounds =
    // this->player.getSpritePlayer()->getGlobalBounds();
    // this->player.getSpritePlayer()->setPosition(
    //     winCenter.x - (playerGlobalBounds.width / 2),
    //     winCenter.y - (playerGlobalBounds.height / 2));
    this->player.draw(window);

    // todo: draw tree
  }
};

#endif