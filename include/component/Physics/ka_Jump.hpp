/*							Jump physics  
							Aoues Kadda
							23 / 11 / 2024
*/

#pragma once
#include "ka_Kinematics.h"



namespace physic {


	template<typename Movable>
	class Jump : public MoveInterface0
	{
		float		_ground;

	public:
		void set_ground(float g) {
			_ground = g;
		}

		float get_ground() const {
			return _ground;
		}

		void move(void* ptr_, float Dt) override {
			auto ptr = static_cast<Movable*>(ptr_);

			auto p = ptr->get_position();
			auto d = ptr->get_dimension();

			if (KeyPressed(VK_UP)) {
				p.y -= 2.f*_velocity.y * Dt;
			}
			else
			p.y += _velocity.y * Dt;

			if (KeyPressed(VK_LEFT)) {
				p.x -= _velocity.x * Dt;
			}

			if (KeyPressed(VK_RIGHT)) {
				p.x += _velocity.x * Dt;
			}


			p.y = p.y > _ground ? _ground : p.y;

			math::clamp(p, _space.delate(-d.x, -d.y));

			ptr->set_position(p);
		}
	};



}