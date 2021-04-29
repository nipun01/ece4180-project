import json
import requests

with open("left_map1.out", "r") as left_file1:
    left_line = left_file.readline()
    left_line.strip()
    left_line = left_line[5:]
    left_array = []
    for char in left_line:
        left_array.append(char)
    data_dict = {"data":left_array}
    data = json.dumps(data_dict)
    
    r = requests.post("http://192.168.137.1:8080", data=data)

with open("left_map2.out", "r") as left_file2:
    left_line = left_file2.readline()
    left_line.strip()
    left_line = left_line[:-4]
    left_array = []
    for char in left_line:
        left_array.append(char)
    data_dict = {"data":left_array}
    data = json.dumps(data_dict)
    
    r = requests.post("http://192.168.137.1:8080", data=data)

with open("right_map1.out", "r") as right_file1:
    right_line = right_file1.readline()
    right_line.strip()
    right_line = right_line[:-4]
    right_array = []
    for char in right_line:
        right_array.append(char)
    data_dict = {"data":right_array}
    data = json.dumps(data_dict)
    
    r = requests.post("http://192.168.137.1:8080", data=data)

with open("right_map2.out", "r") as right_file2:
    right_line = right_file2.readline()
    right_line.strip()
    right_line = right_line[:-4]
    right_array = []
    for char in right_line:
        right_array.append(char)
    data_dict = {"data":right_array}
    data = json.dumps(data_dict)
    
    r = requests.post("http://192.168.137.1:8080", data=data)
    