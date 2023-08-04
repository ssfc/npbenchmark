import time as time
import heapq
from itertools import product
import numpy as np
import copy
import collections


# Q: 页面中函数move(current_location, chosen_direction)的功能是什么？
# 函数的功能是根据一个位置和一个方向，返回一个新的位置。
# 函数使用了一个列表来表示五个可能的方向，分别是不动、上、右、下、左。函数根据方向的索引，从列表中取出对应的元组，然后把元组中的两个值分别加到位置的两个坐标上，得到新的位置，并返回这个位置。
# 这个函数可以用于模拟一个代理在地图上的移动。
# Q: 页面中函数move(current_location, chosen_direction)输入参数的数据类型和含义分别是什么？
# @param loc (tuple): 表示一个位置，包含两个整数，分别是横坐标和纵坐标。例如，(1, 2)表示横坐标为1，纵坐标为2的位置。
# @param dir (int): 表示一个方向，取值范围是0到4，分别对应不动、上、右、下、左。例如，0表示不动，1表示上，2表示右，依此类推。
# Q: 页面中函数move(current_location, chosen_direction)输出结果的数据类型和含义分别是什么？
# @return (tuple): 表示新的位置，包含两个整数，分别是横坐标和纵坐标。例如，(3, 4)表示横坐标为3，纵坐标为4的位置。
def move(current_location, chosen_direction):
    directions = [(0, 0), (0, -1), (1, 0), (0, 1), (-1, 0)]
    return current_location[0] + directions[chosen_direction][0], current_location[1] + directions[chosen_direction][1]


# Q: 页面中函数def get_sum_of_cost(paths)的功能是什么？
# 函数的功能是计算一组路径的总代价，即每个路径的长度减一的和。函数遍历paths中的每个路径，把路径的长度减一累加到结果中，并返回结果。
# 函数还使用了一个断言语句，用于检查每个路径的最后两个节点是否不同，即路径中不存在重复节点。
# 如果最后两个节点相同（路径中出现环），则断言会引发AssertionError异常，表示存在错误。1这个函数可以用于评估一组路径的优劣。
# Q: 页面中函数def get_sum_of_cost(paths)输入参数的数据类型和含义分别是什么？
# @param paths (list): 表示一组路径，列表中的每个元素是一个列表，表示一个路径，路径中的每个元素是一个tuple，表示一个节点二维坐标。
# 例如，[[1, 2, 3], [4, 5, 6]]表示两条路径，第一条路径由节点1、2、3组成，第二条路径由节点4、5、6组成。
# Q: 页面中函数def get_sum_of_cost(paths)输出结果的数据类型和含义分别是什么？
# return result (int): 表示一组路径的总代价，即每个路径的长度减一的和。
# 例如，如果paths = [[1, 2, 3], [4, 5, 6]]，则输出结果为4，因为第一条路径的长度为3，减一为2，第二条路径的长度为3，减一为2，两者相加为4。
def get_sum_of_cost(paths):
    result = 0
    for path in paths:
        # print(path)
        result += len(path) - 1
        if len(path) > 1:
            # 这是一个断言语句，用于检查当前路径的最后两个节点是否不同，即路径中不存在重复节点。
            # 如果最后两个节点相同（路径中出现环），则断言会引发 AssertionError 异常，表示存在错误。
            # 这个断言可以帮助程序员及时发现路径数据的错误。
            assert path[-1] != path[-2]
    return result


