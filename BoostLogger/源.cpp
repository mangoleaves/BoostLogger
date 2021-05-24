#include "BoostLogger.h"

using namespace BoostLogger;

int main(int argc, char* argv[])
{
	InitSeverityLogger();
	// below are some tests.
	src::severity_logger<trivial::severity_level> lg;
	BOOST_LOG_FUNCTION();
	{
		lg.add_attribute("Tag", attrs::constant<std::string>("main"));
		BOOST_LOG_NAMED_SCOPE("inner")
		BOOST_LOG_SEV(lg, trivial::info) << "hello world.";
		BOOST_LOG_SEV(lg, trivial::debug) << "debug.";
		BOOST_LOG_SEV(lg, trivial::fatal) << "fatal.";
	}
	BOOST_LOG_SEV(lg, trivial::trace) << "trace.";
}