@[if msg.kind == msg.KIND_MESSAGE]@
@{from canard_dsdlc_helpers import *}@
#pragma once

#include <stdint.h>
#include <float16.h>
#include <string.h>

@[  for field in msg.fields]@
@[    if field.type.category == field.type.CATEGORY_COMPOUND]@
#include <@(msg_header_name(field))>
@[    end if]@
@[  end for]@

#define @(underscored_name(msg).upper())_MAX_PACK_SIZE @((msg.get_max_bitlen()+7)/8)
#define @(underscored_name(msg).upper())_DT_SIG @('0x%08X' % (msg.get_data_type_signature(),))
@[  if msg.default_dtid is not None]@
#define @(underscored_name(msg).upper())_DT_ID @(msg.default_dtid)
@[  end if]@
@[  if msg.constants]
@[    for constant in msg.constants]@
#define @(underscored_name(msg).upper())_@(constant.name.upper()) @(constant.value)
@[    end for]@
@[  end if]@
@[  if msg.union]
enum @(underscored_name(msg))_type_t {
@[    for field in msg.fields]@
    @(underscored_name(msg).upper())_TYPE_@(field.name.upper()),
@[    end for]@
};
@[  end if]@

struct @(underscored_name(msg))_s {
@[  if msg.union]@
    enum @(underscored_name(msg))_type_t @(underscored_name(msg))_type;
    union {
@[    for field in msg.fields]@
        @(field_cdef(field));
@[    end for]@
    };
@[  else]@
@[    for field in msg.fields]@
    @(field_cdef(field));
@[    end for]@
@[  end if]@
};

@#//if msg.kind == msg.KIND_MESSAGE
@[end if]@
