#if !defined(_OPENGL_H_)
#define _OEPENGL_H_

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <io.h>
#include <fcntl.h>

#define printOpenGLError() PrintOglError(__FILE__, __LINE__)

class COpenGL  
{
typedef enum {
	EVertexShader,
		EFragmentShader,
} EShaderType;

//변수 선언
private:
	HWND	m_hWnd;
	GLuint	Prog;

public:
	FILE*	m_Error;
	HDC     m_hDC;
    HGLRC   m_hRC;
	GLuint	base;

//함수 정의
	COpenGL::COpenGL()
	{

	};

	COpenGL::~COpenGL()
	{

	};

	GLuint COpenGL::GetProgLog()
	{return Prog;};

	void COpenGL::SetHWND(HWND hWnd)
	{
		ASSERT(hWnd);
		m_hWnd = hWnd;
	};

	BOOL COpenGL::SetupPixelFormat()
	{
		static PIXELFORMATDESCRIPTOR pfd = 
			{
				sizeof(PIXELFORMATDESCRIPTOR),
				1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,  
				PFD_TYPE_RGBA,
				24,
				0, 0, 0, 0, 0, 0,
				0,
				0,
				0,
				0, 0, 0, 0,
				16,
				0,
				0,
				PFD_MAIN_PLANE,
				0,
				0, 0, 0
			};    
        
		m_hDC = ::GetDC(m_hWnd);
		GLuint PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
        
		if (!PixelFormat)
		{
			return false;
		}
        
		if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))
		{
			return false;
		}
        
		m_hRC = wglCreateContext(m_hDC);
		if(!m_hRC)
		{
			return false;
		}
        
		if(!wglMakeCurrent(m_hDC, m_hRC))
		{
			return false;
		}
        
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SwapBuffers(m_hDC);
		BuildFont();
		return TRUE;
	};

	void COpenGL::BuildFont()
	{
		HFONT font;                        
		base = glGenLists(96);               
		font = CreateFont(-14,               
							0,
							0,
							0,
					  FW_BOLD,            
						FALSE,               
						FALSE,               
						FALSE,               
				 ANSI_CHARSET,                
				OUT_TT_PRECIS,
		  CLIP_DEFAULT_PRECIS,
		  ANTIALIASED_QUALITY,
	FF_DONTCARE|DEFAULT_PITCH,
							  ANSI_CHARSET); 
   
		SelectObject(m_hDC, font);              
		wglUseFontBitmaps(m_hDC, 32, 96, base); 
	};

	void getGlVersion(int *major, int *minor)
	{
		const char* verstr = (const char*)glGetString( GL_VERSION );
		if( (verstr == NULL) || (sscanf( verstr, "%d.%d", major, minor ) != 2) )
		{
			*major = *minor = 0;
			fprintf( stderr, "Invalid GL_VERSION format!!!\n" );
		}
	};

	void SetupShader(char* filename)
	{
		//opengl version
		int gl_major, gl_minor;

		m_Error=fopen("error.txt","w");

		glewInit();
		
		if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
			fprintf(m_Error,"Ready for GLSL\n");
		else 
		{
			fprintf(m_Error,"No GLSL support\n");
			exit(1);
		}
		
		// Make sure that OpenGL 2.0 is supported by the driver
		getGlVersion(&gl_major, &gl_minor);
		fprintf(m_Error,"GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
		
		if (gl_major < 2)
		{
			fprintf(m_Error,"GL_VERSION major=%d minor=%d\n", gl_major, gl_minor);
			fprintf(m_Error,"Support for OpenGL 2.0 is required for this demo...exiting\n");
			exit(1);
		}
		
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		NextClearColor();
		
		GLchar *VertexShaderSource, *FragmentShaderSource;
		ReadShaderSource(filename,&VertexShaderSource, &FragmentShaderSource);
		InstallShaders(VertexShaderSource, FragmentShaderSource);
		delete VertexShaderSource;
		delete FragmentShaderSource;
	};

	int ReadShaderSource(char *fileName, GLchar **vertexShader, GLchar **fragmentShader)
	{
		int vSize, fSize;
		
		//
		// Allocate memory to hold the source of our shaders.
		//
		vSize = ShaderSize(fileName, EVertexShader);
		fSize = ShaderSize(fileName, EFragmentShader);
		
		if ((vSize == -1) || (fSize == -1))
		{
			fprintf(m_Error,"Cannot determine size of the shader %s\n", fileName);
			return 0;
		}
		
		*vertexShader = (GLchar *) malloc(vSize);
		*fragmentShader = (GLchar *) malloc(fSize);
		
		//
		// Read the source code
		//
		if (!ReadShader(fileName, EVertexShader, *vertexShader, vSize))
		{
			fprintf(m_Error,"Cannot read the file %s.vert\n", fileName);
			return 0;
		}
		
		if (!ReadShader(fileName, EFragmentShader, *fragmentShader, fSize))
		{
			fprintf(m_Error,"Cannot read the file %s.frag\n", fileName);
			return 0;
		}
		
		return 1;
	};

	int ReadShader(char *fileName, EShaderType shaderType, char *shaderText, int size)
	{
		//
		// Reads a shader from the supplied file and returns the shader in the
		// arrays passed in. Returns 1 if successful, 0 if an error occurred.
		// The parameter size is an upper limit of the amount of bytes to read.
		// It is ok for it to be too big.
		//
		FILE *fh;
		char name[100];
		int count;
		
		strcpy(name, fileName);
		
		switch (shaderType) 
		{
		case EVertexShader:
			strcat(name, ".vert");
			break;
		case EFragmentShader:
			strcat(name, ".frag");
			break;
		default:
			fprintf(m_Error,"ERROR: unknown shader file type\n");
			exit(1);
			break;
		}
		
		//
		// Open the file
		//
		fh = fopen(name, "r");
		if (!fh)
			return -1;
		
		//
		// Get the shader from a file.
		//
		fseek(fh, 0, SEEK_SET);
		count = (int) fread(shaderText, 1, size, fh);
		shaderText[count] = '\0';
		
		if (ferror(fh))
			count = 0;
		
		fclose(fh);
		return count;
	};

	int ShaderSize(char *fileName, EShaderType shaderType)
	{
		//
		// Returns the size in bytes of the shader fileName.
		// If an error occurred, it returns -1.
		//
		// File name convention:
		//
		// <fileName>.vert
		// <fileName>.frag
		//
		int fd;
		char name[100];
		int count = -1;
		
		strcpy(name, fileName);
		
		switch (shaderType)
		{
		case EVertexShader:
			strcat(name, ".vert");
			break;
		case EFragmentShader:
			strcat(name, ".frag");
			break;
		default:
			fprintf(m_Error,"ERROR: unknown shader file type\n");
			exit(1);
			break;
		}
		
		//
		// Open the file, seek to the end to find its length
		//
	#ifdef WIN32 
		fd = _open(name, _O_RDONLY);
		if (fd != -1)
		{
			count = _lseek(fd, 0, SEEK_END) + 1;
			_close(fd);
		}
	#else 
		fd = open(name, O_RDONLY);
		if (fd != -1)
		{
			count = lseek(fd, 0, SEEK_END) + 1;
			close(fd);
		}
	#endif 
		
		return count;
	};

	int InstallShaders(const GLchar *Vertex, const GLchar *Fragment)
	{
		GLuint VS, FS;   // handles to objects
		GLint  vertCompiled, fragCompiled;    // status values
		GLint  linked;

		// Create a vertex shader object and a fragment shader object

		VS = glCreateShader(GL_VERTEX_SHADER);
		FS = glCreateShader(GL_FRAGMENT_SHADER);

		// Load source code strings into shaders

		glShaderSource(VS, 1, &Vertex, NULL);
		glShaderSource(FS, 1, &Fragment, NULL);

		// Compile the brick vertex shader, and print out
		// the compiler log file.

		glCompileShader(VS);
		printOpenGLError();  // Check for OpenGL errors
		glGetShaderiv(VS, GL_COMPILE_STATUS, &vertCompiled);
		PrintShaderInfoLog(VS);

		// Compile the brick vertex shader, and print out
		// the compiler log file.

		glCompileShader(FS);
		printOpenGLError();  // Check for OpenGL errors
		glGetShaderiv(FS, GL_COMPILE_STATUS, &fragCompiled);
		PrintShaderInfoLog(FS);

		if (!vertCompiled || !fragCompiled)
			return 0;

		// Create a program object and attach the two compiled shaders

		Prog = glCreateProgram();
		glAttachShader(Prog, VS);
		glAttachShader(Prog, FS);

		// Link the program object and print out the info log

		glLinkProgram(Prog);
		printOpenGLError();  // Check for OpenGL errors
		glGetProgramiv(Prog, GL_LINK_STATUS, &linked);
		PrintProgramInfoLog(Prog);

		if (!linked)
			return 0;

		// Install program object as part of current state

		glUseProgram(Prog);

		// Set up initial uniform values
		return 1;
	}

	int InstallBrickShaders(const GLchar *brickVertex,
							const GLchar *brickFragment)
	{
		GLuint brickVS, brickFS, brickProg;   // handles to objects
		GLint  vertCompiled, fragCompiled;    // status values
		GLint  linked;
		
		// Create a vertex shader object and a fragment shader object
		
		brickVS = glCreateShader(GL_VERTEX_SHADER);
		brickFS = glCreateShader(GL_FRAGMENT_SHADER);
		
		// Load source code strings into shaders
		
		glShaderSource(brickVS, 1, &brickVertex, NULL);
		glShaderSource(brickFS, 1, &brickFragment, NULL);
		
		// Compile the brick vertex shader, and print out
		// the compiler log file.
		
		glCompileShader(brickVS);
		printOpenGLError();  // Check for OpenGL errors
		glGetShaderiv(brickVS, GL_COMPILE_STATUS, &vertCompiled);
		PrintShaderInfoLog(brickVS);
		
		// Compile the brick vertex shader, and print out
		// the compiler log file.
		
		glCompileShader(brickFS);
		printOpenGLError();  // Check for OpenGL errors
		glGetShaderiv(brickFS, GL_COMPILE_STATUS, &fragCompiled);
		PrintShaderInfoLog(brickFS);
		
		if (!vertCompiled || !fragCompiled)
			return 0;
		
		// Create a program object and attach the two compiled shaders
		
		brickProg = glCreateProgram();
		glAttachShader(brickProg, brickVS);
		glAttachShader(brickProg, brickFS);
		
		// Link the program object and print out the info log
		
		glLinkProgram(brickProg);
		printOpenGLError();  // Check for OpenGL errors
		glGetProgramiv(brickProg, GL_LINK_STATUS, &linked);
		PrintProgramInfoLog(brickProg);
		
		if (!linked)
			return 0;
		
		// Install program object as part of current state
		
		glUseProgram(brickProg);
		
		// Set up initial uniform values
		
		glUniform3f(GetUniLoc(brickProg, "BrickColor"), 1.0, 0.3, 0.2);
		glUniform3f(GetUniLoc(brickProg, "MortarColor"), 0.85, 0.86, 0.84);
		glUniform2f(GetUniLoc(brickProg, "BrickSize"), 0.30, 0.15);
		glUniform2f(GetUniLoc(brickProg, "BrickPct"), 0.90, 0.85);
		glUniform3f(GetUniLoc(brickProg, "LightPosition"), 0.0, 0.0, 4.0);
		
		Prog=brickProg;
		return 1;
	};

	GLint GetUniLoc(GLuint program, const GLchar *name)
	{
		GLint loc;
		
		loc = glGetUniformLocation(program, name);
		
		if (loc == -1)
			printf("No such uniform named \"%s\"\n", name);
		
		printOpenGLError();  // Check for OpenGL errors
		return loc;
	};

	int PrintOglError(char *file, int line)
	{
		//
		// Returns 1 if an OpenGL error occurred, 0 otherwise.
		//
		GLenum glErr;
		int    retCode = 0;
		
		glErr = glGetError();
		while (glErr != GL_NO_ERROR)
		{
			fprintf(m_Error,"glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
			retCode = 1;
			glErr = glGetError();
		}
		return retCode;
	};

	void PrintProgramInfoLog(GLuint program)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		GLchar *infoLog;
		
		printOpenGLError();  // Check for OpenGL errors
		
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
		
		printOpenGLError();  // Check for OpenGL errors
		
		if (infologLength > 0)
		{
			infoLog = (GLchar *)malloc(infologLength);
			if (infoLog == NULL)
			{
				fprintf(m_Error,"ERROR: Could not allocate InfoLog buffer\n");
				exit(1);
			}
			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
			fprintf(m_Error,"Program InfoLog:\n%s\n\n", infoLog);
			free(infoLog);
		}
		printOpenGLError();  // Check for OpenGL errors
	};

	void PrintShaderInfoLog(GLuint shader)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		GLchar *infoLog;
		
		printOpenGLError();  // Check for OpenGL errors
		
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
		
		printOpenGLError();  // Check for OpenGL errors
		
		if (infologLength > 0)
		{
			infoLog = (GLchar *)malloc(infologLength);
			if (infoLog == NULL)
			{
				fprintf(m_Error,"ERROR: Could not allocate InfoLog buffer\n");
				exit(1);
			}
			glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
			fprintf(m_Error,"Shader InfoLog:\n%s\n\n", infoLog);
			free(infoLog);
		}
		printOpenGLError();  // Check for OpenGL errors
	};

	void NextClearColor(void)
	{
		static int color = 0;
		
		switch( color++ )
		{
		case 0:  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:  glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
			break;
		default: glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
			color = 0;
			break;
		}
	};
};
#endif




