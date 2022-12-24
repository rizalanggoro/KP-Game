#ifndef tilemap_hpp
#define tilemap_hpp

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "asset.hpp"
#include "enemy.hpp"
#include "json.hpp"
#include "player.hpp"

using namespace std;
using namespace sf;
using json = nlohmann::json;

class Tilemap {
 private:
  Asset *asset;

  vector<int> vectorTilelayerGrass{};
  vector<int> vectorTilelayerDirt{};
  vector<int> vectorTilelayerBrigde{};
  vector<int> vectorTilelayerCollision{};

  vector<RectangleShape> vectorCollision{};

  int height;
  int width;

  Clock clockWater{};
  float waterDelay = 250;
  float waterInterval = 0;
  int currWaterIndex = 0;

  void loadMapJson() {
    ifstream file("assets/map/game.tmj");
    json data = json::parse(file);

    height = data["height"].get<int>();
    width = data["width"].get<int>();

    cout << "map height: " << height << endl;
    cout << "map width: " << width << endl;

    auto layers = data["layers"];
    for (auto it = layers.begin(); it != layers.end(); it++) {
      json layer = it.value();
      string layerType = layer["type"].get<string>();
      string layerName = layer["name"].get<string>();

      if (layerType == "tilelayer") {
        auto layerData = layer["data"].get<vector<int>>();
        if (layerName == "grass") this->vectorTilelayerGrass = layerData;
        if (layerName == "dirt") this->vectorTilelayerDirt = layerData;
        if (layerName == "bridge") this->vectorTilelayerBrigde = layerData;
        if (layerName == "collision")
          this->vectorTilelayerCollision = layerData;
      }
    }
  }

  void loadTilelayerCollision() {
    int index = 0;
    for (int h = 0; h < this->height; h++) {
      for (int w = 0; w < this->width; w++) {
        int tileType = this->vectorTilelayerCollision.at(index);
        if (tileType != 0) {
          RectangleShape rect{};
          rect.setSize(Vector2f(16, 16));
          rect.setFillColor(Color(255, 0, 0, 50));
          rect.setPosition(16 * w, 16 * h);
          this->vectorCollision.push_back(rect);
        }

        index++;
      }
    }
  }

 public:
  Tilemap(Asset *asset) {
    this->asset = asset;

    loadMapJson();
    this->loadTilelayerCollision();
  }

  void draw(RenderWindow &window) {
    // todo: water animation
    waterInterval = clockWater.getElapsedTime().asMilliseconds();

    if (waterInterval >= waterDelay) {
      if (currWaterIndex < (64 / 16) - 1)
        currWaterIndex++;
      else
        currWaterIndex = 0;
      clockWater.restart();
    }

    int index = 0;
    for (int h = 0; h < height; h++) {
      for (int w = 0; w < width; w++) {
        // todo: draw water
        {
          Sprite sprite{};
          sprite.setTexture(this->asset->getVectorWater()->at(currWaterIndex));
          sprite.setPosition(16 * w, 16 * h);

          window.draw(sprite);
        }

        // todo: draw grass
        {
          int firstGid = 1;
          int tileType = this->vectorTilelayerGrass.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite srpite{};
            srpite.setTexture(this->asset->getVectorGrass()->at(tileIndex));
            srpite.setPosition(16 * w, 16 * h);

            window.draw(srpite);
          }
        }

        // todo: draw bridge
        {
          int firstGid = 78;
          int tileType = this->vectorTilelayerBrigde.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(
                this->asset->getVectorWoodBridge()->at(tileIndex));
            sprite.setPosition(16 * w, 16 * h);

            window.draw(sprite);
          }
        }

        index++;
      }
    }

    // todo: draw collider
    for (int a = 0; a < this->vectorCollision.size(); a++) {
      window.draw(this->vectorCollision.at(a));
    }
  }

  int getHeight() { return height; }
  int getWidth() { return width; }

  bool canMove(Player &player, int dir) {
    RectangleShape *playerCollider = NULL;
    if (dir == 0) playerCollider = player.getColliderDown();
    if (dir == 1) playerCollider = player.getColliderUp();
    if (dir == 2) playerCollider = player.getColliderLeft();
    if (dir == 3) playerCollider = player.getColliderRight();

    int count = 0;
    for (int a = 0; a < this->vectorCollision.size(); a++) {
      if (this->vectorCollision.at(a).getGlobalBounds().intersects(
              playerCollider->getGlobalBounds())) {
        count++;
      }
    }

    return count == 0;
  }
};
#endif