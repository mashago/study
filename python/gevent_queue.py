
import random
import gevent
from gevent.queue import Queue

MAX_PLAYER = 100
MAX_TASK = 10


class Task:
    def __init__(self, req_queue, rsp_queue):
        self.req_queue = req_queue
        self.rsp_queue = rsp_queue
        self.player_list = []
    def work_loop(self):
        while True:
            num = random.randint(1, MAX_PLAYER)
            print "num=%s" % num
            player_list = self.pop_player(num)
            print "len(player_list)=%s" % (len(player_list))
            gevent.sleep(1) # TODO should be real client logic
            self.push_player()

    def pop_player(self, num):
        self.req_queue.put([num, self.rsp_queue], block=True)
        player_list = self.rsp_queue.get(block=True)
        self.player_list.extend(player_list)
        return player_list

    def push_player(self):
        self.req_queue.put([0, self.player_list], block = True)
        self.player_list = []


def start(req_queue, rsp_queue):
    m = Task(req_queue, rsp_queue)
    m.work_loop()

total_player = []
def main():
    for n in range(MAX_PLAYER):
        total_player.append(n)

    def send_rsp(item):
        global total_player
        need = item[0]
        queue = item[1]
        player_list = total_player[:need]
        total_player = total_player[need:]
        queue.put(player_list) 

    req_queue = Queue(maxsize=1)
    for n in range(MAX_TASK):
        rsp_queue = Queue(maxsize=1)
        gevent.spawn(start, req_queue, rsp_queue)

    wait_list = []
    while True:
        item = req_queue.get(block=True)
        if not item[0] == 0:
            if len(wait_list) == 0 and len(total_player) >= item[0]:
                send_rsp(item)
            else:
                wait_list.append(item)
        else:
            player_list = item[1]
            total_player.extend(player_list)

            # debug print
            msg = "wait_list:"
            for item in wait_list:
                msg = "%s %s" % (msg, item[0])
            print msg

            while len(wait_list) > 0:
                item = wait_list[0]
                if len(total_player) < item[0]:
                    break
                send_rsp(item)
                del wait_list[0]
            print "len(total_player)=%s" % len(total_player)

main()
