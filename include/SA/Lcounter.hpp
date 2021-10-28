#pragma once
#include <string>
#include <sstream>

/// <summary>
/// line counter class
/// </summary>
class Lcounter {
public:
	Lcounter() {
		emptyLineCount = nonemptyLineCount = 0;
	}
	void countLines(std::stringstream& buffer) {


		std::vector<size_t> positions;

		//find all \n
		size_t pos = buffer.str().find("\n", 0);
		while (pos != std::string::npos)
		{
			positions.push_back(pos);
			pos = buffer.str().find("\n", pos + 1);
		}

		//count empty lines by checking if next found position is sequential number
		for (int i = 0; i < positions.size()-1; i++) {
			if (++positions[i] == positions[i+1]) {
				emptyLineCount++;
				
			}
		}

		nonemptyLineCount = positions.size() - emptyLineCount;

		// count last line if doesn't end with \n
		auto lastPosition = positions.at(positions.size() - 1);
		auto strSize = buffer.str().size();
		if (strSize-1 != lastPosition) {
			nonemptyLineCount++;
		}
		
	}

	int getnonemptyLineCount() {
		return nonemptyLineCount;
	}

	int getEmptyLineCount() {
		return emptyLineCount;
	}

SA_Private:
	int	emptyLineCount,
		nonemptyLineCount;

};