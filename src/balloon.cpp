#include "balloon.h"

balloon::balloon(Rectangle posDim, Vector2D<> vel, int color) : 
	posDim(posDim), vel(vel), color(color) {

}

balloon::~balloon() {

}

void
balloon::render() const {

}

bool
balloon::update(float delta) {
	return false;
}
