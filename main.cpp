#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/asset.hpp"
#include "headers/state_test.hpp"
#include "headers/state_war.hpp"
#include "headers/state_world.hpp"

// #define DEBUG

using namespace sf;
using namespace std;

int main() {
  RenderWindow window(VideoMode(1280, 720), "SFML | Game Fix InsyaAllah",
                      Style::Default);
  window.setFramerateLimit(60);

  string state = "world";

  // todo: states
  // StateTest stateTest{&state, &window};
  StateWorld stateWorld{&state, &window};
  StateWar stateWar{&state, &window};

  // float elapsedTime = 0;
  // Clock clock;

  while (window.isOpen()) {
    // elapsedTime = clock.restart().asSeconds() * 60;
    // cout << "elapsed time: " << elapsedTime << endl;

    Event event{};
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) window.close();
      // if (event.type == Event::KeyPressed)
      //   if (event.key.code == Keyboard::Escape) window.close();

      // if (state == "test") stateTest.handleEvent(event);
      if (state == "world") stateWorld.handleEvent(event);
      if (state == "war") stateWar.handleEvent(event);
    }

    window.clear(Color::White);

    // if (state == "test") stateTest.run(window);
    if (state == "world") stateWorld.run(window);
    if (state == "war") stateWar.run(window);

    window.display();
  }

  return 0;
}
