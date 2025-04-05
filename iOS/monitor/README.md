## iOS energy monitor

### installation 
```bash
pip install -r requirements.txt
```

### Connection and Trust
First, open `Developer Mode` (restart needed). (iOS 18 may close and reopen this mode if energy data isn't accessible) 

Then, connect the iPhone to your mac with an USB. Trust your computer if such prompt pops out on your phone.

Enter the follwing command to check the device accessibility.

```bash
pymobiledevice3 bonjour rsd
```

### wifi remote connection
To monitor the energy consumption without usb plugin, wifi remote connection shall be established. This can be accomplished by executing:

```bash
python3 -m pymobiledevice3 lockdown wifi-connections on
```

After the remote connection is established, the usb connection is no longer needed, and thus the cable shall be unplugged.

### Experiment excution

Start the monitor before your on-device LLM execution, start in advance for about 20 sec:
```bash
python monitor.py -m
```

Execute, and record the start and end time after it finishes.

interrupt the `monitor.py` execution, after about 20sec.

Reexecute it with different args (pass the integer part of start and end):

```bash
python monitor.py -s <start_time> -e <end_time>
```

The result json print out capacity consumption (mAh), energy (mJ), and capacity percentage (%).