//============================================================================
// Name        : CSVparser.cpp
// Author      : SNHU and Josh Gauthier
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-2 Up to Speed in C++, Part 2
//============================================================================

#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "CSVparser.hpp"

namespace csv {

  // Constructor for Parser class. Reads CSV file and stores bid data into a vector of strings.
  // Then parses that data into header vector and content vector
  Parser::Parser(const std::string &data, const DataType &type, char sep)
    : _type(type), _sep(sep)
  {
      std::string line;
      if (type == eFILE)
      {
        _file = data;
        std::ifstream ifile(_file.c_str());
        if (ifile.is_open())
        {
            while (ifile.good())
            {
                getline(ifile, line);
                if (line != "")
                    _originalFile.push_back(line);
            }
            ifile.close();

            if (_originalFile.size() == 0)
              throw Error(std::string("No Data in ").append(_file));
            
            parseHeader();
            parseContent();
        }
        else
            throw Error(std::string("Failed to open ").append(_file));
      }
      else
      {
        std::istringstream stream(data);
        while (std::getline(stream, line))
          if (line != "")
            _originalFile.push_back(line);
        if (_originalFile.size() == 0)
          throw Error(std::string("No Data in pure content"));

        parseHeader();
        parseContent();
      }
  }

  // Destructor for Parser class
  Parser::~Parser(void)
  {
     std::vector<Row *>::iterator it;

     for (it = _content.begin(); it != _content.end(); it++)
          delete *it;
  }

  // Reads first row of csv file and adds it to header vector
  void Parser::parseHeader(void)
  {
      std::stringstream ss(_originalFile[0]);
      std::string item;

      while (std::getline(ss, item, _sep))
          _header.push_back(item);
  }

  // Reads all rows in csv file that come after header
  void Parser::parseContent(void)
  {
     std::vector<std::string>::iterator it;
     
     it = _originalFile.begin();
     it++; // skip header

     // loop thru csv file until reaching the end
     for (; it != _originalFile.end(); it++)
     {
         bool quoted = false;
         int tokenStart = 0;
         unsigned int i = 0;

         // Create heap object from Row class and add header to it
         Row *row = new Row(_header);

         for (; i != it->length(); i++)
         {
              if (it->at(i) == '"')
                  quoted = ((quoted) ? (false) : (true));
              else if (it->at(i) == ',' && !quoted)
              {
                  row->push(it->substr(tokenStart, i - tokenStart));
                  tokenStart = i + 1;
              }
         }

         //end
         row->push(it->substr(tokenStart, it->length() - tokenStart));

         // if value(s) missing
         if (row->size() != _header.size())
          throw Error("corrupted data !");
         _content.push_back(row);
     }
  }

  // Returns row based on row position in the Parser vector
  Row &Parser::getRow(unsigned int rowPosition) const
  {
      if (rowPosition < _content.size())
          return *(_content[rowPosition]);
      throw Error("can't return this row (doesn't exist)");
  }

  // Returns row based on row position, but uses bracket notation just like accessing array items
  Row &Parser::operator[](unsigned int rowPosition) const
  {
      return Parser::getRow(rowPosition);
  }

  // Returns amount of rows in Row vector in Parser object. In other words it returns number of bids
  unsigned int Parser::rowCount(void) const
  {
      return _content.size();
  }

  // Returns the amount of columns in the header of the csv file
  unsigned int Parser::columnCount(void) const
  {
      return _header.size();
  }

  // Access function that returns the header of CSV file that's stored in Parser obect's vector
  std::vector<std::string> Parser::getHeader(void) const
  {
      return _header;
  }

  // Returns header item in csv file based on position argument
  const std::string Parser::getHeaderElement(unsigned int pos) const
  {
      if (pos >= _header.size())
        throw Error("can't return this header (doesn't exist)");
      return _header[pos];
  }

  // Delete row in vector or Row objects, based on position argument
  bool Parser::deleteRow(unsigned int pos)
  {
    if (pos < _content.size())
    {
      delete *(_content.begin() + pos);
      _content.erase(_content.begin() + pos);
      return true;
    }
    return false;
  }

  // Add a row (vector of strings) to _content vector at certain position. Return false if failed
  bool Parser::addRow(unsigned int pos, const std::vector<std::string> &r)
  {
    Row *row = new Row(_header);

    for (auto it = r.begin(); it != r.end(); it++)
      row->push(*it);
    
    if (pos <= _content.size())
    {
      _content.insert(_content.begin() + pos, row);
      return true;
    }
    return false;
  }

  // Output to file the header and content rows of the parser object to _file
  void Parser::sync(void) const
  {
    if (_type == DataType::eFILE)
    {
      std::ofstream f;
      f.open(_file, std::ios::out | std::ios::trunc);

      // header
      unsigned int i = 0;
      for (auto it = _header.begin(); it != _header.end(); it++)
      {
        f << *it;
        if (i < _header.size() - 1)
          f << ",";
        else
          f << std::endl;
        i++;
      }
     
      for (auto it = _content.begin(); it != _content.end(); it++)
        f << **it << std::endl;
      f.close();
    }
  }

  // Return name of file
  const std::string &Parser::getFileName(void) const
  {
      return _file;    
  }
  
  /*
  ** ROW
  */

  // Constructor for Row class, adds header to _header member object which holds the CSV column headers
  Row::Row(const std::vector<std::string> &header)
      : _header(header) {}

  // Deconstructor for Row
  Row::~Row(void) {}

  // Return the number of items in row class
  unsigned int Row::size(void) const
  {
    return _values.size();
  }

  // Adds string to end of vector of strings
  void Row::push(const std::string &value)
  {
    _values.push_back(value);
  }

  // Sets a value in the row to new value. Sets it to argument string
  bool Row::set(const std::string &key, const std::string &value) 
  {
    std::vector<std::string>::const_iterator it;
    int pos = 0;

    for (it = _header.begin(); it != _header.end(); it++)
    {
        if (key == *it)
        {
          _values[pos] = value;
          return true;
        }
        pos++;
    }
    return false;
  }

  // Overload [] operator on Row class. Acts like accessing an array
  // Find row value based on integer position as argument.
  // Returns the string at the position
  const std::string Row::operator[](unsigned int valuePosition) const
  {
       if (valuePosition < _values.size())
           return _values[valuePosition];
       std::cout << "This is ROW CLASS operater overload [], int parameter" << std::endl;
       throw Error("can't return this value (doesn't exist)");
  }


  // Overload [] operator when using on Row class. Acts like an array
  // Find row value using header name as argument.
  // Returns the value under that specific column header
  const std::string Row::operator[](const std::string &key) const
  {
      std::vector<std::string>::const_iterator it;
      int pos = 0;

      for (it = _header.begin(); it != _header.end(); it++)
      {
          if (key == *it)
              return _values[pos];
          pos++;
      }
      std::cout << "This is ROW CLASS operater overload [], string parameter as key" << std::endl;
      
      throw Error("can't return this value (doesn't exist)");
  }

  // Overload << operator when using on an out stream object and Row object
  // Streams out entire row in argument into the out stream object
  std::ostream &operator<<(std::ostream &os, const Row &row)
  {
      for (unsigned int i = 0; i != row._values.size(); i++)
          os << row._values[i] << " | ";

      return os;
  }

  // Overload << operator when using on an output file stream object and Row object
  // Streams out entire row in argument into the output file stream object
  std::ofstream &operator<<(std::ofstream &os, const Row &row)
  {
    for (unsigned int i = 0; i != row._values.size(); i++)
    {
        os << row._values[i];
        if (i < row._values.size() - 1)
          os << ",";
    }
    return os;
  }
}
