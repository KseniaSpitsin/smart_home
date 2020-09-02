#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include "common_utils.hpp"
#include "configuration_file_details.hpp"

namespace smarthome
{

class Location
{
public:
    typedef std::string RoomType;
    typedef std::string FloorType;

public:
    Location(const FloorType& a_floor = ANY, const RoomType& a_room = ANY);
    virtual ~Location() NOEXCEPTIONS;
    //Location(const Location& a_other) = default;
    //Location& operator=(const Location& a_other) = default;

    const RoomType& GetRoom() const NOEXCEPTIONS;
    const FloorType& GetFloor() const NOEXCEPTIONS;

    bool operator==(const Location& a_other) const NOEXCEPTIONS;
    bool operator!=(const Location& a_other) const NOEXCEPTIONS;

private:
    FloorType m_floor;
    RoomType m_room;
};

} //namespace smarthome

#endif //LOCATION_HPP