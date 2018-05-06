import socket
import json
import time

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect(("localhost", 9090))

# advmsg = {
#     "op":"advertise",
#     "topic":"/talker",
#     "type":"std_msgs/String"
# }
# advmsg = json.dumps(advmsg)

# data = {
#     "data" : "hello"
# }

advmsg = {
    "op":"advertise",
    "topic":"/talker3",
    "type":"geometry_msgs/PoseStamped"
}
advmsg = json.dumps(advmsg)

pose = {
    "position" : { "x":1, "y":2, "z":3 },
    "orientation" : { "x":1, "y":2, "z":3, "w":4 }
}

data = {
    "pose" : pose
}



pubmsg = {
    "op":"publish",
    "topic":"/talker3",
    "msg":data
}
pubmsg = json.dumps(pubmsg)


s.send(advmsg)
time.sleep(1)
for i in range(2):
    s.send(pubmsg)



s.close()