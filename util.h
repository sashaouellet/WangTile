/**
 * Houses utility functions for the manipulation of file names and Tiles
 *
 * @author Sasha Ouellet - spaouellet@me.com
 * @version 1.0 - 02/05/17
 */

#ifndef WANGTILE_UTIL_H
#define WANGTILE_UTIL_H

#include <vector>
#include <string>

using namespace std;

namespace util
{
    vector<char> parseFileNameForSideCodes(string, char);
};

#endif //WANGTILE_UTIL_H
