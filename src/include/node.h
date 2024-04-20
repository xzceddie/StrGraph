#ifndef NODE_H
#define NODE_H

#include <string>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>
#include <memory>

namespace StrGraph {

class Node {
public:
}; // class Node


class InputNode: public Node {
    std::string mContent;
public:
    InputNode( const std::string& str )
    : mContent{ str }{
    }

    std::string getValue() const {
        return mContent;
    }

}; // class InputNode


template <typename OpType>
class OperatorNode: public Node {
    std::string mOutput;
    std::vector<std::shared_ptr<InputNode>> mInputs;
    std::vector<std::string> mInputStrs;
    OpType mOp;
public:
    OperatorNode( const std::vector<std::shared_ptr<InputNode>>& inputs, const OpType& op )
    : mInputs{ inputs }
    , mOp( op ) {
        std::transform( mInputs.begin(), mInputs.end(), 
                       std::back_inserter( mInputStrs ),
                       []( const auto& input ) { return input->getValue(); });
    }

    std::string compute() const {
        return mOp( mInputStrs );
    }
}; // class OperatorNode


class OutputNode: public Node {
    std::string mContent;
public:
    std::string getValue() const {
        return mContent;
    }
}; // class OutputNode
    
} // namespace StrGraph

#endif
