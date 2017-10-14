@{
if msg.kind == msg.KIND_MESSAGE:
    empy.include('templates/msg.h', {'msg':msg})
}@
