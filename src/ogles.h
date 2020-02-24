#include <stdlib.h>
#include "..\Common\esUtil.h"

struct UserData
{
    // This creates a handle to a program object
    GLuint programObject;
};

static void Draw(ESContext *esContext);

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
