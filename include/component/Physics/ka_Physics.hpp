//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//     Generale physical interaction and collision  between object 
//     Author	: Kadda Aoues
//     Date		: 1 / 8 / 2024
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once






namespace physic {

	// Overloap?
	// these two object should have two member function
	// get_bounds()->fRect   and  fRect has is_collid(const fRect&).
	template<typename O1, typename O2>
	bool is_collid(O1& p1, O2& p2) {

		if ((p1.get_bounds().is_collid(p2.get_bounds()))
			|| (p2.get_bounds().is_collid(p1.get_bounds())))
			return true;
		return false;
	}

	// treatment of collision between two object
	template<typename O1, typename O2>
	void collision_process(O1& q1, O2& q2, float aproxi = 2.f) {

		auto p1 = q1.get_position();
		auto p2 = q2.get_position();
		float lx = int(p1.x - p2.x) < 0 ? -1.0 : int(p1.x - p2.x) == 0 ? 0.f : 1.f;
		float ly = int(p1.y - p2.y) < 0 ? -1.0 : int(p1.y - p2.y) == 0 ? 0.f : 1.f;
		q1.set_position(p1.x + lx / aproxi, p1.y + ly / aproxi);
		q2.set_position(p2.x - lx / aproxi, p2.y - ly / aproxi);

	}

	template<typename Movable>
	void collision_process(std::vector<Movable>& vm) {

		for (int i = 0; i < vm.size() ; ++i)
			for (int j = i + 1 ; j < vm.size() ; ++j)
				if (is_collid(vm[i], vm[j]))
				{
					collision_process(vm[i], vm[j]);
				}

	}


	// using iterators for every container
	template<typename Movables>
	void collision_process(Movables& vm)
	{
		for (auto ait = vm.begin(); ait != vm.end() ; ++ait)
			for(auto bit = std::next(ait); bit != vm.end() ; ++ bit)
				if (is_collid(*ait, *bit))
				{
					collision_process(*ait, *bit);
				}
	}

}