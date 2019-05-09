#include "player.h"
#include <glm/ext.hpp>

#define X_AXIS vec3(1.0f,0.0f,0.0f)
#define Y_AXIS vec3(0.0f,1.0f,0.0f)
#define Z_AXIS vec3(0.0f,0.0f,1.0f)

Player::Player()
{
	std::srand(std::time(0));

	// Build player's ship (build a hierarchy using primitives and transformations)
	// BEGIN CODE HERE
	input[Input::UP] = false;
	input[Input::DOWN] = false;
	input[Input::LEFT] = false;
	input[Input::RIGHT] = false;
	input[Input::SPACE] = false;
	//base params. Every part of the ship is proportional to those
	vec4 core_color = vec4(128,128,0,1.0);
	float core_rx = glm::radians(-90.0f);
	float core_ry = glm::radians(45.0f);
	float core_sx = 1.0;
	float core_sy = 2.0;
	float core_sz = 1.0;

	vec4 back_core_color = vec4(256,0,0,1.0);
	float back_core_height = 1.0;
	uint back_core_iterate = 20;
	float back_core_sx = 0.75;
	float back_core_sy = 1.0;
	float back_core_sz = 0.5;
	float back_core_tx = 0.0;
	float back_core_ty = 0.0;
	float back_core_tz = -1.0;

	vec4 right_aileron_color = vec4(0,0,256,1.0);
	float right_aileron_tx = -0.375;
	float right_aileron_ty = -0.5;
	float right_aileron_tz = 0.1;
	float right_aileron_sx = 0.5;
	float right_aileron_sy = 0.15;
	float right_aileron_sz = 0.25;
	float right_aileron_rz = glm::radians(60.0f);
	float right_aileron_shx = 0.2;
	float right_aileron_shy = 0.2;

	vec4 left_aileron_color = vec4(256,256,256,1.0);
	float left_aileron_tx = -0.375;
	float left_aileron_ty = 0.5;
	float left_aileron_tz = 0.1;
	float left_aileron_sx = 0.5;
	float left_aileron_sy = 0.15;
	float left_aileron_sz = 0.25;
	float left_aileron_rz = glm::radians(-60.0f);
	float left_aileron_shx = -0.2;
	float left_aileron_shy = 0.2;

	vec4 right_wing_color = vec4(0,256,0,1.0);
	float right_wing_tx = 0.33;
	float right_wing_ty = 1.0;
	float right_wing_tz = -0.2;
	float right_wing_rz = glm::radians(-20.0f);
	float right_wing_sx = 0.3;
	float right_wing_sy = 1.0;
	float right_wing_sz = 0.5;
	float right_wing_shx = 0.1;
	float right_wing_shy = 0.1;

	vec4 right_rocket_color = vec4(256,256,256,1.0);
	int right_rocket_iterate = 20;
	float right_rocket_height = 2.0;
	float right_rocket_tx = 2.75;
	float right_rocket_ty = -3.0;
	float right_rocket_tz = -1.0;
	float right_rocket_sx = 3.33/4;
	float right_rocket_sy = 1.0/4;
	float right_rocket_sz = 1.0;

	vec4 right_rocket_fire_color = vec4(255,0,0,0.5);
	int right_rocket_fire_iterate = 30;
	float right_rocket_fire_height = 1.0;
	float right_rocket_fire_tx = 0.0;
	float right_rocket_fire_ty = 0.0;
	float right_rocket_fire_tz = -1.0;
	float right_rocket_fire_sx = 1.0;
	float right_rocket_fire_sy = 1.0;
	float right_rocket_fire_sz = 1.0;

	vec4 right_2_rocket_color = vec4(128,128,128,1.0);
	int right_2_rocket_iterate = 20;
	float right_2_rocket_height = 2.0;
	float right_2_rocket_tx = 1.9;
	float right_2_rocket_ty = -2.25;
	float right_2_rocket_tz = -1.0;
	float right_2_rocket_sx = 3.33/4;
	float right_2_rocket_sy = 1.0/4;
	float right_2_rocket_sz = 1.0;

	vec4 right_2_rocket_fire_color = vec4(255,0,0,0.5);
	int right_2_rocket_fire_iterate = 30;
	float right_2_rocket_fire_height = 1.0;
	float right_2_rocket_fire_tx = 0.0;
	float right_2_rocket_fire_ty = 0.0;
	float right_2_rocket_fire_tz = -1.0;
	float right_2_rocket_fire_sx = 1.0;
	float right_2_rocket_fire_sy = 1.0;
	float right_2_rocket_fire_sz = 1.0;

	vec4 left_wing_color = vec4(0,256,0,1.0);
	float left_wing_tx = right_wing_tx;
	float left_wing_ty = -right_wing_ty;
	float left_wing_tz = right_wing_tz;
	float left_wing_rz = glm::radians(20.0f);
	float left_wing_sx = right_wing_sx;
	float left_wing_sy = right_wing_sy;
	float left_wing_sz = right_wing_sz;
	float left_wing_shx = right_wing_shx;
	float left_wing_shy = right_wing_shy;

	vec4 left_rocket_color = vec4(256,256,256,1.0);
	int left_rocket_iterate = 20;
	float left_rocket_height = 2.0;
	float left_rocket_tx = 2.75;
	float left_rocket_ty = 3.0;
	float left_rocket_tz = -1.0;
	float left_rocket_sx = 3.33/4;
	float left_rocket_sy = 1.0/4;
	float left_rocket_sz = 1.0;

	vec4 left_rocket_fire_color = vec4(256,0,0,0.5);
	int left_rocket_fire_iterate = 30;
	float left_rocket_fire_height = 1.0;
	float left_rocket_fire_tx = 0.0;
	float left_rocket_fire_ty = 0.0;
	float left_rocket_fire_tz = -1.0;
	float left_rocket_fire_sx = 1.0;
	float left_rocket_fire_sy = 1.0;
	float left_rocket_fire_sz = 1.0;

	vec4 left_2_rocket_color = vec4(256,256,256,1.0);
	int left_2_rocket_iterate = 20;
	float left_2_rocket_height = 2.0;
	float left_2_rocket_tx = 1.9;
	float left_2_rocket_ty = 2.25;
	float left_2_rocket_tz = -1.0;
	float left_2_rocket_sx = 3.33/4;
	float left_2_rocket_sy = 1.0/4;
	float left_2_rocket_sz = 1.0;

	vec4 left_2_rocket_fire_color = vec4(256,0,0,0.5);
	int left_2_rocket_fire_iterate = 30;
	float left_2_rocket_fire_height = 1.0;
	float left_2_rocket_fire_tx = 0.0;
	float left_2_rocket_fire_ty = 0.0;
	float left_2_rocket_fire_tz = -1.0;
	float left_2_rocket_fire_sx = 1.0;
	float left_2_rocket_fire_sy = 1.0;
	float left_2_rocket_fire_sz = 1.0;
	// inverse, translate, rotate, scale, shear
	//core
	core.reset(new Pyramid(core_color));
	auto core_rtx = glm::rotate(core_rx, X_AXIS);
	auto core_rty = glm::rotate(core_ry, Y_AXIS);
	auto core_sc = glm::scale(vec3(core_sx, core_sy, core_sz));
	auto core_t = core_rtx * core_rty * core_sc;
	core->SetTransform(core_t);
	this->core_trans = core_t;
	this->inv_core_trans = glm::inverse(core_t);

	//backcore
	back_core.reset(new Cylinder(back_core_iterate, back_core_color, back_core_height));
	core->AddChild(back_core.get());
	//Undo core rotation
	auto core_irtx = -core_rtx;
	auto core_irty = -core_rty;
	auto back_core_tr = glm::translate(vec3(back_core_tx, back_core_ty, back_core_tz));	
	auto back_core_sc = glm::scale(vec3(back_core_sx, back_core_sy, back_core_sz));
	back_core->SetTransform(core_irty * core_irtx * back_core_tr * back_core_sc);

	// back-rightwing
	right_aileron.reset(new Box(right_aileron_color));
	back_core->AddChild(right_aileron.get());
	auto right_aileron_tr = glm::translate(vec3(right_aileron_tx, right_aileron_ty, right_aileron_tz));
	auto right_aileron_rtz = glm::rotate(right_aileron_rz, Z_AXIS);
	auto right_aileron_sc = glm::scale(vec3(right_aileron_sx, right_aileron_sy, right_aileron_sz));
	glm::mat4 right_aileron_shrz;
	right_aileron_shrz = glm::shearZ3D(right_aileron_shrz, right_aileron_shx, right_aileron_shy);
	right_aileron->SetTransform(right_aileron_tr*right_aileron_rtz*right_aileron_sc*right_aileron_shrz);

	// back-leftwing
	left_aileron.reset(new Box(left_aileron_color));
	back_core->AddChild(left_aileron.get());
	auto left_aileron_tr = glm::translate(vec3(left_aileron_tx, left_aileron_ty, left_aileron_tz));
	auto left_aileron_rtz = glm::rotate(left_aileron_rz, Z_AXIS);
	auto left_aileron_sc = glm::scale(vec3(left_aileron_sx, left_aileron_sy, left_aileron_sz));
	glm::mat4 left_aileron_shrz;
	left_aileron_shrz = glm::shearZ3D(left_aileron_shrz, left_aileron_shx, left_aileron_shy);
	left_aileron->SetTransform(left_aileron_tr*left_aileron_rtz*left_aileron_sc*left_aileron_shrz);

	// rightwing
	right_wing.reset(new Box(right_wing_color));
	core->AddChild(right_wing.get());
	auto right_wing_tr = glm::translate(vec3(right_wing_tx,right_wing_ty,right_wing_tz));
	auto right_wing_rtz = glm::rotate(right_wing_rz, Z_AXIS);
	auto right_wing_sc = glm::scale(vec3(right_wing_sx,right_wing_sy,right_wing_sz));
	glm::mat4 right_wing_shrz;
	right_wing_shrz = glm::shearZ3D(right_wing_shrz, right_wing_shx, right_wing_shy);
	right_wing->SetTransform(core_irty*core_irtx*right_wing_tr*right_wing_rtz*right_wing_sc*right_wing_shrz);

	// inverses for rockets
	glm::mat4 right_wing_ishrz;
	right_wing_ishrz = glm::shearZ3D(right_wing_ishrz, right_wing_shx*(-1), right_wing_shy*(-1));
	auto right_wing_irtz = glm::rotate(right_wing_rz*(-1), Z_AXIS);

	// right-rightrocket
	right_rocket.reset(new Cylinder(right_rocket_iterate, right_rocket_color, right_rocket_height));
	right_wing->AddChild(right_rocket.get());
	auto right_rocket_tr = glm::translate(vec3(right_rocket_tx, right_rocket_ty, right_rocket_tz));
	auto right_rocket_sc = glm::scale(right_rocket_sx, right_rocket_sy, right_rocket_sz);
	auto right_rtrans = right_wing_irtz*right_wing_ishrz*right_rocket_tr*right_rocket_sc;
	right_rocket->SetTransform(right_rtrans);
	this->right_rocket_trans = right_rtrans;

	// right-rightfire
	right_rocket_fire.reset(new Cylinder(right_rocket_fire_iterate, 
		                                  right_rocket_fire_color, right_rocket_fire_height));
	right_rocket->AddChild(right_rocket_fire.get());
	auto right_rocket_fire_tr = glm::translate(vec3(right_rocket_fire_tx, right_rocket_fire_ty, right_rocket_fire_tz));
	auto right_rocket_fire_sc = glm::scale(vec3(right_rocket_fire_sx, right_rocket_fire_sy, right_rocket_fire_sz));
	right_rocket_fire->SetTransform(right_rocket_fire_tr*right_rocket_fire_sc);

	// right-leftrocket
	right_2_rocket.reset(new Cylinder(right_2_rocket_iterate, right_2_rocket_color, right_2_rocket_height));
	right_wing->AddChild(right_2_rocket.get());
	auto right_2_rocket_tr = glm::translate(vec3(right_2_rocket_tx, right_2_rocket_ty, right_2_rocket_tz));
	auto right_2_rocket_sc = glm::scale(right_2_rocket_sx, right_2_rocket_sy, right_2_rocket_sz);
	auto right_2_rtrans = right_wing_irtz*right_wing_ishrz*right_2_rocket_tr*right_2_rocket_sc;
	right_2_rocket->SetTransform(right_2_rtrans);
	this->right_2_rocket_trans = right_2_rtrans;

	// right-leftfire
	right_2_rocket_fire.reset(new Cylinder(right_2_rocket_fire_iterate, 
		                                  right_2_rocket_fire_color, right_2_rocket_fire_height));
	right_2_rocket->AddChild(right_2_rocket_fire.get());
	auto right_2_rocket_fire_tr = glm::translate(vec3(right_2_rocket_fire_tx, right_2_rocket_fire_ty, right_2_rocket_fire_tz));
	auto right_2_rocket_fire_sc = glm::scale(vec3(right_2_rocket_fire_sx, right_2_rocket_fire_sy, right_2_rocket_fire_sz));
	right_2_rocket_fire->SetTransform(right_2_rocket_fire_tr*right_2_rocket_fire_sc);

	// leftwing
	left_wing.reset(new Box(left_wing_color));
	core->AddChild(left_wing.get());
	auto left_wing_tr = glm::translate(vec3(left_wing_tx, left_wing_ty, left_wing_tz));
	auto left_wing_rtz = glm::rotate(left_wing_rz, Z_AXIS);
	auto left_wing_sc = glm::scale(vec3(left_wing_sx, left_wing_sy, left_wing_sz));
	glm::mat4 left_wing_shrz;
	left_wing_shrz = glm::shearZ3D(left_wing_shrz, left_wing_shx,left_wing_shy);
	left_wing->SetTransform(core_irty*core_irtx*left_wing_tr*left_wing_rtz*left_wing_sc*left_wing_shrz);
	// inverses for rockets
	glm::mat4 left_wing_ishrz;
	left_wing_ishrz = glm::shearZ3D(left_wing_ishrz, left_wing_shx*(-1), left_wing_shy*(-1));
	auto left_wing_irtz = glm::rotate(left_wing_rz*(-1), Z_AXIS);

	// left-leftrocket
	left_rocket.reset(new Cylinder(left_rocket_iterate, left_rocket_color, left_rocket_height));
	left_wing->AddChild(left_rocket.get());
	auto left_rocket_tr = glm::translate(vec3(left_rocket_tx, left_rocket_ty, left_rocket_tz));
	auto left_rocket_sc = glm::scale(left_rocket_sx, left_rocket_sy, left_rocket_sz);
	auto left_rtrans = left_wing_irtz*left_wing_ishrz*left_rocket_tr*left_rocket_sc;
	left_rocket->SetTransform(left_rtrans);
	this->left_rocket_trans = left_rtrans;

	// left-leftfire
	left_rocket_fire.reset(new Cylinder(left_rocket_fire_iterate, 
		                                  left_rocket_fire_color, left_rocket_fire_height));
	left_rocket->AddChild(left_rocket_fire.get());
	auto left_rocket_fire_tr = glm::translate(vec3(left_rocket_fire_tx, left_rocket_fire_ty, left_rocket_fire_tz));
	auto left_rocket_fire_sc = glm::scale(vec3(left_rocket_fire_sx, left_rocket_fire_sy, left_rocket_fire_sz));
	left_rocket_fire->SetTransform(left_rocket_fire_tr*left_rocket_fire_sc);

	// left-rightrocket
	left_2_rocket.reset(new Cylinder(left_2_rocket_iterate, left_2_rocket_color, left_2_rocket_height));
	left_wing->AddChild(left_2_rocket.get());
	auto left_2_rocket_tr = glm::translate(vec3(left_2_rocket_tx, left_2_rocket_ty, left_2_rocket_tz));
	auto left_2_rocket_sc = glm::scale(left_2_rocket_sx, left_2_rocket_sy, left_2_rocket_sz);
	auto left_2_rtrans = left_wing_irtz*left_wing_ishrz*left_2_rocket_tr*left_2_rocket_sc;
	left_2_rocket->SetTransform(left_2_rtrans);
	this->left_2_rocket_trans = left_2_rtrans;

	// left-rightfire
	left_2_rocket_fire.reset(new Cylinder(left_2_rocket_fire_iterate, 
		                                  left_2_rocket_fire_color, left_2_rocket_fire_height));
	left_2_rocket->AddChild(left_2_rocket_fire.get());
	auto left_2_rocket_fire_tr = glm::translate(vec3(left_2_rocket_fire_tx, left_2_rocket_fire_ty, left_2_rocket_fire_tz));
	auto left_2_rocket_fire_sc = glm::scale(vec3(left_2_rocket_fire_sx, left_2_rocket_fire_sy, left_2_rocket_fire_sz));
	auto left_2_rocket_fire_trans = left_2_rocket_fire_tr*left_2_rocket_fire_sc;
	left_2_rocket_fire->SetTransform(left_2_rocket_fire_trans);

	// END CODE HERE 

}

