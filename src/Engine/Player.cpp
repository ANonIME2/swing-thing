#include "Player.h"
#include "Force.h"
#include <iostream>
void Player::jump()
{
	addForce(glm::vec2(0.0f, 10.0f), Impulse);
}
