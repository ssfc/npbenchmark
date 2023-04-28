# Evolutionary computation for multicomponent problems: opportunities and future directions

(Prompt: 翻译本文标题)

进化计算用于多组分问题：机遇与未来方向

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2019%20Evolutionary%20computation%20for%20multicomponent%20problems--opportunities%20and%20future%20directions.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2019%20Evolutionary%20computation%20for%20multicomponent%20problems--opportunities%20and%20future%20directions.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文回顾了过去30年进化计算领域为解决难题而做出的各种尝试，探讨了这些尝试是否仍然符合当初的目标，以及现实世界问题的复杂性是否有所增加。我们给出了一些现代实际问题的例子，讨论了它们为什么难以解决，以及它们与进化计算领域研究的问题之间是否存在不匹配。我们鼓励进化计算社区更多地研究多组分问题，因为这些问题更能反映现实世界的复杂性和依赖性。

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

Optimization in Industry: Present Practices and Future Scopes; 46 times. 

### Q: Optimization in Industry: Present Practices and Future Scopes是什么期刊？

Optimization in Industry: Present Practices and Future Scopes不是一个期刊，而是一本书。这本书由Shubhabrata Datta和J. Paulo Davim编辑，于2019年由Springer出版¹。这本书介绍了不同的方法来解决工业问题，如产品设计，过程优化，质量提升，生产力提高和成本最小化，并且包含了一些经典的和进化的优化工具在工业问题上的应用案例¹²。

### Q: Springer是什么？

Springer 是一个全球性的学术出版社和科技服务提供商，总部位于德国柏林。该公司成立于1842年，拥有超过 2600 多个期刊和出版物，覆盖了自然科学、医学、工程学、计算机科学、社会科学和人文科学等各个领域。Springer 出版的期刊和书籍是世界各地学者和研究人员最为重要的参考文献之一。此外，Springer 还提供一系列科技服务，包括数据库、在线图书馆、会议等。Springer 出版的一些知名期刊包括 Nature、Science China、Communications in Mathematical Physics、Journal of Business Ethics 等。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有四位，分别是：

- Mohammad Reza Bonyadi，是Rio Tinto公司的数据科学主管顾问，也是昆士兰大学先进成像中心的研究员。他的研究方向是优化算法，机器学习，计算智能等¹²。
- Zbigniew Michalewicz，是阿德莱德大学计算机科学系的教授，也是波兰科学院计算机科学研究所，波兰日本信息技术学院和Complexica公司的兼职教授。他的研究方向是进化计算，人工智能，优化等¹²³。
- Frank Neumann，是阿德莱德大学计算机科学系的教授，也是Optimisation and Logistics研究小组的负责人。他的研究方向是进化计算，组合优化，生物信息学等² 。
- Markus Wagner，是阿德莱德大学计算机科学系的副教授，也是Optimisation and Logistics研究小组的成员。他的研究方向是进化计算，多目标优化，软件工程等² 。

## 1 Motivation

(Prompt: Generalize Section "Motivation" of this article in Chinese) 

这一节的主要动机是探讨进化计算（Evolutionary Computation, EC）领域的研究与实际应用之间的差距，以及如何缩小这一差距。作者指出，过去30年来，EC领域的研究者们致力于设计适合解决难题的优化方法，但是在真实世界中很难找到基于EC的应用。作者分析了造成这种不匹配的原因，其中最主要的是：（a）**实验关注单一组件的基准问题，而忽略了真实世界问题的多组件性和相互依赖性**；（b）运筹学方法在工业界占据主导地位。作者认为，如果EC领域想要发展出强大的真实世界应用，就需要更多地研究多组件问题，并且利用EC技术的灵活性来应对这类问题的复杂性。

## 2 Two Examples

(Prompt: Generalize Section "Two Examples" of this article in Chinese)

这一节给出了两个真实世界的多组件问题的例子，分别是水箱运输问题和葡萄酒供应链问题。水箱运输问题涉及到如何选择水箱，如何打包水箱，如何选择基地，以及如何规划运输路线，以最大化运输的总价值。葡萄酒供应链问题涉及到如何决定葡萄的收获时间，如何安排压榨机，如何分配储存罐，以及如何安排装瓶线，以及时地完成订单并最小化无用的储存。这两个问题都表现出了**不同组件之间的依赖性**和相互影响，因此不能简单地将它们分解为单一组件的问题来求解。

## 3 Lessons Learned - Dependencies

(Prompt: Generalize Section "Lessons Learned - Dependencies" of this article in Chinese)

## 4 Traveling Thief Problem (TTP)

(Prompt: Generalize Section "Traveling Thief Problem (TTP)" of this article in Chinese)

## 5 Discussion and Future Directions

(Prompt: Generalize Section "Discussion and Future Directions" of this article in Chinese)