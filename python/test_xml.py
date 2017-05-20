
import xml.dom.minidom

def read_xml(file_name):
    print("file_name=%s" % (file_name))

    dom = xml.dom.minidom.parse(file_name)
    root = dom.documentElement

    root_name = root.nodeName
    print("root_name=%s\n" % (root_name))

    maxid_node = root.getElementsByTagName("maxid")[0]
    maxid_value = maxid_node.childNodes[0].data
    print("maxid_value=%s\n" % (maxid_value))

    login_node = root.getElementsByTagName("login")[0]
    if login_node.hasAttribute("username") and login_node.hasAttribute("passwd"):
        username = login_node.getAttribute("username")
        passwd = login_node.getAttribute("passwd")
        print("username=%s passwd=%s\n" % (username, passwd))

    caption_node = login_node.getElementsByTagName("caption")[0]
    caption_value = caption_node.childNodes[0].data
    print("caption_value=%s\n" % (caption_value))

    def print_item_node(item_node):
        if item_node.hasAttribute("id"):
            id = item_node.getAttribute("id")
            print("id=%s" % (id))
        data_node = item_node.getElementsByTagName("data")[0]
        data_value = data_node.childNodes[0].data
        print("data_value=%s" % (data_value))
        
        
    print("login.item:")
    item_nodes = login_node.getElementsByTagName("item")
    for item_node in item_nodes:
        print_item_node(item_node)
    print("\n")

    # to get all root.item, way 1
    # will loop all "item" tag, include children node
    print("root.item: 1")
    item_nodes = root.getElementsByTagName("item")
    for item_node in item_nodes:
        # skip node in children node
        if root.isSameNode(item_node.parentNode) == False:
            continue
        print_item_node(item_node)
    print("\n")

    # to get all root.item, way 2
    print("root.item: 2")
    all_nodes = root.childNodes
    for node in all_nodes:
        if node.nodeName != "item":
            continue
        print_item_node(node)


read_xml("test_xml.xml")
