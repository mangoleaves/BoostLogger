#include "BoostLogger.h"

namespace BoostLogger
{
	auto DataTimeFormatter()
	{
		return expr::stream << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S") << "]";
	}

	auto TagFormatter()
	{
		return	expr::stream << expr::if_(expr::has_attr("Tag"))[
			expr::stream << "[" << expr::attr<std::string>("Tag") << "]"];
	}

	auto ScopeFormatter()
	{
		return expr::stream << "{" << expr::format_named_scope(
			"Scope",
			keywords::format = "%C (%F:%l)"
		) << "}";
	}

	void SeverityColorFormatter(
		logging::record_view const& rec, logging::formatting_ostream& strm)	// parameters are fixed.
	{
		// select color depend on severity.
		logging::value_ref<trivial::severity_level, tag::severity> level = rec[severity];
		switch (level.get())
		{
		case trivial::trace:
			strm << "\033[37m";
			break;
		case trivial::debug:
			strm << "\033[36m";
			break;
		case trivial::info:
			strm << "\033[32m";
			break;
		case trivial::warning:
			strm << "\033[33m";
			break;
		case trivial::error:
		case trivial::fatal:
			strm << "\033[31m";
			break;
		default:
			break;
		}

		// output LineID. rec[...] is equavelant to
		//   logging::extract<ValueType>("AttributeName", rec).
		strm << rec[line_id].get() << ":";

		auto data_time_formatter = DataTimeFormatter();
		data_time_formatter(rec, strm);

		// output severity level.
		strm << "<" << level.get() << "> ";

		auto tag_formatter = TagFormatter();
		tag_formatter(rec, strm);

		auto scope_formatter = ScopeFormatter();
		scope_formatter(rec, strm);

		// output message.
		strm << rec[expr::smessage];

		// restore the default color.
		strm << "\033[0m";
	}

	void SeverityFormatter(
		logging::record_view const& rec, logging::formatting_ostream& strm)
	{
		logging::value_ref<trivial::severity_level, tag::severity> level = rec[severity];
		strm << rec[line_id].get() << ": ";

		auto data_time_formatter = DataTimeFormatter();
		data_time_formatter(rec, strm);

		strm << "<" << level.get() << "> ";

		auto tag_formatter = TagFormatter();
		tag_formatter(rec, strm);

		auto scope_formatter = ScopeFormatter();
		scope_formatter(rec, strm);

		strm << rec[expr::smessage];
	}

	void InitSeverityLogger(
		bool silence,
		std::string log_file_name,
		trivial::severity_level least_level
	)
	{
		// Initialize console sink. It can output color text.
		typedef sinks::synchronous_sink< sinks::text_ostream_backend> TextConsoleSink;
		shared_ptr<TextConsoleSink> sp_console_sink(new TextConsoleSink);

		TextConsoleSink::locked_backend_ptr sp_console_backend = sp_console_sink->locked_backend();

		shared_ptr<std::ostream> sp_stream(std::_Ptr_clog, boost::null_deleter());
		sp_console_backend->add_stream(sp_stream);

		//shared_ptr<std::ofstream> sp_fstream(new std::ofstream(log_file_name));
		//if (sp_fstream->is_open())
		//{
		//	sp_backend->add_stream(sp_fstream);
		//}

		// Initialize formatter.
		sp_console_sink->set_formatter(&SeverityColorFormatter);
		// Initialize filter. We can also use boost::phoenix to write a function like filter.
		sp_console_sink->set_filter(severity >= least_level);

		// Initialize file sink.
		shared_ptr<sinks::text_file_backend> sp_file_backend =
			boost::make_shared<sinks::text_file_backend>(
				keywords::file_name = log_file_name,
				keywords::target_file_name = log_file_name
				);
		typedef sinks::synchronous_sink<sinks::text_file_backend> TextFileSink;
		shared_ptr<TextFileSink> sp_file_sink(new TextFileSink(sp_file_backend));

		sp_file_sink->set_formatter(&SeverityFormatter);
		sp_file_sink->set_filter(severity >= least_level);

		// Add sinks and attributes.
		logging::core::get()->add_sink(sp_console_sink);
		logging::core::get()->add_sink(sp_file_sink);

		logging::add_common_attributes();
		logging::core::get()->add_global_attribute("Scope", attrs::named_scope());
	}
}
