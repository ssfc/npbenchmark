function [fp, ft, ob, Wend]=TTP1Objective(distances,weights,values,av,tour,z,weightofKnapsack,vmax,vmin, rentRate)
% inputs:
% distances: a n by n matrix that shows the distances between the cities (there are n cities)
% weights: the weight of each item (1 by m)
% values: the profit of each item (1 by m)
% av: a m by n matrix showing if the ith item is available in the jth city. 
% tour: a 1 by n+1 array showing the tour (a complete tour)
% z: a 1 by m array, showing which item from which city (if z(i)==j, it means item i from city j)
% weightofKnapsack: maximum weight of the knapsack
% vmax: maximum velocity
% vmin: minimum velocity
% rentRate: the rent rate of the knapsack

% outputs:
% fp: final profit gained form the picked items
% ft: the time takes to finish the tour (including changes of the speed)
% ob: objective value
% Wend: weight of the knapsack at the end of the tour
% City numbers start from 1

if(tour(1)~=tour(length(tour)))
    error('The last city must the same as the first city');
end;

Wc=0;
ft=0;
fp=0;
for i=1:length(tour)-1
    h=i+1;
    selecteditem = find(z==tour(i));
    
    if(sum(av(selecteditem, tour(i)))~=length(selecteditem))
        error('This item is not available at this city');
    end;
    
    if(~isempty(selecteditem))
        Wc=Wc+sum(weights(selecteditem));
        fp=fp+sum(values(selecteditem));
    end;
    ft=ft+(distances(tour(i),tour(h))/(vmax-Wc*(vmax-vmin)/weightofKnapsack)); % "1-" replaced by "vmax-" on 11/09/2016 to make code compatible with instances where vmax!=1.0
end;
Wend = weightofKnapsack - Wc;
ob=fp-ft*rentRate;
end
