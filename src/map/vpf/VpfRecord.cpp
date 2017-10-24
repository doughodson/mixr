#include "mixr/map/vpf/VpfRecord.hpp"

#include "mixr/map/vpf/VpfIndexTable.hpp"
#include "mixr/map/vpf/VpfTable.hpp"
#include "mixr/map/vpf/VpfDataType.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/osg/Vec3d"

#include <cstdlib>
#include <fstream>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VpfRecord, "VpfRecord")
EMPTY_SLOTTABLE(VpfRecord)
EMPTY_COPYDATA(VpfRecord)

VpfRecord::VpfRecord()
{
    STANDARD_CONSTRUCTOR()
}

void VpfRecord::deleteData()
{
    for (int i = 0; i < VpfTable::MAX_COLUMNS; i++) {
        if (data[i] != nullptr) data[i]->unref();
        data[i] = nullptr;
    }
    if (parent != nullptr) {  
        parent->unref();
        parent = nullptr;
    }
}

void VpfRecord::createRecord(VpfTable* x, const char* file, const int idx) 
{
    if (parent != nullptr) parent->unref();
    parent = nullptr;
    if (x != nullptr) {
        parent = x;
        parent->ref();
    }
    if (filename == nullptr) filename = new base::String(file);
    else filename->setStr(file);
    index = idx;

    // ok, we have our table and our file, let's open it up, skip the header, 
    // grab the record, then load up our data, and return
    std::ifstream stream;
    stream.open(filename->getString());
    if (!stream.fail() && parent != 0) {
        // let's try to get the size of my file contents
        stream.seekg(0);
        std::streamoff begin{stream.tellg()};
        stream.seekg(0, std::ios::end);
        std::streamoff end{stream.tellg()};
        std::streamoff mySize{end - begin};
        int size {parent->getRecordSize()};
        if (size > 0) {
            int recordOffset = size * (idx - 1);
            // now offest our header, plus the index * size of one records
            stream.seekg(parent->getHeaderLength() + sizeof(int) + recordOffset, std::ios::beg);
            if (stream.tellg() >= mySize) {
                //std::cout << "TELL G = " << stream.tellg() << std::endl;
                //std::cout << "SIZE = " << mySize << std::endl;
                eor = true;
            }
            else {
                // now read our data
                bool rowNull {};
                for (int i = 0; i < parent->getNumberOfColumns(); i++) {
                    parent->setCurrentColumn(i+1);
                    char dType {parent->getColumnDataType()};
                    int num {std::atoi(parent->getColumnNumber())};
                    // now compare the data types to figure our data
                    if (data[i] == 0) data[i] = new VpfDataType();
                    // integer (4 bytes)
                    if (dType == 'I') {
                        // we read an integer
                        int y {};
                        // 4 byte integer
                        stream.read(reinterpret_cast<char*>(&y), num*sizeof(y));
                        // convert to string
                        char tempBuff[255] {};
                        std::sprintf(tempBuff, "%i", y);
                        //itoa(y, tempBuff, 10);
                        if (y == 0) rowNull = true;
                        // create a data type
                        if (rowNull) data[i]->setValue(NULL);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VpfDataType::LONG_INT);
                    }
                    // short integer (2 bytes)
                    else if (dType == 'S') {
                        // we read a short integer
                        short int y {};
                        // 2 byte integer
                        stream.read((char*)&y, num*sizeof(y));
                        // convert to string
                        char tempBuff[255] {};
                        std::sprintf(tempBuff, "%i", y);
                        //itoa(y, tempBuff, 10);
                        if (rowNull) data[i]->setValue(NULL);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VpfDataType::SHORT_INT);
                    }
                    // text (fixed and variable length)
                    else if (dType == 'T') {
                        char buff[255] {};
                        stream.read(buff, num);
                        buff[num] = 0;
                        if (!rowNull) data[i]->setValue(buff);
                        else data[i]->setValue(NULL);

                        data[i]->setLength(num);
                        data[i]->setType(VpfDataType::FIXED_TEXT);
                    }
                    else if (dType == 'D') {
                        // for every number of digits, we add 20 bits
                        // for the date
                        char buff[255] {};
                        stream.read(buff, num*20);
                        buff[num*20] = 0;
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*20);
                        data[i]->setType(VpfDataType::DATE);
                    }
                    // 4 byte floating point number
                    else if (dType == 'F') {
                        float tempFloat {};
                        char buff[255] {};
                        stream.read((char*)&tempFloat, num*sizeof(tempFloat));
                        //gcvt(tempFloat, 10, buff);  
                        std::sprintf(buff, "%f", tempFloat);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempFloat));
                        data[i]->setType(VpfDataType::FLOAT);
                    }
                    // 8 byte floating point (double)
                    else if (dType == 'R') {
                        double tempDouble {};
                        char buff[255] {};
                        stream.read((char*)&tempDouble, num*sizeof(tempDouble));
                        //gcvt(tempDouble, 10, buff);
                        std::sprintf(buff, "%f", tempDouble);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempDouble));
                        data[i]->setType(VpfDataType::DOUBLE);
                    }
                    // 3 coordinate array
                    else if (dType == 'Z') {
                        float tempFloat {};
                        base::String* string {new base::String()};
                        char buff[255] {};
                        stream.read((char*)&tempFloat, sizeof(tempFloat));
                        //gcvt(tempFloat, 8, buff);
                        std::sprintf(buff, "%f", tempFloat);
                        string->catStr(buff);
                        string->catStr(", ");
                        stream.read((char*)&tempFloat, sizeof(tempFloat));
                        //gcvt(tempFloat, 8, buff);
                        std::sprintf(buff, "%f", tempFloat);
                        string->catStr(buff);
                        string->catStr(", ");
                        //stream.read((char*)&tempFloat, sizeof(tempFloat));
                        stream.read((char*)&tempFloat, sizeof(tempFloat));
                        // this is a quick and easy test for Quiet Not a Number, which we will just revert to 1
                        if (tempFloat != tempFloat) {
                            tempFloat = 1.0f;
                        }
                        //gcvt(tempFloat, 8, buff);
                        std::sprintf(buff, "%f", tempFloat);
                        string->catStr(buff);
                        string->catStr(" ");
                        //std::cout << "COORDINATE STRING = " << string->getString() << std::endl;

                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue((char*)string->getString());
                        data[i]->setLength(sizeof(tempFloat)*3);
                        data[i]->setType(VpfDataType::THREE_D_COORD);
                        string->unref();
                    }
                    else {
                        std::cout << "UNKNOWN TYPE = " << dType << std::endl;
                        // clean up our data row, because we didn't use it
                        data[i]->unref();
                        data[i] = nullptr;
                    }
                }
            }
        }
        else {
            // must be an index file associated with it
            VpfIndexTable* t {parent->getIndexTable()};
            if (t != nullptr) {
                int offset {}, length {};
                t->getRecordPosition(idx, offset, length);
                // now we know the record position, we can open our file and parse it appropriately
                stream.seekg(offset, std::ios::beg);
                //std::cout << "OFFSET / LENGTH = " << offset << ", " << length << std::endl;
                // now read our data
                bool rowNull {};
                bool vLength {};
                for (int i = 0; i < parent->getNumberOfColumns(); i++) {
                    vLength = false;
                    parent->setCurrentColumn(i+1);
                    char dType {parent->getColumnDataType()};
                    int num {std::atoi(parent->getColumnNumber())};
                    if (num == 0) {
                        // this column is variable length!
                        vLength = true;
                    }
                    // now compare the data types to figure our data
                    if (data[i] == 0) data[i] = new VpfDataType();
                    // integer (4 bytes)
                    if (dType == 'I') {
                        // we read an integer
                        int y {};
                        // 4 byte integer
                        stream.read(reinterpret_cast<char*>(&y), num*sizeof(y));
                        // convert to string
                        char tempBuff[255] {};
                        std::sprintf(tempBuff, "%i", y);
                        //itoa(y, tempBuff, 10);
                        if (y == 0) rowNull = true;
                        // create a data type
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VpfDataType::LONG_INT);
                    }
                    // short integer (2 bytes)
                    else if (dType == 'S') {
                        // we read a short integer
                        short int y {};
                        // 2 byte integer
                        stream.read(reinterpret_cast<char*>(&y), num*sizeof(y));
                        // convert to string
                        char tempBuff[255] {};
                        std::sprintf(tempBuff, "%i", y);                        
                        //itoa(y, tempBuff, 10);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(tempBuff);
                        data[i]->setLength(num*sizeof(y));
                        data[i]->setType(VpfDataType::SHORT_INT);
                    }
                    // text (fixed and variable length)
                    else if (dType == 'T') {
                        char buff[255] {};
                        if (vLength) {
                            stream.read(buff, num);
                            buff[num] = 0;
                            data[i]->setLength(1);
                            data[i]->setType(VpfDataType::VARIABLE_TEXT);
                        }
                        else {
                            stream.read(buff, num);
                            buff[num] = 0;
                            data[i]->setLength(num);
                            data[i]->setType(VpfDataType::FIXED_TEXT);
                        }
                        if (!rowNull) data[i]->setValue(buff);
                        else data[i]->setValue(0);
                        
                    }
                    else if (dType == 'D') {
                        // for every number of digits, we add 20 bits
                        // for the date
                        char buff[255] {};
                        stream.read(buff, num*20);
                        buff[num*20] = 0;
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*20);
                        data[i]->setType(VpfDataType::DATE);
                    }
                    // 4 byte floating point number
                    else if (dType == 'F') {
                        float tempFloat {};
                        char buff[255] {};
                        stream.read((char*)&tempFloat, num*sizeof(tempFloat));
                        //gcvt(tempFloat, 10, buff);
                        std::sprintf(buff, "%f", tempFloat);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempFloat));
                        data[i]->setType(VpfDataType::FLOAT);
                    }
                    // 8 byte floating point (double)
                    else if (dType == 'R') {
                        double tempDouble {};
                        char buff[255] {};
                        stream.read((char*)&tempDouble, num*sizeof(tempDouble));
                        //gcvt(tempDouble, 10, buff);
                        std::sprintf(buff, "%f", tempDouble);
                        if (rowNull) data[i]->setValue(0);
                        else data[i]->setValue(buff);
                        data[i]->setLength(num*sizeof(tempDouble));
                        data[i]->setType(VpfDataType::DOUBLE);
                    }
                    // 3 coordinate array
                    else if (dType == 'Z') {
                        if (vLength) {
                            // the first length of the first part of our vLength field is the number of coordinate pairs we have
                            stream.read((char*)&numCoords, sizeof(numCoords));
                            //std::cout << "NUM COORDS = " << numCoords << std::endl;
                            char buff[255] {};
                            float tempFloat {};
                            base::String* string {new base::String()};
                            for (int j = 0; j < numCoords; j++) {
                                stream.read((char*)&tempFloat, sizeof(tempFloat));
                                //gcvt(tempFloat, 8, buff);
                                std::sprintf(buff, "%f", tempFloat);
                                string->catStr(buff);
                                string->catStr(" ");
                                stream.read((char*)&tempFloat, sizeof(tempFloat));
                                //gcvt(tempFloat, 8, buff);
                                std::sprintf(buff, "%f", tempFloat);
                                string->catStr(buff);
                                string->catStr(" ");
                                stream.read(reinterpret_cast<char*>(&tempFloat), sizeof(tempFloat));
                                //gcvt(tempFloat, 8, buff);
                                std::sprintf(buff, "%f", tempFloat);
                                string->catStr(buff);
                                string->catStr(" ");
                            }
                            if (rowNull) data[i]->setValue(0);
                            else data[i]->setValue((char*)string->getString());
                            data[i]->setLength(numCoords * 3);
                            data[i]->setType(VpfDataType::THREE_D_COORD);
                            //std::cout << "COORDINATES = " << string->getString() << std::endl;
                            string->unref();
                        }
                        else {
                            numCoords = 1;
                            float tempFloat {};
                            base::String* string {new base::String()};
                            char buff[255] {};
                            stream.read(reinterpret_cast<char*>(&tempFloat), sizeof(tempFloat));
                            //gcvt(tempFloat, 8, buff);
                            std::sprintf(buff, "%f", tempFloat);
                            string->catStr(buff);
                            string->catStr(" ");
                            stream.read(reinterpret_cast<char*>(&tempFloat), sizeof(tempFloat));
                            //gcvt(tempFloat, 8, buff);
                            std::sprintf(buff, "%f", tempFloat);
                            string->catStr(buff);
                            string->catStr(" ");
                            stream.read(reinterpret_cast<char*>(&tempFloat), sizeof(tempFloat));
                            //gcvt(tempFloat, 8, buff);
                            std::sprintf(buff, "%f", tempFloat);
                            string->catStr(buff);
                            string->catStr(" ");
                            if (rowNull) data[i]->setValue(0);
                            else data[i]->setValue(const_cast<char*>(string->getString()));
                            data[i]->setLength(sizeof(tempFloat)*3);
                            data[i]->setType(VpfDataType::THREE_D_COORD);
                            std::cout << "COORDINATE = " << string->getString() << std::endl;
                            string->unref();
                        }
                    }
                    else {
                        std::cout << "UNKNOWN TYPE = " << dType << std::endl;
                        // clean up our data row, because we didn't use it
                        data[i]->unref();
                        data[i] = 0;
                    }
                }
            }
            else {
                std::cout << "ERROR: VPFTable has no index table and no associated record!" << std::endl;
            }
        }
        stream.close();
    }
    else {
        std::cout << "FAILURE TO OPEN FILE = " << filename->getString() << std::endl;
    }
}


