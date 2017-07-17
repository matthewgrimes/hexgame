#include "Camera.h"
#include "Pipeline.h"
#define zoomMax 3
#define zoomMin 1

void Frame::calculateVectors() {
  rightVector = (Pipeline::getInstance()->getCameraTarget().Cross(
                     Pipeline::getInstance()->getCameraUp()))
                    .Normalize();
  frontVector = Vector3f(rightVector.z, 0.f, -rightVector.x);
  upVector = Vector3f(0.f, 1.f, 0.f);
  Vector3f targetVector = Pipeline::getInstance()->getCameraTarget();
  targetVector.Normalize();

  targetInterceptPlane = targetVector;
  targetInterceptPlane *= Pipeline::getInstance()->getCameraPos().y /
                          sinf(angleFromHorz(targetVector));
  targetInterceptPlane += Pipeline::getInstance()->getCameraPos();
}
float Frame::angleFromHorz(const Vector3f &v) {
  return acos(frontVector.dot(v));
}

Camera::Camera() {
  Speed = Vector2f(0, 0);
  Zoom = 1;

  Angle = 0;
  m_Timer.Start();
}

Camera::~Camera() {
  // dtor
}

void Camera::changeZoom(float factor) {
  if (Zoom * factor > zoomMin && Zoom * factor < zoomMax) {
    // keep track of the scaling
    Zoom *= factor;

    // Pipeline::getInstance()->setFOV(Pipeline::getInstance()->getFOV() *
    // factor );
    Vector3f newPos = Pipeline::getInstance()->getCameraPos();
    newPos -= m_Frame.targetInterceptPlane;
    newPos *= factor;
    newPos += m_Frame.targetInterceptPlane;
    Pipeline::getInstance()->setCameraPos(newPos);
    setIsoAngle(50 * (Zoom - zoomMin) * 1.0 / (zoomMax - zoomMin) + 30);
  }
}

bool Camera::updatePosition(Vector3f &Pos) {
  if (Speed == Vector2f(0, 0)) {
    return 0;
  }
  Vector3f xVel = m_Frame.rightVector;
  Vector3f yVel = m_Frame.frontVector;
  xVel *= -Speed.x * m_Timer.getSecondsElapsed();
  yVel *= Speed.y * m_Timer.getSecondsElapsed();
  Pos += xVel + yVel;
  return 1;
}
bool Camera::rotationAmountOK(float Amount) {
  bool OK = angleSpeed > 0 && Angle + Amount > targetAngle;
  OK = OK || (angleSpeed < 0 && Angle + Amount < targetAngle);
  return !OK;
}

bool Camera::updateAngle(Vector3f &Pos, Vector3f &Target, Vector3f &Up) {
  if (angleSpeed == 0) {
    return 0;
  }

  float rotationAmount = m_Timer.getSecondsElapsed() * angleSpeed * 100;
  if (!rotationAmountOK(rotationAmount)) {
    rotationAmount = targetAngle - Angle;
    angleSpeed = 0;
  }
  Angle += rotationAmount;

  Pos -= m_Frame.targetInterceptPlane;
  Pos.Rotate(rotationAmount, m_Frame.upVector);
  Pos += m_Frame.targetInterceptPlane;

  Target.Rotate(rotationAmount, m_Frame.upVector);
  Up.Rotate(rotationAmount, m_Frame.upVector);

  m_Frame.calculateVectors();
  return 1;
}

void Camera::Update() {
  Vector3f newPos, rotatedTarget, rotatedUp;

  rotatedTarget = Pipeline::getInstance()->getCameraTarget();
  rotatedUp = Pipeline::getInstance()->getCameraUp();
  newPos = Pipeline::getInstance()->getCameraPos();

  m_Frame.calculateVectors();

  if (updateAngle(newPos, rotatedTarget, rotatedUp) || updatePosition(newPos)) {
    Pipeline::getInstance()->SetCamera(newPos, rotatedTarget, rotatedUp);
  }

  m_Timer.Reset();
}

void Camera::Init() {
  setIsoAngle(50 * (Zoom - zoomMin) * 1.0 / (zoomMax - zoomMin) + 30);
}

void Camera::Rotate(float angle, float x, float y, float z) {
  // Angle += angle;
  targetAngle += angle;
  angleSpeed = 2 * (targetAngle > Angle) - 1;
}

void Camera::setIsoAngle(float angle) {
  float dAngle;

  float angleFromHorz;

  m_Frame.calculateVectors();
  Vector3f targetVector = Pipeline::getInstance()->getCameraTarget();
  targetVector.Normalize();

  angleFromHorz = acos(m_Frame.frontVector.dot(targetVector));
  dAngle = ToDegree(angleFromHorz - ToRadian(angle));

  Vector3f newPos, newTarget, newUp;
  newPos = Pipeline::getInstance()->getCameraPos();
  newTarget = Pipeline::getInstance()->getCameraTarget();
  newUp = Pipeline::getInstance()->getCameraUp();

  newPos -= m_Frame.targetInterceptPlane;
  newPos.Rotate(dAngle, m_Frame.rightVector);
  newPos += m_Frame.targetInterceptPlane;

  newTarget.Rotate(dAngle, m_Frame.rightVector);
  newUp.Rotate(dAngle, m_Frame.rightVector);

  Pipeline::getInstance()->SetCamera(newPos, newTarget, newUp);

  m_Frame.calculateVectors();
}

void Camera::Push() {
  stackPos = Pipeline::getInstance()->getCameraPos();
  stackTarget = Pipeline::getInstance()->getCameraTarget();
  stackUp = Pipeline::getInstance()->getCameraUp();
  Pipeline::getInstance()->SetCamera(Vector3f(0.f, 1.f, 0.f),
                                     Vector3f(0.f, -1.f, 0.f),
                                     Vector3f(0.f, 0.f, 1.f));
}
void Camera::Pop() {
  Pipeline::getInstance()->SetCamera(stackPos, stackTarget, stackUp);
}

void Camera::setPos(float x, float y, float z) {
  Pipeline::getInstance()->setCameraPos(Vector3f(x, y, z));
}
