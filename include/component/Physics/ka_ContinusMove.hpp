/*
						Coninus movement and use only direction
						Author : Kadda Aoues
						Date	: 6 / 12 / 2024
						Version : 0.0.1
						Discription : 

						TODO :
						------
						1. Class with full rang of option

*/
#pragma once
#include "ka_Kinematics.h"




namespace physic {

	template<typename T>
	class ContinusMove : public MoveInterface0 {

		const int KonsoleW = konsole->width();
		const int KonsoleH = konsole->hight();

		bool b_up{ false };
		bool b_left{ false };
		bool b_right{ true };
		bool b_down{ false };
		bool b_noreturn{ false };

		void all_false() {
			b_up = false;
			b_left = false;
			b_right = false;
			b_down = false;
		}

		short _left{ VK_LEFT }, _down{ VK_DOWN }, _right{ VK_RIGHT }, _up{ VK_UP };
		short _sequeez{ _u('H') };
		const short _direction[4]{ _left, _up, _right, _down };
		int		_active{ 2 };

	public:
		void set_control(char left, char down, char right, char up) {
			_left = _u(left);
			_down = _u(down);
			_right = _u(right);
			_up = _u(up);
		}

		void noreturn(bool b) {
			b_noreturn = b;
		}

		void set_sequeezKey(char s) {
			_sequeez = _u(s);
		}

		void sequeez() {
			if (b_left) b_left = false;
			if (b_right) b_right = false;
			if (b_up) b_up = false;
			if (b_down) b_down = false;
		}

		short get_direction() const {
			return _direction[_active];
		}

		void move(void *ptr, float Dt) override
		{
			auto _ptr = KA_CAST(T*, ptr);
			auto p = _ptr->get_position();

			if (KeyReleased(_sequeez)) {
				all_false();
			}

			if (KeyReleased(_up)) {
				if (b_noreturn && b_down);
				else
				{
					all_false();
					b_up = true;
					_active = 1;
				}
			}

			if (KeyReleased(_left)) {
				if (b_noreturn && b_right);
				else
				{
					all_false();
					b_left = true;
					_active = 0;
				}
			}

			if (KeyReleased(_right)) {
				if (b_noreturn && b_left);
				else
				{
					all_false();
					b_right = true;
					_active = 2;
				}
			}

			if (KeyReleased(_down)) {
				if (b_noreturn && b_up);
				else
				{
					all_false();
					b_down = true;
					_active = 3;
				}
			}

			if (b_up) {
				p.y -= _velocity.y * Dt;
			}

			if (b_left) {
				p.x -= _velocity.x * Dt;
			}

			if (b_right) {
				p.x += _velocity.x * Dt;
			}

			if (b_down) {
				p.y += _velocity.y * Dt;
			}

			math::clamp<float>(p.x, 0.f, KonsoleW); //konsole->width());
			math::clamp<float>(p.y, 0.f, KonsoleH); //konsole->hight());

			_ptr->set_position(p);
		}

	};


} // End Physic namespace