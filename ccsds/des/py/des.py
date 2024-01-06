import os
import json

requestLen = 4
requestTypeSize = 1
requestTypeBroadcast = 0x01
requestTypeServicesList = 0x02

class DesClient:
    def __init__(self, desFilePath : str):
        self.file = desFilePath
        self.addresses = []
        self.des = self.__getDes()

    def __getDes(self):
        with open(self.file, "a+") as f:
            f.seek(0)
            desJson = f.read()

        try:
            desDict = json.loads(desJson)
        except Exception as msg:
            print(msg)
            return []

        if not "devices" in desDict:
            return []
        else:
            return desDict["devices"]
        
    def __setDes(self, des):
        self.des = des
        desDict = {}
        desDict["devices"] = des
        desJson = json.dumps(desDict, indent=2)
        with open(self.file, "w") as f:
            f.write(desJson)
        return 

    def getDeviceAddress(self, device : str):
        for d in self.des:
            if (d["device"] == device):
                return d["addr"]
        return ''
    
    def getDeviceList(self):
        device_list = []
        for d in self.des:
            device_list.append(d["device"])
        return device_list
    
    def getServicePort(self, device : str, service: str):
        for d in self.des:
            if (d["device"] == device):
                services = d["services"]
                for s in services:
                    if s["name"] == service:
                        return s["port"]
        return ''
    
    def clearDevice(self, device : str):
        des = self.des
        for d in des:
            if (d["device"] == device):
                d["services"] = []
        self.__setDes(des)
        return self.des 
    
    def setDeviceAddr(self, device_name : str, addr : str):
        des = self.des
        for d in des:
            if (d["device"] == device_name):
                d["addr"] = addr
        self.__setDes(des)
        return self.des

    def getDescription(self):
        return self.des
    
    def clearDescription(self):
        self.__setDes([])
        return self.des

    def syncDes(self):
        self.des = self.__getDes()
        return self.des

    def devicesByService(self, service_name : str):
        device_list = []
        for d in self.des:
            for s in d["services"]:
                if s["name"] == service_name:
                    device_list.append(d["device"])
                    break
        return device_list

    def generateBroadcastRequest(self):
        return 'DES'.encode('ascii') + requestTypeBroadcast.to_bytes(1, 'little')
    
    def generateServicesListRequest(self):
        return 'DES'.encode('ascii') + requestTypeServicesList.to_bytes(1, 'little')

    def parseServicesListResponse(self, device : str, data : bytes, port: str):
        if len(data) < requestTypeSize:
            raise ValueError(f'Response len: {len(data)} less than {requestTypeSize} byte')
        
        if (data[0] != requestTypeServicesList):
            raise ValueError(f'First byte invalid. Received: {data[0]}, but expected: {requestTypeServicesList}')
        
        offset = requestTypeSize;
        idx = 0

        servicesList = []

        while(offset < len(data)):
            if data[offset] != idx:
                raise ValueError(f'Service idx invalid. Received: {data[offset]}, but expected: {idx}')
            offset+=1
            idx+=1
            name = (data[offset:].split(b'\x00')[0]).decode('ascii')
            offset += len(name) + 1
            version = (data[offset:].split(b'\x00')[0]).decode('ascii')
            offset += len(version) + 1
            servicesList.append({"name" : name, "version" : version, "port" : port})

        des = self.__getDes()
        for d in des:
            if d["device"] == device:
                d["services"] = servicesList
                break

        self.__setDes(des)
        return des


    def parseBroadcastResponse(self, data : bytes, addr : str):
        if len(data) < 3:
            raise ValueError(f'Response len: {len(data)} less than 3 bytes')

        if (data[0] != requestTypeBroadcast):
            raise ValueError(f'First byte invalid. Received: {data[0]}, but expected: {requestTypeBroadcast}')

        if (data[-1] != 0):
            raise ValueError(f'First byte invalid. Received: {data[-1]}, but expected: {0}')
        
        des = self.__getDes()

        device = data[1:-1].decode('ascii') # name = "myDevice"

        for x in des:
            if "device" not in x:
                raise ValueError(f'No key "device"')
    
            if x["device"] == device:
                x["addr"] = addr
                break
        else:
            des.append({"device" : device, "addr" : addr, "services" : []})
        
        self.__setDes(des)

        return device

