#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/data.hpp"
#include "headers/state_war.hpp"
#include "headers/state_world.hpp"

using namespace sf;
using namespace std;

int main() {
  RenderWindow window(VideoMode(1280, 720), "VILLAGE'S WARSHIPS", Style::Close);

  string state = "world";
  Data data{};
  data.load();

  // todo: states
  StateWorld stateWorld{&state, &window, &data};
  StateWar stateWar{&state, &window, &data};

  while (window.isOpen()) {
    Event event{};
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) window.close();

      if (state == "world") stateWorld.handleEvent(event);
      if (state == "war") stateWar.handleEvent(event);
    }

    window.clear(Color::White);

    if (state == "world") stateWorld.run(window);
    if (state == "war") stateWar.run(window);

    window.display();
  }

  return 0;
}
