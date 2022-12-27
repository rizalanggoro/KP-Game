#ifndef tilemap_hpp
#define tilemap_hpp

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "asset.hpp"
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
  vector<int> vectorTilelayerBridge{};
  vector<int> vectorTilelayerCollision{};

  vector<RectangleShape> vectorCollision{};

  float tileSize = 16;
  float tileTargetSize = 48;
  float tileScaleFactor = 1;

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
        if (layerName == "bridge") this->vectorTilelayerBridge = layerData;
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
          rect.setSize(Vector2f(this->tileTargetSize, this->tileTargetSize));
          rect.setFillColor(Color(255, 0, 0, 50));
          rect.setPosition(this->tileTargetSize * w, this->tileTargetSize * h);
          this->vectorCollision.push_back(rect);
        }

        index++;
      }
    }
  }

 public:
  int getHeight() { return height; }
  int getWidth() { return width; }
  float getTileTargetSize() { return this->tileTargetSize; }

  Tilemap(Asset *asset) {
    this->asset = asset;

    this->tileScaleFactor = this->tileTargetSize / this->tileSize;

    this->loadMapJson();
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
          sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
          sprite.setTexture(this->asset->getVectorWater()->at(currWaterIndex));
          sprite.setPosition(this->tileTargetSize * w,
                             this->tileTargetSize * h);

          window.draw(sprite);
        }

        // todo: draw grass
        {
          int firstGid = 1;
          int tileType = this->vectorTilelayerGrass.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorGrass()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

            window.draw(sprite);
          }
        }

        // todo: draw bridge
        {
          int firstGid = 78;
          int tileType = this->vectorTilelayerBridge.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(
                this->asset->getVectorWoodBridge()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

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

  bool canMove(Player &player, string dir) {
    RectangleShape *playerCollider = NULL;
    if (dir == "d") playerCollider = player.getColliderDown();
    if (dir == "u") playerCollider = player.getColliderUp();
    if (dir == "l") playerCollider = player.getColliderLeft();
    if (dir == "r") playerCollider = player.getColliderRight();

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