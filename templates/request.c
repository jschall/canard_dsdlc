@{
from copy import copy
if msg.kind == msg.KIND_SERVICE:
    msg = copy(msg)
    msg.full_name = '%s_Request' % (msg.full_name,)
    msg.fields = msg.request_fields
    msg.constants = msg.request_constants
    msg.get_max_bitlen = msg.get_max_bitlen_request
    msg.union = msg.request_union
    msg.kind = msg.KIND_MESSAGE
    empy.include('templates/msg.c', {'msg':msg})
}@
