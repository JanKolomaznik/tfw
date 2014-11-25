#pragma once

#include <memory>

#include <boost/filesystem.hpp>

#include "tfw/data/ATransferFunction.hpp"
#include "tfw/data/TransferFunctionPalette.hpp"

namespace tfw {

std::unique_ptr<ATransferFunction>
loadFromFile(boost::filesystem::path aPath);

void
fillTFPalette(tfw::TransferFunctionPalette &palette, boost::filesystem::path dirName);

} // namespace tfw
