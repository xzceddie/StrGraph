#ifndef NODE_H
#define NODE_H

#include <string>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <memory>
#include <iostream>
#include <src/include/node_listener.h>
#include <functional>
#include <thread>
#include <src/include/utils.h>

using OpFunc = std::function<std::string(const std::vector<std::string>&)>;

namespace StrGraph {

// Node can listen to other nodes;
class Node: public NodeListener {
    std::vector<NodeListener*> mListeners;
    mutable std::string mResult;
    int mReadyCount = 0;

public:
    Node() = default;
    Node( const std::string& result ) : mResult(result) {}
    void accept( const std::shared_ptr<NodeListener>& listener ) {
        mListeners.push_back( listener.get() );
    }
    void accept( NodeListener* const listener ) {
        mListeners.push_back( listener );
    }

    virtual std::string getValue() const {
        return mResult;
    }

    auto getListeners() const {
        return mListeners;
    }
    
    // NOTE: there will always be one listener, which is the DAG! If the node is in a DAG
    virtual bool isOutputNode() const {
        return getListeners().size() == 0;
    }

    virtual ~Node() {}

    virtual void setResult( const std::string& res_str ) const {
        mResult = res_str;
    }

    virtual void compute( const int verbose_level = 0 ) const;

    bool isReady() const {
        return mReadyCount == getSubCount();
    }

    void incrementReadyCount() {
        mReadyCount++;
    }

    void resetReadyCount() {
        mReadyCount = 0;
    }

    virtual void onDoneComputing() override ;

}; // class Node



// InputNodes on only contains a string as the data
class InputNode: public Node {
public:
    InputNode( const std::string& input ) : Node( input ) {}
}; // class InputNode


// template <typename OpType>
class OperatorNode: public Node {
    std::string mOutput;
    std::vector<std::weak_ptr<Node>> mInputs;
    mutable std::vector<std::string> mInputStrs;
    // OpType mOp;
    OpFunc mOp;

public:
    OperatorNode( const std::vector<std::shared_ptr<Node>>& inputs, const OpFunc& op )
    : mOp( op ) {
        for( auto& node : inputs ) {
            mInputs.push_back( node );
        }
        // std::transform( inputs.begin(), inputs.end(), 
        //                std::back_inserter( mInputStrs ),
        //                []( const auto& input ) { return input->getValue(); });
    }

    // NOTE: DAG will always subscribe to an operator node
    virtual bool isOutputNode() const override {
        return getListeners().size() == 1;
    }

    virtual void compute( const int verbose_level = 0 ) const override {
        if (verbose_level > 0) {
            std::lock_guard<std::mutex> lock( utils::StdOutMutexHolder::getInstance()->getMutex() );
            std::cout << "----> This Thread ID: " << std::this_thread::get_id() << std::endl;
        }
        std::vector<std::shared_ptr<Node>> tmp_parents;
        for( auto& input : mInputs ) {
            if( auto p = input.lock() ) {
                tmp_parents.push_back( p );
            } else {
                throw std::runtime_error( "Parent Node not found" );
            }
        }

        mInputStrs.clear();
        std::transform( tmp_parents.begin(), tmp_parents.end(),
                       std::back_inserter( mInputStrs ),
                       []( const auto& input ) { return input->getValue(); });
        setResult(mOp( mInputStrs ));
        for( auto& listener : getListeners() ) {
            listener->onDoneComputing();
        }
    }
}; // class OperatorNode

    
} // namespace StrGraph

#endif