//--------------------------------------------------------------------------
// Copyright (C) 2018-2018 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
// http_context_data.cc author Bhagya Tholpady <bbantwal@cisco.com>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "http_context_data.h"

#include "http_msg_section.h"
#include "protocols/packet.h"

using namespace snort;

unsigned HttpContextData::ips_id = 0;

HttpMsgSection* HttpContextData::get_snapshot(const Packet* p)
{
    IpsContext* context = p ? p->context : nullptr;
    HttpContextData* hcd = (HttpContextData*)DetectionEngine::get_data(HttpContextData::ips_id,
            context);

    if ( !hcd )
        return nullptr;

    return hcd->current_section;
}

void HttpContextData::save_snapshot(HttpMsgSection* section)
{
    HttpContextData* hcd = (HttpContextData*)DetectionEngine::get_data(HttpContextData::ips_id);

    if ( !hcd )
    {
        hcd = new HttpContextData;
        DetectionEngine::set_data(HttpContextData::ips_id, hcd);
    }

    hcd->current_section = section;
    section->add_ips_context(DetectionEngine::get_context());
}

HttpMsgSection* HttpContextData::clear_snapshot(IpsContext* context)
{
    HttpMsgSection* section = nullptr;
    HttpContextData* hcd = (HttpContextData*)DetectionEngine::get_data(HttpContextData::ips_id,
            context);

    if ( hcd )
    {
        section = hcd->current_section;
        hcd->clear();
    }

    return section;
}