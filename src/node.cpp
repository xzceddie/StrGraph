#include <src/include/DAG.h>



void StrGraph::NodeListener::subscribe( const std::shared_ptr<StrGraph::Node>& listen_on ) {
    mSubCount++;
    listen_on->accept( this );
}


void StrGraph::Node::compute( const int verbose_level ) const {
    if (verbose_level > 0) {
        std::lock_guard<std::mutex> lock( utils::StdOutMutexHolder::getInstance()->getMutex() );
        std::cout << "Thread ID: " << std::this_thread::get_id() << std::endl;
    }
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
