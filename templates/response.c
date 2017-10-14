@{
from copy import copy
if msg.kind == msg.KIND_SERVICE:
    msg = copy(msg)
    msg.full_name = '%s_Response' % (msg.full_name,)
    msg.fields = msg.response_fields
    msg.constants = msg.response_constants
    msg.get_max_bitlen = msg.get_max_bitlen_response
    msg.union = msg.response_union
    msg.kind = msg.KIND_MESSAGE
    empy.include('templates/msg.c', {'msg':msg})
}@
