#include "ShaderPipeline.h"

ShaderPipeline::ShaderPipeline() {
  m_scale = Vector3f(1.f, 1.f, 1.f);
  m_worldPos = Vector3f(0.0f, 0.0f, 0.0f);
  m_rotateInfo = Vector3f(0.0f, 0.0f, 0.0f);

  Vector3f CameraPos(10.f, 10.0f, -5.f);
  Vector3f CameraTarget(0.f, -1.0f, 1.0f);
  Vector3f CameraUp(0.f, 1.f, 1.f);
  SetCamera(CameraPos, CameraTarget, CameraUp);

  m_Object.Pos = Vector3f(0.f, 0.f, 0.f);
  m_Object.Angle = 0.f;

  SetPerspectiveProj(30.0f, 800, 800, 1.0f, 100.0f);
}

ShaderPipeline::~ShaderPipeline() {}

void ShaderPipeline::setObjectPos(const Vector3f &Pos) { m_Object.Pos = Pos; }

void ShaderPipeline::setObjectAngle(const float Angle) {
  m_Object.Angle = Angle;
}

void ShaderPipeline::InitScaleTransform(Matrix4f &m) const {
  m.m[0][0] = m_scale.x;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = 0.0f;
  m.m[1][0] = 0.0f;
  m.m[1][1] = m_scale.y;
  m.m[1][2] = 0.0f;
  m.m[1][3] = 0.0f;
  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = m_scale.z;
  m.m[2][3] = 0.0f;
  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
}

void ShaderPipeline::InitRotateTransform(Matrix4f &m) const {
  Matrix4f rx, ry, rz;

  const float x = ToRadian(m_rotateInfo.x);
  const float y = ToRadian(m_rotateInfo.y);
  const float z = ToRadian(m_rotateInfo.z);

  rx.m[0][0] = 1.0f;
  rx.m[0][1] = 0.0f;
  rx.m[0][2] = 0.0f;
  rx.m[0][3] = 0.0f;
  rx.m[1][0] = 0.0f;
  rx.m[1][1] = cosf(x);
  rx.m[1][2] = -sinf(x);
  rx.m[1][3] = 0.0f;
  rx.m[2][0] = 0.0f;
  rx.m[2][1] = sinf(x);
  rx.m[2][2] = cosf(x);
  rx.m[2][3] = 0.0f;
  rx.m[3][0] = 0.0f;
  rx.m[3][1] = 0.0f;
  rx.m[3][2] = 0.0f;
  rx.m[3][3] = 1.0f;

  ry.m[0][0] = cosf(y);
  ry.m[0][1] = 0.0f;
  ry.m[0][2] = -sinf(y);
  ry.m[0][3] = 0.0f;
  ry.m[1][0] = 0.0f;
  ry.m[1][1] = 1.0f;
  ry.m[1][2] = 0.0f;
  ry.m[1][3] = 0.0f;
  ry.m[2][0] = sinf(y);
  ry.m[2][1] = 0.0f;
  ry.m[2][2] = cosf(y);
  ry.m[2][3] = 0.0f;
  ry.m[3][0] = 0.0f;
  ry.m[3][1] = 0.0f;
  ry.m[3][2] = 0.0f;
  ry.m[3][3] = 1.0f;

  rz.m[0][0] = cosf(z);
  rz.m[0][1] = -sinf(z);
  rz.m[0][2] = 0.0f;
  rz.m[0][3] = 0.0f;
  rz.m[1][0] = sinf(z);
  rz.m[1][1] = cosf(z);
  rz.m[1][2] = 0.0f;
  rz.m[1][3] = 0.0f;
  rz.m[2][0] = 0.0f;
  rz.m[2][1] = 0.0f;
  rz.m[2][2] = 1.0f;
  rz.m[2][3] = 0.0f;
  rz.m[3][0] = 0.0f;
  rz.m[3][1] = 0.0f;
  rz.m[3][2] = 0.0f;
  rz.m[3][3] = 1.0f;

  m = rz * ry * rx;
}

void ShaderPipeline::InitTranslationTransform(Matrix4f &m) const {
  m.m[0][0] = 1.0f;
  m.m[0][1] = 0.0f;
  m.m[0][2] = 0.0f;
  m.m[0][3] = m_worldPos.x;
  m.m[1][0] = 0.0f;
  m.m[1][1] = 1.0f;
  m.m[1][2] = 0.0f;
  m.m[1][3] = m_worldPos.y;
  m.m[2][0] = 0.0f;
  m.m[2][1] = 0.0f;
  m.m[2][2] = 1.0f;
  m.m[2][3] = m_worldPos.z;
  m.m[3][0] = 0.0f;
  m.m[3][1] = 0.0f;
  m.m[3][2] = 0.0f;
  m.m[3][3] = 1.0f;
}

const Matrix4f &ShaderPipeline::GetVPTrans() {
  Matrix4f CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

  CameraTranslationTrans.InitTranslationTransform(
      -m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
  CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
  PersProjTrans.InitPersProjTransform(m_persProjInfo);

  m_VPTtransformation =
      PersProjTrans * CameraRotateTrans * CameraTranslationTrans;
  return m_VPTtransformation;
}
const Matrix4f &ShaderPipeline::GetOrthoTrans() {
  Matrix4f OrthoTrans;
  OrthoTrans.InitOrthoProjTransform(m_persProjInfo);
  m_Orthotransformation = OrthoTrans;
  return m_Orthotransformation;
}
const Matrix4f &ShaderPipeline::GetObjectTrans() {
  Matrix4f ObjectTranslationTrans, ObjectRotationTrans;

  ObjectTranslationTrans.InitTranslationTransform(
      m_Object.Pos.x, m_Object.Pos.y, m_Object.Pos.z);
  ObjectRotationTrans.InitRotateTransform(0.f, m_Object.Angle, 0.f);
  m_ObjectTransformation = ObjectTranslationTrans * ObjectRotationTrans;
  return m_ObjectTransformation;
}

const Matrix4f &ShaderPipeline::GetWorldTrans() {
  Matrix4f ScaleTrans, RotateTrans, TranslationTrans;
  Matrix4f ObjectTranslationTrans, ObjectRotationTrans;

  ObjectTranslationTrans.InitTranslationTransform(
      m_Object.Pos.x, m_Object.Pos.y, m_Object.Pos.z);
  ObjectRotationTrans.InitRotateTransform(0.f, m_Object.Angle, 0.f);
  m_ObjectTransformation = ObjectTranslationTrans * ObjectRotationTrans;

  ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
  RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y,
                                  m_rotateInfo.z);
  TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y,
                                            m_worldPos.z);

  m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;
  return m_WorldTransformation;
}

const Matrix4f &ShaderPipeline::GetWVPTrans() {
  GetWorldTrans();
  GetVPTrans();

  m_WVPtransformation = m_VPTtransformation * m_WorldTransformation;
  return m_WVPtransformation;
}

const Matrix4f &ShaderPipeline::GetTotalTrans() {
  GetWVPTrans();

  m_TotalTrans = m_WVPtransformation * m_ObjectTransformation;
  return m_TotalTrans;
}

void ShaderPipeline::MoveCamera(const Vector3f &delPos) {
  m_camera.Pos += delPos;
}

float ShaderPipeline::getFOV() { return m_persProjInfo.FOV; }

void ShaderPipeline::setFOV(float FOV) { m_persProjInfo.FOV = FOV; }
