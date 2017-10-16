@{from canard_dsdlc_helpers import *}@
@{indent = 0}@{ind = '    '*indent}@
#include <@(msg_header_name)>
#include <string.h>

uint32_t encode_@(msg_underscored_name)(uint8_t* buffer, @(msg_c_type)* msg) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, @(msg_underscored_name.upper())_MAX_PACK_SIZE);
    _encode_@(msg_underscored_name)(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_@(msg_underscored_name)(const CanardRxTransfer* transfer, @(msg_c_type)* msg) {
    uint32_t bit_ofs = 0;
    _decode_@(msg_underscored_name)(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_@(msg_underscored_name)(uint8_t* buffer, uint32_t* bit_ofs, @(msg_c_type)* msg, bool tao) {
@{indent += 1}@{ind = '    '*indent}@
@(ind)(void)tao;

@[  if msg_union]@
@(ind)@(union_msg_tag_uint_type_from_num_fields(len(msg_fields))) @(msg_underscored_name)_type = msg->@(msg_underscored_name)_type;
@(ind)canardEncodeScalar(buffer, *bit_ofs, @(union_msg_tag_bitlen_from_num_fields(len(msg_fields))), &@(msg_underscored_name)_type);
@(ind)*bit_ofs += @(union_msg_tag_bitlen_from_num_fields(len(msg_fields)));

@(ind)switch(msg->@(msg_underscored_name)_type) {
@{indent += 1}@{ind = '    '*indent}@
@[  end if]@
@[    for field in msg_fields]@
@[      if msg_union]@
@(ind)case @(msg_underscored_name.upper())_TYPE_@(field.name.upper()): {
@{indent += 1}@{ind = '    '*indent}@
@[      end if]@
@[      if field.type.category == field.type.CATEGORY_COMPOUND]@
@(ind)_encode_@(underscored_name(field.type))(buffer, bit_ofs, &msg->@(field.name), @('tao' if field == msg_fields[-1] else 'false'));
@[      elif field.type.category == field.type.CATEGORY_PRIMITIVE]@
@(ind)canardEncodeScalar(buffer, *bit_ofs, @(field.type.bitlen), &msg->@(field.name));
@(ind)*bit_ofs += @(field.type.bitlen);
@[      elif field.type.category == field.type.CATEGORY_ARRAY]@
@[        if field.type.mode == field.type.MODE_DYNAMIC]@
@[          if field == msg_fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
@(ind)if (!tao) {
@{indent += 1}@{ind = '    '*indent}@
@[          end if]@
@(ind)canardEncodeScalar(buffer, *bit_ofs, @(array_len_field_bitlen(field.type)), &msg->@(field.name)_len);
@(ind)*bit_ofs += @(array_len_field_bitlen(field.type));
@[          if field == msg_fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[          end if]@
@(ind)for (size_t i=0; i < msg->@(field.name)_len; i++) {
@[        else]@
@(ind)for (size_t i=0; i < @(field.type.max_size); i++) {
@[        end if]@
@{indent += 1}@{ind = '    '*indent}@
@[        if field.type.value_type.category == field.type.value_type.CATEGORY_PRIMITIVE]@
@(ind)    canardEncodeScalar(buffer, *bit_ofs, @(field.type.value_type.bitlen), &msg->@(field.name)[i]);
@(ind)    *bit_ofs += @(field.type.value_type.bitlen);
@[        elif field.type.value_type.category == field.type.value_type.CATEGORY_COMPOUND]@
@(ind)    _encode_@(underscored_name(field.type.value_type))(buffer, bit_ofs, &msg->@(field.name)[i], @[if field == msg_fields[-1] and field.type.value_type.get_min_bitlen() < 8]tao && i==msg->@(field.name)_len@[else]false@[end if]@);
@[        end if]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      elif field.type.category == field.type.CATEGORY_VOID]@
@(ind)*bit_ofs += @(field.type.bitlen);
@[      end if]@
@[      if msg_union]@
@(ind)break;
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      end if]@
@[    end for]@
@[  if msg_union]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[  end if]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}

void _decode_@(msg_underscored_name)(const CanardRxTransfer* transfer, uint32_t* bit_ofs, @(msg_c_type)* msg, bool tao) {
@{indent += 1}@{ind = '    '*indent}@
@(ind)(void)tao;

@[  if msg_union]@
@(ind)@(union_msg_tag_uint_type_from_num_fields(len(msg_fields))) @(msg_underscored_name)_type;
@(ind)canardDecodeScalar(transfer, *bit_ofs, @(union_msg_tag_bitlen_from_num_fields(len(msg_fields))), false, &@(msg_underscored_name)_type);
@(ind)msg->@(msg_underscored_name)_type = @(msg_underscored_name)_type;
@(ind)*bit_ofs += @(union_msg_tag_bitlen_from_num_fields(len(msg_fields)));

@(ind)switch(msg->@(msg_underscored_name)_type) {
@{indent += 1}@{ind = '    '*indent}@
@[  end if]@
@[    for field in msg_fields]@
@[      if msg_union]@
@(ind)case @(msg_underscored_name.upper())_TYPE_@(field.name.upper()): {
@{indent += 1}@{ind = '    '*indent}@
@[      end if]@
@[      if field.type.category == field.type.CATEGORY_COMPOUND]@
@(ind)_decode_@(underscored_name(field.type))(buffer, bit_ofs, &msg->@(field.name), @('tao' if field == msg_fields[-1] else 'false'));
@[      elif field.type.category == field.type.CATEGORY_PRIMITIVE]@
@(ind)canardDecodeScalar(transfer, *bit_ofs, @(field.type.bitlen), @('true' if uavcan_type_is_signed(field.type) else 'false') &msg->@(field.name));
@(ind)*bit_ofs += @(field.type.bitlen);
@[      elif field.type.category == field.type.CATEGORY_ARRAY]@
@[        if field.type.mode == field.type.MODE_DYNAMIC]@
@[          if field == msg_fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
@(ind)if (!tao) {
@{indent += 1}@{ind = '    '*indent}@
@[          end if]@
@(ind)canardDecodeScalar(transfer, *bit_ofs, @(array_len_field_bitlen(field.type)), false &msg->@(field.name)_len);
@(ind)*bit_ofs += @(array_len_field_bitlen(field.type));
@[          if field == msg_fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[          end if]@
@(ind)for (size_t i=0; i < msg->@(field.name)_len; i++) {
@[        else]@
@(ind)for (size_t i=0; i < @(field.type.max_size); i++) {
@[        end if]@
@{indent += 1}@{ind = '    '*indent}@
@[        if field.type.value_type.category == field.type.value_type.CATEGORY_PRIMITIVE]@
@(ind)    canardDecodeScalar(transfer, *bit_ofs, @(field.type.value_type.bitlen), @('true' if uavcan_type_is_signed(field.type.value_type) else 'false') &msg->@(field.name)[i]);
@(ind)    *bit_ofs += @(field.type.value_type.bitlen);
@[        elif field.type.value_type.category == field.type.value_type.CATEGORY_COMPOUND]@
@(ind)    _decode_@(underscored_name(field.type.value_type))(buffer, bit_ofs, &msg->@(field.name)[i], @[if field == msg_fields[-1] and field.type.value_type.get_min_bitlen() < 8]tao && i==msg->@(field.name)_len@[else]false@[end if]@);
@[        end if]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      elif field.type.category == field.type.CATEGORY_VOID]@
@(ind)*bit_ofs += @(field.type.bitlen);
@[      end if]@
@[      if msg_union]@
@(ind)break;
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      end if]@
@[    end for]@
@[  if msg_union]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[  end if]@
}
