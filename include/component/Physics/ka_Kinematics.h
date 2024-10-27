#pragma once
#include <include/basic/ka_IKinematic.hpp>
#include <include/maths/clamp.hpp>




namespace physic {

	using Kinematics		=  IKinematic<fVec2, fRect>;
	using MoveInterface0    =  I0Move<fVec2, fRect>;
	using MoveInterface1	=  I1Move<fVec2, fRect>;
	using MoveInterface2	=  I0Move<fVec2, fRect>;

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


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//     Controled Move    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

}