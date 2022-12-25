#ifndef state_war_hpp
#define state_war_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "tilemap_war.hpp"

using namespace std;
using namespace sf;

class StateWar {
 private:
  string *state;
  RenderWindow *window;

  Asset asset{};
  View view{};
  // Sprite spriteWater{};
  // int currWaterFrameIndex = 0;
  // Clock clockWater;
  // float frameWaterDelay = 250;
  // float frameWaterInterval = 0;

  TilemapWar tilemapWar{&asset};

  void handleKeyboard() {
    if (Keyboard::isKeyPressed(Keyboard::Up)) this->view.move(0, -5);
    if (Keyboard::isKeyPressed(Keyboard::Down)) this->view.move(0, 5);
    if (Keyboard::isKeyPressed(Keyboard::Right)) this->view.move(5, 0);
    if (Keyboard::isKeyPressed(Keyboard::Left)) this->view.move(-5, 0);
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

    // view = window->getDefaultView();
  }

  void handleEvent(Event &event) {}

  void run(RenderWindow &window) {
    this->window->setView(this->view);

    this->handleKeyboard();

    // window.setView(window.getDefaultView());

    // CircleShape circle{};
    // circle.setRadius(50);
    // circle.setFillColor(Color::Red);
    // window.draw(circle);

    this->tilemapWar.draw(window);

    // todo: handle water frame animation
    // this->frameWaterInterval =
    //     this->clockWater.getElapsedTime().asMilliseconds();

    // if (this->frameWaterInterval >= this->frameWaterDelay) {
    //   if (this->currWaterFrameIndex < 3)
    //     this->currWaterFrameIndex++;
    //   else
    //     this->currWaterFrameIndex = 0;

    //   this->clockWater.restart();
    // }

    // auto wSize = window.getSize();

    // float tileSize = 64;
    // auto scaleFactor = tileSize / 16;

    // // todo: draw water
    // for (int r = 0; r < wSize.y / tileSize; r++) {
    //   for (int c = 0; c < wSize.x / tileSize; c++) {
    //     this->spriteWater.setTexture(
    //         this->asset->getVectorWater()->at(this->currWaterFrameIndex));
    //     this->spriteWater.setScale(scaleFactor, scaleFactor);
    //     this->spriteWater.setPosition(16 * c * scaleFactor,
    //                                   16 * r * scaleFactor);

    //     window.draw(this->spriteWater);
    //   }
    // }
  }
};

#endif