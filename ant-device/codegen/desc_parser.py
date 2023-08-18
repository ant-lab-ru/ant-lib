import os
import re

field_modes = ["ro", "wo", "rw"]

base_types = [
    "uint8_t",
    "int8_t",
    "uint16_t",
    "int16_t",
    "uint32_t",
    "int32_t",
    "char",
    "float",
    "double",
    "bool"
]

def check_field_name(name_string):
    if name_string.count('[')==1 and name_string.count(']')==1:
        left = name_string.index('[')
        right = name_string.index(']')
        if left >= right or left==0 or right!=len(name_string)-1:
            return False

        array_size = name_string[left+1:right]
        if not array_size.isdigit():
            return False
    
        name_string = name_string.split('[', 1)[0]

    for i,c in enumerate(name_string):
        if not 'a'<=c<='z' and not 'A'<=c<='Z' and not c=='_' and not '0'<=c<='9':
            return False
        if i == 0 and '0'<=c<='9':
            return False
    return True

class DescParser:

    name = ''
    raw_fields = []
    fields = []
    types = base_types
    modes = field_modes

    def __init__(self, desc):
        in_name = desc.get("name")
        in_fields = desc.get("fields")

        if type(in_name) != str:
            raise ValueError('В описании нет поля "name"')

        if type(in_fields) != list:
            raise ValueError('В описании нет поля "fields"')

        self.raw_fields = in_fields
        self.name = in_name

        in_types = desc.get("types")
        if type(in_types) == list:
            self.types.extend(in_types)

        if not self.process_fields():
            raise ValueError('Ошибки в описании')
    
    def process_fields(self):
        self.fields = []
        for raw_field in self.raw_fields:
            words = raw_field.split()
            field = {}
            if len(words) < 3:
                print('Поле',words,'должно обязательно содержать модификатор доступа, тип и имя')
                return False

            if words[0] not in self.modes:
                print('Модификатора',words[0],'нет в списке допустимых режимов:', self.modes)
                return False
            field['mode'] = words[0]

            if words[1] not in self.types:
                print('Типа',words[1],'нет в списке допустимых типов:', self.types)
                return False
            field['type'] = words[1]

            if not check_field_name(words[2]):
                print('Некорректное имя поля',words[2])
                return False
            field['name'] = words[2].split('[', 1)[0]

            if len(words) == 3:
                self.fields.append(field)
                continue
            
            return False

        return True
