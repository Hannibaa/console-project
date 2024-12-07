/*         
			Free movement with parametric coordinate
			Author : Kadda Aoues
			Date	: 18/ 11 / 2024
			Version	: 0.0.1
*/

#pragma once
#include <functional>
#include "ka_Kinematics.h"




namespace physic {

	template<typename Moveble>
	class ParaMove : public MoveInterface0
	{
		using Function = std::function<float(float)>;
		
		Function				_X;
		Function				_Y;
		float					_t;
		float					_speed;

		void set_velocity() = delete;

	public:
		ParaMove(Function X, Function Y, float t0 = 0.f)
			: _X(std::move(X))
			, _Y(std::move(Y))
			, _t(t0)
			, _speed(0.01f)
		{}

		void set_functions(Function Fx, Function Fy)
		{
			_X = std::move(Fx);
			_Y = std::move(Fy);
		}

		void reset(float t0 = 0.f) {
			_t = t0;
		}

		float get_step() const {
			return _t;
		}

		void set_speed(float speed) {
			_speed = speed;
		}

		float get_speed() const {
			return _speed;
		}

		void move(void * _ptr, float Dt) override {

			auto ptr = static_cast<Moveble*>(_ptr);

			fVec2  p = ptr->get_position();
			fVec2  dim = ptr->get_dimension();

			_t += _speed * Dt;
			p.x = _X(_t);
			p.y = _Y(_t);

			math::clamp(p, _space.delate(dim));

			ptr->set_position(p);

		}

	};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//			MOVING OBJECT FROM POINT TO ANOTHER WITH DEFINED SPEED    
// 
//			TODO : complete this movements
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename Movable>
	class MoveAB : public MoveInterface0
	{
		float _Period;
		float _speed;
		float _t;

		fVec2 _A, _B;

	public:
		void set_speed(float speed) {
			_speed = speed;
		}

		void set_point_0(const fVec2& A)
		{
			_A = A;
		}

		void set_point_1(const fVec2& B)
		{
			_B = B;
		}

		void set_period(float period)
		{
			_Period = period;
		}


		void reset(float t0 = 0.f) {
			_t = t0;
		}

		float get_step() const {
			return _t;
		}

		void move(void* ptr, float Dt) override {
			_t += _speed * Dt;
			math::clamp(_t, 0.f, 1.f);
			
			fVec2 p = _A + _t * (_B - _A);
			static_cast<Movable*>(ptr)->set_position(p);
		}
	};
}


