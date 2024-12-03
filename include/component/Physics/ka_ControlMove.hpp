#pragma once
#include "ka_Kinematics.h"



namespace physic{


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //     Controled Move    
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename Sprite>
	class ControlMove : public I0Move<fVec2, fRect>
	{
		short key_up{ VK_UP };
		short key_down{ VK_DOWN };
		short key_right{ VK_RIGHT };
		short key_left{ VK_LEFT };

	public:

		void set_controle_keys(short _up, short _down, short _right, short _left)
		{
			key_up = _up;
			key_down = _down;
			key_right = _right;
			key_left = _left;
		}

		void move(void* ptr_, float Dt) override {
			auto ptr = static_cast<Sprite*>(ptr_);
			auto p = ptr->get_position();
			auto d = ptr->get_dimension();

			if (KeyPressed(key_up)) {
				p.y -= _velocity.y * Dt;
			}

			if (KeyPressed(key_left)) {
				p.x -= _velocity.x * Dt;
			}

			if (KeyPressed(key_right)) {
				p.x += _velocity.x * Dt;
			}

			if (KeyPressed(key_down)) {
				p.y += _velocity.y * Dt;
			}


			math::clamp(p, _space.delate(-d.x, -d.y));

			ptr->set_position(p);
		}
	};


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Continues moving with direction control
	//    set speed and box space
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////


}