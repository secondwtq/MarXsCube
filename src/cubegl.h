//
//  cubegl.h
//  MarXsCube
//
//  Created by SeconDatke on 2/9/15.
//  Copyright (c) 2015 MarXsCube Staff. All rights reserved.
//

#ifndef MarXsCube_cubegl_h
#define MarXsCube_cubegl_h

// based on SFML/OpenGL.h
// SFML wrapper has no glext.h with Linux
// so we need this

#include "buildconf.h"
#include "Platform.h"

#if defined(CUBE_PLATFORM_DARWIN)

#if defined(CUBE_CONFIG_USE_GL3)

#include <OpenGL/gl3.h>

#else

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#endif

#elif defined(CUBE_PLATFORM_WINDOWS)

// The Visual C++ version of gl.h uses
// WINGDIAPI and APIENTRY but doesn't define them
#ifdef CUBE_COMPILER_MSVC
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#elif defined(CUBE_PLATFORM_LINUX)

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#endif

/*
#if defined(SFML_OPENGL_ES)
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#endif

#elif defined (SFML_SYSTEM_IOS)

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#elif defined (SFML_SYSTEM_ANDROID)

#include <GLES/gl.h>
#include <GLES/glext.h>

#endif
*/

#endif /* defined(MarXsCube_cubegl_h) */
