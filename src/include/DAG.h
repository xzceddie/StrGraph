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

class DAG: public NodeListener {
private:
    mutable std::deque<std::shared_ptr<Node>> mQueue;
    std::vector<std::shared_ptr<Node>> mInputNodes;
    std::set<int> mNodeIds;
    std::unordered_map<int, std::shared_ptr<Node>> mNodeMap;
    std::vector<std::string> mOutput;

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
            if( mNodeMap.find(i) != mNodeMap.end() )
                nodes.push_back( mNodeMap[i] );
            else throw std::runtime_error( "Node not found" );
        }
        return addOperatorNode( nodes, op );
    }


    std::vector<std::string> doCompute() const {
        std::vector<std::shared_ptr<Node>> last_batch_nodes;
        std::vector<std::string> last_batch_result;
        while( !mQueue.empty() ) {
            last_batch_nodes.clear();
            last_batch_result.clear();
            while( !mQueue.empty() ) {
                auto node = mQueue.front();
                last_batch_nodes.push_back( node );
                mQueue.pop_back();
            }
            for(auto& node: last_batch_nodes) {
                node->compute();
                last_batch_result.push_back( node->getValue() );
            }
        }
        return last_batch_result;
    }

    virtual void onReady( const std::shared_ptr<Node>& node ) const override {
        mQueue.push_back( node );
    }
};


}; // namespace StrGraph


#endif
