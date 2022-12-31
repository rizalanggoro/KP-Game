#ifndef state_war_hpp
#define state_war_hpp

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

#include "asset.hpp"
#include "data.hpp"
#include "direction.hpp"
#include "enemy_boat.hpp"
#include "json.hpp"
#include "player_boat.hpp"
#include "tilemap_war.hpp"

#define DEBUG

using json = nlohmann::json;
using namespace std;
using namespace sf;

class StateWar {
 private:
  string *state;
  RenderWindow *window;
  Data *data;

  bool isInitialized = false;

  int boatLevel = -1;
  float damage = -1;
  float boatVelocity = -1;
  float bulletVelocity = -1;
  float life = -1;
  float maxLife = -1;
  int points = 0;

  bool isGameOver = false;

  Asset asset{"war"};
  View view{};

  TilemapWar tilemapWar{&asset};
  PlayerBoat playerBoat{&asset, &tilemapWar};

  vector<EnemyBoat> vectorEnemyBoat{};
  Clock clockEnemyBoat;
  float spawnEnemyBoatInterval = 0;
  float spawnEnemyBoatDelay = 2000;

  float colliderMapSize = 128;
  Vector2i playerRealPos;

  Clock clockReturn{};
  int returnTextIndex = 0;
  float returnTextInterval = 0;

  void moveMap(Direction direction) {
    float mapMaxY = this->window->getSize().y -
                    this->playerBoat.getBoatTargetSize() -
                    this->colliderMapSize;
    float mapMaxX = this->window->getSize().x -
                    this->playerBoat.getBoatTargetSize() -
                    this->colliderMapSize;
    float mapMin = this->colliderMapSize;

    float px = this->playerRealPos.x;
    float py = this->playerRealPos.y;
    float pv = this->playerBoat.getVelocity();
    float pdv = pv * sqrt(2) / 2;

    switch (direction) {
      case DIR_UP:
        if (py <= mapMin) this->view.move(0, -pv);
        break;

      case DIR_DOWN:
        if (py >= mapMaxY) this->view.move(0, pv);
        break;

      case DIR_RIGHT:
        if (px >= mapMaxX) this->view.move(pv, 0);
        break;

      case DIR_LEFT:
        if (px <= mapMin) this->view.move(-pv, 0);
        break;

      case DIR_UP_RIGHT:
        if (py <= mapMin) this->view.move(0, -pdv);
        if (px >= mapMaxX) this->view.move(pdv, 0);
        break;

      case DIR_UP_LEFT:
        if (py <= mapMin) this->view.move(0, -pdv);
        if (px <= mapMin) this->view.move(-pdv, 0);
        break;

      case DIR_DOWN_RIGHT:
        if (py >= mapMaxY) this->view.move(0, pdv);
        if (px >= mapMaxX) this->view.move(pdv, 0);
        break;

      case DIR_DOWN_LEFT:
        if (py >= mapMaxY) this->view.move(0, pdv);
        if (px <= mapMin) this->view.move(-pdv, 0);
        break;
    }
  }

