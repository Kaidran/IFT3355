#include "tp1.h"
#include <iostream>
#include <typeinfo>
#include <glm/gtx/string_cast.hpp>

CoreTP1::CoreTP1() : Core(), floor(1, vec4(135.0/255, 206.0/255, 250.0/255, 0.75)), f(0), sky(2, vec4(0.0, 0.0, 1.0, 0.5))
{
	// Initialize view matrix
	_viewMatrix = lookAt(vec3(0, 0, 20), vec3(0, 0, 0), vec3(0, 1, 0));

	// Rescale sky
	sky.SetTransform(scale(mat4(), vec3(110.0f)));

}

void CoreTP1::Render(double dt)
{

	if (!game_over)
	{

		// Clean projectiles and fighters if needed
		clean_scene();

		// Make a fighter appear if needed
		spawn_enemies();

		// Make existing fighters fire
		fire_enemies();

		// Update player properties (acceleration, speed, orientation, ...)
		player.Update(dt);

		// Verify boundaries. take a hit if out of bounds
		if (player.Position.x < -18 || player.Position.x > 18 || player.Position.y < -18 || player.Position.y > 18)
		{
			player_hit();
		}
		auto time = glfwGetTime();
		if (player.input[Player::Input::SPACE])
		{
			// Make the player's ship fire if needed
			// BEGIN CODE HERE
			if (player.last_shot >= player.shot_delay)
			{
				player.last_shot = 0.0;
				auto player_rockets = player.getProjectileSpawnPoint();
				glm::vec3 projectile_speed = vec3(0.0f, 0.0f, -50.0f);
				glm::vec4 projectile_color_out = glm::vec4(0.7f, 0.0f, 0.0f, 0.8f);
				glm::vec4 projectile_color_in = glm::vec4(0.7f, 0.0f, 0.0f, 1.0f);
				for (uint i=0; i<4; i++)
				{
					vec3 rocket(player_rockets[i].x, player_rockets[i].y, player_rockets[i].z);
					active_projectiles.push_back(std::unique_ptr<Projectile>
						(new Projectile
							(rocket, projectile_speed, projectile_color_out, projectile_color_in, true)));
				}
			}

			// END CODE HERE
		}

		// Transform and display floor
		f += float(dt) * 2 * pi<float>() * 0.1f;
		floor.SetTransform(translate(mat4(), vec3(0.0f, -13.0f, 0.0f)) *scale(mat4(), vec3(100.0f, 1.0f, 100.0f)) * rotate(mat4(), -1.0f * f, vec3(1.0f, 0.0f, 0.0f)));
		floor.Render();

		// Display sky
		sky.Render();

		// Make the ship winking during the "peaceful period"
		if (time - start_time > spawn_delay_after_start || time - start_time < 1.0 || time - start_time > 1.5 && time - start_time < 2.0 || time - start_time > 2.5 && time - start_time < 3.0 || time - start_time > 3.5 && time - start_time < 4.0 || time - start_time > 4.5 && time - start_time < 5.0)
			player.Render();

		bool player_shot = false;

		for (auto proj = active_projectiles.begin(); proj != active_projectiles.end();)
		{
			// Update and display projectiles
			// BEGIN CODE HERE

			(*proj)->Update(dt);
			(*proj)->Render();

			// END CODE HERE 		

			bool hit_something = false;


			// If the projectile we consider comes from an enemy
			if (!(*proj)->friendly())
			{

				// Test if the projectile collides with the ship
				// If it does, set hit_something and player_shot to true
				// BEGIN CODE HERE

				vec3 pos_hit = (*proj)->position();
				bool gitgud = player.Intersect(pos_hit);
				if (gitgud)
				{
					auto gitRlygud = player.GetAABB();
					for (uint i=0; i < gitRlygud.size(); i++)
					{
						if ((pos_hit.x >= gitRlygud[i].min.x && pos_hit.x <= gitRlygud[i].max.x) &&
							(pos_hit.y >= gitRlygud[i].min.y && pos_hit.y <= gitRlygud[i].max.y) &&
							(pos_hit.z >= gitRlygud[i].min.z && pos_hit.z <= gitRlygud[i].max.z))
						{
							hit_something = true;
							player_shot = true;
							break;
						}
					}
				}
				//}
				// END CODE HERE

			}
			// If the projectile we consider comes from the player 
			else
			{
				// Test if the projectile collides with an enemy
				// If it does, add 1000 points to player.score, then delete properly the enemy
				// BEGIN CODE HERE
				vec3 pos_hit = (*proj)->position();
				for (auto it = active_fighters.begin(); it != active_fighters.end();)
				{
					bool nailedit = (*it)->Intersect(pos_hit);
					if (nailedit)
					{
						player.score += 1000;
						hit_something = true;
						active_fighters.erase(it);
						break;
					}
					++it;

				}


				// END CODE HERE 
			}

			// If the projectile has hit something
			if (hit_something)
			{
				// Destroy projectile properly
				// BEGIN CODE HERE
				active_projectiles.erase(proj);


				// END CODE HERE 

			}
			else
			{
				++proj;
			}
		}

		// If the player has been shot
		if (player_shot)
		{
			player_hit();
		}

		for (auto it = active_fighters.begin(); it != active_fighters.end(); ++it)
		{
			// Update and display fighters
			// BEGIN CODE HERE
			(*it)->Update(dt);
			(*it)->Render();


			// END CODE HERE 

		}


		if (display_aabb)
		{
			// Clear AABB lines buffer
			_lineVertices.clear();

			// Fill AABB lines buffer for fighters, player and player's component (see AABB related functions ...) and call DrawAABBs 
			// BEGIN CODE HERE
			// global box
			auto player_AABB = player.GetGlobalAABB();
			this->AABB(player_AABB.min, player_AABB.max);

			//parts
			auto player_AABB_list = player.GetAABB();
			for (uint i=0; i < player_AABB_list.size(); i++)
			{
				this->AABB(player_AABB_list[i].min, player_AABB_list[i].max);
			}
			// enemies
			for (auto it = active_fighters.begin(); it != active_fighters.end(); ++it)
			{
				auto enemy_AABB = (*it)->GetGeneralAABB();
				this->AABB(enemy_AABB.min, enemy_AABB.max);
			}
			DrawAABBs();


			// END CODE HERE 
		}


		if (player.god_mode)
		{
			DrawText("INVICIBLE !", vec2(0.5, 0), vec4(1, 1, 0, 1), 16U, ALIGN_CENTER);
		}

		// Display player lifes and score
		DrawGameText(player.lifes, player.score);
	}
	else
	{
		// Draw score when game_over
		DrawEndGameText(player.score);
	}
}

