import time as time
import heapq
from itertools import product
import numpy as np
import copy


# Q: 页面中函数move_agent(current_location, chosen_direction)的功能是什么？
# 函数的功能是根据一个位置和一个方向，返回一个新的位置。
# 函数使用了一个列表来表示五个可能的方向，分别是不动、上、右、下、左。函数根据方向的索引，从列表中取出对应的元组，然后把元组中的两个值分别加到位置的两个坐标上，得到新的位置，并返回这个位置。
# 这个函数可以用于模拟一个代理在地图上的移动。
# Q: 页面中函数move_agent(current_location, chosen_direction)输入参数的数据类型和含义分别是什么？
# @param location (tuple): 表示一个位置，包含两个整数，分别是横坐标和纵坐标。例如，(1, 2)表示横坐标为1，纵坐标为2的位置。
# @param dir (int): 表示一个方向，取值范围是0到4，分别对应不动、上、右、下、左。例如，0表示不动，1表示上，2表示右，依此类推。
# Q: 页面中函数move_agent(current_location, chosen_direction)输出结果的数据类型和含义分别是什么？
# @return (tuple): 表示新的位置，包含两个整数，分别是横坐标和纵坐标。例如，(3, 4)表示横坐标为3，纵坐标为4的位置。
def move_agent(current_location, chosen_direction):
    directions = [(0, 0), (1, 0), (-1, 0), (0, 1), (0, -1)]
    return current_location[0] + directions[chosen_direction][0], current_location[1] + directions[chosen_direction][1]


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
    # @param input_constraints (list of dict):
    # [{'agent': 3, 'location': [(2, 0)], 'time_step': 2, 'positive': False, 'meta_agent': {3}},
    # {'agent': 3, 'location': [(2, 1), (3, 1)], 'time_step': 4, 'positive': False, 'meta_agent': {3}},
    # {'agent': 1, 'location': [(3, 4), (3, 3)], 'time_step': 7, 'positive': False, 'meta_agent': {1}},
    # {'agent': 1, 'location': [(1, 1)], 'time_step: 11, 'positive': False, 'meta_agent': {1}},
    # {'agent': 0, 'location': [(1, 1), (1, 0)], 'time_step': 1, 'positive': False, 'meta_agent': {0}}]
    # Q: 页面中函数__init__(self, input_map, input_starts, input_goals, input_heuristics, agents, input_constraints)
    # 输出结果的数据类型和含义分别是什么？
    # @return (void)
    def __init__(self, input_map, input_starts, input_goals, input_heuristics, agents, input_constraints):
        """my_map   - list of lists specifying obstacle positions
        starts      - [(x1, y1), (x2, y2), ...] list of start locations for CBS
        goals       - [(x1, y1), (x2, y2), ...] list of goal locations for CBS
        agents      - the agent (CBS) or meta-agent of the agent (MA-CBS) involved in collision
        constraints - list of dict constraints generated by a CBS splitter; dict = {agent,location,time_step,positive}
        """

        self.map = input_map
        self.num_generated = 0
        self.num_expanded = 0
        self.CPU_time = 0

        self.open_list = []
        self.closed_list = dict()

        self.constraints = input_constraints  # to be used to create constraint_table

        self.agents = [agents]
        print("self.agents:", self.agents)

        # FILTER BY INDEX FOR STARTS AND GOALS AND HEURISTICS
        # 因为是CBS, 所以下面这些都是只有1个成员的list.
        self.starts = [input_starts[self.agents[0]]]
        # print("self.starts: ", self.starts)
        self.goals = [input_goals[self.agents[0]]]
        self.heuristics = [input_heuristics[self.agents[0]]]

        self.constraint_table = []  # constraint table
        self.max_constraints = 0  # CBS的话是[0]
        # print("self.max_constraints:", self.max_constraints)

    # Q: 页面中函数def push_node(self, node)的功能是什么？
    # 把一个节点加入到开放列表中，用于后续的搜索。函数使用了一个元组来表示一个节点的评估值，包括f值（g值和h值的和）、h值、位置、生成编号和节点本身。
    # 函数使用了heapq模块来实现一个优先队列，把评估值最小的节点放在最前面。函数还记录了生成的节点的数量，用于统计信息。
    # 这个函数可以用于A*算法中，来维护一个待扩展的节点集合。
    # Q: 页面中函数def push_node(self, node)输入参数的数据类型和含义分别是什么？
    # @param node (dict): f_value, node['h_val'], node['location'], self.num_generated, node
    # Q: 页面中函数def push_node(self, node)输出结果的数据类型和含义分别是什么？
    # @return (void).
    def push_node(self, node):
        f_value = node['g_val'] + node['h_val']
        heapq.heappush(self.open_list, (f_value, node['h_val'], node['location'], self.num_generated, node))
        self.num_generated += 1

    # Q: 页面中函数def pop_node(self)的功能是什么？
    # 从开放列表中弹出一个节点，用于后续的搜索。
    # 函数使用了heapq模块来实现一个优先队列，把评估值最小的节点放在最前面。
    # 函数还记录了扩展的节点的数量，用于统计信息。
    # Q: 页面中函数def pop_node(self)输入参数的数据类型和含义分别是什么？
    # @param none.
    # Q: 页面中函数def pop_node(self)输出结果的数据类型和含义分别是什么？
    # void return.
    def pop_node(self):
        _, _, _, id, curr = heapq.heappop(self.open_list)

        # print(curr['h_val'])
        self.num_expanded += 1
        return curr

    # Q: 页面中函数def build_constraint_table(self, input_agent_id)的功能是什么？
    # 输入的agent id，构建一个约束表，用于存储该agent与其他agent之间的约束条件。
    # 约束表是一个字典，键是其他代理的id，值是一个列表，包含了该代理与其他代理共享的变量和值。
    # 这个函数可以用于协调图搜索算法中，来维护每个代理的局部问题。
    # return a table that contains the list of constraints of all agents for each time step.
    # agent是一个整数，表示代理的编号。
    # 例如，如果有三个代理，那么它们的编号分别是0, 1, 2。
    # 这个函数中的agent参数是用来指定要为哪个代理构建约束表的。
    # Q: 页面中函数def build_constraint_table(self, input_agent_id)输入参数的数据类型和含义分别是什么？
    # @param input_agent_id (int): 要构建约束表的代理的id。它必须是一个有效的代理id，否则会抛出异常。
    # Q: 页面中函数def build_constraint_table(self, input_agent_id)输出结果的数据类型和含义分别是什么？
    # return constraint_table (dict): dict前的数字代表time stamp
    # {2: [{'agent': 3, 'location': [(2, 0)], 'time_step': 2, 'positive': False, 'meta_agent': {3}}],
    # 4: [{'agent': 3, 'location': [(2, 1), (3, 1)], 'time_step': 4, 'positive': False, 'meta_agent': {3}}]}
    def build_constraint_table(self, input_agent_id):
        constraint_table = dict()

        if not self.constraints:
            return constraint_table
        # 如果self.constraints是空的，也就是说没有给定任何约束，那么就直接返回constraint_table，也就是空的约束表。
        # 这么做的原因是，如果没有约束，那么就不需要构建约束表，也不需要考虑约束对搜索的影响。这样可以节省时间和空间，提高效率。

        for constraint in self.constraints:
            # print("show constraint:", constraint)
            # constraint: {'agent': 0, 'location': [(1, 1), (1, 0)],
            # 'time_step': 1, 'positive': False, 'meta_agent': {0}}
            time_step = constraint['time_step']

            t_constraint = []
            if time_step in constraint_table:
                t_constraint = constraint_table[time_step]

            # positive constraint for agent
            if constraint['positive'] and constraint['agent'] == input_agent_id:

                # constraint_table[time_step].append(constraint)
                t_constraint.append(constraint)
                constraint_table[time_step] = t_constraint
            # and negative (external) constraint for agent
            elif not constraint['positive'] and constraint['agent'] == input_agent_id:
                # constraint_table[time_step].append(constraint)
                t_constraint.append(constraint)
                constraint_table[time_step] = t_constraint
                # enforce positive constraints from other agents (i.e. create neg constraint)
            elif constraint['positive']:
                neg_constraint = copy.deepcopy(constraint)
                neg_constraint['agent'] = input_agent_id
                # neg_constraint['meta_agent'] = meta_agent
                # if edge collision
                if len(constraint['location']) == 2:
                    # switch traversal direction
                    prev_loc = constraint['location'][1]
                    curr_loc = constraint['location'][0]
                    neg_constraint['location'] = [prev_loc, curr_loc]
                neg_constraint['positive'] = False
                # constraint_table[time_step].append(neg_constraint)
                t_constraint.append(neg_constraint)
                constraint_table[time_step] = t_constraint
        # print("constraint table:", constraint_table)
        return constraint_table

    # Q: 页面中函数def get_constraint_violated(self, curr_loc, next_loc, time_step, c_table_agent, agent)的功能是什么？
    # 检查当前代理在给定的时间步是否违反了与另一个代理的约束条件。
    # Q: 页面中函数def get_constraint_violated(self, curr_loc, next_loc, time_step, c_table_agent, agent)
    # 输入参数的数据类型和含义分别是什么？
    # @param curr_loc (tuple): 这是一个元组，表示当前agent的位置坐标。例如，(1, 2)表示当前代理在第一行第二列的位置。
    # @param next_loc (tuple): 这是一个元组，表示当前agent的下一个位置坐标。例如，(2, 2)表示当前代理要移动到第二行第二列的位置。
    # @param time_step (int): 表示当前的时间步。例如，0表示初始状态，1表示第一次移动后的状态，依此类推。
    # @param constraint_table_agent (dict): 表示当前agent的约束表。
    # @param agent (int): 这是一个整数，表示另一个代理的id。它必须是一个有效的代理id，否则会抛出异常。
    # Q: 页面中函数def get_constraint_violated(self, curr_loc, next_loc, time_step, c_table_agent, agent)
    # 输出结果的数据类型和含义分别是什么？
    # return constraint (dict): if a move at time_step violates a "positive" or a "negative" constraint in c_table
    # {2: [{'agent': 3, 'location': [(2, 0)], 'time_step': 2, 'positive': False, 'meta_agent': {3}}],
    # 4: [{'agent': 3, 'location': [(2, 1), (3, 1)], 'time_step': 4, 'positive': False, 'meta_agent': {3}}]}
    def get_constraint_violated(self, curr_loc, next_loc, time_step, constraint_table_agent, agent):

        # print("the move : {}, {}".format(curr_loc, next_loc))

        if time_step not in constraint_table_agent:
            return None

        for constraint in constraint_table_agent[time_step]:

            if agent == constraint['agent']:
                # vertex constraint
                if len(constraint['location']) == 1:
                    # positive constraint
                    if constraint['positive'] and next_loc != constraint['location'][0]:
                        # print("time {} positive constraint : {}".format(time_step, constraint))
                        return constraint
                    # negative constraint
                    elif not constraint['positive'] and next_loc == constraint['location'][0]:
                        # print("time {} negative constraint : {}".format(time_step, constraint))
                        return constraint
                # edge constraint
                else:
                    if constraint['positive'] and constraint['location'] != [curr_loc, next_loc]:
                        # print("time {} positive constraint : {}".format(time_step, constraint))
                        return constraint
                    if not constraint['positive'] and constraint['location'] == [curr_loc, next_loc]:
                        # print("time {} negative constraint : {}".format(time_step, constraint))
                        return constraint
        return None

    # Q: 页面中函数def future_constraint_violated(self, curr_loc, time_step, max_time_step, constraint_table_agent, agent)
    # 的功能是什么？
    # 检查一个代理在当前位置是否会在未来的时间步违反约束条件。
    # 函数遍历从当前时间步加一到最大时间步的范围，对于每个时间步，如果存在约束表，就遍历约束表中的每个约束，如果约束的代理和当前代理相同，就判断约束的类型和位置。如果是正向约束，就要求当前位置和约束位置相同，否则就返回True；如果是反向约束，就要求当前位置和约束位置不同，否则也返回True。如果没有发现任何违反的情况，就返回False。
    # 这个函数可以用于判断一个代理是否已经找到了最终的解决方案，或者是否需要继续搜索。
    # Q:页面中函数def future_constraint_violated(self, curr_loc, time_step, max_time_step, constraint_table_agent, agent)
    # 输入参数的数据类型和含义分别是什么？
    # @param curr_loc (tuple): 表示当前代理的位置坐标。例如，(1, 2)表示横坐标为1，纵坐标为2的位置。
    # @param time_step (int): 表示当前的时间步。例如，0表示初始状态，1表示第一次移动后的状态，依此类推。
    # @param max_time_step (int): 表示最大的时间步。这个参数用于限制搜索的范围，避免无限循环或超时。
    # @param constraint_table_agent (dict):
    # if a move at time_step violates a "positive" or a "negative" constraint in constraint_table
    # {2: [{'agent': 3, 'location': [(2, 0)], 'time_step': 2, 'positive': False, 'meta_agent': {3}}],
    #  4: [{'agent': 3, 'location': [(2, 1), (3, 1)], 'time_step': 4, 'positive': False, 'meta_agent': {3}}]}
    # @param agent (int) : 表示当前代理的编号。例如，如果有三个代理，那么它们的编号分别是0, 1, 2。
    # Q: 页面中函数def future_constraint_violated(self, curr_loc, time_step, max_time_step, constraint_table_agent, agent)
    # 输出结果的数据类型和含义分别是什么？
    # return true/false: 这是一个冲突检测的结果，表示当前代理在当前位置是否会在未来的时间步违反约束条件。如果返回True，表示有冲突，如果返回False，表示没有冲突。
    # 例如，如果当前代理的位置是(1, 1)，时间步是2，最大时间步是5，约束表是
    # {3: [{‘agent’: 0, ‘location’: [(1, 1)], ‘positive’: False}]},
    # 当前agent的编号是0，那么函数会返回True，因为在时间步3，代理0有一个反向约束，禁止它在(1, 1)位置。
    # returns whether an agent at goal node at current time_step will violate a constraint in next time_steps
    def future_constraint_violated(self, curr_loc, time_step, max_time_step, constraint_table_agent, agent):
        for t in range(time_step + 1, max_time_step + 1):
            if t not in constraint_table_agent:
                continue

            for constraint in constraint_table_agent[t]:
                if agent == constraint['agent']:
                    # vertex constraint
                    if len(constraint['location']) == 1:
                        # positive constraint
                        if constraint['positive'] and curr_loc != constraint['location'][0]:
                            # print("future time {} positive constraint : {}".format(t, constraint))
                            return True
                        # negative constraint
                        elif not constraint['positive'] and curr_loc == constraint['location'][0]:
                            # print("time {} negative constraint : {}".format(time_step, constraint))
                            # print("future time {} negative constraint : {}".format(t, constraint))
                            return True
        return False

    # Q: 页面中函数def generate_child_nodes(self, curr)的功能是什么？
    # 根据当前节点生成一组子节点，用于后续的搜索。
    # Q: 页面中函数def generate_child_nodes(self, curr)输入参数的数据类型和含义分别是什么？
    # @param current_node (dict): ‘location’: tuple，例如(1, 2), 表示agent的位置是(1, 2)。
    # ‘g_val’: 一个整数，表示从根节点到当前节点的路径长度。
    # ‘h_val’: 一个整数，表示从当前节点到目标节点的启发式估计。
    # ‘parent’: 一个字典，表示当前节点的父节点，如果当前节点是根节点，则为None。
    # ‘time_step’: 一个整数，表示当前节点的时间步。
    # ‘reached_goal’: 一个布尔值列表，表示每个代理是否已经到达目标位置，列表中的每个元素对应一个代理，例如[True, False]表示第一个代理已经到达目标位置，第二个代理还没有到达目标位置。
    # Q: 页面中函数def generate_child_nodes(self, curr)输出结果的数据类型和含义分别是什么？
    # return children (list): 每个元素是一个dict. ‘location’: tuple，例如(1, 2), 表示agent的位置是(1, 2)。
    # ‘g_val’: 一个整数，表示从根节点到当前节点的路径长度。
    # ‘h_val’: 一个整数，表示从当前节点到目标节点的启发式估计。
    # ‘parent’: 一个字典，表示当前节点的父节点，如果当前节点是根节点，则为None。
    # ‘time_step’: 一个整数，表示当前节点的时间步。
    # ‘reached_goal’: 一个布尔值列表，表示每个代理是否已经到达目标位置，列表中的每个元素对应一个代理，例如[True, False]表示第一个代理已经到达目标位置，第二个代理还没有到达目标位置。
    def generate_child_nodes(self, current_node):
        children = []
        # directions for move() for each agent: 0, 1, 2, 3, 4
        make_directions = product(list(range(5)), repeat=1)
        # print("ma_dirs:", ma_dirs)

        for dirs in make_directions:
            # print(dirs)
            invalid_move = False
            child_loc = []
            # move each agent for new time_step & check for (internal) conflicts with each other
            aloc = move_agent(current_node['location'][0], dirs[0])
            # vertex collision; check for duplicates in child_loc
            if aloc in child_loc:
                invalid_move = True
                # print("internal conflict")
                break
            child_loc.append(move_agent(current_node['location'][0], dirs[0]))

            if invalid_move:
                continue

            # check map constraints and external constraints
            for i in range(len(self.agents)):
                next_loc = child_loc[i]
                # agent out of map bounds
                if next_loc[0] < 0 or next_loc[0] >= len(self.map) or next_loc[1] < 0 or next_loc[1] >= len(
                        self.map[0]):
                    invalid_move = True
                # agechild_locnt collison with map obstacle
                elif self.map[next_loc[0]][next_loc[1]]:
                    invalid_move = True
                # agent is constrained by a negative external constraint
                elif self.get_constraint_violated(current_node['location'][i], next_loc, current_node['time_step'] + 1, self.constraint_table[i],
                                                  self.agents[0]):
                    invalid_move = True
                if invalid_move:
                    break

            if invalid_move:
                continue

            # find h_values for current moves
            h_value = 0
            h_value += self.heuristics[0][child_loc[0]]

            h_test = self.heuristics[0][child_loc[0]]

            assert h_value == h_test

            # g_value = curr['g_val']+ curr['reached_goal'].count(False)
            num_moves = current_node['reached_goal'].count(False)
            # print("(edge) cost (curr -> child) in a* tree == ", num_moves)

            g_value = current_node['g_val'] + num_moves

            reached_goal = [False]
            if not reached_goal[0] and child_loc[0] == self.goals[0]:

                if current_node['time_step'] + 1 <= self.max_constraints:
                    if not self.future_constraint_violated(child_loc[0], current_node['time_step'] + 1,
                                                           self.max_constraints, self.constraint_table[0],
                                                           self.agents[0]):
                        # print("agent ", a, 'has found solution at time_step ', curr['time_step'] + 1)
                        # print ('MAX CONSTRIANT:', self.max_constraints)
                        reached_goal[0] = True
                        # self.max_constraints differs for each node
                else:
                    reached_goal[0] = True

            child = {'location': child_loc,
                     'g_val': g_value,  # number of new locs (cost) added
                     'h_val': h_value,
                     'parent': current_node,
                     'time_step': current_node['time_step'] + 1,
                     'reached_goal': copy.deepcopy(reached_goal)
                     }

            # print(child)

            children.append(child)

        return children

    # Q: 页面中函数def compare_nodes(self, node_1, node_2)的功能是什么？
    # 判断node_1是否比node_2更好，即node_1的评估值是否比node_2的评估值更小。
    # 评估值是由g值和h值的和组成的，g值表示从根节点到当前节点的路径长度，h值表示从当前节点到目标节点的启发式估计。函数返回一个布尔值，如果n1的评估值小于n2的评估值，就返回True，否则返回False。
    # 这个函数可以用于A*算法中，来选择最优的节点进行扩展。
    # Q: 页面中函数def compare_nodes(self, node_1, node_2)输入参数的数据类型和含义分别是什么？
    # @param node_1 (dict): ‘location’: tuple，例如(1, 2), 表示agent的位置是(1, 2)。
    # ‘g_val’: 一个整数，表示从根节点到当前节点的路径长度。
    # ‘h_val’: 一个整数，表示从当前节点到目标节点的启发式估计。
    # ‘parent’: 一个字典，表示当前节点的父节点，如果当前节点是根节点，则为None。
    # ‘time_step’: 一个整数，表示当前节点的时间步。
    # ‘reached_goal’: 一个布尔值列表，表示每个代理是否已经到达目标位置，列表中的每个元素对应一个代理，例如[True, False]表示第一个代理已经到达目标位置，第二个代理还没有到达目标位置。
    # @param node_2 (dict): ‘location’: tuple，例如(1, 2), 表示agent的位置是(1, 2)。
    # ‘g_val’: 一个整数，表示从根节点到当前节点的路径长度。
    # ‘h_val’: 一个整数，表示从当前节点到目标节点的启发式估计。
    # ‘parent’: 一个字典，表示当前节点的父节点，如果当前节点是根节点，则为None。
    # ‘time_step’: 一个整数，表示当前节点的时间步。
    # ‘reached_goal’: 一个布尔值列表，表示每个代理是否已经到达目标位置，列表中的每个元素对应一个代理，例如[True, False]表示第一个代理已经到达目标位置，第二个代理还没有到达目标位置。
    # Q: 页面中函数def compare_nodes(self, node_1, node_2)输出结果的数据类型和含义分别是什么？
    # return (bool): 表示n1是否比n2更好，即n1的评估值是否比n2的评估值更小。
    # 评估值是由g值和h值的和组成的，g值表示从根节点到当前节点的路径长度，h值表示从当前节点到目标节点的启发式估计。如果n1的评估值小于n2的评估值，就返回True，否则返回False。
    # 这个函数可以用于A*算法中，来选择最优的节点进行扩展。
    def compare_nodes(self, node_1, node_2):
        """Return true is n1 is better than n2."""

        # print(n1['g_val'] + n1['h_val'])
        # print(n2['g_val'] + n2['h_val'])

        assert isinstance(node_1['g_val'] + node_1['h_val'], int)
        assert isinstance(node_2['g_val'] + node_2['h_val'], int)

        return node_1['g_val'] + node_1['h_val'] < node_2['g_val'] + node_2['h_val']

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
    def trace_path(self, goal_node, meta_agent):
        path = []
        for i in range(len(meta_agent)):
            path.append([])
        curr = goal_node
        while curr is not None:
            for i in range(len(meta_agent)):
                path[i].append(curr['location'][i])
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

    # Q: 页面中函数def low_level_search(self)的功能是什么？
    # 使用A*算法来寻找a group of agents的最优路径，即从起点到终点的最短且不冲突的路径。
    # Q: 页面中函数def low_level_search(self)输入参数的数据类型和含义分别是什么？
    # self
    # Q: 页面中函数def low_level_search(self)输出结果的数据类型和含义分别是什么？
    # return path (list): 表示一个路径，路径中的每个元素是一个元组，表示一个位置，元组中包含两个整数，分别是横坐标和纵坐标。
    def low_level_search(self):
        self.start_time = time.time()
        print("> build constraint table")
        # ("agents", self.agents)

        # 遍历所有的agents，获取每个agent的编号 agent_id
        # 在当前局面下，agents是只有1个元素的list, i永远是0
        # 在当前局面下，这里没有循环，只有1轮。
        constraint_table_i = self.build_constraint_table(self.agents[0])
        # print("constraint table: ", i, agent_id, constraint_table_i)
        self.constraint_table.append(constraint_table_i)
        print("self constraint table: ", self.constraint_table)
        if constraint_table_i.keys():  # 因为i是0，所以这里只有1轮
            self.max_constraints = max(constraint_table_i.keys())
        print("self.max_constraints:", self.max_constraints)

        h_value = self.heuristics[0][self.starts[0]]
        print("h_value:", h_value)

        # assert h_value == h_test

        root = {'location': [self.starts[0]],
                # 'F_val' : h_value, # only consider children with f_val == F_val
                'g_val': 0,
                'h_val': h_value,
                'parent': None,
                'time_step': 0,
                'reached_goal': [False]
                }

        # check if any agents are already at goal location
        if root['location'][0] == self.goals[0]:
            if root['time_step'] <= self.max_constraints:
                if not self.future_constraint_violated(root['location'][0], root['time_step'], self.max_constraints,
                                                       self.constraint_table[0], self.agents[0]):
                    root['reached_goal'][0] = True

                    self.max_constraints = 0

        self.push_node(root)
        self.closed_list[(tuple(root['location']), root['time_step'])] = [root]

        while len(self.open_list) > 0:
            curr = self.pop_node()
            solution_found = all(curr['reached_goal'][i] for i in range(len(self.agents)))
            # print(curr['reached_goal'] )

            if solution_found:
                return self.trace_path(curr, self.agents)

            children = self.generate_child_nodes(curr)

            for child in children:

                f_value = child['g_val'] + child['h_val']

                if (tuple(child['location']), child['time_step']) in self.closed_list:
                    this_node = self.closed_list[(tuple(child['location']), child['time_step'])]
                    if ((child['g_val'] + child['h_val'] < this_node['g_val'] + this_node['h_val']) and
                        (child['g_val'] < this_node['g_val']) and
                        (child['reached_goal'].count(False) <= this_node['reached_goal'].count(False))):
                        print("child is better than this node in closed list")
                        self.closed_list[(tuple(child['location']), child['time_step'])] = child
                        self.push_node(child)
                else:
                    # print('bye child ',child['location'])
                    self.closed_list[(tuple(child['location']), child['time_step'])] = child
                    self.push_node(child)

        print('no solution')

        # print("\nEND OF A*\n") # comment out if needed
        return None
