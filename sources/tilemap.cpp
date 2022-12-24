#include "../headers/tilemap.hpp"

Tilemap::Tilemap(Asset *asset) {
  this->asset = asset;

  loadMapJson();
  this->loadCollision();
}

void Tilemap::loadMapJson() {
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
    if (layerType == "tilelayer") {
      auto layerData = layer["data"].get<vector<int>>();
      vectorTilelayer.push_back(layerData);
    }
  }
}