CoreTP1::~CoreTP1()
{
}

void CoreTP1::OnKeyW(bool down)
{
	if (!game_over)
	{
		player.input[Player::Input::UP] = down;
	}
}

void CoreTP1::OnKeyS(bool down)
{
	if (!game_over)
	{
		player.input[Player::Input::DOWN] = down;

	}
}

void CoreTP1::OnKeyA(bool down)
{
	if (!game_over)
	{
		player.input[Player::Input::LEFT] = down;
	}
}

void CoreTP1::OnKeyD(bool down)
{
	if (!game_over)
	{
		player.input[Player::Input::RIGHT] = down;
	}
}

void CoreTP1::OnKeyE(bool down)
{
	if (down && !game_over)
	{
		display_aabb = !display_aabb;
	}
}

void CoreTP1::OnKeySPACE(bool down)
{
	if (!game_over)
	{
		player.input[Player::Input::SPACE] = down;
	}
	else
	{
		game_over = false;
		start_time = glfwGetTime();
		player = Player();
	}
}

void CoreTP1::OnKeyTAB(bool down)
{
	if (down)
	{
		player.god_mode = !player.god_mode;
	}
}

void CoreTP1::DrawEndGameText(int score)
{
	DrawText("Fin de partie!", vec2(0.5, 0.6), vec4(1), 32U, ALIGN_CENTER);
	DrawText((std::string("Pointage: ") + std::to_string(score)).c_str(), vec2(0.5, 0.5), vec4(1), 32U, ALIGN_CENTER);
}

void CoreTP1::DrawGameText(int lives, int score)
{
	DrawText((std::string("Vies: ") + std::to_string(lives)).c_str(), vec2(0.99, 0.01), vec4(1), 16U, ALIGN_RIGHT);
	DrawText((std::string("Pointage: ") + std::to_string(score)).c_str(), vec2(0.01, 0.01), vec4(1), 16U, ALIGN_LEFT);
}

