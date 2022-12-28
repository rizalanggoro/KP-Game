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
  Font font{};

  vector<Texture> vectorWater{}, vectorGrass{}, vectorWoodBridge{},
      vectorDirt{};

  vector<Texture> vectorBoatColor1{};
  vector<Texture> vectorBoatColor3{};
  vector<Texture> vectorCannon1{};
  vector<Texture> vectorCannon2{};
  vector<Texture> vectorCannon3{};
  vector<Texture> vectorCannon4{};

  vector<vector<Texture>> vectorCharSpite{};

  vector<Texture> vectorSquareButtons{};
  vector<Texture> vectorGrassBiom{};
  vector<Texture> vectorChest{};
  vector<Texture> vectorSettingMenu{};

  Texture textureFire{};
  Texture textureDialogBox{};

  void loadFont() {
    string path = "assets/fonts/PressStart2P-Regular.ttf";
    if (this->font.loadFromFile(path)) cout << "font loaded!" << endl;
  }

  void loadTexture() {
    this->loadTextureGrass();
    this->loadTextureWater();
    this->loadTextureBridge();
    this->loadTextureDirt();
    this->loadTextureBasicCharSprite();

    this->loadBoat();
    this->loadCannon();
    this->loadTextureFire();
    this->loadTextureSquareButtons();
    this->loadGrassBiom();
    this->loadTextureChest();
    this->loadTextureSettingMenu();
    this->loadTextureDialogBox();
  }

  void loadTextureDialogBox() {
    string path = "assets/images/dialog box.png";
    if (this->textureDialogBox.loadFromFile(path))
      cout << "dialog box loaded!" << endl;
  }

  void loadTextureSettingMenu() {
    string path = "assets/images/Setting menu.png";
    for (int a = 0; a < 2; a++) {
      Texture texture{};
      if (texture.loadFromFile(path, IntRect(a * 128, 0, 128, 144)))
        cout << "setting menu loaded!" << endl;
      this->vectorSettingMenu.push_back(texture);
    }
  }

  void loadTextureChest() {
    for (int r = 0; r < 2; r++) {
      for (int c = 0; c < 5; c++) {
        auto path = "assets/images/Chest.png";
        Texture texture{};
        if (texture.loadFromFile(path, IntRect(c * 48, r * 48, 48, 48)))
          cout << "chest loaded!" << endl;
        this->vectorChest.push_back(texture);
      }
    }
  }

  void loadGrassBiom() {
    for (int r = 0; r < 5; r++) {
      for (int c = 0; c < 9; c++) {
        string path = "assets/images/Basic Grass Biom things 1.png";
        Texture texture{};
        texture.loadFromFile(path, IntRect(c * 16, r * 16, 16, 16));
        this->vectorGrassBiom.push_back(texture);
      }
    }
  }

  void loadTextureSquareButtons() {
    string path = "assets/images/Square Buttons 26x26.png";

    for (int r = 0; r < 4; r++) {
      for (int c = 0; c < 2; c++) {
        Texture texture{};
        texture.loadFromFile(path, IntRect(c * 48, r * 48, 48, 48));

        this->vectorSquareButtons.push_back(texture);
      }
    }
  }

  void loadTextureFire() {
    string path = "assets/images/fire/Fire4_1.png";
    if (this->textureFire.loadFromFile(path))
      cout << path << " loaded!" << endl;
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

  void loadTextureDirt() {
    for (int h = 0; h < 8; h++) {
      for (int w = 0; w < 8; w++) {
        string path = "assets/images/Tilled Dirt.png";
        Texture texture{};
        if (texture.loadFromFile(path, IntRect(w * 16, h * 16, 16, 16)))
          cout << "dirt tiled loaded!" << endl;
        vectorDirt.push_back(texture);
      }
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

    // todo: load boat color 3
    for (int a = 1; a <= 4; a++) {
      string path = "assets/images/Boats_color3/Boat_color3_";
      path += to_string(a);
      path += ".png";

      cout << path << endl;

      Texture texture{};
      if (texture.loadFromFile(path))
        cout << "boat color 3 / " << a << " loaded!" << endl;
      this->vectorBoatColor3.push_back(texture);
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
  Asset() {
    this->loadTexture();
    this->loadFont();
  }

  Font *getFont() { return &this->font; }

  vector<Texture> *getVectorWater() { return &vectorWater; };
  vector<Texture> *getVectorGrass() { return &vectorGrass; };
  vector<Texture> *getVectorWoodBridge() { return &vectorWoodBridge; };
  vector<Texture> *getVectorDirt() { return &vectorDirt; };
  vector<vector<Texture>> *getVectorCharSpite() { return &vectorCharSpite; };

  vector<Texture> *getVectorBoatColor1() { return &this->vectorBoatColor1; }
  vector<Texture> *getVectorBoatColor3() { return &this->vectorBoatColor3; }
  vector<Texture> *getVectorCannon1() { return &this->vectorCannon1; }
  vector<Texture> *getVectorCannon2() { return &this->vectorCannon2; }
  vector<Texture> *getVectorCannon3() { return &this->vectorCannon3; }
  vector<Texture> *getVectorCannon4() { return &this->vectorCannon4; }

  vector<Texture> *getVectorGrassBiom() { return &this->vectorGrassBiom; }
  vector<Texture> *getVectorChest() { return &this->vectorChest; }
  vector<Texture> *getVectorSettingMenu() { return &this->vectorSettingMenu; }

  Texture *getTextureFire() { return &this->textureFire; }
  Texture *getTextureDialogBox() { return &this->textureDialogBox; }

  vector<Texture> *getVectorSquareButtons() {
    return &this->vectorSquareButtons;
  }
};

#endif