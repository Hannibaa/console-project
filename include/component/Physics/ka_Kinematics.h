#pragma once
#include <include/basic/ka_IKinematic.hpp>
#include <include/maths/clamp.hpp>




namespace physic {

	using Kinematics		=  IKinematic<fVec2, fRect>;
	using MoveInterface0    =  I0Move<fVec2, fRect>;
	using MoveInterface1	=  I1Move<fVec2, fRect>;
	using MoveInterface2	=  I0Move<fVec2, fRect>;


	// function ...................................................................................
	template<typename Movable, typename Vector, typename Space>
	void move(Movable* pM, float Dt, Vector& _velocity, Space& _space)
	{
		static float vx = 1.f;
		static float vy = 1.f;

		auto p = pM->get_position();
		auto d = pM->get_dimension();

		p.x += _velocity.x * Dt * vx;
		p.y += _velocity.y * Dt * vy;

		if (p.x < _space.x || p.x > _space.x + _space.dx - d.x + 2.f) vx *= -1.f;
		if (p.y < _space.y || p.y > _space.y + _space.dy - d.y + 2.f) vy *= -1.f;

		pM->set_position(p);
	}


	/*template<typename Movable>
	void moveTo(Movable* ptr, const fVec2& B, float Dt)
	{
		static float _t = 0;
		auto A = ptr->get_position();
		fVec2 p{};
		_t += 
		p.x += _velocity.x * Dt * vx;
		p.y += _velocity.y * Dt * vy;

		ptr->set_position(p);
	}*/

}