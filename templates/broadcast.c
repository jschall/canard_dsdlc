@{
if msg.kind == msg.KIND_MESSAGE:
    empy.include('templates/msg.c', {'msg':msg})
}@
