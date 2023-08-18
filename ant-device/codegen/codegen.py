import os
import re
from desc_parser import DescParser

def generate_api_fields(offset, parser):
    ret_string = ''
    for field in parser.fields:
        ret_string += ' '*offset + field.get('type') + ' ' + field.get('name') + ';\n'
    return ret_string

def generate_api_write_callbacks(offset, parser):
    ret_string = ''
    ret_string += '\n' + ' '*offset + '// Write callbacks\n'
    for field in parser.fields:
        upper_name = field.get('name')[0].upper() + field.get('name')[1:]
        ret_string += ' '*offset + 'int onWrite' + upper_name + '();\n'
    return ret_string

def generate_api_class(folder, parser):
    snake_name = re.sub(r'(?<!^)(?=[A-Z])', '-', parser.name).lower()
    h_file_name = snake_name + '-api.h'
    path = folder + h_file_name

    with open(path, 'w') as fid:
        fid.write('#pragma once\n\n#include <stdint.h>\n#include <stdbool.h>\n\n')
        fid.write('class '+parser.name+"Api\n{\n    public:\n")

        fid.write(generate_api_fields(8, parser))
        fid.write(generate_api_write_callbacks(8, parser))

        fid.write('};\n')
        fid.close()

    print('Generate done '+path)





