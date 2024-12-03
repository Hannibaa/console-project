#pragma once
#include "ka_Kinematics.h"


namespace physic {


	// ------------------------------------- //
// Free Move							 //
// ------------------------------------- //
	class FreeMove1 : public MoveInterface1
	{
		float vx = 1.f;
		float vy = 1.f;

	public:

		void move(console::ITransformable<float>* pM, float Dt) override
		{
			auto p = pM->get_position();
			auto d = pM->get_dimension();

			p.x += _velocity.x * Dt * vx;
			p.y += _velocity.y * Dt * vy;

			if (p.x < _space.x || p.x > _space.x + _space.dx - d.x + 2.f) vx *= -1.f;
			if (p.y < _space.y || p.y > _space.y + _space.dy - d.y + 2.f) vy *= -1.f;

			math::clamp(p.x, _space.x, _space.x + _space.dx);
			math::clamp(p.y, _space.y, _space.y + _space.dy);

			pM->set_position(p);

		}
	};

	template<typename T>
	class FreeMove2 : public MoveInterface2 {

		float vx = 1.f;
		float vy = 1.f;

	public:

		using value_type = T;

		void move(void* ptr, float Dt) override
		{
			auto p = static_cast<value_type*>(ptr)->get_position();
			auto d = static_cast<value_type*>(ptr)->get_dimension();

			p.x += _velocity.x * Dt * vx;
			p.y += _velocity.y * Dt * vy;

			if (p.x < _space.x || p.x > _space.x + _space.dx - d.x + 2.f) vx *= -1.f;
			if (p.y < _space.y || p.y > _space.y + _space.dy - d.y + 2.f) vy *= -1.f;

			static_cast<value_type*>(ptr)->set_position(p);
		}
	};

	template<typename T>
	class FreeMove0 : public MoveInterface0
	{

		float vx = 1.f;
		float vy = 1.f;

	public:

		using value_type = T;

		void move(void* ptr, float Dt) override
		{
			auto p = static_cast<value_type*>(ptr)->get_position();
			auto d = static_cast<value_type*>(ptr)->get_dimension();

			p.x += _velocity.x * Dt * vx;
			p.y += _velocity.y * Dt * vy;

			if (p.x < _space.x || p.x > _space.x + _space.dx - d.x + 2.f) vx *= -1.f;
			if (p.y < _space.y || p.y > _space.y + _space.dy - d.y + 2.f) vy *= -1.f;

			static_cast<value_type*>(ptr)->set_position(p);
		}
	};


	template<typename Movable>
	class Move2p : public MoveInterface0
	{
		fVec2 pt1;
		fVec2 pt2;

		float _t{};

	public:
		
		void set_2points(const fVec2& _p1, const fVec2& _p2) {
			pt1 = _p1;
			pt2 = _p2;
		}

		void move(void* ptr, float Dt) override
		{
			auto p = static_cast<Movable*>(ptr)->get_position();

			_t += _velocity.Norm() * Dt;

			p.x = pt1.x + std::abs(std::sin(_t)) * (pt2.x - pt1.x);
			p.y = pt1.y + std::abs(std::sin(_t)) * (pt2.y - pt1.y);

			static_cast<Movable*>(ptr)->set_position(p);
		}
	};
}