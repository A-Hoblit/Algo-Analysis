///////////////////////////////////////////////////////////////////////////////
// algorithms.hpp
//
// Definitions for four algorithms:
//
// run_length_encode
// longest_frequent_substring
// reformat_date
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <cstring>
#include <sstream>

namespace algorithms {

  // Run-length-encode the given string.
  //
  // uncompressed must be a string containing only lower-case letters or spaces.
  //
  // A run is defined as a sequence of K>2 contiguous copies of the same
  // character c.
  // For example, "aaa" is a run with K=3 and c='a'.
  // This function returns a string based on uncompressed, where every run is
  // replaced with the string
  //   COUNTc
  // where COUNT is the base-10 representation of K. Non-run characters are
  // left as-is.
  //
  // Example inputs and outputs:
  //   "aaa" -> "3a"
  //   "heloooooooo there" -> "hel8o there"
  //   "footloose and fancy free" -> "f2otl2ose and fancy fr2e"
  //
  // Throws std::invalid_argument if the string contains invalid characters.
  std::string run_length_encode(const std::string& uncompressed) {
  
    int length = uncompressed.length();
    std::string encode = "";

    // Check invalid inputs
    for (int i=0; i < length; i++)
    {
      if (!(islower(uncompressed[i])) && !isspace(uncompressed[i]))
      {
        throw std::invalid_argument("Not lowercase alphabetical");
      }
    }
    if(length == 0)
    {
      return encode;
    }

    // Reacreation of pseudocode
    char run_char   = uncompressed[0];
    int  run_length = 1;

    for (int i=1; i < length; i++)
    {
      if (uncompressed[i] == run_char)
      {
        run_length++;
      }
      else
      {
        if (run_length > 1)
        {
          encode += (std::to_string(run_length));
        }
        encode += run_char;
        run_char = uncompressed[i];
        run_length = 1;
      }  
    }
    if (run_length > 1)
    {
      encode += (std::to_string(run_length));
    }
    encode += run_char;

    return encode;
  }

  // Returns the longest substring of text, such that every character in the
  // substring appears at least k times in text.
  // If there are ties, the substring that appears first is returned.
  std::string longest_frequent_substring(const std::string& text, unsigned k) {

    int length = text.length();
    std::map<char, int> freq;

    // Put all characters in the map
    for (int i=0; i < length; i++)
    {
      if ( freq.find(text[i]) == freq.end() ) 
      {
        freq.insert( {text[i], 1} );
      }
      else
      {
        freq[text[i]]++;
      }
    }

    // Generate substrings, and then check substrings for vailidity
    std::string best = "";
    bool potential = true;
    for (int a=0; a < length; a++)
    {
      for (int b=a+1; b <= length; b++)
      {
        std::string cand = text.substr(a, b-a);

        for (int c=0; c < cand.length(); c++)
        {
          if (freq[cand[c]] < k)
          {
            potential = false;
          }
        }
        if (potential)
        {
          if (cand.length() > best.length())
          {
            best = cand;
          }
        }
        potential = true;
      }
    }
  return best;
  }

