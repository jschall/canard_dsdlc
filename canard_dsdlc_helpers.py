import os
import uavcan
import errno
import em

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