  void handleKeyboard() {
    bool keyUp = Keyboard::isKeyPressed(Keyboard::Up);
    bool keyDown = Keyboard::isKeyPressed(Keyboard::Down);
    bool keyRight = Keyboard::isKeyPressed(Keyboard::Right);
    bool keyLeft = Keyboard::isKeyPressed(Keyboard::Left);

    bool canMoveUp = this->tilemapWar.canMove(this->playerBoat.getColliderUp());
    bool canMoveDown =
        this->tilemapWar.canMove(this->playerBoat.getColliderDown());
    bool canMoveRight =
        this->tilemapWar.canMove(this->playerBoat.getColliderRight());
    bool canMoveLeft =
        this->tilemapWar.canMove(this->playerBoat.getColliderLeft());

    bool canMoveMapUp =
        this->tilemapWar.canMoveMap(this->playerBoat.getColliderUp());
    bool canMoveMapDown =
        this->tilemapWar.canMoveMap(this->playerBoat.getColliderDown());
    bool canMoveMapRight =
        this->tilemapWar.canMoveMap(this->playerBoat.getColliderRight());
    bool canMoveMapLeft =
        this->tilemapWar.canMoveMap(this->playerBoat.getColliderLeft());

    if (keyUp && keyRight) {
      if (canMoveUp && canMoveRight && canMoveMapUp && canMoveMapRight) {
        this->moveMap(DIR_UP_RIGHT);
        this->playerBoat.moveUpRight();
      } else if (canMoveUp && canMoveMapUp) {
        this->moveMap(DIR_UP);
        this->playerBoat.moveUp();
      } else if (canMoveRight && canMoveMapRight) {
        this->moveMap(DIR_RIGHT);
        this->playerBoat.moveRight();
      }

    } else if (keyUp && keyLeft) {
      if (canMoveUp && canMoveLeft && canMoveMapUp && canMoveMapLeft) {
        this->moveMap(DIR_UP_LEFT);
        this->playerBoat.moveUpLeft();
      } else if (canMoveUp && canMoveMapUp) {
        this->moveMap(DIR_UP);
        this->playerBoat.moveUp();
      } else if (canMoveLeft && canMoveMapLeft) {
        this->moveMap(DIR_LEFT);
        this->playerBoat.moveLeft();
      }

    } else if (keyDown && keyRight) {
      if (canMoveDown && canMoveRight && canMoveMapDown && canMoveMapRight) {
        this->moveMap(DIR_DOWN_RIGHT);
        this->playerBoat.moveDownRight();
      } else if (canMoveDown && canMoveMapDown) {
        this->moveMap(DIR_DOWN);
        this->playerBoat.moveDown();
      } else if (canMoveRight && canMoveMapRight) {
        this->moveMap(DIR_RIGHT);
        this->playerBoat.moveRight();
      }

    } else if (keyDown && keyLeft) {
      if (canMoveDown && canMoveLeft && canMoveMapDown && canMoveMapLeft) {
        this->moveMap(DIR_DOWN_LEFT);
        this->playerBoat.moveDownLeft();
      } else if (canMoveDown && canMoveMapDown) {
        this->moveMap(DIR_DOWN);
        this->playerBoat.moveDown();
      } else if (canMoveLeft && canMoveMapLeft) {
        this->moveMap(DIR_LEFT);
        this->playerBoat.moveLeft();
      }
    }

    else if (keyUp) {
      if (canMoveUp && canMoveMapUp) {
        this->moveMap(DIR_UP);
        this->playerBoat.moveUp();
      }
    } else if (keyDown) {
      if (canMoveDown && canMoveMapDown) {
        this->moveMap(DIR_DOWN);
        this->playerBoat.moveDown();
      }
    } else if (keyRight) {
      if (canMoveRight && canMoveMapRight) {
        this->moveMap(DIR_RIGHT);
        this->playerBoat.moveRight();
      }
    } else if (keyLeft) {
      if (canMoveLeft && canMoveMapLeft) {
        this->moveMap(DIR_LEFT);
        this->playerBoat.moveLeft();
      }
    }

    if (Keyboard::isKeyPressed(Keyboard::Space)) {
      this->playerBoat.fire();
    }
  }

  void drawGui(RenderWindow &window) {
    this->drawGuiBoatStats(window);
    if (this->isGameOver) this->drawGuiGameOver(window);
  }

