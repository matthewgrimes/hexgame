#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <vector>
#include "math_utils.h"
#include "SDL2/SDL.h"
#include "Timer.h"

struct Frame
{
    Vector3f rightVector;
    Vector3f frontVector;
    Vector3f upVector;
    Vector3f targetInterceptPlane;
    void calculateVectors();
    float angleFromHorz(const Vector3f& v);
};

class Camera
{
public:
    /** Default constructor */
    Camera();
    /** Default destructor */
    virtual ~Camera();

    void Init();

    void setPos(float x, float y, float z);
    void SetspeedX(float s)
    {
        Speed.x = s;
    }
    void SetspeedY(float s)
    {
        Speed.y = s;
    }

    void changeZoom(float factor);
    //! Rotate the camera
    void Rotate(float angle, float x, float y, float z);

    //! Update during main loop
    void Update();

    void Push();
    void Pop();

    const Frame getFrame()
    {
        return m_Frame;
    }
protected:
private:
    void setIsoAngle(float angle);
    bool updateAngle(Vector3f& Pos, Vector3f& Target, Vector3f& Up);
    bool rotationAmountOK(float Amount);
    bool updatePosition(Vector3f& Pos);

    Vector2f Speed;

    float Zoom; //!< Member variable "Zoom"

    float Angle; //!< Member variable "Angle"
    float targetAngle;
    float angleSpeed;

    Frame m_Frame;

    Timer m_Timer;

    Vector3f stackTarget;
    Vector3f stackPos;
    Vector3f stackUp;
};

#endif // CAMERA_H
