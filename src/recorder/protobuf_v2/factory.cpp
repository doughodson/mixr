
#include "mixr/recorder/protobuf_v2/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/recorder/protobuf_v2/DataRecorder.hpp"
#include "mixr/recorder/protobuf_v2/FileWriter.hpp"
#include "mixr/recorder/protobuf_v2/FileReader.hpp"
#include "mixr/recorder/protobuf_v2/OutputHandler.hpp"
#include "mixr/recorder/protobuf_v2/NetInput.hpp"
#include "mixr/recorder/protobuf_v2/NetOutput.hpp"
#include "mixr/recorder/protobuf_v2/TabPrinter.hpp"
#include "mixr/recorder/protobuf_v2/PrintPlayer.hpp"
#include "mixr/recorder/protobuf_v2/PrintSelected.hpp"

#include <string>

namespace mixr {
namespace recorder {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

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