  void drawGuiGameOver(RenderWindow &window) {
    auto wCenter = window.getView().getCenter();
    auto wSize = window.getSize();

    RectangleShape darkBg{};
    darkBg.setFillColor(Color(0, 0, 0, 150));
    darkBg.setSize(Vector2f(wSize.x, wSize.y));
    window.draw(darkBg);

    // todo: draw text title
    Text title{};
    title.setString("GAME OVER!");
    title.setFont(*this->asset.getFont());
    title.setCharacterSize(48);
    title.setStyle(Text::Bold);

    auto titleBounds = title.getGlobalBounds();
    title.setPosition(wCenter.x - titleBounds.width / 2,
                      wCenter.y - titleBounds.height / 2);

    window.draw(title);

    // todo: draw total point
    Text totalPoint{};
    totalPoint.setFont(*this->asset.getFont());
    totalPoint.setString("Total points: " + to_string(this->points));
    totalPoint.setFillColor(Color(192, 192, 192, 255));
    totalPoint.setCharacterSize(24);

    auto titlePos = title.getPosition();
    auto totalPointBounds = totalPoint.getGlobalBounds();
    totalPoint.setPosition(wCenter.x - totalPointBounds.width / 2,
                           titlePos.y + titleBounds.height + 32);

    window.draw(totalPoint);

    string returnText[] = {
        ">>   PRESS ENTER TO GO HOME   <<",
        ">>  PRESS ENTER TO GO HOME  <<",
        ">> PRESS ENTER TO GO HOME <<",
    };

    this->returnTextInterval =
        this->clockReturn.getElapsedTime().asMilliseconds();
    if (this->returnTextInterval >= 250) {
      if (this->returnTextIndex < 2)
        this->returnTextIndex++;
      else
        this->returnTextIndex = 0;
      this->clockReturn.restart();
    }

    Text textReturn{};
    textReturn.setString(returnText[this->returnTextIndex]);
    textReturn.setFillColor(Color::White);
    textReturn.setCharacterSize(16);
    textReturn.setFont(*this->asset.getFont());

    auto textReturnBounds = textReturn.getGlobalBounds();
    textReturn.setPosition(wCenter.x - textReturnBounds.width / 2,
                           wSize.y - 48 - textReturnBounds.height);
    window.draw(textReturn);

    // todo: draw button back
    // Sprite bgButton{};
    // bgButton.setTexture(*this->asset.getTextureBigButton());
    // bgButton.setScale(64 / 27, 64 / 27);

    // auto totalPointPos = totalPoint.getPosition();
    // auto bgButtonBounds = bgButton.getGlobalBounds();
    // bgButton.setPosition(wCenter.x - bgButtonBounds.width / 2,
    //                      wSize.y - bgButtonBounds.height - 48);

    // window.draw(bgButton);

    // todo: draw text button back
    // Text textButtonBack{};
    // textButtonBack.setString("RETURN");
    // textButtonBack.setFont(*this->asset.getFont());
    // textButtonBack.setCharacterSize(16);
    // textButtonBack.setFillColor(Color(182, 137, 98, 255));

    // auto bgButtonPos = bgButton.getPosition();
    // auto textButtonBackBounds = textButtonBack.getGlobalBounds();
    // textButtonBack.setPosition(
    //     bgButtonPos.x + (bgButtonBounds.width - textButtonBackBounds.width) /
    //     2, bgButtonPos.y +
    //         (bgButtonBounds.height - textButtonBackBounds.height) * 2 / 5);

    // window.draw(textButtonBack);
  }

