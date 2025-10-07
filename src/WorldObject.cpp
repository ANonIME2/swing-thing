#include "WorldObject.h"

WorldObject::WorldObject(float x, float y, float width, float height) {
	pos = {x, y};
	size = { width, height };
	for (int i = 2; i < 12; i += 3) {
		vertices[i] = 0.0f;
	}
	vertices[0] = pos.x;
	vertices[1] = pos.y;
	vertices[3] = pos.x;
	vertices[4] = pos.y + height;
	vertices[6] = pos.x + width;
	vertices[7] = pos.y + height;
	vertices[9] = pos.x + width;
	vertices[10] = pos.y;

	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 3;
	//indices[3] = 1;
	//indices[4] = 2;
	//indices[5] = 3;
}

