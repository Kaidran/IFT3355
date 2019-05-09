#include "objects.h"
#include <glm/gtx/transform2.hpp>

#define X_AXIS vec3(1.0f,0.0f,0.0f)
#define Y_AXIS vec3(0.0f,1.0f,0.0f)
#define Z_AXIS vec3(0.0f,0.0f,1.0f)

Projectile::Projectile(const vec3 &position, const vec3 &velocity, const vec4 &color_i, const vec4 &color_o, bool friendly) :
	_inner(1, color_i),
	_outer(1, color_o),
	_position(position),
	_velocity(velocity),
	_friendly(friendly)
{
	AddChild(&_inner);
	_inner.AddChild(&_outer);

	_outer.SetTransform(glm::scale(vec3(2)));
}

void Projectile::Render()
{
	_inner.Render();
	_outer.Render();
}

void Projectile::Update(double dt)
{
	_position += _velocity * decimal(dt);

	SetTransform(glm::translate(_position) * scale(vec3(0.1f)));
}

std::vector<vec3> Entity::GetProjectileSpawnPoint()
{
	return std::vector<vec3>{Position + vec3(0.0f, 0.0f, 0.5f)};
}

Fighter1::Fighter1(const vec3& position, const vec3& velocity, double rate_of_fire, vec3 proj_vel) : Entity(position, velocity, rate_of_fire, proj_vel)
{

	// Build fighter1 (build a hierarchy using primitives and transformations)
	// BEGIN CODE HERE

	Position = position;
	_velocity = velocity;
	rof = rate_of_fire;
	projectile_vel = proj_vel;

	int core_iterations = 3;
	vec4 core_color = vec4(255,0,255,0.5);
	float core_sx = 0.8;
	float core_sy = 0.8;
	float core_sz = 0.8;

	vec4 right_arm_color = vec4(128,0,0,0.5);
	float right_arm_tx = 1.0;
	float right_arm_ty = 0.0;
	float right_arm_tz = 0.0;
	float right_arm_sx = 1.5;
	float right_arm_sy = 0.5;
	float right_arm_sz = 1.5;

	vec4 right_hand_color = vec4(128,0,0,0.5);
	float right_hand_tx = 1.0;
	float right_hand_ty = 0.0;
	float right_hand_tz = 0.0;
	float right_hand_shx = 0.5;
	float right_hand_shy = 0.5;
	float right_hand_rz = glm::radians(-45.0f);
	
	vec4 left_arm_color = vec4(0,0,128,0.5);
	float left_arm_tx = -1.0;
	float left_arm_ty = 0.0;
	float left_arm_tz = 0.0;
	float left_arm_sx = 1.5;
	float left_arm_sy = 0.5;
	float left_arm_sz = 1.5;

	vec4 left_hand_color = vec4(0,0,128,0.5);
	float left_hand_tx = -1.0;
	float left_hand_ty = 0.0;
	float left_hand_tz = 0.0;
	float left_hand_shx = -0.5;
	float left_hand_shy = -0.5;
	float left_hand_rz = glm::radians(45.0f);

	// body
	core.reset(new Sphere(core_iterations, core_color));
	auto core_tr = glm::translate(vec3(position.x, position.y, position.z));
	auto core_sc = glm::scale(vec3(core_sx, core_sy, core_sz));
	core->SetTransform(core_tr*core_sc);
	this->core_trans = core_tr*core_sc;

	// extensions
	right_arm.reset(new Box(right_arm_color));
	core->AddChild(right_arm.get());
	auto right_arm_tr = glm::translate(vec3(right_arm_tx, right_arm_ty, right_arm_tz));
	auto right_arm_sc = glm::scale(vec3(right_arm_sx, right_arm_sy, right_arm_sz));
	right_arm->SetTransform(right_arm_tr*right_arm_sc);

	right_hand.reset(new Pyramid(right_hand_color));
	right_arm->AddChild(right_hand.get());
	auto right_hand_tr = glm::translate(vec3(right_hand_tx, right_hand_ty, right_hand_tz));
	glm::mat4 right_hand_shr;
	right_hand_shr = glm::shearZ3D(right_hand_shr, right_hand_shx, right_hand_shy);
	auto right_hand_rt = glm::rotate(right_hand_rz, Z_AXIS);
	right_hand->SetTransform(right_hand_tr*right_hand_rt*right_hand_shr);

	left_arm.reset(new Box(left_arm_color));
	core->AddChild(left_arm.get());
	auto left_arm_tr = glm::translate(vec3(left_arm_tx, left_arm_ty, left_arm_tz));
	auto left_arm_sc = glm::scale(vec3(left_arm_sx, left_arm_sy, left_arm_sz));
	left_arm->SetTransform(left_arm_tr*left_arm_sc);

	left_hand.reset(new Pyramid(left_hand_color));
	left_arm->AddChild(left_hand.get());
	auto left_hand_tr = glm::translate(vec3(left_hand_tx, left_hand_ty, left_hand_tz));
	glm::mat4 left_hand_shr;
	left_hand_shr = glm::shearZ3D(left_hand_shr, left_hand_shx, left_hand_shy);
	auto left_hand_rt = glm::rotate(left_hand_rz, Z_AXIS);
	left_hand->SetTransform(left_hand_tr*left_hand_rt*left_hand_shr);

	// END CODE HERE 

}

