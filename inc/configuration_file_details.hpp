#ifndef CONFIGURATION_FILE_DETAILS_HPP
#define CONFIGURATION_FILE_DETAILS_HPP

#include <string>

namespace smarthome
{

const std::string ANY = "*";
const std::string CONTROLLER = "Controller";
const std::string SENSOR = "Sensor";

const char AGENT_INFORMATION_DETAILS_DELIMITER = ':';

const char EVENT_TOPIC_DELIMITER = '|';
const char EVENT_TOPIC_INFORMATION_DELIMITER = '~';

} //namespace smarthome

#endif //CONFIGURATION_FILE_DETAILS_HPP