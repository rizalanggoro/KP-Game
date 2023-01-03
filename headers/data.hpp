#ifndef data_hpp
#define data_hpp

#include <fstream>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class Data {
 private:
  json jsonBoats;
  json jsonProfile;

  string pathBoats = "data/boats.json";
  string pathProfile = "data/profile.json";

 public:
  // fungsi untuk memuat data profile dan data kapal dari file .json
  void load() {
    ifstream fileBoats(pathBoats);
    ifstream fileProfile(pathProfile);

    this->jsonBoats = json::parse(fileBoats);
    this->jsonProfile = json::parse(fileProfile);
  }

  // fungsi untuk menyimpan data profile dan data kapal ke file .json
  void save() {
    ofstream fileBoats(pathBoats);
    ofstream fileProfile(pathProfile);

    fileBoats << this->jsonBoats << endl;
    fileProfile << this->jsonProfile << endl;
  }

  // todo: getters
  json *getJsonBoats() { return &this->jsonBoats; }
  json *getJsonProfile() { return &this->jsonProfile; }
};

#endif