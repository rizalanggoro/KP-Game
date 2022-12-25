#ifndef state_war_hpp
#define state_war_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "player_boat.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class StateWar {
 private:
  string *state;
  RenderWindow *window;

  Asset asset{};
  View view{};

  TilemapWar tilemapWar{&asset};
  PlayerBoat playerBoat{&asset, &tilemapWar};

  float colliderMapSize = 32;
  Vector2i playerRealPos;

  // RectangleShape colliderMapUp{}, colliderMapDown{}, colliderMapRight{},
  //     colliderMapLeft{};

  void handleKeyboard() {
    if (Keyboard::isKeyPressed(Keyboard::Up))
      this->playerBoat.getSprite()->move(0, -5);
    if (Keyboard::isKeyPressed(Keyboard::Down))
      this->playerBoat.getSprite()->move(0, 5);
    if (Keyboard::isKeyPressed(Keyboard::Right))
      this->playerBoat.getSprite()->move(5, 0);
    if (Keyboard::isKeyPressed(Keyboard::Left))
      this->playerBoat.getSprite()->move(-5, 0);
  }

 public:
  StateWar(string *state, RenderWindow *window) {
    this->state = state;
    this->window = window;

    this->view = this->window->getDefaultView();
    this->view.setCenter(0, 0);
    auto tileTargetSize = this->tilemapWar.getTileTargetSize();
    this->view.move(this->tilemapWar.getWidth() * tileTargetSize / 2,
                    this->tilemapWar.getHeight() * tileTargetSize / 2);

    this->window->setView(this->view);
  }

  void handleEvent(Event &event) {}

  void run(RenderWindow &window) {
    this->playerRealPos =
        window.mapCoordsToPixel(this->playerBoat.getSprite()->getPosition());
    cout << "pos: " << playerRealPos.x << "," << playerRealPos.y << endl;

    this->window->setView(this->view);

    this->handleKeyboard();

    this->tilemapWar.draw(window);
    this->playerBoat.draw(window);

    // todo: draw collider map
    // auto windowSize = this->window->getSize();
    // this->colliderMapUp.setSize(Vector2f(windowSize.x,
    // this->colliderMapSize)); this->colliderMapDown.setSize(
    //     Vector2f(windowSize.x, this->colliderMapSize));
    // this->colliderMapRight.setSize(
    //     Vector2f(this->colliderMapSize, windowSize.y));
    // this->colliderMapLeft.setSize(
    //     Vector2f(this->colliderMapSize, windowSize.y));

    // this->colliderMapUp.setFillColor(Color(0, 255, 0, 50));
    // this->colliderMapDown.setFillColor(Color(0, 255, 0, 50));
    // this->colliderMapRight.setFillColor(Color(0, 255, 0, 50));
    // this->colliderMapLeft.setFillColor(Color(0, 255, 0, 50));

    // this->colliderMapRight.setPosition(windowSize.x - this->colliderMapSize,
    // 0); this->colliderMapDown.setPosition(0, windowSize.y -
    // this->colliderMapSize);

    // window.setView(window.getDefaultView());
    // window.draw(this->colliderMapUp);
    // window.draw(this->colliderMapDown);
    // window.draw(this->colliderMapRight);
    // window.draw(this->colliderMapLeft);

    // if (this->playerBoat.getSprite()->getGlobalBounds().intersects(
    //         this->colliderMapUp.getGlobalBounds()))
    //   cout << "oke" << endl;
  }
};

#endif