#ifndef asset_hpp
#define asset_hpp

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "json.hpp"

using namespace std;
using namespace sf;
using json = nlohmann::json;

class Asset {
 private:
  string state;

  Font font{};

  vector<int> vectorContainGrass{}, vectorContainDirt{}, vectorContainBridge{};
  vector<Texture> vectorWater{}, vectorGrass{}, vectorWoodBridge{},
      vectorDirt{};

  vector<vector<Texture>> vectorBoats{};

  vector<vector<Texture>> vectorCharSpite{};

  vector<Texture> vectorSquareButtons{};
  vector<Texture> vectorGrassBiom{};
  vector<Texture> vectorChest{};
  vector<Texture> vectorHome{};

  Texture textureBackgroundMenu{};
  Texture textureButtonClose{};
  Texture textureFire{};
  Texture textureStar{};
  Texture textureBigButton{};
  Texture textureCoin{};

  // todo: sound assets
  SoundBuffer soundBullet{}, soundGrassWalk{}, soundGameOver{}, backsoundWar{},
      backsoundWorld{}, soundClick{};

  void loadSounds() {
    this->soundBullet.loadFromFile("assets/sounds/lmg_fire01.wav");
    this->soundGrassWalk.loadFromFile("assets/sounds/Step_grass.wav");
    this->soundGameOver.loadFromFile("assets/sounds/game-over.wav");
    this->backsoundWar.loadFromFile("assets/sounds/worldwar_aftercut.wav");
    this->backsoundWorld.loadFromFile("assets/sounds/worldmenu_aftercut.wav");
    this->soundClick.loadFromFile("assets/sounds/menuUpgradeDll_aftercut1.wav");
  }

  void loadFont() {
    string path = "assets/fonts/PressStart2P-Regular.ttf";
    if (this->font.loadFromFile(path)) cout << "font loaded!" << endl;
  }

  void loadTexture() {
    if (this->state == "world") {
      this->loadTextureTilesetDirt();
      this->loadTextureTilesetBridge();
      this->loadTextureBasicCharSprite();
      this->loadGrassBiom();
      this->loadTextureTilesetHome();
      this->loadTextureTilesetChest();
    } else if (this->state == "war") {
      this->loadTextureFire();
    }

    this->loadTextureTilesetGrass();
    this->loadTextureTilesetWater();
    this->loadTextureSquareButtons();
    this->loadTextureOthers();
    this->loadTextureBoatSets();
  }

  void loadTextureBoatSets() {
    // todo: load boat 1
    {
      string path = "assets/images/Boat 1 Set.png";
      vector<Texture> vectorTexture{};
      for (int a = 0; a < 4; a++) {
        Texture texture{};
        texture.loadFromFile(path, IntRect(a * 128, 0, 128, 128));
        vectorTexture.push_back(texture);
      }
      this->vectorBoats.push_back(vectorTexture);
    }
    // todo: load boat 2
    {
      string path = "assets/images/Boat 2 Set.png";
      vector<Texture> vectorTexture{};
      for (int a = 0; a < 3; a++) {
        Texture texture{};
        texture.loadFromFile(path, IntRect(a * 128, 0, 128, 128));
        vectorTexture.push_back(texture);
      }
      this->vectorBoats.push_back(vectorTexture);
    }
    // todo: load boat 3
    {
      string path = "assets/images/Boat 3 Set.png";
      vector<Texture> vectorTexture{};
      for (int a = 0; a < 4; a++) {
        Texture texture{};
        texture.loadFromFile(path, IntRect(a * 128, 0, 128, 128));
        vectorTexture.push_back(texture);
      }
      this->vectorBoats.push_back(vectorTexture);
    }
    // todo: load boat 4
    {
      string path = "assets/images/Boat 4 Set.png";
      vector<Texture> vectorTexture{};
      for (int a = 0; a < 3; a++) {
        Texture texture{};
        texture.loadFromFile(path, IntRect(a * 128, 0, 128, 128));
        vectorTexture.push_back(texture);
      }
      this->vectorBoats.push_back(vectorTexture);
    }
  }

  void loadTextureTilesetHome() {
    for (int h = 0; h < 4; h++) {
      for (int w = 0; w < 3; w++) {
        string path = "assets/images/home-set (1).png";
        Texture texture{};
        if (texture.loadFromFile(path, IntRect(w * 16, h * 16, 16, 16)))
          cout << "home loaded!" << endl;
        this->vectorHome.push_back(texture);
      }
    }
  }

  void loadTextureOthers() {
    this->textureBackgroundMenu.loadFromFile("assets/images/Setting menu.png");
    this->textureButtonClose.loadFromFile("assets/images/Button close.png");
    this->textureStar.loadFromFile("assets/images/Star.png");
    this->textureBigButton.loadFromFile("assets/images/UI Big Play Button.png");
    this->textureCoin.loadFromFile("assets/images/Coin.png");
  }

