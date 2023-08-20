import time
import heapq
import random
# from single_agent_planner import compute_heuristics, a_star, get_location, get_sum_of_cost
# from pea_star import pea_star

from a_star_class import A_Star, get_sum_of_cost, compute_heuristics

from pea_star_class import PEA_Star


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


class CBSSolver(object):
    """The high-level search of CBS."""

    def __init__(self, input_map, input_starts, input_goals):
        """map   - list of lists specifying obstacle positions
        starts      - [(x1, y1), (x2, y2), ...] list of start locations
        goals       - [(x1, y1), (x2, y2), ...] list of goal locations
        """

        self.start_time = None
        self.map = input_map
        self.starts = input_starts
        self.goals = input_goals
        self.num_of_agents = len(input_goals)

        self.num_of_generated = 0
        self.num_of_expanded = 0
        self.CPU_time = 0

        self.open_list = []

        # compute heuristics for the low-level search
        # 看样子compute_heuristics只有在初始化的时候用到
        self.heuristics = []
        for goal in self.goals:
            self.heuristics.append(compute_heuristics(input_map, goal))
        # print("heuristics:", self.heuristics)

    def push_node(self, node):
        heapq.heappush(self.open_list, (node['cost'], len(node['collisions']), self.num_of_generated, node))
        # print("Generate node {}".format(self.num_of_generated))
        self.num_of_generated += 1

    def pop_node(self):
        _, _, id, node = heapq.heappop(self.open_list)
        print("Expand node {}".format(id))
        self.num_of_expanded += 1
        return node

    # Task 3.1: Return the first collision that occurs between two agents paths (or None if there is no collision)
    def detect_two_paths_first_collision(self, path1, path2):
        # There are two types of collisions: vertex collision and edge collision.
        # A vertex collision occurs if both robots occupy the same location at the same timestep
        # An edge collision occurs if the robots swap their location at the same timestep.
        # Use "get_location(path, t)" to get the location of a robot at time t.
        t_range = max(len(path1), len(path2))
        for t in range(t_range):
            current_location_1 = get_location(path1, t)
            current_location_2 = get_location(path2, t)
            next_location_1 = get_location(path1, t + 1)
            next_location_2 = get_location(path2, t + 1)
            if next_location_1 == next_location_2:  # vertex conflict
                return [next_location_1], t
            if [current_location_1, next_location_1] == [next_location_2, current_location_2]:  # edge conflict
                return [next_location_2, current_location_2], t  # return的值有可能是1个元素(vertex)也可能是俩元素(edge)

        return None

    # Task 3.1: Return a list of first collisions between all agent pairs.
    def detect_all_paths_first_collisions(self, paths):
        # A collision can be represented as dictionary that contains the id of the two agents, the vertex or edge
        # causing the collision, and the timestep at which the collision occurred.
        # Use your detect_collision function to find a collision between two robots.
        first_collisions = []
        for i in range(len(paths) - 1):
            for j in range(i + 1, len(paths)):
                if self.detect_two_paths_first_collision(paths[i], paths[j]) is not None:  # 不冲突就不记录了
                    location, t = self.detect_two_paths_first_collision(paths[i], paths[j])
                    first_collisions.append({'a1': i, 'a2': j, 'loc': location, 'timestep': t + 1})

        return first_collisions

    def paths_violate_constraint(self, constraint, paths):
        assert constraint['positive'] is True
        result = []  # store index of conflicted path
        for i in range(len(paths)):
            if i == constraint['agent']:
                continue
            current_location = get_location(paths[i], constraint['timestep'])
            prev_location = get_location(paths[i], constraint['timestep'] - 1)
            if len(constraint['loc']) == 1:  # vertex constraint
                if constraint['loc'][0] == current_location:
                    result.append(i)
            else:  # edge constraint
                if constraint['loc'][0] == prev_location or constraint['loc'][1] == current_location \
                        or constraint['loc'] == [current_location, prev_location]:
                    result.append(i)
        return result

    # Task 3.2: Return a list of (two) constraints to resolve the given collision
    def standard_splitting(self, collision):
        constraints = []
        # Vertex collision: the first constraint prevents the first agent to be at the specified location at the
        #                   specified timestep, and the second constraint prevents the second agent to be at the
        #                   specified location at the specified timestep.
        if len(collision['loc']) == 1:  # vertex collision
            constraints.append({'agent': collision['a1'],
                                'loc': collision['loc'],
                                'timestep': collision['timestep'],
                                'positive': False
                                })
            constraints.append({'agent': collision['a2'],
                                'loc': collision['loc'],
                                'timestep': collision['timestep'],
                                'positive': False
                                })
        # Edge collision: the first constraint prevents the first agent to traverse the specified edge at the
        #                 specified timestep, and the second constraint prevents the second agent to traverse the
        #                 specified edge at the specified timestep
        else:
            constraints.append({'agent': collision['a1'],
                                'loc': [collision['loc'][0], collision['loc'][1]],
                                'timestep': collision['timestep'],
                                'positive': False
                                })
            constraints.append({'agent': collision['a2'],
                                'loc': [collision['loc'][1], collision['loc'][0]],
                                'timestep': collision['timestep'],
                                'positive': False
                                })
        return constraints

    # Task 4.1: Return a list of (two) constraints to resolve the given collision
    def disjoint_splitting(self, collision):
        # Choose the agent randomly
        constraints = []
        agent = random.randint(0, 1)
        a = 'a' + str(agent + 1)
        # Vertex collision: the first constraint enforces one agent to be at the specified location at the
        #                   specified timestep, and the second constraint prevents the same agent to be at the
        #                   same location at the timestep.
        if len(collision['loc']) == 1:
            constraints.append({'agent': collision[a],
                                'loc': collision['loc'],
                                'timestep': collision['timestep'],
                                'positive': True
                                })
            constraints.append({'agent': collision[a],
                                'loc': collision['loc'],
                                'timestep': collision['timestep'],
                                'positive': False
                                })
        # Edge collision: the first constraint enforces one agent to traverse the specified edge at the
        #                 specified timestep, and the second constraint prevents the same agent to traverse the
        #                 specified edge at the specified timestep
        else:
            if agent == 0:
                constraints.append({'agent': collision[a],
                                    'loc': [collision['loc'][0], collision['loc'][1]],
                                    'timestep': collision['timestep'],
                                    'positive': True
                                    })
                constraints.append({'agent': collision[a],
                                    'loc': [collision['loc'][0], collision['loc'][1]],
                                    'timestep': collision['timestep'],
                                    'positive': False
                                    })
            else:
                constraints.append({'agent': collision[a],
                                    'loc': [collision['loc'][1], collision['loc'][0]],
                                    'timestep': collision['timestep'],
                                    'positive': True
                                    })
                constraints.append({'agent': collision[a],
                                    'loc': [collision['loc'][1], collision['loc'][0]],
                                    'timestep': collision['timestep'],
                                    'positive': False
                                    })
        return constraints


    def find_solution(self, disjoint, a_star_version):
        """ Finds paths for all agents from their start locations to their goal locations

        disjoint         - use disjoint splitting or not
        a_star_version   - version of A*; "a_star" or "pea_star"
        """

        self.start_time = time.time()

        if disjoint:
            splitter = self.disjoint_splitting
        else:
            splitter = self.standard_splitting

        print("USING: ", splitter)

        if a_star_version == "a_star":
            # AStar = a_star # not a class yet
            AStar_method = A_Star
        else:
            AStar_method = PEA_Star

        # Generate the root node
        # constraints   - list of constraints
        # paths         - list of paths, one for each agent
        #               [[(x11, y11), (x12, y12), ...], [(x21, y21), (x22, y22), ...], ...]
        # collisions     - list of collisions in paths

        # Implement A1 LINE 1
        # R.constraints = ∅  # 最开始无约束
        #
        root = {'cost': 0,  # (3) The total cost (N.cost) of the current solution /
                # (summation over all the single-agent path costs). /
                # We denote this cost the f-value of the node.
                'constraints': [],  # (1) A set of constraints (N.constraints).
                # The root of the CT contains an empty set of constraints. /
                # The child of a node in the CT inherits the constraints of the parent /
                # and adds one new constraint for one agent.
                'paths': [],  # (2) A solution (N.solution).
                # A set of k paths, one path for each agent. /
                # The path for agent ai must be consistent with the constraints of ai. /
                # Such paths are found by the low-level
                'collisions': []}
        # Evaluate A1 LINE 1
        # print("root:", root)

        # Implement A1 LINE 2
        # R.solution = find individual paths using the low-level()  # 用低层算法计算每个智能体的path
        for i in range(self.num_of_agents):  # Find initial path for each agent
            astar = AStar_method(self.map, self.starts, self.goals, self.heuristics, i, root['constraints'])
            path = astar.find_path()
            # print("find path:", path)

            # path = ma_star(self.my_map, self.starts, self.goals, self.heuristics,[i], root['constraints'])
            if path is None:
                raise BaseException('No solutions')
            root['paths'].append(path[0])
        # Print A1 LINE 2
        '''
        print("Evaluate A1 LINE 2:")
        for i in range(len(root['paths'])):
            print("agent", i, ":", root['paths'][i])
        '''

        # Implement A1 LINE 3
        # R.cost = SIC(R.solution) // 计算目标函数
        root['cost'] = get_sum_of_cost(root['paths'])
        print("root cost:", root['cost'])
        root['collisions'] = self.detect_all_paths_first_collisions(root['paths'])
        print("root collisions:", root['collisions'])
        # Print A1 LINE 3
        # root collisions:
        # [{'a1': 0, 'a2': 4, 'loc': [(1, 1), (1, 0)], 'timestep': 1},
        # {'a1': 1, 'a2': 2, 'loc': [(1, 1)], 'timestep': 11},
        # {'a1': 1, 'a2': 3, 'loc': [(3, 4), (3, 3)], 'timestep': 7},
        # {'a1': 1, 'a2': 4, 'loc': [(3, 4)], 'timestep': 6},
        # {'a1': 2, 'a2': 3, 'loc': [(3, 1), (2, 1)], 'timestep': 4},
        # {'a1': 2, 'a2': 4, 'loc': [(3, 1)], 'timestep': 3}]
        # Implement A1 LINE 4
        # insert R to OPEN
        self.push_node(root)  # push到open list里面
        print(self.open_list)

        # Task 3.3: High-Level Search
        #           Repeat the following as long as the open list is not empty:
        #             1. Get the next node from the open list (you can use self.pop_node()
        #             2. If this node has no collision, return solution
        #             3. Otherwise, choose the first collision and convert to a list of constraints (using your
        #                standard_splitting function). Add a new child node to your open list for each constraint
        #           Ensure to create a copy of any objects that your child nodes might inherit

        # Implement A1 LINE 5
        # while OPEN not empty do
        iteration = 0  # Iteration需要走144个，重复实验结果一样。
        while len(self.open_list) > 0 and iteration < 500:
            print("iteration: ", iteration)
            # if self.num_of_generated > 50000:
            #     print('reached maximum number of nodes. Returning...')
            #     return None
            # Implement A1 LINE 6
            # P ← the best node from OPEN // the lowest solution cost
            best_node = self.pop_node()
            # Implement A1 LINE 7
            # Validate the paths in P until a conflict occurs.
            # Implement A1 LINE 8
            # if P has no conflict then
            if not best_node['collisions']:  # 为空[]
                self.print_results(best_node)
                for path in best_node['paths']:
                    print(path)
                # Implement A1 LINE 9
                # return P.solution // P is goal
                # number of nodes generated/expanded for comparing implementations
                return best_node['paths'], self.num_of_generated, self.num_of_expanded
            # Implement A1 LINE 10
            # C ← first conflict (ai, aj , v, t) in P
            collision = best_node['collisions'].pop(0)
            # constraints = standard_splitting(collision)
            # constraints = disjoint_splitting(collision)
            constraints = splitter(collision)  # 将当前冲突分解为两个限制

            for constraint in constraints:
                q = {'cost': 0,
                     'constraints': [constraint],
                     'paths': [],
                     'collisions': []
                     }
                for c in best_node['constraints']:
                    if c not in q['constraints']:
                        q['constraints'].append(c)
                for path in best_node['paths']:
                    q['paths'].append(path)

                ai = constraint['agent']
                astar = AStar_method(self.map, self.starts, self.goals, self.heuristics, ai, q['constraints'])
                # print("constraint:", q['constraints'])
                path = astar.find_path()

                if path is not None:
                    q['paths'][ai] = path[0]
                    # task 4
                    continue_flag = False
                    if constraint['positive']:
                        vol = self.paths_violate_constraint(constraint, q['paths'])
                        for v in vol:
                            astar_v = AStar_method(self.map, self.starts, self.goals, self.heuristics, v, q['constraints'])
                            path_v = astar_v.find_path()
                            if path_v is None:
                                continue_flag = True
                            else:
                                q['paths'][v] = path_v[0]
                        if continue_flag:
                            continue
                    q['collisions'] = self.detect_all_paths_first_collisions(q['paths'])
                    q['cost'] = get_sum_of_cost(q['paths'])
                    self.push_node(q)

            iteration += 1
        return None

    def print_results(self, node):
        print("\n Found a solution! \n")
        CPU_time = time.time() - self.start_time
        print("CPU time (s):    {:.2f}".format(CPU_time))
        print("Sum of costs:    {}".format(get_sum_of_cost(node['paths'])))
        print("Expanded nodes:  {}".format(self.num_of_expanded))
        print("Generated nodes: {}".format(self.num_of_generated))

        print("Solution:")
        for i in range(len(node['paths'])):
            print("agent", i, ": ", node['paths'][i])
