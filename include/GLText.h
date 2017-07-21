

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void DrawTexture(int x, int y, GLuint textureid, int textw, int texth);

GLuint LoadTexture(char *File, int *w, int *h);
