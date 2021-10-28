#pragma once
#include <string>
#include <sstream>
#include <vector>

/// <summary>
/// line counter class
/// </summary>
class lineCounter {
public:
	lineCounter() {
		emptyLineCount = nonemptyLineCount = totalLineCount = 0;
	}
	void countLines(std::string& buffer) {

		std::vector<size_t> positions;

		//find all \n
		size_t pos = buffer.find("\n", 0);
		while (pos != std::string::npos)
		{
			positions.push_back(pos);
			pos = buffer.find("\n", pos + 1);
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
		auto strSize = buffer.size();
		if (strSize-1 != lastPosition) {
			nonemptyLineCount++;
		}

		totalLineCount = nonemptyLineCount + emptyLineCount;
	}

	int getnonemptyLineCount() {
		return nonemptyLineCount;
	}

	int getEmptyLineCount() {
		return emptyLineCount;
	}

	int getTotalLineCount() {
		return totalLineCount;
	}

SA_Private:
	int	emptyLineCount,
		nonemptyLineCount,
		totalLineCount;

};