// This function is for retrieving variable length coordinate strings, and
// will return the number of coordinates as well as an base::Vec3d array filled
// with those coordinates
int VpfRecord::getCoordinate(const int column, base::Vec3d vec[], const int idx, const int max)
{
    // first, get the data from the column
    if (data[column-1] != 0) {
        if (data[column-1]->getType() == VpfDataType::THREE_D_COORD) {
            // ok, they have a valid column, now lets convert our string into 3 coordinates (x,y,z from center)
            float first{}, second{}, third{};
            // grab our first coordinate
            char temp[255] {};
            char* x {const_cast<char*>(data[column-1]->getValue())};
            int j {idx};
            int count {};
            int tCount {};
            for (int i = 0; i < numCoords; i++) {
                tCount = 0;
                while (x[count] != ' ') {
                    temp[tCount++] = x[count++];
                }
                temp[tCount] = 0;

                first = static_cast<float>(std::atof(temp));
                // our first value is our LONGITUDE!
                // now, moving on from that first coordinate, we go to our second one
                temp[0] = 0;
                tCount = 0;
                count++;
                while (x[count] != ' ') temp[tCount++] = x[count++];
                temp[tCount] = 0;
                second = static_cast<float>(std::atof(temp));
                //if ((first <= 45 && first >= 44) && (second >= 33 && second <= 34)) {
                    //std::cout << "IDX = " << idx+1 << std::endl;
                    //std::cout << "NUM COORDINATES = " << numCoords << std::endl;
                    //std::cout << "PRIMITIVE ID = " << data[1]->getValue() << std::endl;
                    //std::cout << "LON = " << first << std::endl;
                    //std::cout << "LAT = " << second << std::endl;
                //}
                // now the third coordinate, which right now is just 1
                tCount = 0;
                temp[0] = 0;
                count++;
                while(x[count] != ' ') temp[tCount++] = x[count++];
                temp[tCount] = 0;
                //std::cout << "THIRD = " << temp << std::endl;
                third = static_cast<float>(std::atof(temp));
                count++;
                if (j < max) vec[j++].set(second,first,third);
                j++;
            }
        }
    }
    return numCoords;
}

// this just returns the character string that was parsed from the file
const char* VpfRecord::getData(const int column)
{
    if (column < VpfTable::MAX_COLUMNS && data[column-1] != 0) {
        //std::cout << "RECORD # & DATA = " << index << ", " << data[column-1]->getValue() << std::endl;
        return data[column-1]->getValue();
    } else {
        return nullptr;
    }
}

}
}
