#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
public:
  Display() {}
  virtual ~Display() {}

  virtual void Initialize(const SDLWindowInfo windowInfo) = 0;
  virtual void Cleanup() = 0;

  virtual void setupDraw() = 0;
  virtual void finishDraw() = 0;

  virtual void setupSceneRender() = 0;
  virtual void setupPickingRender() = 0;
  virtual void setupGUIRender() = 0;
  virtual ShaderManager *getShaderManager() = 0;

  virtual ShaderPipeline *getShaderPipeline() = 0;

protected:
private:
};

#endif // DISPLAY_H
