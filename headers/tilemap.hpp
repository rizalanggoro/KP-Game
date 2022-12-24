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
  // Player *player;

  vector<vector<int>> vectorTilelayer{};

  vector<RectangleShape> vectorCollision{};

  int height;
  int width;

  Clock clockWater{};
  float waterDelay = 250;
  float waterInterval = 0;
  int currWaterIndex = 0;

  void loadMapJson();

  void loadCollision() {
    int colliderIndex = 0;
    int colliderLayerIndex = 4;
    for (int h = 0; h < this->height; h++) {
      for (int w = 0; w < this->width; w++) {
        int type =
            this->vectorTilelayer.at(colliderLayerIndex).at(colliderIndex);
        if (type != 0) {
          RectangleShape rect{};
          rect.setSize(Vector2f(16, 16));
          rect.setFillColor(Color(255, 0, 0, 50));
          rect.setPosition(16 * w, 16 * h);
          this->vectorCollision.push_back(rect);
        }

        colliderIndex++;
      }
    }
  }

 public:
  Tilemap(Asset *asset);
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
        Sprite spriteWater{};
        spriteWater.setTexture(
            this->asset->getVectorWater()->at(currWaterIndex));
        spriteWater.setPosition(16 * w, 16 * h);

        window.draw(spriteWater);

        // todo: draw grass
        int grassFirstGid = 1;
        int grassType = vectorTilelayer.at(1).at(index);
        if (grassType != 0) {
          int grassIndex = grassType - grassFirstGid;

          Sprite spriteGrass{};
          spriteGrass.setTexture(this->asset->getVectorGrass()->at(grassIndex));
          spriteGrass.setPosition(16 * w, 16 * h);

          window.draw(spriteGrass);
        }

        // todo: draw bridge
        int bridgeFirstGid = 78;
        int bridgeType = vectorTilelayer.at(3).at(index);
        if (bridgeType != 0) {
          int bridgeIndex = bridgeType - bridgeFirstGid;

          Sprite spriteBridge{};
          spriteBridge.setTexture(
              this->asset->getVectorWoodBridge()->at(bridgeIndex));
          spriteBridge.setPosition(16 * w, 16 * h);

          window.draw(spriteBridge);
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
    // auto gBounds = player.getSpritePlayer()->getGlobalBounds();
    // auto collUp = player.getColliderUp();
    // Vector2f collUpPosLeft = collUp->getPosition();
    // Vector2f collUpPosRight{collUpPosLeft.x + collUp->getSize().x,
    //                         collUpPosLeft.y};

    // int row = (collUpPosLeft.y / 16) - 1;
    // int col1 = collUpPosLeft.x / 16;
    // int col2 = collUpPosRight.x / 16;

    // cout << row << endl;
    // cout << col1 << endl;
    // cout << col2 << endl;

    // int count = 0;
    // for (int c = col1; c <= col2; c++) {
    //   if (this->vectorCollision.at(c + 30 * row)
    //           .getGlobalBounds()
    //           .intersects(collUp->getGlobalBounds()))
    //     count++;
    // }

    RectangleShape *playerCollider = NULL;
    if (dir == 0) playerCollider = player.getColliderDown();
    if (dir == 1) playerCollider = player.getColliderUp();
    if (dir == 2) playerCollider = player.getColliderLeft();
    if (dir == 3) playerCollider = player.getColliderRight();

    int count = 0;
    for (int a = 0; a < this->vectorCollision.size(); a++) {
      if (this->vectorCollision.at(a).getGlobalBounds().intersects(
              playerCollider->getGlobalBounds())) {
        // cout << "intersect" << endl;
        count++;
      }
    }

    return count == 0;
  }
};
#endif