# Use Dijkstra to build a shortest-path tree rooted at the goal location
# Q: 页面中函数def compute_heuristics(input_map, input_goal)的功能是什么？
# 函数的功能是计算一个启发式表，用于存储从每个位置到目标位置的最短路径的代价。
# 函数使用了Dijkstra算法来构建一个以目标位置为根的最短路径树，然后遍历这棵树，把每个位置到目标位置的代价存储在一个字典中，并返回这个字典。
# 这个启发式表可以用于A*算法中，来估计每个位置到目标位置的距离，从而提高搜索效率。
# Q: 页面中函数def compute_heuristics(input_map, input_goal)输入参数的数据类型和含义分别是什么？
# @param input_map (2D list): 表示一个地图
# @param input_goal (tuple): 目标节点的坐标
# Q: 页面中函数def compute_heuristics(input_map, input_goal)输出结果的数据类型和含义分别是什么？
# return h_values (dict): 一个启发式表，字典中的每个键是一个tuple，表示一个节点坐标，字典中的每个值是一个整数，表示从该节点到目标节点的最短路径的代价。
# 例如，{1: 4, 2: 3, 3: 2, 4: 0}表示一个启发式表，其中节点1到节点4的最短路径的代价是4，节点2到节点4的最短路径的代价是3，以此类推。
def compute_heuristics(input_map, input_goal):
    open_list = []
    closed_list = dict()
    root = {'loc': input_goal, 'cost': 0}
    heapq.heappush(open_list, (root['cost'], input_goal, root))
    closed_list[input_goal] = root
    while len(open_list) > 0:
        (cost, loc, curr) = heapq.heappop(open_list)
        for dir in range(1, 5):
            child_loc = move(loc, dir)
            child_cost = cost + 1
            if child_loc[0] < 0 or child_loc[0] >= len(input_map) \
                    or child_loc[1] < 0 or child_loc[1] >= len(input_map[0]):
                continue
            if input_map[child_loc[0]][child_loc[1]]:
                continue
            child = {'loc': child_loc, 'cost': child_cost}
            if child_loc in closed_list:
                existing_node = closed_list[child_loc]
                if existing_node['cost'] > child_cost:
                    closed_list[child_loc] = child
                    # open_list.delete((existing_node['cost'], existing_node['loc'], existing_node))
                    heapq.heappush(open_list, (child_cost, child_loc, child))
            else:
                closed_list[child_loc] = child
                heapq.heappush(open_list, (child_cost, child_loc, child))

    # build the heuristics table
    h_values = dict()
    for loc, node in closed_list.items():
        h_values[loc] = node['cost']
    return h_values


# Q: 页面中函数def get_location(input_path, input_time)的功能是什么？
# 返回一个路径在给定时间的位置。函数根据时间参数，从路径列表中取出对应的元素，即位置元组，并返回这个元组。
# 如果时间参数小于0，函数返回路径的第一个位置；如果时间参数大于等于路径的长度，函数返回路径的最后一个位置；否则，函数返回路径中对应索引的位置。
# Q: 页面中函数def get_location(input_path, input_time)输入参数的数据类型和含义分别是什么？
# @param input_path (list): 列表中的每个元素是一个表示位置的元组。
# 例如，input_path = [(1, 2), (3, 4), (5, 6)]表示一个路径由三个位置组成，每个位置由两个整数表示。
# @param input_time (input_time): 表示要查询的时间点。例如，2表示要查询路径上第二个位置。
# Q: 页面中函数def get_location(input_path, input_time)输出结果的数据类型和含义分别是什么？
# return (tuple): 表示一个位置，包含两个整数，分别是横坐标和纵坐标。例如，(3, 4)表示横坐标为3，纵坐标为4的位置。
def get_location(input_path, input_time):
    if input_time < 0:
        return input_path[0]
    elif input_time < len(input_path):
        return input_path[input_time]
    else:
        return input_path[-1]  # wait at the goal location


