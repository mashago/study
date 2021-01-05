import os
import shutil

def check_diff_file(f1, f2):
    if f1["fsize"] != f2["fsize"]:
        return True

    bufsize = 8*1024
    with open(f1, 'rb') as fp1, open(f2, 'rb') as fp2:
        while True:
            b1 = fp1.read(bufsize)
            b2 = fp2.read(bufsize)
            if b1 != b2:
                return False
            if not b1:
                return True

def main():
    print("hello world")

    p1 = "../lua"
    p2 = "../python"
    p3 = "../output"

    file_list = []
    for root, dirs, files in os.walk(p1):
        for name in files:
            print(root)
            fpath = os.path.join(root, name)
            fsize = os.stat(fpath).st_size
            file_list.append({
                "fpath": fpath,
                "fsize": fsize,
            })

    def sort_key(elem):
        return elem["fsize"]
    file_list.sort(key=sort_key)

    print(len(file_list))
    for t in file_list:
        print(t)

if __name__ == "__main__":
    main()
