/**
 * Houses utility functions for the manipulation of filenames and Tiles
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/05/17
 */

#include "util.h"
#include <iostream>

namespace util
{
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
    }
}