# Q: 页面中函数def trace_path(goal_node, meta_agent)的功能是什么？
# 从目标节点回溯到根节点，得到一组路径，每个路径对应一个代理。
# 函数使用了一个循环，从目标节点开始，依次访问每个节点的父节点，把每个代理的位置添加到对应的路径列表中，直到到达根节点为止。然后，函数把每个路径列表反转，使其从起点到终点的顺序排列，并返回这些路径列表。
# 函数还使用了一些断言语句，用于检查每个路径的有效性和完整性。
# Q: 页面中函数def trace_path(goal_node, meta_agent)输入参数的数据类型和含义分别是什么？
# @param goal_node (tuple): 目标节点
# @param meta_agent (list): 表示一组代理或者一个代理，列表中的每个元素是一个整数，表示一个代理的编号。
# 例如，[0, 1]表示两个代理，编号分别为0和1。
# Q: 页面中函数def trace_path(goal_node, meta_agent)输出结果的数据类型和含义分别是什么？
# return path (list): 表示一个路径，路径中的每个元素是一个元组，表示一个位置，元组中包含两个整数，分别是横坐标和纵坐标。
def trace_path(goal_node, meta_agent):
    path = []
    for i in range(len(meta_agent)):
        path.append([])
    curr = goal_node
    while curr is not None:
        for i in range(len(meta_agent)):
            path[i].append(curr['loc'][i])
        curr = curr['parent']
    for i in range(len(meta_agent)):
        path[i].reverse()
        assert path[i] is not None

        print(path[i])

        if len(path[i]) > 1:
            # remove trailing duplicates
            while path[i][-1] == path[i][-2]:
                path[i].pop()
                print(path[i])
                if len(path[i]) <= 1:
                    break
            # assert path[i][-1] != path[i][-2] # no repeats at the end!!

    assert path is not None
    return path


