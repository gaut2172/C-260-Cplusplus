//============================================================================
// Name        : CSVparser.hpp
// Author      : SNHU and Josh Gauthier
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 2-2 Up to Speed in C++, Part 2
//============================================================================

#ifndef     _CSVPARSER_HPP_
# define    _CSVPARSER_HPP_

# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <sstream>

namespace csv
{
    class Error : public std::runtime_error
    {

      public:
        Error(const std::string &msg):
          std::runtime_error(std::string("CSVparser : ").append(msg))
        {
        }
    };

    class Row                                         // Holds a header and information on 1 bid. Both string vectors
    {
      public:
          // Constructor for Row class, adds header to _header member object which holds the CSV column headers
          Row(const std::vector<std::string> &);
          ~Row(void);

      public:
            // Return the number of items in row class
            unsigned int size(void) const;
            // Adds string to end of vector of strings
            void push(const std::string &);
            // Sets a value in the row to new value. Sets it to argument string
            bool set(const std::string &, const std::string &); 

      private:
         const std::vector<std::string> _header;          // Holds CSV column header data
         std::vector<std::string> _values;                // Holds a row of values in CSV table (1 bid)

        public:

            template<typename T>
            // Returns string at certain position
            const T getValue(unsigned int pos) const
            {
                if (pos < _values.size())
                {
                    T res;
                    std::stringstream ss;
                    ss << _values[pos];
                    ss >> res;
                    return res;
                }
                throw Error("can't return this value (doesn't exist)");
            }

            // Overload [] operator on Row class. Acts like accessing an array
            const std::string operator[](unsigned int) const;
            const std::string operator[](const std::string &valueName) const;
            // Overload << operator when using on an out stream object and Row object
            // Streams out entire row in argument into the out stream object
            friend std::ostream& operator<<(std::ostream& os, const Row &row);
            friend std::ofstream& operator<<(std::ofstream& os, const Row &row);
    };

    enum DataType {
        eFILE = 0,
        ePURE = 1
    };

    class Parser
    {

    public:
       // Constructor and destructor
        Parser(const std::string &, const DataType &type = eFILE, char sep = ',');
        ~Parser(void);

    public:
        // Returns row based on row position in the Parser vector
        Row &getRow(unsigned int row) const;
        // Returns amount of rows in Row vector in Parser object. In other words it returns number of bids
        unsigned int rowCount(void) const;
        // Returns the amount of columns in the header of the csv file
        unsigned int columnCount(void) const;
        // Access function that returns the header of CSV file that's stored in Parser obect's vector
        std::vector<std::string> getHeader(void) const;
        // Returns header item in csv file based on position argument
        const std::string getHeaderElement(unsigned int pos) const;
        // Return name of file
        const std::string &getFileName(void) const;

    public:
        // Delete row in vector or Row objects, based on position argument
        bool deleteRow(unsigned int row);
        // Add a row (vector of strings) to _content vector at certain position. Return false if failed
        bool addRow(unsigned int pos, const std::vector<std::string> &);
        // Output to file the header and content rows of the parser object to _file
        void sync(void) const;

    protected:
      // Reads first row of csv file and adds it to header vector
      void parseHeader(void);
      // Reads all rows in csv file that come after header
      void parseContent(void);

    private:
        // File path
        std::string _file;
        const DataType _type;
        const char _sep;
        // vector of original CSV reading before parsing
        std::vector<std::string> _originalFile;
        // Column headers
        std::vector<std::string> _header;
        // All bid information in CSV file
        std::vector<Row *> _content;

    public:
        // Returns row based on row position, but uses bracket notation just like accessing array items
        Row &operator[](unsigned int row) const;
    };
}

#endif /*!_CSVPARSER_HPP_*/
