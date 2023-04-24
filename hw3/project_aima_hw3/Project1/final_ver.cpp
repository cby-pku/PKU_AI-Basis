//#include <cfloat>
//#include <cmath>
//#include <cinttypes>
//#include <cassert>
//#include <ctime>
//#include <iostream>
//#include <vector>
//#include <type_traits>
//#include <unordered_map>
//#include <array>
//#include <random>
//
//#include "jsoncpp/json.h" // Botzone��C++����ʱĬ�ϰ����˿⣬�����ύ�˿�����
//
//// �ṩ��������Ĺ�����
//class RandomVariables {
//private:
//
//	// �̶��������
//	RandomVariables() = default;
//
//	// �ǹ̶��������
//	// RandomVariables() : random_engine(time(nullptr)) {}
//
//	~RandomVariables() = default;
//
//	std::default_random_engine random_engine;
//	std::uniform_real_distribution<double> uniform_dist;
//	std::uniform_int_distribution<int> uniform_int_dist;
//
//	static RandomVariables rv;
//
//public:
//
//	// ���ȷֲ���������
//	static int uniform_int() {
//		return rv.uniform_int_dist(rv.random_engine);
//	}
//
//	// [0,1)���ȷֲ���ʵ��
//	static double uniform_real() {
//		return rv.uniform_dist(rv.random_engine);
//	}
//
//	// �ȸ��ʷֲ���{0,1,2,n-1}����
//	static std::vector<int> uniform_permutation(int n) {
//		std::vector<int> permutation(n);
//		for (int i = 0; i < n; ++i) {
//			permutation[i] = i;
//		}
//
//		for (int i = 0, j; i < n; ++i) {
//			j = uniform_int() % (n - i) + i;
//			std::swap(permutation[i], permutation[j]);
//		}
//
//		return permutation;
//	}
//};
//
//RandomVariables RandomVariables::rv;
//
//// ��Ϸ״̬�ӿ�
//template<typename ActionType>
//class GameStateBase {
//public:
//
//	GameStateBase() = default;
//	virtual ~GameStateBase() = default;
//
//	using ActionBaseType = ActionType;
//
//	// �������
//	virtual int n_players() const = 0;
//
//	// ��ǰ�������
//	virtual int active_player() const = 0;
//
//	// �����ռ��С
//	virtual int n_actions() const = 0;
//
//	// ��ǰ״̬�µĶ����ռ�
//	virtual std::vector<ActionType> action_space() const = 0;
//
//	// ���������֮ǰ����Ϸ�������ۼƵĻر�
//	virtual std::vector<double> cumulative_rewards() const = 0;
//
//	// ���������ת�Ƶ���ǰ״̬���õļ�ʱ�ر�
//	virtual std::vector<double> rewards() const = 0;
//
//	// ��Ϸ�Ƿ����
//	virtual bool done() const = 0;
//
//	// չʾ��Ϸ״̬
//	virtual void show() const = 0;
//
//	// ��ǰ���ѡ��action��ת�Ƶ�����״̬
//	virtual const GameStateBase& next(const ActionType&) const = 0;
//
//	// ״̬��ϣ
//	friend struct std::hash<GameStateBase>;
//
//	// ����Ӧ����operator==
//	friend bool operator== (const GameStateBase& s1, const GameStateBase& s2) {
//		return s1.cumulative_rewards()[s1.active_player()] == s2.cumulative_rewards()[s2.active_player()];
//	}
//};
//
//// n����Ϸ״̬�ӿ�
//template<typename ActionType, int n>
//class NPlayerGameStateBase : public GameStateBase<ActionType> {
//protected:
//	static constexpr int _n_players = n;
//public:
//	NPlayerGameStateBase() = default;
//	virtual ~NPlayerGameStateBase() = default;
//
//	int n_players() const override { return _n_players; }
//};
//
//
//// ���鼯�㷨
//class UnionFindSet {
//private:
//
//	// ����Ԫ�����������
//	std::vector<int> color;
//
//public:
//
//	UnionFindSet(int n) {
//		for (int i = 0; i < n; ++i) {
//			color.push_back(i);
//		}
//	}
//
//	UnionFindSet() = default;
//
//	// ����һ��Ԫ�����������
//	int find(int x) {
//		return x == color[x] ? x : (color[x] = find(color[x]));
//	}
//
//	// �ϲ�����Ԫ�����������
//	void join(int x, int y) {
//		int cx = find(x), cy = find(y);
//		if (cx != cy) {
//			color[cx] = cy;
//		}
//	}
//};
//
//// N*N����������״̬��
//template<int _N>
//class HexState : public NPlayerGameStateBase<int, 2> {
//private:
//
//	static constexpr int N = _N;
//
//	// �췽����
//	static constexpr int8_t R = 0b01, B = 0b10;
//
//	int steps;
//	std::array<int8_t, N*N> board;
//
//	// ���ڼ���ɫ/��ɫ�����ͨ��
//	mutable UnionFindSet r_detector, b_detector;
//
//	bool r_win() const {
//		return r_detector.find(N*N) == r_detector.find(N*N + 1);
//	}
//
//	bool b_win() const {
//		return b_detector.find(N*N) == b_detector.find(N*N + 1);
//	}
//
//public:
//
//	HexState() : steps(0), board{ 0 }, r_detector(N*N + 2), b_detector(N*N + 2) {
//
//		// ���߽絥����һ��Ԫ�ر�ʾ
//		// N*N:   up, red      /  left, blue
//		// N*N+1: bottom, red  /  right, blue
//		for (int i = 0; i < N; ++i) {
//			r_detector.join(N*N, i);
//			r_detector.join(N*N + 1, N*N - 1 - i);
//			b_detector.join(N*N, N*i);
//			b_detector.join(N*N + 1, N*(i + 1) - 1);
//		}
//	}
//
//	bool done() const override {
//		return r_win() or b_win();
//	}
//
//	int active_player() const override {
//		return steps & 1;
//	}
//
//	std::vector<double> rewards() const override {
//		static const std::vector<double> score_r_win{ 1, -1 },
//			score_b_win{ -1, 1 },
//			score_tie{ 0, 0 };
//
//		return r_win() ? score_r_win : (b_win() ? score_b_win : score_tie);
//	}
//
//	std::vector<double> cumulative_rewards() const override {
//
//		return rewards();
//	}
//
//	inline int n_actions() const override {
//		return N * N - steps;
//	}
//
//	std::vector<int> action_space() const override {
//		std::vector<int> actions;
//		for (int i = 0; i < N*N; ++i) {
//			if (board[i] == 0) {
//				actions.push_back(i);
//			}
//		}
//		return actions;
//	}
//
//	const HexState& next(const int& action) const override {
//		static HexState next_state;
//
//		assert(board[action] == 0);
//		next_state = *this;
//
//		next_state.board[action] = active_player() == 0 ? R : B;
//
//		std::vector<int> neighbors{
//			action - N, action + 1 - N,
//			action + 1, action + N,
//			action - 1 + N, action - 1
//		};
//
//		bool not_top = action >= N,
//			not_bottom = action < N*N - N,
//			not_left = action % N != 0,
//			not_right = action % N != N - 1;
//
//		std::vector<int8_t> conditions{
//			not_top, not_top and not_right,
//			not_right, not_bottom,
//			not_bottom and not_left, not_left
//		};
//
//		UnionFindSet& detector = active_player() == 0 ? next_state.r_detector : next_state.b_detector;
//
//		for (int i = 0; i < conditions.size(); ++i) {
//			if (conditions[i] and
//				next_state.board[neighbors[i]] == next_state.board[action]) {
//				detector.join(neighbors[i], action);
//			}
//		}
//
//		++next_state.steps;
//		return next_state;
//	}
//
//	void show() const override {
//		const static char pieces[] = "_XO";
//		for (int i = 0; i < N; ++i) {
//			for (int j = 0; j < i; ++j) {
//				std::cout << ' ';
//			}
//			for (int j = 0; j < N; ++j) {
//				std::cout << pieces[board[i*N + j]] << ' ';
//			}
//			std::cout << '\n';
//		}
//	}
//
//	friend struct std::hash<HexState>;
//	friend bool operator== (const HexState& s1, const HexState& s2) {
//		return s1.board == s2.board;
//	}
//};
//
//template<int N>
//struct std::hash<HexState<N> > {
//	size_t operator() (const HexState<N>& s) const {
//		size_t code = s.steps;
//		for (int i = 0; i < s.board.size(); ++i) {
//			code ^= size_t(s.board[i]) << (i & ((sizeof(size_t) << 3) - 1));
//		}
//		return code;
//	}
//};
//
//// �����������
//class SearchTreeNode {
//private:
//
//	// Ϊ���еĽ�㽨��Ψһ���
//	int _index;
//
//	// �ý���ж��ٸ��ӽ��
//	int _n_children;
//
//	// �ý��ĸ����
//	SearchTreeNode* _parent;
//
//	// �ý����ӽ���б�
//	std::vector<SearchTreeNode*> _children;
//
//public:
//
//	SearchTreeNode() = default;
//
//	// ����һ�����Ϊindex�Ľ�㣬�ý��û�и��ڵ���ӽ��
//	SearchTreeNode(int index) : _index(index), _parent(nullptr), _n_children(0) {}
//
//	// ��child���Ϊ��ǰ�����ӽ��
//	void add_child(SearchTreeNode* child) {
//		++_n_children;
//		_children.push_back(child);
//		child->_parent = this;
//	}
//
//	// ���ؽ����
//	int index() const { return _index; }
//
//	// ��ǰ����ж����ӽ��
//	int n_children() const { return _n_children; }
//
//	// ��ǰ���ĸ��ڵ�
//	SearchTreeNode* parent() const { return _parent; }
//
//	// ��ǰ���ĵ�child_index���ӽ��
//	SearchTreeNode* child(int child_index) const { return _children[child_index]; }
//
//	// ��ǰ�����ӽڵ��б�
//	const std::vector<SearchTreeNode*>& children() const { return _children; }
//
//	// �ȽϽ����ֻͬ��Ҫ�Ƚϱ��
//	friend bool operator== (const SearchTreeNode& n1, const SearchTreeNode& n2) {
//		return n1._index == n2._index;
//	}
//
//	friend struct std::hash<SearchTreeNode>;
//};
//
//template<>
//struct std::hash<SearchTreeNode> {
//	size_t operator() (const SearchTreeNode& n) const {
//		return n._index;
//	}
//};
//
//// ��������
//class SearchTree {
//private:
//
//	// ���������Ľ���ʶ��
//	int _unique_identifier;
//
//	// ����ǰ�Ĵ�С
//	int _n_nodes;
//
//	// �������ĸ�
//	SearchTreeNode* _root;
//
//	// �����ı��ӳ�䵽���ָ��
//	std::unordered_map<int, SearchTreeNode*> _node_of;
//
//public:
//
//	// ����һ��������������Ϊroot��������1�������
//	SearchTree() : _root(new SearchTreeNode(0)), _unique_identifier(1), _n_nodes(1) {
//		_node_of[0] = _root;
//	}
//
//	// ���ؽ���Ŷ�Ӧ�Ľ��ָ��
//	SearchTreeNode* node_of(int index) const {
//		return _node_of.at(index);
//	}
//
//	// ɾ����ĳ������֮��Ĳ��֣����������ʹ�С��Ϣ
//	void destroy_except_subtree(SearchTreeNode* subtree_root) {
//		SearchTreeNode* node;
//		std::stack<std::pair<SearchTreeNode*, int> > node_stack;
//		node_stack.push(std::make_pair(_root, 0));
//		std::pair<SearchTreeNode*, int> node_searched;
//		int searched, deleted = 0;
//
//		// �������ɾ�����
//		while (not node_stack.empty()) {
//			node_searched = node_stack.top();
//			node_stack.pop();
//			node = node_searched.first;
//			searched = node_searched.second;
//
//			// �����ǰ̽�����˲�Ӧ��ɾ���������ĸ����򲻽���ɾ������
//			if (node == subtree_root) {
//				continue;
//			}
//
//			// ��������ӽ���Ѿ�ɾ������ɾ���ý������
//			if (searched >= node->n_children()) {
//				++deleted;
//				_node_of.erase(node->index());
//				delete node;
//
//			}
//			else {
//				node_stack.push(std::make_pair(node, searched + 1));
//				node_stack.push(std::make_pair(node->child(searched), 0));
//			}
//		}
//
//		_n_nodes -= deleted;
//		_root = subtree_root;
//	}
//
//	// ��������
//	SearchTreeNode* root() const { return _root; }
//
//	// ����һ���µĽ�㣬������������������еĽ�㶼����ͬ
//	SearchTreeNode* create_node() {
//		auto new_node = new SearchTreeNode(_unique_identifier);
//		_node_of[_unique_identifier] = new_node;
//		++_unique_identifier;
//		return new_node;
//	}
//
//	// ��create_node���ɵĽ�����Ϊparent���ӽ�㣬�������Ĵ�С
//	void add_as_child(SearchTreeNode* parent, SearchTreeNode* child) {
//		parent->add_child(child);
//		++_n_nodes;
//	}
//
//	// ��������ǰ�Ĵ�С
//	int n_nodes() const { return _n_nodes; }
//
//	//// ������������Ӧ����֤ÿ���ڵ㶼�ǵ���create_node���ɵģ�
//	//~SearchTree() {
//	//	destroy_except_subtree(nullptr);
//	//}
//};
//
//
//// ѡ���㷨�ӿڣ���done()Ϊtrueʱselected_index()��Ϊѡ���ֵ���
//class SelectionBase {
//public:
//
//	SelectionBase() = default;
//	virtual ~SelectionBase() = default;
//
//	virtual void initialize(int items, double initial_value) = 0;
//	virtual void submit(double value) = 0;
//	virtual bool done() const = 0;
//	virtual int selected_index() const = 0;
//};
//
//// ���ѡ���㷨��ѡ������ֵ������
//class MaxSelection : public SelectionBase {
//private:
//
//	double max_value;
//	int index, total_items, submitted_items;
//
//public:
//
//	MaxSelection() = default;
//
//	// items:��ѡ���ϴ�С��initial_value:���㷨�޹�
//	void initialize(int items, double initial_value) override {
//		index = 0;
//		total_items = items;
//		submitted_items = 0;
//	}
//
//	void submit(double value) override {
//		if (submitted_items == 0 or value > max_value) {
//			max_value = value;
//			index = submitted_items;
//		}
//		++submitted_items;
//	}
//
//	bool done() const override {
//		return submitted_items >= total_items;
//	}
//
//	int selected_index() const override {
//		return index;
//	}
//};
//
//std::vector<double> operator+ (const std::vector<double>& a, const std::vector<double>& b) {
//	std::vector<double> result(a);
//	for (int i = 0; i < result.size(); ++i) {
//		result[i] += b[i];
//	}
//	return result;
//}
//
//std::vector<double>& operator+= (std::vector<double>& a, const std::vector<double>& b) {
//	a = a + b;
//	return a;
//}
//int start; int endtime;
//template<typename GameState>
//class MonteCarloTreeSearch {
//private:
//
//	using ActionType = typename GameState::ActionBaseType;
//
//	static_assert(std::is_base_of<GameStateBase<ActionType>, GameState>::value, "GameState not derived from GameStateBase.");
//
//	// ������
//	SearchTree tree;
//
//	// ��״̬ӳ�䵽�����
//	std::unordered_map<GameState, int> state_to_index;
//
//	// �ѽ����ӳ��Ϊ״̬
//	std::unordered_map<int, GameState> index_to_state;
//
//	// �ѽ���indexӳ�䵽���ʴ���
//	std::unordered_map<int, int> visit_count_of;
//
//	// �ѽ���indexӳ�䵽���ʸýڵ���������õ��ܼ�ֵ
//	std::unordered_map<int, std::vector<double> > value_sums_of;
//
//	// ��ȫ���ģ��
//	std::vector<double> simulate_from(GameState state) const {
//		int action_id;
//		while (not state.done()) {
//			action_id = RandomVariables::uniform_int() % state.n_actions();
//			state = state.next(state.action_space()[action_id]);
//		}
//		return state.cumulative_rewards();
//	}
//
//	// ����һ��·��������;�������ʼ������ܼ�ֵ
//	std::vector<double> sample_path(const GameState& state, double exploration) {
//
//		// ��ǰ״̬���������еı��
//		int index = state_to_index[state];
//
//		// ��ǰ״̬��Ӧ�����������
//		SearchTreeNode* node = tree.node_of(index);
//
//		SearchTreeNode* child;
//
//		GameState next_state;
//
//		std::vector<double> values;
//
//		// ���ʵ��Ľ���������
//		++visit_count_of[index];
//
//		// ���δ��ȫ��չ��ǰ��㣬ѡ��һ��û�������Ķ��������ԣ���չ��ģ��
//		if (node->n_children() < state.n_actions()) {
//
//			// ��չ�Ľ���Ӧ��״̬
//			next_state = state.next(state.action_space()[node->n_children()]);
//
//			// ��������������ӽ��
//			child = tree.create_node();
//			tree.add_as_child(node, child);
//
//			// ά���������Ͻ������״̬֮��Ķ�Ӧ��ϵ
//			state_to_index[next_state] = child->index();
//			index_to_state[child->index()] = next_state;
//
//			// �ӽ���ʼ���ʼ���Ϊ1
//			visit_count_of[child->index()] = 1;
//
//			// �ӽ���ʼ�ۼ�����Ϊģ��õ���ֵ
//			values = simulate_from(next_state);
//			value_sums_of[child->index()] = values;
//
//			// �����ǰ����Ѿ���ȫ��չ����ô����UCT�㷨ѡ������һ���ӽ�����
//		}
//		else if (node->n_children() > 0) {
//
//			MaxSelection selection;
//			selection.initialize(node->n_children(), -DBL_MAX);
//
//			for (int i = 0, child; i < node->n_children(); ++i) {
//
//				// child�ǵ�ǰѡ����ӽ�������еı��
//				child = node->child(i)->index();
//
//				// ѡ��UCTֵ�����ӽ�����̽��
//				selection.submit(value_sums_of[child][state.active_player()] / visit_count_of[child]
//					+ exploration * sqrt(log(visit_count_of[index]) / visit_count_of[child])
//				);
//			}
//
//			next_state = state.next(state.action_space()[selection.selected_index()]);
//			values = sample_path(next_state, exploration);
//		}
//		else {
//			values = state.cumulative_rewards();
//		}
//
//		value_sums_of[index] += values;
//		return values;
//	}
//
//public:
//
//	MonteCarloTreeSearch(const GameState& root_state) {
//
//		// _root_state״̬��Ӧ�������������������б��Ϊ0
//		state_to_index[root_state] = 0;
//		index_to_state[0] = root_state;
//
//		// ��ʼʱ�������ʼ���Ϊ0
//		visit_count_of[0] = 0;
//
//		// ��ʼʱ����û���ۼ�����
//		value_sums_of[0] = std::vector<double>(root_state.n_players(), 0);
//	}
//
//	ActionType select_action(int iterations, double exploration) {
//
//		GameState root_state = index_to_state[0];
//
//		for (int i = 0; i < iterations&&clock() - start < endtime; ++i) {
//
//			// ��������Ӧ��״̬��ʼ��ÿ�β�����һ��·��������;��״̬�ķ��ʼ������ܼ�ֵ
//			sample_path(root_state, exploration);
//		}
//
//		SearchTreeNode* root = tree.root();
//
//		MaxSelection selection;
//		selection.initialize(root->n_children(), -DBL_MAX);
//
//		// ���ο���������չ�����������ӽ�㣬����ѡ��һ��ƽ����ֵ��ߵ�
//		for (int i = 0, child; i < root->n_children(); ++i) {
//
//			// child�ǵ�ǰѡ����ӽ�������еı��
//			child = root->child(i)->index();
//
//			// ��ƽ����ֵ̰��ѡ��
//			selection.submit(value_sums_of[child][root_state.active_player()] / visit_count_of[child]);
//		}
//
//		// Ҳ���԰��շ��ʴ���̰��ѡ��
//
//		return root_state.action_space()[selection.selected_index()];
//	}
//};
//
////////////////////////////////////////////////////
///**********************Main**********************/
//
//using namespace std;
//
//// ���̴�С
//const int N = 11;
//
//// UCT̽����ϵ��
//const double exploration = 0.9;
//
//// ��������
//const int iterations = 5000;
//
//// ��������state�µĶ���
//Json::Value get_next_action(const HexState<N>& state, bool forced_flag) {
//	MonteCarloTreeSearch<HexState<N> > mcts(state);
//	int action = mcts.select_action(iterations, exploration);
//
//	if (forced_flag) {
//		action = 1 * 11 + 2;
//	}
//
//	Json::Value action_json;
//	action_json["x"] = action / N;
//	action_json["y"] = action % N;
//	return action_json;
//}
//
//int main() {
//
//	HexState<N> state;
//	srand(clock());
//	start = clock();
//	endtime = (int)(0.8*(double)CLOCKS_PER_SEC);
//	// ����JSON
//	string str;
//	getline(cin, str);
//	Json::Reader reader;
//	Json::Value input;
//	reader.parse(str, input);
//
//	// �����Լ��յ���������Լ���������������ָ�״̬
//	int turn_id = input["responses"].size();
//	int x, y;
//	bool forced_flag;
//	for (int i = 0; i < turn_id; i++) {
//		x = input["requests"][i]["x"].asInt();
//		y = input["requests"][i]["y"].asInt();
//		if (x >= 0 and y >= 0) {
//			state = state.next(x * N + y);
//		}
//		x = input["responses"][i]["x"].asInt();
//		y = input["responses"][i]["y"].asInt();
//		state = state.next(x * N + y);
//	}
//	x = input["requests"][turn_id]["x"].asInt();
//	y = input["requests"][turn_id]["y"].asInt();
//
//	if (x >= 0 and y >= 0) {
//		state = state.next(x * N + y);
//		forced_flag = false;
//	}
//	else if (input["requests"][0].isMember("forced_x")) {
//		forced_flag = true;
//	}
//	else {
//		forced_flag = false;
//	}
//
//	// �������ߴ�Ϊaction 
//	// �������JSON
//	Json::Value result;
//
//	result["response"] = get_next_action(state, forced_flag);
//
//	Json::FastWriter writer;
//
//	cout << writer.write(result) << endl;
//
//	return 0;
//}