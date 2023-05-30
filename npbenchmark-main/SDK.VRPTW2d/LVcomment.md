# Lv's comment

TTP官方提供了许多论文，挑选结果最好的论文进行复现。(2023年5月30日)

### Q: 查找MAPF问题经常投稿的两个会议/期刊，好像是B类？

NB: ICAPS：国际自动规划与调度会议（International Conference on Automated Planning and Scheduling），是自动规划与调度领域的顶级会议，CCF B类。

尝试用强化学习解决MAPF问题。

### Q: 解决MAPF问题的大邻域搜索方法是什么？

根据我的搜索结果，解决MAPF问题的大邻域搜索方法（Large Neighborhood Search，LNS）是一种基于局部搜索的元启发式算法，可以快速地从一个初始解中修复冲突，得到一个可行解或者一个更优解。⁴

LNS的基本思想是：在每次迭代中，随机地选择一些冲突的智能体，并重新规划它们的路径，以减少或消除冲突。这样，可以在一个较大的邻域中搜索新的解，而不是只在一个较小的邻域中进行微调。⁴⁵⁶

LNS有多种变体，例如MAPF-LNS和MAPF-LNS2，它们分别使用不同的智能体选择策略和路径规划算法。⁴⁵⁶

Source: Conversation with Bing, 2023/5/30
(1) Anytime Multi-Agent Path Finding via Large Neighborhood Search. https://aamas.csc.liv.ac.uk/Proceedings/aamas2021/pdfs/p1581.pdf.
(2) MAPF-LNS2: Fast Repairing for Multi-Agent Path Finding via Large ... - AAAI. https://aaai.org/papers/10256-mapf-lns2-fast-repairing-for-multi-agent-path-finding-via-large-neighborhood-search/.
(3) MAPF-LNS2: fast repairing for Multi-Agent Path Finding via large .... https://research.monash.edu/en/publications/mapf-lns2-fast-repairing-for-multi-agent-path-finding-via-large-n.
(4) Google Maps. https://maps.google.ca/.
(5) My Maps – About – Google Maps. https://www.google.com/maps/about/mymaps/.
(6) Google Maps - Apps on Google Play. https://play.google.com/store/apps/details?id=com.google.android.apps.maps.