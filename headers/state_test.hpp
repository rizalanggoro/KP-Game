#ifndef state_test_hpp
#define state_test_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "asset.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;

class StateTest {
 private:
  string *state;
  View view{};

  Asset *asset;
  Player player{asset};
  RectangleShape test{};

  void handleKeyboard() {
    // if (Keyboard::isKeyPressed(Keyboard::Up)) this->view->move(0, -5);
    // if (Keyboard::isKeyPressed(Keyboard::Down)) this->view->move(0, 5);
    // if (Keyboard::isKeyPressed(Keyboard::Right)) this->view->move(5, 0);
    // if (Keyboard::isKeyPressed(Keyboard::Left)) this->view->move(-5, 0);
  }

 public:
  StateTest(string *state, RenderWindow *window, Asset *asset) {
    this->state = state;
    this->asset = asset;
    // this->view = view;

    view = window->getDefaultView();
    view.setCenter(0, 0);
    view.zoom(.1);

    // this->view->setCenter(1280 / 2, 720 / 2);
    // // this->view->zoom(.1);
    // window->setView(*this->view);
  }

  void handleEvent(Event &event) { this->player.handleEvent(event); }

  void run(RenderWindow &window) {
    window.setView(view);
    // this->handleKeyboard();

    RectangleShape rect{};
    rect.setFillColor(Color::Transparent);
    rect.setOutlineThickness(-1);
    rect.setOutlineColor(Color::Red);
    rect.setSize(Vector2f(1280, 720));
    window.draw(rect);

    this->player.draw(window);

    // test.setFillColor(Color::Black);
    // test.setSize(Vector2f(100, 100));
    // // test.setPosition(100, 100);
    // window.draw(test);

    // if (Keyboard::isKeyPressed(Keyboard::Up)) {
    //   test.move(0, -5);
    // }
    // if (Keyboard::isKeyPressed(Keyboard::Down)) {
    //   test.move(0, 5);
    // }
    // if (Keyboard::isKeyPressed(Keyboard::Right)) this->view->move(5, 0);
    // if (Keyboard::isKeyPressed(Keyboard::Left)) this->view->move(-5, 0);
  }
};

#endif
