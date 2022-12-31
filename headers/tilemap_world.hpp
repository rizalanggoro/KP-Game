#ifndef tilemap_hpp
#define tilemap_hpp

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "asset.hpp"
#include "json.hpp"
#include "player.hpp"

// #define DEBUG

using namespace std;
using namespace sf;
using json = nlohmann::json;

class Tilemap {
 private:
  Asset *asset;

  vector<int> vectorTilelayerGrass{}, vectorTilelayerDirt{},
      vectorTilelayerBridge{}, vectorTilelayerCollision{},
      vectorTilelayerTrunk{}, vectorTilelayerLeaf{}, vectorTilelayerWall{},
      vectorTilelayerRoof{};

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

  Clock clockChest{};
  float chestDelay = 250;
  float chestInterval = 0;
  int currChestIndex = 0;

  Text textWarPoint{};
  Text textShopPoint{};

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
        if (layerName == "trunk") this->vectorTilelayerTrunk = layerData;
        if (layerName == "leaf") this->vectorTilelayerLeaf = layerData;
        if (layerName == "wall") this->vectorTilelayerWall = layerData;
        if (layerName == "roof") this->vectorTilelayerRoof = layerData;
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
          rect.setFillColor(Color::Transparent);
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
  Text *getTextWarPoint() { return &this->textWarPoint; }
  Text *getTextShopPoint() { return &this->textShopPoint; }

  Tilemap(Asset *asset) {
    this->asset = asset;

    this->tileScaleFactor = this->tileTargetSize / this->tileSize;

    this->loadMapJson();
    this->loadTilelayerCollision();
  }

  void draw(RenderWindow &window) {
    // todo: water animation
    waterInterval = clockWater.getElapsedTime().asMilliseconds();
    this->chestInterval = this->clockChest.getElapsedTime().asMilliseconds();

    if (this->chestInterval >= this->chestDelay) {
      if (currChestIndex < 4)
        currChestIndex++;
      else
        currChestIndex = 0;
      clockChest.restart();
    }

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

        // todo: draw trunk
        {
          int firstGid = 161;
          int tileType = this->vectorTilelayerTrunk.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorGrassBiom()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

            window.draw(sprite);
          }
        }

        // todo: draw wall
        {
          int firstGid = 206;
          int tileType = this->vectorTilelayerWall.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorHome()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

            window.draw(sprite);
          }
        }

        // todo: draw dirt
        {
          int firstGid = 93;
          int tileType = this->vectorTilelayerDirt.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorDirt()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

            window.draw(sprite);
          }
        }

        // todo: draw war checkpoint
        {
          this->textWarPoint.setString(">> FIGHT! <<");
          this->textWarPoint.setFont(*this->asset->getFont());
          this->textWarPoint.setCharacterSize(16);
          this->textWarPoint.setFillColor(Color(107, 75, 91, 255));

          auto tBounds = this->textWarPoint.getGlobalBounds();
          float marginX = ((6 * tileTargetSize) - tBounds.width) / 2;
          float marginY = ((6 * tileTargetSize) - tBounds.height) / 2;
          this->textWarPoint.setPosition(30 * this->tileTargetSize + marginX,
                                         17 * this->tileTargetSize + marginY);
          window.draw(this->textWarPoint);

          float chestSize = 48;
          float chestTargetSize = 48 * 3;
          float chestScaleFactor = chestTargetSize / chestSize;

          Sprite spriteChest{};
          spriteChest.setTexture(
              this->asset->getVectorChest()->at(this->currChestIndex));
          spriteChest.setScale(chestScaleFactor, chestScaleFactor);

          auto tPos = this->textWarPoint.getPosition();
          spriteChest.setPosition(
              tPos.x + ((tBounds.width - chestTargetSize) / 2),
              tPos.y - chestTargetSize * 3 / 4);
          window.draw(spriteChest);
        }

        // todo: draw shop / upgrade checkpoint
        {
          this->textShopPoint.setString(">> UPGRADE! <<");
          this->textShopPoint.setFont(*this->asset->getFont());
          this->textShopPoint.setCharacterSize(16);
          this->textShopPoint.setFillColor(Color(107, 75, 91, 255));

          auto tBounds = this->textShopPoint.getGlobalBounds();
          float marginX = ((5 * tileTargetSize) - tBounds.width) / 2;
          float marginY = ((1 * tileTargetSize) - tBounds.height) / 2;
          this->textShopPoint.setPosition(12 * this->tileTargetSize + marginX,
                                          22 * this->tileTargetSize + marginY);
          window.draw(this->textShopPoint);
        }
        index++;
      }
    }

    // todo: draw collider
    for (int a = 0; a < this->vectorCollision.size(); a++) {
      window.draw(this->vectorCollision.at(a));
    }
  }

  void drawLeaf(RenderWindow &window) {
    // todo: draw trunk
    int index = 0;
    for (int h = 0; h < this->height; h++) {
      for (int w = 0; w < this->width; w++) {
        // todo: draw leaf
        {
          int firstGid = 161;
          int tileType = this->vectorTilelayerLeaf.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorGrassBiom()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

            window.draw(sprite);
          }
        }

        // todo: draw roof
        {
          int firstGid = 206;
          int tileType = this->vectorTilelayerRoof.at(index);
          if (tileType != 0) {
            int tileIndex = tileType - firstGid;

            Sprite sprite{};
            sprite.setTexture(this->asset->getVectorHome()->at(tileIndex));
            sprite.setScale(this->tileScaleFactor, this->tileScaleFactor);
            sprite.setPosition(this->tileTargetSize * w,
                               this->tileTargetSize * h);

            window.draw(sprite);
          }
        }

        index++;
      }
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