#include "Weapon.h"
#include "Character.h"

void WeaponFileLoader::parseLine(const std::string &field,
                                 const std::string &data) {
  ItemFileLoader::parseLine(field, data);

  if (field == "minrange") {
    weapon->minRange = ParseInt(data);
  }
  if (field == "maxrange") {
    weapon->maxRange = ParseInt(data);
  }
  if (field == "damage") {
    ParseDamageEffect(data, &weapon->damage);
  }
  if (field == "type") {
    if (data == "melee") {
      weapon->type = MELEE;
      weapon->damage.setBackstabBonus(GRADIENT);
      weapon->damage.setHeightBonus(TEN);
    } else if (data == "ranged") {
      weapon->type = RANGED;
      weapon->damage.setBackstabBonus(BS_BOOLEAN);
      weapon->damage.setHeightBonus(TWENTY);
    }
  }
}

bool Weapon::loadFromFile(const std::string &fileName) {
  WeaponFileLoader loader(fileName);
  loader.setWeapon(this);
  loader.parseFile();

  return 1;
}

int Weapon::getMinRange() { return minRange; }

int Weapon::getMaxRange() { return maxRange; }

int Weapon::getDamage() { return damage.getBaseHealthEffectAmount(); }

int Weapon::getAOE() { return damage.getAOE(); }

weaponTypes Weapon::getType() { return type; }
