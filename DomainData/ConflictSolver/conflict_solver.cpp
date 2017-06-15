#include "conflict_solver.h"
#include <iostream>
#include <map>
#include <queue>
#include <cassert>
#include <qdebug>


namespace {
	
	typedef std::pair<int, double> Min_dist_pair;
	const int DIRS = 8;
	const int MAX_CNT = 1000;
	const double X = 1;
	const int MAX_ORDERS = 10;
	int dx_[DIRS] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dy_[DIRS] = { 1, 1, 0, -1, -1, -1, 0, 1 };

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

	struct DpState {
		IntPoint point;
		int last_com;
		int cnt_com;

		DpState() : point(), last_com(-1), cnt_com(0) {}
		DpState(IntPoint point_, int last_com_, int cnt_com_) : point(point_), last_com(last_com_), cnt_com(cnt_com_) {}
		bool operator == (const DpState &p) const {
			return point == p.point && last_com == p.last_com && cnt_com == p.cnt_com;
		}
		bool operator < (const DpState &p) const {
			return point < p.point;
		}

	};

	double MinTime(IntPoint start, IntPoint finish, double velocity) {
		return sqrt((start.x - finish.x) * (start.x - finish.x) + (start.y - finish.y) * (start.y - finish.y) + (start.z - finish.z) * (start.z - finish.z)) / velocity;
	}

	std::pair<IntPoint, int> ToClosestPoint(Point p, Vector v) {
		
		int closest_dir = 0;
		double max_cos = -1;
		for (int dir = 0; dir < DIRS; dir++) {
			if (Vector::cos(Vector(dx_[dir], dy_[dir], 0), v) > max_cos) {
				max_cos = Vector::cos(Vector(dx_[dir], dy_[dir], 0), v);
				closest_dir = dir;
			}
		}

		IntPoint basic_point = IntPoint((int)p.x, (int)p.y, (int)p.z);
		IntPoint res = basic_point;
		max_cos = -1;
		for (int dx = 0; dx <= 1; dx++) {
			for (int dy = 0; dy <= 1; dy++) {
				double cos = Vector::cos(Vector(p, Point(basic_point.x + dx, basic_point.y + dy, 0)), Vector(dx_[closest_dir], dy_[closest_dir], 0));
				if (cos > max_cos) {
					max_cos = cos;
					res = IntPoint(basic_point.x + dx, basic_point.y + dy, 0);
				}
			}
		}
		return { res, closest_dir };
	}

	bool intersect(IntPoint v, IntPoint u, double begin, double end, double velocity, 
		const std::vector<std::pair<double, Track>> &new_tracks) {
		Track track = Track();
		track.AddPoint(Point(v.x / X, v.y / X, v.z / X));
		track.AddPoint(Point(u.x / X, u.y / X, u.z / X));
		for (auto new_track : new_tracks) {
			Track new_track_slice = new_track.second.GetSlice(begin, end, velocity).first;
			if (ConflictManager::CheckIntersection(track, new_track_slice, velocity, new_track.first) > -0.5) return true;
		}
		return false;
	}



	void scalePoint(IntPoint &p) {
		p.x *= X;
		p.y *= X;
		p.z *= X;
	}
	
