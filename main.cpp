#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/state_test.hpp"
#include "headers/state_world.hpp"

using namespace sf;
using namespace std;

int main() {
  RenderWindow window(VideoMode(1280, 720), "SFML | Starter V2");
  window.setFramerateLimit(60);

  string state = "world";
  // auto wCenter = window.getView().getCenter();
  // cout << wCenter.x << "," << wCenter.y << endl;

  // View view = window.getDefaultView();
  // view.setSize(30 * 16, 20 * 16);

  // view.setCenter(0, 0);
  // auto size = view.getSize();
  // cout << size.x << endl;
  // cout << size.y << endl;

  // view.zoom(.3);
  // view.move((30 * 16) / 2, (20 * 16) / 2);
  // window.setView(view);

  // auto vCenter = view.getCenter();
  // cout << vCenter.x << "," << vCenter.y << endl;

  // auto vGlobal = view.getSize();
  // cout << vGlobal.x << "," << vGlobal.y << endl;

  // view.setSize(1280, 720);
  // view.setCenter(0, 0);
  // view.zoom(.3);

  // todo: states
  StateTest stateTest{&state, &window};
  StateWorld stateWorld{&state, &window};

  while (window.isOpen()) {
    Event event{};
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) window.close();

      if (state == "test") stateTest.handleEvent(event);
      if (state == "world") stateWorld.handleEvent(event);
    }

    window.clear(Color::White);

    if (state == "test") stateTest.run(window);
    if (state == "world") stateWorld.run(window);

    window.display();
  }

  return 0;
}
