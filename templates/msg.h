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
    @(field_union_type_enum_name(msg, field)),
@[    end for]@
};
@[  end if]@

@(uavcan_type_to_ctype(msg)) {
@[  if msg.union]@
    enum @(underscored_name(msg))_type_t @(underscored_name(msg))_type;
    union {
@[    for field in msg.fields]@
@[      if field.type.category != field.type.CATEGORY_VOID]@
        @(field_cdef(field));
@[      end if]@
@[    end for]@
    };
@[  else]@
@[    for field in msg.fields]@
@[      if field.type.category != field.type.CATEGORY_VOID]@
    @(field_cdef(field));
@[      end if]@
@[    end for]@
@[  end if]@
};

void _encode_@(underscored_name(msg))(uint8_t buffer[], uint32_t* bit_ofs, @(uavcan_type_to_ctype(msg))* msg, bool tao);
@[end if]@
