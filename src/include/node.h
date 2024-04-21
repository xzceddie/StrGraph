#ifndef NODE_H
#define NODE_H

#include <string>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <memory>
#include <iostream>

namespace StrGraph {


class Node;
class NodeListener: public std::enable_shared_from_this<NodeListener> {
    // std::vector<std::shared_ptr<Node>> mSub{};
    int mSubCount = 0;
    
public:
    NodeListener() = default;
    void subscribe( const std::shared_ptr<Node>& listen_on ) ;

    NodeListener( const std::shared_ptr<Node>& listen_on ) {
        subscribe( listen_on );
    }

    virtual void onDoneComputing() {}
    virtual void onReady( const std::shared_ptr<Node>& node ) const {}
    int getSubCount() const { return mSubCount; }
};


class Node: public NodeListener, public std::enable_shared_from_this<Node> {
    std::vector<std::shared_ptr<NodeListener>> mListeners;
    mutable std::string mResult;
    int mReadyCount = 0;

public:
    Node() = default;
    Node( const std::string& result ) : mResult(result) {}
    void accept( const std::shared_ptr<NodeListener>& listener ) {
        mListeners.push_back( listener );
    }

    virtual std::string getValue() const {
        return mResult;
    }

    auto getListeners() const {
        return mListeners;
    }

    virtual ~Node() {}

    virtual void setResult( const std::string& res_str ) const {
        mResult = res_str;
    }

    virtual void compute() const;

    bool isReady() const {
        return mReadyCount == getSubCount();
    }

    void incrementReadyCount() {
        mReadyCount++;
    }

    virtual void onDoneComputing() override ;

}; // class Node



class InputNode: public Node {
public:
    InputNode( const std::string& input ) : Node( input ) {}
}; // class InputNode


template <typename OpType>
class OperatorNode: public Node {
    std::string mOutput;
    std::vector<std::weak_ptr<Node>> mInputs;
    mutable std::vector<std::string> mInputStrs;
    OpType mOp;

public:
    OperatorNode( const std::vector<std::shared_ptr<Node>>& inputs, const OpType& op )
    //  mInputs{ inputs }
    : mOp( op ) {
        for( auto& node : inputs ) {
            mInputs.push_back( node );
        }
        std::transform( inputs.begin(), inputs.end(), 
                       std::back_inserter( mInputStrs ),
                       []( const auto& input ) { return input->getValue(); });
    }

    virtual void compute() const override {
        std::vector<std::shared_ptr<Node>> tmp_parents;
        for( auto& input : mInputs ) {
            if( auto p = input.lock() ) {
                tmp_parents.push_back( p );
            } else {
                throw std::runtime_error( "Node not found" );
            }
        }
        mInputStrs.clear();
        std::transform( tmp_parents.begin(), tmp_parents.end(),
                       std::back_inserter( mInputStrs ),
                       []( const auto& input ) { return input->getValue(); });
        setResult(mOp( mInputStrs ));
        std::cout << "Result: " << getValue() << std::endl;
        for( auto& listener : getListeners() ) {
            listener->onDoneComputing();
        }
    }
}; // class OperatorNode

    
} // namespace StrGraph

#endif
