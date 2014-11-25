#include "tfw/data/TransferFunctionLoading.hpp"
#include "tfw/data/ATransferFunction.hpp"

/*#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/tuple/tuple.hpp>*/

#include <fstream>
#include <strstream>

namespace tfw {

std::unique_ptr<ATransferFunction>
loadFromFile(boost::filesystem::path aPath)
{
	//TODO proper loading
	auto tf = std::unique_ptr<TransferFunction1D>(new TransferFunction1D(0.0, 2000.0));
	tf->setName(aPath.stem().string());
	std::ifstream in(aPath.string());
	std::string line;
	while (std::getline(in, line)) {
		std::istringstream line_stream(line);
		int mapped = 0;
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		float alpha = 0.0f;
		line_stream >> mapped >> red >> green >> blue >> alpha;

		tf->setChannelValue(mapped, red, 0);
		tf->setChannelValue(mapped, green, 1);
		tf->setChannelValue(mapped, blue, 2);
		tf->setChannelValue(mapped, alpha, 3);

		/*using boost::spirit::qi::double_;
		using boost::spirit::qi::int_;
		using boost::spirit::qi::phrase_parse;
		using boost::spirit::ascii::space;
		boost::tuple<int, double, double, double> rec;
		phrase_parse(
			line.begin(),
			line.end(),
			(
			"[" >> int_ >> "]" >>
			double_ >> double_ >> double_
			),
			space,
			rec
			);*/
	}
	return std::move(tf);
}

void
fillTFPalette(tfw::TransferFunctionPalette &palette, boost::filesystem::path dirName)
{
	if (!boost::filesystem::exists(dirName)) {
		//LOG( "Directory \'" << dirName << "\' doesn't exist!" );
		return;
	}
	if (!boost::filesystem::is_directory(dirName) ){
		//LOG( "\'" << dirName << "\' is not a directory!" );
		return;
	}

	boost::filesystem::directory_iterator dirIt(dirName);
	boost::filesystem::directory_iterator end;
	for ( ;dirIt != end; ++dirIt ) {
		boost::filesystem::path p = dirIt->path();
		if (p.extension() == ".txt") {
			//LOG("Found TF file :" << p.string());
			auto tf = tfw::loadFromFile(p);
			if (tf) {
				palette.add(std::move(tf));
			}
		}
	}
}


} // namespace tfw
