#ifndef NODE_NODE_LISTENER_H
#define NODE_NODE_LISTENER_H


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
    // virtual void onReady( const std::shared_ptr<Node>& node ) const {}
    virtual void onReady( Node* node ) const {}
    int getSubCount() const { return mSubCount; }
};
    
} // namespace StrGraph



#endif