void CoreTP1::spawn_enemies()
{
	auto time = glfwGetTime();
	auto x = time - start_time;

	if (x > spawn_delay_after_start)
	{
		// Spawn an ennemy (at least two types, chosen randomly) in the scene, at a random position
		// BEGIN CODE HERE
		// random chance to spawn enemy with a limit of enemies on screen
		// as opposed to the model who seemed like aiming for fixed time spawning enemies
		int create_choice = rand() %40;
		if (active_fighters.size() < 20 && create_choice == 1) 
		{
			// choose fighter
			int fighter_type = rand() %2;
			// choose position
			float pos_x = 0.01*(rand() %2000);
			float pos_y = 0.01*(rand() %2000);
			// choose quadrant
			int quad = rand() %4;
			float randx = rand()%50;
			float randy = rand()%50;
			if (quad == 0)
			{
				randx = -randx;
				randy = -randy;
			}
			else if (quad == 1)
			{
				pos_x = -pos_x;
				randy = -randy;
			}
			else if (quad == 2)
			{
				pos_y = -pos_y;
				randx = -randx;
			}
			else
			{
				pos_x = -pos_x;
				pos_y = -pos_y;
			}
			if (fighter_type == 0)
			{
				active_fighters.push_back(std::unique_ptr<Entity>(new Fighter1
					(vec3(pos_x,pos_y,-100.0f), vec3(0.0f,0.0f,0.35f), 15.0, vec3(0.0f,0.0f,70.0f))));
			}
			else
			{
				active_fighters.push_back(std::unique_ptr<Entity>(new Fighter2
					(vec3(pos_x,pos_y,-100.0f), vec3(0.0f,0.0f,0.25f), 8.0, vec3(randx,randy,50.0f))));				
			}
		}

		// END CODE HERE 
	}
}

void CoreTP1::fire_enemies()
{
	// Make every active fighter fire if needed
	// BEGIN CODE HERE
		for (auto it = active_fighters.begin(); it != active_fighters.end(); ++it)
		{
			if ((*it)->last_shot > (*it)->rof)
			{
				(*it)->last_shot = 0;
				auto spawn_point = (*it)->GetProjectileSpawnPoint();
				glm::vec4 projectile_color_out = glm::vec4(0.7f, 0.0f, 0.0f, 0.8f);
				glm::vec4 projectile_color_in = glm::vec4(0.7f, 0.0f, 0.0f, 1.0f);

				for (uint i=0; i < spawn_point.size(); i++)
				{
					vec3 spawn_fire(spawn_point[i].x, spawn_point[i].y, spawn_point[i].z);
					active_projectiles.push_back(std::unique_ptr<Projectile>
							(new Projectile
								(spawn_fire, (*it)->projectile_vel, projectile_color_out, projectile_color_in, false)));
				}
			}
			else
				(*it)->last_shot += 0.1;
		}


	// END CODE HERE 
}

void CoreTP1::clean_scene()
{

	// Remove projectiles if needed
	// BEGIN CODE HERE
	for (auto proj = active_projectiles.begin(); proj != active_projectiles.end();)
	{
		vec3 pos = (*proj)->position();
		if (pos.z < -200 || pos.z > 50)
		{
			active_projectiles.erase(proj);
			continue;
		}
		proj++;
	}

	// END CODE HERE 

	// Remove fighters if needed
	// BEGIN CODE HERE
	for (auto it = active_fighters.begin(); it != active_fighters.end();)
	{
		vec3 pos = (*it)->Position;
		if (pos.z > 50)
		{
			player.score -= 1000;
			active_fighters.erase(it);
		}
		else
		{
			it++;
		}
	}


	// END CODE HERE
	
}

void CoreTP1::clear_scene()
{
	// Delete every projectile, every fighter and reset clock to the current time
	// BEGIN CODE HERE
	active_projectiles.clear();
	active_fighters.clear();
	start_time = glfwGetTime();

	// END CODE HERE

}

void CoreTP1::player_hit()
{
	// If god_mode is not activated, decrease players.lifes, clear the scene, reset player position. If player has no lifes left, set game_over to true
	// BEGIN CODE HERE
	if (!player.god_mode)
	{
		int tmp_lifes = player.lifes - 1;
		int tmp_score = player.score;
		clear_scene();
		if (tmp_lifes == 0)
			game_over = true;
		player = Player();
		player.lifes = tmp_lifes;
		player.score = tmp_score;

	}


	// END CODE HERE
}
