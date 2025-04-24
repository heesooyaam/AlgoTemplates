cmake_minimum_required(VERSION 3.28)
project(AlgoTemplates)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack=268435456")

include_directories(Graphs)
add_executable(AlgoTemplates
#        Graphs/ShortestPathes/FordBellman/FordBellmanFractional.cpp
#        Graphs/Flows/MCMF/Matrix/JhonsonPotentialsInteger.cpp
#        Graphs/Flows/MCMF/List/JhonsonPotentialsOnList.cpp
#        Graphs/ShortestPathes/Dijkstra/DijkstraInteger.cpp
#        Graphs/ShortestPathes/Dijkstra/DijkstraFractional.cpp
#        Graphs/Flows/MCMF/Matrix/JhonsonPotentialsInteger.cpp
#        Graphs/Flows/MCMF/Matrix/JhonsonPotentialsFractional.cpp
#        Graphs/Flows/MCMF/Matrix/JhonsonPotentialsInteger.cpp
#        Graphs/Flows/MCMF/List/JhonsonPotentialsInteger.cpp
#        Graphs/Flows/MCMF/List/JhonsonPotentialsFractional.cpp
#        Structures/Trees/Segment/Persistent/Min.cpp
#        Structures/Trees/Segment/Persistent/LongestSegmentOfOnes.cpp
#        Structures/Trees/Segment/Persistent/Sum.cpp
#        Math/ModInt.cpp
#        Math/DivisorsSieve.cpp
#        Math/FastGcdLcm.cpp
#        Math/FactorizationSieve.cpp
#        Structures/Trees/Splay/ImplicitSplayTreeBase.cpp
#        Structures/Trees/Splay/ImplicitSplayTreeWithLazyOperations.cpp
#        Structures/Trees/Segment/Default/MaxAdd.cpp
#        Structures/Trees/Segment/Default/MaxAddWithPos.cpp
        Structures/Trees/Segment/Default/Template.cpp
        #        Structures/Trees/Segment/Default/MaxAddWithPos2.cpp
)
