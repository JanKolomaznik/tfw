#pragma once

#include <memory>

#include <boost/filesystem.hpp>

#include "tfw/data/ATransferFunction.hpp"


namespace tfw {

std::unique_ptr<ATransferFunction>
loadFromFile(boost::filesystem::path aPath);


} // namesapce tfw