  void drawGuiBoatStats(RenderWindow &window) {
    float targetBgBoat = 96;
    float bgBoatScale = targetBgBoat / 32;

    Sprite bgBoat{};
    bgBoat.setTexture(this->asset.getVectorSquareButtons()->at(4));
    bgBoat.setScale(bgBoatScale, bgBoatScale);
    bgBoat.setPosition(-8 * bgBoatScale, -8 * bgBoatScale);
    window.draw(bgBoat);

    auto bgBoatPos = bgBoat.getPosition();
    auto bgBoatBounds = bgBoat.getGlobalBounds();

    float boatTargetSize = 72;
    float boatScale = boatTargetSize / 128;

    Sprite boat{};
    boat.setTexture(
        this->asset.getVectorBoats()->at(this->boatLevel - 1).at(0));
    boat.setScale(boatScale, boatScale);

    auto boatBounds = boat.getGlobalBounds();
    boat.setPosition(
        bgBoatPos.x + (bgBoatBounds.width - boatBounds.width) / 2,
        bgBoatPos.y + (bgBoatBounds.height - boatBounds.height) / 2);

    window.draw(boat);

    // todo: draw point
    Text textPoint{};
    textPoint.setString("Points: " + to_string(this->points));
    textPoint.setFont(*this->asset.getFont());
    textPoint.setFillColor(Color::White);
    textPoint.setCharacterSize(16);
    textPoint.setOutlineColor(Color(124, 153, 159, 255));
    textPoint.setOutlineThickness(3);
    textPoint.setLetterSpacing(1.32);
    textPoint.setPosition(bgBoatPos.x + targetBgBoat + (16 * bgBoatScale),
                          bgBoatPos.y + (8 * bgBoatScale) + 24);

    window.draw(textPoint);

    // todo: draw life
    RectangleShape rectLifeBg{}, rectLife{};
    rectLifeBg.setFillColor(Color(0, 0, 0, 100));
    rectLifeBg.setSize(Vector2f(256, 12));
    rectLifeBg.setOutlineThickness(3);
    rectLifeBg.setOutlineColor(Color(124, 153, 159, 255));

    rectLife.setFillColor(Color::Red);
    rectLife.setSize(Vector2f(this->life / this->maxLife * 256 - 6, 12 - 6));

    auto textPointPos = textPoint.getPosition();
    rectLifeBg.setPosition(textPointPos.x, textPointPos.y + 32);

    auto rectLifeBgPos = rectLifeBg.getPosition();
    rectLife.setPosition(rectLifeBgPos.x + 3, rectLifeBgPos.y + 3);

    window.draw(rectLifeBg);
    window.draw(rectLife);

    if (this->life <= 0) {
      this->life = 0;
      this->isGameOver = true;
    }
  }

  void initialize() {
    cout << "initialize called" << endl;

    this->playerBoat.getVectorFire()->clear();
    this->vectorEnemyBoat.clear();

    this->isInitialized = true;

    this->view = this->window->getDefaultView();
    this->view.setCenter(0, 0);

    auto tileTargetSize = this->tilemapWar.getTileTargetSize();
    this->view.move(this->tilemapWar.getWidth() * tileTargetSize / 2,
                    this->tilemapWar.getHeight() * tileTargetSize / 2);

    this->window->setView(this->view);

    this->playerBoat.getSprite()->setPosition(
        25 * tileTargetSize - this->playerBoat.getBoatTargetSize() / 2,
        this->tilemapWar.getHeight() * tileTargetSize / 2 -
            this->playerBoat.getBoatTargetSize() / 2);

    // todo: load data
    this->boatLevel =
        (*this->data->getJsonProfile())["selectedBoat"].get<int>();
    this->damage =
        (*this->data->getJsonBoats())["boats"][this->boatLevel - 1]["values"][0]
            .get<float>();
    this->boatVelocity =
        (*this->data->getJsonBoats())["boats"][this->boatLevel - 1]["values"][1]
            .get<float>();
    this->bulletVelocity =
        (*this->data->getJsonBoats())["boats"][this->boatLevel - 1]["values"][2]
            .get<float>();
    this->life =
        (*this->data->getJsonBoats())["boats"][this->boatLevel - 1]["values"][3]
            .get<float>();
    this->maxLife = this->life;

    this->playerBoat.setLevel(this->boatLevel);
    this->playerBoat.setVelocity(this->boatVelocity);
    this->playerBoat.setBulletVelocity(this->bulletVelocity);
  }

 public:
  StateWar(string *state, RenderWindow *window, Data *data) {
    this->state = state;
    this->window = window;
    this->data = data;
  }