void Player::Render()
{
	// Render player's ship
	// BEGIN CODE HERE
	core->Render();
	back_core->Render();
	right_wing->Render();
	left_wing->Render();
	right_aileron->Render();
	left_aileron->Render();
	left_rocket->Render();
	left_rocket_fire->Render();
	left_2_rocket->Render();
	left_2_rocket_fire->Render();
	right_rocket->Render();
	right_rocket_fire->Render();
	right_2_rocket->Render();
	right_2_rocket_fire->Render();
	// END CODE HERE 

}

void Player::Update(double dt)
{
	accel = vec2();

	// Modify accel vector according to input[x], with x in {DOWN,UP,LEFT,RIGHT} 
	// BEGIN CODE HERE
	if (input[UP])
	{
		accel[1] += accel_value;
	}
	if (input[LEFT])
	{
		accel[0] -= accel_value;
	}
	if (input[DOWN])
	{
		accel[1] -= accel_value;
	}
	if (input[RIGHT])
	{
		accel[0] += accel_value;
	}

	// END CODE HERE 

	// Update speed, position and rotation
	// BEGIN CODE HERE
	speed += accel;
	speed.x = glm::max(-max_speed, speed.x);
	speed.x = glm::min(max_speed, speed.x);
	speed.y = glm::max(-max_speed, speed.y);
	speed.y = glm::min(max_speed, speed.y);
	
	vec3 vpos(speed, 0);
	//TODO :: position update is wrong
	Position = {vpos};
	float rot_x;
	float x_angle = rotation_coeff*speed[1];
	if (x_angle < 0)
		rot_x = glm::min(max_x_rotation, x_angle);
	else
		rot_x = glm::max(-max_x_rotation, x_angle);
	float rot_z;
	float z_angle = rotation_coeff*speed[0];
	if (z_angle < 0)
		rot_z = glm::min(max_z_rotation, z_angle);
	else
		rot_z = glm::max(-max_z_rotation, z_angle);

	// END CODE HERE 

	// Apply transformation
	// BEGIN CODE HERE
	auto translate_update = glm::translate(vpos);
	auto rotate_x_update = glm::rotate(glm::radians(rot_x), X_AXIS);
	auto rotate_z_update = glm::rotate(glm::radians(-rot_z), Z_AXIS);
	core->SetTransform(translate_update*rotate_x_update*rotate_z_update*core_trans);
	// END CODE HERE

	// Modify rocket fire length
	// BEGIN CODE HERE
	float speed_fire_value = 2*glm::max(abs(accel[0]), abs(accel[1]));
	float random = rand()%20*0.02+0.60 + 2*speed_fire_value;
	auto translate_fire_update = glm::translate(vec3(0.0f, 0.0f,-random));
	auto scale_fire_update = glm::scale(vec3(1.0f,1.0f,random));

	left_2_rocket_fire->SetTransform(translate_fire_update*scale_fire_update);
	left_rocket_fire->SetTransform(translate_fire_update*scale_fire_update);
	right_rocket_fire->SetTransform(translate_fire_update*scale_fire_update);
	right_2_rocket_fire->SetTransform(translate_fire_update*scale_fire_update);



	// END CODE HERE

	// Modify last time fired
	// BEGIN CODE HERE
	last_shot += dt;
	// END CODE HERE
}

