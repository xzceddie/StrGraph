#ifndef UTILS_H
#define UTILS_H

#include <mutex>
#include <atomic>
#include <iostream>
#include <memory>
#include <string>

namespace StrGraph {

namespace utils {

class StdOutMutexHolder {
private:
    static std::unique_ptr<StdOutMutexHolder> mInstance;
    StdOutMutexHolder() = default;
    std::mutex mStdOutMutex;

public:
    std::mutex& getMutex() { return mStdOutMutex; }

    static std::unique_ptr<StdOutMutexHolder>& getInstance() {
        if( !mInstance.get() ) {
            mInstance.reset(new StdOutMutexHolder());
        }
        return mInstance;
    }
}; // class StdOutMutexHolder

std::unique_ptr<StdOutMutexHolder> StdOutMutexHolder::mInstance{nullptr};


class BlockPrintFormatter {
    std::string mEnclosing;
public:
    BlockPrintFormatter(const std::string& enclosing)
    : mEnclosing{ enclosing } {
        std::lock_guard<std::mutex> lock( StdOutMutexHolder::getInstance()->getMutex() );
        std::cout << mEnclosing << std::endl;
    }

    ~BlockPrintFormatter() {
        std::lock_guard<std::mutex> lock( StdOutMutexHolder::getInstance()->getMutex() );
        std::cout << mEnclosing << std::endl;
    }
}; // class BlockPrintFormatter
    
    
} // namespace utils

} // namespace StrGraph

#endif