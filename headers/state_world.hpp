#ifndef state_world_hpp
#define state_world_hpp

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "asset.hpp"
#include "player.hpp"
#include "tilemap_world.hpp"

using namespace std;
using namespace sf;

class StateWorld {
 private:
  string *state;
  View view, viewGui;
  RenderWindow *window;

  Asset asset{"world"};
  Player player{&asset};
  Tilemap tilemap{&asset};

  Vector2i playerRealPos;

  float colliderMapSize = 48;

  bool isInWarPoint = false;
  bool isInShopPoint = false;
  bool isPaused = false;
  bool isShopOpenned = false;

  void handleKeyboard() {
    bool keyUp = Keyboard::isKeyPressed(Keyboard::Up);
    bool keyDown = Keyboard::isKeyPressed(Keyboard::Down);
    bool keyRight = Keyboard::isKeyPressed(Keyboard::Right);
    bool keyLeft = Keyboard::isKeyPressed(Keyboard::Left);

    auto mapMaxY = this->window->getSize().y - 48 - this->colliderMapSize;
    auto mapMaxX = this->window->getSize().x - 48 - this->colliderMapSize;
    auto mapMin = this->colliderMapSize;

    // todo: handle two direction move
    if (keyDown && keyLeft) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, "d");
      auto canMoveLeft = this->tilemap.canMove(this->player, "l");

