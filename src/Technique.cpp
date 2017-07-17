#include "Technique.h"
#include "HexUtils.h"

void TechniqueFileLoader::parseLine(const std::string &field,
                                    const std::string &data) {
  if (field == "name") {
    technique->Name = ParseString(data);
  }
  if (field == "range") {
    technique->range = ParseInt(data);
  }
  if (field == "damage") {
    ParseDamageEffect(data, &technique->effect);
    technique->effect.setBackstabBonus(BS_NONE);
    technique->effect.setHeightBonus(H_NONE);
  }
}

Technique::Technique() {
  // ctor
}

Technique::~Technique() {
  // dtor
}

void Technique::loadFromFile(const std::string &fileName) {
  TechniqueFileLoader loader(fileName);
  loader.setTechnique(this);
  loader.parseFile();
}
