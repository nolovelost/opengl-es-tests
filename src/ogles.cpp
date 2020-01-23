#include "ogles.h"

int Engine::Initialize()
{
    // Initialize esContext and set the handle to the userData within it
    esInitContext(&esContext);
    esContext.userData = &userData;

    // Create a window with the appropriate context, name, res and type
    esCreateWindow(&esContext, "test-gles", 320, 240, ES_WINDOW_RGB);
    
    if (!Init(&esContext))
	return 0;

    esRegisterDrawFunc(&esContext, Draw);

    esMainLoop(&esContext);
    
    return 1;
}

// type is either vertex or fragment shader
// shaderSrc is the source of the shader material
/*
This function is used to load the shader. The basic idea behind this shader
based lib is that there are two distinct objects at any given time: shader obj
and the program obj. The shader obj is assigned a shader source file and only
then can it be compiled. After that, the program obj is assigned one and only
one shader obj each for vertex and fragment shader (if the shader obj with 
the shader has been successfully compiled).
Then a program obj is created and the compiled shader obj is assigned to the
program obj which is then linked (as in normal c compilation and linking).
Finally can the program obj be used anytime to draw its defined processes.
*/
GLuint Engine::LoadShader(GLenum type, const char* shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create a shader object and assign it to the var 'shader'
    // Again, the type is either a vertex shader or a fragment shader
    shader = glCreateShader(type);

    if (shader == 0)
	return 0;

    // Load the shader source
    // Here, handle to shaderSrc is assigned to the shader obj
    // If length is NULL, and below it is, then the string is assumed
    // to be null terminated.
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    // This is an online shader compilation. Sometimes there is a need
    // to compile the shaders offline due to the implementation of OpenGL ES 2.0
    // in question.
    glCompileShader(shader);

    // At this point there can be a few errors. These can be inferred and
    // dealt with using the following command.
    // If the shader fails to compile, compiled will be GL_FALSE and vice-versa.
    // Failed info is stored in info log which is used to store any and all
    // information.
    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    // ERROR Handling
    if (!compiled)
    {
	GLint infoLen = 0;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	if (infoLen > 1)
	{
	    char* infoLog = (char*)malloc(sizeof(char) * infoLen);

	    glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
	    esLogMessage("ERROR:: COMPILING SHADER:\n%s\n", infoLog);

	    free(infoLog);
	}

	// Marks the shader object for deletion. Doesn't do immediately.
	// Is deleted only when no program references it anymore.
	// For the current case, where the compilation hasn't happened,
	// the shader is deleted immediately as no program obj can be referencing
	// it at the moment.
	glDeleteShader(shader);
	return 0;
    }

    return shader;
}

// Responsible for defining Program Object and linking it
// Also, feed in vertex/fragment information for the geometry
// Also, inject (bind) attribute information to the shaders that requires them
int Engine::Init(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    UserData *userData = (UserData*)esContext->userData;
    GLbyte vShaderStr[] =
	"attribute vec4 vPosition;    \n"
	"void main()                  \n"
	"{                            \n"
	"   gl_Position = vPosition;  \n"
	"}                            \n";

    GLbyte fShaderStr[] =
	"precision mediump float;\n"\
	"void main()                                  \n"
	"{                                            \n"
	"  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
	"}                                            \n";

    GLuint vShaderObject;
    GLuint fShaderObject;
    GLuint programObject;
    GLint linked;

    // Create the shader objects for both vertex and fragment shaders
    vShaderObject = LoadShader(GL_VERTEX_SHADER, (const char*)vShaderStr);
    fShaderObject = LoadShader(GL_FRAGMENT_SHADER, (const char*)fShaderStr);

    // Create the program object
    programObject = glCreateProgram();

    // Attach the Shader Objects to the Program Object
    // Each Program Object created needs to have attached to it one
    // and only one each of Vertex Shader Object and Fragment Shader Object
    glAttachShader(programObject, vShaderObject);
    glAttachShader(programObject, fShaderObject);

    // Link the Program Object (similar idea to using the linker)
    // Here the final Hardware Instructions are generated for ease of use
    // It validates that all the varying variables used by the Fragment
    // Shader is provided by the Vertex Shader. It also validates that all
    // Uniforms used in the shaders have matching types i.e. the world to screen
    // matrix should be a valid Mat4x4 type. It finally makes sure that the
    // shaders written fit into the limits of the implementation.
    glLinkProgram(programObject);

    // Attributes are allowed to be sent only to the Vertex Shaders
    glBindAttribLocation(programObject, 0, "vPosition");

    // Error Handling of Program Linking
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked)
    {
	GLint infoLen = 0;

	glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

	if (infoLen > 1)
	{
	    char* infoLog = (char*)malloc(sizeof(char) * infoLen);

	    glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
	    esLogMessage("ERROR:: LINKING PROGRAM:\n%s\n", infoLog);

	    free(infoLog);
	}

	glDeleteProgram(programObject);
	return GL_FALSE;
    }

    // It is important to note here that we do not draw USE the Program Object
    // thus linked. It is the only thing separating the renderer from drawning
    // what has been defined in the shaders and compiled into Shader Objects.
    // It is done in the Draw() method.
    
    // Store the Program Object within the UserData type
    userData->programObject = programObject;
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return GL_TRUE;
}

static void Draw(ESContext *esContext)
{
    UserData* userData = (UserData*)esContext->userData;

    GLfloat vVertices[] =
	{
	    0.0f, 0.5f, 0.0f,
	    0.5f, -0.5f, 0.0f,
	    -0.5f, -0.5f, 0.0f	    
	};

    // Set the Viewport
    glViewport (0, 0, esContext->width, esContext->height);

    // Clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // User the Program Object
    glUseProgram(userData->programObject);

    // Load the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
