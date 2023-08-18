import os
import json
from desc_parser import DescParser
import codegen 

path = "ant-device/codegen/test.json"
print("Code generator v0.0.1")

with open(path) as file_desc:
    desc = json.load(file_desc)
    a = DescParser(desc)



    codegen.generate_api_class("./ant-device/codegen/out/", a)

    # print(a.generate_fields_t())
    # print(a.generate_accessors_prototypes())
    # print(a.generate_accessors_weak_definitions())

