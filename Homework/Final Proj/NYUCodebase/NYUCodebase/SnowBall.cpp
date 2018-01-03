#include "SnowBall.h"
#include "Settings.h"
#define HORIZONTAL_SPEED_LIMIT 0.0018f
#define JUMP_SPEED 0.0022f

SnowBall::SnowBall(unsigned int x, unsigned int y, bool SnowBall)
	: startX(x), startY(y), SnowBallSide(SnowBall) {
	model.SetPosition(startX *0.5, startY*0.5, 0.0f);
	}

const float* SnowBall::GetVertices() const {
	return STATE_VERTICES[0];
}
const float* SnowBall::GetTexture() const {
	return STATE_TEXTURE[0];
}

float SnowBall::GetVelocityY() const {
	return velocityY;
}

void SnowBall::ProcessInput(Uint32 mse) {
	accelerationX = 0.00001f;
	velocityX += accelerationX * mse;
	// Jumping
	velocityY += 0;
	// Appearance
	model.Translate(0.0f, velocityY * mse, 0.0f);	
}

const float SnowBall::STATE_VERTICES[][12] = {
	{ -0.23958333333333334f, 0.2465277777777778f, -0.23958333333333334f, -0.2465277777777778f, 0.23958333333333334f, 0.2465277777777778f, 0.23958333333333334f, 0.2465277777777778f, -0.23958333333333334f, -0.2465277777777778f, 0.23958333333333334f, -0.2465277777777778f },
	{ -0.22916666666666666f, 0.3194444444444444f, -0.22916666666666666f, -0.3194444444444444f, 0.22916666666666666f, 0.3194444444444444f, 0.22916666666666666f, 0.3194444444444444f, -0.22916666666666666f, -0.3194444444444444f, 0.22916666666666666f, -0.3194444444444444f },
	{ -0.23958333333333334f, 0.3194444444444444f, -0.23958333333333334f, -0.3194444444444444f, 0.23958333333333334f, 0.3194444444444444f, 0.23958333333333334f, 0.3194444444444444f, -0.23958333333333334f, -0.3194444444444444f, 0.23958333333333334f, -0.3194444444444444f },
	{ -0.2326388888888889f, 0.3263888888888889f, -0.2326388888888889f, -0.3263888888888889f, 0.2326388888888889f, 0.3263888888888889f, 0.2326388888888889f, 0.3263888888888889f, -0.2326388888888889f, -0.3263888888888889f, 0.2326388888888889f, -0.3263888888888889f },
	{ -0.22916666666666666f, 0.3194444444444444f, -0.22916666666666666f, -0.3194444444444444f, 0.22916666666666666f, 0.3194444444444444f, 0.22916666666666666f, 0.3194444444444444f, -0.22916666666666666f, -0.3194444444444444f, 0.22916666666666666f, -0.3194444444444444f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f },
	{ -0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, 0.3368055555555556f, 0.25f, 0.3368055555555556f, -0.25f, -0.3368055555555556f, 0.25f, -0.3368055555555556f }
};

const float SnowBall::STATE_TEXTURE[][12] = {
	{ 0.712890625f, 0.19140625f, 0.712890625f, 0.330078125f, 0.84765625f, 0.19140625f, 0.84765625f, 0.19140625f, 0.712890625f, 0.330078125f, 0.84765625f, 0.330078125f },
	{ 0.0f, 0.3828125f, 0.0f, 0.5625f, 0.12890625f, 0.3828125f, 0.12890625f, 0.3828125f, 0.0f, 0.5625f, 0.12890625f, 0.5625f },
	{ 0.85546875f, 0.0f, 0.85546875f, 0.1796875f, 0.990234375f, 0.0f, 0.990234375f, 0.0f, 0.85546875f, 0.1796875f, 0.990234375f, 0.1796875f },
	{ 0.85546875f, 0.181640625f, 0.85546875f, 0.365234375f, 0.986328125f, 0.181640625f, 0.986328125f, 0.181640625f, 0.85546875f, 0.365234375f, 0.986328125f, 0.365234375f },
	{ 0.130859375f, 0.3828125f, 0.130859375f, 0.5625f, 0.259765625f, 0.3828125f, 0.259765625f, 0.3828125f, 0.130859375f, 0.5625f, 0.259765625f, 0.5625f },
	{ 0.0f, 0.0f, 0.0f, 0.189453125f, 0.140625f, 0.0f, 0.140625f, 0.0f, 0.0f, 0.189453125f, 0.140625f, 0.189453125f },
	{ 0.142578125f, 0.0f, 0.142578125f, 0.189453125f, 0.283203125f, 0.0f, 0.283203125f, 0.0f, 0.142578125f, 0.189453125f, 0.283203125f, 0.189453125f },
	{ 0.28515625f, 0.0f, 0.28515625f, 0.189453125f, 0.42578125f, 0.0f, 0.42578125f, 0.0f, 0.28515625f, 0.189453125f, 0.42578125f, 0.189453125f },
	{ 0.0f, 0.19140625f, 0.0f, 0.380859375f, 0.140625f, 0.19140625f, 0.140625f, 0.19140625f, 0.0f, 0.380859375f, 0.140625f, 0.380859375f },
	{ 0.142578125f, 0.19140625f, 0.142578125f, 0.380859375f, 0.283203125f, 0.19140625f, 0.283203125f, 0.19140625f, 0.142578125f, 0.380859375f, 0.283203125f, 0.380859375f },
	{ 0.28515625f, 0.19140625f, 0.28515625f, 0.380859375f, 0.42578125f, 0.19140625f, 0.42578125f, 0.19140625f, 0.28515625f, 0.380859375f, 0.42578125f, 0.380859375f },
	{ 0.427734375f, 0.0f, 0.427734375f, 0.189453125f, 0.568359375f, 0.0f, 0.568359375f, 0.0f, 0.427734375f, 0.189453125f, 0.568359375f, 0.189453125f },
	{ 0.5703125f, 0.0f, 0.5703125f, 0.189453125f, 0.7109375f, 0.0f, 0.7109375f, 0.0f, 0.5703125f, 0.189453125f, 0.7109375f, 0.189453125f },
	{ 0.427734375f, 0.19140625f, 0.427734375f, 0.380859375f, 0.568359375f, 0.19140625f, 0.568359375f, 0.19140625f, 0.427734375f, 0.380859375f, 0.568359375f, 0.380859375f },
	{ 0.712890625f, 0.0f, 0.712890625f, 0.189453125f, 0.853515625f, 0.0f, 0.853515625f, 0.0f, 0.712890625f, 0.189453125f, 0.853515625f, 0.189453125f },
	{ 0.5703125f, 0.19140625f, 0.5703125f, 0.380859375f, 0.7109375f, 0.19140625f, 0.7109375f, 0.19140625f, 0.5703125f, 0.380859375f, 0.7109375f, 0.380859375f }
};