  void handleEvent(Event &event) {
    if (event.type == Event::Resized) {
      this->view.setSize(event.size.width, event.size.height);
      this->window->setView(this->view);
    } else if (event.type == Event::KeyReleased) {
      auto code = event.key.code;
      if (code == Keyboard::Space) {
        this->playerBoat.unfire();
      }
    } else if (event.type == Event::KeyPressed) {
      auto code = event.key.code;
      if (code == Keyboard::Enter) {
        if (this->isGameOver) {
          // todo: save points
          int currentPoint =
              (*this->data->getJsonProfile())["points"].get<int>();
          (*this->data->getJsonProfile())["points"] =
              (currentPoint + this->points);
          this->data->save();
          this->data->load();

          // todo: reset state
          *this->state = "world";
          this->isGameOver = false;
          this->isInitialized = false;
          this->points = 0;
        }
      }
      if (code == Keyboard::Escape) {
        // if (this->isGameOver) {
        //   *this->state = "world";
        //   this->isInitialized = false;
        // }
      }
    }
  }

  void run(RenderWindow &window) {
    if (!this->isInitialized) this->initialize();

    // todo: spawn enemy boat
    if (!this->isGameOver) {
      this->spawnEnemyBoatInterval =
          this->clockEnemyBoat.getElapsedTime().asMilliseconds();
      if (this->spawnEnemyBoatInterval >= this->spawnEnemyBoatDelay) {
        if (this->vectorEnemyBoat.size() <= 3) {
          EnemyBoat newEnemyBoat{&this->asset, &this->tilemapWar,
                                 &this->playerBoat};
          int randomPos[5][2] = {
              {6, 29}, {20, 53}, {50, 53}, {73, 35}, {40, 6},
          };
          int randomPosIndex = rand() % 5;
          newEnemyBoat.setInitPos(randomPos[randomPosIndex][0],
                                  randomPos[randomPosIndex][1]);
          this->vectorEnemyBoat.push_back(newEnemyBoat);
        }

        this->clockEnemyBoat.restart();
      }
    }

    this->playerRealPos = window.mapCoordsToPixel(
        this->playerBoat.getColliderBox()->getPosition());
    this->window->setView(this->view);

    this->handleKeyboard();

    this->tilemapWar.draw(window);

    if (!this->isGameOver) this->playerBoat.draw(window);

    // todo: draw enemy
    if (!this->isGameOver)
      for (int a = 0; a < this->vectorEnemyBoat.size(); a++) {
        EnemyBoat *enemyBoat = &this->vectorEnemyBoat.at(a);
        enemyBoat->draw(window);

        // todo: check collision
        for (int b = 0; b < this->playerBoat.getVectorFire()->size(); b++) {
          Fire *fire = &this->playerBoat.getVectorFire()->at(b);
          if (enemyBoat->getColliderFire()->getGlobalBounds().intersects(
                  fire->getFireCollider()->getGlobalBounds())) {
            this->playerBoat.getVectorFire()->erase(
                this->playerBoat.getVectorFire()->begin() + b);
            enemyBoat->decreaseHp(this->damage);
          }
        }

        // todo: check hp
        if (enemyBoat->getHp() <= 0) {
          this->vectorEnemyBoat.erase(this->vectorEnemyBoat.begin() + a);
          this->points += 5;
        }

        // todo: check collision enemy's fire and player
        for (int b = 0; b < enemyBoat->getVectorFire()->size(); b++) {
          Fire *fire = &enemyBoat->getVectorFire()->at(b);
          if (this->playerBoat.getColliderBoxFire()
                  ->getGlobalBounds()
                  .intersects(fire->getFireCollider()->getGlobalBounds())) {
            enemyBoat->getVectorFire()->erase(
                enemyBoat->getVectorFire()->begin() + b);
            if (!this->isGameOver) this->life -= this->damage / 2;
          }
        }
      }

    // todo: reset view
    this->window->setView(this->window->getDefaultView());

    this->drawGui(window);

    // todo: set view
    this->window->setView(this->view);
  }
};

#endif