#pragma once

#include <stdexcept>
#include <exception>
#include <boost/exception/all.hpp>

#define TFW_THROW(x) BOOST_THROW_EXCEPTION(x)

namespace tfw {

class TFWException: public virtual boost::exception, public virtual std::exception {};

} // namespace tfw

