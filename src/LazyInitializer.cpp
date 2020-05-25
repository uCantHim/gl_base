#include "LazyInitializer.h"

#include "Window.h"



void glb::OpenGlLazyInit::addLazyInitializer(std::function<void(void)> func)
{
	if (!Window::isContextCreated())
		lazyInitializers.push_back(std::move(func));
	else
		std::invoke(func);
}

void glb::OpenGlLazyInit::initAll()
{
	for (const auto& func : lazyInitializers)
		std::invoke(func);
}



template<class Derived>
glb::OpenGlLazyInitializer<Derived>::OpenGlLazyInitializer()
{
	addLazyInitializer(std::bind(&Derived::openGlLazyInit, this));
}
