#parameters
#K,L,iter,S,R
#need to compile efanna_graph & nsg

BuildGraph=./src/libs/efanna_graph/tests/test_nndescent
GistDest=./test/gist/gist_query.fvecs

#Build kNN Graph
#$BuildGraph $GistDest ./test/gist/gist_400nn.graph 60 70 500 ./test/gist/gist.nsg

