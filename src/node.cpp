#include <src/include/DAG.h>



void StrGraph::NodeListener::subscribe( const std::shared_ptr<StrGraph::Node>& listen_on ) {
    mSub.push_back(listen_on);
    mSub.back()->accept( shared_from_this() );
}


void StrGraph::Node::compute() const {
    for( auto& listener : mListeners ) {
        listener->onDoneComputing();
    }
}

void StrGraph::Node::onDoneComputing() {
    incrementReadyCount();
    if( isReady() ) {
        for( auto& listener : mListeners ) {
            listener->onReady( std::enable_shared_from_this<StrGraph::Node>::shared_from_this() );
        } 
    }
}
