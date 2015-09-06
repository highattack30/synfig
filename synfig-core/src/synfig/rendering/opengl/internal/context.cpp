/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/opengl/internal/context.cpp
**	\brief Context
**
**	$Id$
**
**	\legal
**	......... ... 2014 Ivan Mahonin
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif

#include <synfig/general.h>

#include "context.h"

#endif

using namespace synfig;
using namespace rendering;

/* === M A C R O S ========================================================= */

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

/* === G L O B A L S ======================================================= */

typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

void gl::Context::ContextInfo::make_current() const
{
	glXMakeContextCurrent(display, drawable, read_drawable, context);
}

gl::Context::ContextInfo gl::Context::ContextInfo::get_current(Display *default_display)
{
	ContextInfo ci;
	ci.display = glXGetCurrentDisplay();
	if (!ci.display) ci.display = default_display;
	ci.drawable = glXGetCurrentDrawable();
	ci.read_drawable = glXGetCurrentReadDrawable();
	ci.context = glXGetCurrentContext();
	return ci;
}

gl::Context::Context():
	display(NULL),
	config(None),
	pbuffer(None),
	context(NULL)
{
	// open display (we will use default display and screen 0)
	display = XOpenDisplay(NULL);
	context_info.display = display;

	// choose config
	if (display)
	{
		int config_attribs[] = {
			GLX_DOUBLEBUFFER,      False,
			GLX_RED_SIZE,          8,
			GLX_GREEN_SIZE,        8,
			GLX_BLUE_SIZE,         8,
			GLX_ALPHA_SIZE,        8,
			GLX_DEPTH_SIZE,        24,
			GLX_STENCIL_SIZE,      8,
			GLX_ACCUM_RED_SIZE,    8,
			GLX_ACCUM_GREEN_SIZE,  8,
			GLX_ACCUM_BLUE_SIZE,   8,
			GLX_ACCUM_ALPHA_SIZE,  8,
			GLX_DRAWABLE_TYPE,     GLX_PBUFFER_BIT,
			None };
		int nelements = 0;
		GLXFBConfig *configs = glXChooseFBConfig(display, 0, config_attribs, &nelements);
		if (configs != NULL && nelements > 0)
			config = configs[0];
	}

	// create pbuffer
	if (config)
	{
		int pbuffer_attribs[] = {
			GLX_PBUFFER_WIDTH, 256,
			GLX_PBUFFER_HEIGHT, 256,
			None };
		pbuffer = glXCreatePbuffer(display, config, pbuffer_attribs);
		context_info.drawable = pbuffer;
		context_info.read_drawable = pbuffer;
	}

	// create context
	if (pbuffer)
	{
		int context_attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
			GLX_CONTEXT_MINOR_VERSION_ARB, 3,
			None };
		GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
		context = glXCreateContextAttribsARB(display, config, NULL, True, context_attribs);
		context_info.context = context;
	}

	assert(context);
	use();
	check();
}

gl::Context::~Context()
{
	unuse();
	if (context)
		glXDestroyContext(display, context);
	context = NULL;
	context_info.context = NULL;
	if (pbuffer)
		glXDestroyPbuffer(display, pbuffer);
	pbuffer = None;
	context_info.drawable = None;
	context_info.read_drawable = None;
	config = None;
	if (display)
		XCloseDisplay(display);
	display = NULL;
	context_info.display = None;
}

bool gl::Context::is_current() const
{
	return is_valid()
		&& context_info == ContextInfo::get_current(display);
}

void gl::Context::use()
{
	if (is_valid()) {
		context_stack.push_back(ContextInfo::get_current(display));
		context_info.make_current();
	}
}

void gl::Context::unuse()
{
	assert(is_current());
	if (is_valid()) {
		assert(!context_stack.empty());
		context_stack.back().make_current();
		context_stack.pop_back();
	}
}

void gl::Context::check(const std::string &s) {
	if (GLenum error = glGetError())
		warning("%s GL error: 0x%x", s.c_str(), error);
}

/* === E N T R Y P O I N T ================================================= */