	std::vector<Point>BuildPathOrdBounded(double time_delta, IntPoint start, int st_dir, IntPoint finish, 
		int fin_dir, double velocity, int max_ords,
		const std::vector<std::pair<double, Track>> &new_tracks) {

		scalePoint(start);
		scalePoint(finish);


		std::map<DpState, double > min_dist;
		std::map<DpState, DpState> prev;
		std::map<DpState, bool> used;
		std::priority_queue<std::pair<double, DpState> > heap;
		DpState init_state(start, st_dir, 0);
		min_dist[init_state] = time_delta;
		heap.push({ -time_delta, init_state });
		int cnt = 0;

		DpState fin_state;
		qDebug() << "STARTED\n";
		qDebug() << start.x << " " << start.y << " " << start.z << "\n";
		qDebug() << finish.x << " " << finish.y << " " << finish.z << "\n";

		while (!heap.empty()) {
			cnt++;
			if (cnt > MAX_CNT) break;
			auto top = heap.top();
			heap.pop();
			DpState v = top.second;

			if (used[v]) continue;
			used[v] = 1;
			if (finish == v.point && fin_dir == v.last_com) {
				fin_state = v;
				break;
			}

			qDebug() << v.point.x << " " << v.point.y << " " << v.point.z << "\n";

			int last_dir = v.last_com;
	
			for (int t1 = last_dir - 1; t1 <= last_dir + 1; t1++) {
				int t = (t1 + DIRS) % DIRS;
				int dx = dx_[t], dy = dy_[t];
				IntPoint u = IntPoint(v.point.x + dx, v.point.y + dy, v.point.z);
				DpState to = DpState(u, t, v.cnt_com + (t != last_dir));
				if (used[to] || to.cnt_com > max_ords) continue;

				double time = min_dist[v] + MinTime(v.point, u, velocity);
				if (min_dist.count(to) == 0 || time < min_dist[to]) {
					if (!intersect(v.point, u, min_dist[v], min_dist[v] + MinTime(v.point, u, velocity), velocity, new_tracks)) {
						prev[to] = v;
						min_dist[to] = time;
						heap.push({ -(time + MinTime(u, finish, velocity)), to });
					}
				}

			}

		}
		if (fin_state.last_com == -1) return std::vector<Point>();
		qDebug() << "FINISHED\n";
		std::vector<Point> path;
		DpState cur = fin_state;
		while (1) {
			IntPoint p = cur.point;
			path.push_back(Point(p.x / X, p.y / X, p.z / X));
			if (cur.point == start) break;
			cur = prev[cur];
		}
		return path;
	}

	void BuildNewPath(double time_delta, Track &track, IntPoint start, int st_dir, IntPoint finish, int fin_dir, double velocity,
		const std::vector<std::pair<double, Track>> &new_tracks) {
		
		int left = -1, right = MAX_ORDERS;

		while (left != right - 1) {
			int mid = (left + right) / 2;
			if (BuildPathOrdBounded(time_delta, start, st_dir, finish, fin_dir, velocity, mid, new_tracks).size() ) {
				right = mid;
			}
			else {
				left = mid;
			}
		}
		std::vector<Point> path = BuildPathOrdBounded(time_delta, start, st_dir, finish, fin_dir, velocity, right, new_tracks);
				
	std::reverse(path.begin(), path.end());
		std::cout << "new path\n";
		for (auto tp : path) {
			std::cout << "(" << tp.x << " " << tp.y << " " << tp.z << ")" << std::endl;
			track.AddPoint(Point(tp.x, tp.y, tp.z));
		}
	}
}





void ConflictSolver::Update(double time) {
	std::vector<std::vector<Aircraft_ptr>> clasters = BuildClasses(conflict_manager_->GetAircrafts(), conflict_manager_->GetConflicts());
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
		int len = trajectory.GetLen();
		Point begPoint = trajectory.GetTrackPoint(0);
		Point endPoint = trajectory.GetTrackPoint(len - 1);
		new_track.AddPoint(begPoint);
		std::pair<IntPoint, int> beg = ToClosestPoint(begPoint, Vector(trajectory.GetTrackPoint(0), trajectory.GetTrackPoint(1)));
		std::pair<IntPoint, int> end = ToClosestPoint(endPoint, Vector(trajectory.GetTrackPoint(len - 1), trajectory.GetTrackPoint(len - 2)));
		double time_delta = Vector(trajectory.GetTrackPoint(0), Point(beg.first.x, beg.first.y, 0)).Len() / aircraft->GetVelocity();
		BuildNewPath(time_delta, new_track, beg.first, beg.second, end.first, (DIRS / 2 + end.second) % DIRS, aircraft->GetVelocity(), new_tracks);
		new_track.AddPoint(endPoint);
		new_tracks.push_back({ aircraft->GetVelocity(), new_track });
		Track to_correct = Track();
		for (size_t i = 1; i < new_track.GetLen() - 1; i++) to_correct.AddPoint(new_track.GetTrackPoint(i));
		aircraft->InsertTrack(endPoint, to_correct);
	}
}

ConflictSolver::ConflictSolver(ConflictManager_ptr conf_manager) : conflict_manager_(conf_manager) {}
