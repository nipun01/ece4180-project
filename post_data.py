import json
import requests

with open("left_map.out", "r") as left_file, open("right_map.out", "r") as right_file:
    left_line = left_file.readline()
    left_line.strip()
    left_line = left_line[5:]
    left_line = left_line[:-4]
    left_array = []
    for char in left_line:
        left_array.append(char)
    data_dict = {"data":left_array}
    data = json.dumps(data_dict)
    
    r = requests.post("http://192.168.137.1:8080", data=data)

    right_line = right_file.readline()
    right_line.strip()
    right_line = right_line[5:]
    right_line = right_line[:-4]
    right_array = []
    for char in right_line:
        right_array.append(char)
    data_dict = {"data":right_array}
    data = json.dumps(data_dict)
    
    r = requests.post("http://192.168.137.1:8080", data=data)
    