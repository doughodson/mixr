
#include "mxrp/recorder/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/recorder/DataRecorder.hpp"
#include "mxrp/recorder/FileWriter.hpp"
#include "mxrp/recorder/FileReader.hpp"
#include "mxrp/recorder/OutputHandler.hpp"
#include "mxrp/recorder/NetInput.hpp"
#include "mxrp/recorder/NetOutput.hpp"
#include "mxrp/recorder/TabPrinter.hpp"
#include "mxrp/recorder/PrintPlayer.hpp"
#include "mxrp/recorder/PrintSelected.hpp"

#include <string>

namespace mxrp {
namespace recorder {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == FileWriter::getFactoryName() ) {
        obj = new FileWriter();
    }
    else if ( name == FileReader::getFactoryName() ) {
        obj = new FileReader();
    }
    else if ( name == NetInput::getFactoryName() ) {
        obj = new NetInput();
    }
    else if ( name == NetOutput::getFactoryName() ) {
        obj = new NetOutput();
    }
    else if ( name == OutputHandler::getFactoryName() ) {
        obj = new OutputHandler();
    }
    else if ( name == TabPrinter::getFactoryName() ) {
        obj = new TabPrinter();
    }
    else if ( name == PrintPlayer::getFactoryName() ) {
        obj = new PrintPlayer();
    }
    else if ( name == DataRecorder::getFactoryName()) {
        obj = new DataRecorder();
    }
    else if ( name == PrintSelected::getFactoryName() ) {
        obj = new PrintSelected();
    }

    return obj;
}

}
}
