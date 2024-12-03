#pragma once
/*
8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
									This file discrib the Kenimatic of points and body 
									Only Base Class represented

									Author : Kadda Aoues
									Date	: 18 / 10 /2024

									Contents :
									IIKinematic < Vector, Space, T> interface
									IKinematic that implimentation of functions
									IMove<Movable> : IKinematic  interface

8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888
*/

#include <include/concepts/my_concepts.h>
#include <include/basic/ka_ITransformable.hpp>

namespace physic {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//         O INTERFACE OF KINEMATICS
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename Vector, typename Space, typename T = typename  Vector::value_type>
	requires Concept::is_geometrical_vector<Vector>
	class IIKinematic {
	protected:
		Vector				_velocity;
		Vector				_acceleration;
		Space				_space;

	public:

		virtual void set_velocity(const Vector&) = 0;
		virtual void set_acceleration(const Vector&) = 0;
		virtual void set_space(const Space&) = 0;

		virtual Vector get_velocity() const = 0;
		virtual Vector get_acceleration() const = 0;
		virtual Space get_space() const = 0;

		virtual ~IIKinematic() = default;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//			KINEMATIC INTERFACE OF POINTS
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename Vector, typename Space, typename T = typename  Vector::value_type>
	class IKinematic : public IIKinematic<Vector, Space> {
	public:

		void set_velocity(const Vector& v) override {  // 1.
			this->_velocity = v;
		}
		void set_velocity(T vx, T vy) {
			this->_velocity.x = vx;
			this->_velocity.y = vy;
		}

		Vector get_velocity() const override {			// 2.
			return this->_velocity;
		}

		void set_acceleration(const Vector& a) override { // 3.
			this->_acceleration = a;
		}

		void set_acceleration(T ax, T ay) {
			this->_acceleration.x = ax;
			this->_acceleration.y = ay;
		}

		Vector get_acceleration() const override {   // 4.
			return this->_acceleration;
		}

		void set_space(const Space& space) override { // 5.
			this->_space = space; 
		}

		Space get_space() const override {            //  6.
			return this->_space ;
		}

		virtual ~IKinematic() = default;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    IMOTION AND IMOVE :
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////


	template<typename Movable, typename V, typename S, typename T = typename V::value_type>
	class IMove : public IKinematic<V, S>
	{
	public:
		virtual void move(Movable* pMovable, float Dt) = 0;
	};

	// Interface1 void*
	template<typename V, typename S, typename T = typename V::value_type>
	class I0Move : public IMove<void, V, S> {};

	// interface2 transformable*
	template<typename V, typename S, typename T = typename V::value_type>
	class I1Move : public IMove<console::ITransformable<float>,V, S> {};

	template<typename V, typename S, typename T = typename V::value_type>
	class I2Move : public IKinematic<V, S>
	{
	public:
		template<typename Movable>
		void move(Movable& m, float Dt);
	};
}