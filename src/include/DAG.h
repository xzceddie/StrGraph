#ifndef DAG_H
#define DAG_H

#include <__tuple_dir/tuple_element.h>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <src/include/node.h>


namespace StrGraph {

class DAG {
private:
    std::queue<std::shared_ptr<Node>> mQueue;
    std::vector<std::shared_ptr<Node>> mInputNodes;
    std::set<int> mNodeIds;
    std::unordered_map<int, std::shared_ptr<Node>> mNodeMap;

public:
    DAG() = default;

    DAG( const std::vector<std::shared_ptr<Node>>& input_nodes )
    : mInputNodes{ input_nodes } {
        for( int i = 0; i < input_nodes.size(); i++ ) {
            mNodeIds.insert( i );
            mNodeMap[i] = input_nodes[i];
        }
    }

    std::vector<std::shared_ptr<Node>> getInputs() const {
        return mInputNodes;
    }

    template<typename OpType>
    int addOperatorNode( const std::vector<std::shared_ptr<Node>>& parent_nodes, const OpType& op ) {
        int id = mNodeIds.size();
        mNodeIds.insert( id );
        mNodeMap[id] = std::make_shared<OperatorNode<OpType>>( parent_nodes, op );
        return id;
    }

    template<typename OpType>
    int addOperatorNode( const std::vector<int>& parent_nodes, const OpType& op ) {
        std::vector<std::shared_ptr<Node>> nodes;
        for( int i : parent_nodes ) {
            nodes.push_back( mNodeMap[i] );
        }
        return addOperatorNode( nodes, op );
    }
};


}; // namespace StrGraph


#endif
