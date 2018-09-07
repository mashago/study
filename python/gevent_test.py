
import random
import gevent
from gevent.queue import Queue

MAX_PLAYER = 100
MAX_TASK = 10
g_player_list = []

class Task:
    def __init__(self, req_queue, rsp_queue):
        self.req_queue = req_queue
        self.rsp_queue = rsp_queue
    def work_loop(self):
        while True:
            need = random.randint(1, MAX_PLAYER)
            print "need=%s" % need
            self.req_queue.put([need, self.rsp_queue], block=True)
            player_list = self.rsp_queue.get(block=True)
            print "len(player_list)=%s len(g_player_list)=%s" % (len(player_list), len(g_player_list))
            gevent.sleep(1) # TODO should be real client logic
            self.req_queue.put([0, player_list], block = True)


def start(req_queue, rsp_queue):
    m = Task(req_queue, rsp_queue)
    m.work_loop()


def main():
    for n in range(MAX_PLAYER):
        g_player_list.append(n)

    req_queue = Queue(maxsize=1)
    for n in range(MAX_TASK):
        rsp_queue = Queue(maxsize=1)
        gevent.spawn(start, req_queue, rsp_queue)

    def send_rsp(item):
        need = item[0]
        queue = item[1]

        total = len(g_player_list)
        player_list = []
        while need > 0:
            player_list.append(g_player_list[total-1])
            del g_player_list[total-1]
            total = total - 1
            need = need - 1
        queue.put(player_list) 
        

    wait_list = []
    while True:
        item = req_queue.get(block=True)
        if not item[0] == 0:
            if len(wait_list) == 0 and len(g_player_list) >= item[0]:
                send_rsp(item)
            else:
                wait_list.append(item)
        else:
            player_list = item[1]
            for player in player_list:
                g_player_list.append(player)

            # debug print
            msg = "wait_list:"
            for item in wait_list:
                msg = "%s %s" % (msg, item[0])
            print msg

            while len(wait_list) > 0:
                item = wait_list[0]
                if len(g_player_list) < item[0]:
                    break
                send_rsp(item)
                del wait_list[0]

main()
