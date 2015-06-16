# python3


from random import choice
import json
import sys
from pprint import pprint

destinations = ["Frankfurt", "London", "Paris", "New York"]
seat_pref = ["window", "middle", "aisle"]
times = ["morning", "afternoon", "evening"]
seat_class = ["eco", "buisness", "first"]

if len(sys.argv) > 1:
    count = int(sys.argv[1])
else:
    count = 10
data = {}
#data = json.loads('{"passengers": []}')
passengers = []

for i in range(count):
    dest = choice(destinations)
    time = choice(times)
    passenger = {'ID': i, 'destination': dest, 'time': time}
    passengers.append(passenger)

data["passengers"] = passengers
pprint(data)

with open('data.json', 'w') as outfile:
    json.dump(data, outfile, sort_keys = True, indent = 4, separators=(',', ': '))