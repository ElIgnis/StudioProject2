#include "Player.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "MeshBuilder.h"
#include "Vertex.h"
#include "LoadTGA.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	position = camera.position;
	modeCustomer = false;
	modeGuard = false;
	modeVillain = false;
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void Player::Update(double dt)
{
	position = camera.position;
}

void Player::Render()
{
	
}
