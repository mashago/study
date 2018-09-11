
import random
import gevent
from gevent.queue import Queue
from gevent.lock import Semaphore

MAX_PLAYER = 100
MAX_TASK = 50

class Task:
    def __init__(self, hub):
        self.player_list = []
        self.hub = hub
    def work_loop(self):
        while True:
            num = random.randint(1, 3)
            print "num=%s" % num
            player_list = self.pop_player(num)
            print "work_loop len(player_list)=%s" % (len(player_list))
            gevent.sleep(1) # TODO should be real client logic
            self.push_player()

    def pop_player(self, num):
        player_list = self.hub.acquire_player(num)
        self.player_list.extend(player_list)
        return player_list

    def push_player(self):
        self.hub.release_player(self.player_list)
        self.player_list = []


def start_task(hub):
    m = Task(hub)
    m.work_loop()

class Hub:
    def __init__(self, num):
        self.need = 0
        self.block_queue = Queue(maxsize=1)
        self.total_player = []
        for n in range(num):
            self.total_player.append(n)
        self.semaphore = Semaphore()

    def _get_player(self, num):
        if len(self.total_player) < num:
            # block here
            self.need = num
            print "_get_player set need=%s" % self.need
            self.block_queue.get(block=True)

        assert len(self.total_player) >= num, ("_get_player error total_player=%s num=%s" % (len(self.total_player), num))
        size = len(self.total_player)
        player_list = self.total_player[size-num:]
        self.total_player = self.total_player[:size-num]

        return player_list

    def acquire_player(self, num):
        # lock
        self.semaphore.acquire()
        print "acquire_player num=%s" % num
        player_list = self._get_player(num)
        # unlock
        self.semaphore.release()
        return player_list

    def release_player(self, player_list):
        self.total_player.extend(player_list) 
        print "release_player need=%s len(self.total_player)=%s" % (self.need, len(self.total_player))
        if self.need == 0 or self.need > len(self.total_player):
            return
        self.need = 0
        self.block_queue.put(1, block=True)


def main():
    hub = Hub(MAX_PLAYER)

    ge_list = []
    for n in range(MAX_TASK):
        g = gevent.spawn(start_task, hub)
        ge_list.append(g)

    gevent.joinall(ge_list, 30)

main()
