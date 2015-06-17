#!/bin/python3
## @package generate_data
# by Alex Timmermann and Frederik Will
# script for generating random passengers
# parameter: int passengers to be generated, default 10
# disclaimer: may not fullfill real world probabilities

import json
import sys

from pprint import pprint
from random import randint
from random import choice


# defining possible passenger attributes
destinations = ["Frankfurt", "London", "Paris", "New York"]
seat_prefs = ["window", "middle", "aisle"]
times = ["morning", "afternoon", "evening"]
seat_classes = ["economy", "buisness", "first"]
current_ID = 0

# number of passengers to generate
# default 10
if len(sys.argv) > 1:
    count = int(sys.argv[1])
else:
    count = 10

data = {}
passengers = []

## generates a passenger group of group_size
# if group_size=1 returns a single passenger
# else returns a group as a list
def generate_passenger(group_size=1):
    class_rand = randint(0, 100)
    if class_rand < 60:
        seat_class = seat_classes[0]
    elif class_rand < 90:
        seat_class = seat_classes[1]
    else:
        seat_class = seat_classes[2]
    if group_size == 1:
        return generate_passenger_with_class(seat_class)
    else:
        group = []
        for i in range(0, group_size):
            group.append(generate_passenger_with_class(seat_class))
        return group

## basically a helper, returns a passenger for a specified seat_class
#
#
def generate_passenger_with_class(seat_class):
    global current_ID
    dest = choice(destinations)
    time = choice(times)
    seat_pref = choice(seat_prefs)
    passenger = {'ID': current_ID, 'destination': dest, 'time': time, 'class': seat_class, 'seat_pref': seat_pref}
    current_ID = current_ID + 1
    return passenger


# generate groups
# following code could be refactored
current_passenger = 0
while current_passenger < count:
    group_rand = randint(0, 100)
    if group_rand < 50:
        passengers.append(generate_passenger())
        current_passenger += 1
    else:
        group_size_rand = randint(0, 99)
        if group_size_rand < 50:
            group_size = 2
        elif group_size_rand < 70:
            group_size = 3
        elif group_size_rand < 85:
            group_size = 4
        elif group_size_rand < 95:
            group_size = 5
        elif group_size_rand < 100:
            group_size = 6
        if group_size > (count - current_passenger):
            group_size = (count - current_passenger)
        passengers.append(generate_passenger(group_size))
        current_passenger += group_size

# generate passengers
#for i in range(count):
#        passengers.append(generate_passenger())

#seat_class = choice(seat_classes)

data["passengers"] = passengers
pprint(data)

with open('passengers.json', 'w') as outfile:
    json.dump(data, outfile, sort_keys=True, indent=4, separators=(',', ': '))
