#include "Segment.h"
#include "LKH.h"
#include "Hashing.h"
#include "Sequence.h"
#include "BIT.h"
#include "Genetic.h"

/*
 * The LinKernighan function seeks to improve a tour by sequential
 * and non-sequential edge exchanges.
 *
 * The function returns the cost of the resulting tour.
 */

int* LKH(char* fileName,std::vector<std::vector<int>> & allTours)
{
	GainType Cost, OldOptimum;
	double Time, LastTime;
	Node* N;
	int i;

	/* Read the specification of the problem */
	ParameterFileName = fileName;
	ReadParameters();
	StartTime = LastTime = GetTime();
	MaxMatrixDimension = 20000;
	MergeWithTour =
		Recombination == GPX2 ? MergeWithTourGPX2 :
		Recombination == CLARIST ? MergeWithTourCLARIST :
		MergeWithTourIPT;
	ReadProblem();
	if (SubproblemSize > 0) {
		if (DelaunayPartitioning)
			SolveDelaunaySubproblems();
		else if (KarpPartitioning)
			SolveKarpSubproblems();
		else if (KCenterPartitioning)
			SolveKCenterSubproblems();
		else if (KMeansPartitioning)
			SolveKMeansSubproblems();
		else if (RohePartitioning)
			SolveRoheSubproblems();
		else if (MoorePartitioning || SierpinskiPartitioning)
			SolveSFCSubproblems();
		else
			SolveTourSegmentSubproblems();
		return BestTour;
	}
	AllocateStructures();
	if (ProblemType == TSPTW)
		TSPTW_Reduce();
	if (ProblemType == VRPB || ProblemType == VRPBTW)
		VRPB_Reduce();
	if (ProblemType == PDPTW)
		PDPTW_Reduce();
	CreateCandidateSet();
	InitializeStatistics();

	std::vector<std::vector<int>> tours(Runs, std::vector<int>(DimensionSaved));
	std::vector<int> tmpTour(1 + Dimension);
//	std::vector<std::vector<int>> allTours;

	if (Norm != 0 || Penalty) {
		Norm = 9999;
		BestCost = PLUS_INFINITY;
		BestPenalty = CurrentPenalty = PLUS_INFINITY;
	}
	else {
		/* The ascent has solved the problem! */
		Optimum = BestCost = (GainType)LowerBound;
		UpdateStatistics(Optimum, GetTime() - LastTime);
		RecordBetterTour();
		RecordBestTour();
		CurrentPenalty = PLUS_INFINITY;
		BestPenalty = CurrentPenalty = Penalty ? Penalty() : 0;
		WriteTour(OutputTourFileName, BestTour, BestCost);
		WriteTour(TourFileName, BestTour, BestCost);
		Runs = 0;
	}

	/* Find a specified number (Runs) of local optima */

	for (Run = 1; Run <= Runs; Run++) {
		LastTime = GetTime();
		if (LastTime - StartTime >= TotalTimeLimit) {
			if (TraceLevel >= 1)
				printff("*** Time limit exceeded ***\n");
			Run--;
			break;
		}
//		Cost = FindTour();      /* using the Lin-Kernighan heuristic */
		if (m_IsAllToursNeeded == 1){
			Cost = FindTour(tmpTour, allTours);
		}
		else {
			Cost = FindTour();      /* using the Lin-Kernighan heuristic */
		}
			
		if (MaxPopulationSize > 1 && !TSPTW_Makespan) {
			/* Genetic algorithm */
			int i;
			for (i = 0; i < PopulationSize; i++) {
				GainType OldPenalty = CurrentPenalty;
				GainType OldCost = Cost;
				Cost = MergeTourWithIndividual(i);
				if (TraceLevel >= 1 &&
					(CurrentPenalty < OldPenalty ||
					(CurrentPenalty == OldPenalty && Cost < OldCost))) {
/*					if (CurrentPenalty)
						printff("  Merged with %d: Cost = " GainFormat "_"
							GainFormat, i + 1, CurrentPenalty, Cost);
					else
						printff("  Merged with %d: Cost = " GainFormat,
							i + 1, Cost);

					if (Optimum != MINUS_INFINITY && Optimum != 0) {
						if (ProblemType != CCVRP && ProblemType != TRP &&
							ProblemType != MLP &&
							MTSPObjective != MINMAX &&
							MTSPObjective != MINMAX_SIZE)
							printff(", Gap = %0.4f%%",
								100.0 * (Cost - Optimum) / Optimum);
						else
							printff(", Gap = %0.4f%%",
								100.0 * (CurrentPenalty - Optimum) /
								Optimum);
					}
					printff("\n");
*/
				}
			}
			if (!HasFitness(CurrentPenalty, Cost)) {
				if (PopulationSize < MaxPopulationSize) {
					AddToPopulation(CurrentPenalty, Cost);
					if (TraceLevel >= 1)
						PrintPopulation();
				}
				else if (SmallerFitness(CurrentPenalty, Cost,
					PopulationSize - 1)) {
					i = ReplacementIndividual(CurrentPenalty, Cost);
					ReplaceIndividualWithTour(i, CurrentPenalty, Cost);
					if (TraceLevel >= 1)
						PrintPopulation();
				}
			}
		}
		else if (Run > 1 && !TSPTW_Makespan)
			Cost = MergeTourWithBestTour();
		if (CurrentPenalty < BestPenalty ||
			(CurrentPenalty == BestPenalty && Cost < BestCost)) {
			BestPenalty = CurrentPenalty;
			BestCost = Cost;
			RecordBetterTour();
			RecordBestTour();
			WriteTour(TourFileName, BestTour, BestCost);
		}
		OldOptimum = Optimum;
		if (!Penalty ||
			(MTSPObjective != MINMAX && MTSPObjective != MINMAX_SIZE)) {
			if (CurrentPenalty == 0 && Cost < Optimum)
				Optimum = Cost;
		}
		else if (CurrentPenalty < Optimum)
			Optimum = CurrentPenalty;
		if (Optimum < OldOptimum) {
		//	printff("*** New OPTIMUM = " GainFormat " ***\n", Optimum);
			if (FirstNode->InputSuc) {
				Node* N = FirstNode;
				while ((N = N->InputSuc = N->Suc) != FirstNode);
			}
		}
		Time = fabs(GetTime() - LastTime);
		UpdateStatistics(Cost, Time);
		if (TraceLevel >= 1 && Cost != PLUS_INFINITY) {
	//		printff("Run %d: ", Run);
			//StatusReport(Cost, LastTime, "");
	//		printff("\n");
		}
		if (StopAtOptimum && MaxPopulationSize >= 1) {
			if (ProblemType != CCVRP && ProblemType != TRP &&
				ProblemType != MLP &&
				MTSPObjective != MINMAX &&
				MTSPObjective != MINMAX_SIZE ?
				CurrentPenalty == 0 && Cost == Optimum :
				CurrentPenalty == Optimum) {
				Runs = Run;
				break;
			}
		}
		if (PopulationSize >= 2 &&
			(PopulationSize == MaxPopulationSize ||
				Run >= 2 * MaxPopulationSize) && Run < Runs) {
			Node* N;
			int Parent1, Parent2;
			Parent1 = LinearSelection(PopulationSize, 1.25);
			do
				Parent2 = LinearSelection(PopulationSize, 1.25);
			while (Parent2 == Parent1);
			ApplyCrossover(Parent1, Parent2);
			N = FirstNode;
			do {
				if (ProblemType != HCP && ProblemType != HPP) {
					int d = C(N, N->Suc);
					AddCandidate(N, N->Suc, d, INT_MAX);
					AddCandidate(N->Suc, N, d, INT_MAX);
				}
				N = N->InitialSuc = N->Suc;
			} while (N != FirstNode);
		}
		SRandom(++Seed);
	}
	PrintStatistics();
	if (Salesmen > 1) {
		if (Dimension == DimensionSaved) {
			for (i = 1; i <= Dimension; i++) {
				N = &NodeSet[BestTour[i - 1]];
				(N->Suc = &NodeSet[BestTour[i]])->Pred = N;
			}
		}
		else {
			for (i = 1; i <= DimensionSaved; i++) {
				Node* N1 = &NodeSet[BestTour[i - 1]];
				Node* N2 = &NodeSet[BestTour[i]];
				Node* M1 = &NodeSet[N1->Id + DimensionSaved];
				Node* M2 = &NodeSet[N2->Id + DimensionSaved];
				(M1->Suc = N1)->Pred = M1;
				(N1->Suc = M2)->Pred = N1;
				(M2->Suc = N2)->Pred = M2;
			}
		}
		CurrentPenalty = BestPenalty;
		MTSP_Report(BestPenalty, BestCost);
		MTSP_WriteSolution(MTSPSolutionFileName, BestPenalty, BestCost);
		SINTEF_WriteSolution(SINTEFSolutionFileName, BestCost);
	}
	if (ProblemType == ACVRP ||
		ProblemType == BWTSP ||
		ProblemType == CCVRP ||
		ProblemType == CTSP ||
		ProblemType == CVRP ||
		ProblemType == CVRPTW ||
		ProblemType == MLP ||
		ProblemType == M_PDTSP ||
		ProblemType == M1_PDTSP ||
		MTSPObjective != -1 ||
		ProblemType == ONE_PDTSP ||
		ProblemType == OVRP ||
		ProblemType == PDTSP ||
		ProblemType == PDTSPL ||
		ProblemType == PDPTW ||
		ProblemType == RCTVRP ||
		ProblemType == RCTVRPTW ||
		ProblemType == SOP ||
		ProblemType == TRP ||
		ProblemType == TSPTW ||
		ProblemType == VRPB ||
		ProblemType == VRPBTW || ProblemType == VRPPD) {
	//	printff("Best %s solution:\n", Type);
		CurrentPenalty = BestPenalty;
		SOP_Report(BestCost);
	}
//	printff("\n");
//	system("pause");
	return BestTour;
}