// TODO :: Bug with left wing, rocket-2 and rocket-1 height inversed
// check player.cpp implementation to fix
std::vector<glm::vec3> Player::getProjectileSpawnPoint() const
{
	std::vector<vec3> results;

	// Fill the vector with the ship's projectile spawn positions
	// BEGIN CODE HERE
	vec4 v(Position,0.0f);
	vec4 u(1.0f,1.0f,1.0f,1.0f);
	vec4 rocket1 = v+u*right_rocket_trans;
	vec4 rocket2 = v-u*right_2_rocket_trans;
	vec4 rocket3 = v-u*left_2_rocket_trans;
	vec4 rocket4 = v+u*left_rocket_trans;
	vec3 send_back1(rocket1.x, rocket1.y, rocket1.z);
	vec3 send_back2(rocket2.x, rocket2.y, rocket2.z);
	vec3 send_back3(rocket3.x, rocket3.y, rocket3.z);
	vec3 send_back4(rocket4.x, rocket4.y, rocket4.z);
	results.push_back(send_back1);
	results.push_back(send_back2);
	results.push_back(send_back3);
	results.push_back(send_back4);
	// END CODE HERE

	return results;
}

AABB Player::GetGlobalAABB() const
{
	return core->GetGeneralAABB();
}

std::vector<AABB> Player::GetAABB() const
{
	return core->GetAABBList();
}

bool Player::Intersect(vec3 world_pos) const
{
	return core->Intersect(world_pos);
}
