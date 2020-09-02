#include "location.hpp"

namespace smarthome
{

Location::Location(const FloorType& a_floor, const RoomType& a_room)
    : m_floor(a_floor)
    , m_room(a_room)
{
}

Location::~Location() NOEXCEPTIONS
{
}

const Location::RoomType& Location::GetRoom() const NOEXCEPTIONS
{
    return m_room;
}

const Location::FloorType& Location::GetFloor() const NOEXCEPTIONS
{
    return m_floor;
}

bool Location::operator==(const Location& a_other) const NOEXCEPTIONS
{
    return m_floor == a_other.m_floor && m_room == a_other.m_room;
}

bool Location::operator!=(const Location& a_other) const NOEXCEPTIONS
{
    return !(*this == a_other);
}

} //namespace smarthome