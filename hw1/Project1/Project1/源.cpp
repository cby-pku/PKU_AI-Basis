#include "problem/directed_graph.hpp"

#include "algorithm/heuristic_search.hpp"
#include "algorithm/uniform_cost_search.hpp"

const char pos_names[][20] = {
	"Oradea", // 0
	"Zerind",
	"Arad",    // start:2
	"Sibiu",
	"Fagaras",

	"Timisoara", // 5
	"Rimnicu Vilcea",
	"Lugoj",
	"Pitesti",
	"Mehadia",

	"Drobeta", // 10
	"Craiova",
	"Neamt",
	"Iasi",
	"Vaslui",

	"Giurgiu", // 15
	"Bucharest",  // end:16
	"Urziceni",
	"Hirsova",
	"Eforie"
};

// ���㵽Ŀ��㣨���16��Bucharest��ֱ�߾��룩
double to_target_dis[] = {
	380, 374, 366, 253, 176,
	329, 193, 244, 100, 241,
	242, 160, 234, 226, 199,
	77, 0, 80, 151, 161
};

// �����ߵ���㣬����߽���¼һ��
int u[] = { 0,0,1,2,2, 5,7,9,10,11, 11,3,3,4,6, 8,16,16,12,13, 14,17,18 };

// �����ߵ��յ�
int v[] = { 1,3,2,5,3, 7,9,10,11,6, 8,6,4,16,8, 16,15,17,13,14, 17,18,19 };

// �����ߵ�Ȩ��
double w[] = { 71,151,75,118,140, 111,70,75,120,146, 138,80,99,211,97, 101,90,85,87,92, 142,98,86 };

double a_star_estimator(const DirectedGraphState& s) {
	// ��дA*��״̬��ֵ����
	return -(s.cumulative_cost() + to_target_dis[s.current_node()]);
}

double uniform_cost_estimator(const DirectedGraphState& s) {
	return -s.cumulative_cost();
}

double greedy_estimator(const DirectedGraphState& s) {
	// ��д̰�ĵ�״̬��ֵ����
	return -to_target_dis[s.current_node()];
}


int main() {
	DirectedGraph graph(20);

	for (int i = 0; i < 23; ++i) {
		graph.add_edge(u[i], v[i], w[i]);
		graph.add_edge(v[i], u[i], w[i]);
	}

	DirectedGraphState s(graph, 2, 16);
	HeuristicSearch<DirectedGraphState> hs(s);

	hs.search(greedy_estimator);

	return 0;
}
//#include <iostream>
//#include <ctime>
//
//#include "problem/queens.hpp"
//
//#include "algorithm/depth_first_search.hpp"
//#include "algorithm/breadth_first_search.hpp"
//
//int main() {
//	std::ios::sync_with_stdio(false);
//
//	time_t t0 = time(nullptr);
//
//	QueensState state(11);
//	//BreadthFirstSearch<QueensState> bfs(state);
//	//bfs.search(true, false);
//
//	DepthFirstSearch<QueensState> dfs(state);
//	dfs.search(true, false);
//
//	std::cout << time(nullptr) - t0 << std::endl;
//	return 0;
//}
