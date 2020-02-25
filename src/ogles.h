#include <stdlib.h>
#include "..\Common\esUtil.h"
//#include "..\Common\esUtil_win32.cpp"

struct UserData
{
    // This creates a handle to a program object
    GLuint programObject;
};

static void Draw(ESContext *esContext);
static void Shutdown(ESContext *esContext);
//static int esMain(ESContext *esContext);

class Engine
{
public:
    ESContext esContext;
    UserData userData;

public:
    Engine(){};
    
    int Initialize();
    
    GLuint LoadShader(GLenum type, const char *shaderSrc);
    int Init(ESContext *esContext);

};
