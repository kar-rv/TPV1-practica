#include "arrow.h"

arrow::arrow(Rectangle posDim, Vector2D<> vel) : 
	posDim(posDim), vel(vel) {

}

arrow::~arrow() {

}

void
arrow::render() const {

}

bool
arrow::update(float delta) {
	return false;
}
