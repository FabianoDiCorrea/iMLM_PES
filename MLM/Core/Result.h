#ifndef MLM_RESULT_H
#define MLM_RESULT_H

#include <string>

template <typename T>
struct Result {
    bool success = false;
    T value;
    std::string errorMessage;

    static Result<T> Success(const T& val) {
        return { true, val, "" };
    }

    static Result<T> Failure(const std::string& error) {
        return { false, T(), error };
    }
};

struct OperationResult {
    bool success = false;
    std::string message;

    static OperationResult Success(const std::string& msg = "") {
        return { true, msg };
    }

    static OperationResult Failure(const std::string& error) {
        return { false, error };
    }
};

#endif // MLM_RESULT_H