GainType LinKernighan()
{
	GainType Cost, Gain, G0;
	int X2, i, it = 0;
	Node* t1, * t2, * SUCt1;
	Candidate* Nt1;
	Segment* S;
	SSegment* SS;
	double EntryTime = GetTime();

	Cost = 0;
	Reversed = 0;
	S = FirstSegment;
	i = 0;
	do {
		S->Size = 0;
		S->Rank = ++i;
		S->Reversed = 0;
		S->First = S->Last = 0;
	} while ((S = S->Suc) != FirstSegment);
	SS = FirstSSegment;
	i = 0;
	do {
		SS->Size = 0;
		SS->Rank = ++i;
		SS->Reversed = 0;
		SS->First = SS->Last = 0;
	} while ((SS = SS->Suc) != FirstSSegment);

	FirstActive = LastActive = 0;
	Swaps = 0;

	/* Compute the cost of the initial tour, Cost.
	   Compute the corresponding hash value, Hash.
	   Initialize the segment list.
	   Make all nodes "active" (so that they can be used as t1). */

	Cost = 0;
	Hash = 0;
	i = 0;
	t1 = FirstNode;
	do {
		t2 = t1->OldSuc = t1->Suc;
		t1->OldPred = t1->Pred;
		t1->Rank = ++i;
		Cost += (t1->SucCost = t2->PredCost = C(t1, t2)) - t1->Pi - t2->Pi;
		Hash ^= Rand[t1->Id] * Rand[t2->Id];
		t1->Cost = INT_MAX;
		for (Nt1 = t1->CandidateSet; (t2 = Nt1->To); Nt1++)
			if (t2 != t1->Pred && t2 != t1->Suc && Nt1->Cost < t1->Cost)
				t1->Cost = Nt1->Cost;
		t1->Parent = S;
		S->Size++;
		if (S->Size == 1)
			S->First = t1;
		S->Last = t1;
		if (SS->Size == 0)
			SS->First = S;
		S->Parent = SS;
		SS->Last = S;
		if (S->Size == GroupSize) {
			S = S->Suc;
			SS->Size++;
			if (SS->Size == SGroupSize)
				SS = SS->Suc;
		}
		t1->OldPredExcluded = t1->OldSucExcluded = 0;
		t1->Next = 0;
		if (KickType == 0 || Kicks == 0 || Trial == 1 ||
			!InBestTour(t1, t1->Pred) || !InBestTour(t1, t1->Suc))
			Activate(t1);
	} while ((t1 = t1->Suc) != FirstNode);
	if (S->Size < GroupSize)
		SS->Size++;
	Cost /= Precision;
	if (TSPTW_Makespan)
		Cost = TSPTW_CurrentMakespanCost = TSPTW_MakespanCost();
	CurrentPenalty = PLUS_INFINITY;
	CurrentPenalty = Penalty ? Penalty() : 0;
	if (TraceLevel >= 3 ||
		(TraceLevel == 2 &&
		(CurrentPenalty < BetterPenalty ||
			(CurrentPenalty == BetterPenalty && Cost < BetterCost))))
		//StatusReport(Cost, EntryTime, "");
	PredSucCostAvailable = 1;
	BIT_Update();

	/* Loop as long as improvements are found */
	do {
		/* Choose t1 as the first "active" node */
		while ((t1 = RemoveFirstActive())) {
			if (GetTime() - EntryTime >= TimeLimit ||
				GetTime() - StartTime >= TotalTimeLimit) {
				if (TraceLevel >= 1)
					printff("*** Time limit exceeded");
				goto End_LinKernighan;
			}
			/* t1 is now "passive" */
			SUCt1 = SUC(t1);
			if ((TraceLevel >= 3 || (TraceLevel == 2 && Trial == 1)) &&
				++it % (Dimension >= 100000 ? 10000 :
					Dimension >= 10000 ? 1000 : 100) == 0)
	//			printff("#%d: Time = %0.2f sec.\n",
		//			it, fabs(GetTime() - EntryTime));
			/* Choose t2 as one of t1's two neighbors on the tour */
			for (X2 = 1; X2 <= 2; X2++) {
				t2 = X2 == 1 ? PRED(t1) : SUCt1;
				if (FixedOrCommon(t1, t2) ||
					(RestrictedSearch && Near(t1, t2) &&
					(Trial == 1 ||
						(Trial > BackboneTrials &&
						(KickType == 0 || Kicks == 0)))))
					continue;
				G0 = C(t1, t2);
				/* Try to find a tour-improving chain of moves */
				do
					t2 = Swaps == 0 ? BestMove(t1, t2, &G0, &Gain) :
					BestSubsequentMove(t1, t2, &G0, &Gain);
				while (t2);
				if (PenaltyGain > 0 || Gain > 0) {
					/* An improvement has been found */
#ifdef HAVE_LONG_LONG
					assert(Gain % Precision == 0);
#else
					assert(fmod(Gain, Precision) == 0);
#endif
					Cost -= Gain / Precision;
					CurrentPenalty -= PenaltyGain;
					StoreTour();
					TSPTW_CurrentMakespanCost = Cost;
					if (TraceLevel >= 3 ||
						(TraceLevel == 2 &&
						(CurrentPenalty < BetterPenalty ||
							(CurrentPenalty == BetterPenalty &&
								Cost < BetterCost))))
						//StatusReport(Cost, EntryTime, "");
					if (HashSearch(HTable, Hash, Cost))
						goto End_LinKernighan;
					/* Make t1 "active" again */
					Activate(t1);
					OldSwaps = 0;
					break;
				}
				OldSwaps = 0;
				RestoreTour();
			}
		}
		if (HashSearch(HTable, Hash, Cost))
			goto End_LinKernighan;
		HashInsert(HTable, Hash, Cost);
		/* Try to find improvements using non-sequential 4/5-opt moves */
		CurrentPenalty = PLUS_INFINITY;
		CurrentPenalty = Penalty ? Penalty() : 0;
		PenaltyGain = Gain = 0;
		if (Gain23Used && ((Gain = Gain23()) > 0 || PenaltyGain > 0)) {
			/* An improvement has been found */
#ifdef HAVE_LONG_LONG
			assert(Gain % Precision == 0);
#else
			assert(fmod(Gain, Precision) == 0);
#endif
			Cost -= Gain / Precision;
			CurrentPenalty -= PenaltyGain;
			TSPTW_CurrentMakespanCost = Cost;
			StoreTour();
			if (TraceLevel >= 3 ||
				(TraceLevel == 2 &&
				(CurrentPenalty < BetterPenalty ||
					(CurrentPenalty == BetterPenalty && Cost < BetterCost))))
				//StatusReport(Cost, EntryTime, "+ ");
			if (HashSearch(HTable, Hash, Cost))
				goto End_LinKernighan;
		}
	} while (PenaltyGain > 0 || Gain > 0);
End_LinKernighan:
	PredSucCostAvailable = 0;
	NormalizeNodeList();
	NormalizeSegmentList();
	Reversed = 0;
	return Cost;
}
