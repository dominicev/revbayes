/**
 * @file
 * This file contains the implementation of StringUtilities, which 
 * helper functions for manipulating strings.
 *
 * @brief Declaration of StringUtilities
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include <iostream>
#include <fstream>
#include "StringUtilities.h"


std::string StringUtilities::getFileContentsAsString(std::string& s) {

    // open file
	std::ifstream fStrm;
    fStrm.open(s.c_str(), std::ios::in);
    if ( !fStrm.is_open() )
        return "";
        
    // read the file
    int ch;
    std::string retStr = "";
    while ( (ch = fStrm.get()) != EOF)
        {
        char c = (char)ch;
        
        if (ch == '\n' || ch == '\r' || ch == EOF)
            retStr += '\n';
        else
            retStr += c;
        } 

    // close file
    fStrm.close();

    return retStr;
}


/** Find the last component of a file path */
std::string StringUtilities::getLastPathComponent(const std::string& s) {

    std::vector<std::string> sVec;
    stringSplit(s, "/", sVec);
    std::string lastComponent = "";
    if (sVec.size() > 0)
        lastComponent = sVec[sVec.size()-1];
    return lastComponent;
}


/** Utility function for dividing string into pieces */
void StringUtilities::stringSplit(std::string str, std::string delim, std::vector<std::string>& results) {

    size_t cutAt;
    while ( (cutAt = str.find_first_of(delim)) != str.npos )
        {
        if (cutAt > 0)
            results.push_back(str.substr(0, cutAt));
        str = str.substr(cutAt+1);
        }
    if (str.length() > 0)
        results.push_back(str);
}


/** Utility function for converting string to all lower case */
void StringUtilities::toLower(std::string& str) {

    for (size_t i=0; i<str.size(); i++)
        str[i] = tolower(str[i]);
}
