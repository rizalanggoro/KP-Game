#ifndef state_world_hpp
#define state_world_hpp

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>

#include "asset.hpp"
#include "data.hpp"
#include "direction.hpp"
#include "json.hpp"
#include "player.hpp"
#include "tilemap_world.hpp"

using json = nlohmann::json;
using namespace std;
using namespace sf;

class StateWorld {
 private:
  string *state;
  Data *data;
  View view, viewGui;
  RenderWindow *window;

  Asset asset{"world"};
  Player player{&asset};
  Tilemap tilemap{&asset};

  Vector2i playerRealPos;
  Vector2i mouseRealPos;

  float colliderMapSize = 48;

  bool isInWarPoint = false;
  bool isInShopPoint = false;
  bool isConfirmExitShowed = false;
  bool isShopOpenned = false;

  Sprite buttonClose{}, buttonAction{};
  Sprite buttonBoat1{}, buttonBoat2{}, buttonBoat3{}, buttonBoat4{};
  int shopSelectedBoatIndex = 0;

  Clock clockFramerate{};
  float multiplier = 1;

  Sound backsoundWorld{}, soundClick{};

  void moveMap(Direction direction) {
    float mapMaxY = this->window->getSize().y - 48 - this->colliderMapSize;
    float mapMaxX = this->window->getSize().x - 48 - this->colliderMapSize;
    float mapMin = this->colliderMapSize;

    float px = this->playerRealPos.x;
    float py = this->playerRealPos.y;
    float pv = this->player.getVelocity() * this->multiplier;
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

    // todo: handle two direction move
    if (keyDown && keyLeft) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, "d");
      auto canMoveLeft = this->tilemap.canMove(this->player, "l");

