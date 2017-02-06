/**
 * Houses utility functions for the manipulation of filenames and Tiles
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/05/17
 */

#include <stdexcept>
#include "util.h"

namespace util
{
    /**
     * Given the the text of the file name, and the delimiter to search as the separation, returns the vector of side codes
     * this bitmap file represents
     *
     * @param text The text of the file name to extract the side codes from
     * @param delimiter The character delimiter (usually an underscore) that separates the file name from the codes
     * @return The size 4 vector of the codes (i.e. {G, B, G, C})
     * @throws invalid_argument If the delimiter provided is not found in the file name
     */
    vector<char> parseFileNameForSideCodes(string text, char delimiter)
    {
        vector<char> codes;
        int start = text.find(delimiter, 0);

        if (start != string::npos)
        {
            string codeSuffix = text.substr(start + 1, text.find(".bmp", 0) - 5);

            codes.push_back(codeSuffix[0]);
            codes.push_back(codeSuffix[1]);
            codes.push_back(codeSuffix[2]);
            codes.push_back(codeSuffix[3]);
        }
        else
        {
            throw invalid_argument("Name of file does not follow expected format with given delimiter to separate side codes");
        }

        return codes;
    }
}
