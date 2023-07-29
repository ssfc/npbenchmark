#!/usr/bin/python
import argparse
import glob
from pathlib import Path
from cbs_basic import CBSSolver  # original cbs with standard/disjoint splitting

# cbs with different improvements
from icbs_cardinal_bypass import ICBS_CB_Solver  # only cardinal dectection and bypass
from icbs_complete import ICBS_Solver  # all improvements including MA-CBS

from independent import IndependentSolver
from prioritized import PrioritizedPlanningSolver
from visualize import Animation
from single_agent_planner import get_sum_of_cost

high_level_SOLVER = "CBS"

low_level_SOLVER = "a_star"


# 分别打印多个智能体起点和终点的位置
def print_mapf_instance(input_map, input_start, input_goal):
    print('Start locations')
    print_locations(input_map, input_start)
    print('Goal locations')
    print_locations(input_map, input_goal)


def print_locations(input_map, input_location):
    starts_map = [[-1 for _ in range(len(input_map[0]))] for _ in range(len(input_map))]
    for i in range(len(input_location)):
        starts_map[input_location[i][0]][input_location[i][1]] = i
    to_print = ''
    for x in range(len(input_map)):
        for y in range(len(input_map[0])):
            if starts_map[x][y] >= 0:
                to_print += str(starts_map[x][y]) + ' '
            elif input_map[x][y]:
                to_print += '@ '
            else:
                to_print += '. '
        to_print += '\n'
    print(to_print)


def import_mapf_instance(filename):
    f = Path(filename)
    if not f.is_file():
        raise BaseException(filename + " does not exist.")
    f = open(filename, 'r')
    # first line: #rows #columns
    line = f.readline()
    rows, columns = [int(x) for x in line.split(' ')]
    rows = int(rows)
    columns = int(columns)
    # #rows lines with the map
    imported_map = []
    for r in range(rows):
        line = f.readline()
        imported_map.append([])
        for cell in line:
            if cell == '@':
                imported_map[-1].append(True)
            elif cell == '.':
                imported_map[-1].append(False)
    # agents
    line = f.readline()
    num_agents = int(line)
    # agents lines with the start/goal positions
    import_start = []
    import_goal = []
    for a in range(num_agents):
        line = f.readline()
        sx, sy, gx, gy = [int(x) for x in line.split(' ')]  # 用的是屏幕坐标系，原点在左上角，x轴向下，y轴向右。
        import_start.append((sx, sy))
        import_goal.append((gx, gy))
    f.close()
    return imported_map, import_start, import_goal


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Runs various MAPF algorithms')
    parser.add_argument('--instance', type=str, default='instances/test_1.txt',
                        help='The name of the instance file(s)')
    parser.add_argument('--batch', action='store_true', default=False,
                        help='Use batch output instead of animation')
    parser.add_argument('--disjoint', action='store_true', default=False,
                        help='Use the disjoint splitting')
    parser.add_argument('--hlsolver', type=str, default=high_level_SOLVER,
                        help='The solver to use (one of: {CBS,ICBS_CB,ICBS}), defaults to ' + str(high_level_SOLVER))
    parser.add_argument('--llsolver', type=str, default=low_level_SOLVER,
                        help='The solver to use (one of: {a_star,pea_star,epea_star}), defaults to ' + str(low_level_SOLVER))
    args = parser.parse_args()

    result_file = open("results.csv", "w", buffering=1)

    # node_results_file = open("nodes-cleaned.csv", "w", buffering=1)

    nodes_gen_file = open("nodes-gen-cleaned.csv", "w", buffering=1)
    nodes_exp_file = open("nodes-exp-cleaned.csv", "w", buffering=1)

    if args.batch:
        input_instance = sorted(glob.glob("instances/test*"))
        # take test_1.txt for example
        # LINE 1: Map size is 8 columns 8 rows.
        # LINE 2 to 9: dot means space, @ means obstacles.
        # LINE 10: 5 means num of agents.
        # LINE 11 to 15: in each row, the first pair are (x, y) coordinates of agent start position,
        # the second pair are coordinates (x, y) of agent end position. The origin are in the upper left corner.
    else:
        input_instance = sorted(glob.glob(args.instance))

    for file in input_instance:

        print("***Import an instance***")

        print(file)  # print filename
        test_map, test_start, test_goal = import_mapf_instance(file)
        print_mapf_instance(test_map, test_start, test_goal)  # 在map中表明agent的起点和终点

        if args.hlsolver == "CBS":  # 执行CBS算法
            print("***Run CBS***")
            cbs = CBSSolver(test_map, test_start, test_goal)  # 原来是初始化一个类
            # solution = cbs.find_solution(args.disjoint)

            # if solution is not None:
            #     # print(solution)
            #     paths, nodes_gen, nodes_exp = [solution[i] for i in range(3)]
            #     if paths is None:
            #         raise BaseException('No solutions')  
            # else:
            #     raise BaseException('No solutions')

        elif args.hlsolver == "ICBS_CB":  # 执行ICBS with CB算法
            print("***Run ICBS with CB***")
            cbs = ICBS_CB_Solver(test_map, test_start, test_goal)  # 原来是初始化一个类

        elif args.hlsolver == "ICBS":  # 执行ICBS算法 Comment: 看来总共有3种算法
            print("***Run ICBS***")
            cbs = ICBS_Solver(test_map, test_start, test_goal)  # 原来是初始化一个类
            # solution = cbs.find_solution(args.disjoint)

            # if solution is not None:
            #     # print(solution)
            #     paths, nodes_gen, nodes_exp = [solution[i] for i in range(3)]
            #     if paths is None:
            #         raise BaseException('No solutions')  
            # else:
            #     raise BaseException('No solutions')

        # elif args.solver == "Independent":
        #     print("***Run Independent***")
        #     solver = IndependentSolver(my_map, starts, goals)
        #     paths, nodes_gen, nodes_exp = solver.find_solution()
        # elif args.solver == "Prioritized":
        #     print("***Run Prioritized***")
        #     solver = PrioritizedPlanningSolver(my_map, starts, goals)
        #     paths, nodes_gen, nodes_exp = solver.find_solution()

        else:
            raise RuntimeError("Unknown solver!")

        solution = cbs.find_solution(args.disjoint, args.llsolver)

        if solution is not None:
            # print(solution)
            paths, nodes_gen, nodes_exp = [solution[i] for i in range(3)]
            if paths is None:
                raise BaseException('No solutions')
        else:
            raise BaseException('No solutions')

        cost = get_sum_of_cost(paths)
        result_file.write("{},{}\n".format(file, cost))

        nodes_gen_file.write("{},{}\n".format(file, nodes_gen))
        nodes_exp_file.write("{},{}\n".format(file, nodes_exp))

        if not args.batch:
            print("***Test paths on a simulation***")
            animation = Animation(test_map, test_start, test_goal, paths)
            # animation.save("output.mp4", 1.0)
            animation.show()
            # animation.save('demo/fig.gif', 1)

    result_file.close()

# debug on laptop by Pycharm:
# cd to code directory.
# python run_experiments.py --instance instances/test_1.txt --hlsolver CBS


