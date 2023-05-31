#include "LKH.h"
#include "Genetic.h"

/*
 * The ReadParameters function reads the name of a parameter file from
 * standard input and reads the problem parameters from this file.
 *
 * All entries of the parameter file are of the form <keyword >= <value>
 * (or <keyword><whitespace><value>), where <keyword> denotes an alphanumeric
 * keyword and <value> denotes alphanumeric or numeric data. Keywords are not
 * case sensitive.
 *
 * The order of specifications in the file is arbitrary. The following
 * specification is mandatory.
 *
 * PROBLEM_FILE = <string>
 * Specifies the name of the problem file.
 *
 * Additional control information may be supplied in the following format:
 *
 * ASCENT_CANDIDATES = <integer>
 * The number of candidate edges to be associated with each node during the
 * ascent. The candidate set is complemented such that every candidate edge
 * is associated with both its two end nodes.
 * Default: 50
 *
 * BACKBONE_TRIALS = <integer>
 * The number of backbone trials in each run.
 * Default: 0
 *
 * BACKTRACKING = { YES | NO }
 * Specifies whether a backtracking k-opt move is to be used as the first
 * move in a sequence of moves (where k = MOVE_TYPE).
 * Default: NO
 *
 * BWTSP = <integer> <integer> [ <integer> ]
 * Specifies the three parameters (B, Q, L) to a BWTSP instance.
 *   B: Number of black nodes.
 *   Q: Maximum number of white nodes on "black-to-black" paths.
 *   L: Maximum length of any "black-to-black" path.
 * Default: 0 0
 *
 * CANDIDATE_FILE = <string>
 * Specifies the name of a file to which the candidate sets are to be written.
 * If, however, the file already exists, the candidate edges are read from the
 * file.
 * The first line of the file contains the number of nodes.
 * Each of the following lines contains a node number, the number of
 * the dad of the node in the minimum spanning tree (0, if the node has no dad),
 * the number of candidate edges emanating from the node, followed by the
 * candidate edges.
 * For each candidate edge its end node number and alpha-value are given.
 * It is possible to give more than one CANDIDATE_FILE specification. In this
 * case the given files are read and the union of their candidate edges is
 * used as candidate sets.
 *
 * CANDIDATE_SET_TYPE = { ALPHA | DELAUNAY [ PURE ] | NEAREST-NEIGHBOR |
 *                        POPMUSIC | QUADRANT }
 * Specifies the candidate set type.
 * ALPHA is LKH's default type. ALPHA and POPMUSIC are applicable in general.
 * The other types can only be used for instances given by coordinates.
 * The optional suffix PURE for the DELAUNAY type specifies that only
 * edges of the Delaunay graph are used as candidates.
 * Default: ALPHA
 *
 * COMMENT <string>
 * A comment.
 *
 * # <string>
 * A comment.
 *
 * DEPOT = <integer>
 * Specifies the depot node.
 * Default: 1
 *
 * DISTANCE : <real>
 * The maximum length allowed for each route in a CVRP.
 * Default: DBL_MAX
 *
 * EDGE_FILE = <string>
 * Specifies the name of a file of candidate edges in Concorde format.
 * The first line of the file contains the number of nodes followed by
 * the number of edges.
 * Each of the following lines contains the number of the two end nodes of
 * an edge and its cost. The cost may be omitted. OBS: nodes are numbered
 * from zero.
 * It is possible to give more than one EDGE_FILE specification. In this
 * case the given files are read and the union of their candidate edges is
 * used as candidate sets.
 *
 * EOF
 * Terminates the input data. The entry is optional.
 *
 * EXCESS = <real>
 * The maximum alpha-value allowed for any candidate edge is set to
 * EXCESS times the absolute value of the lower bound of a solution
 * tour (determined by the ascent).
 * Default: 1.0/DIMENSION
 *
 * EXTERNAL_SALESMEN = <integer>
 * Specifies the number of external salesmen for an OCMTSP problem.
 * Default: 0
 *
 * EXTRA_CANDIDATES = <integer> [ SYMMETRIC ]
 * Number of extra candidate edges to be added to the candidate set
 * of each node. Their candidate set type may be specified after the
 * keyword EXTRA_CANDIDATE_SET_TYPE.
 * The integer may be followed by the keyword SYMMETRIC, signifying
 * that these extra candidate edges is to be complemented such
 * that each of them is associated with both its two end nodes.
 * Default: 0
 *
 * EXTRA_CANDIDATE_SET_TYPE = { NEAREST-NEIGHBOR | QUADRANT }
 * The candidate set type of extra candidate edges.
 * Default: QUADRANT
 *
 * GAIN23 = { YES | NO }
 * Specifies whether the Gain23 function is used.
 * Default: YES
 *
 * GAIN_CRITERION = { YES | NO }
 * Specifies whether Lin and Kernighan's gain criterion is used.
 * Default: YES
 *
 * INITIAL_PERIOD = <integer>
 * The length of the first period in the ascent.
 * Default: DIMENSION/2 (but at least 100)
 *
 * INITIAL_STEP_SIZE = <integer>
 * The initial step size used in the ascent.
 * Default: 1
 *
 * INITIAL_TOUR_ALGORITHM = { BORUVKA | CVRP | GREEDY | MOORE | MTSP |
 *       NEAREST-NEIGHBOR | QUICK-BORUVKA | SIERPINSKI | SOP | TSPDL | WALK }
 * Specifies the algorithm for obtaining an initial tour.
 * Default: WALK
 *
 * INITIAL_TOUR_FILE = <string>
 * Specifies the name of a file containing a tour to be used as the
 * initial tour in the search. The tour is given by a list of integers
 * giving the sequence in which the nodes are visited in the tour.
 * The tour is terminated by a -1.
 * See also INITIAL_TOUR_FRACTION.
 *
 * INITIAL_TOUR_FRACTION = <real in [0;1]>
 * Specifies the fraction of the initial tour to be constructed by means
 * of INITIAL_TOUR_FILE edges.
 * Default: 1.0
 *
 * INPUT_TOUR_FILE = <string>
 * Specifies the name of a file containing a tour. The tour is used to
 * limit the search (the last edge to be excluded in a non-gainful move
 * must not belong to the tour). In addition, the Alpha field of its
 * edges is set to zero. The tour is given by a list of integers giving
 * the sequence in which the nodes are visited in the tour. The tour is
 * terminated by a -1.
 *
 * KICK_TYPE = <integer>
 * Specifies the value of k for a random k-swap kick (an extension of the
 * double-bridge move). If KICK_TYPE is zero, then the LKH's special kicking
 * strategy, WALK, is used.
 * Default: 0
 *
 * KICKS = <integer>
 * Specifies the number of times to "kick" a tour found by Lin-Kernighan.
 * Each kick is a random k-swap kick-move. However, if KICKS is zero, then
 * LKH's special kicking strategy, WALK, is used.
 * Default: 1
 *
 * MAKESPAN = { YES | NO }
 * Specifies if makespan optimization is to be used for a TSPTW instance.
 * Default: NO
 *
 * MAX_BREADTH = <integer>
 * The maximum number of candidate edges considered at each level of
 * the search for a move.
 * Default: INT_MAX
 *
 * MAX_CANDIDATES = <integer> [ SYMMETRIC ]
 * The maximum number of candidate edges to be associated with each node.
 * The integer may be followed by the keyword SYMMETRIC, signifying
 * that the candidate set is to be complemented such that every candidate
 * edge is associated with both its two end nodes.
 * If MAX_CANDIDATES is zero the candidate sets are made up of the
 * edges represented in the CANDIDATE_FILEs, the INITIAL_TOUR_FILE,
 * the INPUT_TOUR_FILE, the SUBPROBLEM_TOUR_FILE, and the MERGE_TOUR_FILEs.
 * Default: 5
 *
 * MAX_SWAPS = <integer>
 * Specifies the maximum number of swaps (flips) allowed in any search
 * for a tour improvement.
 * Default: DIMENSION
 *
 * MAX_TRIALS = <integer>
 * The maximum number of trials in each run.
 * Default: DIMENSION
 *
 * MERGE_TOUR_FILE = <string>
 * Specifies the name of a tour to be merged. The edges of the tour are
 * added to the candidate sets.
 * It is possible to give more than two MERGE_TOUR_FILE specifications.
 *
 * MOVE_TYPE = <integer> [ SPECIAL ]
 * Specifies the move type to be used in as submove in Lin-Kernighan.
 * An integer value k >= 2 signifies that a sequential k-opt move is to be used.
 * The specifier SPECIAL can be given in order to use LKH-3’s specially
 * designed moves. For this type of moves, k must be 3 or 5.
 * Default: 5
 *
 * MTSP_MIN_SIZE = <integer>
 * Specifies the minimum number of cities each salesman must visit in an
 * MTSP or CVRP instance. If negative, its value is set to
 *     DIMENSION/(ceil(1.0*DIMENSION/MTSP_MAX_SIZE) + 1)
 * Default: 1
 *
 * MTSP_MAX_SIZE = <integer>
 * Specifies the maximum number of cities each salesman may visit in an
 * MTSP or CVRP instance.
 * Default: DIMENSION-1.
 *
 * MTSP_OBJECTIVE = [ MINMAX | MINMAX_SIZE | MINSUM ]
 * Specifies the objective function type for a multiple traveling salesman
 * problem.
 *      MINMAX      - Minimize the length of the longest route.
 *      MINMAX_SIZE - Minimize the size of the largest route.
 *      MINSUM      - Minimize the length of the overall tour.
 *                    All routes must satisfy the MTSP_MIN_SIZE and
 *                    MTSP_MAX_SIZE constraints.
 *
 * MTSP_SOLUTION_FILE = <string>
 * Specifies the name of a file where the solution of an MTSP (or VRP) instance
 * is to be written.
 * The character $ in the name has a spe¬cial meaning. All occurrences are
 * replaced by the cost of the tour.
 *
 * NONSEQUENTIAL_MOVE_TYPE = <integer>
 * Specifies the nonsequential move type to be used. A value K >= 4
 * signifies that attempts are made to improve a tour by nonsequential
 * k-opt moves where 4 <= k <= K. Note, however, that the effect depends
 * on the specifications of PATCHING_C and PATCHING_A.
 * Default: (MOVE_TYPE + PATCHING_C + PATCHING_A - 1)
 *
 * OUTPUT_TOUR_FILE = <string>
 * Specifies the name of a file where the best tour is to be written.
 * Each time a trial has produced a new best tour, the tour is written
 * to this file.
 * The character $ in the name has a special meaning. All occurrences
 * are replaced by the cost of the tour.
 *
 * OPTIMUM = <integer>
 * Known optimal tour length. If STOP_AT_OPTIMUM is YES, a run will be
 * terminated if the tour length becomes equal to this value.
 * Default: MINUS_INFINITY
 *
 * PATCHING_A = <integer> [ RESTRICTED | EXTENDED ]
 * The maximum number of disjoint alternating cycles to be used for
 * patching. An attempt to patch cycles is made if the corresponding
 * non-sequential move is gainful.
 * The integer may be followed by the keyword RESTRICTED or EXTENDED.
 * The keyword RESTRICTED signifies that gainful moves are only
 * considered if all its inclusion edges are candidate edges.
 * The keyword EXTENDED signifies that the non-sequential move need
 * not be gainful if only all its inclusion edges are candidate edges.
 * Default: 1
 *
 * PATCHING_C = <integer> [ RESTRICTED | EXTENDED ]
 * The maximum number of disjoint cycles to be patched in an attempt
 * to find a feasible and gainful move. An attempt to patch cycles is
 * made if the corresponding non-sequential move is gainful.
 * The integer may be followed by the keyword RESTRICTED or EXTENDED.
 * The keyword RESTRICTED signifies that gainful moves are only
 * considered if all its inclusion edges are candidate edges.
 * The keyword EXTENDED signifies that the non-sequential move need
 * not be gainful if only all its inclusion edges are candidate edges.
 * Default: 0
 *
 * PI_FILE = <string>
 * Specifies the name of a file to which penalties (Pi-values determined
 * by the ascent) are to be written. If the file already exists, the
 * penalties are read from the file, and the ascent is skipped.
 * The first line of the file contains the number of nodes. Each of the
 * following lines is of the form
 *       <integer> <integer>
 * where the first integer is a node number, and the second integer is
 * the Pi-value associated with the node.
 * The file name "0" represents a file with all Pi-values equal to zero.
 *
 * POPMUSIC_INITIAL_TOUR = { YES | NO }
 * Specifies whether the best POPMUSIC tour is to be used as intial tour
 * for Lin-Kernighan.
 * Default: NO
 *
 * POPMUSIC_MAX_NEIGHBORS = <int>
 * Maximum number of nearest neighbors used as candidates in 3-opt for
 * POPMUSIC.
 * Default: 5
 * 
 * POPMUSIC_SAMPLE_SIZE = <int>
 * Sample size.
 * Default: 10
 *
 * POPMUSIC_SOLUTIONS = <int>
 * Number of solutions to be generated.
 * Default: 50
 *
 * POPMUSIC_TRIALS = <int>
 * Number of trials used in iterated 3-opt for POPMUSIC.
 * If the value is zero, the number of trials is the size of the subpath
 * to be optimized.
 * Default: 1
 *
 * POPULATION_SIZE = <integer>
 * Specifies the maximum size of the population in the genetic algorithm.
 * Default: 0
 *
 * PRECISION = <integer>
 * The internal precision in the representation of transformed distances:
 *    d[i][j] = PRECISION*c[i][j] + pi[i] + pi[j],
 * where d[i][j], c[i][j], pi[i] and pi[j] are all integral.
 * Default: 100 (which corresponds to 2 decimal places)
 *
 * RECOMBINATION = { IPT | GPX2 | CLARIST }
 * Default: IPT
 *
 * RESTRICTED_SEARCH = { YES | NO }
 * Specifies whether the following search pruning technique is used:
 * The first edge to be broken in a move must not belong to the currently
 * best solution tour. When no solution tour is known, it must not belong
 * to the minimum spanning 1-tree.
 * Default: YES
 *
 * RUNS = <integer>
 * The total number of runs.
 * Default: 10
 *
 * SALESMEN = <integer>
 * Specifies the number of salesmen/vehicles.
 * Default: 1
 *
 * SCALE = <integer>
 * Scale factor. Distances are multiplied by this factor.
 * Default: 1
 *
 * SEED = <integer>
 * Specifies the initial seed for random number generation. If zero, the
 * seed is derived from the system clock.
 * Default: 1
 *
 * SINTEF_SOLUTION_FILE = <string>
 * Specifies the name of a file where the solution of an MTSP or VRP instance
 * is to be written. The solution is written in SINTEF format.
 * The character $ in the name has a special meaning. All occurrences are
 * replaced by the cost of the tour.
 *
 * SPECIAL
 * Equivalent to the following parameter specifications:
 *
 *    GAIN23 = NO
 *    KICKS = 1
 *    KICK_TYPE = 4
 *    MAX_SWAPS = 0
 *    MOVE_TYPE = 5 SPECIAL
 *    POPULATION_SIZE = 10
 *
 * STOP_AT_OPTIMUM = { YES | NO }
 * Specifies whether a run is stopped, if the tour length becomes equal
 * to OPTIMUM.
 * Default: YES
 *
 * SUBGRADIENT = { YES | NO }
 * Specifies whether the Pi-values should be determined by subgradient
 * optimization.
 * Default: YES
 *
 * SUBPROBLEM_SIZE = <integer> [ DELAUNAY | KARP | K-CENTER | K-MEANS | MOORE |
 *                               ROHE | SIERPINSKI ] [ BORDERS ] [ COMPRESSED ]
 * The number of nodes in a division of the original problem into subproblems.
 * The division is made according to the tour given by SUBPROBLEM_TOUR_FILE.
 * The value 0 signifies that no division is made.
 * By default, the subproblems are determined by subdividing the tour into
 * segments of equal size. However, the integer may be followed by DELAUNAY,
 * KARP, K-CENTER, K-MEANS, MOORE, ROHE or SIERPINSKI. DELAUNAY specifies that
 * the Delaunay partitioning scheme is used, KARP that Karp's partitioning
 * scheme is used, K-CENTER that a partitioning scheme based on K-center
 * clustering, K-MEANS that a partitioning scheme based on K-means clustering
 * is used, ROHE that Rohe's random rectangle/cube partitioning scheme is used,
 * and MOORE or SIERPINSKI that a partitioning scheme based on either a Moore
 * or Sierpinski space-filling curve is used.
 * The BORDERS specification signifies that the subproblems along the borders
 * between subproblems are to be solved too.
 * The COMPRESSED specification signifies that each subproblem is compressed by
 * removing from the problem all nodes with two incident subproblem tour edges
 * that belong to all tours to be merged (at least two MERGE_TOUR_FILEs should
 * be given).
 * Default: 0
 *
 * SUBPROBLEM_TOUR_FILE = <string>
 * Specifies the name of a file containing a tour to be used for dividing
 * the original problem into subproblems. The approximate number of nodes
 * in each is * given by SUBPROBLEM_SIZE.
 * The tour is given by a list of integers giving the sequence in which the
 * nodes are visited in the tour. The tour is terminated by a -1
 *
 * SUBSEQUENT_MOVE_TYPE = <integer> [ SPECIAL ]
 * Specifies the move type to be used for all moves following the first move
 * in a sequence of moves. The value K >= 2 signifies that a K-opt move is to
 * be used. The value 0 signifies that all moves are of the same type
 * (K = MOVE_TYPE).
 * The specifier SPECIAL can be given in order to use LKH-3’s specially
 * designed moves. For this type of moves, k must be 3 or 5.
 * Default: 0
 *
 * SUBSEQUENT_PATCHING = { YES | NO }
 * Specifies whether patching is used for moves following the first move
 * in a sequence of moves.
 * Default: YES
 *
 * TIME_LIMIT = <real>
 * Specifies a time limit in seconds for each run.
 * Default: DBL_MAX
 *
 * TOTAL_TIME_LIMIT = <real>
 * Specifies a total time limit in seconds.
 * Default: DBL_MAX
 *
 * TOUR_FILE = <string>
 * Specifies the name of a file where the best tour is to be written.
 * When a run has produced a new best tour, the tour is written to this file.
 * The character $ in the name has a special meaning. All occurrences
 * are replaced by the cost of the tour.
 *
 * TRACE_LEVEL = <integer>
 * Specifies the level of detail of the output given during the solution
 * process. The value 0 signifies a minimum amount of output. The higher
 * the value is the more information is given.
 * Default: 1
 *
 * VEHICLES = <integer>
 * Specifies the number of vehicles/salesmen.
 * Default: 1
 * 
 * List of abbreviations
 * ---------------------
 *
 * A string value may be abbreviated to the first few letters of the string,
 * if that abbreviation is unambiguous.
 *
 *     Value        Abbreviation
 *     ALPHA             A
 *     BORDERS           B
 *     BORUVKA           B
 *     CLARIST           C
 *     COMPRESSED        C
 *     CVRP              C
 *     DELAUNAY          D
 *     EXTENDED          E
 *     GPX2              G
 *     GREEDY            G
 *     IPT               I
 *     KARP              KA
 *     K-CENTER          K-C
 *     K-MEANS           K-M
 *     MOORE             MO
 *     MTSP              MT
 *     NEAREST-NEIGHBOR  N
 *     NO                N
 *     POPMUSIC          P
 *     PURE              P
 *     QUADRANT          Q
 *     QUICK-BORUVKA     Q
 *     RESTRICTED        R
 *     ROHE              R
 *     SIERPINSKI        SI
 *     SOP               SO
 *     SPECIAL           S
 *     SYMMETRIC         S
 *     TSPDL             T
 *     WALK              W
 *     YES               Y
 */

