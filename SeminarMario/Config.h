#pragma once

enum EventSenders { SENDER_ENTITY_STATE, SENDER_KEYBOARD, SENDER_TIMER };
enum EventTypes { EVENT_KEY_PRESSED, EVENT_PHYSICS, EVENT_GRAPHICS, EVENT_TIMER };
enum EventCodes {
	ENTITY_PHYSICS_FINISHED,
	ENTITY_FINISHED_ANIMATION,
	TIME_TICK,
	TIME_FLICKERING_FINISH,
	KEY_ESC = 27,
	KEY_LEFT = 'a',
	KEY_RIGHT = 'd',
	KEY_DOWN = 's',
	KEY_UP = 'w',
	KEY_SHOOT = 'e',
	COLLISION_WITH_ENEMY,
	COLLISION_WITH_FOOD_LIFE,
	COLLISION_WITH_FOOD_SCORE,
};

