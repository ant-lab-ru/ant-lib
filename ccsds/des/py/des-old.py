import os
import json

requestLen = 4
requestTypeSize = 1
requestTypeBroadcast = 0x01
requestTypeServicesList = 0x02

class DesClient:
    def __init__(self, desFilePath : str):
        self.file = desFilePath

    def generateBroadcastRequest(self):
        req = bytearray(requestLen)
        req[0] = 'D'
        req[1] = 'E'
        req[2] = 'S'
        req[3] = requestTypeBroadcast
        return req
    
    def generateServicesListRequest(self):
        req = bytearray(requestLen)
        req[0] = 'D'
        req[1] = 'E'
        req[2] = 'S'
        req[3] = requestTypeServicesList
        return req
    
    def parseServicesListResponse(self, device : str, data : bytes):
        if len(data) < requestTypeSize:
            raise ValueError(f'Response len: {len(data)} less than {requestTypeSize} byte')
        
        if (data[0] != requestTypeServicesList):
            raise ValueError(f'First byte invalid. Received: {data[0]}, but expected: {requestTypeServicesList}')
        
        data = data[1:]
        idx = 0

        servicesList = []

        while(len(data) > 0):
            if data[0] != idx:
                raise ValueError(f'Service idx invalid. Received: {data[0]}, but expected: {idx}')
            data=data[1:]
            idx+=1
            name = (data.split(bytes([0]))[0]).decode('ascii')
            data = data[len(name) + 1:]
            version = (data.split(bytes([0]))[0]).decode('ascii')
            data = data[len(version) + 1:]
            servicesList.append({"name" : name, "version" : version})

        with open(self.file, "r") as f:
            desJson = f.read()

        desDict = json.loads(desJson)
        des = desDict["devices"]
        for d in des:
            if d["device"] == device:
                d["services"] = servicesList
                break

        desDict["devices"] = des

        desJson = json.dumps(desDict, indent=2)

        with open(self.file, "w") as f:
            f.write(desJson)

        return des


    def parseBroadcastResponse(self, data : bytes, addr : str):
        if len(data) < 3:
            raise ValueError(f'Response len: {len(data)} less than 3 bytes')
        
        if (data[0] != requestTypeBroadcast):
            raise ValueError(f'First byte invalid. Received: {data[0]}, but expected: {requestTypeBroadcast}')
        
        if (data[-1] != 0):
            raise ValueError(f'First byte invalid. Received: {data[-1]}, but expected: {0}')
        
        with open(self.file, "w+") as f:
            desJson = f.read()

        if desJson == '': desJson = "{}"

        desDict = json.loads(desJson)
        if "devices" not in desDict:
            desDict = {"devices" : []}

        des = desDict["devices"]
        
        device = data[1:-1].decode('ascii') # name = "myDevice"

        for x in des:
            if "device" not in x:
                raise ValueError(f'No key "device"')
    
            if x["device"] == device:
                continue

        newDevice = {"device" : device, "addr" : addr, "services" : []}
        des.append(newDevice)
        desDict["devices"] = des

        desJson = json.dumps(desDict, indent=2)

        with open(self.file, "w") as f:
            f.write(desJson)

        return device

