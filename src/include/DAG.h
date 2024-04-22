#ifndef DAG_H
#define DAG_H

#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <src/include/node.h>
#include <iostream>
#include <thread>
#include <utility>

namespace StrGraph {

class DAG: public NodeListener {
private:
    mutable bool mDirty;
    mutable std::deque<Node*> mQueue;
    std::vector<std::shared_ptr<Node>> mInputNodes;
    std::set<int> mNodeIds;
    std::unordered_map<int, std::shared_ptr<Node>> mNodeMap;        // id -> node
    std::unordered_map<Node*, int> mInverseNodeMap; // node -> id
    mutable std::vector<std::string> mOutput;
    mutable std::vector<std::pair<int, std::string>> mOutputInfo;

public:
    // DAG() = default;

    // DAG( const std::vector<std::shared_ptr<Node>>& input_nodes )
    // : mDirty{ true }
    // , mInputNodes{ input_nodes } {
    //     for( int i = 0; i < input_nodes.size(); i++ ) {
    //         mNodeIds.insert( i );
    //         mNodeMap[i] = input_nodes[i];
    //         mQueue.push_back( input_nodes[i].get() );
    //     }
    // }

    DAG( const std::vector<std::string>& input_strings )
    : mDirty{ true }
    , mInputNodes{} {
        for( int i = 0; i < input_strings.size(); i++ ) {
            mNodeIds.insert( i );
            mNodeMap[i] = std::make_shared<Node>( input_strings[i] );
            mInputNodes.push_back( mNodeMap[i] );
            mInverseNodeMap[mNodeMap[i].get()] = i;
            mQueue.push_back( mNodeMap[i].get() );
        }
    }

    // static DAG create( const std::vector<std::string>& input_strings ) {
    //     return DAG( input_strings );
    // }
    // DAG( DAG&& other ) = default;
    // DAG( const DAG& other ) = default;

    std::vector<std::shared_ptr<Node>> getInputs() const {
        return mInputNodes;
    }

    // mark dirty
    void touch() {
        mDirty = true;
        for(auto& [nodeId, node]: mNodeMap) {
            node -> resetReadyCount();
        }
        mOutput.clear();
        mOutputInfo.clear();
    }

    void markDirty() { // alias
        touch();
    }

    // if is integrated, can query the result of any node
    bool isIntegrated() const {
        return !mDirty;
    }

    void resetInputContent( const int node_id, const std::string& new_content ) {
        mInputNodes[node_id]->setResult( new_content );
        touch();
    }

    int addOperatorNode( const std::vector<std::shared_ptr<Node>>& parent_nodes, const OpFunc& op ) {
        int id = mNodeIds.size();
        mNodeIds.insert( id );
        mNodeMap[id] = std::make_shared<OperatorNode>( parent_nodes, op );
        mInverseNodeMap[mNodeMap[id].get()] = id;
        for(auto& ele: parent_nodes) {
            mNodeMap[id]->subscribe( ele );
        }
        subscribe(mNodeMap[id]);
        return id;
    }

    // template<typename OpType>
    int addOperatorNodeById( const std::vector<int>& parent_nodes, const OpFunc& op ) {
        std::vector<std::shared_ptr<Node>> nodes;
        for( int i : parent_nodes ) {
            if( mNodeMap.find(i) != mNodeMap.end() )
                nodes.push_back( mNodeMap[i] );
            else throw std::runtime_error( "Node not found" );
        }
        return addOperatorNode( nodes, op );
    }


    std::vector<std::string> doCompute(const bool use_multi_thread = false, int verbose_level = 0) const {
        if (verbose_level > 0) {
            std::cout << "--> Start computing..., verbose_level: " << verbose_level << std::endl;
        }
        if( !mDirty )
            return mOutput;

        if( mQueue.empty() ) {
            for(auto& node: mInputNodes) {
                mQueue.push_back(node.get());
            }
        }
        std::vector<Node*> last_batch_nodes;
        std::vector<std::string> last_batch_result;
        while( !mQueue.empty() ) {
            last_batch_nodes.clear();
            last_batch_result.clear();
            while( !mQueue.empty() ) {
                auto node = mQueue.front();
                last_batch_nodes.push_back( node );
                mQueue.pop_front();
            }
            if (!use_multi_thread) {
                for(auto& node: last_batch_nodes) {
                    node->compute(verbose_level);
                    last_batch_result.push_back( node->getValue() );
                    if( node->isOutputNode() ) {
                        mOutputInfo.push_back(make_pair(mInverseNodeMap.at(node), node->getValue()));
                    }
                }
            } else {
                if (verbose_level > 0) {
                    std::cout << "---> Firing up multi-threads: " << std::endl;
                }
                std::vector<std::thread> last_batch_threads;
                for(auto& node: last_batch_nodes) {
                    std::thread t([&node, verbose_level](){ node->compute(verbose_level);});
                    last_batch_threads.push_back(std::move(t));
                }
                for( auto& t : last_batch_threads ) {
                    t.join();
                }
                for( auto& node : last_batch_nodes ) {
                    last_batch_result.push_back( node->getValue() );
                    if( node->isOutputNode() ) {
                        mOutputInfo.push_back(make_pair(mInverseNodeMap.at(node), node->getValue()));
                    }
                }
            }
        }
        mDirty = false;
        
        for(const auto&[node_id, content]: mOutputInfo) {
            mOutput.push_back( content );
        }
        return mOutput;
    }

    virtual void onReady( Node* node ) const override {
        mQueue.push_back( node );
    }
};


}; // namespace StrGraph


#endif