  void loadTextureTilesetChest() {
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
    string path = "assets/images/Fire4_1.png";
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

  void loadTextureTilesetDirt() {
    int firstGid = this->state == "world" ? 93 : -1;
    int index = 0;
    for (int h = 0; h < 8; h++) {
      for (int w = 0; w < 8; w++) {
        string path = "assets/images/Tilled Dirt.png";
        Texture texture{};

        if (find(this->vectorContainDirt.begin(), this->vectorContainDirt.end(),
                 (index + firstGid)) != this->vectorContainDirt.end()) {
          if (texture.loadFromFile(path, IntRect(w * 16, h * 16, 16, 16)))
            cout << "dirt tiled loaded!" << endl;
        }

        vectorDirt.push_back(texture);
        index++;
      }
    }
  }

  void loadTextureTilesetBridge() {
    int imageHeight = 48;
    int imageWidth = 80;

    int firstGid = this->state == "world" ? 78 : 0;
    int index = 0;

    for (int a = 0; a < (imageHeight / 16); a++) {
      for (int b = 0; b < (imageWidth / 16); b++) {
        Texture texture{};
        auto path = "assets/images/Wood Bridge.png";

        if (find(this->vectorContainBridge.begin(),
                 this->vectorContainBridge.end(),
                 (index + firstGid)) != this->vectorContainBridge.end()) {
          if (texture.loadFromFile(path, IntRect(b * 16, a * 16, 16, 16)))
            cout << "wood bridge tiled loaded!" << endl;
        }

        vectorWoodBridge.push_back(texture);
        index++;
      }
    }
  }

  void loadTextureTilesetGrass() {
    int imageHeight = 112;
    int imageWidth = 176;

    int firstGid = -1;
    if (this->state == "world") firstGid = 1;
    if (this->state == "war") firstGid = 5;

    int index = 0;
    for (int a = 0; a < (imageHeight / 16); a++) {
      for (int b = 0; b < (imageWidth / 16); b++) {
        Texture texture{};
        auto path = "assets/images/Grass tiles v.2.png";
        if (find(this->vectorContainGrass.begin(),
                 this->vectorContainGrass.end(),
                 (index + firstGid)) != this->vectorContainGrass.end()) {
          if (texture.loadFromFile(path, IntRect(b * 16, a * 16, 16, 16)))
            cout << "grass tiled loaded!" << endl;
        }
        vectorGrass.push_back(texture);

        index++;
      }
    }
  }

  void loadTextureTilesetWater() {
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

  void loadMapJson() {
    if (this->state == "world") {
      ifstream file("assets/map/game.tmj");
      json data = json::parse(file);
      auto layers = data["layers"];
      for (auto it = layers.begin(); it != layers.end(); it++) {
        auto layer = it.value();
        string layername = layer["name"].get<string>();
        if (layername == "grass")
          this->vectorContainGrass = layer["data"].get<vector<int>>();
        if (layername == "dirt")
          this->vectorContainDirt = layer["data"].get<vector<int>>();
        if (layername == "bridge")
          this->vectorContainBridge = layer["data"].get<vector<int>>();
      }

    } else if (this->state == "war") {
      ifstream file("assets/map/map-war.tmj");
      json data = json::parse(file);
      auto layers = data["layers"];
      for (auto it = layers.begin(); it != layers.end(); it++) {
        auto layer = it.value();
        string layername = layer["name"].get<string>();
        if (layername == "grass") {
          this->vectorContainGrass = layer["data"].get<vector<int>>();
        }
      }
    }
  }

 public:
  Asset(string state) {
    this->state = state;

    this->loadMapJson();
    this->loadTexture();
    this->loadFont();
    this->loadSounds();
  }

  // todo: sound assets
  SoundBuffer *getSoundBullet() { return &this->soundBullet; }
  SoundBuffer *getSoundGrassWalk() { return &this->soundGrassWalk; }
  SoundBuffer *getSoundGameOver() { return &this->soundGameOver; }
  SoundBuffer *getSoundClick() { return &this->soundClick; }
  SoundBuffer *getBacksoundWar() { return &this->backsoundWar; }
  SoundBuffer *getBacksoundWorld() { return &this->backsoundWorld; }

  // todo: font assets
  Font *getFont() { return &this->font; }

  // todo: texture assets
  vector<Texture> *getVectorWater() { return &vectorWater; };
  vector<Texture> *getVectorGrass() { return &vectorGrass; };
  vector<Texture> *getVectorWoodBridge() { return &vectorWoodBridge; };
  vector<Texture> *getVectorDirt() { return &vectorDirt; };
  vector<vector<Texture>> *getVectorCharSpite() { return &vectorCharSpite; };

  vector<vector<Texture>> *getVectorBoats() { return &this->vectorBoats; }

  vector<Texture> *getVectorGrassBiom() { return &this->vectorGrassBiom; }
  vector<Texture> *getVectorChest() { return &this->vectorChest; }
  vector<Texture> *getVectorHome() { return &this->vectorHome; }

  Texture *getTextureFire() { return &this->textureFire; }
  Texture *getTextureStar() { return &this->textureStar; }
  Texture *getTextureButtonClose() { return &this->textureButtonClose; }
  Texture *getTextureBigButton() { return &this->textureBigButton; }
  Texture *getTextureBackgroundMenu() { return &this->textureBackgroundMenu; }
  Texture *getTextureCoin() { return &this->textureCoin; }

  vector<Texture> *getVectorSquareButtons() {
    return &this->vectorSquareButtons;
  }
};

#endif