class A_Star(object):
    # Q: 页面中函数__init__(self, input_map, input_starts, input_goals, input_heuristics, agents, input_constraints)的功能是什么？
    # 初始化一个CBS对象，即一个用于解决多智能体路径规划问题的冲突检测搜索对象。
    # Q: 页面中函数__init__(self, input_map, input_starts, input_goals, input_heuristics, agents, input_constraints)
    # 输入参数的数据类型和含义分别是什么？
    # @param input_map (2D list): 表示一个地图
    # @param input_starts (list of tuple): list of start locations for CBS
    # @param input_goals (list of tuple): list of goal locations for CBS
    # @param input_heuristics (list of dict): [{(3, 0): 0, (4, 0): 1, (3, 1): 1, (2, 0): 1, (2, 1): 2, (1, 0): 2,
    # (4, 1): 2, (3, 2): 2,
    # (5, 0): 2, (1, 1): 3, (0, 0): 3, (4, 2): 3, (3, 3): 3, (5, 1): 3, (6, 0): 3, (0, 1): 4, (1, 2): 4, (4, 3): 4,
    # (3, 4): 4, (2, 3): 4, (5, 2): 4, (6, 1): 4, (7, 0): 4, (2, 4): 5, (4, 4): 5, (3, 5): 5, (5, 3): 5, (6, 2): 5,
    # (7, 1): 5, (2, 5): 6, (1, 4): 6, (4, 5): 6, (3, 6): 6, (5, 4): 6, (6, 3): 6, (7, 2): 6, (1, 5): 7, (0, 4): 7,
    # (2, 6): 7, (4, 6): 7, (3, 7): 7, (6, 4): 7, (7, 3): 7, (0, 3): 8, (0, 5): 8, (1, 6): 8, (2, 7): 8, (5, 6): 8,
    # (7, 4): 8, (6, 5): 8, (0, 6): 9, (1, 7): 9, (5, 7): 9, (7, 5): 9, (0, 7): 10, (6, 7): 10, (7, 6): 10, (7, 7): 11},
    # Comment: goal(3, 0), 障碍物的点不在其中, 距离是折线的最短路径。
    # @param agents (list): [0] or [1] or [2] or [3]
    def __init__(self, input_map, input_starts, input_goals, input_heuristics, agents, input_constraints):
        """my_map   - list of lists specifying obstacle positions
        starts      - [(x1, y1), (x2, y2), ...] list of start locations for CBS
        goals       - [(x1, y1), (x2, y2), ...] list of goal locations for CBS
        agents      - the agent (CBS) or meta-agent of the agent (MA-CBS) involved in collision
        constraints - list of dict constraints generated by a CBS splitter; dict = {agent,loc,timestep,positive}
        """

        self.map = input_map
        self.num_generated = 0
        self.num_expanded = 0
        self.CPU_time = 0

        self.open_list = []
        self.closed_list = dict()

        self.constraints = input_constraints  # to be used to create c_table

        self.agents = agents

        # check if meta_agent is only a simple agent (from basic CBS)
        if not isinstance(agents, list):
            self.agents = [agents]
            # print(meta_agent)

            # add meta_agent keys to constraints
            for constraint in self.constraints:
                constraint['meta_agent'] = {constraint['agent']}

        print("input agents:", self.agents)

        # FILTER BY INDEX FOR STARTS AND GOALS AND HEURISTICS
        self.starts = [input_starts[a] for a in self.agents]
        self.heuristics = [input_heuristics[a] for a in self.agents]
        self.goals = [input_goals[a] for a in self.agents]

        self.constraint_table = []  # constraint table
        self.max_constraints = np.zeros((len(self.agents),), dtype=int)

    def push_node(self, node):
        f_value = node['g_val'] + node['h_val']
        paths_left = node['reached_goal'].count(False)

        # heapq.heappush(self.open_list, (f_value, node['g_val'], node['h_val'], paths_left, node['loc'], self.num_generated, node))
        heapq.heappush(self.open_list, (f_value, node['h_val'], node['loc'], self.num_generated, node))
        self.num_generated += 1

    def pop_node(self):
        _, _, _, id, curr = heapq.heappop(self.open_list)

        # print(curr['h_val'])
        self.num_expanded += 1
        return curr

    # return a table that contains the list of constraints of all agents for each time step.
    # agent是一个整数，表示代理的编号。
    # 例如，如果有三个代理，那么它们的编号分别是0, 1, 2。
    # 这个函数中的agent参数是用来指定要为哪个代理构建约束表的。
    def build_constraint_table(self, input_agent_id):
        constraint_table = dict()

        if not self.constraints:
            return constraint_table
        # 如果self.constraints是空的，也就是说没有给定任何约束，那么就直接返回constraint_table，也就是空的约束表。
        # 这么做的原因是，如果没有约束，那么就不需要构建约束表，也不需要考虑约束对搜索的影响。这样可以节省时间和空间，提高效率。

        for constraint in self.constraints:
            # print("show constraint:", constraint)
            # constraint: {'agent': 0, 'loc': [(1, 1), (1, 0)], 'timestep': 1, 'positive': False, 'meta_agent': {0}}
            timestep = constraint['timestep']

            t_constraint = []
            if timestep in constraint_table:
                t_constraint = constraint_table[timestep]

            # positive constraint for agent
            if constraint['positive'] and constraint['agent'] == input_agent_id:

                # constraint_table[timestep].append(constraint)
                t_constraint.append(constraint)
                constraint_table[timestep] = t_constraint
            # and negative (external) constraint for agent
            elif not constraint['positive'] and constraint['agent'] == input_agent_id:
                # constraint_table[timestep].append(constraint)
                t_constraint.append(constraint)
                constraint_table[timestep] = t_constraint
                # enforce positive constraints from other agents (i.e. create neg constraint)
            elif constraint['positive']:
                neg_constraint = copy.deepcopy(constraint)
                neg_constraint['agent'] = input_agent_id
                # neg_constraint['meta_agent'] = meta_agent
                # if edge collision
                if len(constraint['loc']) == 2:
                    # switch traversal direction
                    prev_loc = constraint['loc'][1]
                    curr_loc = constraint['loc'][0]
                    neg_constraint['loc'] = [prev_loc, curr_loc]
                neg_constraint['positive'] = False
                # constraint_table[timestep].append(neg_constraint)
                t_constraint.append(neg_constraint)
                constraint_table[timestep] = t_constraint

        return constraint_table

    # returns if a move at timestep violates a "positive" or a "negative" constraint in c_table
    def constraint_violated(self, curr_loc, next_loc, timestep, c_table_agent, agent):

        # print("the move : {}, {}".format(curr_loc, next_loc))

        if timestep not in c_table_agent:
            return None

        for constraint in c_table_agent[timestep]:

            if agent == constraint['agent']:
                # vertex constraint
                if len(constraint['loc']) == 1:
                    # positive constraint
                    if constraint['positive'] and next_loc != constraint['loc'][0]:
                        # print("time {} positive constraint : {}".format(timestep, constraint))
                        return constraint
                    # negative constraint
                    elif not constraint['positive'] and next_loc == constraint['loc'][0]:
                        # print("time {} negative constraint : {}".format(timestep, constraint))
                        return constraint
                # edge constraint
                else:
                    if constraint['positive'] and constraint['loc'] != [curr_loc, next_loc]:
                        # print("time {} positive constraint : {}".format(timestep, constraint))
                        return constraint
                    if not constraint['positive'] and constraint['loc'] == [curr_loc, next_loc]:
                        # print("time {} negative constraint : {}".format(timestep, constraint))
                        return constraint

        return None

    # returns whether an agent at goal node at current timestep will violate a constraint in next timesteps
    def future_constraint_violated(self, curr_loc, timestep, max_timestep, c_table_agent, agent):

        for t in range(timestep + 1, max_timestep + 1):
            if t not in c_table_agent:
                continue

            for constraint in c_table_agent[t]:

                if agent == constraint['agent']:
                    # vertex constraint
                    if len(constraint['loc']) == 1:
                        # positive constraint
                        if constraint['positive'] and curr_loc != constraint['loc'][0]:
                            # print("future time {} positive constraint : {}".format(t, constraint))
                            return True
                        # negative constraint
                        elif not constraint['positive'] and curr_loc == constraint['loc'][0]:
                            # print("time {} negative constraint : {}".format(timestep, constraint))
                            # print("future time {} negative constraint : {}".format(t, constraint))
                            return True

        return False

    def generate_child_nodes(self, curr):

        children = []
        ma_dirs = product(list(range(5)),
                          repeat=len(self.agents))  # directions for move() for each agent: 0, 1, 2, 3, 4

        for dirs in ma_dirs:
            # print(dirs)
            invalid_move = False
            child_loc = []
            # move each agent for new timestep & check for (internal) conflicts with each other
            for i, a in enumerate(self.agents):
                aloc = move(curr['loc'][i], dirs[i])
                # vertex collision; check for duplicates in child_loc
                if aloc in child_loc:
                    invalid_move = True
                    # print("internal conflict")
                    break
                child_loc.append(move(curr['loc'][i], dirs[i]))

            if invalid_move:
                continue

            for i, a in enumerate(self.agents):
                # edge collision: check for matching locs in curr_loc and child_loc between two agents
                for j, a in enumerate(self.agents):
                    if i != j:
                        # print(ai, aj)
                        if child_loc[i] == curr['loc'][j] and child_loc[j] == curr['loc'][i]:
                            invalid_move = True

            if invalid_move:
                continue

            # check map constraints and external constraints
            for i, a in enumerate(self.agents):
                next_loc = child_loc[i]
                # agent out of map bounds
                if next_loc[0] < 0 or next_loc[0] >= len(self.map) or next_loc[1] < 0 or next_loc[1] >= len(
                        self.map[0]):
                    invalid_move = True
                # agechild_locnt collison with map obstacle
                elif self.map[next_loc[0]][next_loc[1]]:
                    invalid_move = True
                # agent is constrained by a negative external constraint
                elif self.constraint_violated(curr['loc'][i], next_loc, curr['timestep'] + 1, self.constraint_table[i],
                                              self.agents[i]):
                    invalid_move = True
                if invalid_move:
                    break

            if invalid_move:
                continue

            # find h_values for current moves
            h_value = 0
            for i in range(len(self.agents)):
                h_value += self.heuristics[i][child_loc[i]]

            h_test = sum([self.heuristics[i][child_loc[i]] for i in range(len(self.agents))])

            assert h_value == h_test

            # g_value = curr['g_val']+ curr['reached_goal'].count(False)
            num_moves = curr['reached_goal'].count(False)
            # print("(edge) cost (curr -> child) in a* tree == ", num_moves)

            g_value = curr['g_val'] + num_moves

            reached_goal = [False for i in range(len(self.agents))]
            # for i, a in enumerate(self.agents):
            #     # print(child_loc[i], goal_loc[i])
            #     # print(max_constraints[i], curr['timestep']+1)

            #     if child_loc[i] == self.goals[i] and (curr['timestep']+1 > self.max_constraints[i]):
            #         # print("agent ", a, 'has reached_goal at timestep ', curr['timestep'] + 1)
            #         # print (self.max_constraints[i])
            #         reached_goal[i] = True

            for i, a in enumerate(self.agents):

                if not reached_goal[i] and child_loc[i] == self.goals[i]:

                    if curr['timestep'] + 1 <= self.max_constraints[i]:
                        if not self.future_constraint_violated(child_loc[i], curr['timestep'] + 1,
                                                               self.max_constraints[i], self.constraint_table[i],
                                                               self.agents[i]):
                            # print("agent ", a, 'has found solution at timestep ', curr['timestep'] + 1)
                            # print ('MAX CONSTRIANT:', self.max_constraints[i])
                            reached_goal[i] = True
                            # self.max_constraints[i] differs for each node
                    else:
                        reached_goal[i] = True

            child = {'loc': child_loc,
                     'g_val': g_value,  # number of new locs (cost) added
                     'h_val': h_value,
                     'parent': curr,
                     'timestep': curr['timestep'] + 1,
                     'reached_goal': copy.deepcopy(reached_goal)
                     }

            # print(child)

            children.append(child)

        return children

    def compare_nodes(self, n1, n2):
        """Return true is n1 is better than n2."""

        # print(n1['g_val'] + n1['h_val'])
        # print(n2['g_val'] + n2['h_val'])

        assert isinstance(n1['g_val'] + n1['h_val'], int)
        assert isinstance(n2['g_val'] + n2['h_val'], int)

        return n1['g_val'] + n1['h_val'] < n2['g_val'] + n2['h_val']

    def find_path(self):
        self.start_time = time.time()
        print("> build constraint table")
        # ("agents", self.agents)

        for i, agent_id in enumerate(self.agents):
            table_i = self.build_constraint_table(agent_id)
            # print("table", i, a, table_i)
            self.constraint_table.append(table_i)
            if table_i.keys():
                self.max_constraints[i] = max(table_i.keys())

        h_value = sum([self.heuristics[i][self.starts[i]] for i in range(len(self.agents))])

        # assert h_value == h_test

        root = {'loc': [self.starts[j] for j in range(len(self.agents))],
                # 'F_val' : h_value, # only consider children with f_val == F_val
                'g_val': 0,
                'h_val': h_value,
                'parent': None,
                'timestep': 0,
                'reached_goal': [False for i in range(len(self.agents))]
                }

        # check if any agents are already at goal loc
        for i, a in enumerate(self.agents):
            if root['loc'][i] == self.goals[i]:

                if root['timestep'] <= self.max_constraints[i]:
                    if not self.future_constraint_violated(root['loc'][i], root['timestep'], self.max_constraints[i],
                                                           self.constraint_table[i], self.agents[i]):
                        root['reached_goal'][i] = True

                        self.max_constraints[i] = 0

        self.push_node(root)
        self.closed_list[(tuple(root['loc']), root['timestep'])] = [root]

        while len(self.open_list) > 0:
            curr = self.pop_node()
            solution_found = all(curr['reached_goal'][i] for i in range(len(self.agents)))
            # print(curr['reached_goal'] )

            if solution_found:
                return trace_path(curr, self.agents)

            children = self.generate_child_nodes(curr)

            for child in children:

                f_value = child['g_val'] + child['h_val']

                if (tuple(child['loc']), child['timestep']) in self.closed_list:
                    existing = self.closed_list[(tuple(child['loc']), child['timestep'])]
                    # if child not in existing_nodes:
                    #     print("child not in existing closed list")
                    if (child['g_val'] + child['h_val'] < existing['g_val'] + existing['h_val']) and (
                            child['g_val'] < existing['g_val']) and child['reached_goal'].count(False) <= existing[
                        'reached_goal'].count(False):
                        print("child is better than existing in closed list")
                        self.closed_list[(tuple(child['loc']), child['timestep'])] = child
                        self.push_node(child)
                else:
                    # print('bye child ',child['loc'])
                    self.closed_list[(tuple(child['loc']), child['timestep'])] = child
                    self.push_node(child)

        print('no solution')

        # print("\nEND OF A*\n") # comment out if needed
        return None
