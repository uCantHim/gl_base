#include "InputEvents.h"



glb::KeyEvent::KeyEvent(eKey key, eKeyMod mods)
	:
	key(key),
	mods(mods)
{
}


glb::MouseEvent::MouseEvent(vec2 cursorPosition)
{
	cursorPos = cursorPosition;
}


glb::MouseButtonEvent::MouseButtonEvent(eMouseButton button, eKeyMod mods)
	:
	button(button),
	mods(mods)
{
}
