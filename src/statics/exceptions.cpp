/**
 * @file exceptions.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/exceptions.h>
#include <sstream>

namespace Statics {

StaticsException::StaticsException()
    : msg("")
{
}

StaticsException::StaticsException(std::string msg)
    : msg(msg)
{
}

void StaticsException::setMsg(std::string msg) {
    this->msg = msg;
}

const char *StaticsException::what() const noexcept {
    return msg.c_str();
}

NoReturnException::NoReturnException() {
    setMsg("A non-void function was missing a return statement along a control "
           "flow path");
}

IllegalReturnTypeException::IllegalReturnTypeException() {
    setMsg("A function returned a value of the wrong type, or a void function "
           "returned a value");
}

IllegalTypeException::IllegalTypeException() {
    setMsg("An operation was performed on an illegal type");
}

UndeclaredIdentifierException::UndeclaredIdentifierException(std::string id) {
    std::stringstream ss;
    ss << "Identifier '" << id << "' was used before it was declared";
    setMsg(ss.str());
}

UndefinedIdentifierException::UndefinedIdentifierException(std::string id) {
    std::stringstream ss;
    ss << "Identifier '" << id << "' was used before it was defined";
    setMsg(ss.str());
}

RedeclaredIdentifierException::RedeclaredIdentifierException(std::string id) {
    std::stringstream ss;
    ss << "Identifier '" << id << "' was declared more than once";
    setMsg(ss.str());
}

UndeclaredFunctionException::UndeclaredFunctionException(std::string id) {
    std::stringstream ss;
    ss << "Function '" << id << "' was called before it was declared";
    setMsg(ss.str());
}

UndefinedFunctionException::UndefinedFunctionException(std::string id) {
    std::stringstream ss;
    ss << "Function '" << id << "' was called but was never defined";
    setMsg(ss.str());
}

RedefinedFunctionException::RedefinedFunctionException(std::string id) {
    std::stringstream ss;
    ss << "Function '" << id << "' was defined more than once";
    setMsg(ss.str());
}

UndeclaredTypeException::UndeclaredTypeException(std::string id) {
    std::stringstream ss;
    ss << "Type '" << id << "' was used before it was declared";
    setMsg(ss.str());
}

RedeclaredTypeException::RedeclaredTypeException(std::string id) {
    std::stringstream ss;
    ss << "Identifier '" << id << "' was declared more than once";
    setMsg(ss.str());
}

IncorrectSignatureException::IncorrectSignatureException(std::string id) {
    std::stringstream ss;
    ss << "Signature of function '" << id << "' did not match earlier " <<
          "declaration";
    setMsg(ss.str());
}

IncorrectLinkageException::IncorrectLinkageException(std::string id) {
    std::stringstream ss;
    ss << "Linkage of function '" << id << "' did not match earlier " <<
          "declaration";
    setMsg(ss.str());
}

ExternalFunctionDefinedException::ExternalFunctionDefinedException(std::string id) {
    std::stringstream ss;
    ss << "Function '" << id << "' was marked external then defined";
    setMsg(ss.str());
}

IllegalLValueException::IllegalLValueException() {
    setMsg("Attempted to assign to an illegal lvalue");
}

InvalidEntrypointException::InvalidEntrypointException() {
    setMsg("Required entry point is missing or has an incorrect signature");
}

}
