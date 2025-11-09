#include "Player.h"
#include <iostream>
void Player::jump()
{
	addForce(glm::vec2(0.0f, 10.0f), Impulse);
	std::cout << pos.x << "  "<< pos.y<<std::endl;
}
