#pragma once

#include "../domain_object.h"
#include "../ConflictManager/conflict_manager.h"

class ConflictSolver : public DomainObject {
public:
	void Update(double time);
	void SolveClaster(const std::vector<Aircraft_ptr> &);
	ConflictSolver(ConflictManager_ptr);
private:
	ConflictManager_ptr conflict_manager_;
};
