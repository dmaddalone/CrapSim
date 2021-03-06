#ifndef CRAPSIMVERSION_H_INCLUDED
#define CRAPSIMVERSION_H_INCLUDED

#include <string>

namespace CrapSimVersion{

	//Date Version Types
	static const std::string DATE {"09"};
	static const std::string MONTH {"12"};
	static const std::string YEAR {"2014"};
	std::string DateVersion() {return (YEAR+MONTH+DATE); }

	//Software Status
	// Pre-alpha = The software is still under active development and not
	//   feature complete or ready for consumption by anyone other than
	//   software developers.
	// Alpha = The software is complete enough for internal testing.
	// Beta = The software is complete enough for external testing.
	// Release Candidate = The software is almost ready for final release.
	//   No feature development or enhancement of the software is undertaken;
	//   tightly scoped bug fixes are the only code you're allowed to write
	//   in this phase,
	// Release
	static const std::string STATUS {"Beta"};

	//Semantic Versioning
	// MAJOR version when you make incompatible changes,
	// MINOR version when you add functionality in a backwards-compatible manner, and
	// PATCH version when you make backwards-compatible bug fixes.
	static const int MAJOR = 0;
	static const int MINOR = 7;
	static const int PATCH = 0;
	std::string SemanticVersion() { return (std::to_string(MAJOR) + "." + std::to_string(MINOR) + "." + std::to_string(PATCH)); }
}

#endif // CRAPSIMVERSION_H_INCLUDED