      if (canMoveDown && canMoveLeft)
        // this->view.move(-speed, speed);
        this->player.moveDownLeft();
      else if (canMoveDown)
        // this->view.move(0, speed);
        this->player.moveDown();
      else if (canMoveLeft)
        // this->view.move(-speed, 0);
        this->player.moveLeft();

    } else if (keyDown && keyRight) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveDown = this->tilemap.canMove(this->player, "d");
      auto canMoveRight = this->tilemap.canMove(this->player, "r");

      if (canMoveDown && canMoveRight)
        // this->view.move(speed, speed);
        this->player.moveDownRight();
      else if (canMoveDown)
        // this->view.move(0, speed);
        this->player.moveDown();
      else if (canMoveRight)
        // this->view.move(speed, 0);
        this->player.moveRight();

    } else if (keyUp && keyLeft) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, "u");
      auto canMoveLeft = this->tilemap.canMove(this->player, "l");

      if (canMoveUp && canMoveLeft)
        // this->view.move(-speed, -speed);
        this->player.moveUpLeft();
      else if (canMoveUp)
        // this->view.move(0, -speed);
        this->player.moveUp();
      else if (canMoveLeft)
        // this->view.move(-speed, 0);
        this->player.moveLeft();

    } else if (keyUp && keyRight) {
      auto speed = sqrt(2) * this->player.getVelocity() / 2;
      auto canMoveUp = this->tilemap.canMove(this->player, "u");
      auto canMoveRight = this->tilemap.canMove(this->player, "r");

      if (canMoveUp && canMoveRight)
        // this->view.move(speed, -speed);
        this->player.moveUpRight();
      else if (canMoveUp)
        // this->view.move(0, -speed);
        this->player.moveUp();
      else if (canMoveRight)
        // this->view.move(speed, 0);
        this->player.moveRight();

    }

    // todo: handle one direction move
    else if (keyDown) {
      if (this->tilemap.canMove(player, "d")) {
        if (this->playerRealPos.y >= mapMaxY)
          this->view.move(0, this->player.getVelocity());
        this->player.moveDown();
      }
    } else if (keyUp) {
      if (this->tilemap.canMove(player, "u")) {
        if (this->playerRealPos.y <= mapMin)
          this->view.move(0, -this->player.getVelocity());
        this->player.moveUp();
      }
    } else if (keyLeft) {
      if (this->tilemap.canMove(player, "l")) {
        if (this->playerRealPos.x <= mapMin)
          this->view.move(-this->player.getVelocity(), 0);
        this->player.moveLeft();
      }
    } else if (keyRight) {
      if (this->tilemap.canMove(player, "r")) {
        if (this->playerRealPos.x >= mapMaxX)
          this->view.move(this->player.getVelocity(), 0);
        this->player.moveRight();
      }
    }
  }

  void drawGui(RenderWindow &window) {
    RectangleShape rect{};
    rect.setFillColor(Color::Red);
    rect.setSize(Vector2f(100, 100));
    window.draw(rect);

    if (this->isPaused) this->drawPauseMenu(window);
    if (!this->isPaused) {
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
    int prices[] = {0, 300, 600, 1200};
    for (int a = 0; a < 4; a++) {
      Sprite bgBoat{};
      bgBoat.setTexture(this->asset.getVectorSquareButtons()->at(0));
      bgBoat.setTextureRect(IntRect(8, 8, 32, 32));
      bgBoat.setPosition(bgPos.x + (a * bgBoatSize) + 32, titlePos.y + 48);
      bgBoat.setScale(bgBoatScaleFactor, bgBoatScaleFactor);
      window.draw(bgBoat);

      auto bgBoatPos = bgBoat.getPosition();
      auto boatScaleFactor = (bgBoatSize - 32) / 128;

      Sprite boat{};
      boat.setTexture(this->asset.getVectorBoatColor1()->at(a));
      boat.setScale(boatScaleFactor, boatScaleFactor);
      auto boatBounds = boat.getGlobalBounds();
      boat.setPosition(bgBoatPos.x + (bgBoatSize - boatBounds.width) / 2,
                       bgBoatPos.y + (bgBoatSize - boatBounds.height) / 2);
      if (a > 0) boat.setColor(Color::Black);
      window.draw(boat);

      // todo: draw price
      auto boatPos = boat.getPosition();
      Text price{};
      price.setFont(*this->asset.getFont());
      price.setString("$" + to_string(prices[a]));
      price.setCharacterSize(12);

      auto priceBounds = price.getGlobalBounds();
      price.setPosition(boatPos.x + (bgBoatSize - priceBounds.width - 32) / 2,
                        boatPos.y + bgBoatSize);

      window.draw(price);
    }
  }

  void drawPauseMenu(RenderWindow &window) {
    auto wCenter = window.getView().getCenter();

    RectangleShape bgDark{};
    bgDark.setFillColor(Color(0, 0, 0, 100));
    bgDark.setSize(Vector2f(window.getSize().x, window.getSize().y));
    window.draw(bgDark);

    Sprite background{};
    background.setTexture(*this->asset.getTextureBackgroundMenu());
    background.setScale(720 / 128, window.getSize().y / 144);

    auto bgBounds = background.getGlobalBounds();
    background.setPosition(wCenter.x - (bgBounds.width / 2),
                           wCenter.y - (bgBounds.height / 2));
    window.draw(background);

    auto bgPos = background.getPosition();

    Text title{};
    title.setFont(*this->asset.getFont());
    title.setString("Paused!");
    title.setFillColor(Color::White);

    auto titleBounds = title.getGlobalBounds();
    title.setPosition(bgPos.x + (bgBounds.width - titleBounds.width) / 2,
                      bgPos.y + 3 * 8 * window.getSize().y / 144);

    window.draw(title);
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
  StateWorld(string *state, RenderWindow *window) {
    this->state = state;
    this->window = window;

    view = window->getDefaultView();
    view.setCenter(0, 0);
    this->viewGui = window->getDefaultView();
    this->viewGui.setCenter(window->getSize().x / 2, window->getSize().y / 2);

    auto tileTargetSize = this->tilemap.getTileTargetSize();
    auto width = this->tilemap.getWidth();
    auto height = this->tilemap.getHeight();

    this->player.setPosition(15 * this->tilemap.getTileTargetSize(),
                             18 * this->tilemap.getTileTargetSize());
    this->view.move(
        this->tilemap.getWidth() / 2 * this->tilemap.getTileTargetSize(),
        this->tilemap.getHeight() / 1.5 * this->tilemap.getTileTargetSize());
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
          *this->state = "war";
        }
        if (this->isInShopPoint) {
          this->isShopOpenned = true;
        }
      } else if (code == Keyboard::Escape) {
        if (this->isShopOpenned) {
          this->isShopOpenned = false;
        } else {
          if (this->isPaused) {
            this->isPaused = false;
          } else {
            this->isPaused = true;
          }
        }
      }
    }
  }

  void run(RenderWindow &window) {
    if (!this->isPaused && !this->isShopOpenned) {
      this->handleKeyboard();
    }

    window.setView(view);

    this->playerRealPos = window.mapCoordsToPixel(
        this->player.getRectColliderBox()->getPosition());

    RectangleShape rect{};
    rect.setOutlineColor(Color::Red);
    rect.setOutlineThickness(2);

    auto wSize = window.getSize();
    rect.setSize(Vector2f(tilemap.getWidth() * 16, tilemap.getHeight() * 16));

    window.draw(rect);

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