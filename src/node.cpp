#include <src/include/DAG.h>



void StrGraph::NodeListener::subscribe( const std::shared_ptr<StrGraph::Node>& listen_on ) {
    mSubCount++;
    listen_on->accept( this );
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
            listener->onReady( this );
        } 
    }
}
