import uavcan.dsdl
import argparse
import os
import em
from canard_dsdlc_helpers import *

templates = [
    {'source_file': 'request.h',
     'output_file': 'include/@(msg_header_name_request(msg))'},
    {'source_file': 'response.h',
     'output_file': 'include/@(msg_header_name_response(msg))'},
    {'source_file': 'broadcast.h',
     'output_file': 'include/@(msg_header_name(msg))'},
    {'source_file': 'service.h',
     'output_file': 'include/@(msg_header_name(msg))'},
    {'source_file': 'request.c',
     'output_file': 'src/@(msg_c_file_name_request(msg))'},
    {'source_file': 'response.c',
     'output_file': 'src/@(msg_c_file_name_response(msg))'},
    {'source_file': 'broadcast.c',
     'output_file': 'src/@(msg_c_file_name(msg))'},
]

parser = argparse.ArgumentParser()
parser.add_argument('namespace_dir', nargs='+')
parser.add_argument('build_dir', nargs=1)
args = parser.parse_args()

namespace_paths = args.namespace_dir
build_dir = args.build_dir[0]
templates_dir = os.path.join(os.path.dirname(__file__), 'templates')

if os.path.exists(build_dir):
    assert os.path.isdir(build_dir) and not os.listdir(build_dir), "\"%s\" exists and is not an empty directory" % (build_dir,)

messages = uavcan.dsdl.parse_namespaces(namespace_paths)

for template in templates:
    with open(os.path.join(templates_dir, template['source_file']), 'rb') as f:
        template['source'] = f.read()

for msg in messages:
    print(msg.full_name)
    for template in templates:
        output = em.expand(template['source'], msg=msg)

        if not output.strip():
            continue

        output_file = os.path.join(build_dir, em.expand('@{from canard_dsdlc_helpers import *}'+template['output_file'], msg=msg))
        mkdir_p(os.path.dirname(output_file))
        with open(output_file, 'wb') as f:
            f.write(output)