static char Delimiters[] = "= \n\t\r\f\v\xef\xbb\xbf";
static char *GetFileName(char *Line);
static char *ReadYesOrNo(int *V);
#undef max
static size_t max(size_t a, size_t b);

void ReadParameters()
{
    char *Line, *Keyword, *Token, *Name;
    unsigned int i;

    // 下面像是在给文档里介绍的一堆参数初始化
    ProblemFileName = PiFileName = InputTourFileName =
        OutputTourFileName = TourFileName = 0;
    ProblemFileName = "pr2392.tsp";
    CandidateFiles = MergeTourFiles = 0;
    AscentCandidates = 50;
    BackboneTrials = 0;
    Backtracking = 0;
    BWTSP_B = 0;
    BWTSP_Q = 0;
    BWTSP_L = INT_MAX;
    CandidateSetSymmetric = 0;
    CandidateSetType = ALPHA;
    Crossover = ERXT;
    DelaunayPartitioning = 0;
    DelaunayPure = 0;
    DemandDimension = 1;
    DistanceLimit = DBL_MAX;
    Excess = -1;
    ExternalSalesmen = 0;
    ExtraCandidates = 0;
    ExtraCandidateSetSymmetric = 0;
    ExtraCandidateSetType = QUADRANT;
    Gain23Used = 1;
    GainCriterionUsed = 1;
    GridSize = 1000000.0;
    InitialPeriod = -1;
    InitialStepSize = 0;
    InitialTourAlgorithm = WALK;
    InitialTourFraction = 1.0;
    KarpPartitioning = 0;
    KCenterPartitioning = 0;
    KMeansPartitioning = 0;
    Kicks = 1;
    KickType = 0;
    MaxBreadth = INT_MAX;
    MaxCandidates = 5;
    MaxPopulationSize = 0;
    MaxSwaps = -1;
    MaxTrials = -1;
    MoorePartitioning = 0;
    MoveType = 5; // fix
    MoveTypeSpecial = 0;
    MTSPDepot = 1;
    MTSPMinSize = 1;
    MTSPMaxSize = -1;
    MTSPObjective = -1;
    NonsequentialMoveType = -1;
    Optimum = 378032; // fix
    PatchingA = 2; // fix
    PatchingC = 3; // fix
    PatchingAExtended = 0;
    PatchingARestricted = 0;
    PatchingCExtended = 0;
    PatchingCRestricted = 0;
    Precision = 100;
    POPMUSIC_InitialTour = 0;
    POPMUSIC_MaxNeighbors = 5;
    POPMUSIC_SampleSize = 10;
    POPMUSIC_Solutions = 50;
    POPMUSIC_Trials = 1;
    Recombination = IPT;
    RestrictedSearch = 1;
    RohePartitioning = 0;
    Runs = 10; // 把RUNS固定
    Salesmen = 1;
    Scale = -1;
    Seed = 1;
    SierpinskiPartitioning = 0;
    StopAtOptimum = 1;
    Subgradient = 1;
    SubproblemBorders = 0;
    SubproblemsCompressed = 0;
    SubproblemSize = 0;
    SubsequentMoveType = 0;
    SubsequentMoveTypeSpecial = 0;
    SubsequentPatching = 1;
    TimeLimit = DBL_MAX;
    TotalTimeLimit = DBL_MAX;
    TraceLevel = 1;
    TSPTW_Makespan = 0;

    if (!ProblemFileName)
        eprintf("Problem file name is missing");
    if (SubproblemSize == 0 && SubproblemTourFileName != 0)
        eprintf("SUBPROBLEM_SIZE specification is missing");
    if (SubproblemSize > 0 && SubproblemTourFileName == 0)
        eprintf("SUBPROBLEM_TOUR_FILE specification is missing");
    if (SubproblemSize > 0 && Salesmen > 1)
        eprintf("SUBPROBLEM specification not possible for SALESMEN > 1");
    if (CandidateSetType != DELAUNAY)
        DelaunayPure = 0;
    free(LastLine);
    LastLine = 0;
}

