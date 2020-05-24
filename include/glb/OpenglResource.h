#pragma once
#ifndef OPENGLRESOURCE_H
#define OPENGLRESOURCE_H

#include <memory>
#include <functional>
#include <type_traits>
#include <utility>

#include <GL/glew.h>

#ifdef glSharedBuffer
	#error glSharedBuffer if already defined! Appearently, there has been a patch that introduced this function or object. Congrats, you are screwed!
#endif // glSharedBuffer
#ifdef glUniqueBuffer
	#error glSharedBuffer if already defined! Appearently, there has been a patch that introduced this function or object. Congrats, you are screwed!
#endif // glUniqueBuffer


/*			+++ OpenglResource +++

This file defines wrapper classes for OpenGL state objects.

These wrappers manage OpenGL handles on their own. They are intended to replace plain GLuints as resource
handles.

glShared*-objects can be copied and moved while preserving OpenGL state. Thus, they can be used to share
contained OpenGL state between multiple instances.
The contained OpenGL objects will only be destroyed when no references to them (glShared*-objects) exist anymore.

glUnique*-objects can only be moved, ensuring that only a single owner exists at any time.


--- Example:

GLuint bufHnd = 0;
glCreateBuffers(1, &bufHnd);

auto sharedBuffer = glSharedBuffer(bufHnd);
auto anotherBuffer = sharedBuffer;			// anotherBuffer now references the same OpenGL object as sharedBuffer.

sharedBuffer.release();						// anotherBuffer still contains a valid OpenGL object since
											// it will only be destroyed when no glSharedBuffer-instances
											// reference it anymore.

anotherBuffer = 0;							// The previously created buffer will be destroyed since it's not referenced by anyone.
											// This assignment to 0 is equivalent to anotherBuffer.release(), just not as expressive.



Attention:
Assigning a new value to a pointer to the resource handle would assign that value to ALL copies of that handle and
the previously contained object will NOT be destroyed, leak and just exist forever, unable to be deleted.
Instead, use either glShared*.assign(), which first releases the contained object reference and then returns a pointer
that is safe to modify, or release() the reference first, then assign to the pointer.

--- Example, bad:

auto handle = glSharedBuffer();		// The newly created buffer handle contains no OpenGL object.
glCreateBuffers(1, &handle);		// OK, a new OpenGL object will be stored in the buffer handle.
[...]
glCreateBuffers(1, &handle);		// BAD, the previously created buffer leaked and will remain in GPU memory forever.


--- Example, good:

auto handle = glSharedBuffer();			// The newly created buffer handle contains no OpenGL object.
glCreateBuffers(1, &handle);			// OK, a new OpenGL object will be stored in the buffer handle.
[...]
handle.release();						// The previously created buffer is destroyed since the only reference to it is released.
glCreateBuffers(1, &handle);			// OK, the handle contains no valid OpenGL object anymore.

glCreateBuffers(1, handle.assign());	// OK, assign() releases the OpenGL object, the returned pointer can be safely modified.
*/



// DELETERS

struct _buffer_deleter
{
public:
	void operator()(GLuint* handle) const noexcept {
		glDeleteBuffers(1, handle);
		delete handle;
	}
};

struct _texture_deleter
{
public:
	void operator()(GLuint* handle) const noexcept {
		glDeleteTextures(1, handle);
		delete handle;
	}
};

struct _vertex_array_deleter
{
public:
	void operator()(GLuint* handle) const noexcept {
		glDeleteVertexArrays(1, handle);
		delete handle;
	}
};

struct _transform_feedback_deleter
{
public:
	void operator()(GLuint* handle) const noexcept {
		glDeleteTransformFeedbacks(1, handle);
		delete handle;
	}
};

struct _framebuffer_deleter
{
public:
	void operator()(GLuint* handle) const noexcept {
		glDeleteFramebuffers(1, handle);
		delete handle;
	}
};

struct _renderbuffer_deleter
{
public:
	void operator()(GLuint* handle) const noexcept {
		glDeleteRenderbuffers(1, handle);
		delete handle;
	}
};

struct _program_deleter
{
public:
	void operator()(const GLuint* handle) const noexcept {
		glDeleteProgram(*handle);
		delete handle;
	}
};



// SHARED RESOURCE
template<class _Del>
class _gl_shared_resource
{
public:
	// Default constructor, initializes buffer handle to 0.
	_gl_shared_resource() = default;
	~_gl_shared_resource() noexcept = default;

