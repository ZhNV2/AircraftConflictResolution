#include "conflict_solver.h"
#include <iostream>
#include <map>
#include <queue>


namespace {

	void dfs(Aircraft_ptr aircraft, std::vector<Aircraft_ptr> &claster, std::map<Aircraft_ptr, bool> &used,
		const std::map < Aircraft_ptr, std::vector<Aircraft_ptr>> &graph) {
		used[aircraft] = true;
		claster.push_back(aircraft);
		if (graph.count(aircraft) == 0) return;
		for (auto u : graph.at(aircraft)) {
			if (!used[u]) {
				dfs(u, claster, used, graph);
			}
		}
	}

	std::vector<std::vector<Aircraft_ptr>> BuildClasses(std::vector<Aircraft_ptr> aircrafts, std::vector<ConflictHolder> conflict_holders) {
		std::vector<std::vector<Aircraft_ptr>> clasters;
		std::map<Aircraft_ptr, std::vector<Aircraft_ptr>> graph;
		for (auto conflict : conflict_holders) {
			graph[conflict.first_aircraft].push_back(conflict.second_aircraft);
			graph[conflict.second_aircraft].push_back(conflict.first_aircraft);
		}
		std::map<Aircraft_ptr, bool> used;
		for (auto aircraft : aircrafts) {
			if (used[aircraft]) continue;
			std::vector<Aircraft_ptr> claster;
			dfs(aircraft, claster, used, graph);
			clasters.push_back(claster);
		}
		return clasters;
	}

	class IntPoint {
	public:
		int x, y, z;
		IntPoint() : x(0), y(0), z(0) {}
		IntPoint(int x1, int y1, int z1) : x(x1), y(y1), z(z1) {}
		bool operator == (const IntPoint &p) const {
			return x == p.x && y == p.y && z == p.z;
		}
		bool operator < (const IntPoint &p) const {
			return x < p.x || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z);
		}
		
	};


	double MinTime(IntPoint start, IntPoint finish, double velocity) {
		return sqrt((start.x - finish.x) * (start.x - finish.x) + (start.y - finish.y) * (start.y - finish.y) + (start.z - finish.z) * (start.z - finish.z)) / velocity;
	}

	IntPoint ToClosestPoint(Point p) {
		return IntPoint((int)p.x, (int)p.y, (int)p.z);
	}

	bool intersect(IntPoint v, IntPoint u, double begin, double end, double velocity, 
		const std::vector<std::pair<double, Track>> &new_tracks) {
		Track track = Track();
		track.AddPoint(Point(v.x, v.y, v.z));
		track.AddPoint(Point(u.x, u.y, u.z));
		for (auto new_track : new_tracks) {
			Track new_track_slice = new_track.second.GetSlice(begin, end, velocity).first;
			if (ConflictManager::CheckIntersection(track, new_track_slice, velocity, new_track.first) > -0.5) return true;
		}
		return false;
	}

	
	void BuildNewPath(Track &track, IntPoint start, IntPoint finish, double velocity,
		const std::vector<std::pair<double, Track>> &new_tracks) {
		std::map<IntPoint, double> min_dist;
		std::map<IntPoint, IntPoint> prev;
		std::map<IntPoint, bool> used;
		std::priority_queue<std::pair<double, IntPoint> > heap;
		min_dist[start] = 0;
		heap.push({ 0, start });
		while (!heap.empty()) {
			auto top = heap.top();
			heap.pop();
			IntPoint v = top.second; 
			if (used[v]) continue;
			used[v] = 1;
			if (finish == v) break;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					for (int dz = -1; dz <= 1; dz++) {
						IntPoint u = IntPoint(v.x + dx, v.y + dy, v.z + dz);
						if (used[u]) continue;
						double time = min_dist[v] + MinTime(v, u, velocity);
						if (min_dist.count(u) == 0 || time < min_dist[u]) {
							if (!intersect(v, u, min_dist[v], min_dist[v] + MinTime(v, u, velocity), velocity, new_tracks)) {
								prev[u] = v;
								min_dist[u] = time;
								heap.push({ -(time + MinTime(u, finish, velocity)), u });
							}
						}
					}
				}
			}
		}
		std::vector<IntPoint> path;
		IntPoint cur = finish;
		while (1) {
			path.push_back(cur);
			if (cur == start) break;
			cur = prev[cur];
		}
		
		std::reverse(path.begin(), path.end());
		std::cout << "new path\n";
		for (auto tp : path) {
			std::cout << "(" << tp.x << " " << tp.y << " " << tp.z << ")" << std::endl;
			track.AddPoint(Point(tp.x, tp.y, tp.z));
		}
	}
}





void ConflictSolver::Update(double time) {
	std::vector<std::vector<Aircraft_ptr>> clasters = BuildClasses(conflict_manager->GetAircrafts(), conflict_manager->GetConflicts());
	std::cout << "solver" << std::endl;
	for (auto claster : clasters) {
		for (auto aircraft : claster) {
			std::cout << aircraft->GetID() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "----" << std::endl;
	for (auto claster : clasters) {
		SolveClaster(claster);
	}
}

void ConflictSolver::SolveClaster(const std::vector<Aircraft_ptr>& claster) {
	std::vector<std::pair<double, Track>> new_tracks;
	for (auto aircraft : claster) {
		Track trajectory = aircraft->GetLastPredicion();
		bool intersect = false;
		for (auto track : new_tracks) {
			intersect |= (ConflictManager::CheckIntersection(track.second, trajectory, track.first, aircraft->GetVelocity()) > -0.5);
		}
		if (!intersect) {
			new_tracks.push_back({ aircraft->GetVelocity(), trajectory });
			continue;
		}
		Track new_track = Track();
		Point begPoint = trajectory.GetTrackPoint(0);
		Point endPoint = trajectory.GetTrackPoint(trajectory.GetLen() - 1);
		new_track.AddPoint(begPoint);
		IntPoint beg = ToClosestPoint(begPoint);
		IntPoint end = ToClosestPoint(endPoint);
		BuildNewPath(new_track, beg, end, aircraft->GetVelocity(), new_tracks);
		new_track.AddPoint(endPoint);
		new_tracks.push_back({ aircraft->GetVelocity(), new_track });
		Track to_correct = Track();
		for (size_t i = 1; i < new_track.GetLen() - 1; i++) to_correct.AddPoint(new_track.GetTrackPoint(i));
		aircraft->InsertTrack(endPoint, to_correct);
	}
}

ConflictSolver::~ConflictSolver() {
}

ConflictSolver::ConflictSolver(ConflictManager_ptr conf_manager) : conflict_manager(conf_manager) {}
