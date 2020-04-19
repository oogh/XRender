//
// Created by Oogh on 2020/3/19.
//

#ifndef ANDROIDDEMO_XEXCEPTION_H
#define ANDROIDDEMO_XEXCEPTION_H

#include <stdexcept>

class XException: public std::exception {
public:
    explicit XException(const char* what) {
        mWhat = what;
    }

    const char* what() const throw() {
        return mWhat;
    }

private:
    const char* mWhat;
};
#endif //ANDROIDDEMO_XEXCEPTION_H
