#ifndef EVENT_CHANNEL_CONFIGURATION_HPP
#define EVENT_CHANNEL_CONFIGURATION_HPP

#include <cstddef>
#include <sys/sysinfo.h>

namespace smarthome
{

const size_t EVENT_CHANNEL_CAPACITY = 20;
const int NUM_OF_CHANNELS = get_nprocs_conf();

} //namespace smarthome

#endif //EVENT_CHANNEL_CONFIGURATION_HPP