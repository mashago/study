
print("******* show dbs")
db.getMongo().getDBs()

print("******* use test")
db.getSisterDB("test")

print("******* show collections")
db.getCollectionNames()

var role = {"id" : 1, "name" : "masha", "age" : 20}

print("******* insert")
db.role_info.insert(role)

print("******* find")
db.role_info.find()
// db.role.find().forEach(printjson)

print("******* findOne")
db.role_info.findOne()

print("******* update")
role.age = 25
db.role_info.update({"id" : 1}, role)

print("******* findOne")
db.role_info.findOne({"id" : 1})

print("******* remove")
db.role_info.remove({"id" : 1})