// 就是读取文件名嘛，有什么好解读的；
static char *GetFileName(char *Line)
{
    char *Rest = strtok(Line, "\n\t\r\f"), *t;

    if (!Rest)
        return 0;
    while (isspace(*Rest))
        Rest++;
    if (!Line) {
        if (*Rest == '=')
            Rest++;
    }
    while (isspace(*Rest))
        Rest++;
    for (t = Rest + strlen(Rest) - 1; isspace(*t); t--)
        *t = '\0';
    if (!strlen(Rest))
        return 0;
    t = (char *) malloc(strlen(Rest) + 1);
    strcpy(t, Rest);
    return t;
}

// 就是按字面意思读取YES或者NO啊
static char *ReadYesOrNo(int *V)
{
    char *Token = strtok(0, Delimiters);

    if (Token) {
        unsigned int i;
        for (i = 0; i < strlen(Token); i++)
            Token[i] = (char) toupper(Token[i]);
        if (!strncmp(Token, "YES", strlen(Token)))
            *V = 1;
        else if (!strncmp(Token, "NO", strlen(Token)))
            *V = 0;
        else
            Token = 0;
    }
    return Token;
}

// 返回两个参数中较大的一个
static size_t max(size_t a, size_t b)
{
    return a > b ? a : b;
}
