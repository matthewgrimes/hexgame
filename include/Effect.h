#ifndef EFFECT_H
#define EFFECT_H

#include <vector>
class Character;

class Effect {
public:
  /** Default constructor */
  Effect();
  /** Default destructor */
  virtual ~Effect();

  virtual void setBaseHealthEffectAmount(int value) = 0;
  virtual int getBaseHealthEffectAmount() = 0;

  virtual int calculateHealthEffectTotal(Character *source,
                                         Character *target) = 0;

protected:
  int baseHealthEffectAmount;
  int areaOfEffect;

private:
};

#endif // EFFECT_H