  // Reformats a string containing a date into YYYY-MM-DD format.
  //
  // input may be formatted in one of four patterns:
  // 1. Y-M-D
  //    where Y, M, and D are positive integers
  // 2. M/D/Y
  //    where Y, M, and D are positive integers
  // 3. MONTH DAY, YEAR
  //    where
  //    - MONTH is a case-insensitive name of a month, ex.
  //      "january", "FEBRUARY", "March", etc.
  //    - DAY is a positive integer representing a day, e.x. "15"
  //    - YEAR is a positive integer representing a year, e.x. "2022"
  // 4. MON DAY, YEAR
  //    where
  //    - MON is a case-insensitive three-letter abbreviation of a month, ex.
  //      "jan", "FEB", "Mar", etc.
  //    - DAY is the same as above
  //    - YEAR is the same as above
  //
  // Any leading spaces or trailing spaces are ignored.
  //
  // Returns a string in strict YYYY-MM-DD format.
  //
  // Throws std::invalid argument if:
  // - input does not fit any of the four patterns
  // - MONTH is not a valid month name
  // - MON is not a valid month abbreviation
  // - DAY is not in the range [1, 31]
  // - YEAR is not in the range [1900, 2099]
  std::string reformat_date(const std::string& input) {

  //std::string input = "2000-12-10";
  //std::string input = "12/10/2000";
  //std::string input = "December 10, 2000";
  //std::string input = "Dec 10, 2000";

  std::string year = "";
  std::string month = "";
  std::string day = "";
  std::string date = "";

  std::vector<std::string> mon_List = {"jan","feb","mar","apr","may","jun",
                                       "jul","aug","sep","oct","nov","dec"};
  std::vector<std::string> month_List = {"january","february","march",
                                          "april","may","june","july",
                                          "august", "september", "october",
                                          "november","december"};
  std::vector<long int> year_Range = {1900, 2099};
  std::vector<long int> month_Range = {1, 12};
  std::vector<long int> day_Range = {1, 31};  

  // Tokenize the input string
  std::string word = "";
  std::stringstream stream(input);
  std::vector<std::string> tokens;
  while(stream>>word) 
  {
    tokens.push_back(word);
  }

  int token_case = tokens.size();

  switch(token_case)
  {
  // 1 token means Y-M-D or M/D/Y format
    case 1: 
    {
      long int dates[3] = {0,0,0};
      bool ymd = false;

      // Remove deliminators - and /
      std::istringstream dateStream(tokens[0]);
      std::vector<std::string> dateTokens;
      std::string tokenBuf = "";
      if (tokens[0].find('-') != std::string::npos)
      {
        while(std::getline(dateStream, tokenBuf, '-'))
        {
          dateTokens.push_back(tokenBuf);
        }
        ymd = true;
      }
      else if (tokens[0].find('/') != std::string::npos)
      {
        while(std::getline(dateStream, tokenBuf, '/'))
        {
          dateTokens.push_back(tokenBuf);
        }
        ymd = false;
      }

      // Ensure there is the right number of tokens
      if (dateTokens.size() != 3)
      {
        throw std::invalid_argument("Numerical date wrong length.");
      }

      // Extract numbers
      for (int i=0; i < 3; i++)
      {
        char * endptr;
        dates[i] = strtol(dateTokens[i].c_str(), &endptr, 10);
        if (*endptr != '\0' && *endptr != ' ')
        {
          throw std::invalid_argument("Bad numerical date data.");
        }
      }

      // Check dates are in range for each format
      if (ymd) // Y-M-D format
      {
        if (dates[0] >= year_Range[0]  && dates[0] <= year_Range[1]  &&
            dates[1] >= month_Range[0] && dates[1] <= month_Range[1] &&
            dates[2] >= day_Range[0]   && dates[2] <= day_Range[1])
        {
          year  = std::to_string(dates[0]);
          month = std::to_string(dates[1]);
          day   = std::to_string(dates[2]);
        }
        else
        {
          throw std::invalid_argument("Year-Month-Day out of range.");
        }
      }
      else // M/D/Y format
      {
        if (dates[0] >= month_Range[0] && dates[0] <= month_Range[1] &&
            dates[1] >= day_Range[0]   && dates[1] <= day_Range[1]   &&
            dates[2] >= year_Range[0]  && dates[2] <= year_Range[1])
        {
          month = std::to_string(dates[0]);
          day   = std::to_string(dates[1]);
          year  = std::to_string(dates[2]);
        }
        else
        {
          throw std::invalid_argument("Month/Day/Year out of range.");
        }
      }
      break;
    }
  // 3 tokens means MONTH D, Y or MON D, Y format
    case 3: 
    {
      // Make the month token lowecase
      std::string tokenBuf = "";
      for (int i=0; i < tokens[0].length(); i++)
      {
        tokenBuf += tolower(tokens[0][i]);
      }

      // Find which month the token is numerically.
      if (tokenBuf.length() == 3)
      {
        for (int i=0; i < 12; i++)
        {
          if (tokenBuf == mon_List[i])
          {
            month = std::to_string(i+1);
            break;
          }
        }
      }
      else
      {
        for (int i=0; i < 12; i++)
        {
          if (tokenBuf == month_List[i])
          {
            month = std::to_string(i+1);
            break;
          }
        }
      }

      if (month == "")
      {
        throw std::invalid_argument("Bad text month");
      }

      // Remove the comma from the day token
      tokens[1].pop_back();

      // Tokenize the numerical data
      long int dates[2] = {0,0};
      for (int i=0; i < 2; i++)
      {
        char * endptr;
        dates[i] = strtol(tokens[i+1].c_str(), &endptr, 10);
        if (*endptr != '\0' && *endptr != ' ')
        {
          throw std::invalid_argument("Bad numerical date data.");
        }
      }

      if (dates[0] >= day_Range[0] && dates[0] <= day_Range[1] &&
          dates[1] >= year_Range[0] && dates[1] <= year_Range[1])
      {
        day = std::to_string(dates[0]);
        year = std::to_string(dates[1]);
      }
      else
      {
        throw std::invalid_argument("Day Year dates out of range");
      }

      break;
    }
    default:
    {
      throw std::invalid_argument("Input does not match date cases");
      break;
    }
  }

  if (month.length() == 1)
  {
    month.insert(0, 1, '0');
  }
  if (day.length() == 1)
  {
    day.insert(0, 1, '0');
  }

  date = year + '-' + month + '-' + day;

  return date;

  }
}


