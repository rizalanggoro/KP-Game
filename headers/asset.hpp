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

  vector<Texture> vectorBoatColor1{};
  vector<Texture> vectorCannon1{};
  vector<Texture> vectorCannon2{};
  vector<Texture> vectorCannon3{};
  vector<Texture> vectorCannon4{};

  vector<vector<Texture>> vectorCharSpite{};

  void loadTexture() {
    loadTextureGrass();
    loadTextureWater();
    loadTextureBridge();
    loadTextureBasicCharSprite();

    this->loadBoat();
    this->loadCannon();
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

  void loadBoat() {
    // todo: load boat color 1
    for (int a = 1; a <= 4; a++) {
      string path = "assets/images/Boats_color1/Boat_color1_";
      path += to_string(a);
      path += ".png";

      cout << path << endl;

      Texture texture{};
      if (texture.loadFromFile(path))
        cout << "boat color 1 / " << a << " loaded!" << endl;
      this->vectorBoatColor1.push_back(texture);
    }
  }

  void loadCannon() {
    int count[] = {4, 3, 4, 3};
    for (int a = 0; a < 4; a++) {
      for (int b = 1; b <= count[a]; b++) {
        string path = "assets/images/Cannon";
        path += to_string(a + 1);
        path += "_color1/Cannon";
        path += to_string(a + 1);
        path += "_color1_";
        path += to_string(b);
        path += ".png";

        Texture texture{};
        if (texture.loadFromFile(path)) cout << path << " loaded!" << endl;

        if (a == 0) this->vectorCannon1.push_back(texture);
        if (a == 1) this->vectorCannon2.push_back(texture);
        if (a == 2) this->vectorCannon3.push_back(texture);
        if (a == 3) this->vectorCannon4.push_back(texture);
      }
    }
  }

 public:
  Asset() { loadTexture(); }

  vector<Texture> *getVectorWater() { return &vectorWater; };
  vector<Texture> *getVectorGrass() { return &vectorGrass; };
  vector<Texture> *getVectorWoodBridge() { return &vectorWoodBridge; };
  vector<vector<Texture>> *getVectorCharSpite() { return &vectorCharSpite; };

  vector<Texture> *getVectorBoatColor1() { return &this->vectorBoatColor1; }
  vector<Texture> *getVectorCannon1() { return &this->vectorCannon1; }
  vector<Texture> *getVectorCannon2() { return &this->vectorCannon2; }
  vector<Texture> *getVectorCannon3() { return &this->vectorCannon3; }
  vector<Texture> *getVectorCannon4() { return &this->vectorCannon4; }
};

#endif