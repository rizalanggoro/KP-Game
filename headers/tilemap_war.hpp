#ifndef tilemap_war_hpp
#define tilemap_war_hpp

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "asset.hpp"
#include "json.hpp"
// #include "player_boat.hpp"

using namespace std;
using namespace sf;
using json = nlohmann::json;

class TilemapWar {
 private:
  Asset *asset;

  float tileSize = 16;
  // float tileTargetSize = 48;
  float tileTargetSize = 16;
  float tileScaleFactor = 0;

  vector<int> vectorTilelayerGrass{}, vectorTilelayerCollision{};
  vector<RectangleShape> vectorCollision{};

  int width;
  int height;

  Clock clockWaterFrame;
  int currWaterFrameIndex = 0;
  float waterFrameDelay = 250;
  float waterFrameInterval = 0;

  void loadMapJson() {
    ifstream file("assets/map/map-war.tmj");
    json data = json::parse(file);

    this->width = data["width"].get<int>();
    this->height = data["height"].get<int>();

    cout << "map war height: " << this->height << endl;
    cout << "map war width: " << this->width << endl;

    // todo: load tile layers
    auto layers = data["layers"];
    for (auto it = layers.begin(); it != layers.end(); it++) {
      auto layer = it.value();
      string layerType = layer["type"].get<string>();
      string layerName = layer["name"].get<string>();
      if (layerType == "tilelayer") {
        auto data = layer["data"].get<vector<int>>();
        if (layerName == "grass") this->vectorTilelayerGrass = data;
        if (layerName == "collision") this->vectorTilelayerCollision = data;
      }
    }
  }

  void loadCollision() {
    int index = 0;
    for (int h = 0; h < this->height; h++) {
      for (int w = 0; w < this->width; w++) {
        int type = this->vectorTilelayerCollision.at(index);
        if (type != 0) {
          RectangleShape rect{};
          rect.setFillColor(Color(255, 0, 0, 50));
          rect.setSize(Vector2f(this->tileTargetSize, this->tileTargetSize));
          rect.setPosition(w * this->tileTargetSize, h * this->tileTargetSize);

          this->vectorCollision.push_back(rect);
        }

        index++;
      }
    }
  }

 public:
  int getWidth() { return this->width; }
  int getHeight() { return this->height; }
  float getTileTargetSize() { return this->tileTargetSize; }
  float getScaleFactor() { return this->tileScaleFactor; }

  TilemapWar(Asset *asset) {
    this->asset = asset;

    this->tileScaleFactor = this->tileTargetSize / this->tileSize;

    this->loadMapJson();
    this->loadCollision();
  }

  void draw(RenderWindow &window) {
    // todo: handle water animation
    this->waterFrameInterval =
        this->clockWaterFrame.getElapsedTime().asMilliseconds();
    if (this->waterFrameInterval >= this->waterFrameDelay) {
      if (this->currWaterFrameIndex < this->asset->getVectorWater()->size() - 1)
        this->currWaterFrameIndex++;
      else
        this->currWaterFrameIndex = 0;

      this->clockWaterFrame.restart();
    }

    int index = 0;
    for (int r = 0; r < this->height; r++) {
      for (int c = 0; c < this->width; c++) {
        // todo: draw water
        {
          Sprite sprite{};
          sprite.setTexture(
              this->asset->getVectorWater()->at(this->currWaterFrameIndex));
          sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
          sprite.setPosition(c * this->tileTargetSize,
                             r * this->tileTargetSize);
          window.draw(sprite);
        }

        // todo: draw grass
        {
          int firstGid = 5;
          int tileType = this->vectorTilelayerGrass.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorGrass()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * c,
                               this->tileTargetSize * r);

            window.draw(sprite);
          }
        }

        index++;
      }
    }

    // todo: draw collision
    for (int a = 0; a < this->vectorCollision.size(); a++) {
      window.draw(this->vectorCollision.at(a));
    }
  }

  bool canMove(RectangleShape *rect) {
    int count = 0;
    for (auto collider : this->vectorCollision) {
      if (rect->getGlobalBounds().intersects(collider.getGlobalBounds()))
        count++;
    }
    return count == 0;
  }
};

#endif