#pragma once

#include <iostream>
#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/log/common.hpp>
#include <boost/log/trivial.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword_fwd.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/expressions/record.hpp>

#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>

#include <boost/log/sinks.hpp>

#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/support/date_time.hpp>

namespace BoostLogger
{
	namespace logging = boost::log;
	namespace trivial = boost::log::trivial;
	namespace src = boost::log::sources;
	namespace expr = boost::log::expressions;
	namespace sinks = boost::log::sinks;
	namespace attrs = boost::log::attributes;
	namespace keywords = boost::log::keywords;

	using boost::shared_ptr;

	// added by add_common_attributes
	BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int);
	BOOST_LOG_ATTRIBUTE_KEYWORD(time_stamp, "TimeStamp", boost::posix_time::ptime);

	// added by severity_logger
	BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", trivial::severity_level);

	// should be added manually.
	BOOST_LOG_ATTRIBUTE_KEYWORD(scope_attr, "Scope", attrs::named_scope);
	BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string);

	// define a time-formatter to output time.
	// the part "expr::stream << expr::... " is equavelant to 
	// the similar part in "sink->set_formatter( expr::stream << expr::... ")".
	auto DataTimeFormatter();

	// define a tag-formatter.
	// the way to define tag-formatter is same as previous one,
	// because we will use expr::if_.
	auto TagFormatter();

	// define a scope-formatter, which is same as previous one.
	auto ScopeFormatter();

	void SeverityColorFormatter(logging::record_view const& rec, logging::formatting_ostream& strm);
	
	void SeverityFormatter(logging::record_view const& rec, logging::formatting_ostream& strm);

	void InitSeverityLogger(
		bool silence = false,
		std::string log_file_name = "default.log",
		trivial::severity_level least_level = trivial::trace
	);
}