	_gl_shared_resource(const _gl_shared_resource&) = default;
	_gl_shared_resource& operator=(const _gl_shared_resource&) = default;
	_gl_shared_resource(_gl_shared_resource&&) noexcept = default;
	_gl_shared_resource& operator=(_gl_shared_resource&&) noexcept = default;

	// Takes the resource handle that will be managed.
	explicit _gl_shared_resource(GLuint handle)
		:
		_handle(std::shared_ptr<GLuint>(new GLuint(handle), _Del()))
	{
	}

	// Assignment operator from GLuint
	inline auto operator=(GLuint rhs) noexcept -> _gl_shared_resource& {
		_handle = std::shared_ptr<GLuint>(new GLuint(rhs), _Del());
		return *this;
	}

	// The natural-feeling address operator
	[[nodiscard]]
	inline auto operator&() const noexcept -> GLuint* {
		return _handle.get();
	}

	// The natural value-access operator
	[[nodiscard]]
	inline auto operator*() const noexcept -> GLuint {
		return *_handle;
	}

	// Releases the contained object. Deletes the object if this is the last reference to it.
	inline void release() noexcept {
		_handle.reset(new GLuint(0), _Del());
	}

	// Releases the contained object. Returns an assignable pointer to the object handle.
	inline auto assign() noexcept -> GLuint* {
		release();
		return _handle.get();
	}

	// Swaps the handle with another object.
	inline void swap(_gl_shared_resource& other) noexcept {
		_handle.swap(other._handle);
	}

private:
	std::shared_ptr<GLuint> _handle{ new GLuint{0}, _Del() };
};



// UNIQUE RESOURCE
template<class _Del>
class _gl_unique_resource
{
public:
	using _gl_deleter_func = std::function<void(GLuint*)>;

	// Default constructor, initializes buffer handle to 0.
	_gl_unique_resource() = default;
	~_gl_unique_resource() noexcept = default;

	_gl_unique_resource(const _gl_unique_resource&) = delete;
	_gl_unique_resource& operator=(const _gl_unique_resource&) = delete;
	_gl_unique_resource(_gl_unique_resource&&) noexcept = default;
	_gl_unique_resource& operator=(_gl_unique_resource&&) noexcept = default;

	// Takes the resource handle that will be managed.
	explicit _gl_unique_resource(GLuint handle)
		:
		_handle(std::unique_ptr<GLuint, _gl_deleter_func>(new GLuint(handle), _Del()))
	{
	}

	// Assignment operator from GLuint
	inline auto operator=(GLuint rhs) noexcept -> _gl_unique_resource& {
		_handle = std::unique_ptr<GLuint, _gl_deleter_func>(new GLuint(rhs), _Del());
		return *this;
	}

	// The natural-feeling address operator
	inline auto operator&() const noexcept -> GLuint* {
		return _handle.get();
	}

	// The natural value-access operator
	inline auto operator*() const noexcept -> GLuint {
		return *_handle;
	}

	// Deletes the contained object.
	inline void release() noexcept {
		_handle.reset(new GLuint(0));
	}

	// Deletes the contained object. Returns an assignable pointer to the object handle.
	inline auto assign() noexcept -> GLuint* {
		release();
		return _handle.get();
	}

	inline void swap(_gl_unique_resource& other) noexcept {
		_handle.swap(other._handle);
	}

private:
	std::unique_ptr<GLuint, _gl_deleter_func> _handle{ new GLuint{0}, _Del() };
};


using glSharedBuffer			= _gl_shared_resource<_buffer_deleter>;
using glSharedTexture			= _gl_shared_resource<_texture_deleter>;
using glSharedVertexArray		= _gl_shared_resource<_vertex_array_deleter>;
using glSharedTransformFeedback = _gl_shared_resource<_transform_feedback_deleter>;
using glSharedFramebuffer		= _gl_shared_resource<_framebuffer_deleter>;
using glSharedRenderbuffer		= _gl_shared_resource<_renderbuffer_deleter>;
using glSharedProgram			= _gl_shared_resource<_program_deleter>;

using glUniqueBuffer			= _gl_unique_resource<_buffer_deleter>;
using glUniqueTexture			= _gl_unique_resource<_texture_deleter>;
using glUniqueVertexArray		= _gl_unique_resource<_vertex_array_deleter>;
using glUniqueTransformFeedback = _gl_unique_resource<_transform_feedback_deleter>;
using glUniqueFramebuffer		= _gl_unique_resource<_framebuffer_deleter>;
using glUniqueRenderbuffer		= _gl_unique_resource<_renderbuffer_deleter>;
using glUniqueProgram			= _gl_unique_resource<_program_deleter>;

#endif