void Fighter1::Render()
{
	// Render fighter1
	// BEGIN CODE HERE
	core->Render();
	right_arm->Render();
	right_hand->Render();
	left_arm->Render();
	left_hand->Render();

	// END CODE HERE 

}

void Fighter1::Update(double dt)
{
	// Update fighter1's position
	// BEGIN CODE HERE
	Position += _velocity;
	auto update_trans = glm::translate(_velocity);
	auto update_rot = glm::rotate(glm::radians(10.0f), Z_AXIS);
	core_trans = core_trans*update_trans*update_rot;
	core->SetTransform(core_trans);


	// END CODE HERE 

}

Fighter2::Fighter2(const vec3& position, const vec3& velocity, double rate_of_fire, vec3 proj_vel) : Entity(position, velocity, rate_of_fire, proj_vel)
{

	// Build fighter2 (build a hierarchy using primitives and transformations)
	// BEGIN CODE HERE

	Position = position;
	_velocity = velocity;
	rof = rate_of_fire;
	projectile_vel = proj_vel;

	int core_iterations = 3;
	float core_height = 1.0;
	vec4 core_color = vec4(64,0,32,1.0);
	float core_rz = glm::radians(90.0f);

	vec4 hilt_side_color = vec4(0,0,128,0.5);
	float hilt_side_tx = 0.0;
	float hilt_side_ty = 0.0;
	float hilt_side_tz = 1.0;
	float hilt_side_rz = glm::radians(90.0f);
	float hilt_side_sx = 0.5;
	float hilt_side_sy = 4.0;
	float hilt_side_sz = 0.5;

	vec4 hilt_side_sph1_color = vec4(128,64,192,0.5);
	uint hilt_side_sph1_iterations = 2;
	float hilt_side_sph1_tx = 0.0;
	float hilt_side_sph1_ty = 0.5;
	float hilt_side_sph1_tz = 0.0;
	float hilt_side_sph1_sx = 0.5;
	float hilt_side_sph1_sy = 0.25;
	float hilt_side_sph1_sz = 0.5;

	vec4 hilt_side_sph2_color = vec4(128,64,192,0.5);
	uint hilt_side_sph2_iterations = 2;
	float hilt_side_sph2_tx = 0.0;
	float hilt_side_sph2_ty = -0.5;
	float hilt_side_sph2_tz = 0.0;
	float hilt_side_sph2_sx = 0.5;
	float hilt_side_sph2_sy = 0.25;
	float hilt_side_sph2_sz = 0.5;

	vec4 hilt_cross_color = vec4(0,0,128,0.5);
	float hilt_cross_tx = 0.0;
	float hilt_cross_ty = -0.5;
	float hilt_cross_tz = 1.0;
	float hilt_cross_sx = 0.5;
	float hilt_cross_sy = 4.0;
	float hilt_cross_sz = 0.5;

	vec4 hilt_cross_sph1_color = vec4(128,64,192,0.5);
	uint hilt_cross_sph1_iterations = 2;
	float hilt_cross_sph1_tx = 0.0;
	float hilt_cross_sph1_ty = 0.5;
	float hilt_cross_sph1_tz = 0.0;
	float hilt_cross_sph1_sx = 0.5;
	float hilt_cross_sph1_sy = 0.25;
	float hilt_cross_sph1_sz = 0.5;

	vec4 hilt_cross_sph2_color = vec4(128,64,192,0.5);
	uint hilt_cross_sph2_iterations = 2;
	float hilt_cross_sph2_tx = 0.0;
	float hilt_cross_sph2_ty = -0.5;
	float hilt_cross_sph2_tz = 0.0;
	float hilt_cross_sph2_sx = 0.5;
	float hilt_cross_sph2_sy = 0.25;
	float hilt_cross_sph2_sz = 0.5;

	vec4 point_color = vec4(128,0,0,1.0);
	float point_tx = 0.0;
	float point_ty = 0.0;
	float point_tz = 1.0;
	float point_sx = 1.0;
	float point_sy = 0.25;
	float point_sz = 1.0;
	
	// body
	core.reset(new Cylinder(core_iterations, core_color, core_height));
	auto core_tr = glm::translate(vec3(position.x, position.y, position.z));
	auto core_rtz = glm::rotate(core_rz, Z_AXIS);
	this->core_trans = core_tr*core_rtz;
	core->SetTransform(core_trans);

	// cross-shape by making two boxes
	hilt_side.reset(new Box(hilt_side_color));
	core->AddChild(hilt_side.get());
	auto hilt_side_tr = glm::translate(vec3(hilt_side_tx, hilt_side_ty, hilt_side_tz));
	auto hilt_side_rtz = glm::rotate(hilt_side_rz, Z_AXIS);
	auto hilt_side_sc = glm::scale(vec3(hilt_side_sx, hilt_side_sy, hilt_side_sz));
	hilt_side->SetTransform(hilt_side_tr*hilt_side_rtz*hilt_side_sc);

	hilt_cross.reset(new Box(hilt_cross_color));
	core->AddChild(hilt_cross.get());
	auto hilt_cross_tr = glm::translate(vec3(hilt_cross_tx, hilt_cross_ty, hilt_cross_tz));
	auto hilt_cross_sc = glm::scale(vec3(hilt_cross_sx, hilt_cross_sy, hilt_cross_sz));
	hilt_cross->SetTransform(hilt_cross_tr*hilt_cross_sc);

	// the four sided hilt "ornaments"

	hilt_side_sph1.reset(new Sphere(hilt_side_sph1_iterations, hilt_side_sph1_color));
	hilt_side->AddChild(hilt_side_sph1.get());
	auto hilt_side_sph1_tr = glm::translate(vec3(hilt_side_sph1_tx, hilt_side_sph1_ty, hilt_side_sph1_tz));
	auto hilt_side_sph1_sc = glm::scale(vec3(hilt_side_sph1_sx, hilt_side_sph1_sy, hilt_side_sph1_sz));
	hilt_side_sph1->SetTransform(hilt_side_sph1_tr*hilt_side_sph1_sc);

	hilt_side_sph2.reset(new Sphere(hilt_side_sph2_iterations, hilt_side_sph2_color));
	hilt_side->AddChild(hilt_side_sph2.get());
	auto hilt_side_sph2_tr = glm::translate(vec3(hilt_side_sph2_tx, hilt_side_sph2_ty, hilt_side_sph2_tz));
	auto hilt_side_sph2_sc = glm::scale(vec3(hilt_side_sph2_sx, hilt_side_sph2_sy, hilt_side_sph2_sz));
	hilt_side_sph2->SetTransform(hilt_side_sph2_tr*hilt_side_sph2_sc);

	hilt_cross_sph1.reset(new Sphere(hilt_cross_sph1_iterations, hilt_cross_sph1_color));
	hilt_cross->AddChild(hilt_cross_sph1.get());
	auto hilt_cross_sph1_tr = glm::translate(vec3(hilt_cross_sph1_tx, hilt_cross_sph1_ty, hilt_cross_sph1_tz));
	auto hilt_cross_sph1_sc = glm::scale(vec3(hilt_cross_sph1_sx, hilt_cross_sph1_sy, hilt_cross_sph1_sz));
	hilt_cross_sph1->SetTransform(hilt_cross_sph1_tr*hilt_cross_sph1_sc);

	hilt_cross_sph2.reset(new Sphere(hilt_cross_sph2_iterations, hilt_cross_sph2_color));
	hilt_cross->AddChild(hilt_cross_sph2.get());
	auto hilt_cross_sph2_tr = glm::translate(vec3(hilt_cross_sph2_tx, hilt_cross_sph2_ty, hilt_cross_sph2_tz));
	auto hilt_cross_sph2_sc = glm::scale(vec3(hilt_cross_sph2_sx, hilt_cross_sph2_sy, hilt_cross_sph2_sz));
	hilt_cross_sph2->SetTransform(hilt_cross_sph2_tr*hilt_cross_sph2_sc);

	// point shape is arbitrarily child to hilt_cross
	// TODO :: fix point facing direction for cool looking rotation
	point.reset(new Pyramid(point_color));
	hilt_cross->AddChild(point.get());
	auto point_tr = glm::translate(vec3(point_tx, point_ty, point_tz));
	auto point_rty = glm::inverse(hilt_side_rtz);
	auto point_rtz = glm::inverse(core_rtz);
	auto point_sc = glm::scale(vec3(point_sx, point_sy, point_sz));
	point->SetTransform(point_rtz*point_rty*point_tr*point_sc);

	// END CODE HERE 

}

void Fighter2::Render()
{
	// Render fighter2
	// BEGIN CODE HERE
	core->Render();
	hilt_side->Render();
	hilt_cross->Render();
	point->Render();
	hilt_side_sph1->Render();
	hilt_side_sph2->Render();
	hilt_cross_sph1->Render();
	hilt_cross_sph2->Render();
	// END CODE HERE 

}

void Fighter2::Update(double dt)
{
	// Update fighter2's position
	// BEGIN CODE HERE
	Position += _velocity;
	auto update_trans = glm::translate(_velocity);
	auto update_rot = glm::rotate(glm::radians(10.0f), Z_AXIS);
	core_trans = core_trans*update_trans*update_rot;
	core->SetTransform(core_trans);


	// END CODE HERE 

}
