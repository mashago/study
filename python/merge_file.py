import os
import shutil
import hashlib

def check_same_file_fast(f1, f2):
    if f1["fsize"] != f2["fsize"]:
        return False

    bufsize = 4*1024
    with open(f1["fpath"], 'rb') as fp1, open(f2["fpath"], 'rb') as fp2:
        while True:
            b1 = fp1.read(bufsize)
            b2 = fp2.read(bufsize)
            if b1 != b2:
                return False
            if not b1:
                return False

    return True

def check_same_file_list(file_list):
    ret = []
    if len(file_list) == 2:
        is_same = check_same_file_fast(file_list[0], file_list[1])
        if not is_same:
            return ret
        group = []
        for f in file_list:
            group.append(f["fpath"])
        ret.append(group)
        return ret

    # use md5
    md5_dict = {}
    for f in file_list:
        with open(f["fpath"], 'rb') as fp:
            data = fp.read()
        f_md5 = hashlib.md5(data).hexdigest()
        # print(f_md5)
        if not md5_dict.__contains__(f_md5):
            md5_dict[f_md5] = [f["fpath"]]
        else:
            md5_dict[f_md5].append(f["fpath"])

    for _, v in md5_dict.items():
        if len(v) > 1:
            group = []
            for fpath in v:
                group.append(fpath)
            ret.append(group)

    return ret

def main():
    print("hello merge file")

    path_list = ["../"]

    file_list = []
    for path in path_list:
        for root, dirs, files in os.walk(path):
            for name in files:
                fpath = os.path.join(root, name)
                fsize = os.stat(fpath).st_size
                file_list.append({
                    "fpath": fpath,
                    "fsize": fsize,
                })
    print("total file: %d" % len(file_list))

    def sort_key(elem):
        return elem["fsize"]
    file_list.sort(key=sort_key)

    same_size_file_dict = {}
    last_f = None
    for f in file_list:
        if not last_f or last_f["fsize"] != f["fsize"]:
            last_f = f
            continue
        if same_size_file_dict.__contains__(f["fsize"]):
            same_size_file_dict[f["fsize"]].append(f)
        else:
            same_size_file_dict[f["fsize"]] = [last_f, f]

    for fsize, f_list in same_size_file_dict.items():
        same_file_group_list = check_same_file_list(f_list)
        if not same_file_group_list:
            continue
        print(fsize)
        for group in same_file_group_list:
            print(group)


if __name__ == "__main__":
    main()
