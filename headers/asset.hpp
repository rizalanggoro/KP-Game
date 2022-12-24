#ifndef asset_hpp
#define asset_hpp

#include <SFML/Graphics.hpp>
#include <iostream>

#include "json.hpp"

using namespace std;
using namespace sf;
using json = nlohmann::json;

class Asset {
 private:
  Texture tilledDirt{};

  vector<Texture> vectorWater{}, vectorGrass{}, vectorWoodBridge{};
  vector<vector<Texture>> vectorCharSpite{};

  void loadTexture() {
    loadTextureGrass();
    loadTextureWater();
    loadTextureBridge();
    loadTextureBasicCharSprite();
  }

  void loadTextureBasicCharSprite() {
    auto path = "assets/images/Basic Charakter Spritesheet.png";
    for (int r = 0; r < 4; r++) {
      vector<Texture> vectorChar{};
      for (int c = 0; c < 4; c++) {
        Texture texture{};
        if (texture.loadFromFile(path, IntRect(c * 48, r * 48, 48, 48)))
          cout << "char: " << r << "," << c << " loaded!" << endl;
        vectorChar.push_back(texture);
      }
      this->vectorCharSpite.push_back(vectorChar);
    }
  }

  void loadTextureBridge() {
    int imageHeight = 48;
    int imageWidth = 80;

    for (int a = 0; a < (imageHeight / 16); a++) {
      for (int b = 0; b < (imageWidth / 16); b++) {
        Texture texture{};
        auto path = "assets/images/Wood Bridge.png";
        if (texture.loadFromFile(path, IntRect(b * 16, a * 16, 16, 16)))
          cout << "wood bridge tiled loaded!" << endl;
        vectorWoodBridge.push_back(texture);
      }
    }
  }

  void loadTextureGrass() {
    int imageHeight = 112;
    int imageWidth = 176;

    for (int a = 0; a < (imageHeight / 16); a++) {
      for (int b = 0; b < (imageWidth / 16); b++) {
        Texture texture{};
        auto path = "assets/images/Grass tiles v.2.png";
        if (texture.loadFromFile(path, IntRect(b * 16, a * 16, 16, 16)))
          cout << "grass tiled loaded!" << endl;
        vectorGrass.push_back(texture);
      }
    }
  }

  void loadTextureWater() {
    int imageHeight = 16;
    int imageWidth = 64;
    for (int a = 0; a < (imageWidth / 16); a++) {
      Texture texture{};
      if (texture.loadFromFile("assets/images/Water.png",
                               IntRect(a * 16, 0, 16, 16)))
        cout << "water loaded!" << endl;
      vectorWater.push_back(texture);
    }
  }

 public:
  Asset() { loadTexture(); }

  vector<Texture> *getVectorWater() { return &vectorWater; };
  vector<Texture> *getVectorGrass() { return &vectorGrass; };
  vector<Texture> *getVectorWoodBridge() { return &vectorWoodBridge; };
  vector<vector<Texture>> *getVectorCharSpite() { return &vectorCharSpite; };
};

#endif