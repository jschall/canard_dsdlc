@[if msg.kind == msg.KIND_MESSAGE]@
#pragma once

#include <stdint.h>
#include <float16.h>
#include <string.h>

@[  for field in msg.fields]@
@[    if field.type.category == field.type.CATEGORY_COMPOUND]
#include <@(msg_header_name(field))>
@[    end if]@
@[  end for]@

@[  for constant in msg.constants]@
#define @(underscored_name(msg).upper())_@(constant.name.upper()) @(constant.value)
@[  end for]@



@#//if msg.kind == msg.KIND_MESSAGE
@[end if]@