      if (canMoveDown && canMoveLeft) {
        this->moveMap(DIR_DOWN_LEFT);
        this->player.moveDownLeft();
      } else if (canMoveDown) {
        this->moveMap(DIR_DOWN);
        this->player.moveDown();
      } else if (canMoveLeft) {
        this->moveMap(DIR_LEFT);
        this->player.moveLeft();
      }

    } else if (keyDown && keyRight) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, "d");
      auto canMoveRight = this->tilemap.canMove(this->player, "r");

      if (canMoveDown && canMoveRight) {
        this->moveMap(DIR_DOWN_RIGHT);
        this->player.moveDownRight();
      } else if (canMoveDown) {
        this->moveMap(DIR_DOWN);
        this->player.moveDown();
      } else if (canMoveRight) {
        this->moveMap(DIR_RIGHT);
        this->player.moveRight();
      }

    } else if (keyUp && keyLeft) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, "u");
      auto canMoveLeft = this->tilemap.canMove(this->player, "l");

      if (canMoveUp && canMoveLeft) {
        this->moveMap(DIR_UP_LEFT);
        this->player.moveUpLeft();
      } else if (canMoveUp) {
        this->moveMap(DIR_UP);
        this->player.moveUp();
      } else if (canMoveLeft) {
        this->moveMap(DIR_LEFT);
        this->player.moveLeft();
      }

    } else if (keyUp && keyRight) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, "u");
      auto canMoveRight = this->tilemap.canMove(this->player, "r");

      if (canMoveUp && canMoveRight) {
        this->moveMap(DIR_UP_RIGHT);
        this->player.moveUpRight();
      } else if (canMoveUp) {
        this->moveMap(DIR_UP);
        this->player.moveUp();
      } else if (canMoveRight) {
        this->moveMap(DIR_RIGHT);
        this->player.moveRight();
      }

    }

    // todo: handle one direction move
    else if (keyDown) {
      if (this->tilemap.canMove(player, "d")) {
        this->moveMap(DIR_DOWN);
        this->player.moveDown();
      }
    } else if (keyUp) {
      if (this->tilemap.canMove(player, "u")) {
        this->moveMap(DIR_UP);
        this->player.moveUp();
      }
    } else if (keyLeft) {
      if (this->tilemap.canMove(player, "l")) {
        this->moveMap(DIR_LEFT);
        this->player.moveLeft();
      }
    } else if (keyRight) {
      if (this->tilemap.canMove(player, "r")) {
        this->moveMap(DIR_RIGHT);
        this->player.moveRight();
      }
    }
  }

  void drawGui(RenderWindow &window) {
    this->drawGuiPoints(window);

    if (this->isConfirmExitShowed) this->drawConfirmExit(window);
    if (!this->isConfirmExitShowed) {
      if (this->isInWarPoint) {
        this->drawInstruction(window,
                              "Press enter to go to the battlefield "
                              "and\nplunder the enemy's treasure!");
      }
      if (this->isInShopPoint && !this->isShopOpenned) {
        this->drawInstruction(
            window, "Wanna upgrade something?\nEnter for upgrade something.");
      }
      if (this->isShopOpenned) {
        this->drawShop(window);
      }
    }
  }

  void drawGuiPoints(RenderWindow &window) {
    Sprite coin{};
    coin.setTexture(*this->asset.getTextureCoin());
    coin.setScale(40 / 12, 40 / 12);
    coin.setPosition(16, 16);
    window.draw(coin);

    auto coinPos = coin.getPosition();
    auto coinBounds = coin.getGlobalBounds();

    Text textPoint{};
    textPoint.setString(
        to_string((*this->data->getJsonProfile())["points"].get<int>()));
    textPoint.setCharacterSize(20);
    textPoint.setFont(*this->asset.getFont());
    textPoint.setFillColor(Color::White);
    textPoint.setOutlineColor(Color(0, 0, 0, 200));
    textPoint.setOutlineThickness(2);
    textPoint.setLetterSpacing(1.32);

    auto textPointBounds = textPoint.getGlobalBounds();
    textPoint.setPosition(
        coinPos.x + coinBounds.width + 16,
        coinPos.y + (coinBounds.height - textPointBounds.height) / 2);
    window.draw(textPoint);
  }

  void drawShop(RenderWindow &window) {
    RectangleShape bgDark{};
    bgDark.setFillColor(Color(0, 0, 0, 100));
    bgDark.setSize(Vector2f(window.getSize().x, window.getSize().y));
    window.draw(bgDark);

    auto wCenter = window.getView().getCenter();
    float scaleFactor = (720 - 96) / 122;

    Sprite background{};
    background.setTexture(*this->asset.getTextureBackgroundMenu());
    background.setScale(scaleFactor, scaleFactor);
    auto bgSize = background.getGlobalBounds();
    background.setPosition(wCenter.x - bgSize.width / 2,
                           wCenter.y - bgSize.height / 2);
    window.draw(background);

    auto bgPos = background.getPosition();

    // todo: draw button close
    buttonClose.setTexture(*this->asset.getTextureButtonClose());
    buttonClose.setPosition(bgPos.x + bgSize.width - 48, bgPos.y - 16);
    buttonClose.setScale(64 / 20, 64 / 20);
    window.draw(buttonClose);

    // todo: draw title
    Text title{};
    title.setString("UPGRADE");
    title.setFont(*this->asset.getFont());
    title.setOutlineColor(Color(124, 153, 159, 255));
    title.setOutlineThickness(3);
    title.setLetterSpacing(1.32);
    title.setCharacterSize(24);

    auto titleBounds = title.getGlobalBounds();

    title.setPosition(bgPos.x + (bgSize.width - titleBounds.width) / 2,
                      bgPos.y + 48);
    window.draw(title);

    auto titlePos = title.getPosition();

    // todo: draw boat list
    auto bgBoatSize = (bgSize.width - 64) / 4;
    float bgBoatScaleFactor = bgBoatSize / 32;
    Vector2f _bgBoatPos;
    for (int a = 0; a < 4; a++) {
      Sprite *buttonBoat;
      if (a == 0) buttonBoat = &this->buttonBoat1;
      if (a == 1) buttonBoat = &this->buttonBoat2;
      if (a == 2) buttonBoat = &this->buttonBoat3;
      if (a == 3) buttonBoat = &this->buttonBoat4;

      buttonBoat->setTexture(this->asset.getVectorSquareButtons()->at(
          this->shopSelectedBoatIndex == a ? 0 : 4));
      buttonBoat->setTextureRect(IntRect(8, 8, 32, 32));
      buttonBoat->setPosition(bgPos.x + (a * bgBoatSize) + 32, titlePos.y + 48);
      buttonBoat->setScale(bgBoatScaleFactor, bgBoatScaleFactor);
      window.draw(*buttonBoat);

      auto bgBoatPos = buttonBoat->getPosition();
      auto boatScaleFactor = (bgBoatSize - 32) / 128;

      Sprite boat{};
      boat.setTexture(this->asset.getVectorBoats()->at(a).at(0));
      boat.setScale(boatScaleFactor, boatScaleFactor);
      auto boatBounds = boat.getGlobalBounds();
      boat.setPosition(bgBoatPos.x + (bgBoatSize - boatBounds.width) / 2,
                       bgBoatPos.y + (bgBoatSize - boatBounds.height) / 2);

      bool isOwned =
          (*this->data->getJsonBoats())["boats"][a]["owned"].get<bool>();
      if (!isOwned) boat.setColor(Color::Black);
      window.draw(boat);

      // todo: draw price
      auto boatPos = boat.getPosition();
      auto jsonBoats = (*this->data->getJsonBoats())["boats"];

      Text price{};
      price.setFont(*this->asset.getFont());
      if (!isOwned)
        price.setString("$" + to_string(jsonBoats[a]["price"].get<int>()));
      else {
        if ((*this->data->getJsonProfile())["selectedBoat"] == (a + 1))
          price.setString("Used");
      }
      price.setCharacterSize(16);

      auto priceBounds = price.getGlobalBounds();
      price.setPosition(boatPos.x + (bgBoatSize - priceBounds.width - 32) / 2,
                        boatPos.y + bgBoatSize);

      window.draw(price);

      if (a == 3) _bgBoatPos = buttonBoat->getPosition();
    }

    // todo: draw items
    string items[] = {"Damage", "Boat Speed", "Bullet Speed", "Life"};
    Vector2f lastItemPos;
    for (int a = 0; a < 4; a++) {
      Text text{};
      text.setFont(*this->asset.getFont());
      text.setString(items[a]);
      text.setCharacterSize(16);
      text.setFillColor(Color::White);
      text.setPosition(bgPos.x + 48,
                       _bgBoatPos.y + bgBoatSize + (a * (32 + 40)) + 64);

      auto textPos = text.getPosition();
      auto textBounds = text.getGlobalBounds();

      //  todo: draw star / level
      for (int b = 0;
           b <
           (*this->data->getJsonBoats())["boats"][this->shopSelectedBoatIndex]
                                        ["levels"][a]
                                            .get<int>();
           b++) {
        float targetStarSize = 32;
        Sprite star{};
        star.setTexture(*this->asset.getTextureStar());
        star.setPosition(textPos.x + (b * 40),
                         textPos.y + textBounds.height + 8);
        star.setScale(targetStarSize / 14, targetStarSize / 14);
        window.draw(star);
      }

      if (a == 3) lastItemPos = text.getPosition();

      window.draw(text);
    }

    // todo: draw button actions
    float buttonActionTargetHeight = 72;
    this->buttonAction.setTexture(*this->asset.getTextureBigButton());
    this->buttonAction.setScale(buttonActionTargetHeight / 27,
                                buttonActionTargetHeight / 27);

    auto buttonActionBounds = this->buttonAction.getGlobalBounds();
    this->buttonAction.setPosition(
        bgPos.x + (bgSize.width - buttonActionBounds.width) / 2,
        bgPos.y + bgSize.height - (buttonActionTargetHeight * 3 / 5));

    window.draw(this->buttonAction);

    // todo: draw button action text
    auto bgActionPos = this->buttonAction.getPosition();
    Text textButtonAction{};
    auto textValue =
        (*this->data
              ->getJsonBoats())["boats"][this->shopSelectedBoatIndex]["owned"]
            ? "USE"
            : "BUY";
    textButtonAction.setString(textValue);
    textButtonAction.setFillColor(Color(182, 137, 98, 255));
    textButtonAction.setFont(*this->asset.getFont());
    textButtonAction.setCharacterSize(20);

    auto textButtonActionBounds = textButtonAction.getGlobalBounds();
    textButtonAction.setPosition(
        bgActionPos.x +
            (buttonActionBounds.width - textButtonActionBounds.width) / 2,
        bgActionPos.y - 4 +
            (buttonActionBounds.height - textButtonActionBounds.height) / 2);
    window.draw(textButtonAction);
  }

  void drawConfirmExit(RenderWindow &window) {
    auto wCenter = window.getView().getCenter();

    RectangleShape bgDark{};
    bgDark.setFillColor(Color(0, 0, 0, 150));
    bgDark.setSize(Vector2f(window.getSize().x, window.getSize().y));
    window.draw(bgDark);

    Text title{};
    title.setFont(*this->asset.getFont());
    title.setString("Do you want to exit the game?");
    title.setCharacterSize(32);
    title.setStyle(Text::Bold);
    title.setFillColor(Color::White);

    auto titleBounds = title.getGlobalBounds();
    title.setPosition(wCenter.x - titleBounds.width / 2,
                      wCenter.y - titleBounds.height / 2);

    window.draw(title);

    Text subtitle{};
    subtitle.setFont(*this->asset.getFont());
    subtitle.setString(">> PRESS ENTER TO EXIT <<");
    subtitle.setCharacterSize(16);
    subtitle.setFillColor(Color::White);

    auto titlePos = title.getPosition();
    auto subtitleBounds = subtitle.getGlobalBounds();
    subtitle.setPosition(wCenter.x - subtitleBounds.width / 2,
                         window.getSize().y - subtitleBounds.height - 48);
    window.draw(subtitle);
  }

  void drawInstruction(RenderWindow &window, string instruction) {
    auto wSize = window.getSize();

    Text text{};
    text.setLineSpacing(2);
    text.setCharacterSize(24);
    text.setString(instruction);
    text.setFont(*this->asset.getFont());
    text.setFillColor(Color::White);
    text.setOutlineColor(Color(124, 153, 159, 255));
    text.setOutlineThickness(3);
    text.setLetterSpacing(1.32);

    auto textBounds = text.getGlobalBounds();
    text.setPosition(48, wSize.y - textBounds.height - 48);

    window.draw(text);
  }

 public:
  StateWorld(string *state, RenderWindow *window, Data *data) {
    this->state = state;
    this->window = window;
    this->data = data;

    this->soundClick.setBuffer(*this->asset.getSoundClick());
    this->backsoundWorld.setBuffer(*this->asset.getBacksoundWorld());
    this->backsoundWorld.setLoop(true);
    this->backsoundWorld.setVolume(40);

    view = window->getDefaultView();
    view.setCenter(0, 0);
    this->viewGui = window->getDefaultView();
    this->viewGui.setCenter(window->getSize().x / 2, window->getSize().y / 2);

    auto tileTargetSize = this->tilemap.getTileTargetSize();
    auto width = this->tilemap.getWidth();
    auto height = this->tilemap.getHeight();

    this->player.setPosition(17 * this->tilemap.getTileTargetSize(),
                             19 * this->tilemap.getTileTargetSize());
    this->view.move(
        this->tilemap.getWidth() / 2 * this->tilemap.getTileTargetSize(),
        this->tilemap.getHeight() / 2 * this->tilemap.getTileTargetSize());
  }

  void handleEvent(Event &event) {
    this->player.handleEvent(event);
    if (event.type == Event::Resized) {
      auto size = event.size;
      this->view.setSize(size.width, size.height);
      this->viewGui.setSize(size.width, size.height);
      this->viewGui.setCenter(size.width / 2, size.height / 2);
    } else if (event.type == Event::KeyPressed) {
      auto code = event.key.code;
      if (code == Keyboard::Enter) {
        if (this->isInWarPoint) {
          this->backsoundWorld.stop();
          *this->state = "war";
        }
        if (this->isInShopPoint) {
          this->isShopOpenned = true;
        }
        if (this->isConfirmExitShowed) window->close();
      } else if (code == Keyboard::Escape) {
        if (this->isShopOpenned) {
          this->isShopOpenned = false;
        } else {
          if (this->isConfirmExitShowed) {
            this->isConfirmExitShowed = false;
          } else {
            this->isConfirmExitShowed = true;
          }
        }
      }
    } else if (event.type == Event::MouseButtonPressed) {
      auto mx = this->mouseRealPos.x;
      auto my = this->mouseRealPos.y;
      if (this->buttonClose.getGlobalBounds().contains(mx, my)) {
        this->isShopOpenned = false;
        this->soundClick.play();
      }

      if (this->buttonBoat1.getGlobalBounds().contains(mx, my)) {
        this->shopSelectedBoatIndex = 0;
        this->soundClick.play();
      }
      if (this->buttonBoat2.getGlobalBounds().contains(mx, my)) {
        this->shopSelectedBoatIndex = 1;
        this->soundClick.play();
      }
      if (this->buttonBoat3.getGlobalBounds().contains(mx, my)) {
        this->shopSelectedBoatIndex = 2;
        this->soundClick.play();
      }
      if (this->buttonBoat4.getGlobalBounds().contains(mx, my)) {
        this->shopSelectedBoatIndex = 3;
        this->soundClick.play();
      }

      if (this->buttonAction.getGlobalBounds().contains(mx, my)) {
        this->soundClick.play();
        cout << "button action clicked" << endl;
        bool isActionUse =
            (*this->data->getJsonBoats())["boats"][this->shopSelectedBoatIndex]
                                         ["owned"]
                                             .get<bool>();

        if (isActionUse) {
          // todo: change boat
          (*this->data->getJsonProfile())["selectedBoat"] =
              (this->shopSelectedBoatIndex + 1);
          this->isShopOpenned = false;
          this->data->save();
          this->data->load();
        } else {
          // todo: buy a new boat
          json *jsonBoatsPtr = this->data->getJsonBoats();
          json *jsonProfilePtr = this->data->getJsonProfile();

          int points = (*jsonProfilePtr)["points"].get<int>();
          int boatPrice =
              (*jsonBoatsPtr)["boats"][this->shopSelectedBoatIndex]["price"]
                  .get<int>();

          if (points >= boatPrice) {
            (*jsonProfilePtr)["points"] = (points - boatPrice);
            (*this->data->getJsonBoats())["boats"][this->shopSelectedBoatIndex]
                                         ["owned"] = true;

            this->data->save();
            this->data->load();
          }
        }
      }
    }
  }

  void run(RenderWindow &window) {
    if (this->backsoundWorld.getStatus() != 2) this->backsoundWorld.play();

    this->multiplier = this->clockFramerate.restart().asSeconds() * 60;
    this->mouseRealPos = Mouse::getPosition(window);

    if (!this->isConfirmExitShowed && !this->isShopOpenned) {
      this->handleKeyboard();
    }

    window.setView(view);

    this->playerRealPos = window.mapCoordsToPixel(
        this->player.getRectColliderBox()->getPosition());

    // todo: draw ground
    tilemap.draw(window);

    // todo: draw sprite player
    this->player.draw(window);

    // todo: draw leaf
    this->tilemap.drawLeaf(window);

    this->window->setView(this->viewGui);
    this->drawGui(window);

    // todo: check player in war point
    this->isInWarPoint =
        this->player.getRectColliderBox()->getGlobalBounds().intersects(
            this->tilemap.getTextWarPoint()->getGlobalBounds());

    // todo; check player in shop point
    this->isInShopPoint =
        this->player.getRectColliderBox()->getGlobalBounds().intersects(
            this->tilemap.getTextShopPoint()->getGlobalBounds());
  }
};

#endif