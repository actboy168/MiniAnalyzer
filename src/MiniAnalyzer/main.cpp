
#include <ReplayAnalyzer/RepAnalyzer.hpp>
#include <ReplayAnalyzer/File.hpp>
#include <memory>
#include <iostream>

#pragma comment(lib, "ReplayAnalyzer.lib")
#pragma comment(lib, "Zlib.lib")

class Redirect
{
public:
	Redirect(std::string const& filename)
		: log_(filename)
	{
		if (log_)
		{
			old_buf_ = std::cout.rdbuf(log_.rdbuf());
		}
		else
		{
			old_buf_ = nullptr;
		}
	}

	~Redirect()
	{
		if (old_buf_) 
			std::cout.rdbuf(old_buf_);
	}

private:
	std::ofstream   log_;
	std::streambuf* old_buf_;
};

void analyzer(const char* in_path, const char* out_path) 
{
	Redirect r(out_path);

	using namespace WRep;
	try
	{
		RepAnalyzer::Analyze(ReadFile(in_path).Reads<std::string>());
	}
	catch (Exception const& e)
	{
		std::cout << e.what() <<std::endl;
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() <<std::endl;
	}
}


int main(int argc, char* argv[]) 
{
	if ((argc == 2 && (argv[1][0] == '-' && argv[1][1] == 'h' && argv[1][2] == '\0'))
		|| (argc >= 4))
	{
		std::cout << "USAGE:" <<std::endl;
		std::cout << "     minianalyzer [replay_path [output_file_path]]" <<std::endl;
	}

	if (argc == 1)
	{
		analyzer("LastReplay.w3g", "output.log");
	}
	else if (argc == 2)
	{
		analyzer(argv[1], "output.log");
	}
	else if (argc == 3)
	{
		analyzer(argv[1], argv[2]);
	}

	return 0;
}
