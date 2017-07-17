#include "Actor.h"
#include "GLOBALS.h"
#include "Pipeline.h"
Actor::Actor() {
  // ctor
  Angle = 0;

  m_MotionTimer.Start();
  m_AnimationTimer.Start();

  moveList.clear();
}

Actor::~Actor() {
  // dtor
}

void Actor::setPos(float x, float y) {
  Position.setX(x);
  Position.setY(y);
}

void Actor::setPosition(int i, int j) {
  Position.setI(i);
  Position.setJ(j);
}

void Actor::Render() {
  std::vector<Matrix4f> Transforms;

  BoneTransform(m_AnimationTimer.getSecondsElapsed(), Transforms);

  for (unsigned int i = 0; i < Transforms.size(); i++) {
    setBoneTransform(i, Transforms[i]);
  }
  Pipeline::getInstance()->setObjectAngle(-Angle);
  Pipeline::getInstance()->setObjectPos(
      Vector3f(Position.getX(), Position.getZ(), Position.getY()));
  AnimatedMesh::RenderWithOutline(1.f, 1.f, 1.f, 10.f);
  Pipeline::getInstance()->setObjectAngle(0.);
}

int Actor::getI() { return Position.getI(); }

int Actor::getJ() { return Position.getJ(); }

void Actor::Update() {

  // setOutlined(false);

  if (!moveList.empty()) {
    if (moveList.back() == NONE) {
      moveList.pop_back();
    }
    /** animation stuff goes here
     *
     * */
    if (m_MotionTimer.getSecondsElapsed() >= .5 &&
        Angle != moveList.back() * 60) {
      Angle = moveList.back() * (60);
      m_MotionTimer.Reset();
    }
    if (m_MotionTimer.getSecondsElapsed() >= 1.) {
      switch (moveList.back()) {
      case EAST:
        Position.setX(Position.getX() + 2 * HORZDIST);
        break;
      case WEST:
        Position.setX(Position.getX() - 2 * HORZDIST);
        break;
      case NORTHEAST:
        Position.setX(Position.getX() + HORZDIST);
        Position.setY(Position.getY() - VERTDIST);
        break;
      case NORTHWEST:
        Position.setX(Position.getX() - HORZDIST);
        Position.setY(Position.getY() - VERTDIST);
        break;
      case SOUTHWEST:
        Position.setX(Position.getX() - HORZDIST);
        Position.setY(Position.getY() + VERTDIST);
        break;
      case SOUTHEAST:
        Position.setX(Position.getX() + HORZDIST);
        Position.setY(Position.getY() + VERTDIST);
        break;
      default:
        break;
      }
      m_MotionTimer.Reset();
      moveList.pop_back();
    }
  }

  // update moving
  Moving = !moveList.empty();
}

bool Actor::turnOrderCompareFunction(const Actor *a, const Actor *b) {
  return a->turnCountdown < b->turnCountdown;
}

void Actor::RenderPortrait() { m_Portrait.Render(); }
