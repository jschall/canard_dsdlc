@[if msg.kind == msg.KIND_SERVICE]@
@{from canard_dsdlc_helpers import *}@
#pragma once
#include <@(msg_header_name_request(msg))>
#include <@(msg_header_name_response(msg))>
@[end if]@
