@[if msg.kind == msg.KIND_MESSAGE]@
@{from canard_dsdlc_helpers import *}@
@{indent = 0}@{ind = '    '*indent}@
#include <@(msg_header_name(msg))>
#include <string.h>

uint32_t encode_@(underscored_name(msg))(uint8_t* buffer, @(uavcan_type_to_ctype(msg))* msg) {
    uint32_t bit_ofs = 0;
    memset(buffer, 0, @(underscored_name(msg).upper())_MAX_PACK_SIZE);
    _encode_@(underscored_name(msg))(buffer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

uint32_t decode_@(underscored_name(msg))(const CanardRxTransfer* transfer, @(uavcan_type_to_ctype(msg))* msg) {
    uint32_t bit_ofs = 0;
    _decode_@(underscored_name(msg))(transfer, &bit_ofs, msg, true);
    return (bit_ofs+7)/8;
}

void _encode_@(underscored_name(msg))(uint8_t* buffer, uint32_t* bit_ofs, @(uavcan_type_to_ctype(msg))* msg, bool tao) {
@{indent += 1}@{ind = '    '*indent}@
@(ind)(void)tao;

@[  if msg.union]@
@(ind)@(union_msg_tag_ctype(msg)) @(underscored_name(msg))_type = msg->@(underscored_name(msg))_type;
@(ind)canardEncodeScalar(buffer, *bit_ofs, @(union_msg_tag_bitlen(msg)), &@(underscored_name(msg))_type);
@(ind)*bit_ofs += @(union_msg_tag_bitlen(msg));

@(ind)switch(msg->@(underscored_name(msg))_type) {
@{indent += 1}@{ind = '    '*indent}@
@[  end if]@
@[    for field in msg.fields]@
@[      if msg.union]@
@(ind)case @(field_union_type_enum_name(msg, field)): {
@{indent += 1}@{ind = '    '*indent}@
@[      end if]@
@[      if field.type.category == field.type.CATEGORY_COMPOUND]@
@(ind)_encode_@(underscored_name(field.type))(buffer, bit_ofs, &msg->@(field.name), @('tao' if field == msg.fields[-1] else 'false'));
@[      elif field.type.category == field.type.CATEGORY_PRIMITIVE]@
@(ind)canardEncodeScalar(buffer, *bit_ofs, @(field.type.bitlen), &msg->@(field.name));
@(ind)*bit_ofs += @(field.type.bitlen);
@[      elif field.type.category == field.type.CATEGORY_ARRAY]@
@[        if field.type.mode == field.type.MODE_DYNAMIC]@
@[          if field == msg.fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
@(ind)if (!tao) {
@{indent += 1}@{ind = '    '*indent}@
@[          end if]@
@(ind)canardEncodeScalar(buffer, *bit_ofs, @(array_len_field_bitlen(field.type)), &msg->@(field.name)_len);
@(ind)*bit_ofs += @(array_len_field_bitlen(field.type));
@[          if field == msg.fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
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
@(ind)    _encode_@(underscored_name(field.type.value_type))(buffer, bit_ofs, &msg->@(field.name)[i], @[if field == msg.fields[-1] and field.type.value_type.get_min_bitlen() < 8]tao && i==msg->@(field.name)_len@[else]false@[end if]@);
@[        end if]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      elif field.type.category == field.type.CATEGORY_VOID]@
@(ind)*bit_ofs += @(field.type.bitlen);
@[      end if]@
@[      if msg.union]@
@(ind)break;
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      end if]@
@[    end for]@
@[  if msg.union]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[  end if]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}

void _decode_@(underscored_name(msg))(const CanardRxTransfer* transfer, uint32_t* bit_ofs, @(uavcan_type_to_ctype(msg))* msg, bool tao) {
@{indent += 1}@{ind = '    '*indent}@
@(ind)(void)tao;

@[  if msg.union]@
@(ind)@(union_msg_tag_ctype(msg)) @(underscored_name(msg))_type;
@(ind)canardDecodeScalar(transfer, *bit_ofs, @(union_msg_tag_bitlen(msg)), false, &@(underscored_name(msg))_type);
@(ind)msg->@(underscored_name(msg))_type = @(underscored_name(msg))_type;
@(ind)*bit_ofs += @(union_msg_tag_bitlen(msg));

@(ind)switch(msg->@(underscored_name(msg))_type) {
@{indent += 1}@{ind = '    '*indent}@
@[  end if]@
@[    for field in msg.fields]@
@[      if msg.union]@
@(ind)case @(field_union_type_enum_name(msg, field)): {
@{indent += 1}@{ind = '    '*indent}@
@[      end if]@
@[      if field.type.category == field.type.CATEGORY_COMPOUND]@
@(ind)_decode_@(underscored_name(field.type))(buffer, bit_ofs, &msg->@(field.name), @('tao' if field == msg.fields[-1] else 'false'));
@[      elif field.type.category == field.type.CATEGORY_PRIMITIVE]@
@(ind)canardDecodeScalar(transfer, *bit_ofs, @(field.type.bitlen), @('true' if uavcan_type_is_signed(field.type) else 'false') &msg->@(field.name));
@(ind)*bit_ofs += @(field.type.bitlen);
@[      elif field.type.category == field.type.CATEGORY_ARRAY]@
@[        if field.type.mode == field.type.MODE_DYNAMIC]@
@[          if field == msg.fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
@(ind)if (!tao) {
@{indent += 1}@{ind = '    '*indent}@
@[          end if]@
@(ind)canardDecodeScalar(transfer, *bit_ofs, @(array_len_field_bitlen(field.type)), false &msg->@(field.name)_len);
@(ind)*bit_ofs += @(array_len_field_bitlen(field.type));
@[          if field == msg.fields[-1] and field.type.value_type.get_min_bitlen() >= 8]@
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
@(ind)    _decode_@(underscored_name(field.type.value_type))(buffer, bit_ofs, &msg->@(field.name)[i], @[if field == msg.fields[-1] and field.type.value_type.get_min_bitlen() < 8]tao && i==msg->@(field.name)_len@[else]false@[end if]@);
@[        end if]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      elif field.type.category == field.type.CATEGORY_VOID]@
@(ind)*bit_ofs += @(field.type.bitlen);
@[      end if]@
@[      if msg.union]@
@(ind)break;
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[      end if]@
@[    end for]@
@[  if msg.union]@
@{indent -= 1}@{ind = '    '*indent}@
@(ind)}
@[  end if]@
}

@[end if]@
