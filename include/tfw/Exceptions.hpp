#pragma once

#include <stdexcept>
#include <exception>
#include <boost/exception/all.hpp>

#define TFW_THROW(x) BOOST_THROW_EXCEPTION(x)

#define TFW_ASSERT(x) assert(x)

namespace tfw {

class TFWException: public virtual boost::exception, public virtual std::exception {};

class EUnsupportedTransferFunctionType : public TFWException {};

} // namespace tfw

