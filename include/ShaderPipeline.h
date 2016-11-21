#pragma once

#include "math_utils.h"

class ShaderPipeline
{
public:
    ShaderPipeline();
    ~ShaderPipeline();

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void WorldPos(const Vector3f& Pos)
    {
        m_worldPos = Pos;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(const PersProjInfo& p)
    {
        m_persProjInfo = p;
    }

    float getFOV();
    void setFOV(float FOV);

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        m_persProjInfo.FOV = FOV;
        m_persProjInfo.Width = Width;
        m_persProjInfo.Height = Height;
        m_persProjInfo.zNear = zNear;
        m_persProjInfo.zFar = zFar;
    }

    void SetCamera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }

    void setCameraPos(const Vector3f& Pos)
    {
        m_camera.Pos = Pos;
    }

    void MoveCamera(const Vector3f& delPos);

    const Matrix4f& GetObjectTrans();
    const Matrix4f& GetOrthoTrans();
    const Matrix4f& GetVPTrans();
    const Matrix4f& GetWVPTrans();
    const Matrix4f& GetWorldTrans();
    const Matrix4f& GetTotalTrans();

    struct pCamera
    {
        Vector3f Pos;
        Vector3f Target;
        Vector3f Up;
    };

    struct pObject
    {
        Vector3f Pos;
        float Angle;
    };

    Vector3f getCameraPos()
    {
        return m_camera.Pos;
    }
    const Vector3f getCameraTarget()
    {
        return m_camera.Target;
    }
    Vector3f getCameraUp()
    {
        return m_camera.Up;
    }

    void setObjectPos(const Vector3f& Pos);
    void setObjectAngle(const float Angle);

private:

    void InitScaleTransform(Matrix4f& m) const;
    void InitRotateTransform(Matrix4f& m) const;
    void InitTranslationTransform(Matrix4f& m) const;


    Vector3f m_scale;
    Vector3f m_worldPos;
    Vector3f m_rotateInfo;

    PersProjInfo m_persProjInfo;
    pCamera m_camera;

    Matrix4f m_transformation;
    Matrix4f m_Orthotransformation;
    Matrix4f m_WVPtransformation;
    Matrix4f m_VPTtransformation;
    Matrix4f m_WorldTransformation;
    Matrix4f m_ObjectTransformation;
    Matrix4f m_TotalTrans;

    pObject m_Object;
};

