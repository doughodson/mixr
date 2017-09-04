
#include "mixr/recorder/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/recorder/DataRecorder.hpp"
#include "mixr/recorder/FileWriter.hpp"
#include "mixr/recorder/FileReader.hpp"
#include "mixr/recorder/OutputHandler.hpp"
#include "mixr/recorder/NetInput.hpp"
#include "mixr/recorder/NetOutput.hpp"
#include "mixr/recorder/TabPrinter.hpp"
#include "mixr/recorder/PrintPlayer.hpp"
#include "mixr/recorder/PrintSelected.hpp"

#include <string>

namespace mixr {
namespace recorder {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

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
