import os
import uavcan
import errno
import em
import math
import copy

def array_len_field_bitlen(max_size):
    return int(math.ceil(math.log(max_size+1,2)))

def c_int_type_bitlen(bitlen):
    for ret in (8, 16, 32, 64):
        if bitlen <= ret:
            return ret

def uavcan_type_to_ctype(uavcan_type):
    if uavcan_type.category == uavcan_type.CATEGORY_COMPOUND:
        return 'struct %s_s' % (underscored_name(uavcan_type))
    elif uavcan_type.category == uavcan_type.CATEGORY_PRIMITIVE:
        if uavcan_type.kind == uavcan_type.KIND_BOOLEAN:
            return 'bool'
        elif uavcan_type.kind == uavcan_type.KIND_UNSIGNED_INT:
            return 'uint%u_t' % (c_int_type_bitlen(uavcan_type.bitlen))
        elif uavcan_type.kind == uavcan_type.KIND_SIGNED_INT:
            return 'int%u_t' % (c_int_type_bitlen(uavcan_type.bitlen))
        elif uavcan_type.kind == uavcan_type.KIND_FLOAT:
            return 'float' if uavcan_type.bitlen <= 32 else 'double'

def field_cdef(field):
    if field.type.category == field.type.CATEGORY_ARRAY:
        if field.type.mode == field.type.MODE_STATIC:
            return '%s %s[%u]' % (uavcan_type_to_ctype(field.type.value_type), field.name, field.type.max_size)
        else:
            return 'struct {uint%u_t %s_len; %s %s[%u]}' % (c_int_type_bitlen(array_len_field_bitlen(field.type.max_size)), field.name, uavcan_type_to_ctype(field.type.value_type), field.name, field.type.max_size)
    else:
        return '%s %s' % (uavcan_type_to_ctype(field.type), field.name)


def ctype(obj):
    if isinstance(obj, uavcan.dsdl.Field):
        obj = obj.type

    if obj.category == uavcan.dsdl.Type.CATEGORY_PRIMITIVE:
        return obj.full_name
    if obj.category == uavcan.dsdl.Type.CATEGORY_COMPOUND:
        return 'struct %s_s' % (underscored_name(obj),)

def indent(string, n):
    if string.strip():
        string = '    '*n + string
        string.replace('\n', '\n' + '    '*n)
    return string

def msg_header_name_request(obj):
    if isinstance(obj, uavcan.dsdl.Field):
        obj = obj.type
    return os.path.join(rel_path(obj), '%s_Request.h' % (short_name(obj),))

def msg_header_name_response(obj):
    if isinstance(obj, uavcan.dsdl.Field):
        obj = obj.type
    return os.path.join(rel_path(obj), '%s_Response.h' % (short_name(obj),))

def msg_header_name(obj):
    if isinstance(obj, uavcan.dsdl.Field):
        obj = obj.type
    return os.path.join(rel_path(obj), '%s.h' % (short_name(obj),))

def underscored_name(obj):
    return obj.full_name.replace('.','_')

def rel_path(obj):
    return os.path.join(*obj.full_name.split('.')[:-1])

def short_name(obj):
    return obj.full_name.split('.')[-1]

# https://stackoverflow.com/questions/600268/mkdir-p-functionality-in-python
def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise
