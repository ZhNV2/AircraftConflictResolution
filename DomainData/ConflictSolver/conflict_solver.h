#pragma once
#ifndef _CONFLICT_SOLVER_H_
#define _CONFLICT_SOLVER_H_

#include "../domain_object.h"
#include "../ConflictManager/conflict_manager.h"

class ConflictSolver : public DomainObject {
public:
	void Update(double time);
	void SolveClaster(const std::vector<Aircraft_ptr> &);
	~ConflictSolver() override;
	ConflictSolver(ConflictManager_ptr);
private:
	ConflictManager_ptr conflict_manager;
};
#endif // !_CONFLICT_SOLVER_H_
