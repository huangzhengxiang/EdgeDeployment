import os
import json
import subprocess
import argparse
import time

raw_file = "raw.json"
result_file = "result.json"

def getOnce() -> dict:
    needed = {}
    info = subprocess.check_output(["pymobiledevice3", "diagnostics", "battery", "single"])
    info = json.JSONDecoder().decode(info.decode())
    needed["UpdateTime"] = info["UpdateTime"]
    needed["Voltage"] = info["Voltage"]
    needed["AppleRawCurrentCapacity"] = info["AppleRawCurrentCapacity"]
    needed["AppleRawMaxCapacity"] = info["AppleRawMaxCapacity"]
    needed["Percentage"] = needed["AppleRawCurrentCapacity"] / needed["AppleRawMaxCapacity"]
    return needed

def monitor(path: str, interval: int = 5):
    records = []
    try:
        while True:
            records.append(getOnce())
            time.sleep(interval)
    except KeyboardInterrupt:
        with open(path, "wt") as f:
            json.dump(records, f, indent=4)
    
def get_results(input_path: str, output_path: str, start: int, end: int):
    assert start < end
    res = {} # capacity consumption, energy consumption(capacity*voltage)
    with open(input_path, "rt") as f:
        data = json.load(f)
    # process
    startp = 0
    endp = 0x7f7f7f7f
    start_item = None
    end_item = None
    for item in data:
        if (item["UpdateTime"] <= start) and (item["UpdateTime"] > startp):
            startp = item["UpdateTime"]
            start_item = item
        if (item["UpdateTime"] >= end) and (item["UpdateTime"] < endp):
            endp = item["UpdateTime"]
            end_item = item 
    assert (start_item is not None) and (end_item is not None), "required time interval data not found~"
    res["capacity"] = start_item["AppleRawCurrentCapacity"] - end_item["AppleRawCurrentCapacity"]
    res["energy"] = start_item["AppleRawCurrentCapacity"]*start_item["Voltage"]/1000 - end_item["AppleRawCurrentCapacity"]*end_item["Voltage"]/1000
    res["capacity_percentage"] = res["capacity"]/end_item["AppleRawMaxCapacity"]
    with open(output_path, "wt") as f:
        json.dump(res, f, indent=4)
    

if __name__=="__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", action="store_true", default=False)
    parser.add_argument("-s", type=int, default=0)
    parser.add_argument("-e", type=int, default=0x7f7f7f7f)
    args = parser.parse_args()
    if (args.m):
        monitor(os.path.join("data", raw_file))
    else:
        get_results(os.path.join("data", raw_file), os.path.join("data", result_file), args.s, args.e)