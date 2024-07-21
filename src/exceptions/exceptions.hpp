#pragma once

#include <exception>
#include <stdexcept>

class BaseExeption : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class InternalError : public BaseExeption {
    using BaseExeption::BaseExeption;
};

class InvalidArgument : public BaseExeption {
    using BaseExeption::BaseExeption;
};

class ArleadyExists : public BaseExeption {
    using BaseExeption::BaseExeption;
};
