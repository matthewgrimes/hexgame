#ifndef DAMAGEEFFECT_H
#define DAMAGEEFFECT_H

#include "Effect.h"

enum BackstabBonus { BS_NONE, BS_BOOLEAN, GRADIENT };
enum HeightBonus { H_NONE, TEN, TWENTY };

class damageEffect : public Effect {
public:
  /** Default constructor */
  damageEffect();
  /** Default destructor */
  virtual ~damageEffect();

  virtual void setBaseHealthEffectAmount(int value);
  virtual int getBaseHealthEffectAmount();

  void setAOE(int value);
  int getAOE();

  BackstabBonus getBackstabBonus();
  void setBackstabBonus(BackstabBonus bonus);

  HeightBonus getHeightBonus();
  void setHeightBonus(HeightBonus bonus);

  virtual int calculateHealthEffectTotal(Character *source, Character *target);
  float calculateBackstabBonus(Character *source, Character *target);
  float calculateHeightBonus(Character *source, Character *target);

protected:
private:
  BackstabBonus backstabBonus;
  HeightBonus heightBonus;
};

#endif // DAMAGEEFFECT_H
