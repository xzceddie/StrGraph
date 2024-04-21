#ifndef OPERATORS_H
#define OPERATORS_H

#include <string>
#include <functional>
#include <vector>
#include <memory>

namespace StrGraph {

using OpFunc = std::function<std::string(const std::vector<std::string>&)>;

class ConcatOperator {
    std::string mDelim;
public:
    ConcatOperator( const std::string& delim )
    : mDelim{ delim }
    {
    }

    std::string operator()( const std::vector<std::string>& inputs ) {
        std::string res;
        if( inputs.size() == 0 )
            return "";
        if ( inputs.size() == 1 )
            return mDelim;
        for( int i = 0; i < inputs.size() - 1; ++i) {
            res.append( inputs[i] );
            res.append( mDelim );
        }
        res.append( inputs.back() );
        return res;
    }
};

class RepeatOperator {
    int mCount;
    std::string mDelim;
public:
    RepeatOperator( const int count, const std::string& delim )
    : mCount{ count }, mDelim{ delim }
    {
    }

    std::string operator()( const std::vector<std::string>& inputs ) {
        assert( inputs.size() == 1 );
        const std::string& orig_piece = inputs.front();
        std::string res;
        if( mCount == 0 )
            return "";
        if ( mCount == 1 )
            return orig_piece;

        for( int i = 0; i < mCount - 1; ++i) {
            res.append( orig_piece );
            res.append( mDelim );
        }
        res.append( orig_piece );
        return res;
    }
};

class ToUpperOperator {
public:
    std::string operator()( const std::vector<std::string>& inputs ) {
        assert( inputs.size() == 1 );
        std::string res = inputs.front();
        for(auto& c : res) {
            c = toupper(c);
        }
        return res;
    }
}; // class ToUpperOperator

class ToLowerOperator {
public:
    std::string operator()( const std::vector<std::string>& inputs ) {
        assert( inputs.size() == 1 );
        std::string res = inputs.front();
        for(auto& c : res) {
            c = tolower(c);
        }
        return res;
    }
}; // class ToLowerOperator


class InsertOperator {
    std::string mDelim;
    int mCount;
public:
    InsertOperator( const int count, const std::string& delim )
    : mDelim{ delim }
    , mCount{ count }
    {
        assert( count > 0 );
    }
    std::string operator()( const std::vector<std::string>& inputs ) {
        assert( inputs.size() == 1 );
        std::string res;
        
        const std::string& input_str = inputs.front();

        for( int i = 0; i < input_str.size(); ++i ) {
            // std::cout << "i: " << i << ", str: " << res << std::endl;
            res.push_back( input_str.at(i) );
            if( i % mCount == mCount - 1 && i != input_str.size() - 1 ) {
                res.append( mDelim );
            }
        }
        return res;
    }
}; // class InsertOperator

// positive -> rotate right; negative -> rotate left; 0 -> no change
class RotateOperator {
    int mCount;
public:
    RotateOperator( const int count )
    : mCount{ count }
    {
    }
    std::string operator()( const std::vector<std::string>& inputs ) {
        assert( inputs.size() == 1 );
        const std::string& input_str = inputs.front();
        if ( mCount == 0)
            return input_str;

        std::string res;
        if( mCount < 0)
            std::rotate_copy( input_str.begin(), input_str.begin() + abs(mCount), input_str.end(), std::back_inserter(res) );
        else
            std::rotate_copy( input_str.begin(), input_str.end() - mCount, input_str.end(), std::back_inserter(res) );
        return res;
    }
}; // class InsertOperator


} // namespace StrGraph

#endif
