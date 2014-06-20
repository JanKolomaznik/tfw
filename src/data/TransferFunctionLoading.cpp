#include "tfw/data/TransferFunctionLoading.hpp"

namespace tfw {

std::unique_ptr<ATransferFunction>
loadFromFile(boost::filesystem::path aPath)
{
	return std::unique_ptr<ATransferFunction>();
}

} // namespace tfw
