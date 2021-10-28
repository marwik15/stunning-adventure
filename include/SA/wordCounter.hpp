#pragma once
#include <vector>

class wordCounter{
SA_Private:
    /// <summary>
  /// store used words, grouped by alphabetical order
  /// </summary>
    std::vector<std::vector<std::pair<std::string, int>>> wordCount;
public:
    wordCounter(bool merge_uppercase = false) { //to do : implement merge
        
        //symbols between letter cases are also included
        int asciRange, asciRangeMin, asciRangeMax = 0;
        asciRangeMin = 65;

        if (merge_uppercase) {
            asciRangeMax = 87;
            asciRange = asciRangeMax - asciRangeMin;
        }
        else {
            asciRangeMin = 65;
            asciRangeMax = 122;
            asciRange = asciRangeMax - asciRangeMin;
        }

        // initialize vector
        wordCount.reserve(asciRange);

        //populate vector with alphabet 
        for (int c = asciRangeMin; c <= asciRangeMax; c++) {
            //not elegant ;d 
            std::vector<std::pair<std::string, int>> d;
            d.push_back(std::make_pair<std::string, int>(std::string(1, (char)c), 1));
            wordCount.push_back(d);

        }
    }

    /// <summary>
    /// save and/or count provided word/char 
    /// (small optimization) the method checks only those groups of words that start with the same letter 
    /// </summary>
    /// <param name="s">word or char</param>
    void count(std::string& s) {
        //map asci (char) number to vector index 
        int asciToindex = (int)s[0] - 65;

        for (int i = 0; i < wordCount[asciToindex].size(); i++) {
            //check if word is already in our "database"
            if (wordCount[asciToindex].at(i).first == s) {
                wordCount[asciToindex].at(i).second++;
                return;
            }
        }
        //if not found, add new word 
        wordCount[asciToindex].push_back(std::make_pair<std::string, int>(std::string(s), 1));
    }

};

