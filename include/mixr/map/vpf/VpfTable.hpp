
#ifndef __mixr_map_vpf_VpfTable_H__
#define __mixr_map_vpf_VpfTable_H__

#include "mixr/base/Object.hpp"

#include <array>

namespace mixr {
namespace base { class List; class String; }
namespace vpf {
class VpfIndexTable;
class VpfRecord;

// --------------------------------------------------------------
// Class: VpfTable
// Description: Default vector product format table
// --------------------------------------------------------------
class VpfTable : public base::Object
{
   DECLARE_SUBCLASS(VpfTable, base::Object)

public:
    VpfTable();

    struct ColumnDefinitions {
        char name[255];
        char dataType;
        char number[255];
        char keyType;
        char columnDesc[255];
        char valueDescTableName[255];
        char thematicIndex[255];
        char narrTable[255];
    };

    // Set functions
    virtual void setHeaderLength(const int);
    virtual void setByteOrder(char);
    virtual void setTableDesc(char*);
    virtual void setNarrTable(char*);
    // Column functions
    virtual void setCurrentColumn(const int);
    virtual void setColumnName(char*);
    virtual void setColumnDataType(char);
    virtual void setColumnNumber(char*);
    virtual void setColumnKeyType(char);
    virtual void setColumnDesc(char*);
    virtual void setColumnValueDescTableName(char*);
    virtual void setColumnThematicIndex(char*);
    virtual void setColumnNarrTable(char*);

    // Get functions
    int         getHeaderLength()               { return headerLength; }
    const char  getByteOrder()                  { return byteOrder; }
    const char* getTableDesc()                  { return tableDesc.data(); }
    const char* getNarrTable()                  { return narrTable.data(); }
    const char* getColumnName()                 { return columns[cCol].name; }
    const char  getColumnDataType()             { return columns[cCol].dataType; }
    const char* getColumnNumber()               { return columns[cCol].number; }
    const char  getColumnKeyType()              { return columns[cCol].keyType; }
    const char* getColumnDesc()                 { return columns[cCol].columnDesc; }
    const char* getColumnValueDescTableName()   { return columns[cCol].valueDescTableName; }
    const char* getColumnThematicIndex()        { return columns[cCol].thematicIndex; }
    const char* getColumnNarrTable()            { return columns[cCol].narrTable; }
    VpfIndexTable* getIndexTable()              { return idxTable; }

    int getNumberOfRows()                       { return numRows; }
    int getNumberOfColumns()                    { return numColumns; }
    bool isLoaded()                             { return loaded; }

    // Specific set function (for directories to know what type we are)
    virtual void setType(const int x)           { type = x; }
    int getType()                               { return type; }
    bool isType(const int x)                    { if (type == x) return true; else return false; }

    // Size of one record (if we are fixed length)
    int getRecordSize()                         { return recordSize; }
    VpfRecord* getRecord(const int idx);             

    virtual bool loadTableFromFile(const char* pathname, const char* filename, const int xType = -1);
    static const int MAX_COLUMNS {50};
    static const int MAX_ROWS    {5000};

    virtual void loadIndexFile();

private:
    void determineRecordSize();
    int headerLength {};   
    char byteOrder {};
    std::array<char, 255> tableDesc {};
    std::array<char, 255> narrTable {};

    int cCol {};                                  // Column we are on
    int numColumns {};                            // Number of columns in our column list
    int numRows {};                               // Number of data elements (rows of data) we have
    bool loaded {};                               // Have we already read the file?
    int recordSize {};                            // Size of 1 record (if we aren't variable length)
    base::String* path {};                        // Path file
    base::String* name {};                        // Name of our table (basically the name of the file)

    std::array<ColumnDefinitions, MAX_COLUMNS> columns;
    std::array<VpfRecord*, MAX_ROWS> records {};  // Holds our records

    VpfIndexTable* idxTable {};                   // Holds our associated index table (if we need one)
    int type {};                                  // Type of table we are (header table, feature table, etc...)
};

}
}

#endif
