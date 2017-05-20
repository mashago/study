
import os
import sys
import codecs
import shutil

import xml.dom.minidom
import xlrd # excel read lib
import MySQLdb # mysql lib

# set coding as utf8
reload(sys) 
sys.setdefaultencoding('utf8')

# 是否定义了格式
def isSpecifiedForamt(value):
    if value != u"i" and value != u"f" and value != u"s" and value != u"{i}" and value != u"{f}" and value != u"{s}":
        return False
    return True

def isSpecifiedFieldName(value):
    if value == None:
        return False
    return True

def toString(v):
    if v == None:
        return None
    return (u"%s" % v)

def hadleInt(v):
    if v == None or v == "":
        return 0
    try:
        v = int(float(v))
    except Exception, e:
        return None
    return str(v)

def hadleFloat(v):
    if v == None or v == "":
        return 0
    try:
        v = float(v)
    except Exception, e:
        return None
    return v

def hadleString(v):
    if v == None or v == "":
        return "\"\""
    v = toString(v)
    v = v.replace("\"", "\\\"")
    return (u"\"%s\"" % v)

def hadleIntArray(v):
    if v == None or v == "":
        return "{}"
    v = str(v)
    arr = v.split(",")
    mstr = ""
    for x in arr:
        if hadleInt(x) == None:
            return None
        else:
            mstr += ("%s," % hadleInt(x))
    return "{%s}" % mstr[0: -1]

def hadleFloatArray(v):
    if v == None or v == "":
        return "{}"
    v = str(v)
    arr = v.split(",")
    mstr = ""
    for x in arr:
        if hadleFloat(x) == None:
            return None
        else:
            mstr += ("%s," % hadleFloat(x))
    return "{%s}" % mstr[0: -1]

def hadleStringArray(v):
    if v == None or v == "":
        return "{}"
    v = toString(v)
    arr = v.split(",")
    mstr = ""
    for x in arr:
        if hadleString(x) == None:
            return None
        else:
            mstr += ("%s," % hadleString(x))
    return "{%s}" % mstr[0: -1]

def hadle(v, type):
    if type == "i":
        v = hadleInt(v)
        if v != None:
            return True, v
    elif type == "f":
        v = hadleFloat(v)
        if v != None:
            return True, v
    elif type == "s":
        v = hadleString(v)
        if v != None:
            return True, v
    elif type == "{i}":
        v = hadleIntArray(v)
        if v != None:
            return True, v
    elif type == "{f}":
        v = hadleFloatArray(v)
        if v != None:
            return True, v
    elif type == "{s}":
        v = hadleStringArray(v)
        if v != None:
            return True, v	

    return False, None


# 解析excel
def parseExcel(filenane, savepath): #{

    # 1. load db config from config.xml
    # 2. connect to mysql
    # 3. load from excel, create sql
    # 4. do insert sql

    # 1. load db config from config.xml
    dom = xml.dom.minidom.parse('config.xml')
    root = dom.documentElement
    mysqlCfgList = root.getElementsByTagName('mysql')
    mysqlCfg = mysqlCfgList[0]
    db_host = mysqlCfg.getAttribute("ip")
    db_user = mysqlCfg.getAttribute("user_name")
    db_pwd = mysqlCfg.getAttribute("password")
    db_name = mysqlCfg.getAttribute("db_name")
    print ("db_host=%s db_user=%s db_pwd=%s db_name=%s" % (db_host, db_user, db_pwd, db_name))


    # 2. connect to mysql
    db = MySQLdb.connect(db_host, db_user, db_pwd, db_name)
    cursor = db.cursor()


    # 3. load from excel, create sql
    # 打开excel
    excel = xlrd.open_workbook(filenane)
    # 获取sheets
    sheets = excel.sheets()
    # 遍历所有的sheet
    for sheet in sheets: #{
        print "nrows=", sheet.nrows, " cols=", sheet.ncols

        # 判断行数是否满足要求，如果小于5行自判定为没有任何数据
        if sheet.nrows < 5:
            continue

        # make key string
        keyStr = "("
        for c in xrange(0, sheet.ncols):
            if c == 0:
                # ignore first cols
                continue
            if c != 1:
                keyStr += ","
            keyStr += ("%s" % (sheet.cell_value(4, c)))
        keyStr += ")"
        # print "keyStr=", keyStr

        # make value string
        valList = []
        # 读取第六行的数据
        for r in xrange(5, sheet.nrows): #{
            valStr = "("
            for c in xrange(0, sheet.ncols): #{
                if c == 0:
                    # ignore first cols
                    continue
                
                if c != 1:
                    valStr += ","
                
                ret = hadle(sheet.cell_value(r, c), sheet.cell_value(0, c))
                valStr += ("%s" % (ret[1]))
            #}

            valStr += ")"
            # print valStr
            valList.append(valStr)
        #}

        # make sql
        sql = "INSERT INTO redeem_code " + keyStr + " VALUES "
        for i, j in enumerate(valList):
            if i != 0:
                sql += ","
            sql += j

        sql += ";"
        print "sql=", sql


        # 4. do insert sql
        try:
            cursor.execute(sql)
            db.commit()
        except MySQLdb.Error,e:
            print "Mysql Error %d: %s" % (e.args[0], e.args[1])


    #}

#}

#解析当前目录下或是自定目录下的xls或xlsx文件
#当前pyhton文件的路径
curFilePath = os.path.split(os.path.realpath(__file__))[0]
#xls或xlsx所在的目录
excelPath = curFilePath
#是否发生错误
error = False

#指定的目录
if len(sys.argv) >= 2: 
    if os.path.isdir(sys.argv[1]):
        excelPath = os.path.realpath(sys.argv[1])
    else:
        error = True
        print u'指定的位置不是一个正确的目录'

if not error:
    print u"存储位置\n", excelPath
    #遍历所有的xls或xlsx文件
    lists = os.listdir(excelPath)
    for x in lists:
        excelFile = os.path.join(excelPath, x)
        if os.path.isfile(excelFile) and (excelFile.endswith('xls') or excelFile.endswith('xlsx')) and not x.startswith("~$"):
            parseExcel(excelFile, excelPath)

    raw_input("press enter to exit\n");
