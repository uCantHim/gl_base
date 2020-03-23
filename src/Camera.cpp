#include "Camera.h"

#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



glb::Camera::Camera(Viewport viewport, float fovDegrees, vec2 depthBounds)
    :
    viewport(viewport),
    depthBounds(depthBounds),
    fov(fovDegrees)
{
	calcViewMatrix();
    makePerspective(viewport, fovDegrees, depthBounds.x, depthBounds.y);
}

glb::Camera::Camera(Viewport viewport, float left, float right, float bottom, float top, vec2 depthBounds)
    :
    viewport(viewport)
{
    calcViewMatrix();
    makeOrthogonal(left, right, bottom, top, depthBounds.x, depthBounds.y);
}

mat4 glb::Camera::getViewMatrix() const
{
	return viewMatrix;
}

mat4 glb::Camera::getProjectionMatrix() const
{
	return projectionMatrix;
}

vec3 glb::Camera::getPosition() const
{
	return position;
}

vec3 glb::Camera::getForwardVector() const
{
	return forwardVector;
}

vec3 glb::Camera::getUpVector() const
{
	return upVector;
}

auto glb::Camera::getViewport() const -> const Viewport&
{
	return viewport;
}

void glb::Camera::setPosition(vec3 newPos)
{
	position = newPos;
    calcViewMatrix();
	//viewMatrix = glm::translate(viewMatrix, newPos);
}

void glb::Camera::setForwardVector(vec3 forward)
{
	forwardVector = forward;
	calcViewMatrix();
}

void glb::Camera::setUpVector(vec3 up)
{
	upVector = up;
	calcViewMatrix();
}

void glb::Camera::setViewport(Viewport newViewport)
{
	viewport = newViewport;
	aspect = static_cast<float> (viewport.size.x) / static_cast<float> (viewport.size.y);
    if (!isOrtho)
        calcProjMatrix();
}

void glb::Camera::setDepthBounds(float minDepth, float maxDepth)
{
	depthBounds = vec2(minDepth, maxDepth);
	calcProjMatrix();
}

void glb::Camera::setFov(float newFov)
{
	fov = newFov;
	calcProjMatrix();
}

void glb::Camera::makePerspective(Viewport viewport, float fov, float zNear, float zFar)
{
    isOrtho = false;
    this->viewport = viewport;
	aspect = static_cast<float> (viewport.size.x) / static_cast<float> (viewport.size.y);
    this->fov = fov;
    depthBounds = vec2(zNear, zFar);
    calcProjMatrix();
}

void glb::Camera::makeOrthogonal(float left, float right, float bottom, float top, float zNear, float zFar)
{
    isOrtho = true;
    orthoLeft = left;
    orthoRight = right;
    orthoBottom = bottom;
    orthoTop = top;
    depthBounds = vec2(zNear, zFar);
    calcProjMatrix();
}

void glb::Camera::updateViewport() const
{
	glViewport(viewport.offset.x, viewport.offset.y, viewport.size.x, viewport.size.y);
}

void glb::Camera::calcViewMatrix()
{
	viewMatrix = glm::lookAt(position, position + forwardVector, upVector);
}

void glb::Camera::calcProjMatrix()
{
    if (isOrtho)
        projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, depthBounds.x, depthBounds.y);
    else
        projectionMatrix = glm::perspective(glm::radians(fov), aspect, depthBounds.x, depthBounds.y);
}
