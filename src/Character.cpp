#include "Character.h"
#include "HexMap.h"
#include "HexUtils.h"
#include "Pipeline.h"
#include "ServiceLocator.h"
#include <cmath>
#include <fstream>

void CharacterFileLoader::parseLine(const std::string &field,
                                    const std::string &data) {
  if (field.compare("name") == 0) {
    character->name = ParseString(data);
    ServiceLocator::getTextLog().addText("Character name: " + character->name);
  } else if (field.compare("maxhp") == 0) {
    character->stats.maxHitPoints = ParseInt(data);
    character->stats.hitPoints = character->stats.maxHitPoints;
  } else if (field.compare("speed") == 0) {
    character->stats.speed = ParseInt(data);
  } else if (field.compare("agility") == 0) {
    character->stats.agility = ParseInt(data);
  } else if (field.compare("model") == 0) {
    std::size_t spacemarker = data.find_first_of(" ");
    character->LoadMesh(data.substr(0, spacemarker).c_str());
    character->m_Portrait.LoadMesh("data/meshes/actorportrait.obj");
    character->m_PortraitHealthBar.LoadMesh(
        "data/meshes/actorportraithealthbar.obj");
    character->m_PortraitHealthBar.setTexture("data/tiletextures/green.png");
    if (spacemarker != std::string::npos) {
      character->setTexture(data.substr(spacemarker + 1).c_str());
      character->m_Portrait.setTexture(data.substr(spacemarker + 1).c_str());
    }
  } else if (field.compare("normalmap") == 0) {
    character->setNormalMap(data);
    printf("Setting Normal map\n");
  } else if (field.compare("weapon") == 0) {
    Weapon *newWeapon = new Weapon;
    newWeapon->loadFromFile(data);
    character->setWeapon(newWeapon);
    character->addItem(newWeapon);
  } else if (field == "technique") {
    character->techniques.push_back(new Technique);
    character->techniques.back()->loadFromFile(data);
  } else if (field == "AI") {
    character->setAI(true);
  } else if (field == "portrait") {
  }
}

Character::Character() {
  name = std::string("");

  moved = false;
  attacked = false;
  AI = false;

  Inventory.clear();
}

Character::~Character() {
  movementData.clear();
  for (auto item : Inventory) {
    delete item;
  }
  Inventory.clear();
  for (auto tech : techniques) {
    delete tech;
  }
  techniques.clear();
}

void Character::newTurn() {
  moved = false;
  attacked = false;
}

bool Character::loadFromFile(const std::string &fileName) {
  CharacterFileLoader loader(fileName);
  loader.setCharacter(this);
  loader.parseFile();
  return 1;
}

void Character::setMovementData(std::map<std::pair<int, int>, int> value) {
  movementData = value;
}
//! given an input position, extract a movement vector from movementData
void Character::executeMove(int i, int j) {
  std::pair<int, int> current = std::make_pair(i, j);
  moveList.push_back((Directions)movementData[current]);
  while (current != std::make_pair(Position.getI(), Position.getJ())) {
    Directions direction = (Directions)((movementData[current] + 3) % 6);
    current =
        std::make_pair(NeighborI(current.first, current.second, direction),
                       NeighborJ(current.first, current.second, direction));
    moveList.push_back((Directions)movementData[current]);

    incrementTurnCountdown(DELAYPERTILE / (float)(getSpeed()));
  }
  moveList.pop_back();
}

void Character::decreaseHitPoints(int value) {
  stats.hitPoints = std::max(stats.hitPoints - value, 0);
}

void Character::increaseHitPoints(int value) {
  stats.hitPoints = std::min(stats.hitPoints + value, stats.maxHitPoints);
}

void Character::addItem(Item *value) { Inventory.push_back(value); }

void Character::setWeapon(Weapon *value) { currentWeapon = value; }

Weapon *Character::getCurrentWeapon() { return currentWeapon; }

void Character::performAttack(Character *target) {
  std::string outputString;
  outputString = getName() + std::string(" is attacking ") + target->getName() +
                 std::string(" with ") + getCurrentAttack()->getName() +
                 std::string(":");
  ServiceLocator::getTextLog().addText(outputString);

  getCurrentAttack()->perform(this, target);

  std::string output = target->getName();
  output += std::string(" now has ");
  output += std::to_string(target->getHitPoints());
  output += std::string("/");
  output += std::to_string(target->getMaxHitPoints());
  output += std::string(" HP.");

  ServiceLocator::getTextLog().addText(output);
}

int Character::simulateAttack(Character *target) {
  return getCurrentAttack()->calculateDamage(this, target);
}

void Character::printStatus() {
  std::string output;

  ServiceLocator::getTextLog().addText(getName(), textLogColor(0.f, 1.f, 1.f));

  output = "HP: " + std::to_string(getHitPoints()) + "/" +
           std::to_string(getMaxHitPoints());
  ServiceLocator::getTextLog().addText(output);

  output = "Countdown: " + std::to_string(getTurnCountdown());
  ServiceLocator::getTextLog().addText(output);

  output = "Equipped Weapon: " + getCurrentWeapon()->getName();
  ServiceLocator::getTextLog().addText(output);

  output = "Range: " + std::to_string(getCurrentWeapon()->getMinRange()) + "-" +
           std::to_string(getCurrentWeapon()->getMaxRange());
  ServiceLocator::getTextLog().addText(output);

  output = "Damage: " + std::to_string(getCurrentWeapon()->getDamage());
  ServiceLocator::getTextLog().addText(output);

  for (auto tech : techniques) {
    output = "Technique: " + tech->getName();
    ServiceLocator::getTextLog().addText(output);
    output = " Damage: " + std::to_string(tech->getDamage());
    output += " AOE: " + std::to_string(tech->getAOE());
    ServiceLocator::getTextLog().addText(output);
  }
}

void Character::prepareWeaponAttack() {
  currentAttack.setMinRange(currentWeapon->getMinRange());
  currentAttack.setMaxRange(currentWeapon->getMaxRange());
  currentAttack.setDamage(currentWeapon->getDamageEffect());
  currentAttack.setName(currentWeapon->getName());
};

void Character::prepareTechniqueAttack() {
  if (!currentTechnique) {
    currentTechnique = techniques.back();
  }
  currentAttack.setMinRange(0);
  currentAttack.setMaxRange(currentTechnique->getRange());
  currentAttack.setDamage(currentTechnique->getDamageEffect());
  currentAttack.setName(currentTechnique->getName());
};

void Character::Render() { Actor::Render(); }

bool Character::isAlive() const { return stats.hitPoints > 0; }

void Character::RenderPortrait() {
  Vector3f objectPos = Pipeline::getInstance()->getObjectPos();
  Vector3f newPos = objectPos;

  Pipeline::getInstance()->setObjectPos(Vector3f(objectPos.x, 0.f, 0.f));
  Pipeline::getInstance()->setObjectScale(
      Vector3f(1, 1, getHitPointsPercent()));
  Pipeline::getInstance()->setObjectPos(
      Vector3f(objectPos.x, 0.f, objectPos.z - (1 - getHitPointsPercent())));
  m_PortraitHealthBar.Render();

  Pipeline::getInstance()->setObjectScale(Vector3f(1, 1, 1));
  Pipeline::getInstance()->setObjectPos(objectPos);
  Pipeline::getInstance()->Scale(1, 1, 1);
  m_Portrait.Render();
}
