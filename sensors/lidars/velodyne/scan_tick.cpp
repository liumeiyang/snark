// This file is part of snark, a generic and flexible library for robotics research
// Copyright (c) 2011 The University of Sydney
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the University of Sydney nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
// HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "scan_tick.h"
#include "hdl64/packet.h"
#include "puck/packet.h"

namespace snark {  namespace velodyne {

namespace detail {
    
static unsigned int packet_angle_( const velodyne::hdl64::packet& p ) { return p.blocks[0].rotation() + 9000; } // 0 = behind the vehicle

static unsigned int packet_angle_( const velodyne::puck::packet& p ) { return p.blocks[0].azimuth() + ( 36000 - 9000 ) + 9000; } // quick and dirty: -90 degrees for now; see todo comments in puck/calculator.cpp

} // namespace detail {
    
template < typename P >
bool scan_tick::is_new_scan( const P& packet, unsigned int last_angle )
{
    unsigned int angle = detail::packet_angle_( packet );
    if( angle > 36000 ) { angle -= 36000; }
    return angle < last_angle;
}

template < typename P >
bool scan_tick::is_new_scan( const P& packet )
{
    bool tick = false;
    unsigned int angle = detail::packet_angle_( packet );
    if( angle > 36000 ) { angle -= 36000; }
    if( !last_angle_ || angle < *last_angle_ ) { tick = true; }
    last_angle_ = angle;
    return tick;
}

template bool scan_tick::is_new_scan< hdl64::packet >( const hdl64::packet& packet );
template bool scan_tick::is_new_scan< puck::packet >( const puck::packet& packet );
template bool scan_tick::is_new_scan< hdl64::packet >( const hdl64::packet& packet, unsigned int last_angle );
template bool scan_tick::is_new_scan< puck::packet >( const puck::packet& packet, unsigned int last_angle );

} } // namespace snark {  namespace velodyne {
